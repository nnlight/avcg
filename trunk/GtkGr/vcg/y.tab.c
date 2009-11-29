
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.4.1"

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

/* Line 189 of yacc.c  */
#line 1 "grammar.y"



/*--------------------------------------------------------------------*/
/*            A bison parser specification                            */
/*--------------------------------------------------------------------*/


/*--------------------------------------------------------------------*/
/* Prototypes and Externals					      */
/*--------------------------------------------------------------------*/


extern int line_nr;
extern int pos_nr;

/* Error messages */

int nr_errors;

#define __yy_bcopy bcopy

#define YYDEBUG 1
 
/*--------------------------------------------------------------------*/
/* Tokens from scanner                                                */ 
/*--------------------------------------------------------------------*/



/* Line 189 of yacc.c  */
#line 269 "grammar.y"




/*--------------------------------------------------------------------*/
/* Includes                                                           */ 
/*--------------------------------------------------------------------*/

#define Y_TAB_H
#include <string.h>

#ifndef YYLTYPE
typedef struct yyltype {
		int timestamp;
		int first_line;
		int first_column;
		int last_line;
		int last_column;
		char *text;
	}  yyltype;

#define YYLTYPE yyltype
#endif



typedef struct stree_node *syntaxtree;
#undef yysyntaxtree
#define yysyntaxtree syntaxtree

#define BISONGEN
#undef  YACCGEN

#define YYVALGLOBAL



#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "globals.h"
#include "main.h"
#include "options.h"

#ifndef YY_CHAR
#define YY_CHAR char
extern YY_CHAR *yytext;
#endif

/* the following is added for flex 2.4.6 */

#ifndef YY_MALLOC_DECL
#define YY_MALLOC_DECL
#include <malloc.h>
#endif
 
#undef YYVALGLOBAL
 
extern yysyntaxtree Syntax_Tree;

#define exit(a) gs_exit(a)
extern void gs_exit            _PP((int x));
 
void   line_directive _PP((char *text));
void   escape_transl  _PP((char *text));
void   syntaxerror    _PP((int line,int pos,char *mesge));
void   warning        _PP((int line,int pos,char *mesge));
 
#ifndef yysyntaxerror
#define yysyntaxerror(l,p,m) syntaxerror(l,p,m)
#endif
 
#ifndef yyerror
#define yyerror(x) { \
        SPRINTF(message,"unexpected %s \"%s\" (%s)", \
                 ((yychar<0)?"(?)":yytokname[YYTRANSLATE(yychar)]),     \
                 (strlen(yytext)<48?yytext:"(?)"),x); \
        syntaxerror(line_nr,pos_nr,message);\
    }
#endif
 
/* Memory Management */

#ifdef MEMBLOCKSIZE
#define PARSEBLOCKSIZE (MEMBLOCKSIZE/sizeof(struct stree_node)+1)
#endif

/* Hash Table */

#define hash_size 22079
 


#ifndef NULL
#define NULL 0
#endif

#ifndef REVERT
#define REVERT(x) Revert(x)
#endif

#ifndef COPY
#define COPY(x) Copy(x)
#endif



#ifndef STDPARSER
#define STDPARSER

/* $Id: stdph.skel,v 1.8 1994/01/20 23:44:31 sander Exp sander $ */

#undef  PARSEGENSTD
#define PARSEGENSTD

/*--------------------------------------------------------------------*/
/*  Standard Tree Construction Interface   			      */
/*--------------------------------------------------------------------*/

#ifndef ALIGN
#define ALIGN 8
#define IALIGN (ALIGN-1)
#endif
#ifndef PARSEBLOCKSIZE
#define PARSEBLOCKSIZE 10000
#endif

/*-------------------*/
/* syntax tree nodes */
/*-------------------*/

union  special {
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
};

struct stree_node {
        int  tag_field;
        int  first_line;
        int  first_column;
        int  last_line;
        int  last_column;
#ifdef USERFTYPE
	USERFTYPE user_field;
#endif
        struct stree_node *father;
        union  special     contents;
        struct stree_node *xson[1];
};


/* typedef struct stree_node *syntaxtree; */


#undef yysyntaxtree
#define yysyntaxtree syntaxtree 


#define tag(x)           ((x)->tag_field)
#define nr_of_sons(x)    (ConstructorArity((x)->tag_field))
#define xfirst_line(x)    ((x)->first_line)
#define xfirst_column(x)  ((x)->first_column)
#define xlast_line(x)     ((x)->last_line)
#define xlast_column(x)   ((x)->last_column)
#define xfather(x)        ((x)->father)

#ifdef USERFTYPE
#define	user_field(x)     ((x)->user_field)
#endif

#define get_byte(x)      ((x)->contents.byte)
#define get_snum(x)      ((x)->contents.snum)
#define get_usnum(x)     ((x)->contents.usnum)
#define get_num(x)       ((x)->contents.num)
#define get_unum(x)      ((x)->contents.unum)
#define get_lnum(x)      ((x)->contents.lnum)
#define get_ulnum(x)     ((x)->contents.ulnum)
#define get_realnum(x)   ((x)->contents.realnum)
#define get_lrealnum(x)  ((x)->contents.lrealnum)
#define get_string(x)    ((x)->contents.string)

#define son1(x)    ((x)->xson[0])
#define son2(x)    ((x)->xson[1])
#define son3(x)    ((x)->xson[2])
#define son4(x)    ((x)->xson[3])
#define son5(x)    ((x)->xson[4])
#define son6(x)    ((x)->xson[5])
#define son7(x)    ((x)->xson[6])
#define son8(x)    ((x)->xson[7])
#define son9(x)    ((x)->xson[8])
#define son(x,i)   ((x)->xson[i-1])

#ifndef Y_TAB_H


#include "y.tab.h"


#define Y_TAB_H
#endif /* Y_TAB_H */


/*------------*/ 
/* Prototypes */ 
/*------------*/ 


char * ParseMalloc(int x);
void ParseFree(void);

union special *UnionByte(unsigned char x);
union special *UnionSnum(short int x);
union special *UnionUsnum(unsigned short int x);
union special *UnionNum(int x);
union special *UnionUnum(unsigned int x);
union special *UnionLnum(long int x);
union special *UnionUlnum(unsigned long int x);
union special *UnionRealnum(float x);
union special *UnionLrealnum(double x);
union special *UnionString(char *x);

syntaxtree BuildCont(int mtag,union special *x,YYLTYPE *l);
yysyntaxtree BuildTree(int mtag,int len,union special *x,YYLTYPE *l, ...);

syntaxtree Copy(syntaxtree x);
syntaxtree Revert(syntaxtree list);

const char *ConstructorName(int i);
int   ConstructorArity(int i);

 
#undef  yyparseinit
#define yyparseinit() /**/ 

#endif /* STDPARSER */

/*-- end of standard tree construction interface ---------------------*/




/*--------------------------------------------------------------------*/
/* Static part                                                        */
/*--------------------------------------------------------------------*/

/* Static Global Variables */
/*-------------------------*/


/* Constructors: */

#define T_Co_index_val  0
#define T_Co_stern  1
#define T_Co_range  2
#define T_Co_index  3
#define T_Co_index_value  4
#define T_Co_string  5
#define T_Co_char  6
#define T_Co_float  7
#define T_Co_integer  8
#define T_Co_z  9
#define T_Co_y  10
#define T_Co_x  11
#define T_Co_right_neighbor  12
#define T_Co_left_neighbor  13
#define T_Co_lower_neighbor  14
#define T_Co_upper_neighbor  15
#define T_Co_right_margin  16
#define T_Co_left_margin  17
#define T_Co_bottom_margin  18
#define T_Co_top_margin  19
#define T_Co_equal_column  20
#define T_Co_equal_row  21
#define T_Co_equal_position  22
#define T_Co_behind  23
#define T_Co_in_font  24
#define T_Co_right  25
#define T_Co_left  26
#define T_Co_below  27
#define T_Co_above  28
#define T_Co_limit  29
#define T_Co_cluster  30
#define T_Co_xrange  31
#define T_Co_high_margin  32
#define T_Co_low_margin  33
#define T_Co_neighbors  34
#define T_Co_greater  35
#define T_Co_smaller  36
#define T_Co_equal  37
#define T_Co_string_array  38
#define T_Co_dimension  39
#define T_Co_name  40
#define T_Co_interval  41
#define T_Co_nodes  42
#define T_Co_size  43
#define T_Co_solid  44
#define T_Co_line  45
#define T_Co_none  46
#define T_Co_invisible  47
#define T_Co_dashed  48
#define T_Co_dotted  49
#define T_Co_continuous  50
#define T_Co_anchor  51
#define T_Co_linestyle  52
#define T_Co_barrowstyle  53
#define T_Co_arrowstyle  54
#define T_Co_barrowsize  55
#define T_Co_arrowsize  56
#define T_Co_barrowcolor  57
#define T_Co_arrowcolor  58
#define T_Co_arrowheight  59
#define T_Co_arrowwidth  60
#define T_Co_priority  61
#define T_Co_class  62
#define T_Co_thickness  63
#define T_Co_targetname  64
#define T_Co_sourcename  65
#define T_Co_around  66
#define T_Co_top  67
#define T_Co_bottom  68
#define T_Co_triangle  69
#define T_Co_ellipse  70
#define T_Co_rhomb  71
#define T_Co_box  72
#define T_Co_right_justify  73
#define T_Co_left_justify  74
#define T_Co_center  75
#define T_Co_iconstyle  76
#define T_Co_iconheight  77
#define T_Co_iconwidth  78
#define T_Co_anchorpoints  79
#define T_Co_iconfile  80
#define T_Co_bordercolor  81
#define T_Co_fontname  82
#define T_Co_constraint_attribute  83
#define T_Co_edge_attribute  84
#define T_Co_node_attribute  85
#define T_Co_free  86
#define T_Co_fixed  87
#define T_Co_fpfish  88
#define T_Co_pfish  89
#define T_Co_fcfish  90
#define T_Co_cfish  91
#define T_Co_medianbary  92
#define T_Co_barymedian  93
#define T_Co_median  94
#define T_Co_bary  95
#define T_Co_no  96
#define T_Co_yes  97
#define T_Co_grey  98
#define T_Co_manual  99
#define T_Co_every  100
#define T_Co_depthfirst  101
#define T_Co_minbackwards  102
#define T_Co_minoutdegree  103
#define T_Co_maxoutdegree  104
#define T_Co_minindegree  105
#define T_Co_maxindegree  106
#define T_Co_mindegree  107
#define T_Co_maxdegree  108
#define T_Co_mindepthslow  109
#define T_Co_maxdepthslow  110
#define T_Co_mindepth  111
#define T_Co_maxdepth  112
#define T_Co_tree  113
#define T_Co_constaints  114
#define T_Co_planar  115
#define T_Co_isi  116
#define T_Co_barycenter  117
#define T_Co_right_to_left  118
#define T_Co_left_to_right  119
#define T_Co_bottom_to_top  120
#define T_Co_top_to_bottom  121
#define T_Co_low  122
#define T_Co_high  123
#define T_Co_colindex  124
#define T_Co_yellowgreen  125
#define T_Co_yellow  126
#define T_Co_white  127
#define T_Co_turquoise  128
#define T_Co_red  129
#define T_Co_purple  130
#define T_Co_pink  131
#define T_Co_orchid  132
#define T_Co_orange  133
#define T_Co_magenta  134
#define T_Co_lilac  135
#define T_Co_lightyellow  136
#define T_Co_lightred  137
#define T_Co_lightmagenta  138
#define T_Co_lightgrey  139
#define T_Co_lightgreen  140
#define T_Co_lightcyan  141
#define T_Co_lightblue  142
#define T_Co_khaki  143
#define T_Co_green  144
#define T_Co_gold  145
#define T_Co_darkyellow  146
#define T_Co_darkred  147
#define T_Co_darkmagenta  148
#define T_Co_darkgrey  149
#define T_Co_darkgreen  150
#define T_Co_darkcyan  151
#define T_Co_darkblue  152
#define T_Co_cyan  153
#define T_Co_blue  154
#define T_Co_black  155
#define T_Co_aquamarine  156
#define T_Co_yscrollbar  157
#define T_Co_xscrollbar  158
#define T_Co_outputfunction  159
#define T_Co_inputfunction  160
#define T_Co_topsort  161
#define T_Co_layoutparameter  162
#define T_Co_include  163
#define T_Co_typename  164
#define T_Co_straight_max  165
#define T_Co_rubber_min  166
#define T_Co_rubber_max  167
#define T_Co_pendel_min  168
#define T_Co_pendel_max  169
#define T_Co_cross_min  170
#define T_Co_cross_max  171
#define T_Co_bend_max  172
#define T_Co_view_splines  173
#define T_Co_view_nodes  174
#define T_Co_view_edges  175
#define T_Co_view_method  176
#define T_Co_crossing_weight  177
#define T_Co_crossing_opt  178
#define T_Co_crossing_phase2  179
#define T_Co_treefactor  180
#define T_Co_spreadlevel  181
#define T_Co_arrow_mode  182
#define T_Co_port_sharing  183
#define T_Co_node_alignment  184
#define T_Co_orientation  185
#define T_Co_dummy  186
#define T_Co_nonearedges  187
#define T_Co_smanhatten  188
#define T_Co_manhatten  189
#define T_Co_priophase  190
#define T_Co_straightphase  191
#define T_Co_hidesingles  192
#define T_Co_finetuning  193
#define T_Co_dirty_edge_label  194
#define T_Co_display_edge_label  195
#define T_Co_late_edge_label  196
#define T_Co_splinefactor  197
#define T_Co_nearfactor  198
#define T_Co_upfactor  199
#define T_Co_downfactor  200
#define T_Co_layoutfrequency  201
#define T_Co_layoutalgorithm  202
#define T_Co_colentry  203
#define T_Co_infoname  204
#define T_Co_classname  205
#define T_Co_hidden  206
#define T_Co_yraster  207
#define T_Co_xlraster  208
#define T_Co_xraster  209
#define T_Co_yspace  210
#define T_Co_xlspace  211
#define T_Co_xspace  212
#define T_Co_ybase  213
#define T_Co_xbase  214
#define T_Co_ymax  215
#define T_Co_xmax  216
#define T_Co_status  217
#define T_Co_horizontal_order  218
#define T_Co_level  219
#define T_Co_shape  220
#define T_Co_textmode  221
#define T_Co_stretch  222
#define T_Co_shrink  223
#define T_Co_scaling  224
#define T_Co_folding  225
#define T_Co_loc  226
#define T_Co_ydef  227
#define T_Co_xdef  228
#define T_Co_borderwidth  229
#define T_Co_height  230
#define T_Co_width  231
#define T_Co_colorborder  232
#define T_Co_textcolor  233
#define T_Co_color  234
#define T_Co_info3  235
#define T_Co_info2  236
#define T_Co_info1  237
#define T_Co_label  238
#define T_Co_title  239
#define T_Co_constraint  240
#define T_Co_back_edge  241
#define T_Co_bent_near_edge  242
#define T_Co_near_edge  243
#define T_Co_edge  244
#define T_Co_node  245
#define T_Co_graph  246
#define T_Co_foldedge_defaults  247
#define T_Co_foldnode_defaults  248
#define T_Co_edge_defaults  249
#define T_Co_node_defaults  250
#define T_Co_graph_attribute  251
#define T_Co_graph_entry  252

