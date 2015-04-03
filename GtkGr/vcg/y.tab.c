/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */


/************* FIRST USER DECLARATIONS *****/


#define YYDEBUG 1
 

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "globals.h"
#include "main.h"
#include "options.h"
#include "grammar.h"



#ifndef yyerror
extern char *yytext;
#define yyerror(x) { \
        syntaxerror(line_nr,pos_nr, \
                 "unexpected %s \"%s\" (%s)", \
                 ((yychar<0)?"(?)":yytname[YYTRANSLATE(yychar)]),     \
                 (strlen(yytext)<48?yytext:"(?)"), \
				 x); \
    }
#endif
 

#ifndef yylocate
#define yylocate(x) (&(x))
#endif





/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LEXWORD_ABOVE = 258,
     LEXWORD_ANCHORPOINTS = 259,
     LEXWORD_ANCHOR = 260,
     LEXWORD_AQUAMARINE = 261,
     LEXWORD_AROUND = 262,
     LEXWORD_ARROWMODE = 263,
     LEXWORD_ARROWHEIGHT = 264,
     LEXWORD_ARROWWIDTH = 265,
     LEXWORD_ARROWCOLOR = 266,
     LEXWORD_ARROWSTYLE = 267,
     LEXWORD_ARROWSIZE = 268,
     LEXWORD_BARROWCOLOR = 269,
     LEXWORD_BARROWSTYLE = 270,
     LEXWORD_BARROWSIZE = 271,
     LEXWORD_BACKEDGE = 272,
     LEXWORD_BARYCENTER = 273,
     LEXWORD_BARY = 274,
     LEXWORD_BARYMEDIAN = 275,
     LEXWORD_BEHIND = 276,
     LEXWORD_BELOW = 277,
     LEXWORD_BLACK = 278,
     LEXWORD_BLUE = 279,
     LEXWORD_BMAX = 280,
     LEXWORD_BORDERCOLOR = 281,
     LEXWORD_BORDERWIDTH = 282,
     LEXWORD_BOTTOM_MARGIN = 283,
     LEXWORD_BOTTOM_TO_TOP = 284,
     LEXWORD_BOTTOM = 285,
     LEXWORD_BOX = 286,
     LEXWORD_BENTNEAREDGE = 287,
     LEXWORD_CENTER = 288,
     LEXWORD_CFISH = 289,
     LEXWORD_CLASSNAME = 290,
     LEXWORD_CLASS = 291,
     LEXWORD_CLUSTER = 292,
     LEXWORD_CMIN = 293,
     LEXWORD_CMAX = 294,
     LEXWORD_COLORENTRY = 295,
     LEXWORD_COLOR = 296,
     LEXWORD_CONSTRAINTS = 297,
     LEXWORD_CONSTRAINT = 298,
     LEXWORD_CONTINUOUS = 299,
     LEXWORD_CROSSING_WEIGHT = 300,
     LEXWORD_CROSSING_OPT = 301,
     LEXWORD_CROSSING_P2 = 302,
     LEXWORD_CYAN = 303,
     LEXWORD_DARKBLUE = 304,
     LEXWORD_DARKCYAN = 305,
     LEXWORD_DARKGREEN = 306,
     LEXWORD_DARKGREY = 307,
     LEXWORD_DARKMAGENTA = 308,
     LEXWORD_DARKRED = 309,
     LEXWORD_DARKYELLOW = 310,
     LEXWORD_DASHED = 311,
     LEXWORD_DFS = 312,
     LEXWORD_DIMENSION = 313,
     LEXWORD_DIRTY_EDGE_LABELS = 314,
     LEXWORD_DISPLAY_EDGE_LABELS = 315,
     LEXWORD_DOTTED = 316,
     LEXWORD_EDGE1 = 317,
     LEXWORD_EDGE2 = 318,
     LEXWORD_EDGES = 319,
     LEXWORD_ELLIPSE = 320,
     LEXWORD_EQUAL_COLUMN = 321,
     LEXWORD_EQUAL_POSITION = 322,
     LEXWORD_EQUAL_ROW = 323,
     LEXWORD_EQUAL = 324,
     LEXWORD_EVERY = 325,
     LEXWORD_FCFISH = 326,
     LEXWORD_FPFISH = 327,
     LEXWORD_FIXED = 328,
     LEXWORD_FREE = 329,
     LEXWORD_FINETUNING = 330,
     LEXWORD_FOLDEDGE = 331,
     LEXWORD_FOLDNODE = 332,
     LEXWORD_FOLDING = 333,
     LEXWORD_FONTNAME = 334,
     LEXWORD_GOLD = 335,
     LEXWORD_GRAPH = 336,
     LEXWORD_GREATER = 337,
     LEXWORD_GREEN = 338,
     LEXWORD_GREY = 339,
     LEXWORD_HEIGHT = 340,
     LEXWORD_HIDESINGLES = 341,
     LEXWORD_HIGH_MARGIN = 342,
     LEXWORD_HIGH = 343,
     LEXWORD_HIDDEN = 344,
     LEXWORD_HORDER = 345,
     LEXWORD_ICONFILE = 346,
     LEXWORD_ICONHEIGHT = 347,
     LEXWORD_ICONSTYLE = 348,
     LEXWORD_ICONWIDTH = 349,
     LEXWORD_INCLUDE = 350,
     LEXWORD_INFONAME = 351,
     LEXWORD_INFO1 = 352,
     LEXWORD_INFO2 = 353,
     LEXWORD_INFO3 = 354,
     LEXWORD_INPUTFUNCTION = 355,
     LEXWORD_INTERVAL = 356,
     LEXWORD_INVISIBLE = 357,
     LEXWORD_IN_FRONT = 358,
     LEXWORD_ISI = 359,
     LEXWORD_KHAKI = 360,
     LEXWORD_TEXTCOLOR = 361,
     LEXWORD_LABEL = 362,
     LEXWORD_LATE_LABELS = 363,
     LEXWORD_LAYOUTALGORITHM = 364,
     LEXWORD_LAYOUTFREQUENCY = 365,
     LEXWORD_LAYOUTPARAMETER = 366,
     LEXWORD_LAYOUTDOWNFACTOR = 367,
     LEXWORD_LAYOUTUPFACTOR = 368,
     LEXWORD_LAYOUTNEARFACTOR = 369,
     LEXWORD_LAYOUTSPLINEFACTOR = 370,
     LEXWORD_LEFT_JUSTIFY = 371,
     LEXWORD_LEFT_MARGIN = 372,
     LEXWORD_LEFT_NEIGHBOR = 373,
     LEXWORD_LEFT_TO_RIGHT = 374,
     LEXWORD_LEFT = 375,
     LEXWORD_LEVEL = 376,
     LEXWORD_VORDER = 377,
     LEXWORD_LIGHTBLUE = 378,
     LEXWORD_LIGHTCYAN = 379,
     LEXWORD_LIGHTGREEN = 380,
     LEXWORD_LIGHTGREY = 381,
     LEXWORD_LIGHTMAGENTA = 382,
     LEXWORD_LIGHTRED = 383,
     LEXWORD_LIGHTYELLOW = 384,
     LEXWORD_LILAC = 385,
     LEXWORD_LIMIT = 386,
     LEXWORD_LINE = 387,
     LEXWORD_LINESTYLE = 388,
     LEXWORD_LOC = 389,
     LEXWORD_LOWER_NEIGHBOR = 390,
     LEXWORD_LOW_MARGIN = 391,
     LEXWORD_LOW = 392,
     LEXWORD_MAGENTA = 393,
     LEXWORD_MANHATTEN = 394,
     LEXWORD_MANUAL = 395,
     LEXWORD_MAXDEPTHSLOW = 396,
     LEXWORD_MAXDEPTH = 397,
     LEXWORD_MAXDEGREE = 398,
     LEXWORD_MAXINDEGREE = 399,
     LEXWORD_MAXOUTDEGREE = 400,
     LEXWORD_MEDIAN = 401,
     LEXWORD_MEDIANBARY = 402,
     LEXWORD_MINDEPTHSLOW = 403,
     LEXWORD_MINDEPTH = 404,
     LEXWORD_MINDEGREE = 405,
     LEXWORD_MININDEGREE = 406,
     LEXWORD_MINOUTDEGREE = 407,
     LEXWORD_MINBACK = 408,
     LEXWORD_NAME = 409,
     LEXWORD_NEAREDGE = 410,
     LEXWORD_NEIGHBORS = 411,
     LEXWORD_NEAREDGES = 412,
     LEXWORD_NONEAREDGES = 413,
     LEXWORD_NODE1 = 414,
     LEXWORD_NODE2 = 415,
     LEXWORD_NODES = 416,
     LEXWORD_NODE_ALIGN = 417,
     LEXWORD_NONE = 418,
     LEXWORD_NO = 419,
     LEXWORD_ORANGE = 420,
     LEXWORD_ORCHID = 421,
     LEXWORD_ORIENTATION = 422,
     LEXWORD_OUTPUTFUNCTION = 423,
     LEXWORD_PFISH = 424,
     LEXWORD_PINK = 425,
     LEXWORD_PLANAR = 426,
     LEXWORD_PMIN = 427,
     LEXWORD_PMAX = 428,
     LEXWORD_PORTSHARING = 429,
     LEXWORD_PRIORITYPHASE = 430,
     LEXWORD_PRIORITY = 431,
     LEXWORD_PURPLE = 432,
     LEXWORD_RANGE = 433,
     LEXWORD_RED = 434,
     LEXWORD_RHOMB = 435,
     LEXWORD_RIGHT_JUSTIFY = 436,
     LEXWORD_RIGHT_MARGIN = 437,
     LEXWORD_RIGHT_NEIGHBOR = 438,
     LEXWORD_RIGHT_TO_LEFT = 439,
     LEXWORD_RIGHT = 440,
     LEXWORD_RMIN = 441,
     LEXWORD_RMAX = 442,
     LEXWORD_SCALING = 443,
     LEXWORD_SHAPE = 444,
     LEXWORD_SHRINK = 445,
     LEXWORD_SMAX = 446,
     LEXWORD_SMANHATTEN = 447,
     LEXWORD_SIZE = 448,
     LEXWORD_SMALLER = 449,
     LEXWORD_SOLID = 450,
     LEXWORD_SOURCENAME = 451,
     LEXWORD_SPLINES = 452,
     LEXWORD_SPREADLEVEL = 453,
     LEXWORD_STATUS = 454,
     LEXWORD_STRETCH = 455,
     LEXWORD_STRAIGHTPHASE = 456,
     LEXWORD_TARGETNAME = 457,
     LEXWORD_TEXTMODE = 458,
     LEXWORD_THICKNESS = 459,
     LEXWORD_TITLE = 460,
     LEXWORD_TOPSORT = 461,
     LEXWORD_TOP_MARGIN = 462,
     LEXWORD_TOP_TO_BOTTOM = 463,
     LEXWORD_TOP = 464,
     LEXWORD_TREE = 465,
     LEXWORD_TREEFACTOR = 466,
     LEXWORD_TRIANGLE = 467,
     LEXWORD_TURQUOISE = 468,
     LEXWORD_TYPENAME = 469,
     LEXWORD_UPPER_NEIGHBOR = 470,
     LEXWORD_VIEW = 471,
     LEXWORD_WHITE = 472,
     LEXWORD_WIDTH = 473,
     LEXWORD_XBASE = 474,
     LEXWORD_XMAX = 475,
     LEXWORD_XRASTER = 476,
     LEXWORD_XLRASTER = 477,
     LEXWORD_XSCROLLBAR = 478,
     LEXWORD_XSPACE = 479,
     LEXWORD_XLSPACE = 480,
     LEXWORD_YBASE = 481,
     LEXWORD_YELLOWGREEN = 482,
     LEXWORD_YELLOW = 483,
     LEXWORD_YES = 484,
     LEXWORD_YMAX = 485,
     LEXWORD_YRASTER = 486,
     LEXWORD_YSCROLLBAR = 487,
     LEXWORD_YSPACE = 488,
     LEX_INT = 489,
     LEX_FLOAT = 490,
     LEX_CHAR = 491,
     LEX_STRING = 492
   };
