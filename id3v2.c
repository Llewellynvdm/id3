#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "id3v2.h"

/*

  (c) 2000 squell ^ zero functionality!
  see the file 'COPYING' for license conditions

*/

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int  uint;

enum ID3_hdr_flag {
    UNSYNC = 0x80,
    XTND   = 0x40,
    XPER   = 0x20
};

enum ID3_frm_flag1 {
    TAP    = 0x80,
    FAP    = 0x40,
    RO     = 0x20
};

enum ID3_frm_flag2 {
    PACK   = 0x80,
    ENC    = 0x40,
    GRP    = 0x20
};

struct raw_hdr {
    uchar ID   [3];
    uchar ver;
    uchar rev;
    uchar flags;
    uchar size [4];
};

struct raw_frm {
    uchar ID   [4];
    uchar size [4];
    uchar flags[2];
};

/* ==================================================== */

static int copyfile(FILE *dest, FILE *src)
{
    char buffer[0x4000];                              /* 16kb buffer */
    size_t r, w;

    do {
        r = fread (buffer, 1, sizeof buffer, src);
        w = fwrite(buffer, 1, r, dest);
        if(w != r) return 0;
    } while(r == sizeof buffer);

    return 1;
}

static int paddfile(FILE *dest, char c, size_t len)
{
    char buffer[0x4000];                              /* 16kb buffer */
    size_t w;

    memset(buffer, c, sizeof buffer);

    while(len > sizeof buffer) {
        w = fwrite(buffer, sizeof buffer, 1, dest);
        if(w!=1) return 0;
        len -= sizeof buffer;
    }

    w = fwrite(buffer, 1, len, dest);

    return w == len;
}

/* ==================================================== */

 /* en-/de- unsynchronizing */

static uchar *unsync_dec(uchar *buf, ulong size)
{
    uchar *dst, *src;

    dst = src = buf;                          /* in-place de-unsync'ing */
    while(size--)
        if( (*dst++ = *src++) == 0xFF && size > 0 ) {
            size--;
            if( (*dst = *src++) != 0x00 ) dst++;
        }
    return dst;
}

static uchar *unsync_enc(uchar *buf, ulong size)
{
    uchar *dst, *src;

    dst = src = buf;                          /* in-place en-unsync'ing */
    while(size--);

     /* to be 'ritten ;) */

}

/* ==================================================== */

static ulong ul4(uchar n[4])
{
    return (ulong)n[0]<<24 |
           (ulong)n[1]<<16 |
           (ulong)n[2]<< 8 |
           (ulong)n[3]<< 0 ;
}

static void nbo4(uchar h[4], ulong n)
{
    h[0] = (n >> 24);
    h[1] = (n >> 16);
    h[2] = (n >>  8);
    h[3] = (n      );
}

static void setsize(struct raw_hdr *h, ulong size)
{
    h->size[0] = (size >> 21) & 0x7F;
    h->size[1] = (size >> 14) & 0x7F;
    h->size[2] = (size >>  7) & 0x7F;
    h->size[3] = (size      ) & 0x7F;
}

static ulong getsize(struct raw_hdr *h)
{
    return (h->size[0] & 0x7F) << 21
         | (h->size[1] & 0x7F) << 14
         | (h->size[2] & 0x7F) <<  7
         | (h->size[3] & 0x7F);
}

static ulong calcsize(uchar *buf, ulong max)
{
    struct raw_frm *frame;
    ulong size = 0;
    ulong step;

    while(size < max && *buf) {            /* terminates if used properly */
        frame = (struct raw_frm*)buf;
        step  = sizeof(*frame) + ul4(frame->size);
        size += step;
        buf  += step;
    }
    return *buf? 0 : size;
}

/* ==================================================== */

void *ID3_readf(const char *fname, unsigned long *tagsize)
{
    struct raw_hdr rh;
    uchar *buf;
    uchar *dst, *src;
    ulong size;

    FILE *f = fopen(fname, "rb");

    if( !f ) return 0;

    if( fread(&rh, sizeof(struct raw_hdr), 1, f) != 1 )
        goto abort;                                          /* IO error */

    if( memcmp(rh.ID, "ID3", 3) != 0 || rh.ver != 3 )
        goto abort;                                /* not an ID3v2.3 tag */

    size = getsize(&rh);

    buf = malloc(size+4);                          /* over-alloc 4 chars */
    if(!buf)                                         /* ohhhhhhh.. crap. */
        goto abort;

    buf[size] = 0;       /* make sure we have a pseudoframe to terminate */

    if( fread(buf,size,1,f) != 1 )
        goto abort_mem;                            /* IO error part deux */

    if( rh.flags & UNSYNC )
        size = unsync_dec(buf, size) - buf;

    if( rh.flags & XTND ) {                 /* get rid of extended header */
        ulong xsiz = ul4(buf);
        size -= xsiz;
        memmove(&buf[0], &buf[xsiz], size);
    }

    size = calcsize(buf, size);                 /* check semantics of tag */
    if(tagsize) *tagsize = size;

    if(size == 0)                                /* semantic error in tag */
        goto abort;

    fclose(f);
    return buf;

abort_mem:                     /* de-alloc, close file and return failure */
    free(buf);
abort:                                   /* close file and return failure */
    fclose(f);
    return 0;
}