/* Constructor Names */

static const char * const yyconst_name[] = {
	"T_Co_index_val",
	"T_Co_stern",
	"T_Co_range",
	"T_Co_index",
	"T_Co_index_value",
	"T_Co_string",
	"T_Co_char",
	"T_Co_float",
	"T_Co_integer",
	"T_Co_z",
	"T_Co_y",
	"T_Co_x",
	"T_Co_right_neighbor",
	"T_Co_left_neighbor",
	"T_Co_lower_neighbor",
	"T_Co_upper_neighbor",
	"T_Co_right_margin",
	"T_Co_left_margin",
	"T_Co_bottom_margin",
	"T_Co_top_margin",
	"T_Co_equal_column",
	"T_Co_equal_row",
	"T_Co_equal_position",
	"T_Co_behind",
	"T_Co_in_font",
	"T_Co_right",
	"T_Co_left",
	"T_Co_below",
	"T_Co_above",
	"T_Co_limit",
	"T_Co_cluster",
	"T_Co_xrange",
	"T_Co_high_margin",
	"T_Co_low_margin",
	"T_Co_neighbors",
	"T_Co_greater",
	"T_Co_smaller",
	"T_Co_equal",
	"T_Co_string_array",
	"T_Co_dimension",
	"T_Co_name",
	"T_Co_interval",
	"T_Co_nodes",
	"T_Co_size",
	"T_Co_solid",
	"T_Co_line",
	"T_Co_none",
	"T_Co_invisible",
	"T_Co_dashed",
	"T_Co_dotted",
	"T_Co_continuous",
	"T_Co_anchor",
	"T_Co_linestyle",
	"T_Co_barrowstyle",
	"T_Co_arrowstyle",
	"T_Co_barrowsize",
	"T_Co_arrowsize",
	"T_Co_barrowcolor",
	"T_Co_arrowcolor",
	"T_Co_arrowheight",
	"T_Co_arrowwidth",
	"T_Co_priority",
	"T_Co_class",
	"T_Co_thickness",
	"T_Co_targetname",
	"T_Co_sourcename",
	"T_Co_around",
	"T_Co_top",
	"T_Co_bottom",
	"T_Co_triangle",
	"T_Co_ellipse",
	"T_Co_rhomb",
	"T_Co_box",
	"T_Co_right_justify",
	"T_Co_left_justify",
	"T_Co_center",
	"T_Co_iconstyle",
	"T_Co_iconheight",
	"T_Co_iconwidth",
	"T_Co_anchorpoints",
	"T_Co_iconfile",
	"T_Co_bordercolor",
	"T_Co_fontname",
	"T_Co_constraint_attribute",
	"T_Co_edge_attribute",
	"T_Co_node_attribute",
	"T_Co_free",
	"T_Co_fixed",
	"T_Co_fpfish",
	"T_Co_pfish",
	"T_Co_fcfish",
	"T_Co_cfish",
	"T_Co_medianbary",
	"T_Co_barymedian",
	"T_Co_median",
	"T_Co_bary",
	"T_Co_no",
	"T_Co_yes",
	"T_Co_grey",
	"T_Co_manual",
	"T_Co_every",
	"T_Co_depthfirst",
	"T_Co_minbackwards",
	"T_Co_minoutdegree",
	"T_Co_maxoutdegree",
	"T_Co_minindegree",
	"T_Co_maxindegree",
	"T_Co_mindegree",
	"T_Co_maxdegree",
	"T_Co_mindepthslow",
	"T_Co_maxdepthslow",
	"T_Co_mindepth",
	"T_Co_maxdepth",
	"T_Co_tree",
	"T_Co_constaints",
	"T_Co_planar",
	"T_Co_isi",
	"T_Co_barycenter",
	"T_Co_right_to_left",
	"T_Co_left_to_right",
	"T_Co_bottom_to_top",
	"T_Co_top_to_bottom",
	"T_Co_low",
	"T_Co_high",
	"T_Co_colindex",
	"T_Co_yellowgreen",
	"T_Co_yellow",
	"T_Co_white",
	"T_Co_turquoise",
	"T_Co_red",
	"T_Co_purple",
	"T_Co_pink",
	"T_Co_orchid",
	"T_Co_orange",
	"T_Co_magenta",
	"T_Co_lilac",
	"T_Co_lightyellow",
	"T_Co_lightred",
	"T_Co_lightmagenta",
	"T_Co_lightgrey",
	"T_Co_lightgreen",
	"T_Co_lightcyan",
	"T_Co_lightblue",
	"T_Co_khaki",
	"T_Co_green",
	"T_Co_gold",
	"T_Co_darkyellow",
	"T_Co_darkred",
	"T_Co_darkmagenta",
	"T_Co_darkgrey",
	"T_Co_darkgreen",
	"T_Co_darkcyan",
	"T_Co_darkblue",
	"T_Co_cyan",
	"T_Co_blue",
	"T_Co_black",
	"T_Co_aquamarine",
	"T_Co_yscrollbar",
	"T_Co_xscrollbar",
	"T_Co_outputfunction",
	"T_Co_inputfunction",
	"T_Co_topsort",
	"T_Co_layoutparameter",
	"T_Co_include",
	"T_Co_typename",
	"T_Co_straight_max",
	"T_Co_rubber_min",
	"T_Co_rubber_max",
	"T_Co_pendel_min",
	"T_Co_pendel_max",
	"T_Co_cross_min",
	"T_Co_cross_max",
	"T_Co_bend_max",
	"T_Co_view_splines",
	"T_Co_view_nodes",
	"T_Co_view_edges",
	"T_Co_view_method",
	"T_Co_crossing_weight",
	"T_Co_crossing_opt",
	"T_Co_crossing_phase2",
	"T_Co_treefactor",
	"T_Co_spreadlevel",
	"T_Co_arrow_mode",
	"T_Co_port_sharing",
	"T_Co_node_alignment",
	"T_Co_orientation",
	"T_Co_dummy",
	"T_Co_nonearedges",
	"T_Co_smanhatten",
	"T_Co_manhatten",
	"T_Co_priophase",
	"T_Co_straightphase",
	"T_Co_hidesingles",
	"T_Co_finetuning",
	"T_Co_dirty_edge_label",
	"T_Co_display_edge_label",
	"T_Co_late_edge_label",
	"T_Co_splinefactor",
	"T_Co_nearfactor",
	"T_Co_upfactor",
	"T_Co_downfactor",
	"T_Co_layoutfrequency",
	"T_Co_layoutalgorithm",
	"T_Co_colentry",
	"T_Co_infoname",
	"T_Co_classname",
	"T_Co_hidden",
	"T_Co_yraster",
	"T_Co_xlraster",
	"T_Co_xraster",
	"T_Co_yspace",
	"T_Co_xlspace",
	"T_Co_xspace",
	"T_Co_ybase",
	"T_Co_xbase",
	"T_Co_ymax",
	"T_Co_xmax",
	"T_Co_status",
	"T_Co_horizontal_order",
	"T_Co_level",
	"T_Co_shape",
	"T_Co_textmode",
	"T_Co_stretch",
	"T_Co_shrink",
	"T_Co_scaling",
	"T_Co_folding",
	"T_Co_loc",
	"T_Co_ydef",
	"T_Co_xdef",
	"T_Co_borderwidth",
	"T_Co_height",
	"T_Co_width",
	"T_Co_colorborder",
	"T_Co_textcolor",
	"T_Co_color",
	"T_Co_info3",
	"T_Co_info2",
	"T_Co_info1",
	"T_Co_label",
	"T_Co_title",
	"T_Co_constraint",
	"T_Co_back_edge",
	"T_Co_bent_near_edge",
	"T_Co_near_edge",
	"T_Co_edge",
	"T_Co_node",
	"T_Co_graph",
	"T_Co_foldedge_defaults",
	"T_Co_foldnode_defaults",
	"T_Co_edge_defaults",
	"T_Co_node_defaults",
	"T_Co_graph_attribute",
	"T_Co_graph_entry",
	""
};

/* Constructor Arities */

static const int yyconst_arity[] = {
	0   , /* index_val */
	1   , /* stern */
	2   , /* range */
	2   , /* index */
	2   , /* index_value */
	0   , /* string */
	0   , /* char */
	0   , /* float */
	0   , /* integer */
	0   , /* z */
	0   , /* y */
	0   , /* x */
	0   , /* right_neighbor */
	0   , /* left_neighbor */
	0   , /* lower_neighbor */
	0   , /* upper_neighbor */
	0   , /* right_margin */
	0   , /* left_margin */
	0   , /* bottom_margin */
	0   , /* top_margin */
	0   , /* equal_column */
	0   , /* equal_row */
	0   , /* equal_position */
	0   , /* behind */
	0   , /* in_font */
	0   , /* right */
	0   , /* left */
	0   , /* below */
	0   , /* above */
	0   , /* limit */
	0   , /* cluster */
	0   , /* xrange */
	0   , /* high_margin */
	0   , /* low_margin */
	0   , /* neighbors */
	0   , /* greater */
	0   , /* smaller */
	0   , /* equal */
	2   , /* string_array */
	1   , /* dimension */
	1   , /* name */
	1   , /* interval */
	1   , /* nodes */
	1   , /* size */
	0   , /* solid */
	0   , /* line */
	0   , /* none */
	0   , /* invisible */
	0   , /* dashed */
	0   , /* dotted */
	0   , /* continuous */
	1   , /* anchor */
	1   , /* linestyle */
	1   , /* barrowstyle */
	1   , /* arrowstyle */
	1   , /* barrowsize */
	1   , /* arrowsize */
	1   , /* barrowcolor */
	1   , /* arrowcolor */
	1   , /* arrowheight */
	1   , /* arrowwidth */
	1   , /* priority */
	1   , /* class */
	1   , /* thickness */
	1   , /* targetname */
	1   , /* sourcename */
	0   , /* around */
	0   , /* top */
	0   , /* bottom */
	0   , /* triangle */
	0   , /* ellipse */
	0   , /* rhomb */
	0   , /* box */
	0   , /* right_justify */
	0   , /* left_justify */
	0   , /* center */
	1   , /* iconstyle */
	1   , /* iconheight */
	1   , /* iconwidth */
	1   , /* anchorpoints */
	1   , /* iconfile */
	1   , /* bordercolor */
	1   , /* fontname */
	2   , /* constraint_attribute */
	2   , /* edge_attribute */
	2   , /* node_attribute */
	0   , /* free */
	0   , /* fixed */
	0   , /* fpfish */
	0   , /* pfish */
	0   , /* fcfish */
	0   , /* cfish */
	0   , /* medianbary */
	0   , /* barymedian */
	0   , /* median */
	0   , /* bary */
	0   , /* no */
	0   , /* yes */
	0   , /* grey */
	0   , /* manual */
	0   , /* every */
	0   , /* depthfirst */
	0   , /* minbackwards */
	0   , /* minoutdegree */
	0   , /* maxoutdegree */
	0   , /* minindegree */
	0   , /* maxindegree */
	0   , /* mindegree */
	0   , /* maxdegree */
	0   , /* mindepthslow */
	0   , /* maxdepthslow */
	0   , /* mindepth */
	0   , /* maxdepth */
	0   , /* tree */
	0   , /* constaints */
	0   , /* planar */
	0   , /* isi */
	0   , /* barycenter */
	0   , /* right_to_left */
	0   , /* left_to_right */
	0   , /* bottom_to_top */
	0   , /* top_to_bottom */
	0   , /* low */
	0   , /* high */
	1   , /* colindex */
	0   , /* yellowgreen */
	0   , /* yellow */
	0   , /* white */
	0   , /* turquoise */
	0   , /* red */
	0   , /* purple */
	0   , /* pink */
	0   , /* orchid */
	0   , /* orange */
	0   , /* magenta */
	0   , /* lilac */
	0   , /* lightyellow */
	0   , /* lightred */
	0   , /* lightmagenta */
	0   , /* lightgrey */
	0   , /* lightgreen */
	0   , /* lightcyan */
	0   , /* lightblue */
	0   , /* khaki */
	0   , /* green */
	0   , /* gold */
	0   , /* darkyellow */
	0   , /* darkred */
	0   , /* darkmagenta */
	0   , /* darkgrey */
	0   , /* darkgreen */
	0   , /* darkcyan */
	0   , /* darkblue */
	0   , /* cyan */
	0   , /* blue */
	0   , /* black */
	0   , /* aquamarine */
	1   , /* yscrollbar */
	1   , /* xscrollbar */
	1   , /* outputfunction */
	1   , /* inputfunction */
	1   , /* topsort */
	1   , /* layoutparameter */
	1   , /* include */
	1   , /* typename */
	1   , /* straight_max */
	1   , /* rubber_min */
	1   , /* rubber_max */
	1   , /* pendel_min */
	1   , /* pendel_max */
	1   , /* cross_min */
	1   , /* cross_max */
	1   , /* bend_max */
	1   , /* view_splines */
	1   , /* view_nodes */
	1   , /* view_edges */
	1   , /* view_method */
	1   , /* crossing_weight */
	1   , /* crossing_opt */
	1   , /* crossing_phase2 */
	1   , /* treefactor */
	1   , /* spreadlevel */
	1   , /* arrow_mode */
	1   , /* port_sharing */
	1   , /* node_alignment */
	1   , /* orientation */
	0   , /* dummy */
	0   , /* nonearedges */
	1   , /* smanhatten */
	1   , /* manhatten */
	1   , /* priophase */
	1   , /* straightphase */
	1   , /* hidesingles */
	1   , /* finetuning */
	1   , /* dirty_edge_label */
	1   , /* display_edge_label */
	1   , /* late_edge_label */
	1   , /* splinefactor */
	1   , /* nearfactor */
	1   , /* upfactor */
	1   , /* downfactor */
	1   , /* layoutfrequency */
	1   , /* layoutalgorithm */
	4   , /* colentry */
	2   , /* infoname */
	2   , /* classname */
	1   , /* hidden */
	1   , /* yraster */
	1   , /* xlraster */
	1   , /* xraster */
	1   , /* yspace */
	1   , /* xlspace */
	1   , /* xspace */
	1   , /* ybase */
	1   , /* xbase */
	1   , /* ymax */
	1   , /* xmax */
	1   , /* status */
	1   , /* horizontal_order */
	1   , /* level */
	1   , /* shape */
	1   , /* textmode */
	1   , /* stretch */
	1   , /* shrink */
	1   , /* scaling */
	1   , /* folding */
	2   , /* loc */
	1   , /* ydef */
	1   , /* xdef */
	1   , /* borderwidth */
	1   , /* height */
	1   , /* width */
	1   , /* colorborder */
	1   , /* textcolor */
	1   , /* color */
	1   , /* info3 */
	1   , /* info2 */
	1   , /* info1 */
	1   , /* label */
	1   , /* title */
	1   , /* constraint */
	1   , /* back_edge */
	1   , /* bent_near_edge */
	1   , /* near_edge */
	1   , /* edge */
	1   , /* node */
	1   , /* graph */
	1   , /* foldedge_defaults */
	1   , /* foldnode_defaults */
	1   , /* edge_defaults */
	1   , /* node_defaults */
	1   , /* graph_attribute */
	2   , /* graph_entry */
	0
};