#endif
/* Tokens.  */
#define LEXWORD_ABOVE 258
#define LEXWORD_ANCHORPOINTS 259
#define LEXWORD_ANCHOR 260
#define LEXWORD_AQUAMARINE 261
#define LEXWORD_AROUND 262
#define LEXWORD_ARROWMODE 263
#define LEXWORD_ARROWHEIGHT 264
#define LEXWORD_ARROWWIDTH 265
#define LEXWORD_ARROWCOLOR 266
#define LEXWORD_ARROWSTYLE 267
#define LEXWORD_ARROWSIZE 268
#define LEXWORD_BARROWCOLOR 269
#define LEXWORD_BARROWSTYLE 270
#define LEXWORD_BARROWSIZE 271
#define LEXWORD_BACKEDGE 272
#define LEXWORD_BARYCENTER 273
#define LEXWORD_BARY 274
#define LEXWORD_BARYMEDIAN 275
#define LEXWORD_BEHIND 276
#define LEXWORD_BELOW 277
#define LEXWORD_BLACK 278
#define LEXWORD_BLUE 279
#define LEXWORD_BMAX 280
#define LEXWORD_BORDERCOLOR 281
#define LEXWORD_BORDERWIDTH 282
#define LEXWORD_BOTTOM_MARGIN 283
#define LEXWORD_BOTTOM_TO_TOP 284
#define LEXWORD_BOTTOM 285
#define LEXWORD_BOX 286
#define LEXWORD_BENTNEAREDGE 287
#define LEXWORD_CENTER 288
#define LEXWORD_CFISH 289
#define LEXWORD_CLASSNAME 290
#define LEXWORD_CLASS 291
#define LEXWORD_CLUSTER 292
#define LEXWORD_CMIN 293
#define LEXWORD_CMAX 294
#define LEXWORD_COLORENTRY 295
#define LEXWORD_COLOR 296
#define LEXWORD_CONSTRAINTS 297
#define LEXWORD_CONSTRAINT 298
#define LEXWORD_CONTINUOUS 299
#define LEXWORD_CROSSING_WEIGHT 300
#define LEXWORD_CROSSING_OPT 301
#define LEXWORD_CROSSING_P2 302
#define LEXWORD_CYAN 303
#define LEXWORD_DARKBLUE 304
#define LEXWORD_DARKCYAN 305
#define LEXWORD_DARKGREEN 306
#define LEXWORD_DARKGREY 307
#define LEXWORD_DARKMAGENTA 308
#define LEXWORD_DARKRED 309
#define LEXWORD_DARKYELLOW 310
#define LEXWORD_DASHED 311
#define LEXWORD_DFS 312
#define LEXWORD_DIMENSION 313
#define LEXWORD_DIRTY_EDGE_LABELS 314
#define LEXWORD_DISPLAY_EDGE_LABELS 315
#define LEXWORD_DOTTED 316
#define LEXWORD_EDGE1 317
#define LEXWORD_EDGE2 318
#define LEXWORD_EDGES 319
#define LEXWORD_ELLIPSE 320
#define LEXWORD_EQUAL_COLUMN 321
#define LEXWORD_EQUAL_POSITION 322
#define LEXWORD_EQUAL_ROW 323
#define LEXWORD_EQUAL 324
#define LEXWORD_EVERY 325
#define LEXWORD_FCFISH 326
#define LEXWORD_FPFISH 327
#define LEXWORD_FIXED 328
#define LEXWORD_FREE 329
#define LEXWORD_FINETUNING 330
#define LEXWORD_FOLDEDGE 331
#define LEXWORD_FOLDNODE 332
#define LEXWORD_FOLDING 333
#define LEXWORD_FONTNAME 334
#define LEXWORD_GOLD 335
#define LEXWORD_GRAPH 336
#define LEXWORD_GREATER 337
#define LEXWORD_GREEN 338
#define LEXWORD_GREY 339
#define LEXWORD_HEIGHT 340
#define LEXWORD_HIDESINGLES 341
#define LEXWORD_HIGH_MARGIN 342
#define LEXWORD_HIGH 343
#define LEXWORD_HIDDEN 344
#define LEXWORD_HORDER 345
#define LEXWORD_ICONFILE 346
#define LEXWORD_ICONHEIGHT 347
#define LEXWORD_ICONSTYLE 348
#define LEXWORD_ICONWIDTH 349
#define LEXWORD_INCLUDE 350
#define LEXWORD_INFONAME 351
#define LEXWORD_INFO1 352
#define LEXWORD_INFO2 353
#define LEXWORD_INFO3 354
#define LEXWORD_INPUTFUNCTION 355
#define LEXWORD_INTERVAL 356
#define LEXWORD_INVISIBLE 357
#define LEXWORD_IN_FRONT 358
#define LEXWORD_ISI 359
#define LEXWORD_KHAKI 360
#define LEXWORD_TEXTCOLOR 361
#define LEXWORD_LABEL 362
#define LEXWORD_LATE_LABELS 363
#define LEXWORD_LAYOUTALGORITHM 364
#define LEXWORD_LAYOUTFREQUENCY 365
#define LEXWORD_LAYOUTPARAMETER 366
#define LEXWORD_LAYOUTDOWNFACTOR 367
#define LEXWORD_LAYOUTUPFACTOR 368
#define LEXWORD_LAYOUTNEARFACTOR 369
#define LEXWORD_LAYOUTSPLINEFACTOR 370
#define LEXWORD_LEFT_JUSTIFY 371
#define LEXWORD_LEFT_MARGIN 372
#define LEXWORD_LEFT_NEIGHBOR 373
#define LEXWORD_LEFT_TO_RIGHT 374
#define LEXWORD_LEFT 375
#define LEXWORD_LEVEL 376
#define LEXWORD_VORDER 377
#define LEXWORD_LIGHTBLUE 378
#define LEXWORD_LIGHTCYAN 379
#define LEXWORD_LIGHTGREEN 380
#define LEXWORD_LIGHTGREY 381
#define LEXWORD_LIGHTMAGENTA 382
#define LEXWORD_LIGHTRED 383
#define LEXWORD_LIGHTYELLOW 384
#define LEXWORD_LILAC 385
#define LEXWORD_LIMIT 386
#define LEXWORD_LINE 387
#define LEXWORD_LINESTYLE 388
#define LEXWORD_LOC 389
#define LEXWORD_LOWER_NEIGHBOR 390
#define LEXWORD_LOW_MARGIN 391
#define LEXWORD_LOW 392
#define LEXWORD_MAGENTA 393
#define LEXWORD_MANHATTEN 394
#define LEXWORD_MANUAL 395
#define LEXWORD_MAXDEPTHSLOW 396
#define LEXWORD_MAXDEPTH 397
#define LEXWORD_MAXDEGREE 398
#define LEXWORD_MAXINDEGREE 399
#define LEXWORD_MAXOUTDEGREE 400
#define LEXWORD_MEDIAN 401
#define LEXWORD_MEDIANBARY 402
#define LEXWORD_MINDEPTHSLOW 403
#define LEXWORD_MINDEPTH 404
#define LEXWORD_MINDEGREE 405
#define LEXWORD_MININDEGREE 406
#define LEXWORD_MINOUTDEGREE 407
#define LEXWORD_MINBACK 408
#define LEXWORD_NAME 409
#define LEXWORD_NEAREDGE 410
#define LEXWORD_NEIGHBORS 411
#define LEXWORD_NEAREDGES 412
#define LEXWORD_NONEAREDGES 413
#define LEXWORD_NODE1 414
#define LEXWORD_NODE2 415
#define LEXWORD_NODES 416
#define LEXWORD_NODE_ALIGN 417
#define LEXWORD_NONE 418
#define LEXWORD_NO 419
#define LEXWORD_ORANGE 420
#define LEXWORD_ORCHID 421
#define LEXWORD_ORIENTATION 422
#define LEXWORD_OUTPUTFUNCTION 423
#define LEXWORD_PFISH 424
#define LEXWORD_PINK 425
#define LEXWORD_PLANAR 426
#define LEXWORD_PMIN 427
#define LEXWORD_PMAX 428
#define LEXWORD_PORTSHARING 429
#define LEXWORD_PRIORITYPHASE 430
#define LEXWORD_PRIORITY 431
#define LEXWORD_PURPLE 432
#define LEXWORD_RANGE 433
#define LEXWORD_RED 434
#define LEXWORD_RHOMB 435
#define LEXWORD_RIGHT_JUSTIFY 436
#define LEXWORD_RIGHT_MARGIN 437
#define LEXWORD_RIGHT_NEIGHBOR 438
#define LEXWORD_RIGHT_TO_LEFT 439
#define LEXWORD_RIGHT 440
#define LEXWORD_RMIN 441
#define LEXWORD_RMAX 442
#define LEXWORD_SCALING 443
#define LEXWORD_SHAPE 444
#define LEXWORD_SHRINK 445
#define LEXWORD_SMAX 446
#define LEXWORD_SMANHATTEN 447
#define LEXWORD_SIZE 448
#define LEXWORD_SMALLER 449
#define LEXWORD_SOLID 450
#define LEXWORD_SOURCENAME 451
#define LEXWORD_SPLINES 452
#define LEXWORD_SPREADLEVEL 453
#define LEXWORD_STATUS 454
#define LEXWORD_STRETCH 455
#define LEXWORD_STRAIGHTPHASE 456
#define LEXWORD_TARGETNAME 457
#define LEXWORD_TEXTMODE 458
#define LEXWORD_THICKNESS 459
#define LEXWORD_TITLE 460
#define LEXWORD_TOPSORT 461
#define LEXWORD_TOP_MARGIN 462
#define LEXWORD_TOP_TO_BOTTOM 463
#define LEXWORD_TOP 464
#define LEXWORD_TREE 465
#define LEXWORD_TREEFACTOR 466
#define LEXWORD_TRIANGLE 467
#define LEXWORD_TURQUOISE 468
#define LEXWORD_TYPENAME 469
#define LEXWORD_UPPER_NEIGHBOR 470
#define LEXWORD_VIEW 471
#define LEXWORD_WHITE 472
#define LEXWORD_WIDTH 473
#define LEXWORD_XBASE 474
#define LEXWORD_XMAX 475
#define LEXWORD_XRASTER 476
#define LEXWORD_XLRASTER 477
#define LEXWORD_XSCROLLBAR 478
#define LEXWORD_XSPACE 479
#define LEXWORD_XLSPACE 480
#define LEXWORD_YBASE 481
#define LEXWORD_YELLOWGREEN 482
#define LEXWORD_YELLOW 483
#define LEXWORD_YES 484
#define LEXWORD_YMAX 485
#define LEXWORD_YRASTER 486
#define LEXWORD_YSCROLLBAR 487
#define LEXWORD_YSPACE 488
#define LEX_INT 489
#define LEX_FLOAT 490
#define LEX_CHAR 491
#define LEX_STRING 492




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

 
	unsigned char      byte;
	short int          snum;
	unsigned short int usnum;
	int                num;
	unsigned int       unum;
	long int           lnum;
	unsigned long int  ulnum;
	float              realnum;
	double             lrealnum;
	char              *string;
	yysyntaxtree       tree;



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/************* SECOND USER DECLARATIONS *****/



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1495

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  248
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  316
/* YYNRULES -- Number of states.  */
#define YYNSTATES  654

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   492

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,   244,     2,     2,   246,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   240,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   245,     2,   247,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     241,   242,   243,   238,     2,   239,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    11,    13,    15,    17,    19,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    41,    45,
      49,    53,    57,    61,    65,    69,    73,    77,    81,    85,
      89,    99,   103,   107,   111,   115,   119,   123,   127,   131,
     135,   139,   143,   147,   151,   155,   159,   163,   167,   171,
     175,   179,   183,   187,   192,   197,   204,   208,   212,   216,
     220,   224,   228,   232,   236,   240,   244,   248,   252,   256,
     260,   264,   266,   270,   274,   278,   282,   286,   290,   294,
     298,   302,   306,   310,   314,   318,   322,   326,   330,   334,
     338,   342,   346,   350,   354,   358,   362,   366,   370,   374,
     378,   382,   386,   390,   392,   394,   396,   398,   400,   402,
     404,   406,   408,   410,   412,   414,   416,   418,   420,   422,
     424,   426,   428,   430,   432,   434,   436,   438,   440,   442,
     444,   446,   448,   450,   452,   454,   456,   458,   460,   462,
     464,   466,   468,   470,   472,   474,   476,   478,   480,   482,
     484,   486,   488,   490,   492,   494,   496,   498,   500,   502,
     504,   506,   508,   510,   512,   514,   516,   518,   520,   522,
     524,   526,   528,   530,   532,   534,   536,   539,   542,   545,
     548,   553,   556,   558,   563,   568,   573,   578,   581,   583,
     588,   591,   593,   597,   601,   605,   609,   613,   617,   621,
     625,   629,   633,   637,   641,   645,   649,   653,   663,   667,
     671,   675,   679,   683,   687,   691,   695,   699,   703,   707,
     711,   713,   715,   717,   719,   721,   723,   725,   727,   729,
     731,   733,   735,   737,   741,   745,   749,   753,   757,   761,
     765,   769,   773,   777,   781,   785,   789,   793,   797,   801,
     805,   809,   813,   817,   821,   823,   825,   827,   829,   831,
     833,   835,   837,   841,   845,   849,   855,   859,   863,   867,
     870,   872,   874,   876,   878,   880,   882,   884,   886,   888,
     890,   892,   894,   896,   898,   900,   902,   904,   906,   908,
     910,   912,   914,   916,   918,   920,   922,   924,   926,   928,
     930,   932,   934,   936,   938,   940,   944,   947,   949,   951,
     955,   959,   963,   969,   971,   973,   975
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     249,     0,    -1,    81,   238,   250,   239,    -1,   250,   251,
      -1,   251,    -1,   252,    -1,   265,    -1,   266,    -1,   263,
      -1,   264,    -1,   249,    -1,   267,    -1,   269,    -1,   270,
      -1,   271,    -1,   272,    -1,   274,    -1,   205,   240,   296,
      -1,   107,   240,   296,    -1,    97,   240,   296,    -1,    98,
     240,   296,    -1,    99,   240,   296,    -1,    41,   240,   253,
      -1,   106,   240,   253,    -1,    26,   240,   253,    -1,   218,
     240,   294,    -1,    85,   240,   294,    -1,    27,   240,   294,
      -1,   241,   240,   294,    -1,   242,   240,   294,    -1,   134,
     238,   241,   240,   294,   242,   240,   294,   239,    -1,    78,
     240,   294,    -1,   188,   240,   295,    -1,   190,   240,   294,
      -1,   200,   240,   294,    -1,   203,   240,   277,    -1,   189,
     240,   278,    -1,   121,   240,   294,    -1,   122,   240,   294,
      -1,    90,   240,   294,    -1,   199,   240,   258,    -1,   220,
     240,   294,    -1,   230,   240,   294,    -1,   219,   240,   294,
      -1,   226,   240,   294,    -1,   224,   240,   294,    -1,   225,
     240,   294,    -1,   233,   240,   294,    -1,   221,   240,   294,
      -1,   222,   240,   294,    -1,   231,   240,   294,    -1,   102,
     240,   294,    -1,    89,   240,   294,    -1,    35,   294,   240,
     296,    -1,    96,   294,   240,   296,    -1,    40,   294,   240,
     294,   294,   294,    -1,   109,   240,   256,    -1,   110,   240,
     257,    -1,   112,   240,   294,    -1,   113,   240,   294,    -1,
     114,   240,   294,    -1,   115,   240,   294,    -1,   108,   240,
     259,    -1,    60,   240,   259,    -1,    59,   240,   259,    -1,
      75,   240,   259,    -1,    86,   240,   259,    -1,   201,   240,
     259,    -1,   175,   240,   259,    -1,   139,   240,   259,    -1,
     192,   240,   259,    -1,   158,    -1,   157,   240,   164,    -1,
     157,   240,   229,    -1,   167,   240,   255,    -1,   162,   240,
     279,    -1,   174,   240,   259,    -1,     8,   240,   262,    -1,
     198,   240,   294,    -1,   211,   240,   295,    -1,    47,   240,
     259,    -1,    46,   240,   259,    -1,    45,   240,   260,    -1,
     216,   240,   261,    -1,    64,   240,   259,    -1,   161,   240,
     259,    -1,   197,   240,   259,    -1,    25,   240,   294,    -1,
      39,   240,   294,    -1,    38,   240,   294,    -1,   173,   240,
     294,    -1,   172,   240,   294,    -1,   187,   240,   294,    -1,
     186,   240,   294,    -1,   191,   240,   294,    -1,   214,   240,
     296,    -1,    95,   240,   296,    -1,   111,   240,   289,    -1,
     206,   240,   254,    -1,   100,   240,   296,    -1,   168,   240,
     296,    -1,   223,   240,   294,    -1,   232,   240,   294,    -1,
       6,    -1,    23,    -1,    24,    -1,    48,    -1,    49,    -1,
      50,    -1,    51,    -1,    52,    -1,    53,    -1,    54,    -1,
      55,    -1,    80,    -1,    83,    -1,   105,    -1,   123,    -1,
     124,    -1,   125,    -1,   126,    -1,   127,    -1,   128,    -1,
     129,    -1,   130,    -1,   138,    -1,   165,    -1,   166,    -1,
     170,    -1,   177,    -1,   179,    -1,   213,    -1,   217,    -1,
     228,    -1,   227,    -1,   294,    -1,    88,    -1,   137,    -1,
     208,    -1,    29,    -1,   119,    -1,   184,    -1,    18,    -1,
     104,    -1,   171,    -1,    42,    -1,   210,    -1,   142,    -1,
     149,    -1,   141,    -1,   148,    -1,   143,    -1,   150,    -1,
     144,    -1,   151,    -1,   145,    -1,   152,    -1,   153,    -1,
      57,    -1,    70,    -1,   140,    -1,    23,    -1,    84,    -1,
     217,    -1,   229,    -1,   164,    -1,    19,    -1,   146,    -1,
      20,    -1,   147,    -1,    34,    -1,    71,    -1,   169,    -1,
      72,    -1,    73,    -1,    74,    -1,    77,   276,    -1,    76,
     281,    -1,   159,   276,    -1,    62,   281,    -1,   160,   238,
     268,   239,    -1,   268,   276,    -1,   276,    -1,    63,   238,
     273,   239,    -1,   155,   238,   273,   239,    -1,    32,   238,
     273,   239,    -1,    17,   238,   273,   239,    -1,   273,   281,
      -1,   281,    -1,    43,   238,   275,   239,    -1,   275,   284,
      -1,   284,    -1,   205,   240,   296,    -1,   107,   240,   296,
      -1,    97,   240,   296,    -1,    98,   240,   296,    -1,    99,
     240,   296,    -1,    79,   240,   296,    -1,    41,   240,   253,
      -1,   106,   240,   253,    -1,    26,   240,   253,    -1,    91,
     240,   296,    -1,     4,   240,   296,    -1,   214,   240,   296,
      -1,   218,   240,   294,    -1,    85,   240,   294,    -1,    27,
     240,   294,    -1,   134,   238,   241,   240,   294,   242,   240,
     294,   239,    -1,    78,   240,   294,    -1,   188,   240,   295,
      -1,   190,   240,   294,    -1,   200,   240,   294,    -1,    94,
     240,   294,    -1,    92,   240,   294,    -1,   203,   240,   277,
      -1,    93,   240,   280,    -1,   189,   240,   278,    -1,   121,
     240,   294,    -1,   122,   240,   294,    -1,    90,   240,   294,
      -1,    33,    -1,   116,    -1,   181,    -1,    31,    -1,   180,
      -1,    65,    -1,   212,    -1,    30,    -1,   209,    -1,    33,
      -1,    30,    -1,   209,    -1,     7,    -1,   196,   240,   296,
      -1,   202,   240,   296,    -1,   107,   240,   296,    -1,   106,
     240,   253,    -1,    79,   240,   296,    -1,    41,   240,   253,
      -1,   214,   240,   296,    -1,   204,   240,   294,    -1,    36,
     240,   294,    -1,   176,   240,   294,    -1,    10,   240,   294,
      -1,     9,   240,   294,    -1,    11,   240,   253,    -1,    14,
     240,   253,    -1,    13,   240,   294,    -1,    16,   240,   294,
      -1,    12,   240,   283,    -1,    15,   240,   283,    -1,   133,
     240,   282,    -1,     5,   240,   294,    -1,    90,   240,   294,
      -1,    44,    -1,   195,    -1,    61,    -1,    56,    -1,   102,
      -1,   163,    -1,   132,    -1,   195,    -1,   205,   240,   296,
      -1,   176,   240,   294,    -1,   193,   240,   294,    -1,   161,
     240,   238,   285,   239,    -1,   101,   240,   289,    -1,   154,
     240,   286,    -1,    58,   240,   287,    -1,   285,   296,    -1,
     296,    -1,    69,    -1,   194,    -1,    82,    -1,   156,    -1,
     136,    -1,    87,    -1,   178,    -1,    37,    -1,   131,    -1,
       3,    -1,    22,    -1,   120,    -1,   185,    -1,   103,    -1,
      21,    -1,    67,    -1,    68,    -1,    66,    -1,   207,    -1,
      28,    -1,   117,    -1,   182,    -1,   215,    -1,   135,    -1,
     118,    -1,   183,    -1,   241,    -1,   242,    -1,   243,    -1,
     234,    -1,   235,    -1,   236,    -1,   237,    -1,   289,    -1,
     238,   290,   239,    -1,   290,   291,    -1,   291,    -1,   288,
      -1,   293,   240,   288,    -1,   292,   240,   288,    -1,   244,
     240,   288,    -1,   245,   294,   246,   294,   247,    -1,   234,
      -1,   234,    -1,   235,    -1,   237,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   367,   367,   373,   376,   382,   384,   386,   388,   390,
     392,   394,   396,   398,   400,   402,   404,   409,   412,   415,
     418,   421,   424,   427,   430,   433,   436,   439,   442,   444,
     446,   449,   452,   455,   458,   461,   464,   467,   470,   473,
     476,   480,   483,   486,   489,   492,   495,   498,   501,   504,
     507,   510,   513,   516,   519,   522,   526,   529,   532,   535,
     538,   541,   545,   548,   551,   554,   557,   560,   563,   566,
     569,   572,   574,   577,   580,   583,   586,   589,   593,   596,
     600,   603,   606,   610,   613,   616,   619,   623,   626,   629,
     632,   635,   638,   641,   644,   648,   651,   654,   657,   660,
     663,   666,   669,   675,   677,   679,   681,   683,   685,   687,
     689,   691,   693,   695,   697,   699,   701,   703,   705,   707,
     709,   711,   713,   715,   717,   719,   721,   723,   725,   727,
     729,   731,   733,   735,   737,   739,   744,   746,   751,   753,
     755,   757,   763,   765,   767,   769,   772,   774,   776,   778,
     780,   782,   784,   786,   788,   790,   792,   794,   796,   802,
     804,   809,   811,   813,   818,   820,   824,   826,   828,   830,
     834,   836,   838,   840,   844,   846,   850,   855,   860,   865,
     871,   877,   880,   886,   891,   896,   901,   907,   910,   916,
     923,   926,   932,   935,   938,   941,   944,   947,   950,   953,
     956,   959,   962,   965,   968,   971,   974,   977,   980,   983,
     986,   989,   992,   995,   998,  1001,  1004,  1007,  1010,  1013,
    1018,  1020,  1022,  1026,  1028,  1030,  1032,  1036,  1038,  1040,
    1044,  1046,  1048,  1053,  1056,  1059,  1062,  1065,  1068,  1071,
    1074,  1077,  1080,  1083,  1086,  1089,  1092,  1095,  1098,  1101,
    1104,  1107,  1110,  1113,  1119,  1121,  1123,  1125,  1127,  1132,
    1134,  1136,  1140,  1143,  1146,  1149,  1152,  1155,  1158,  1163,
    1166,  1170,  1172,  1174,  1176,  1178,  1180,  1182,  1184,  1186,
    1188,  1190,  1192,  1194,  1196,  1198,  1200,  1202,  1204,  1206,
    1208,  1210,  1212,  1214,  1216,  1218,  1220,  1224,  1226,  1228,
    1232,  1234,  1236,  1238,  1240,  1244,  1249,  1252,  1256,  1258,
    1261,  1264,  1269,  1274,  1278,  1282,  1286
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LEXWORD_ABOVE", "LEXWORD_ANCHORPOINTS",
  "LEXWORD_ANCHOR", "LEXWORD_AQUAMARINE", "LEXWORD_AROUND",
  "LEXWORD_ARROWMODE", "LEXWORD_ARROWHEIGHT", "LEXWORD_ARROWWIDTH",
  "LEXWORD_ARROWCOLOR", "LEXWORD_ARROWSTYLE", "LEXWORD_ARROWSIZE",
  "LEXWORD_BARROWCOLOR", "LEXWORD_BARROWSTYLE", "LEXWORD_BARROWSIZE",
  "LEXWORD_BACKEDGE", "LEXWORD_BARYCENTER", "LEXWORD_BARY",
  "LEXWORD_BARYMEDIAN", "LEXWORD_BEHIND", "LEXWORD_BELOW", "LEXWORD_BLACK",
  "LEXWORD_BLUE", "LEXWORD_BMAX", "LEXWORD_BORDERCOLOR",
  "LEXWORD_BORDERWIDTH", "LEXWORD_BOTTOM_MARGIN", "LEXWORD_BOTTOM_TO_TOP",
  "LEXWORD_BOTTOM", "LEXWORD_BOX", "LEXWORD_BENTNEAREDGE",
  "LEXWORD_CENTER", "LEXWORD_CFISH", "LEXWORD_CLASSNAME", "LEXWORD_CLASS",
  "LEXWORD_CLUSTER", "LEXWORD_CMIN", "LEXWORD_CMAX", "LEXWORD_COLORENTRY",
  "LEXWORD_COLOR", "LEXWORD_CONSTRAINTS", "LEXWORD_CONSTRAINT",
  "LEXWORD_CONTINUOUS", "LEXWORD_CROSSING_WEIGHT", "LEXWORD_CROSSING_OPT",
  "LEXWORD_CROSSING_P2", "LEXWORD_CYAN", "LEXWORD_DARKBLUE",
  "LEXWORD_DARKCYAN", "LEXWORD_DARKGREEN", "LEXWORD_DARKGREY",
  "LEXWORD_DARKMAGENTA", "LEXWORD_DARKRED", "LEXWORD_DARKYELLOW",
  "LEXWORD_DASHED", "LEXWORD_DFS", "LEXWORD_DIMENSION",
  "LEXWORD_DIRTY_EDGE_LABELS", "LEXWORD_DISPLAY_EDGE_LABELS",
  "LEXWORD_DOTTED", "LEXWORD_EDGE1", "LEXWORD_EDGE2", "LEXWORD_EDGES",
  "LEXWORD_ELLIPSE", "LEXWORD_EQUAL_COLUMN", "LEXWORD_EQUAL_POSITION",
  "LEXWORD_EQUAL_ROW", "LEXWORD_EQUAL", "LEXWORD_EVERY", "LEXWORD_FCFISH",
  "LEXWORD_FPFISH", "LEXWORD_FIXED", "LEXWORD_FREE", "LEXWORD_FINETUNING",
  "LEXWORD_FOLDEDGE", "LEXWORD_FOLDNODE", "LEXWORD_FOLDING",
  "LEXWORD_FONTNAME", "LEXWORD_GOLD", "LEXWORD_GRAPH", "LEXWORD_GREATER",
  "LEXWORD_GREEN", "LEXWORD_GREY", "LEXWORD_HEIGHT", "LEXWORD_HIDESINGLES",
  "LEXWORD_HIGH_MARGIN", "LEXWORD_HIGH", "LEXWORD_HIDDEN",
  "LEXWORD_HORDER", "LEXWORD_ICONFILE", "LEXWORD_ICONHEIGHT",
  "LEXWORD_ICONSTYLE", "LEXWORD_ICONWIDTH", "LEXWORD_INCLUDE",
  "LEXWORD_INFONAME", "LEXWORD_INFO1", "LEXWORD_INFO2", "LEXWORD_INFO3",
  "LEXWORD_INPUTFUNCTION", "LEXWORD_INTERVAL", "LEXWORD_INVISIBLE",
  "LEXWORD_IN_FRONT", "LEXWORD_ISI", "LEXWORD_KHAKI", "LEXWORD_TEXTCOLOR",
  "LEXWORD_LABEL", "LEXWORD_LATE_LABELS", "LEXWORD_LAYOUTALGORITHM",
  "LEXWORD_LAYOUTFREQUENCY", "LEXWORD_LAYOUTPARAMETER",
  "LEXWORD_LAYOUTDOWNFACTOR", "LEXWORD_LAYOUTUPFACTOR",
  "LEXWORD_LAYOUTNEARFACTOR", "LEXWORD_LAYOUTSPLINEFACTOR",
  "LEXWORD_LEFT_JUSTIFY", "LEXWORD_LEFT_MARGIN", "LEXWORD_LEFT_NEIGHBOR",
  "LEXWORD_LEFT_TO_RIGHT", "LEXWORD_LEFT", "LEXWORD_LEVEL",
  "LEXWORD_VORDER", "LEXWORD_LIGHTBLUE", "LEXWORD_LIGHTCYAN",
  "LEXWORD_LIGHTGREEN", "LEXWORD_LIGHTGREY", "LEXWORD_LIGHTMAGENTA",
  "LEXWORD_LIGHTRED", "LEXWORD_LIGHTYELLOW", "LEXWORD_LILAC",
  "LEXWORD_LIMIT", "LEXWORD_LINE", "LEXWORD_LINESTYLE", "LEXWORD_LOC",
  "LEXWORD_LOWER_NEIGHBOR", "LEXWORD_LOW_MARGIN", "LEXWORD_LOW",
  "LEXWORD_MAGENTA", "LEXWORD_MANHATTEN", "LEXWORD_MANUAL",
  "LEXWORD_MAXDEPTHSLOW", "LEXWORD_MAXDEPTH", "LEXWORD_MAXDEGREE",
  "LEXWORD_MAXINDEGREE", "LEXWORD_MAXOUTDEGREE", "LEXWORD_MEDIAN",
  "LEXWORD_MEDIANBARY", "LEXWORD_MINDEPTHSLOW", "LEXWORD_MINDEPTH",
  "LEXWORD_MINDEGREE", "LEXWORD_MININDEGREE", "LEXWORD_MINOUTDEGREE",
  "LEXWORD_MINBACK", "LEXWORD_NAME", "LEXWORD_NEAREDGE",
  "LEXWORD_NEIGHBORS", "LEXWORD_NEAREDGES", "LEXWORD_NONEAREDGES",
  "LEXWORD_NODE1", "LEXWORD_NODE2", "LEXWORD_NODES", "LEXWORD_NODE_ALIGN",
  "LEXWORD_NONE", "LEXWORD_NO", "LEXWORD_ORANGE", "LEXWORD_ORCHID",
  "LEXWORD_ORIENTATION", "LEXWORD_OUTPUTFUNCTION", "LEXWORD_PFISH",
  "LEXWORD_PINK", "LEXWORD_PLANAR", "LEXWORD_PMIN", "LEXWORD_PMAX",
  "LEXWORD_PORTSHARING", "LEXWORD_PRIORITYPHASE", "LEXWORD_PRIORITY",
  "LEXWORD_PURPLE", "LEXWORD_RANGE", "LEXWORD_RED", "LEXWORD_RHOMB",
  "LEXWORD_RIGHT_JUSTIFY", "LEXWORD_RIGHT_MARGIN",
  "LEXWORD_RIGHT_NEIGHBOR", "LEXWORD_RIGHT_TO_LEFT", "LEXWORD_RIGHT",
  "LEXWORD_RMIN", "LEXWORD_RMAX", "LEXWORD_SCALING", "LEXWORD_SHAPE",
  "LEXWORD_SHRINK", "LEXWORD_SMAX", "LEXWORD_SMANHATTEN", "LEXWORD_SIZE",
  "LEXWORD_SMALLER", "LEXWORD_SOLID", "LEXWORD_SOURCENAME",
  "LEXWORD_SPLINES", "LEXWORD_SPREADLEVEL", "LEXWORD_STATUS",
  "LEXWORD_STRETCH", "LEXWORD_STRAIGHTPHASE", "LEXWORD_TARGETNAME",
  "LEXWORD_TEXTMODE", "LEXWORD_THICKNESS", "LEXWORD_TITLE",
  "LEXWORD_TOPSORT", "LEXWORD_TOP_MARGIN", "LEXWORD_TOP_TO_BOTTOM",
  "LEXWORD_TOP", "LEXWORD_TREE", "LEXWORD_TREEFACTOR", "LEXWORD_TRIANGLE",
  "LEXWORD_TURQUOISE", "LEXWORD_TYPENAME", "LEXWORD_UPPER_NEIGHBOR",
  "LEXWORD_VIEW", "LEXWORD_WHITE", "LEXWORD_WIDTH", "LEXWORD_XBASE",
  "LEXWORD_XMAX", "LEXWORD_XRASTER", "LEXWORD_XLRASTER",
  "LEXWORD_XSCROLLBAR", "LEXWORD_XSPACE", "LEXWORD_XLSPACE",
  "LEXWORD_YBASE", "LEXWORD_YELLOWGREEN", "LEXWORD_YELLOW", "LEXWORD_YES",
  "LEXWORD_YMAX", "LEXWORD_YRASTER", "LEXWORD_YSCROLLBAR",
  "LEXWORD_YSPACE", "LEX_INT", "LEX_FLOAT", "LEX_CHAR", "LEX_STRING",
  "'{'", "'}'", "':'", "'x'", "'y'", "'z'", "'*'", "'['", "'-'", "']'",
  "$accept", "graph", "graph_entry_list", "graph_entry", "graph_attribute",
  "enum_color", "enum_topsort", "enum_orientation", "enum_layoutalgorithm",
  "enum_layoutfrequency", "enum_status", "enum_yes_no",
  "enum_cross_weight", "enum_view", "enum_arrow_mode", "foldnode_defaults",
  "foldedge_defaults", "node_defaults", "edge_defaults", "node",
  "node_attribute_list", "edge", "nearedge", "bentnearedge", "backedge",
  "edge_attribute_list", "constraint", "constraint_attribute_list",
  "node_attribute", "enum_textmode", "enum_shape", "enum_node_align",
  "enum_iconstyle", "edge_attribute", "enum_linestyle", "enum_arrowstyle",
  "constraint_attribute", "string_array", "enum_name", "enum_dimension",
  "attribute_value", "array_value", "index_value_list", "index_value",
  "range", "index", "int_const", "float_const", "str_const", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   123,   125,
      58,   120,   121,   122,    42,    91,    45,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   248,   249,   250,   250,   251,   251,   251,   251,   251,
     251,   251,   251,   251,   251,   251,   251,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   252,   252,   252,   252,
     252,   252,   252,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   254,   254,   255,   255,
     255,   255,   256,   256,   256,   256,   256,   256,   256,   256,
     256,   256,   256,   256,   256,   256,   256,   256,   256,   257,
     257,   258,   258,   258,   259,   259,   260,   260,   260,   260,
     261,   261,   261,   261,   262,   262,   263,   264,   265,   266,
     267,   268,   268,   269,   270,   271,   272,   273,   273,   274,
     275,   275,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   276,   276,   276,   276,
     277,   277,   277,   278,   278,   278,   278,   279,   279,   279,
     280,   280,   280,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   282,   282,   282,   282,   282,   283,
     283,   283,   284,   284,   284,   284,   284,   284,   284,   285,
     285,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   287,   287,   287,
     288,   288,   288,   288,   288,   289,   290,   290,   291,   291,
     291,   291,   292,   293,   294,   295,   296
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       9,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     6,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       4,     2,     1,     4,     4,     4,     4,     2,     1,     4,
       2,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     9,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     5,     3,     3,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     2,     1,     1,     3,
       3,     3,     5,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      10,     0,     4,     5,     8,     9,     6,     7,    11,    12,
      13,    14,    15,    16,     0,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,     0,     0,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     3,
     174,   175,    77,     0,   188,    87,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   134,   133,    24,   135,
      27,     0,     0,    89,    88,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,   191,   166,   168,   167,   169,
      82,   165,   164,    81,    80,    64,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    31,
      26,    66,    52,    39,   316,    96,     0,    19,    20,    21,
      99,    51,    23,    18,    62,   142,   145,   158,   143,   149,
     147,   151,   153,   155,   150,   148,   152,   154,   156,   157,
     144,   146,    56,   159,   160,    57,     0,    97,    58,    59,
      60,    61,    37,    38,     0,    69,     0,    72,    73,     0,
     182,    85,   227,   229,   228,    75,   139,   140,   141,   138,
      74,   100,    91,    90,    76,    68,    93,    92,   315,    32,
     223,   225,   224,   226,    36,    33,    94,    70,    86,    78,
     161,   162,   163,    40,    34,    67,   220,   221,   222,    35,
      17,   136,   137,    98,    79,    95,   170,   171,   173,   172,
      83,    25,    43,    41,    48,    49,   101,    45,    46,    44,
      42,    50,   102,    47,    28,    29,   186,   187,   185,    53,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     252,   244,   243,   245,   260,   259,   261,   249,   247,   246,
     250,   248,   241,   238,   237,   253,   236,   235,   254,   257,
     256,   258,   255,   251,   242,   233,   234,   240,   239,   183,
     202,   200,   206,   198,   208,   197,   205,   219,   201,   213,
     232,   230,   231,   215,   212,   194,   195,   196,   199,   193,
     217,   218,     0,   209,   216,   210,   211,   214,   192,   203,
     204,    54,   300,   301,   302,   303,     0,     0,   308,   304,
       0,   307,     0,     0,     0,   184,   180,   181,     0,   297,
     298,   299,   268,   266,   280,   285,   281,   290,   278,   288,
     286,   287,   271,   273,   276,   284,   291,   295,   282,   279,
     294,   275,   274,   277,   292,   296,   283,   272,   289,   293,
     267,     0,   263,   264,   262,     0,     0,     0,   305,   306,
       0,     0,     0,    55,     0,   270,     0,   300,   311,     0,
     310,   309,     0,   265,   269,     0,     0,     0,     0,   312,
       0,     0,    30,   207
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   100,   101,   102,   103,   298,   473,   440,   412,   415,
     463,   323,   320,   480,   262,   104,   105,   106,   107,   108,
     429,   109,   110,   111,   112,   263,   113,   314,   186,   469,
     454,   435,   553,   264,   533,   517,   315,   634,   620,   592,
     578,   579,   580,   581,   582,   583,   299,   449,   385
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -597
static const yytype_int16 yypact[] =
{
     -45,  -200,    40,   763,  -597,  -198,  -194,  -190,  -185,  -182,
    -179,  -174,  -171,  -155,  -174,  -154,  -150,  -149,  -143,  -138,
    -134,  -123,  1275,  -148,  -122,  -120,  1275,     2,  -118,  -114,
    -108,  -105,   -95,   -85,  -174,   -84,   -82,   -76,   -75,   -74,
     -73,   -72,   -66,   -65,   -59,   -58,   -57,   -56,   -52,   -37,
     -36,   -32,  -104,   -28,   -79,   -27,  -597,     2,   -13,   -17,
     -12,    -9,     7,     8,    10,    11,    13,    14,    16,    18,
      21,    25,    27,    28,    29,    31,    32,    37,    39,    41,
      45,    49,    50,    51,    52,    53,    57,    58,    60,    61,
      62,    63,    64,    66,    67,    68,    69,    70,    71,    72,
    -597,   544,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,   -49,  1275,  -174,  1214,  -174,  1275,
    -597,    73,  -174,  -174,    74,  1214,    24,    26,  -146,  -146,
    -146,  -146,    79,    80,    82,    83,    87,    89,    90,    91,
      96,    99,   100,   101,   103,   105,   110,   116,   119,   120,
     121,   124,   126,  -597,  1275,  -146,  -146,  -597,   128,   129,
     130,   131,   132,   133,   135,   136,   138,   141,   142,   143,
     144,   145,   146,   147,   148,   151,   152,    42,   153,   154,
     155,   157,   158,   159,   160,   166,  -597,  -174,  -174,  -146,
    -174,  -174,  -170,   167,  -170,  -170,  -170,  -170,  -174,  1214,
    -170,  -146,    -1,   -61,   176,  -174,  -174,  -174,  -174,  -174,
    -174,   174,  -146,  1275,  -145,  -597,     2,  -146,   -20,   -15,
    -170,  -174,  -174,  -146,  -146,  -174,  -174,  -102,   -26,  -174,
    -174,  -146,  -146,  -174,   -16,  -174,  -146,   -18,  -170,   -67,
    -102,  -170,   -23,  -174,  -174,  -174,  -174,  -174,  -174,  -174,
    -174,  -174,  -174,  -174,  -174,  -174,  -174,  -174,  -597,  -597,
    -597,  -597,  -597,   997,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  1010,  -170,  -597,  -597,  -174,  -597,   179,   180,   181,
     182,   183,   184,   187,   172,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -174,  -174,  -174,
    1214,  -106,  -174,  1214,  -106,  -174,  -174,  1214,  -170,  -174,
    1214,  -170,   -24,  -174,  -170,  -170,  -174,  -170,  1043,  -597,
    -597,  -170,  1214,  -174,  1214,  -174,  -170,  -174,  -174,  -170,
    -174,    -3,  -174,  -170,  -170,  -170,  1214,  -170,  -174,  -174,
     175,  -102,   -26,  -174,  -174,   -18,  -170,  -170,  -174,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -170,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -107,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,   188,  -597,  1056,  -597,  -597,   311,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -174,  -189,   176,  1280,   191,  -174,  -174,  -170,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,   190,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,   194,  -597,  -597,  -597,   195,  -174,  -597,  -597,
    -173,  -597,   197,   198,  -174,  -597,  -597,  -597,  -174,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -170,  -597,  -597,  -597,  -174,  -161,   -19,  -597,  -597,
    -161,  -161,   -80,  -597,  -206,  -597,   189,  -597,  -597,  -174,
    -597,  -597,   199,  -597,  -597,   200,   -21,  -174,  -174,  -597,
     203,   204,  -597,  -597
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -597,   444,  -597,   345,  -597,   -78,  -597,  -597,  -597,  -597,
    -597,   224,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -103,  -597,  -597,   -55,    75,
      77,  -597,  -597,   -14,  -597,   117,   139,  -597,  -597,  -597,
    -596,  -203,  -597,  -128,  -597,  -597,   -11,  -218,   -81
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -314
static const yytype_int16 yytable[] =
{
     121,   417,   215,   124,   550,   450,   158,   460,   153,   413,
     432,   476,   157,   433,   436,   466,   301,   395,   321,   427,
     528,   471,   474,   193,   260,   261,   514,   551,   159,   160,
     638,   384,   529,   643,   640,   641,     1,   530,     3,   451,
       4,   396,   114,   161,   115,   316,   317,   306,   477,   478,
     116,   348,   589,   590,   591,   117,   397,   515,   118,   119,
     120,   572,   573,   574,   575,   416,   628,   384,   461,   122,
     472,   576,   577,   637,   573,   574,   575,   416,   531,   414,
     162,   163,   307,   322,   428,   123,   125,   164,   126,   516,
     154,   127,   165,   166,   167,   168,   169,   128,   467,   170,
     171,   172,   129,   398,   437,   265,   130,   300,   173,   174,
     426,   303,   304,   387,   388,   389,   390,   131,   155,   393,
     156,   392,   187,   175,   176,   308,   188,   572,   573,   574,
     575,   416,   189,   448,   211,   190,   177,   576,   577,   441,
     399,   400,   401,   402,   403,   191,   479,   404,   405,   406,
     407,   408,   409,   563,   452,   192,   194,   470,   195,   213,
     475,   430,   642,   468,   196,   197,   198,   199,   200,   438,
     410,   532,   318,   319,   201,   202,   379,   380,   309,   382,
     383,   203,   204,   205,   206,   310,   453,   391,   207,   434,
     178,   179,   180,   439,   418,   419,   420,   421,   422,   423,
     311,   462,   181,   208,   209,   182,   552,   183,   210,   411,
     442,   443,   212,   214,   446,   447,   184,   312,   455,   456,
     185,   499,   459,   217,   464,   216,   649,   639,   218,   313,
     307,   219,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   220,   221,   497,
     222,   223,   513,   224,   225,   519,   226,   524,   227,   523,
     527,   228,   526,   535,   536,   229,   538,   230,   231,   232,
     540,   233,   234,   308,   541,   545,   543,   235,   548,   236,
     370,   237,   555,   556,   557,   238,   559,   497,   558,   239,
     240,   241,   242,   243,   500,   568,   569,   244,   245,   593,
     246,   247,   248,   249,   250,   571,   251,   252,   253,   254,
     255,   256,   257,   302,   305,   158,   510,   511,   512,   327,
     328,   518,   329,   330,   521,   522,   309,   331,   525,   332,
     333,   334,   534,   310,   497,   537,   335,   159,   160,   336,
     337,   338,   542,   339,   544,   340,   546,   547,   311,   549,
     341,   554,   161,   324,   325,   326,   342,   560,   561,   343,
     344,   345,   565,   566,   346,   312,   347,   570,   351,   352,
     353,   354,   355,   356,   587,   357,   358,   313,   359,   349,
     350,   360,   361,   362,   363,   364,   365,   366,   367,   162,
     163,   368,   369,   371,   372,   373,   164,   374,   375,   376,
     377,   165,   166,   167,   168,   169,   378,   386,   170,   171,
     172,   508,   497,   381,   416,   424,   562,   173,   174,   501,
     502,   503,   504,   505,   506,   394,   624,   507,   584,   621,
     625,   645,   175,   176,  -313,   626,   425,   630,   631,   647,
     648,   431,   652,   653,     2,   177,   259,   444,   445,   564,
     567,   520,   629,   509,     0,   457,   458,     0,     0,     0,
     465,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   588,
       0,     0,     0,     0,   622,   623,     0,     0,     0,   178,
     179,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,     0,   182,     0,   183,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,   185,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     635,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     586,     0,     5,   644,     0,     0,     0,     0,     0,     0,
       0,     6,     0,     0,     0,     0,   627,     0,     0,     7,
       8,     9,     0,   632,     0,     0,    10,   633,     0,    11,
       0,     0,    12,    13,    14,    15,     0,    16,     0,    17,
      18,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    20,    21,     0,    22,    23,    24,     0,
       0,     0,     0,     0,   636,     0,     0,     0,     0,    25,
      26,    27,    28,     0,     0,     1,     0,     0,   646,    29,
      30,     0,     0,    31,    32,     0,   650,   651,     0,    33,
      34,    35,    36,    37,    38,     0,    39,     0,     0,     0,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
       0,    55,    56,    57,    58,    59,    60,     0,     0,     0,
       0,    61,    62,     0,     0,     0,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,    72,    73,     0,     0,     0,
       0,    74,    75,    76,    77,    78,     0,    79,     0,    80,
      81,     0,     0,     0,     0,    82,     0,     0,    83,     0,
      84,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,     5,     0,     0,    94,    95,    96,    97,     0,     0,
       6,     0,     0,   258,     0,    98,    99,     0,     7,     8,
       9,     0,     0,     0,     0,    10,     0,     0,    11,     0,
       0,    12,    13,    14,    15,     0,    16,     0,    17,    18,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    21,     0,    22,    23,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,    26,
      27,    28,     0,     0,     1,     0,     0,     0,    29,    30,
       0,     0,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,     0,    39,     0,     0,     0,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     0,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,     0,     0,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,     0,
      55,    56,    57,    58,    59,    60,     0,     0,     0,     0,
      61,    62,     0,     0,     0,    63,    64,    65,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,    72,    73,     0,     0,     0,     0,
      74,    75,    76,    77,    78,     0,    79,     0,    80,    81,
       0,     0,     0,     0,    82,     0,     0,    83,     0,    84,
       0,    85,    86,    87,    88,    89,    90,    91,    92,    93,
       0,     0,     0,    94,    95,    96,    97,     0,     0,     0,
       0,     0,   132,     0,    98,    99,   133,   134,   135,   136,
     137,   138,   139,   140,     0,   132,     0,     0,     0,   133,
     134,   135,   136,   137,   138,   139,   140,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,     0,     0,   141,     0,   132,     0,
       0,   142,   133,   134,   135,   136,   137,   138,   139,   140,
       0,   132,     0,     0,     0,   133,   134,   135,   136,   137,
     138,   139,   140,     0,     0,     0,   143,     0,     0,   141,
       0,     0,     0,     0,   142,     0,     0,   144,     0,   143,
       0,     0,   141,     0,     0,     0,     0,   142,     0,     0,
     144,     0,     0,   145,   146,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,   146,     0,     0,
       0,     0,   143,     0,     0,     0,     0,     0,     0,     0,
     147,     0,     0,   144,     0,   143,     0,     0,     0,     0,
       0,     0,     0,   147,     0,     0,   144,     0,     0,   145,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   148,     0,     0,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,     0,     0,   147,
       0,     0,     0,   149,     0,     0,     0,     0,     0,   150,
       0,   151,     0,     0,     0,     0,   149,     0,     0,     0,
       0,   152,   150,     0,   151,     0,     0,     0,     0,   148,
     266,     0,     0,     0,   152,     0,     0,     0,     0,     0,
       0,     0,   148,     0,     0,     0,   496,   267,   268,   149,
       0,     0,     0,     0,     0,   150,     0,   151,     0,   498,
       0,     0,   149,     0,     0,     0,     0,   152,   150,     0,
     151,     0,   269,   270,   271,   272,   273,   274,   275,   276,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     132,     0,   539,   594,   133,   134,   135,   136,   137,   138,
     139,   140,     0,     0,   277,   585,     0,   278,     0,     0,
       0,   595,   596,     0,     0,     0,     0,     0,   597,     0,
       0,   141,     0,     0,     0,     0,   142,   598,     0,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   280,   281,   282,
     283,   284,   285,   286,   287,     0,   599,   600,   601,   602,
       0,     0,   288,     0,   143,     0,     0,     0,     0,     0,
       0,     0,   603,     0,     0,   144,     0,   604,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   289,
     290,   145,   146,   605,   291,     0,     0,     0,     0,     0,
       0,   292,     0,   293,     0,     0,     0,   606,   607,     0,
     608,     0,     0,     0,     0,     0,     0,     0,   147,     0,
       0,   609,     0,     0,     0,   610,   611,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   294,     0,     0,
       0,   295,     0,     0,     0,     0,   612,     0,     0,     0,
       0,   296,   297,     0,     0,     0,     0,     0,   120,     0,
       0,   148,     0,     0,     0,     0,     0,     0,   613,     0,
       0,     0,   614,   615,     0,   616,     0,     0,     0,     0,
       0,   149,     0,     0,   617,     0,     0,   150,     0,   151,
       0,     0,     0,     0,     0,     0,     0,   618,     0,   152,
       0,     0,     0,     0,     0,   619
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-597))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      11,   204,    57,    14,     7,    31,     4,    23,    22,    70,
      30,    34,    26,    33,    29,    33,   119,    18,   164,   164,
      44,    88,   240,    34,    73,    74,   132,    30,    26,    27,
     626,   237,    56,   239,   630,   631,    81,    61,   238,    65,
       0,    42,   240,    41,   238,    19,    20,   125,    71,    72,
     240,   154,   241,   242,   243,   240,    57,   163,   240,   238,
     234,   234,   235,   236,   237,   238,   239,   237,    84,   240,
     137,   244,   245,   234,   235,   236,   237,   238,   102,   140,
      78,    79,    58,   229,   229,   240,   240,    85,   238,   195,
     238,   240,    90,    91,    92,    93,    94,   240,   116,    97,
      98,    99,   240,   104,   119,   116,   240,   118,   106,   107,
     213,   122,   123,   194,   195,   196,   197,   240,   240,   200,
     240,   199,   240,   121,   122,   101,   240,   234,   235,   236,
     237,   238,   240,   235,   238,   240,   134,   244,   245,   220,
     141,   142,   143,   144,   145,   240,   169,   148,   149,   150,
     151,   152,   153,   371,   180,   240,   240,   238,   240,   238,
     241,   216,   242,   181,   240,   240,   240,   240,   240,   184,
     171,   195,   146,   147,   240,   240,   187,   188,   154,   190,
     191,   240,   240,   240,   240,   161,   212,   198,   240,   209,
     188,   189,   190,   208,   205,   206,   207,   208,   209,   210,
     176,   217,   200,   240,   240,   203,   209,   205,   240,   210,
     221,   222,   240,   240,   225,   226,   214,   193,   229,   230,
     218,   302,   233,   240,   235,   238,   247,   246,   240,   205,
      58,   240,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   240,   240,   263,
     240,   240,   330,   240,   240,   333,   240,   338,   240,   337,
     341,   240,   340,   344,   345,   240,   347,   240,   240,   240,
     351,   240,   240,   101,   352,   356,   354,   240,   359,   240,
     238,   240,   363,   364,   365,   240,   367,   301,   366,   240,
     240,   240,   240,   240,   305,   376,   377,   240,   240,   502,
     240,   240,   240,   240,   240,   386,   240,   240,   240,   240,
     240,   240,   240,   240,   240,     4,   327,   328,   329,   240,
     240,   332,   240,   240,   335,   336,   154,   240,   339,   240,
     240,   240,   343,   161,   348,   346,   240,    26,    27,   240,
     240,   240,   353,   240,   355,   240,   357,   358,   176,   360,
     240,   362,    41,   129,   130,   131,   240,   368,   369,   240,
     240,   240,   373,   374,   240,   193,   240,   378,   240,   240,
     240,   240,   240,   240,   429,   240,   240,   205,   240,   155,
     156,   240,   240,   240,   240,   240,   240,   240,   240,    78,
      79,   240,   240,   240,   240,   240,    85,   240,   240,   240,
     240,    90,    91,    92,    93,    94,   240,   240,    97,    98,
      99,   239,   426,   189,   238,   241,   241,   106,   107,   240,
     240,   240,   240,   240,   240,   201,   507,   240,   240,   238,
     240,   242,   121,   122,   240,   240,   212,   240,   240,   240,
     240,   217,   239,   239,     0,   134,   101,   223,   224,   372,
     375,   334,   580,   314,    -1,   231,   232,    -1,    -1,    -1,
     236,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   500,
      -1,    -1,    -1,    -1,   505,   506,    -1,    -1,    -1,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   200,    -1,    -1,   203,    -1,   205,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     621,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     239,    -1,     8,   634,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    -1,    -1,   577,    -1,    -1,    25,
      26,    27,    -1,   584,    -1,    -1,    32,   588,    -1,    35,
      -1,    -1,    38,    39,    40,    41,    -1,    43,    -1,    45,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,   625,    -1,    -1,    -1,    -1,    75,
      76,    77,    78,    -1,    -1,    81,    -1,    -1,   639,    85,
      86,    -1,    -1,    89,    90,    -1,   647,   648,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,    -1,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,   121,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,
      -1,   157,   158,   159,   160,   161,   162,    -1,    -1,    -1,
      -1,   167,   168,    -1,    -1,    -1,   172,   173,   174,   175,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,   190,   191,   192,    -1,    -1,    -1,
      -1,   197,   198,   199,   200,   201,    -1,   203,    -1,   205,
     206,    -1,    -1,    -1,    -1,   211,    -1,    -1,   214,    -1,
     216,    -1,   218,   219,   220,   221,   222,   223,   224,   225,
     226,     8,    -1,    -1,   230,   231,   232,   233,    -1,    -1,
      17,    -1,    -1,   239,    -1,   241,   242,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,
      -1,    38,    39,    40,    41,    -1,    43,    -1,    45,    46,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    60,    -1,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    78,    -1,    -1,    81,    -1,    -1,    -1,    85,    86,
      -1,    -1,    89,    90,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,   121,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   155,    -1,
     157,   158,   159,   160,   161,   162,    -1,    -1,    -1,    -1,
     167,   168,    -1,    -1,    -1,   172,   173,   174,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     187,   188,   189,   190,   191,   192,    -1,    -1,    -1,    -1,
     197,   198,   199,   200,   201,    -1,   203,    -1,   205,   206,
      -1,    -1,    -1,    -1,   211,    -1,    -1,   214,    -1,   216,
      -1,   218,   219,   220,   221,   222,   223,   224,   225,   226,
      -1,    -1,    -1,   230,   231,   232,   233,    -1,    -1,    -1,
      -1,    -1,     5,    -1,   241,   242,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,     5,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,     5,    -1,
      -1,    41,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,     5,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    79,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    90,    -1,    79,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      90,    -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,   107,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    90,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    90,    -1,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,   133,
      -1,    -1,    -1,   196,    -1,    -1,    -1,    -1,    -1,   202,
      -1,   204,    -1,    -1,    -1,    -1,   196,    -1,    -1,    -1,
      -1,   214,   202,    -1,   204,    -1,    -1,    -1,    -1,   176,
       6,    -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,   239,    23,    24,   196,
      -1,    -1,    -1,    -1,    -1,   202,    -1,   204,    -1,   239,
      -1,    -1,   196,    -1,    -1,    -1,    -1,   214,   202,    -1,
     204,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
     214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       5,    -1,   239,     3,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    80,   239,    -1,    83,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    -1,    -1,    28,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    41,    37,    -1,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,    66,    67,    68,    69,
      -1,    -1,   138,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    90,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
     166,   106,   107,   103,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,   179,    -1,    -1,    -1,   117,   118,    -1,
     120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,   131,    -1,    -1,    -1,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,
      -1,   217,    -1,    -1,    -1,    -1,   156,    -1,    -1,    -1,
      -1,   227,   228,    -1,    -1,    -1,    -1,    -1,   234,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,   182,   183,    -1,   185,    -1,    -1,    -1,    -1,
      -1,   196,    -1,    -1,   194,    -1,    -1,   202,    -1,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,   214,
      -1,    -1,    -1,    -1,    -1,   215
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    81,   249,   238,     0,     8,    17,    25,    26,    27,
      32,    35,    38,    39,    40,    41,    43,    45,    46,    47,
      59,    60,    62,    63,    64,    75,    76,    77,    78,    85,
      86,    89,    90,    95,    96,    97,    98,    99,   100,   102,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     121,   122,   134,   139,   155,   157,   158,   159,   160,   161,
     162,   167,   168,   172,   173,   174,   175,   186,   187,   188,
     189,   190,   191,   192,   197,   198,   199,   200,   201,   203,
     205,   206,   211,   214,   216,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   230,   231,   232,   233,   241,   242,
     249,   250,   251,   252,   263,   264,   265,   266,   267,   269,
     270,   271,   272,   274,   240,   238,   240,   240,   240,   238,
     234,   294,   240,   240,   294,   240,   238,   240,   240,   240,
     240,   240,     5,     9,    10,    11,    12,    13,    14,    15,
      16,    36,    41,    79,    90,   106,   107,   133,   176,   196,
     202,   204,   214,   281,   238,   240,   240,   281,     4,    26,
      27,    41,    78,    79,    85,    90,    91,    92,    93,    94,
      97,    98,    99,   106,   107,   121,   122,   134,   188,   189,
     190,   200,   203,   205,   214,   218,   276,   240,   240,   240,
     240,   240,   240,   294,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   238,   240,   238,   240,   276,   238,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   239,   251,
      73,    74,   262,   273,   281,   294,     6,    23,    24,    48,
      49,    50,    51,    52,    53,    54,    55,    80,    83,   105,
     123,   124,   125,   126,   127,   128,   129,   130,   138,   165,
     166,   170,   177,   179,   213,   217,   227,   228,   253,   294,
     294,   273,   240,   294,   294,   240,   253,    58,   101,   154,
     161,   176,   193,   205,   275,   284,    19,    20,   146,   147,
     260,   164,   229,   259,   259,   259,   259,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   273,   259,
     259,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     238,   240,   240,   240,   240,   240,   240,   240,   240,   294,
     294,   259,   294,   294,   237,   296,   240,   296,   296,   296,
     296,   294,   253,   296,   259,    18,    42,    57,   104,   141,
     142,   143,   144,   145,   148,   149,   150,   151,   152,   153,
     171,   210,   256,    70,   140,   257,   238,   289,   294,   294,
     294,   294,   294,   294,   241,   259,   273,   164,   229,   268,
     276,   259,    30,    33,   209,   279,    29,   119,   184,   208,
     255,   296,   294,   294,   259,   259,   294,   294,   235,   295,
      31,    65,   180,   212,   278,   294,   294,   259,   259,   294,
      23,    84,   217,   258,   294,   259,    33,   116,   181,   277,
     296,    88,   137,   254,   295,   296,    34,    71,    72,   169,
     261,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   239,   281,   239,   296,
     294,   240,   240,   240,   240,   240,   240,   240,   239,   284,
     294,   294,   294,   253,   132,   163,   195,   283,   294,   253,
     283,   294,   294,   253,   296,   294,   253,   296,    44,    56,
      61,   102,   195,   282,   294,   296,   296,   294,   296,   239,
     296,   253,   294,   253,   294,   296,   294,   294,   296,   294,
       7,    30,   209,   280,   294,   296,   296,   296,   253,   296,
     294,   294,   241,   295,   278,   294,   294,   277,   296,   296,
     294,   296,   234,   235,   236,   237,   244,   245,   288,   289,
     290,   291,   292,   293,   240,   239,   239,   276,   294,   241,
     242,   243,   287,   289,     3,    21,    22,    28,    37,    66,
      67,    68,    69,    82,    87,   103,   117,   118,   120,   131,
     135,   136,   156,   178,   182,   183,   185,   194,   207,   215,
     286,   238,   294,   294,   296,   240,   240,   294,   239,   291,
     240,   240,   294,   294,   285,   296,   294,   234,   288,   246,
     288,   288,   242,   239,   296,   242,   294,   240,   240,   247,
     294,   294,   239,   239
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); Syntax_Tree = (yyval.tree); }
    break;

  case 3:

    {
		 (yyval.tree) = T_graph_entry((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 4:

    {
		 (yyval.tree) = T_graph_entry((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 5:

    {
		 (yyval.tree) = T_graph_attribute((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)]))); }
    break;

  case 6:

    {
		 (yyval.tree) = T_node_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 7:

    {
		 (yyval.tree) = T_edge_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 8:

    {
		 (yyval.tree) = T_foldnode_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));}
    break;

  case 9:

    {
		 (yyval.tree) = T_foldedge_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));}
    break;

  case 10:

    {
		 (yyval.tree) = T_graph((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));           }
    break;

  case 11:

    {
		 (yyval.tree) = T_node((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));            }
    break;

  case 12:

    {
		 (yyval.tree) = T_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));            }
    break;

  case 13:

    {
		 (yyval.tree) = T_near_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));       }
    break;

  case 14:

    {
		 (yyval.tree) = T_bent_near_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 15:

    {
		 (yyval.tree) = T_back_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));       }
    break;

  case 16:

    {
		 (yyval.tree) = T_constraint((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));      }
    break;

  case 17:

    {
		 (yyval.tree) = T_title((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 18:

    {
		 (yyval.tree) = T_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 19:

    {
		 (yyval.tree) = T_info1((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 20:

    {
		 (yyval.tree) = T_info2((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 21:

    {
		 (yyval.tree) = T_info3((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 22:

    {
		 (yyval.tree) = T_color((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 23:

    {
		 (yyval.tree) = T_textcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));          }
    break;

  case 24:

    {
		 (yyval.tree) = T_colorborder((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 25:

    {
		 (yyval.tree) = T_width((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 26:

    {
		 (yyval.tree) = T_height((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 27:

    {
		 (yyval.tree) = T_borderwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 28:

    {
		 (yyval.tree) = T_xdef((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 29:

    {
		 (yyval.tree) = T_ydef((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 30:

    {
		 (yyval.tree) = T_loc((yyvsp[(5) - (9)].tree),(yyvsp[(8) - (9)].tree),yylocate((yylsp[(1) - (9)])));       }
    break;

  case 31:

    {
		 (yyval.tree) = T_folding((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 32:

    {
		 (yyval.tree) = T_scaling((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 33:

    {
		 (yyval.tree) = T_shrink((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 34:

    {
		 (yyval.tree) = T_stretch((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 35:

    {
		 (yyval.tree) = T_textmode((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 36:

    {
		 (yyval.tree) = T_shape((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    	       }
    break;

  case 37:

    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 38:

    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 39:

    {
		 (yyval.tree) = T_horizontal_order((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 40:

    {
		 (yyval.tree) = T_status((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 41:

    {
		 (yyval.tree) = T_xmax((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 42:

    {
		 (yyval.tree) = T_ymax((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 43:

    {
		 (yyval.tree) = T_xbase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 44:

    {
		 (yyval.tree) = T_ybase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 45:

    {
		 (yyval.tree) = T_xspace((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 46:

    {
		 (yyval.tree) = T_xlspace((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 47:

    {
		 (yyval.tree) = T_yspace((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 48:

    {
		 (yyval.tree) = T_xraster((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 49:

    {
		 (yyval.tree) = T_xlraster((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 50:

    {
		 (yyval.tree) = T_yraster((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 51:

    {
		 (yyval.tree) = T_hidden((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 	       }
    break;

  case 52:

    {
		 (yyval.tree) = T_hidden((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 	       }
    break;

  case 53:

    {
		 (yyval.tree) = T_classname((yyvsp[(2) - (4)].tree),(yyvsp[(4) - (4)].tree),yylocate((yylsp[(1) - (4)])));       }
    break;

  case 54:

    {
		 (yyval.tree) = T_infoname((yyvsp[(2) - (4)].tree),(yyvsp[(4) - (4)].tree),yylocate((yylsp[(1) - (4)])));        }
    break;

  case 55:

    {
		 (yyval.tree) = T_colentry((yyvsp[(2) - (6)].tree),(yyvsp[(4) - (6)].tree),(yyvsp[(5) - (6)].tree),(yyvsp[(6) - (6)].tree),yylocate((yylsp[(1) - (6)])));   }
    break;

  case 56:

    {
		 (yyval.tree) = T_layoutalgorithm((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 57:

    {
		 (yyval.tree) = T_layoutfrequency((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 58:

    {
		 (yyval.tree) = T_downfactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 59:

    {
		 (yyval.tree) = T_upfactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 60:

    {
		 (yyval.tree) = T_nearfactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 61:

    {
		 (yyval.tree) = T_splinefactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 62:

    {
		 (yyval.tree) = T_late_edge_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 63:

    {
		 (yyval.tree) = T_display_edge_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 64:

    {
		 (yyval.tree) = T_dirty_edge_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 65:

    {
		 (yyval.tree) = T_finetuning((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 66:

    {
		 (yyval.tree) = T_hidesingles((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 67:

    {
		 (yyval.tree) = T_straightphase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 68:

    {
		 (yyval.tree) = T_priophase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 69:

    {
		 (yyval.tree) = T_manhatten((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 70:

    {
		 (yyval.tree) = T_smanhatten((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 71:

    {
		 (yyval.tree) = T_nonearedges(yylocate((yylsp[(1) - (1)])));	       }
    break;

  case 72:

    {
		 (yyval.tree) = T_nonearedges(yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 73:

    {
		 (yyval.tree) = T_dummy(yylocate((yylsp[(1) - (3)])));	     	       }
    break;

  case 74:

    {
		 (yyval.tree) = T_orientation((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 75:

    {
		 (yyval.tree) = T_node_alignment((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 76:

    {
		 (yyval.tree) = T_port_sharing((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 77:

    {
		 (yyval.tree) = T_arrow_mode((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 78:

    {
		 (yyval.tree) = T_spreadlevel((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 79:

    {
		 (yyval.tree) = T_treefactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 80:

    {
		 (yyval.tree) = T_crossing_phase2((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 81:

    {
		 (yyval.tree) = T_crossing_opt((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 82:

    {
		 (yyval.tree) = T_crossing_weight((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 83:

    {
		 (yyval.tree) = T_view_method((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       	}
    break;

  case 84:

    {
		 (yyval.tree) = T_view_edges((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       	}
    break;

  case 85:

    {
		 (yyval.tree) = T_view_nodes((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       	}
    break;

  case 86:

    {
		 (yyval.tree) = T_view_splines((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  	}
    break;

  case 87:

    {
		 (yyval.tree) = T_bend_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   		}
    break;

  case 88:

    {
		 (yyval.tree) = T_cross_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  		}
    break;

  case 89:

    {
		 (yyval.tree) = T_cross_min((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  		}
    break;

  case 90:

    {
		 (yyval.tree) = T_pendel_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 		}
    break;

  case 91:

    {
		 (yyval.tree) = T_pendel_min((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  	}
    break;

  case 92:

    {
		 (yyval.tree) = T_rubber_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 		}
    break;

  case 93:

    {
		 (yyval.tree) = T_rubber_min((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  	}
    break;

  case 94:

    {
		 (yyval.tree) = T_straight_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 	}
    break;

  case 95:

    {
		 (yyval.tree) = T_typename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 96:

    {
		 (yyval.tree) = T_include((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 97:

    {
		 (yyval.tree) = T_layoutparameter((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 98:

    {
		 (yyval.tree) = T_topsort((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 99:

    {
		 (yyval.tree) = T_inputfunction((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 100:

    {
		 (yyval.tree) = T_outputfunction((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 101:

    {
		 (yyval.tree) = T_xscrollbar((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 102:

    {
		 (yyval.tree) = T_yscrollbar((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 103:

    {
		 (yyval.tree) = T_aquamarine(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 104:

    {
		 (yyval.tree) = T_black(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 105:

    {
		 (yyval.tree) = T_blue(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 106:

    {
		 (yyval.tree) = T_cyan(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 107:

    {
		 (yyval.tree) = T_darkblue(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 108:

    {
		 (yyval.tree) = T_darkcyan(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 109:

    {
		 (yyval.tree) = T_darkgreen(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 110:

    {
		 (yyval.tree) = T_darkgrey(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 111:

    {
		 (yyval.tree) = T_darkmagenta(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 112:

    {
		 (yyval.tree) = T_darkred(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 113:

    {
		 (yyval.tree) = T_darkyellow(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 114:

    {
		 (yyval.tree) = T_gold(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 115:

    {
		 (yyval.tree) = T_green(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 116:

    {
		 (yyval.tree) = T_khaki(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 117:

    {
		 (yyval.tree) = T_lightblue(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 118:

    {
		 (yyval.tree) = T_lightcyan(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 119:

    {
		 (yyval.tree) = T_lightgreen(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 120:

    {
		 (yyval.tree) = T_lightgrey(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 121:

    {
		 (yyval.tree) = T_lightmagenta(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 122:

    {
		 (yyval.tree) = T_lightred(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 123:

    {
		 (yyval.tree) = T_lightyellow(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 124:

    {
		 (yyval.tree) = T_lilac(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 125:

    {
		 (yyval.tree) = T_magenta(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 126:

    {
		 (yyval.tree) = T_orange(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 127:

    {
		 (yyval.tree) = T_orchid(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 128:

    {
		 (yyval.tree) = T_pink(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 129:

    {
		 (yyval.tree) = T_purple(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 130:

    {
		 (yyval.tree) = T_red(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 131:

    {
		 (yyval.tree) = T_turquoise(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 132:

    {
		 (yyval.tree) = T_white(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 133:

    {
		 (yyval.tree) = T_yellow(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 134:

    {
		 (yyval.tree) = T_yellowgreen(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 135:

    {
		 (yyval.tree) = T_colindex((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 136:

    {
		 (yyval.tree) = T_high(yylocate((yylsp[(1) - (1)]))); 	       }
    break;

  case 137:

    {
		 (yyval.tree) = T_low(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 138:

    {
		 (yyval.tree) = T_top_to_bottom(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 139:

    {
		 (yyval.tree) = T_bottom_to_top(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 140:

    {
		 (yyval.tree) = T_left_to_right(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 141:

    {
		 (yyval.tree) = T_right_to_left(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 142:

    {
		 (yyval.tree) = T_barycenter(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 143:

    {
		 (yyval.tree) = T_isi(yylocate((yylsp[(1) - (1)])));           }
    break;

  case 144:

    {
		 (yyval.tree) = T_planar(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 145:

    {
		 (yyval.tree) = T_constaints(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 146:

    {
		 (yyval.tree) = T_tree(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 147:

    {
		 (yyval.tree) = T_maxdepth(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 148:

    {
		 (yyval.tree) = T_mindepth(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 149:

    {
		 (yyval.tree) = T_maxdepthslow(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 150:

    {
		 (yyval.tree) = T_mindepthslow(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 151:

    {
		 (yyval.tree) = T_maxdegree(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 152:

    {
		 (yyval.tree) = T_mindegree(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 153:

    {
		 (yyval.tree) = T_maxindegree(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 154:

    {
		 (yyval.tree) = T_minindegree(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 155:

    {
		 (yyval.tree) = T_maxoutdegree(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 156:

    {
		 (yyval.tree) = T_minoutdegree(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 157:

    {
		 (yyval.tree) = T_minbackwards(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 158:

    {
		 (yyval.tree) = T_depthfirst(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 159:

    {
		 (yyval.tree) = T_every(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 160:

    {
		 (yyval.tree) = T_manual(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 161:

    {
		 (yyval.tree) = T_black(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 162:

    {
		 (yyval.tree) = T_grey(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 163:

    {
		 (yyval.tree) = T_white(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 164:

    {
		 (yyval.tree) = T_yes(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 165:

    {
		 (yyval.tree) = T_no(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 166:

    {
		 (yyval.tree) = T_bary(yylocate((yylsp[(1) - (1)]))); 	       }
    break;

  case 167:

    {
		 (yyval.tree) = T_median(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 168:

    {
		 (yyval.tree) = T_barymedian(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 169:

    {
		 (yyval.tree) = T_medianbary(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 170:

    {
		 (yyval.tree) = T_cfish(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 171:

    {
		 (yyval.tree) = T_fcfish(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 172:

    {
		 (yyval.tree) = T_pfish(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 173:

    {
		 (yyval.tree) = T_fpfish(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 174:

    {
		 (yyval.tree) = T_fixed(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 175:

    {
		 (yyval.tree) = T_free(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 176:

    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 177:

    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 178:

    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 179:

    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 180:

    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 181:

    {
		 (yyval.tree) = T_node_attribute((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 182:

    {
		 (yyval.tree) = T_node_attribute((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 183:

    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 184:

    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 185:

    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 186:

    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 187:

    {
		 (yyval.tree) = T_edge_attribute((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 188:

    {
		 (yyval.tree) = T_edge_attribute((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 189:

    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 190:

    {
		 (yyval.tree) = T_constraint_attribute((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 191:

    {
		 (yyval.tree) = T_constraint_attribute((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 192:

    {
		 (yyval.tree) = T_title((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 193:

    {
		 (yyval.tree) = T_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 194:

    {
		 (yyval.tree) = T_info1((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 195:

    {
		 (yyval.tree) = T_info2((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 196:

    {
		 (yyval.tree) = T_info3((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 197:

    {
		 (yyval.tree) = T_fontname((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 198:

    {
		 (yyval.tree) = T_color((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 199:

    {
		 (yyval.tree) = T_textcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 200:

    {
		 (yyval.tree) = T_bordercolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 201:

    {
		 (yyval.tree) = T_iconfile((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 202:

    {
		 (yyval.tree) = T_anchorpoints((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 203:

    {
		 (yyval.tree) = T_typename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 204:

    {
		 (yyval.tree) = T_width((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 205:

    {
		 (yyval.tree) = T_height((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 206:

    {
		 (yyval.tree) = T_borderwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 207:

    {
		 (yyval.tree) = T_loc((yyvsp[(5) - (9)].tree),(yyvsp[(8) - (9)].tree),yylocate((yylsp[(1) - (9)])));       }
    break;

  case 208:

    {
		 (yyval.tree) = T_folding((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 209:

    {
		 (yyval.tree) = T_scaling((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 210:

    {
		 (yyval.tree) = T_shrink((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 211:

    {
		 (yyval.tree) = T_stretch((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 212:

    {
		 (yyval.tree) = T_iconwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 213:

    {
		 (yyval.tree) = T_iconheight((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 214:

    {
		 (yyval.tree) = T_textmode((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 215:

    {
		 (yyval.tree) = T_iconstyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 216:

    {
		 (yyval.tree) = T_shape((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    	 }
    break;

  case 217:

    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 218:

    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 219:

    {
		 (yyval.tree) = T_horizontal_order((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 220:

    {
		 (yyval.tree) = T_center(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 221:

    {
		 (yyval.tree) = T_left_justify(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 222:

    {
		 (yyval.tree) = T_right_justify(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 223:

    {
		 (yyval.tree) = T_box(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 224:

    {
		 (yyval.tree) = T_rhomb(yylocate((yylsp[(1) - (1)])));		}
    break;

  case 225:

    {
		 (yyval.tree) = T_ellipse(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 226:

    {
		 (yyval.tree) = T_triangle(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 227:

    {
		 (yyval.tree) = T_bottom(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 228:

    {
		 (yyval.tree) = T_top(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 229:

    {
		 (yyval.tree) = T_center(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 230:

    {
		 (yyval.tree) = T_bottom(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 231:

    {
		 (yyval.tree) = T_top(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 232:

    {
		 (yyval.tree) = T_around(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 233:

    {
		 (yyval.tree) = T_sourcename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 234:

    {
		 (yyval.tree) = T_targetname((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 235:

    {
		 (yyval.tree) = T_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 236:

    {
		 (yyval.tree) = T_textcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));          }
    break;

  case 237:

    {
		 (yyval.tree) = T_fontname((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 238:

    {
		 (yyval.tree) = T_color((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 239:

    {
		 (yyval.tree) = T_typename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 240:

    {
		 (yyval.tree) = T_thickness((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 241:

    {
		 (yyval.tree) = T_class((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 242:

    {
		 (yyval.tree) = T_priority((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 243:

    {
		 (yyval.tree) = T_arrowwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 244:

    {
		 (yyval.tree) = T_arrowheight((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 245:

    {
		 (yyval.tree) = T_arrowcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 246:

    {
		 (yyval.tree) = T_barrowcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 247:

    {
		 (yyval.tree) = T_arrowsize((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 248:

    {
		 (yyval.tree) = T_barrowsize((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 249:

    {
		 (yyval.tree) = T_arrowstyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 250:

    {
		 (yyval.tree) = T_barrowstyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 251:

    {
		 (yyval.tree) = T_linestyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 252:

    {
		 (yyval.tree) = T_anchor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 253:

    {
		 (yyval.tree) = T_horizontal_order((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 254:

    {
		 (yyval.tree) = T_continuous(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 255:

    {
		 (yyval.tree) = T_continuous(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 256:

    {
		 (yyval.tree) = T_dotted(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 257:

    {
		 (yyval.tree) = T_dashed(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 258:

    {
		 (yyval.tree) = T_invisible(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 259:

    {
		 (yyval.tree) = T_none(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 260:

    {
		 (yyval.tree) = T_line(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 261:

    {
		 (yyval.tree) = T_solid(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 262:

    {
		 (yyval.tree) = T_title((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 263:

    {
		 (yyval.tree) = T_priority((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 264:

    {
		 (yyval.tree) = T_size((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 265:

    {
		 (yyval.tree) = T_nodes((yyvsp[(4) - (5)].tree),yylocate((yylsp[(1) - (5)])));     }
    break;

  case 266:

    {
		 (yyval.tree) = T_interval((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 267:

    {
		 (yyval.tree) = T_name((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 268:

    {
		 (yyval.tree) = T_dimension((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 269:

    {
		 (yyval.tree) = T_string_array((yyvsp[(1) - (2)].tree),(yyvsp[(2) - (2)].tree),yylocate((yylsp[(1) - (2)])));    }
    break;

  case 270:

    {
		 (yyval.tree) = T_string_array(NULL,(yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 271:

    {
		 (yyval.tree) = T_equal(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 272:

    {
		 (yyval.tree) = T_smaller(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 273:

    {
		 (yyval.tree) = T_greater(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 274:

    {
		 (yyval.tree) = T_neighbors(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 275:

    {
		 (yyval.tree) = T_low_margin(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 276:

    {
		 (yyval.tree) = T_high_margin(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 277:

    {
		 (yyval.tree) = T_xrange(yylocate((yylsp[(1) - (1)])));           }
    break;

  case 278:

    {
		 (yyval.tree) = T_cluster(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 279:

    {
		 (yyval.tree) = T_limit(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 280:

    {
		 (yyval.tree) = T_above(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 281:

    {
		 (yyval.tree) = T_below(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 282:

    {
		 (yyval.tree) = T_left(yylocate((yylsp[(1) - (1)])));             }
    break;

  case 283:

    {
		 (yyval.tree) = T_right(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 284:

    {
		 (yyval.tree) = T_in_font(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 285:

    {
		 (yyval.tree) = T_behind(yylocate((yylsp[(1) - (1)])));           }
    break;

  case 286:

    {
		 (yyval.tree) = T_equal_position(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 287:

    {
		 (yyval.tree) = T_equal_row(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 288:

    {
		 (yyval.tree) = T_equal_column(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 289:

    {
		 (yyval.tree) = T_top_margin(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 290:

    {
		 (yyval.tree) = T_bottom_margin(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 291:

    {
		 (yyval.tree) = T_left_margin(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 292:

    {
		 (yyval.tree) = T_right_margin(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 293:

    {
		 (yyval.tree) = T_upper_neighbor(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 294:

    {
		 (yyval.tree) = T_lower_neighbor(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 295:

    {
		 (yyval.tree) = T_left_neighbor(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 296:

    {
		 (yyval.tree) = T_right_neighbor(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 297:

    {
		 (yyval.tree) = T_x(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 298:

    {
		 (yyval.tree) = T_y(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 299:

    {
		 (yyval.tree) = T_z(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 300:

    {
		 (yyval.tree) = T_integer((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)]))); }
    break;

  case 301:

    {
		 (yyval.tree) = T_float((yyvsp[(1) - (1)].lrealnum),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 302:

    {
		 (yyval.tree) = T_char((yyvsp[(1) - (1)].byte),yylocate((yylsp[(1) - (1)])));    }
    break;

  case 303:

    {
		 (yyval.tree) = T_string((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 304:

    {
		 (yyval.tree) = (yyvsp[(1) - (1)].tree); }
    break;

  case 305:

    {
		 (yyval.tree) = REVERT((yyvsp[(2) - (3)].tree)); }
    break;

  case 306:

    {
		 (yyval.tree) = T_index_value((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 307:

    {
		 (yyval.tree) = T_index_value((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 308:

    {
		 (yyval.tree) = (yyvsp[(1) - (1)].tree);           }
    break;

  case 309:

    {
		 (yyval.tree) = T_index((yyvsp[(1) - (3)].tree),(yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 310:

    {
		 (yyval.tree) = T_range((yyvsp[(1) - (3)].tree),(yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 311:

    {
		 (yyval.tree) = T_stern((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 312:

    {
		 (yyval.tree) = T_range((yyvsp[(2) - (5)].tree),(yyvsp[(4) - (5)].tree),yylocate((yylsp[(1) - (5)]))); }
    break;

  case 313:

    {
		 (yyval.tree) = T_index_val((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));    }
    break;

  case 314:

    {
		 (yyval.tree) = T_integer((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 315:

    {
		 (yyval.tree) = T_float((yyvsp[(1) - (1)].lrealnum),yylocate((yylsp[(1) - (1)])));    }
    break;

  case 316:

    {
		 (yyval.tree) = T_string((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));   }
    break;



      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}








/*----------------------------*/
/* Main entry point of parser */
/*----------------------------*/

/*   returns the number of detected errors and binds the syntaxtree
 *   to the variable Syntax_Tree.
 */
int parse(void)
{
	nr_errors=0;
	debugmessage("init_lex()\n", "");
	init_lex();
	line_nr = 1;
	pos_nr = 0;
	debugmessage("yyparse()\n", "");
	yyparse();

	return (nr_errors);
} /* parse */
 




