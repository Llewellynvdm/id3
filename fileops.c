#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fileops.h"
#if !defined(__WIN32__)
#    include <unistd.h>
#endif

/*

  (c) 2003, 2005 squell ^ zero functionality!
  see the file 'COPYING' for license conditions

*/

int fcopy(FILE *dest, FILE *src)
{
    char buffer[0x4000];                              /* 16kb buffer */
    size_t r, w;

    do {
        r = fread (buffer, 1, sizeof buffer, src);
        w = fwrite(buffer, 1, r, dest);
        if(w != r) return 0;
    } while(r == sizeof buffer);

    return feof(src);
}

int fpadd(FILE *dest, char c, size_t len)
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

FILE *ftemp(char *templ, const char *mode)
{
    FILE *f;
#if defined(__DJGPP__) || defined(__WIN32__)
    FILE *fc;
    if(mktemp(templ) && (fc = fopen(templ, "wb+"))) {
        if(f = freopen(templ, mode, fc)) return f;
        fclose(fc);
#else
    int fd = mkstemp(templ);
    if(fd >= 0) {
        if(f = fdopen(fd, mode)) return f;
        close(fd);
#endif
        unlink(templ);
    }
    return 0;
}

FILE *opentemp(const char *hint, char **name)            /* free() name! */
{
    static const char template[] = "idXXXXXX";

    char *buf, *dirsep = strrchr(hint, '/');
    size_t prefix = dirsep? dirsep-hint+1 : 0;
    FILE *f;

    if(buf = malloc(prefix + sizeof template)) {
        strncpy(buf, hint, prefix);
        strncpy(buf+prefix, template, sizeof template);
        if(f = ftemp(buf, "wb")) {
            if(name) *name = buf;
            else     free(buf);
            return f;
        }
        free(buf);
    }

    return 0;
}

/* ==================================================== */

int cpfile(const char *srcnam, const char *dstnam)
{
    FILE *src, *dst;
    int result = 0;

    if(src = fopen(srcnam, "rb")) {
        if(dst = fopen(dstnam, "wb")) {
            result = fcopy(dst, src);
            fclose(dst);
        }
        fclose(src);
    }
    return result;
}