/* Build Macros */

#define T_index_val(s0,l) BuildCont(T_Co_index_val,UnionLnum(s0),l)
#define T_stern(s0,l) BuildTree(T_Co_stern,1,UnionNum(0),l,s0)
#define T_range(s0,s1,l) BuildTree(T_Co_range,2,UnionNum(0),l,s0,s1)
#define T_index(s0,s1,l) BuildTree(T_Co_index,2,UnionNum(0),l,s0,s1)
#define T_index_value(s0,s1,l) BuildTree(T_Co_index_value,2,UnionNum(0),l,s0,s1)
#define T_string(s0,l) BuildCont(T_Co_string,UnionLnum(s0),l)
#define T_char(s0,l) BuildCont(T_Co_char,UnionByte(s0),l)
#define T_float(s0,l) BuildCont(T_Co_float,UnionLrealnum(s0),l)
#define T_integer(s0,l) BuildCont(T_Co_integer,UnionLnum(s0),l)
#define T_z(l) BuildCont(T_Co_z,UnionNum(0),l)
#define T_y(l) BuildCont(T_Co_y,UnionNum(0),l)
#define T_x(l) BuildCont(T_Co_x,UnionNum(0),l)
#define T_right_neighbor(l) BuildCont(T_Co_right_neighbor,UnionNum(0),l)
#define T_left_neighbor(l) BuildCont(T_Co_left_neighbor,UnionNum(0),l)
#define T_lower_neighbor(l) BuildCont(T_Co_lower_neighbor,UnionNum(0),l)
#define T_upper_neighbor(l) BuildCont(T_Co_upper_neighbor,UnionNum(0),l)
#define T_right_margin(l) BuildCont(T_Co_right_margin,UnionNum(0),l)
#define T_left_margin(l) BuildCont(T_Co_left_margin,UnionNum(0),l)
#define T_bottom_margin(l) BuildCont(T_Co_bottom_margin,UnionNum(0),l)
#define T_top_margin(l) BuildCont(T_Co_top_margin,UnionNum(0),l)
#define T_equal_column(l) BuildCont(T_Co_equal_column,UnionNum(0),l)
#define T_equal_row(l) BuildCont(T_Co_equal_row,UnionNum(0),l)
#define T_equal_position(l) BuildCont(T_Co_equal_position,UnionNum(0),l)
#define T_behind(l) BuildCont(T_Co_behind,UnionNum(0),l)
#define T_in_font(l) BuildCont(T_Co_in_font,UnionNum(0),l)
#define T_right(l) BuildCont(T_Co_right,UnionNum(0),l)
#define T_left(l) BuildCont(T_Co_left,UnionNum(0),l)
#define T_below(l) BuildCont(T_Co_below,UnionNum(0),l)
#define T_above(l) BuildCont(T_Co_above,UnionNum(0),l)
#define T_limit(l) BuildCont(T_Co_limit,UnionNum(0),l)
#define T_cluster(l) BuildCont(T_Co_cluster,UnionNum(0),l)
#define T_xrange(l) BuildCont(T_Co_xrange,UnionNum(0),l)
#define T_high_margin(l) BuildCont(T_Co_high_margin,UnionNum(0),l)
#define T_low_margin(l) BuildCont(T_Co_low_margin,UnionNum(0),l)
#define T_neighbors(l) BuildCont(T_Co_neighbors,UnionNum(0),l)
#define T_greater(l) BuildCont(T_Co_greater,UnionNum(0),l)
#define T_smaller(l) BuildCont(T_Co_smaller,UnionNum(0),l)
#define T_equal(l) BuildCont(T_Co_equal,UnionNum(0),l)
#define T_string_array(s0,s1,l) BuildTree(T_Co_string_array,2,UnionNum(0),l,s0,s1)
#define T_dimension(s0,l) BuildTree(T_Co_dimension,1,UnionNum(0),l,s0)
#define T_name(s0,l) BuildTree(T_Co_name,1,UnionNum(0),l,s0)
#define T_interval(s0,l) BuildTree(T_Co_interval,1,UnionNum(0),l,s0)
#define T_nodes(s0,l) BuildTree(T_Co_nodes,1,UnionNum(0),l,s0)
#define T_size(s0,l) BuildTree(T_Co_size,1,UnionNum(0),l,s0)
#define T_solid(l) BuildCont(T_Co_solid,UnionNum(0),l)
#define T_line(l) BuildCont(T_Co_line,UnionNum(0),l)
#define T_none(l) BuildCont(T_Co_none,UnionNum(0),l)
#define T_invisible(l) BuildCont(T_Co_invisible,UnionNum(0),l)
#define T_dashed(l) BuildCont(T_Co_dashed,UnionNum(0),l)
#define T_dotted(l) BuildCont(T_Co_dotted,UnionNum(0),l)
#define T_continuous(l) BuildCont(T_Co_continuous,UnionNum(0),l)
#define T_anchor(s0,l) BuildTree(T_Co_anchor,1,UnionNum(0),l,s0)
#define T_linestyle(s0,l) BuildTree(T_Co_linestyle,1,UnionNum(0),l,s0)
#define T_barrowstyle(s0,l) BuildTree(T_Co_barrowstyle,1,UnionNum(0),l,s0)
#define T_arrowstyle(s0,l) BuildTree(T_Co_arrowstyle,1,UnionNum(0),l,s0)
#define T_barrowsize(s0,l) BuildTree(T_Co_barrowsize,1,UnionNum(0),l,s0)
#define T_arrowsize(s0,l) BuildTree(T_Co_arrowsize,1,UnionNum(0),l,s0)
#define T_barrowcolor(s0,l) BuildTree(T_Co_barrowcolor,1,UnionNum(0),l,s0)
#define T_arrowcolor(s0,l) BuildTree(T_Co_arrowcolor,1,UnionNum(0),l,s0)
#define T_arrowheight(s0,l) BuildTree(T_Co_arrowheight,1,UnionNum(0),l,s0)
#define T_arrowwidth(s0,l) BuildTree(T_Co_arrowwidth,1,UnionNum(0),l,s0)
#define T_priority(s0,l) BuildTree(T_Co_priority,1,UnionNum(0),l,s0)
#define T_class(s0,l) BuildTree(T_Co_class,1,UnionNum(0),l,s0)
#define T_thickness(s0,l) BuildTree(T_Co_thickness,1,UnionNum(0),l,s0)
#define T_targetname(s0,l) BuildTree(T_Co_targetname,1,UnionNum(0),l,s0)
#define T_sourcename(s0,l) BuildTree(T_Co_sourcename,1,UnionNum(0),l,s0)
#define T_around(l) BuildCont(T_Co_around,UnionNum(0),l)
#define T_top(l) BuildCont(T_Co_top,UnionNum(0),l)
#define T_bottom(l) BuildCont(T_Co_bottom,UnionNum(0),l)
#define T_triangle(l) BuildCont(T_Co_triangle,UnionNum(0),l)
#define T_ellipse(l) BuildCont(T_Co_ellipse,UnionNum(0),l)
#define T_rhomb(l) BuildCont(T_Co_rhomb,UnionNum(0),l)
#define T_box(l) BuildCont(T_Co_box,UnionNum(0),l)
#define T_right_justify(l) BuildCont(T_Co_right_justify,UnionNum(0),l)
#define T_left_justify(l) BuildCont(T_Co_left_justify,UnionNum(0),l)
#define T_center(l) BuildCont(T_Co_center,UnionNum(0),l)
#define T_iconstyle(s0,l) BuildTree(T_Co_iconstyle,1,UnionNum(0),l,s0)
#define T_iconheight(s0,l) BuildTree(T_Co_iconheight,1,UnionNum(0),l,s0)
#define T_iconwidth(s0,l) BuildTree(T_Co_iconwidth,1,UnionNum(0),l,s0)
#define T_anchorpoints(s0,l) BuildTree(T_Co_anchorpoints,1,UnionNum(0),l,s0)
#define T_iconfile(s0,l) BuildTree(T_Co_iconfile,1,UnionNum(0),l,s0)
#define T_bordercolor(s0,l) BuildTree(T_Co_bordercolor,1,UnionNum(0),l,s0)
#define T_fontname(s0,l) BuildTree(T_Co_fontname,1,UnionNum(0),l,s0)
#define T_constraint_attribute(s0,s1,l) BuildTree(T_Co_constraint_attribute,2,UnionNum(0),l,s0,s1)
#define T_edge_attribute(s0,s1,l) BuildTree(T_Co_edge_attribute,2,UnionNum(0),l,s0,s1)
#define T_node_attribute(s0,s1,l) BuildTree(T_Co_node_attribute,2,UnionNum(0),l,s0,s1)
#define T_free(l) BuildCont(T_Co_free,UnionNum(0),l)
#define T_fixed(l) BuildCont(T_Co_fixed,UnionNum(0),l)
#define T_fpfish(l) BuildCont(T_Co_fpfish,UnionNum(0),l)
#define T_pfish(l) BuildCont(T_Co_pfish,UnionNum(0),l)
#define T_fcfish(l) BuildCont(T_Co_fcfish,UnionNum(0),l)
#define T_cfish(l) BuildCont(T_Co_cfish,UnionNum(0),l)
#define T_medianbary(l) BuildCont(T_Co_medianbary,UnionNum(0),l)
#define T_barymedian(l) BuildCont(T_Co_barymedian,UnionNum(0),l)
#define T_median(l) BuildCont(T_Co_median,UnionNum(0),l)
#define T_bary(l) BuildCont(T_Co_bary,UnionNum(0),l)
#define T_no(l) BuildCont(T_Co_no,UnionNum(0),l)
#define T_yes(l) BuildCont(T_Co_yes,UnionNum(0),l)
#define T_grey(l) BuildCont(T_Co_grey,UnionNum(0),l)
#define T_manual(l) BuildCont(T_Co_manual,UnionNum(0),l)
#define T_every(l) BuildCont(T_Co_every,UnionNum(0),l)
#define T_depthfirst(l) BuildCont(T_Co_depthfirst,UnionNum(0),l)
#define T_minbackwards(l) BuildCont(T_Co_minbackwards,UnionNum(0),l)
#define T_minoutdegree(l) BuildCont(T_Co_minoutdegree,UnionNum(0),l)
#define T_maxoutdegree(l) BuildCont(T_Co_maxoutdegree,UnionNum(0),l)
#define T_minindegree(l) BuildCont(T_Co_minindegree,UnionNum(0),l)
#define T_maxindegree(l) BuildCont(T_Co_maxindegree,UnionNum(0),l)
#define T_mindegree(l) BuildCont(T_Co_mindegree,UnionNum(0),l)
#define T_maxdegree(l) BuildCont(T_Co_maxdegree,UnionNum(0),l)
#define T_mindepthslow(l) BuildCont(T_Co_mindepthslow,UnionNum(0),l)
#define T_maxdepthslow(l) BuildCont(T_Co_maxdepthslow,UnionNum(0),l)
#define T_mindepth(l) BuildCont(T_Co_mindepth,UnionNum(0),l)
#define T_maxdepth(l) BuildCont(T_Co_maxdepth,UnionNum(0),l)
#define T_tree(l) BuildCont(T_Co_tree,UnionNum(0),l)
#define T_constaints(l) BuildCont(T_Co_constaints,UnionNum(0),l)
#define T_planar(l) BuildCont(T_Co_planar,UnionNum(0),l)
#define T_isi(l) BuildCont(T_Co_isi,UnionNum(0),l)
#define T_barycenter(l) BuildCont(T_Co_barycenter,UnionNum(0),l)
#define T_right_to_left(l) BuildCont(T_Co_right_to_left,UnionNum(0),l)
#define T_left_to_right(l) BuildCont(T_Co_left_to_right,UnionNum(0),l)
#define T_bottom_to_top(l) BuildCont(T_Co_bottom_to_top,UnionNum(0),l)
#define T_top_to_bottom(l) BuildCont(T_Co_top_to_bottom,UnionNum(0),l)
#define T_low(l) BuildCont(T_Co_low,UnionNum(0),l)
#define T_high(l) BuildCont(T_Co_high,UnionNum(0),l)
#define T_colindex(s0,l) BuildTree(T_Co_colindex,1,UnionNum(0),l,s0)
#define T_yellowgreen(l) BuildCont(T_Co_yellowgreen,UnionNum(0),l)
#define T_yellow(l) BuildCont(T_Co_yellow,UnionNum(0),l)
#define T_white(l) BuildCont(T_Co_white,UnionNum(0),l)
#define T_turquoise(l) BuildCont(T_Co_turquoise,UnionNum(0),l)
#define T_red(l) BuildCont(T_Co_red,UnionNum(0),l)
#define T_purple(l) BuildCont(T_Co_purple,UnionNum(0),l)
#define T_pink(l) BuildCont(T_Co_pink,UnionNum(0),l)
#define T_orchid(l) BuildCont(T_Co_orchid,UnionNum(0),l)
#define T_orange(l) BuildCont(T_Co_orange,UnionNum(0),l)
#define T_magenta(l) BuildCont(T_Co_magenta,UnionNum(0),l)
#define T_lilac(l) BuildCont(T_Co_lilac,UnionNum(0),l)
#define T_lightyellow(l) BuildCont(T_Co_lightyellow,UnionNum(0),l)
#define T_lightred(l) BuildCont(T_Co_lightred,UnionNum(0),l)
#define T_lightmagenta(l) BuildCont(T_Co_lightmagenta,UnionNum(0),l)
#define T_lightgrey(l) BuildCont(T_Co_lightgrey,UnionNum(0),l)
#define T_lightgreen(l) BuildCont(T_Co_lightgreen,UnionNum(0),l)
#define T_lightcyan(l) BuildCont(T_Co_lightcyan,UnionNum(0),l)
#define T_lightblue(l) BuildCont(T_Co_lightblue,UnionNum(0),l)
#define T_khaki(l) BuildCont(T_Co_khaki,UnionNum(0),l)
#define T_green(l) BuildCont(T_Co_green,UnionNum(0),l)
#define T_gold(l) BuildCont(T_Co_gold,UnionNum(0),l)
#define T_darkyellow(l) BuildCont(T_Co_darkyellow,UnionNum(0),l)
#define T_darkred(l) BuildCont(T_Co_darkred,UnionNum(0),l)
#define T_darkmagenta(l) BuildCont(T_Co_darkmagenta,UnionNum(0),l)
#define T_darkgrey(l) BuildCont(T_Co_darkgrey,UnionNum(0),l)
#define T_darkgreen(l) BuildCont(T_Co_darkgreen,UnionNum(0),l)
#define T_darkcyan(l) BuildCont(T_Co_darkcyan,UnionNum(0),l)
#define T_darkblue(l) BuildCont(T_Co_darkblue,UnionNum(0),l)
#define T_cyan(l) BuildCont(T_Co_cyan,UnionNum(0),l)
#define T_blue(l) BuildCont(T_Co_blue,UnionNum(0),l)
#define T_black(l) BuildCont(T_Co_black,UnionNum(0),l)
#define T_aquamarine(l) BuildCont(T_Co_aquamarine,UnionNum(0),l)
#define T_yscrollbar(s0,l) BuildTree(T_Co_yscrollbar,1,UnionNum(0),l,s0)
#define T_xscrollbar(s0,l) BuildTree(T_Co_xscrollbar,1,UnionNum(0),l,s0)
#define T_outputfunction(s0,l) BuildTree(T_Co_outputfunction,1,UnionNum(0),l,s0)
#define T_inputfunction(s0,l) BuildTree(T_Co_inputfunction,1,UnionNum(0),l,s0)
#define T_topsort(s0,l) BuildTree(T_Co_topsort,1,UnionNum(0),l,s0)
#define T_layoutparameter(s0,l) BuildTree(T_Co_layoutparameter,1,UnionNum(0),l,s0)
#define T_include(s0,l) BuildTree(T_Co_include,1,UnionNum(0),l,s0)
#define T_typename(s0,l) BuildTree(T_Co_typename,1,UnionNum(0),l,s0)
#define T_straight_max(s0,l) BuildTree(T_Co_straight_max,1,UnionNum(0),l,s0)
#define T_rubber_min(s0,l) BuildTree(T_Co_rubber_min,1,UnionNum(0),l,s0)
#define T_rubber_max(s0,l) BuildTree(T_Co_rubber_max,1,UnionNum(0),l,s0)
#define T_pendel_min(s0,l) BuildTree(T_Co_pendel_min,1,UnionNum(0),l,s0)
#define T_pendel_max(s0,l) BuildTree(T_Co_pendel_max,1,UnionNum(0),l,s0)
#define T_cross_min(s0,l) BuildTree(T_Co_cross_min,1,UnionNum(0),l,s0)
#define T_cross_max(s0,l) BuildTree(T_Co_cross_max,1,UnionNum(0),l,s0)
#define T_bend_max(s0,l) BuildTree(T_Co_bend_max,1,UnionNum(0),l,s0)
#define T_view_splines(s0,l) BuildTree(T_Co_view_splines,1,UnionNum(0),l,s0)
#define T_view_nodes(s0,l) BuildTree(T_Co_view_nodes,1,UnionNum(0),l,s0)
#define T_view_edges(s0,l) BuildTree(T_Co_view_edges,1,UnionNum(0),l,s0)
#define T_view_method(s0,l) BuildTree(T_Co_view_method,1,UnionNum(0),l,s0)
#define T_crossing_weight(s0,l) BuildTree(T_Co_crossing_weight,1,UnionNum(0),l,s0)
#define T_crossing_opt(s0,l) BuildTree(T_Co_crossing_opt,1,UnionNum(0),l,s0)
#define T_crossing_phase2(s0,l) BuildTree(T_Co_crossing_phase2,1,UnionNum(0),l,s0)
#define T_treefactor(s0,l) BuildTree(T_Co_treefactor,1,UnionNum(0),l,s0)
#define T_spreadlevel(s0,l) BuildTree(T_Co_spreadlevel,1,UnionNum(0),l,s0)
#define T_arrow_mode(s0,l) BuildTree(T_Co_arrow_mode,1,UnionNum(0),l,s0)
#define T_port_sharing(s0,l) BuildTree(T_Co_port_sharing,1,UnionNum(0),l,s0)
#define T_node_alignment(s0,l) BuildTree(T_Co_node_alignment,1,UnionNum(0),l,s0)
#define T_orientation(s0,l) BuildTree(T_Co_orientation,1,UnionNum(0),l,s0)
#define T_dummy(l) BuildCont(T_Co_dummy,UnionNum(0),l)
#define T_nonearedges(l) BuildCont(T_Co_nonearedges,UnionNum(0),l)
#define T_smanhatten(s0,l) BuildTree(T_Co_smanhatten,1,UnionNum(0),l,s0)
#define T_manhatten(s0,l) BuildTree(T_Co_manhatten,1,UnionNum(0),l,s0)
#define T_priophase(s0,l) BuildTree(T_Co_priophase,1,UnionNum(0),l,s0)
#define T_straightphase(s0,l) BuildTree(T_Co_straightphase,1,UnionNum(0),l,s0)
#define T_hidesingles(s0,l) BuildTree(T_Co_hidesingles,1,UnionNum(0),l,s0)
#define T_finetuning(s0,l) BuildTree(T_Co_finetuning,1,UnionNum(0),l,s0)
#define T_dirty_edge_label(s0,l) BuildTree(T_Co_dirty_edge_label,1,UnionNum(0),l,s0)
#define T_display_edge_label(s0,l) BuildTree(T_Co_display_edge_label,1,UnionNum(0),l,s0)
#define T_late_edge_label(s0,l) BuildTree(T_Co_late_edge_label,1,UnionNum(0),l,s0)
#define T_splinefactor(s0,l) BuildTree(T_Co_splinefactor,1,UnionNum(0),l,s0)
#define T_nearfactor(s0,l) BuildTree(T_Co_nearfactor,1,UnionNum(0),l,s0)
#define T_upfactor(s0,l) BuildTree(T_Co_upfactor,1,UnionNum(0),l,s0)
#define T_downfactor(s0,l) BuildTree(T_Co_downfactor,1,UnionNum(0),l,s0)
#define T_layoutfrequency(s0,l) BuildTree(T_Co_layoutfrequency,1,UnionNum(0),l,s0)
#define T_layoutalgorithm(s0,l) BuildTree(T_Co_layoutalgorithm,1,UnionNum(0),l,s0)
#define T_colentry(s0,s1,s2,s3,l) BuildTree(T_Co_colentry,4,UnionNum(0),l,s0,s1,s2,s3)
#define T_infoname(s0,s1,l) BuildTree(T_Co_infoname,2,UnionNum(0),l,s0,s1)
#define T_classname(s0,s1,l) BuildTree(T_Co_classname,2,UnionNum(0),l,s0,s1)
#define T_hidden(s0,l) BuildTree(T_Co_hidden,1,UnionNum(0),l,s0)
#define T_yraster(s0,l) BuildTree(T_Co_yraster,1,UnionNum(0),l,s0)
#define T_xlraster(s0,l) BuildTree(T_Co_xlraster,1,UnionNum(0),l,s0)
#define T_xraster(s0,l) BuildTree(T_Co_xraster,1,UnionNum(0),l,s0)
#define T_yspace(s0,l) BuildTree(T_Co_yspace,1,UnionNum(0),l,s0)
#define T_xlspace(s0,l) BuildTree(T_Co_xlspace,1,UnionNum(0),l,s0)
#define T_xspace(s0,l) BuildTree(T_Co_xspace,1,UnionNum(0),l,s0)
#define T_ybase(s0,l) BuildTree(T_Co_ybase,1,UnionNum(0),l,s0)
#define T_xbase(s0,l) BuildTree(T_Co_xbase,1,UnionNum(0),l,s0)
#define T_ymax(s0,l) BuildTree(T_Co_ymax,1,UnionNum(0),l,s0)
#define T_xmax(s0,l) BuildTree(T_Co_xmax,1,UnionNum(0),l,s0)
#define T_status(s0,l) BuildTree(T_Co_status,1,UnionNum(0),l,s0)
#define T_horizontal_order(s0,l) BuildTree(T_Co_horizontal_order,1,UnionNum(0),l,s0)
#define T_level(s0,l) BuildTree(T_Co_level,1,UnionNum(0),l,s0)
#define T_shape(s0,l) BuildTree(T_Co_shape,1,UnionNum(0),l,s0)
#define T_textmode(s0,l) BuildTree(T_Co_textmode,1,UnionNum(0),l,s0)
#define T_stretch(s0,l) BuildTree(T_Co_stretch,1,UnionNum(0),l,s0)
#define T_shrink(s0,l) BuildTree(T_Co_shrink,1,UnionNum(0),l,s0)
#define T_scaling(s0,l) BuildTree(T_Co_scaling,1,UnionNum(0),l,s0)
#define T_folding(s0,l) BuildTree(T_Co_folding,1,UnionNum(0),l,s0)
#define T_loc(s0,s1,l) BuildTree(T_Co_loc,2,UnionNum(0),l,s0,s1)
#define T_ydef(s0,l) BuildTree(T_Co_ydef,1,UnionNum(0),l,s0)
#define T_xdef(s0,l) BuildTree(T_Co_xdef,1,UnionNum(0),l,s0)
#define T_borderwidth(s0,l) BuildTree(T_Co_borderwidth,1,UnionNum(0),l,s0)
#define T_height(s0,l) BuildTree(T_Co_height,1,UnionNum(0),l,s0)
#define T_width(s0,l) BuildTree(T_Co_width,1,UnionNum(0),l,s0)
#define T_colorborder(s0,l) BuildTree(T_Co_colorborder,1,UnionNum(0),l,s0)
#define T_textcolor(s0,l) BuildTree(T_Co_textcolor,1,UnionNum(0),l,s0)
#define T_color(s0,l) BuildTree(T_Co_color,1,UnionNum(0),l,s0)
#define T_info3(s0,l) BuildTree(T_Co_info3,1,UnionNum(0),l,s0)
#define T_info2(s0,l) BuildTree(T_Co_info2,1,UnionNum(0),l,s0)
#define T_info1(s0,l) BuildTree(T_Co_info1,1,UnionNum(0),l,s0)
#define T_label(s0,l) BuildTree(T_Co_label,1,UnionNum(0),l,s0)
#define T_title(s0,l) BuildTree(T_Co_title,1,UnionNum(0),l,s0)
#define T_constraint(s0,l) BuildTree(T_Co_constraint,1,UnionNum(0),l,s0)
#define T_back_edge(s0,l) BuildTree(T_Co_back_edge,1,UnionNum(0),l,s0)
#define T_bent_near_edge(s0,l) BuildTree(T_Co_bent_near_edge,1,UnionNum(0),l,s0)
#define T_near_edge(s0,l) BuildTree(T_Co_near_edge,1,UnionNum(0),l,s0)
#define T_edge(s0,l) BuildTree(T_Co_edge,1,UnionNum(0),l,s0)
#define T_node(s0,l) BuildTree(T_Co_node,1,UnionNum(0),l,s0)
#define T_graph(s0,l) BuildTree(T_Co_graph,1,UnionNum(0),l,s0)
#define T_foldedge_defaults(s0,l) BuildTree(T_Co_foldedge_defaults,1,UnionNum(0),l,s0)
#define T_foldnode_defaults(s0,l) BuildTree(T_Co_foldnode_defaults,1,UnionNum(0),l,s0)
#define T_edge_defaults(s0,l) BuildTree(T_Co_edge_defaults,1,UnionNum(0),l,s0)
#define T_node_defaults(s0,l) BuildTree(T_Co_node_defaults,1,UnionNum(0),l,s0)
#define T_graph_attribute(s0,l) BuildTree(T_Co_graph_attribute,1,UnionNum(0),l,s0)
#define T_graph_entry(s0,s1,l) BuildTree(T_Co_graph_entry,2,UnionNum(0),l,s0,s1)




