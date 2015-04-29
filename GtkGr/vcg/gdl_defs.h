#ifndef _GDL_DEFS_H_
#define _GDL_DEFS_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef enum Color_t
{
    WHITE        = 0,
    BLUE         = 1,
    RED          = 2,
    GREEN        = 3,
    YELLOW       = 4,
    MAGENTA      = 5,
    CYAN         = 6,
    DARKGREY     = 7,
    DARKBLUE     = 8,
    DARKRED      = 9,
    DARKGREEN    = 10,
    DARKYELLOW   = 11,
    DARKMAGENTA  = 12,
    DARKCYAN     = 13,
    GOLD         = 14,
    LIGHTGREY    = 15,
    LIGHTBLUE    = 16,
    LIGHTRED     = 17,
    LIGHTGREEN   = 18,
    LIGHTYELLOW  = 19,
    LIGHTMAGENTA = 20,
    LIGHTCYAN    = 21,
    LILAC        = 22,
    TURQUOISE    = 23,
    AQUAMARINE   = 24,
    KHAKI        = 25,
    PURPLE       = 26,
    YELLOWGREEN  = 27,
    PINK         = 28,
    ORANGE       = 29,
    ORCHID       = 30,
    BLACK        = 31,
    /* number of colors */
    BASECMAPSIZE = 32,
    CMAPSIZE     = 256
} Color_t;

extern const unsigned char gdl_OrigColormapRed[BASECMAPSIZE];
extern const unsigned char gdl_OrigColormapGreen[BASECMAPSIZE];
extern const unsigned char gdl_OrigColormapBlue[BASECMAPSIZE];

enum Linestyle_t
{
    LS_SOLID     = 0,
    LS_DOTTED    = 1,
    LS_DASHED    = 2,
    LS_UNVISIBLE = 3
};

enum Arrowstyle_t
{
    AS_NONE     = 0,
    AS_SOLID    = 1,
    AS_LINE     = 2,
    AS_NONESPEC = 3
};

enum Nodeshape_t
{
    NS_BOX      = 0,
    NS_RHOMB    = 1,
    NS_ELLIPSE  = 2,
    NS_TRIANGLE = 3
};

enum Textmode_t
{
    TM_CENTER = 0,
    TM_LEFT   = 1,
    TM_RIGHT  = 2
};



#define COLOR(c)    (c)

/* Linestyles */
#define SOLID      LS_SOLID
#define DOTTED     LS_DOTTED
#define DASHED     LS_DASHED
#define UNVISIBLE  LS_UNVISIBLE

/* Arrowstyles */
#define ASNONE     AS_NONE
#define ASSOLID    AS_SOLID
#define ASLINE     AS_LINE
#define ASNONESPEC AS_NONESPEC

/* Shapes */
#define BOX        NS_BOX
#define RHOMB      NS_RHOMB
#define ELLIPSE    NS_ELLIPSE
#define TRIANGLE   NS_TRIANGLE

/* Textmodes */
#define CENTER     TM_CENTER
#define LEFT       TM_LEFT
#define RIGHT      TM_RIGHT



#ifdef __cplusplus
}
#endif

#endif /* _GDL_DEFS_H_ */