int ID3_writef(const char *fname, void *src)
{
    struct raw_hdr new_h = { "ID3", 3, 0, 0, 0 };
    struct raw_hdr rh;
    struct raw_frm fm;
    ulong size = calcsize(src,ULONG_MAX);

    FILE *f = fopen(fname, "rb+");

    if( !f ) return 0;

    if( fread(&rh, sizeof(struct raw_hdr), 1, f) != 1 )
        goto abort;                                          /* IO error */

    if( memcmp(rh.ID, "ID3", 3) == 0 ) {             /* allready tagged? */
        ulong orig;

        if( rh.ver != 3 )
            goto abort;                          /* only handles ID3v2.3 */

        orig = getsize(&rh);

        if( size <= orig ) {                    /* enough reserved space */
/***        setsize(&new_h, orig);
            rewind(f);
            fwrite(&new_h, sizeof new_h, 1, f);
            fwrite(src, size, 1, f);
            paddfile(f, 0, orig-size);     (commented out for debugging)
            fclose(f);
            return 1;
 ***/   }
        fseek(f, orig, SEEK_CUR);
    } else {
        rewind(f);
    }

    {
        char *tmp   = tmpnam(0);
        FILE *nf    = fopen(tmp, "wb");
        ulong nsize = ((size+sizeof new_h+0x1FF) & ~0x1FF) - sizeof new_h;
        int ok;                                      /* rnd to 512 bytes */

        if(!nf)
            goto abort;

        if(size != 0) {
            setsize(&new_h, nsize);
            ok = fwrite(&new_h, sizeof new_h, 1, nf) == 1
              && fwrite(src, size, 1, nf)            == 1
              && paddfile(nf, 0, nsize-size)
              && copyfile(nf, f);
        } else {
            ok = copyfile(nf, f);               /* remove ID3v2 tag only */
        }
        fclose(f);
        fclose(nf);
        if(ok) {
            rename(tmp, fname);
        } else {
            remove(tmp);                                      /* failure */
            return 0;
        }
    }
    return 1;

abort:                                   /* close file and return failure */
    fclose(f);
    return 0;
}

void ID3_free(void *buf)
{
    free(buf);
}

/* ==================================================== */

void ID3_start(ID3FRAME f, void *buf)
{
    f->ID[4] = 0;
    f->data = buf;
    f->size = 0;
}

int ID3_frame(ID3FRAME f)
{
    struct raw_frm *frame;

    f->data += f->size + sizeof *frame;
    frame = (struct raw_frm*)f->data - 1;

    f->size = ul4(frame->size);                        /* copy essentials */
    memcpy(f->ID, frame->ID, 4);
    f->tag_volit  = frame->flags[0] & TAP;
    f->file_volit = frame->flags[0] & FAP;

    f->readonly   = frame->flags[0] & RO;
    f->packed     = frame->flags[1] & PACK;
    f->encrypted  = frame->flags[1] & ENC;
    f->grouped    = frame->flags[1] & GRP;

    return (isupper(f->ID[0]) || isdigit(f->ID[0]))
        && (isupper(f->ID[1]) || isdigit(f->ID[1]))
        && (isupper(f->ID[2]) || isdigit(f->ID[2]))
        && (isupper(f->ID[3]) || isdigit(f->ID[3]));
}

void *ID3_put(void *dest, const char ID[4], const void *src, size_t len)
{
    struct raw_frm *frame = (struct raw_frm*)dest;
    char *cdest           = (char*)dest + sizeof *frame;

    memcpy(frame->ID, ID, 4);
    nbo4(frame->size, len);
    frame->flags[0] = 0;
    frame->flags[1] = 0;

    memcpy(cdest, src, len);
    cdest[len] = 0;                                        /* suffixing 0 */
    return cdest + len;
}

/*

char bripz[] = "\0blaaaaaaaaaaaaaaaaaat";

int main()
{
    void *d = malloc(100000), *i, *o;
    ID3FRAME f;
    char bla[1000];

    i = ID3_readf("test2.mp3");
    o = d;
    ID3_start(f, i);
    while( ID3_frame(f) ) {
        puts(f->ID);
        strncpy(bla, f->data+1, f->size-1);
        bla[f->size-1] = 0;
        puts(bla);
        if( strcmp(f->ID, "TALB") == 0 ) {
            o = ID3_put(o, "TALB", bripz, sizeof bripz);
        } else {
            o = ID3_put(o,f->ID, f->data, f->size);
        }
    }
    ID3_writef("test2.mp3", d);
}

*/