/* from scanner */

#ifdef ANSI_C
void init_lex(void);
int yylex(void);
#else
void init_lex();
int yylex();
#endif

static char message[1024];
 
/* for error messages */



static const char * const yytokname[] = {
"<start/end of input>$"
	,"error"
	,"$illegal."
	,"lexem ABOVE"
	,"lexem ANCHORPOINTS"
	,"lexem ANCHOR"
	,"lexem AQUAMARINE"
	,"lexem AROUND"
	,"lexem ARROWMODE"
	,"lexem ARROWHEIGHT"
	,"lexem ARROWWIDTH"
	,"lexem ARROWCOLOR"
	,"lexem ARROWSTYLE"
	,"lexem ARROWSIZE"
	,"lexem BARROWCOLOR"
	,"lexem BARROWSTYLE"
	,"lexem BARROWSIZE"
	,"lexem BACKEDGE"
	,"lexem BARYCENTER"
	,"lexem BARY"
	,"lexem BARYMEDIAN"
	,"lexem BEHIND"
	,"lexem BELOW"
	,"lexem BLACK"
	,"lexem BLUE"
	,"lexem BMAX"
	,"lexem BORDERCOLOR"
	,"lexem BORDERWIDTH"
	,"lexem BOTTOM_MARGIN"
	,"lexem BOTTOM_TO_TOP"
	,"lexem BOTTOM"
	,"lexem BOX"
	,"lexem BENTNEAREDGE"
	,"lexem CENTER"
	,"lexem CFISH"
	,"lexem CLASSNAME"
	,"lexem CLASS"
	,"lexem CLUSTER"
	,"lexem CMIN"
	,"lexem CMAX"
	,"lexem COLORENTRY"
	,"lexem COLOR"
	,"lexem CONSTRAINTS"
	,"lexem CONSTRAINT"
	,"lexem CONTINUOUS"
	,"lexem CROSSING_WEIGHT"
	,"lexem CROSSING_OPT"
	,"lexem CROSSING_P2"
	,"lexem CYAN"
	,"lexem DARKBLUE"
	,"lexem DARKCYAN"
	,"lexem DARKGREEN"
	,"lexem DARKGREY"
	,"lexem DARKMAGENTA"
	,"lexem DARKRED"
	,"lexem DARKYELLOW"
	,"lexem DASHED"
	,"lexem DFS"
	,"lexem DIMENSION"
	,"lexem DIRTY_EDGE_LABELS"
	,"lexem DISPLAY_EDGE_LABELS"
	,"lexem DOTTED"
	,"lexem EDGE1"
	,"lexem EDGE2"
	,"lexem EDGES"
	,"lexem ELLIPSE"
	,"lexem EQUAL_COLUMN"
	,"lexem EQUAL_POSITION"
	,"lexem EQUAL_ROW"
	,"lexem EQUAL"
	,"lexem EVERY"
	,"lexem FCFISH"
	,"lexem FPFISH"
	,"lexem FIXED"
	,"lexem FREE"
	,"lexem FINETUNING"
	,"lexem FOLDEDGE"
	,"lexem FOLDNODE"
	,"lexem FOLDING"
	,"lexem FONTNAME"
	,"lexem GOLD"
	,"lexem GRAPH"
	,"lexem GREATER"
	,"lexem GREEN"
	,"lexem GREY"
	,"lexem HEIGHT"
	,"lexem HIDESINGLES"
	,"lexem HIGH_MARGIN"
	,"lexem HIGH"
	,"lexem HIDDEN"
	,"lexem HORDER"
	,"lexem ICONFILE"
	,"lexem ICONHEIGHT"
	,"lexem ICONSTYLE"
	,"lexem ICONWIDTH"
	,"lexem INCLUDE"
	,"lexem INFONAME"
	,"lexem INFO1"
	,"lexem INFO2"
	,"lexem INFO3"
	,"lexem INPUTFUNCTION"
	,"lexem INTERVAL"
	,"lexem INVISIBLE"
	,"lexem IN_FRONT"
	,"lexem ISI"
	,"lexem KHAKI"
	,"lexem TEXTCOLOR"
	,"lexem LABEL"
	,"lexem LATE_LABELS"
	,"lexem LAYOUTALGORITHM"
	,"lexem LAYOUTFREQUENCY"
	,"lexem LAYOUTPARAMETER"
	,"lexem LAYOUTDOWNFACTOR"
	,"lexem LAYOUTUPFACTOR"
	,"lexem LAYOUTNEARFACTOR"
	,"lexem LAYOUTSPLINEFACTOR"
	,"lexem LEFT_JUSTIFY"
	,"lexem LEFT_MARGIN"
	,"lexem LEFT_NEIGHBOR"
	,"lexem LEFT_TO_RIGHT"
	,"lexem LEFT"
	,"lexem LEVEL"
	,"lexem VORDER"
	,"lexem LIGHTBLUE"
	,"lexem LIGHTCYAN"
	,"lexem LIGHTGREEN"
	,"lexem LIGHTGREY"
	,"lexem LIGHTMAGENTA"
	,"lexem LIGHTRED"
	,"lexem LIGHTYELLOW"
	,"lexem LILAC"
	,"lexem LIMIT"
	,"lexem LINE"
	,"lexem LINESTYLE"
	,"lexem LOC"
	,"lexem LOWER_NEIGHBOR"
	,"lexem LOW_MARGIN"
	,"lexem LOW"
	,"lexem MAGENTA"
	,"lexem MANHATTEN"
	,"lexem MANUAL"
	,"lexem MAXDEPTHSLOW"
	,"lexem MAXDEPTH"
	,"lexem MAXDEGREE"
	,"lexem MAXINDEGREE"
	,"lexem MAXOUTDEGREE"
	,"lexem MEDIAN"
	,"lexem MEDIANBARY"
	,"lexem MINDEPTHSLOW"
	,"lexem MINDEPTH"
	,"lexem MINDEGREE"
	,"lexem MININDEGREE"
	,"lexem MINOUTDEGREE"
	,"lexem MINBACK"
	,"lexem NAME"
	,"lexem NEAREDGE"
	,"lexem NEIGHBORS"
	,"lexem NEAREDGES"
	,"lexem NONEAREDGES"
	,"lexem NODE1"
	,"lexem NODE2"
	,"lexem NODES"
	,"lexem NODE_ALIGN"
	,"lexem NONE"
	,"lexem NO"
	,"lexem ORANGE"
	,"lexem ORCHID"
	,"lexem ORIENTATION"
	,"lexem OUTPUTFUNCTION"
	,"lexem PFISH"
	,"lexem PINK"
	,"lexem PLANAR"
	,"lexem PMIN"
	,"lexem PMAX"
	,"lexem PORTSHARING"
	,"lexem PRIORITYPHASE"
	,"lexem PRIORITY"
	,"lexem PURPLE"
	,"lexem RANGE"
	,"lexem RED"
	,"lexem RHOMB"
	,"lexem RIGHT_JUSTIFY"
	,"lexem RIGHT_MARGIN"
	,"lexem RIGHT_NEIGHBOR"
	,"lexem RIGHT_TO_LEFT"
	,"lexem RIGHT"
	,"lexem RMIN"
	,"lexem RMAX"
	,"lexem SCALING"
	,"lexem SHAPE"
	,"lexem SHRINK"
	,"lexem SMAX"
	,"lexem SMANHATTEN"
	,"lexem SIZE"
	,"lexem SMALLER"
	,"lexem SOLID"
	,"lexem SOURCENAME"
	,"lexem SPLINES"
	,"lexem SPREADLEVEL"
	,"lexem STATUS"
	,"lexem STRETCH"
	,"lexem STRAIGHTPHASE"
	,"lexem TARGETNAME"
	,"lexem TEXTMODE"
	,"lexem THICKNESS"
	,"lexem TITLE"
	,"lexem TOPSORT"
	,"lexem TOP_MARGIN"
	,"lexem TOP_TO_BOTTOM"
	,"lexem TOP"
	,"lexem TREE"
	,"lexem TREEFACTOR"
	,"lexem TRIANGLE"
	,"lexem TURQUOISE"
	,"lexem TYPENAME"
	,"lexem UPPER_NEIGHBOR"
	,"lexem VIEW"
	,"lexem WHITE"
	,"lexem WIDTH"
	,"lexem XBASE"
	,"lexem XMAX"
	,"lexem XRASTER"
	,"lexem XLRASTER"
	,"lexem XSCROLLBAR"
	,"lexem XSPACE"
	,"lexem XLSPACE"
	,"lexem YBASE"
	,"lexem YELLOWGREEN"
	,"lexem YELLOW"
	,"lexem YES"
	,"lexem YMAX"
	,"lexem YRASTER"
	,"lexem YSCROLLBAR"
	,"lexem YSPACE"
	,"lexem INT"
	,"lexem FLOAT"
	,"lexem CHAR"
	,"lexem STRING"
	,"'{'"
	,"'}'"
	,"':'"
	,"'x'"
	,"'y'"
	,"'z'"
	,"'*'"
	,"'['"
	,"'-'"
	,"']'"
	,"graph"
	,"graph_entry_list"
	,"graph_entry"
	,"graph_attribute"
	,"enum_color"
	,"enum_topsort"
	,"enum_orientation"
	,"enum_layoutalgorithm"
	,"enum_layoutfrequency"
	,"enum_status"
	,"enum_yes_no"
	,"enum_cross_weight"
	,"enum_view"
	,"enum_arrow_mode"
	,"foldnode_defaults"
	,"foldedge_defaults"
	,"node_defaults"
	,"edge_defaults"
	,"node"
	,"node_attribute_list"
	,"edge"
	,"nearedge"
	,"bentnearedge"
	,"backedge"
	,"edge_attribute_list"
	,"constraint"
	,"constraint_attribute_list"
	,"node_attribute"
	,"enum_textmode"
	,"enum_shape"
	,"enum_node_align"
	,"enum_iconstyle"
	,"edge_attribute"
	,"enum_linestyle"
	,"enum_arrowstyle"
	,"constraint_attribute"
	,"string_array"
	,"enum_name"
	,"enum_dimension"
	,"attribute_value"
	,"array_value"
	,"index_value_list"
	,"index_value"
	,"range"
	,"index"
	,"int_const"
	,"float_const"
	,"str_const"
	,""
};



