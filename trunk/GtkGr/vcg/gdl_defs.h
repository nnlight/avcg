#ifndef _GDL_DEFS_H_
#define _GDL_DEFS_H_

/*#ifdef __cplusplus
extern "C" {
#endif*/


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



#define	COLOR(c)	(c)

/* Linestyles */
#define	SOLID	   LS_SOLID
#define	DOTTED	   LS_DOTTED
#define	DASHED	   LS_DASHED
#define	UNVISIBLE  LS_UNVISIBLE

/* Arrowstyles */
#define	ASNONE 	   AS_NONE
#define	ASSOLID	   AS_SOLID
#define	ASLINE 	   AS_LINE
#define	ASNONESPEC AS_NONESPEC



/*#ifdef __cplusplus
}
#endif*/

#endif /* _GDL_DEFS_H_ */
