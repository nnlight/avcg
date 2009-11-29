
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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

/* Line 1676 of yacc.c  */
#line 1922 "grammar.y"
 
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



/* Line 1676 of yacc.c  */
#line 542 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

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

extern YYLTYPE yylloc;
