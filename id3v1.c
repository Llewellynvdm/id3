/*

  ID3v1.1 genres + Winamp extensions

*/

#include "id3v1.h"

const char *const ID3v1_genre[] = {
/*  1*/ "Blues",
/*  2*/ "Classic Rock",
/*  3*/ "Country",
/*  4*/ "Dance",
/*  5*/ "Disco",
/*  6*/ "Funk",
/*  7*/ "Grunge",
/*  8*/ "Hip-Hop",
/*  9*/ "Jazz",
/* 10*/ "Metal",
/* 11*/ "New Age",
/* 12*/ "Oldies",
/* 13*/ "Other",
/* 14*/ "Pop",
/* 15*/ "R&B",
/* 16*/ "Rap",
/* 17*/ "Reggae",
/* 18*/ "Rock",
/* 19*/ "Techno",
/* 20*/ "Industrial",
/* 21*/ "Alternative",
/* 22*/ "Ska",
/* 23*/ "Death Metal",
/* 24*/ "Pranks",
/* 25*/ "Soundtrack",
/* 26*/ "Euro-Techno",
/* 27*/ "Ambient",
/* 28*/ "Trip-Hop",
/* 29*/ "Vocal",
/* 30*/ "Jazz+Funk",
/* 31*/ "Fusion",
/* 32*/ "Trance",
/* 33*/ "Classical",
/* 34*/ "Instrumental",
/* 35*/ "Acid",
/* 36*/ "House",
/* 37*/ "Game",
/* 38*/ "Sound Clip",
/* 39*/ "Gospel",
/* 40*/ "Noise",
/* 41*/ "Alternative Rock",
/* 42*/ "Bass",
/* 43*/ "Soul",
/* 44*/ "Punk",
/* 45*/ "Space",
/* 46*/ "Meditative",
/* 47*/ "Instrumental Pop",
/* 48*/ "Instrumental Rock",
/* 49*/ "Ethnic",
/* 50*/ "Gothic",
/* 51*/ "Darkwave",
/* 52*/ "Techno-Industrial",
/* 53*/ "Electronic",
/* 54*/ "Pop-Folk",
/* 55*/ "Eurodance",
/* 56*/ "Dream",
/* 57*/ "Southern Rock",
/* 58*/ "Comedy",
/* 59*/ "Cult",
/* 60*/ "Gangsta Rap",
/* 61*/ "Top 40",
/* 62*/ "Christian Rap",
/* 63*/ "Pop/Funk",
/* 64*/ "Jungle",
/* 65*/ "Native American",
/* 66*/ "Cabaret",
/* 67*/ "New Wave",
/* 68*/ "Psychedelic",
/* 69*/ "Rave",
/* 70*/ "Showtunes",
/* 71*/ "Trailer",
/* 72*/ "Lo-Fi",
/* 73*/ "Tribal",
/* 74*/ "Acid Punk",
/* 75*/ "Acid Jazz",
/* 76*/ "Polka",
/* 77*/ "Retro",
/* 78*/ "Musical",
/* 79*/ "Rock & Roll",
/* 80*/ "Hard Rock",
/* 81*/ "Folk",                     /* Winamp extensions */
/* 82*/ "Folk/Rock",
/* 83*/ "National Folk",
/* 84*/ "Swing",
/* 85*/ "Fast-Fusion",
/* 86*/ "Bebob",
/* 87*/ "Latin",
/* 88*/ "Revival",
/* 89*/ "Celtic",
/* 90*/ "Bluegrass",
/* 91*/ "Avantgarde",
/* 92*/ "Gothic Rock",
/* 93*/ "Progressive Rock",
/* 94*/ "Psychedelic Rock",
/* 95*/ "Symphonic Rock",
/* 96*/ "Slow Rock",
/* 97*/ "Big Band",
/* 98*/ "Chorus",
/* 99*/ "Easy Listening",
/*100*/ "Acoustic",
/*101*/ "Humour",
/*102*/ "Speech",
/*103*/ "Chanson",
/*104*/ "Opera",
/*105*/ "Chamber Music",
/*106*/ "Sonata",
/*107*/ "Symphony",
/*108*/ "Booty Bass",
/*109*/ "Primus",
/*110*/ "Porn Groove",
/*111*/ "Satire",
/*112*/ "Slow Jam",                 /* Winamp 1.7 */
/*113*/ "Club",
/*114*/ "Tango",
/*115*/ "Samba",
/*116*/ "Folklore",
/*117*/ "Ballad",                   /* Winamp 1.90 */
/*118*/ "Power Ballad",
/*119*/ "Rhythmic Soul",
/*120*/ "Freestyle",
/*121*/ "Duet",
/*122*/ "Punk Rock",
/*123*/ "Drum Solo",
/*124*/ "A Capella",
/*125*/ "Euro-House",
/*126*/ "Dance Hall",
/*127*/ "Goa",
/*128*/ "Drum & Bass",
/*129*/ "Club-House",
/*130*/ "Hardcore",
/*131*/ "Terror",
/*132*/ "Indie",
/*133*/ "BritPop",
/*134*/ "Negerpunk",
/*135*/ "Polsk Punk",
/*136*/ "Beat",
/*137*/ "Christian Gangsta Rap",
/*138*/ "Heavy Metal",
/*139*/ "Black Metal",
/*140*/ "Crossover",
/*141*/ "Contemporary Christian",
/*142*/ "Christian Rock",
/*143*/ "Merengue",                 /* Winamp 1.91 */
/*144*/ "Salsa",
/*145*/ "Thrash Metal",
/*146*/ "Anime",
/*147*/ "JPop",
/*148*/ "Synthpop",
};

const int ID3v1_numgenres = sizeof ID3v1_genre / sizeof *ID3v1_genre;