#ifndef yyerror
#define yyerror(x) { \
	(void)sprintf(message,"unexpected %s (%s)", \
		 ((yychar<0)?"(?)":yytokname[YYTRANSLATE(yychar)]),x);  \
	(void)printf("Line %d Pos %d: %s !\n",line_nr,pos_nr,message);\
	nr_errors++;\
    }
#endif

#ifdef BISONGEN
#ifndef yylocate
#define yylocate(x) (&(x))
#endif
#endif

#ifdef YACCGEN
static YYLTYPE yystdlocation;

static YYLTYPE *yystdloc(void) 
{   yystdlocation->first_line = yystdlocation->last_line = line_nr; 
    yystdlocation->first_column = yystdlocation->last_column = pos_nr; 
    return(&yystdlocation);
}
#ifndef yylocate
#define yylocate(x) (yystdloc())
#endif
#endif /* YACCGEN */

#ifndef yysyntaxtree
#define yysyntaxtree char*
#endif

#define YY_NEVERNEEDED (@1)
#undef  YY_NEVERNEEDED 

/*-------------------------------------------------------------*/
/*               YACC or BISON Specification                   */
/*-------------------------------------------------------------*/






/* Line 189 of yacc.c  */
#line 1759 "y.tab.c"

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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 2285 "y.tab.c"
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


/* Line 264 of yacc.c  */
#line 2310 "y.tab.c"

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
# if YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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
       0,  1998,  1998,  2004,  2007,  2013,  2015,  2017,  2019,  2021,
    2023,  2025,  2027,  2029,  2031,  2033,  2035,  2040,  2043,  2046,
    2049,  2052,  2055,  2058,  2061,  2064,  2067,  2070,  2073,  2075,
    2077,  2080,  2083,  2086,  2089,  2092,  2095,  2098,  2101,  2104,
    2107,  2111,  2114,  2117,  2120,  2123,  2126,  2129,  2132,  2135,
    2138,  2141,  2144,  2147,  2150,  2153,  2157,  2160,  2163,  2166,
    2169,  2172,  2176,  2179,  2182,  2185,  2188,  2191,  2194,  2197,
    2200,  2203,  2205,  2208,  2211,  2214,  2217,  2220,  2224,  2227,
    2231,  2234,  2237,  2241,  2244,  2247,  2250,  2254,  2257,  2260,
    2263,  2266,  2269,  2272,  2275,  2279,  2282,  2285,  2288,  2291,
    2294,  2297,  2300,  2306,  2308,  2310,  2312,  2314,  2316,  2318,
    2320,  2322,  2324,  2326,  2328,  2330,  2332,  2334,  2336,  2338,
    2340,  2342,  2344,  2346,  2348,  2350,  2352,  2354,  2356,  2358,
    2360,  2362,  2364,  2366,  2368,  2370,  2375,  2377,  2382,  2384,
    2386,  2388,  2394,  2396,  2398,  2400,  2403,  2405,  2407,  2409,
    2411,  2413,  2415,  2417,  2419,  2421,  2423,  2425,  2427,  2433,
    2435,  2440,  2442,  2444,  2449,  2451,  2455,  2457,  2459,  2461,
    2465,  2467,  2469,  2471,  2475,  2477,  2481,  2486,  2491,  2496,
    2502,  2508,  2511,  2517,  2522,  2527,  2532,  2538,  2541,  2547,
    2554,  2557,  2563,  2566,  2569,  2572,  2575,  2578,  2581,  2584,
    2587,  2590,  2593,  2596,  2599,  2602,  2605,  2608,  2611,  2614,
    2617,  2620,  2623,  2626,  2629,  2632,  2635,  2638,  2641,  2644,
    2649,  2651,  2653,  2657,  2659,  2661,  2663,  2667,  2669,  2671,
    2675,  2677,  2679,  2684,  2687,  2690,  2693,  2696,  2699,  2702,
    2705,  2708,  2711,  2714,  2717,  2720,  2723,  2726,  2729,  2732,
    2735,  2738,  2741,  2744,  2750,  2752,  2754,  2756,  2758,  2763,
    2765,  2767,  2771,  2774,  2777,  2780,  2783,  2786,  2789,  2794,
    2797,  2801,  2803,  2805,  2807,  2809,  2811,  2813,  2815,  2817,
    2819,  2821,  2823,  2825,  2827,  2829,  2831,  2833,  2835,  2837,
    2839,  2841,  2843,  2845,  2847,  2849,  2851,  2855,  2857,  2859,
    2863,  2865,  2867,  2869,  2871,  2875,  2880,  2883,  2887,  2889,
    2892,  2895,  2900,  2905,  2909,  2913,  2917
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

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
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
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
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
# if YYLTYPE_IS_TRIVIAL
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
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



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
    YYLTYPE yyerror_range[2];

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

#if YYLTYPE_IS_TRIVIAL
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
  if (yyn == YYPACT_NINF)
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

/* Line 1455 of yacc.c  */
#line 1999 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); Syntax_Tree = (yyval.tree); }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 2005 "grammar.y"
    {
		 (yyval.tree) = T_graph_entry((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 2008 "grammar.y"
    {
		 (yyval.tree) = T_graph_entry((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 2013 "grammar.y"
    {
		 (yyval.tree) = T_graph_attribute((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)]))); }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 2015 "grammar.y"
    {
		 (yyval.tree) = T_node_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 2017 "grammar.y"
    {
		 (yyval.tree) = T_edge_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 2019 "grammar.y"
    {
		 (yyval.tree) = T_foldnode_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 2021 "grammar.y"
    {
		 (yyval.tree) = T_foldedge_defaults((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 2023 "grammar.y"
    {
		 (yyval.tree) = T_graph((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));           }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 2025 "grammar.y"
    {
		 (yyval.tree) = T_node((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));            }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 2027 "grammar.y"
    {
		 (yyval.tree) = T_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));            }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 2029 "grammar.y"
    {
		 (yyval.tree) = T_near_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));       }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 2031 "grammar.y"
    {
		 (yyval.tree) = T_bent_near_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 2033 "grammar.y"
    {
		 (yyval.tree) = T_back_edge((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));       }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 2035 "grammar.y"
    {
		 (yyval.tree) = T_constraint((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));      }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 2041 "grammar.y"
    {
		 (yyval.tree) = T_title((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 2044 "grammar.y"
    {
		 (yyval.tree) = T_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 2047 "grammar.y"
    {
		 (yyval.tree) = T_info1((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 2050 "grammar.y"
    {
		 (yyval.tree) = T_info2((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 2053 "grammar.y"
    {
		 (yyval.tree) = T_info3((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 2056 "grammar.y"
    {
		 (yyval.tree) = T_color((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 2059 "grammar.y"
    {
		 (yyval.tree) = T_textcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));          }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 2062 "grammar.y"
    {
		 (yyval.tree) = T_colorborder((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 2065 "grammar.y"
    {
		 (yyval.tree) = T_width((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 2068 "grammar.y"
    {
		 (yyval.tree) = T_height((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 2071 "grammar.y"
    {
		 (yyval.tree) = T_borderwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 2073 "grammar.y"
    {
		 (yyval.tree) = T_xdef((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 2075 "grammar.y"
    {
		 (yyval.tree) = T_ydef((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 2078 "grammar.y"
    {
		 (yyval.tree) = T_loc((yyvsp[(5) - (9)].tree),(yyvsp[(8) - (9)].tree),yylocate((yylsp[(1) - (9)])));       }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 2081 "grammar.y"
    {
		 (yyval.tree) = T_folding((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 2084 "grammar.y"
    {
		 (yyval.tree) = T_scaling((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 2087 "grammar.y"
    {
		 (yyval.tree) = T_shrink((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 2090 "grammar.y"
    {
		 (yyval.tree) = T_stretch((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 2093 "grammar.y"
    {
		 (yyval.tree) = T_textmode((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 2096 "grammar.y"
    {
		 (yyval.tree) = T_shape((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    	       }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 2099 "grammar.y"
    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 2102 "grammar.y"
    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 2105 "grammar.y"
    {
		 (yyval.tree) = T_horizontal_order((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 2108 "grammar.y"
    {
		 (yyval.tree) = T_status((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 2112 "grammar.y"
    {
		 (yyval.tree) = T_xmax((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 2115 "grammar.y"
    {
		 (yyval.tree) = T_ymax((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));               }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 2118 "grammar.y"
    {
		 (yyval.tree) = T_xbase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 2121 "grammar.y"
    {
		 (yyval.tree) = T_ybase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));              }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 2124 "grammar.y"
    {
		 (yyval.tree) = T_xspace((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 2127 "grammar.y"
    {
		 (yyval.tree) = T_xlspace((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 2130 "grammar.y"
    {
		 (yyval.tree) = T_yspace((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));             }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 2133 "grammar.y"
    {
		 (yyval.tree) = T_xraster((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 2136 "grammar.y"
    {
		 (yyval.tree) = T_xlraster((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 2139 "grammar.y"
    {
		 (yyval.tree) = T_yraster((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 2142 "grammar.y"
    {
		 (yyval.tree) = T_hidden((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 	       }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 2145 "grammar.y"
    {
		 (yyval.tree) = T_hidden((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 	       }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 2148 "grammar.y"
    {
		 (yyval.tree) = T_classname((yyvsp[(2) - (4)].tree),(yyvsp[(4) - (4)].tree),yylocate((yylsp[(1) - (4)])));       }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 2151 "grammar.y"
    {
		 (yyval.tree) = T_infoname((yyvsp[(2) - (4)].tree),(yyvsp[(4) - (4)].tree),yylocate((yylsp[(1) - (4)])));        }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 2154 "grammar.y"
    {
		 (yyval.tree) = T_colentry((yyvsp[(2) - (6)].tree),(yyvsp[(4) - (6)].tree),(yyvsp[(5) - (6)].tree),(yyvsp[(6) - (6)].tree),yylocate((yylsp[(1) - (6)])));   }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 2158 "grammar.y"
    {
		 (yyval.tree) = T_layoutalgorithm((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 2161 "grammar.y"
    {
		 (yyval.tree) = T_layoutfrequency((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 2164 "grammar.y"
    {
		 (yyval.tree) = T_downfactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 2167 "grammar.y"
    {
		 (yyval.tree) = T_upfactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 2170 "grammar.y"
    {
		 (yyval.tree) = T_nearfactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 2173 "grammar.y"
    {
		 (yyval.tree) = T_splinefactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 2177 "grammar.y"
    {
		 (yyval.tree) = T_late_edge_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 2180 "grammar.y"
    {
		 (yyval.tree) = T_display_edge_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 2183 "grammar.y"
    {
		 (yyval.tree) = T_dirty_edge_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 2186 "grammar.y"
    {
		 (yyval.tree) = T_finetuning((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 2189 "grammar.y"
    {
		 (yyval.tree) = T_hidesingles((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 2192 "grammar.y"
    {
		 (yyval.tree) = T_straightphase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 2195 "grammar.y"
    {
		 (yyval.tree) = T_priophase((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 2198 "grammar.y"
    {
		 (yyval.tree) = T_manhatten((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 2201 "grammar.y"
    {
		 (yyval.tree) = T_smanhatten((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 2203 "grammar.y"
    {
		 (yyval.tree) = T_nonearedges(yylocate((yylsp[(1) - (1)])));	       }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 2206 "grammar.y"
    {
		 (yyval.tree) = T_nonearedges(yylocate((yylsp[(1) - (3)])));	       }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 2209 "grammar.y"
    {
		 (yyval.tree) = T_dummy(yylocate((yylsp[(1) - (3)])));	     	       }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 2212 "grammar.y"
    {
		 (yyval.tree) = T_orientation((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 2215 "grammar.y"
    {
		 (yyval.tree) = T_node_alignment((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 2218 "grammar.y"
    {
		 (yyval.tree) = T_port_sharing((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 2221 "grammar.y"
    {
		 (yyval.tree) = T_arrow_mode((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 2225 "grammar.y"
    {
		 (yyval.tree) = T_spreadlevel((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 2228 "grammar.y"
    {
		 (yyval.tree) = T_treefactor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 2232 "grammar.y"
    {
		 (yyval.tree) = T_crossing_phase2((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 2235 "grammar.y"
    {
		 (yyval.tree) = T_crossing_opt((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 2238 "grammar.y"
    {
		 (yyval.tree) = T_crossing_weight((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 2242 "grammar.y"
    {
		 (yyval.tree) = T_view_method((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       	}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 2245 "grammar.y"
    {
		 (yyval.tree) = T_view_edges((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       	}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 2248 "grammar.y"
    {
		 (yyval.tree) = T_view_nodes((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       	}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 2251 "grammar.y"
    {
		 (yyval.tree) = T_view_splines((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  	}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 2255 "grammar.y"
    {
		 (yyval.tree) = T_bend_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   		}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 2258 "grammar.y"
    {
		 (yyval.tree) = T_cross_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  		}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 2261 "grammar.y"
    {
		 (yyval.tree) = T_cross_min((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  		}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 2264 "grammar.y"
    {
		 (yyval.tree) = T_pendel_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 		}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 2267 "grammar.y"
    {
		 (yyval.tree) = T_pendel_min((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  	}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 2270 "grammar.y"
    {
		 (yyval.tree) = T_rubber_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 		}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 2273 "grammar.y"
    {
		 (yyval.tree) = T_rubber_min((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  	}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 2276 "grammar.y"
    {
		 (yyval.tree) = T_straight_max((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); 	}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 2280 "grammar.y"
    {
		 (yyval.tree) = T_typename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));           }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 2283 "grammar.y"
    {
		 (yyval.tree) = T_include((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 2286 "grammar.y"
    {
		 (yyval.tree) = T_layoutparameter((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 2289 "grammar.y"
    {
		 (yyval.tree) = T_topsort((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 2292 "grammar.y"
    {
		 (yyval.tree) = T_inputfunction((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 2295 "grammar.y"
    {
		 (yyval.tree) = T_outputfunction((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 2298 "grammar.y"
    {
		 (yyval.tree) = T_xscrollbar((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 2301 "grammar.y"
    {
		 (yyval.tree) = T_yscrollbar((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 2306 "grammar.y"
    {
		 (yyval.tree) = T_aquamarine(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 2308 "grammar.y"
    {
		 (yyval.tree) = T_black(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 2310 "grammar.y"
    {
		 (yyval.tree) = T_blue(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 2312 "grammar.y"
    {
		 (yyval.tree) = T_cyan(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 2314 "grammar.y"
    {
		 (yyval.tree) = T_darkblue(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 2316 "grammar.y"
    {
		 (yyval.tree) = T_darkcyan(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 2318 "grammar.y"
    {
		 (yyval.tree) = T_darkgreen(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 2320 "grammar.y"
    {
		 (yyval.tree) = T_darkgrey(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 2322 "grammar.y"
    {
		 (yyval.tree) = T_darkmagenta(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 2324 "grammar.y"
    {
		 (yyval.tree) = T_darkred(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 2326 "grammar.y"
    {
		 (yyval.tree) = T_darkyellow(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 2328 "grammar.y"
    {
		 (yyval.tree) = T_gold(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 2330 "grammar.y"
    {
		 (yyval.tree) = T_green(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 2332 "grammar.y"
    {
		 (yyval.tree) = T_khaki(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 2334 "grammar.y"
    {
		 (yyval.tree) = T_lightblue(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 2336 "grammar.y"
    {
		 (yyval.tree) = T_lightcyan(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 2338 "grammar.y"
    {
		 (yyval.tree) = T_lightgreen(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 2340 "grammar.y"
    {
		 (yyval.tree) = T_lightgrey(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 2342 "grammar.y"
    {
		 (yyval.tree) = T_lightmagenta(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 2344 "grammar.y"
    {
		 (yyval.tree) = T_lightred(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 2346 "grammar.y"
    {
		 (yyval.tree) = T_lightyellow(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 2348 "grammar.y"
    {
		 (yyval.tree) = T_lilac(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 2350 "grammar.y"
    {
		 (yyval.tree) = T_magenta(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 2352 "grammar.y"
    {
		 (yyval.tree) = T_orange(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 2354 "grammar.y"
    {
		 (yyval.tree) = T_orchid(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 2356 "grammar.y"
    {
		 (yyval.tree) = T_pink(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 2358 "grammar.y"
    {
		 (yyval.tree) = T_purple(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 2360 "grammar.y"
    {
		 (yyval.tree) = T_red(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 2362 "grammar.y"
    {
		 (yyval.tree) = T_turquoise(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 2364 "grammar.y"
    {
		 (yyval.tree) = T_white(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 2366 "grammar.y"
    {
		 (yyval.tree) = T_yellow(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 2368 "grammar.y"
    {
		 (yyval.tree) = T_yellowgreen(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 2370 "grammar.y"
    {
		 (yyval.tree) = T_colindex((yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 2375 "grammar.y"
    {
		 (yyval.tree) = T_high(yylocate((yylsp[(1) - (1)]))); 	       }
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 2377 "grammar.y"
    {
		 (yyval.tree) = T_low(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 2382 "grammar.y"
    {
		 (yyval.tree) = T_top_to_bottom(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 2384 "grammar.y"
    {
		 (yyval.tree) = T_bottom_to_top(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 2386 "grammar.y"
    {
		 (yyval.tree) = T_left_to_right(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 2388 "grammar.y"
    {
		 (yyval.tree) = T_right_to_left(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 2394 "grammar.y"
    {
		 (yyval.tree) = T_barycenter(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 2396 "grammar.y"
    {
		 (yyval.tree) = T_isi(yylocate((yylsp[(1) - (1)])));           }
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 2398 "grammar.y"
    {
		 (yyval.tree) = T_planar(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 2400 "grammar.y"
    {
		 (yyval.tree) = T_constaints(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 2403 "grammar.y"
    {
		 (yyval.tree) = T_tree(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 2405 "grammar.y"
    {
		 (yyval.tree) = T_maxdepth(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 2407 "grammar.y"
    {
		 (yyval.tree) = T_mindepth(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 2409 "grammar.y"
    {
		 (yyval.tree) = T_maxdepthslow(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 2411 "grammar.y"
    {
		 (yyval.tree) = T_mindepthslow(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 2413 "grammar.y"
    {
		 (yyval.tree) = T_maxdegree(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 2415 "grammar.y"
    {
		 (yyval.tree) = T_mindegree(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 2417 "grammar.y"
    {
		 (yyval.tree) = T_maxindegree(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 2419 "grammar.y"
    {
		 (yyval.tree) = T_minindegree(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 2421 "grammar.y"
    {
		 (yyval.tree) = T_maxoutdegree(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 2423 "grammar.y"
    {
		 (yyval.tree) = T_minoutdegree(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 2425 "grammar.y"
    {
		 (yyval.tree) = T_minbackwards(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 2427 "grammar.y"
    {
		 (yyval.tree) = T_depthfirst(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 2433 "grammar.y"
    {
		 (yyval.tree) = T_every(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 2435 "grammar.y"
    {
		 (yyval.tree) = T_manual(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 2440 "grammar.y"
    {
		 (yyval.tree) = T_black(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 2442 "grammar.y"
    {
		 (yyval.tree) = T_grey(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 2444 "grammar.y"
    {
		 (yyval.tree) = T_white(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 2449 "grammar.y"
    {
		 (yyval.tree) = T_yes(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 2451 "grammar.y"
    {
		 (yyval.tree) = T_no(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 2455 "grammar.y"
    {
		 (yyval.tree) = T_bary(yylocate((yylsp[(1) - (1)]))); 	       }
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 2457 "grammar.y"
    {
		 (yyval.tree) = T_median(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 2459 "grammar.y"
    {
		 (yyval.tree) = T_barymedian(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 2461 "grammar.y"
    {
		 (yyval.tree) = T_medianbary(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 2465 "grammar.y"
    {
		 (yyval.tree) = T_cfish(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 2467 "grammar.y"
    {
		 (yyval.tree) = T_fcfish(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 2469 "grammar.y"
    {
		 (yyval.tree) = T_pfish(yylocate((yylsp[(1) - (1)]))); 	}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 2471 "grammar.y"
    {
		 (yyval.tree) = T_fpfish(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 2475 "grammar.y"
    {
		 (yyval.tree) = T_fixed(yylocate((yylsp[(1) - (1)])));         }
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 2477 "grammar.y"
    {
		 (yyval.tree) = T_free(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 2482 "grammar.y"
    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 2487 "grammar.y"
    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 2492 "grammar.y"
    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 2497 "grammar.y"
    {
		 (yyval.tree) = (yyvsp[(2) - (2)].tree);              }
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 2503 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 2509 "grammar.y"
    {
		 (yyval.tree) = T_node_attribute((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 2512 "grammar.y"
    {
		 (yyval.tree) = T_node_attribute((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 2518 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 2523 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 2528 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 2533 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 2539 "grammar.y"
    {
		 (yyval.tree) = T_edge_attribute((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 2542 "grammar.y"
    {
		 (yyval.tree) = T_edge_attribute((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 2548 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(3) - (4)].tree)); }
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 2555 "grammar.y"
    {
		 (yyval.tree) = T_constraint_attribute((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 2558 "grammar.y"
    {
		 (yyval.tree) = T_constraint_attribute((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 2564 "grammar.y"
    {
		 (yyval.tree) = T_title((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 2567 "grammar.y"
    {
		 (yyval.tree) = T_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 2570 "grammar.y"
    {
		 (yyval.tree) = T_info1((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 2573 "grammar.y"
    {
		 (yyval.tree) = T_info2((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 2576 "grammar.y"
    {
		 (yyval.tree) = T_info3((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 2579 "grammar.y"
    {
		 (yyval.tree) = T_fontname((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 2582 "grammar.y"
    {
		 (yyval.tree) = T_color((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 2585 "grammar.y"
    {
		 (yyval.tree) = T_textcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 2588 "grammar.y"
    {
		 (yyval.tree) = T_bordercolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 2591 "grammar.y"
    {
		 (yyval.tree) = T_iconfile((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 2594 "grammar.y"
    {
		 (yyval.tree) = T_anchorpoints((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 2597 "grammar.y"
    {
		 (yyval.tree) = T_typename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 2600 "grammar.y"
    {
		 (yyval.tree) = T_width((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 2603 "grammar.y"
    {
		 (yyval.tree) = T_height((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 2606 "grammar.y"
    {
		 (yyval.tree) = T_borderwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 2609 "grammar.y"
    {
		 (yyval.tree) = T_loc((yyvsp[(5) - (9)].tree),(yyvsp[(8) - (9)].tree),yylocate((yylsp[(1) - (9)])));       }
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 2612 "grammar.y"
    {
		 (yyval.tree) = T_folding((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 2615 "grammar.y"
    {
		 (yyval.tree) = T_scaling((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 2618 "grammar.y"
    {
		 (yyval.tree) = T_shrink((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));       }
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 2621 "grammar.y"
    {
		 (yyval.tree) = T_stretch((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 2624 "grammar.y"
    {
		 (yyval.tree) = T_iconwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 2627 "grammar.y"
    {
		 (yyval.tree) = T_iconheight((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 2630 "grammar.y"
    {
		 (yyval.tree) = T_textmode((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 2633 "grammar.y"
    {
		 (yyval.tree) = T_iconstyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 2636 "grammar.y"
    {
		 (yyval.tree) = T_shape((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    	 }
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 2639 "grammar.y"
    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 2642 "grammar.y"
    {
		 (yyval.tree) = T_level((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));            }
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 2645 "grammar.y"
    {
		 (yyval.tree) = T_horizontal_order((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 2649 "grammar.y"
    {
		 (yyval.tree) = T_center(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 2651 "grammar.y"
    {
		 (yyval.tree) = T_left_justify(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 2653 "grammar.y"
    {
		 (yyval.tree) = T_right_justify(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 2657 "grammar.y"
    {
		 (yyval.tree) = T_box(yylocate((yylsp[(1) - (1)]))); 		}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 2659 "grammar.y"
    {
		 (yyval.tree) = T_rhomb(yylocate((yylsp[(1) - (1)])));		}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 2661 "grammar.y"
    {
		 (yyval.tree) = T_ellipse(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 2663 "grammar.y"
    {
		 (yyval.tree) = T_triangle(yylocate((yylsp[(1) - (1)])));	}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 2667 "grammar.y"
    {
		 (yyval.tree) = T_bottom(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 2669 "grammar.y"
    {
		 (yyval.tree) = T_top(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 2671 "grammar.y"
    {
		 (yyval.tree) = T_center(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 2675 "grammar.y"
    {
		 (yyval.tree) = T_bottom(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 2677 "grammar.y"
    {
		 (yyval.tree) = T_top(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 2679 "grammar.y"
    {
		 (yyval.tree) = T_around(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 2685 "grammar.y"
    {
		 (yyval.tree) = T_sourcename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 2688 "grammar.y"
    {
		 (yyval.tree) = T_targetname((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 2691 "grammar.y"
    {
		 (yyval.tree) = T_label((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 2694 "grammar.y"
    {
		 (yyval.tree) = T_textcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));          }
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 2697 "grammar.y"
    {
		 (yyval.tree) = T_fontname((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 2700 "grammar.y"
    {
		 (yyval.tree) = T_color((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 2703 "grammar.y"
    {
		 (yyval.tree) = T_typename((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 2706 "grammar.y"
    {
		 (yyval.tree) = T_thickness((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 2709 "grammar.y"
    {
		 (yyval.tree) = T_class((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));         }
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 2712 "grammar.y"
    {
		 (yyval.tree) = T_priority((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 2715 "grammar.y"
    {
		 (yyval.tree) = T_arrowwidth((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 2718 "grammar.y"
    {
		 (yyval.tree) = T_arrowheight((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));   }
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 2721 "grammar.y"
    {
		 (yyval.tree) = T_arrowcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 2724 "grammar.y"
    {
		 (yyval.tree) = T_barrowcolor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 2727 "grammar.y"
    {
		 (yyval.tree) = T_arrowsize((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 2730 "grammar.y"
    {
		 (yyval.tree) = T_barrowsize((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 2733 "grammar.y"
    {
		 (yyval.tree) = T_arrowstyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 2736 "grammar.y"
    {
		 (yyval.tree) = T_barrowstyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 2739 "grammar.y"
    {
		 (yyval.tree) = T_linestyle((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 2742 "grammar.y"
    {
		 (yyval.tree) = T_anchor((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));        }
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 2745 "grammar.y"
    {
		 (yyval.tree) = T_horizontal_order((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 2750 "grammar.y"
    {
		 (yyval.tree) = T_continuous(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 2752 "grammar.y"
    {
		 (yyval.tree) = T_continuous(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 2754 "grammar.y"
    {
		 (yyval.tree) = T_dotted(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 2756 "grammar.y"
    {
		 (yyval.tree) = T_dashed(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 2758 "grammar.y"
    {
		 (yyval.tree) = T_invisible(yylocate((yylsp[(1) - (1)])));  }
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 2763 "grammar.y"
    {
		 (yyval.tree) = T_none(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 2765 "grammar.y"
    {
		 (yyval.tree) = T_line(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 2767 "grammar.y"
    {
		 (yyval.tree) = T_solid(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 2772 "grammar.y"
    {
		 (yyval.tree) = T_title((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));     }
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 2775 "grammar.y"
    {
		 (yyval.tree) = T_priority((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 2778 "grammar.y"
    {
		 (yyval.tree) = T_size((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 2781 "grammar.y"
    {
		 (yyval.tree) = T_nodes((yyvsp[(4) - (5)].tree),yylocate((yylsp[(1) - (5)])));     }
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 2784 "grammar.y"
    {
		 (yyval.tree) = T_interval((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));  }
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 2787 "grammar.y"
    {
		 (yyval.tree) = T_name((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));      }
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 2790 "grammar.y"
    {
		 (yyval.tree) = T_dimension((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 2795 "grammar.y"
    {
		 (yyval.tree) = T_string_array((yyvsp[(1) - (2)].tree),(yyvsp[(2) - (2)].tree),yylocate((yylsp[(1) - (2)])));    }
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 2797 "grammar.y"
    {
		 (yyval.tree) = T_string_array(NULL,(yyvsp[(1) - (1)].tree),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 2801 "grammar.y"
    {
		 (yyval.tree) = T_equal(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 2803 "grammar.y"
    {
		 (yyval.tree) = T_smaller(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2805 "grammar.y"
    {
		 (yyval.tree) = T_greater(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2807 "grammar.y"
    {
		 (yyval.tree) = T_neighbors(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2809 "grammar.y"
    {
		 (yyval.tree) = T_low_margin(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2811 "grammar.y"
    {
		 (yyval.tree) = T_high_margin(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2813 "grammar.y"
    {
		 (yyval.tree) = T_xrange(yylocate((yylsp[(1) - (1)])));           }
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2815 "grammar.y"
    {
		 (yyval.tree) = T_cluster(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2817 "grammar.y"
    {
		 (yyval.tree) = T_limit(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2819 "grammar.y"
    {
		 (yyval.tree) = T_above(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2821 "grammar.y"
    {
		 (yyval.tree) = T_below(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2823 "grammar.y"
    {
		 (yyval.tree) = T_left(yylocate((yylsp[(1) - (1)])));             }
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2825 "grammar.y"
    {
		 (yyval.tree) = T_right(yylocate((yylsp[(1) - (1)])));            }
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2827 "grammar.y"
    {
		 (yyval.tree) = T_in_font(yylocate((yylsp[(1) - (1)])));          }
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2829 "grammar.y"
    {
		 (yyval.tree) = T_behind(yylocate((yylsp[(1) - (1)])));           }
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2831 "grammar.y"
    {
		 (yyval.tree) = T_equal_position(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2833 "grammar.y"
    {
		 (yyval.tree) = T_equal_row(yylocate((yylsp[(1) - (1)])));        }
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2835 "grammar.y"
    {
		 (yyval.tree) = T_equal_column(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2837 "grammar.y"
    {
		 (yyval.tree) = T_top_margin(yylocate((yylsp[(1) - (1)])));       }
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2839 "grammar.y"
    {
		 (yyval.tree) = T_bottom_margin(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2841 "grammar.y"
    {
		 (yyval.tree) = T_left_margin(yylocate((yylsp[(1) - (1)])));      }
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2843 "grammar.y"
    {
		 (yyval.tree) = T_right_margin(yylocate((yylsp[(1) - (1)])));     }
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2845 "grammar.y"
    {
		 (yyval.tree) = T_upper_neighbor(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2847 "grammar.y"
    {
		 (yyval.tree) = T_lower_neighbor(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2849 "grammar.y"
    {
		 (yyval.tree) = T_left_neighbor(yylocate((yylsp[(1) - (1)])));    }
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2851 "grammar.y"
    {
		 (yyval.tree) = T_right_neighbor(yylocate((yylsp[(1) - (1)])));   }
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2855 "grammar.y"
    {
		 (yyval.tree) = T_x(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2857 "grammar.y"
    {
		 (yyval.tree) = T_y(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2859 "grammar.y"
    {
		 (yyval.tree) = T_z(yylocate((yylsp[(1) - (1)]))); }
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2863 "grammar.y"
    {
		 (yyval.tree) = T_integer((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)]))); }
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2865 "grammar.y"
    {
		 (yyval.tree) = T_float((yyvsp[(1) - (1)].lrealnum),yylocate((yylsp[(1) - (1)])));   }
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2867 "grammar.y"
    {
		 (yyval.tree) = T_char((yyvsp[(1) - (1)].byte),yylocate((yylsp[(1) - (1)])));    }
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 2869 "grammar.y"
    {
		 (yyval.tree) = T_string((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 2871 "grammar.y"
    {
		 (yyval.tree) = (yyvsp[(1) - (1)].tree); }
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2876 "grammar.y"
    {
		 (yyval.tree) = REVERT((yyvsp[(2) - (3)].tree)); }
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2881 "grammar.y"
    {
		 (yyval.tree) = T_index_value((yyvsp[(2) - (2)].tree),(yyvsp[(1) - (2)].tree),yylocate((yylsp[(1) - (2)])));   }
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2883 "grammar.y"
    {
		 (yyval.tree) = T_index_value((yyvsp[(1) - (1)].tree),NULL,yylocate((yylsp[(1) - (1)]))); }
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2887 "grammar.y"
    {
		 (yyval.tree) = (yyvsp[(1) - (1)].tree);           }
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2890 "grammar.y"
    {
		 (yyval.tree) = T_index((yyvsp[(1) - (3)].tree),(yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2893 "grammar.y"
    {
		 (yyval.tree) = T_range((yyvsp[(1) - (3)].tree),(yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)]))); }
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2896 "grammar.y"
    {
		 (yyval.tree) = T_stern((yyvsp[(3) - (3)].tree),yylocate((yylsp[(1) - (3)])));    }
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2901 "grammar.y"
    {
		 (yyval.tree) = T_range((yyvsp[(2) - (5)].tree),(yyvsp[(4) - (5)].tree),yylocate((yylsp[(1) - (5)]))); }
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2905 "grammar.y"
    {
		 (yyval.tree) = T_index_val((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));    }
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2909 "grammar.y"
    {
		 (yyval.tree) = T_integer((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));  }
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2913 "grammar.y"
    {
		 (yyval.tree) = T_float((yyvsp[(1) - (1)].lrealnum),yylocate((yylsp[(1) - (1)])));    }
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2917 "grammar.y"
    {
		 (yyval.tree) = T_string((yyvsp[(1) - (1)].lnum),yylocate((yylsp[(1) - (1)])));   }
    break;



/* Line 1455 of yacc.c  */
#line 6898 "y.tab.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

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

  yyerror_range[0] = yylsp[1-yylen];
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
      if (yyn != YYPACT_NINF)
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
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



/* Line 1675 of yacc.c  */
#line 2923 "grammar.y"





/*----------------------------*/
/* Main entry point of parser */
/*----------------------------*/

#ifndef yyparseinit
#define yyparseinit() /*NOTHING*/
#endif

yysyntaxtree  Syntax_Tree;

/*   returns the number of detected errors and binds the syntaxtree
 *   to the variable syntaxtree.
 */

#ifdef ANSI_C
int parse(void)
#else
int parse()
#endif
{
	nr_errors=0;
	debugmessage("init_lex()\n", "");
        init_lex();
	line_nr = 1;
	pos_nr = 0;
	debugmessage("yyparseinit()\n", "");
	yyparseinit();	
	debugmessage("yyparse()\n", "");
        yyparse();
#ifdef YYVALGLOBAL
        Syntax_Tree = yyval.tree;
#endif
        return(nr_errors);
}
 





/*--------------------------------------------------------------*/
/*   Line directives                                            */
/*--------------------------------------------------------------*/
 
/*  Handle directive left by the C preprocessor, i.e.
 *
 *    # line 123 "foo.h"
 *    # line 125
 *    # 126 "foo.h" 2
 *
 *  et cetera.
 */
void line_directive(char *text)
{
        char *c,*d;

        c = d = text;
        while ((*c) && (*c!='"')) c++;
        while ((*d) && ((*d<'0') || (*d>'9'))) d++;
 
        if (d<c) { line_nr = atoi(d); pos_nr = 0; }
 
        if (*c) {
                c++;
                d = c;
                while ((*d) && (*d!='"')) d++;
                *d=0;
                strcpy(filename, c);
        }
}


/*--------------------------------------------------------------*/
/*   Translate escape sequences in strings                      */
/*--------------------------------------------------------------*/
 
/*  Translate escape sequences in strings. The result remains
 *  in text, because the result is shorter !
 *
 *   \\ -> \        \" -> "     \n -> CR    
 *
 *  et cetera.
 */
void escape_transl(char *text)
{
        char *c,*d;
	int i;

        c = d = text;
	while (*c) {
		if (*c == '\\') {
			c++;
			switch (*c) {
			case 'a' : *d++ = '\a'; break;
			case 'b' : *d++ = '\b'; break;
			case 'f' : *d++ = '\f'; break;
			case 'n' : *d++ = '\n'; break;
			case 'r' : *d++ = '\r'; break;
			case 't' : *d++ = '\t'; break;
			case 'v' : *d++ = '\v'; break;
			case 'x' :
				   c++;
				   i = 0;
				   while (  ((*c>='0')&&(*c<='9')) 
					  ||((*c>='A')&&(*c<='F'))
					  ||((*c>='a')&&(*c<='f')) ) {
					if ((*c>='0')&&(*c<='9')) 
						i = i*16 + (*c)-'0'; 
					else if ((*c>='A')&&(*c<='F')) 
						i = i*16 + (*c)-'A'+10; 
					else if ((*c>='a')&&(*c<='f')) 
						i = i*16 + (*c)-'a'+10; 
					c++;
				   }
				   c--;
				   *d++ = i;
				   break;
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
				  i = (*c) - '0';
				  c++;
				  if ((*c>='0') && (*c<='7')) {
					i = i*8 +(*c)-'0';
				  	c++;
				  	if ((*c>='0') && (*c<='7')) {
						i = i*8 +(*c)-'0';
						c++;
					}
				  }
				  c--;
				  *d++ = i;
				  break;
			default : *d++ = *c; break;
			}
		}
		else *d++ = *c;
		c++;
	}
	*d = 0;
}


/*====================================================================*/
/*   Errors and Warnings                                              */
/*====================================================================*/

/*
 *   syntaxerror prints an error message with position of the
 *   error place in the specification, and increments the counter of
 *   errors.
 */

static void fatal_error _PP((char *));

static char myprivmessage[16000];  /* Please DON'T reuse this */

void syntaxerror(int line, int pos, char *mesge)
{
        strcpy(myprivmessage,mesge);
        if (islower(*myprivmessage))
                *myprivmessage = toupper(*myprivmessage);
        FPRINTF(stderr,"Syntax error (%s: l:%d p:%d): %s !\n",
                filename,line,pos,myprivmessage);
        nr_errors++;
        if (nr_errors>nr_max_errors)
                fatal_error("Too many syntax errors");
}


/*
 *   warning prints a warning with position of the problematic place
 *   in the specification, but does not increment the counter of
 *   errors.
 */

void warning(int line, int pos, char *mesge)
{
        strcpy(myprivmessage,mesge);
        if (islower(*myprivmessage))
                *myprivmessage = toupper(*myprivmessage);
        FPRINTF(stderr,"Warning (%s: l:%d p:%d): %s !\n",
                filename,line,pos,myprivmessage);
}


/*--------------------------------------------------------------*/






/*--------------------------------------------------------------------*/
/*  Standard Tree Construction Routines                               */
/*--------------------------------------------------------------------*/

#ifdef PARSEGENSTD 


#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>

/*--------------------------------------------------------------------*/
/* Fatal error: exit with message                                     */
/*--------------------------------------------------------------------*/

static void fatal_error(char *message)
{
        (void)fprintf(stderr,"Fatal error: %s !\n",message);
        (void)fprintf(stderr,"Aborted !\n");
        exit(-1);
}

/*--------------------------------------------------------------------*/
/* Memory allocation for syntax tree nodes                            */
/*--------------------------------------------------------------------*/

#ifndef ALIGN
#define ALIGN 8
#define IALIGN (ALIGN-1)
#endif

#ifndef PARSEBLOCKSIZE
#define PARSEBLOCKSIZE 10000
#endif


/*   The following in invisible from outside:        
 *   The heap consists of a list of memory blocks of size parseheapsize.
 *   The parseheap is the actual memory block.
 */ 

static yysyntaxtree parseheap = (yysyntaxtree)0;  /* the heap */ 
static yysyntaxtree parseheapstart;               /* the base */
static yysyntaxtree parseheaptop;                 /* the top  */
static yysyntaxtree parseheapend;                 /* the end  */
static int parseheapsize = PARSEBLOCKSIZE;     /* the size of one block */


static void alloc_block(void)
{
        yysyntaxtree help, *help2;

	help =(yysyntaxtree)malloc(parseheapsize*sizeof(struct stree_node));
        if (!help) fatal_error("memory exhausted");
	help2  = (yysyntaxtree *)help;
	*help2 = (yysyntaxtree)parseheap;
	parseheap = help;
	parseheapstart = parseheaptop =
		(yysyntaxtree)((long)parseheap + (long)sizeof(yysyntaxtree));
	parseheapend = parseheap;
	parseheapend += (parseheapsize-2);
        if ((long)parseheaptop&IALIGN) 
		parseheaptop = (yysyntaxtree)
			(((long)parseheaptop+(long)IALIGN)&(long)(~IALIGN));
}


/*  allocate x bytes */

static yysyntaxtree parsemalloc(int x)
{
        yysyntaxtree help;
        int  y;

	if (!parseheap) alloc_block();

        y = x;
        if (y&IALIGN) y = (y+IALIGN)&(~IALIGN);

        help = parseheaptop;
        parseheaptop = (yysyntaxtree)((long)parseheaptop+(long)y);

        if (parseheaptop > parseheapend) {

		/* heap too small -> allocate new heap block */

		alloc_block();
                help = parseheaptop;
                parseheaptop = (yysyntaxtree)((long)parseheaptop+(long)y);
        	if (parseheaptop > parseheapend) 
			fatal_error("syntax tree node too large");
        }
        return (help);
}


/* allocate yysyntaxtree node with x sons */

static yysyntaxtree st_malloc(int x)
{
        yysyntaxtree help;

        help =parsemalloc(sizeof(struct stree_node)+x*sizeof(yysyntaxtree));
        return (help);
}


/* global allocate x bytes */

#ifdef ANSI_C
char * ParseMalloc(int x)
#else
char * ParseMalloc(x)
int x;
#endif
{
	return((char *)parsemalloc(x));
}

/* global deallocate the complete syntax tree heap */

static union special *stdunion = 0;

#ifdef ANSI_C
void ParseFree(void)
#else
void ParseFree()
#endif
{
        yysyntaxtree help, help2;

	help = parseheap;
	while (help) {
		help2 = *(yysyntaxtree *)help;
		(void)free((char *)help);
		help = help2;
	}	

	parseheap = (yysyntaxtree)0;
	stdunion = 0;
}
 
/*--------------------------------------------------------------------*/
/* Create unions for syntax tree nodes                                */
/*--------------------------------------------------------------------*/


#ifdef ANSI_C
static void freeunion(union special *x)
#else
static void freeunion(x)
union special *x;
#endif
{
	x->string = (char *)stdunion;
	stdunion = x;	
}

#ifdef ANSI_C
static union special *nextunion(void)
#else
static union special *nextunion()
#endif
{
	union special *help;
	if (!stdunion) {
        	stdunion = (union special *)
				parsemalloc(sizeof(union special));
		stdunion->string = 0;
	}
	help = stdunion;
	stdunion = (union special *)stdunion->string;
	return(help);
}

#ifdef ANSI_C
union special *UnionByte(unsigned char x)
#else
union special *UnionByte(x)
unsigned char x;
#endif
{
	union special *help;

	help = nextunion();
	help->byte = x;
	return(help);
}

#ifdef ANSI_C
union special *UnionSnum(short int x)
#else
union special *UnionSnum(x)
short int x;
#endif
{
	union special *help;

	help = nextunion();
	help->snum = x;
	return(help);
}

#ifdef ANSI_C
union special *UnionUsnum(unsigned short int x)
#else
union special *UnionUsnum(x)
unsigned short int x;
#endif
{
	union special *help;

	help = nextunion();
	help->usnum = x;
	return(help);
}

#ifdef ANSI_C
union special *UnionNum(int x)
#else
union special *UnionNum(x)
int x;
#endif
{
	union special *help;

	help = nextunion();
	help->num = x;
	return(help);     
}

#ifdef ANSI_C
union special *UnionUnum(unsigned int x)
#else
union special *UnionUnum(x)
unsigned int x;
#endif
{
	union special *help;

	help = nextunion();
	help->unum = x;
	return(help);     
}

#ifdef ANSI_C
union special *UnionLnum(long int x)
#else
union special *UnionLnum(x)
long int x;
#endif
{
	union special *help;

	help = nextunion();
	help->lnum = x;
	return(help);     
}

#ifdef ANSI_C
union special *UnionUlnum(unsigned long int x)
#else
union special *UnionUlnum(x)
unsigned long int x;
#endif
{
	union special *help;

	help = nextunion();
	help->ulnum = x;
	return(help);     
}

#ifdef ANSI_C
union special *UnionRealnum(float x)
#else
union special *UnionRealnum(x)
float x;
#endif
{
	union special *help;

	help = nextunion();
	help->realnum = x;
	return(help);     
}

#ifdef ANSI_C
union special *UnionLrealnum(double x)
#else
union special *UnionLrealnum(x)
double x;
#endif
{
	union special *help;

	help = nextunion();
	help->lrealnum = x;
	return(help);     
}

#ifdef ANSI_C
union special *UnionString(char *x)
#else
union special *UnionString(x)
char *x;
#endif
{
	union special *help;

	help = nextunion();
	help->string = x;
	return(help);     
}


/*--------------------------------------------------------------------*/
/* Build syntax tree nodes                                            */
/*--------------------------------------------------------------------*/

/* Table for varargs */

static yysyntaxtree TreeTab[1024];

#ifdef USERFTYPE
#ifndef USERFINIT
#define USERFINIT ((USERFTYPE)0)
#endif
#endif

/* without sons */

#ifdef ANSI_C
syntaxtree BuildCont(int mtag,union special *x,YYLTYPE *l)
#else
syntaxtree BuildCont(mtag,x,l)
int mtag;
union special *x;
YYLTYPE *l;
#endif
{
        yysyntaxtree help;
        help = st_malloc(1);

	__yy_bcopy(x,&help->contents, sizeof(union special));
	freeunion(x);
	tag(help)          = mtag;
	xfirst_line(help)   =l->first_line;
	xfirst_column(help) =l->first_column;
	xlast_line(help)    =l->last_line;
	xlast_column(help)  =l->last_column;
	xfather(help)       =(yysyntaxtree)0;
#ifdef USERFTYPE
	user_field(help)    =USERFINIT; 
#endif

	son1(help) = (yysyntaxtree)0; 

        return(help);
}

/* with sons */

#ifdef ANSI_C
yysyntaxtree BuildTree(int mtag,int len,union special *x,YYLTYPE *l, ...)
#else
yysyntaxtree BuildTree(va_alist)
va_dcl
#endif
{
	int i,j;
	va_list pvar;
        yysyntaxtree help;

#ifndef ANSI_C
	int mtag;
	int len;
	union special *x;
	YYLTYPE *l;
	va_start(pvar);
#endif

#ifdef ANSI_C
	va_start(pvar,l);
#endif

#ifndef ANSI_C
	mtag = va_arg(pvar, int);
	len = va_arg(pvar, int);
	x   = va_arg(pvar, union special *);
	l   = va_arg(pvar, YYLTYPE *);
#endif
 
	i = 0;
	help = va_arg(pvar, yysyntaxtree);
	while (i < len) {
		TreeTab[i++] = help;
		help = va_arg(pvar, yysyntaxtree);
	}
	va_end(pvar);

        help = st_malloc((i<1?1:i));

	__yy_bcopy(x,&help->contents, sizeof(union special));
	freeunion(x);
	tag(help)          = mtag;
	xfirst_line(help)   =l->first_line;
	xfirst_column(help) =l->first_column;
	xlast_line(help)    =l->last_line;
	xlast_column(help)  =l->last_column;
	xfather(help)       =(yysyntaxtree)0;
#ifdef USERFTYPE
	user_field(help)    =USERFINIT; 
#endif

	for (j=1; j<=i; j++) {
		son(help,j) = TreeTab[j-1]; 
		if (TreeTab[j-1]!=(yysyntaxtree)0)
			xfather(TreeTab[j-1]) = help;
	}
        return(help);
}


/* copy syntax tree */

#ifdef ANSI_C
yysyntaxtree Copy(yysyntaxtree x)
#else
yysyntaxtree Copy(x)
yysyntaxtree x;
#endif
{
	register int j,len;
        yysyntaxtree help; 

	if (!x) return(x);
	len = nr_of_sons(x);
        help = st_malloc((len<1?1:len));

	__yy_bcopy(&x->contents,&help->contents, sizeof(union special));
	tag(help)          = tag(x);
	xfirst_line(help)   = xfirst_line(x);
	xfirst_column(help) = xfirst_column(x);
	xlast_line(help)    = xlast_line(x);
	xlast_column(help)  = xlast_column(x);
	xfather(help)       = (yysyntaxtree)0;
#ifdef USERFTYPE
	user_field(help)    =user_field(x); 
#endif
	son1(help) = (yysyntaxtree)0; 

	for (j=1; j<=len; j++) {
		son(help,j) = Copy(son(x,j)); 
		if (son(help,j))
			xfather(son(help,j)) = help; 
	}
        return(help);
}

/* revert list */

#ifdef ANSI_C
yysyntaxtree Revert(yysyntaxtree list)
#else
yysyntaxtree Revert(list)
yysyntaxtree list;
#endif
{
        yysyntaxtree last, act, next; 

	last = (yysyntaxtree)0;
	act  = list;
	while (act) {
		next = son2(act);
		son2(act) = last;
		if (last) xfather(last)=act;
		last = act;	
		act = next;
	}
	if (last) xfather(last)=(yysyntaxtree)0;
        return(last);
}

/*--------------------------------------------------------------------*/
/* yield constructor name                                             */
/*--------------------------------------------------------------------*/

#ifdef ANSI_C
const char *ConstructorName(int i)
#else
const char *ConstructorName(i)
int i;
#endif
{
	return(yyconst_name[i]);
}

/*--------------------------------------------------------------------*/
/* yield constructor arity                                            */
/*--------------------------------------------------------------------*/

#ifdef ANSI_C
int ConstructorArity(int i)
#else
int ConstructorArity(i)
int i;
#endif
{
	return(yyconst_arity[i]);
}


#endif /* PARSEGENSTD */

/*--------------------------------------------------------------------*/




