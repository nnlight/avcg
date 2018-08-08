/*--------------------------------------------------------------------*/
/*              Scanner and Parser Interface                          */
/*--------------------------------------------------------------------*/

#ifndef SCANPARSE_H
#define SCANPARSE_H


extern int line_nr;
extern int pos_nr;
extern int nr_errors;
#define FILENAME_BUF_SIZE 128
extern char filename[FILENAME_BUF_SIZE];

extern void init_lex( FILE *f, const char *fname);
extern int parse();


void lex_rule_match( char *text);
void   line_directive (char *text);
void   escape_transl  (char *text);
void   syntaxerror    (int line, int pos, const char *fmt, ...);
void   fatal_error(const char *message);



void FreeHash(void);
long HashInsert( char *s);
long HashTableSize(void);
char *Decode(long x);




typedef struct stree_node *yysyntaxtree;

extern yysyntaxtree Syntax_Tree;

/*--------------------------------------------------------------------*/
/*  Standard Tree Construction Interface                              */
/*--------------------------------------------------------------------*/

/*-------------------*/
/* syntax tree nodes */
/*-------------------*/

union  special {
        unsigned char      byte;
        long int           lnum;
        double             lrealnum;
        char              *string;
};

struct stree_node {
        int  tag_field;
        int  first_line;
        int  first_column;
        int  last_line;
        int  last_column;
        struct stree_node *father;
        union  special     contents;
        struct stree_node *xson[0];
};

#define tag(x)           ((x)->tag_field)
#define nr_of_sons(x)    (ConstructorArity((x)->tag_field))
#define xfirst_line(x)    ((x)->first_line)
#define xfirst_column(x)  ((x)->first_column)
#define xlast_line(x)     ((x)->last_line)
#define xlast_column(x)   ((x)->last_column)
#define xfather(x)        ((x)->father)

#define get_byte(x)      ((x)->contents.byte)
#define get_lnum(x)      ((x)->contents.lnum)
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

extern int yylex();

#define Y_TAB_H
#endif /* Y_TAB_H */


/*------------*/
/* Prototypes */
/*------------*/

char * ParseMalloc(int x);
void ParseFree(void);

union special UnionByte(unsigned char x);
union special UnionLnum(long int x);
union special UnionLrealnum(double x);
union special UnionString(char *x);

yysyntaxtree BuildCont(int mtag, union special x, YYLTYPE *l);
yysyntaxtree BuildTree(int mtag, int len, YYLTYPE *l, ...);

yysyntaxtree Copy(yysyntaxtree x);
yysyntaxtree Revert(yysyntaxtree list);

const char *ConstructorName(int mtag);
int ConstructorArity(int mtag);


/*-- end of standard tree construction interface ---------------------*/



/* Constructors: */
typedef enum st_tag {
    #define ST_NODE(name, arity, contents_type) name,
    #include "parse_gdl.mac.h"
    ST_TAG_LAST
} st_tag_t;

/* Build Macros */

#define UnionFake() UnionString(NULL)

#define T_index_val(s0,l) BuildCont(T_Co_index_val,UnionLnum(s0),l)
#define T_stern(s0,l) BuildTree(T_Co_stern,1,l,s0)
#define T_range(s0,s1,l) BuildTree(T_Co_range,2,l,s0,s1)
#define T_index(s0,s1,l) BuildTree(T_Co_index,2,l,s0,s1)
#define T_index_value(s0,s1,l) BuildTree(T_Co_index_value,2,l,s0,s1)
#define T_string(s0,l) BuildCont(T_Co_string,UnionLnum(s0),l)
#define T_char(s0,l) BuildCont(T_Co_char,UnionByte(s0),l)
#define T_float(s0,l) BuildCont(T_Co_float,UnionLrealnum(s0),l)
#define T_integer(s0,l) BuildCont(T_Co_integer,UnionLnum(s0),l)
#define T_z(l) BuildCont(T_Co_z,UnionFake(),l)
#define T_y(l) BuildCont(T_Co_y,UnionFake(),l)
#define T_x(l) BuildCont(T_Co_x,UnionFake(),l)
#define T_right_neighbor(l) BuildCont(T_Co_right_neighbor,UnionFake(),l)
#define T_left_neighbor(l) BuildCont(T_Co_left_neighbor,UnionFake(),l)
#define T_lower_neighbor(l) BuildCont(T_Co_lower_neighbor,UnionFake(),l)
#define T_upper_neighbor(l) BuildCont(T_Co_upper_neighbor,UnionFake(),l)
#define T_right_margin(l) BuildCont(T_Co_right_margin,UnionFake(),l)
#define T_left_margin(l) BuildCont(T_Co_left_margin,UnionFake(),l)
#define T_bottom_margin(l) BuildCont(T_Co_bottom_margin,UnionFake(),l)
#define T_top_margin(l) BuildCont(T_Co_top_margin,UnionFake(),l)
#define T_equal_column(l) BuildCont(T_Co_equal_column,UnionFake(),l)
#define T_equal_row(l) BuildCont(T_Co_equal_row,UnionFake(),l)
#define T_equal_position(l) BuildCont(T_Co_equal_position,UnionFake(),l)
#define T_behind(l) BuildCont(T_Co_behind,UnionFake(),l)
#define T_in_font(l) BuildCont(T_Co_in_font,UnionFake(),l)
#define T_right(l) BuildCont(T_Co_right,UnionFake(),l)
#define T_left(l) BuildCont(T_Co_left,UnionFake(),l)
#define T_below(l) BuildCont(T_Co_below,UnionFake(),l)
#define T_above(l) BuildCont(T_Co_above,UnionFake(),l)
#define T_limit(l) BuildCont(T_Co_limit,UnionFake(),l)
#define T_cluster(l) BuildCont(T_Co_cluster,UnionFake(),l)
#define T_xrange(l) BuildCont(T_Co_xrange,UnionFake(),l)
#define T_high_margin(l) BuildCont(T_Co_high_margin,UnionFake(),l)
#define T_low_margin(l) BuildCont(T_Co_low_margin,UnionFake(),l)
#define T_neighbors(l) BuildCont(T_Co_neighbors,UnionFake(),l)
#define T_greater(l) BuildCont(T_Co_greater,UnionFake(),l)
#define T_smaller(l) BuildCont(T_Co_smaller,UnionFake(),l)
#define T_equal(l) BuildCont(T_Co_equal,UnionFake(),l)
#define T_string_array(s0,s1,l) BuildTree(T_Co_string_array,2,l,s0,s1)
#define T_dimension(s0,l) BuildTree(T_Co_dimension,1,l,s0)
#define T_name(s0,l) BuildTree(T_Co_name,1,l,s0)
#define T_interval(s0,l) BuildTree(T_Co_interval,1,l,s0)
#define T_nodes(s0,l) BuildTree(T_Co_nodes,1,l,s0)
#define T_size(s0,l) BuildTree(T_Co_size,1,l,s0)
#define T_solid(l) BuildCont(T_Co_solid,UnionFake(),l)
#define T_line(l) BuildCont(T_Co_line,UnionFake(),l)
#define T_none(l) BuildCont(T_Co_none,UnionFake(),l)
#define T_invisible(l) BuildCont(T_Co_invisible,UnionFake(),l)
#define T_dashed(l) BuildCont(T_Co_dashed,UnionFake(),l)
#define T_dotted(l) BuildCont(T_Co_dotted,UnionFake(),l)
#define T_continuous(l) BuildCont(T_Co_continuous,UnionFake(),l)
#define T_anchor(s0,l) BuildTree(T_Co_anchor,1,l,s0)
#define T_linestyle(s0,l) BuildTree(T_Co_linestyle,1,l,s0)
#define T_barrowstyle(s0,l) BuildTree(T_Co_barrowstyle,1,l,s0)
#define T_arrowstyle(s0,l) BuildTree(T_Co_arrowstyle,1,l,s0)
#define T_barrowsize(s0,l) BuildTree(T_Co_barrowsize,1,l,s0)
#define T_arrowsize(s0,l) BuildTree(T_Co_arrowsize,1,l,s0)
#define T_barrowcolor(s0,l) BuildTree(T_Co_barrowcolor,1,l,s0)
#define T_arrowcolor(s0,l) BuildTree(T_Co_arrowcolor,1,l,s0)
#define T_arrowheight(s0,l) BuildTree(T_Co_arrowheight,1,l,s0)
#define T_arrowwidth(s0,l) BuildTree(T_Co_arrowwidth,1,l,s0)
#define T_priority(s0,l) BuildTree(T_Co_priority,1,l,s0)
#define T_class(s0,l) BuildTree(T_Co_class,1,l,s0)
#define T_thickness(s0,l) BuildTree(T_Co_thickness,1,l,s0)
#define T_targetname(s0,l) BuildTree(T_Co_targetname,1,l,s0)
#define T_sourcename(s0,l) BuildTree(T_Co_sourcename,1,l,s0)
#define T_around(l) BuildCont(T_Co_around,UnionFake(),l)
#define T_top(l) BuildCont(T_Co_top,UnionFake(),l)
#define T_bottom(l) BuildCont(T_Co_bottom,UnionFake(),l)
#define T_triangle(l) BuildCont(T_Co_triangle,UnionFake(),l)
#define T_ellipse(l) BuildCont(T_Co_ellipse,UnionFake(),l)
#define T_rhomb(l) BuildCont(T_Co_rhomb,UnionFake(),l)
#define T_box(l) BuildCont(T_Co_box,UnionFake(),l)
#define T_right_justify(l) BuildCont(T_Co_right_justify,UnionFake(),l)
#define T_left_justify(l) BuildCont(T_Co_left_justify,UnionFake(),l)
#define T_center(l) BuildCont(T_Co_center,UnionFake(),l)
#define T_iconstyle(s0,l) BuildTree(T_Co_iconstyle,1,l,s0)
#define T_iconheight(s0,l) BuildTree(T_Co_iconheight,1,l,s0)
#define T_iconwidth(s0,l) BuildTree(T_Co_iconwidth,1,l,s0)
#define T_anchorpoints(s0,l) BuildTree(T_Co_anchorpoints,1,l,s0)
#define T_iconfile(s0,l) BuildTree(T_Co_iconfile,1,l,s0)
#define T_bordercolor(s0,l) BuildTree(T_Co_bordercolor,1,l,s0)
#define T_fontname(s0,l) BuildTree(T_Co_fontname,1,l,s0)
#define T_constraint_attribute(s0,s1,l) BuildTree(T_Co_constraint_attribute,2,l,s0,s1)
#define T_edge_attribute(s0,s1,l) BuildTree(T_Co_edge_attribute,2,l,s0,s1)
#define T_node_attribute(s0,s1,l) BuildTree(T_Co_node_attribute,2,l,s0,s1)
#define T_free(l) BuildCont(T_Co_free,UnionFake(),l)
#define T_fixed(l) BuildCont(T_Co_fixed,UnionFake(),l)
#define T_fpfish(l) BuildCont(T_Co_fpfish,UnionFake(),l)
#define T_pfish(l) BuildCont(T_Co_pfish,UnionFake(),l)
#define T_fcfish(l) BuildCont(T_Co_fcfish,UnionFake(),l)
#define T_cfish(l) BuildCont(T_Co_cfish,UnionFake(),l)
#define T_medianbary(l) BuildCont(T_Co_medianbary,UnionFake(),l)
#define T_barymedian(l) BuildCont(T_Co_barymedian,UnionFake(),l)
#define T_median(l) BuildCont(T_Co_median,UnionFake(),l)
#define T_bary(l) BuildCont(T_Co_bary,UnionFake(),l)
#define T_no(l) BuildCont(T_Co_no,UnionFake(),l)
#define T_yes(l) BuildCont(T_Co_yes,UnionFake(),l)
#define T_grey(l) BuildCont(T_Co_grey,UnionFake(),l)
#define T_manual(l) BuildCont(T_Co_manual,UnionFake(),l)
#define T_every(l) BuildCont(T_Co_every,UnionFake(),l)
#define T_depthfirst(l) BuildCont(T_Co_depthfirst,UnionFake(),l)
#define T_minbackwards(l) BuildCont(T_Co_minbackwards,UnionFake(),l)
#define T_minoutdegree(l) BuildCont(T_Co_minoutdegree,UnionFake(),l)
#define T_maxoutdegree(l) BuildCont(T_Co_maxoutdegree,UnionFake(),l)
#define T_minindegree(l) BuildCont(T_Co_minindegree,UnionFake(),l)
#define T_maxindegree(l) BuildCont(T_Co_maxindegree,UnionFake(),l)
#define T_mindegree(l) BuildCont(T_Co_mindegree,UnionFake(),l)
#define T_maxdegree(l) BuildCont(T_Co_maxdegree,UnionFake(),l)
#define T_mindepthslow(l) BuildCont(T_Co_mindepthslow,UnionFake(),l)
#define T_maxdepthslow(l) BuildCont(T_Co_maxdepthslow,UnionFake(),l)
#define T_mindepth(l) BuildCont(T_Co_mindepth,UnionFake(),l)
#define T_maxdepth(l) BuildCont(T_Co_maxdepth,UnionFake(),l)
#define T_tree(l) BuildCont(T_Co_tree,UnionFake(),l)
#define T_constaints(l) BuildCont(T_Co_constaints,UnionFake(),l)
#define T_planar(l) BuildCont(T_Co_planar,UnionFake(),l)
#define T_isi(l) BuildCont(T_Co_isi,UnionFake(),l)
#define T_barycenter(l) BuildCont(T_Co_barycenter,UnionFake(),l)
#define T_right_to_left(l) BuildCont(T_Co_right_to_left,UnionFake(),l)
#define T_left_to_right(l) BuildCont(T_Co_left_to_right,UnionFake(),l)
#define T_bottom_to_top(l) BuildCont(T_Co_bottom_to_top,UnionFake(),l)
#define T_top_to_bottom(l) BuildCont(T_Co_top_to_bottom,UnionFake(),l)
#define T_low(l) BuildCont(T_Co_low,UnionFake(),l)
#define T_high(l) BuildCont(T_Co_high,UnionFake(),l)
#define T_colindex(s0,l) BuildTree(T_Co_colindex,1,l,s0)
#define T_yellowgreen(l) BuildCont(T_Co_yellowgreen,UnionFake(),l)
#define T_yellow(l) BuildCont(T_Co_yellow,UnionFake(),l)
#define T_white(l) BuildCont(T_Co_white,UnionFake(),l)
#define T_turquoise(l) BuildCont(T_Co_turquoise,UnionFake(),l)
#define T_red(l) BuildCont(T_Co_red,UnionFake(),l)
#define T_purple(l) BuildCont(T_Co_purple,UnionFake(),l)
#define T_pink(l) BuildCont(T_Co_pink,UnionFake(),l)
#define T_orchid(l) BuildCont(T_Co_orchid,UnionFake(),l)
#define T_orange(l) BuildCont(T_Co_orange,UnionFake(),l)
#define T_magenta(l) BuildCont(T_Co_magenta,UnionFake(),l)
#define T_lilac(l) BuildCont(T_Co_lilac,UnionFake(),l)
#define T_lightyellow(l) BuildCont(T_Co_lightyellow,UnionFake(),l)
#define T_lightred(l) BuildCont(T_Co_lightred,UnionFake(),l)
#define T_lightmagenta(l) BuildCont(T_Co_lightmagenta,UnionFake(),l)
#define T_lightgrey(l) BuildCont(T_Co_lightgrey,UnionFake(),l)
#define T_lightgreen(l) BuildCont(T_Co_lightgreen,UnionFake(),l)
#define T_lightcyan(l) BuildCont(T_Co_lightcyan,UnionFake(),l)
#define T_lightblue(l) BuildCont(T_Co_lightblue,UnionFake(),l)
#define T_khaki(l) BuildCont(T_Co_khaki,UnionFake(),l)
#define T_green(l) BuildCont(T_Co_green,UnionFake(),l)
#define T_gold(l) BuildCont(T_Co_gold,UnionFake(),l)
#define T_darkyellow(l) BuildCont(T_Co_darkyellow,UnionFake(),l)
#define T_darkred(l) BuildCont(T_Co_darkred,UnionFake(),l)
#define T_darkmagenta(l) BuildCont(T_Co_darkmagenta,UnionFake(),l)
#define T_darkgrey(l) BuildCont(T_Co_darkgrey,UnionFake(),l)
#define T_darkgreen(l) BuildCont(T_Co_darkgreen,UnionFake(),l)
#define T_darkcyan(l) BuildCont(T_Co_darkcyan,UnionFake(),l)
#define T_darkblue(l) BuildCont(T_Co_darkblue,UnionFake(),l)
#define T_cyan(l) BuildCont(T_Co_cyan,UnionFake(),l)
#define T_blue(l) BuildCont(T_Co_blue,UnionFake(),l)
#define T_black(l) BuildCont(T_Co_black,UnionFake(),l)
#define T_aquamarine(l) BuildCont(T_Co_aquamarine,UnionFake(),l)
#define T_yscrollbar(s0,l) BuildTree(T_Co_yscrollbar,1,l,s0)
#define T_xscrollbar(s0,l) BuildTree(T_Co_xscrollbar,1,l,s0)
#define T_outputfunction(s0,l) BuildTree(T_Co_outputfunction,1,l,s0)
#define T_inputfunction(s0,l) BuildTree(T_Co_inputfunction,1,l,s0)
#define T_topsort(s0,l) BuildTree(T_Co_topsort,1,l,s0)
#define T_layoutparameter(s0,l) BuildTree(T_Co_layoutparameter,1,l,s0)
#define T_include(s0,l) BuildTree(T_Co_include,1,l,s0)
#define T_typename(s0,l) BuildTree(T_Co_typename,1,l,s0)
#define T_straight_max(s0,l) BuildTree(T_Co_straight_max,1,l,s0)
#define T_rubber_min(s0,l) BuildTree(T_Co_rubber_min,1,l,s0)
#define T_rubber_max(s0,l) BuildTree(T_Co_rubber_max,1,l,s0)
#define T_pendel_min(s0,l) BuildTree(T_Co_pendel_min,1,l,s0)
#define T_pendel_max(s0,l) BuildTree(T_Co_pendel_max,1,l,s0)
#define T_cross_min(s0,l) BuildTree(T_Co_cross_min,1,l,s0)
#define T_cross_max(s0,l) BuildTree(T_Co_cross_max,1,l,s0)
#define T_bend_max(s0,l) BuildTree(T_Co_bend_max,1,l,s0)
#define T_view_splines(s0,l) BuildTree(T_Co_view_splines,1,l,s0)
#define T_view_nodes(s0,l) BuildTree(T_Co_view_nodes,1,l,s0)
#define T_view_edges(s0,l) BuildTree(T_Co_view_edges,1,l,s0)
#define T_view_method(s0,l) BuildTree(T_Co_view_method,1,l,s0)
#define T_crossing_weight(s0,l) BuildTree(T_Co_crossing_weight,1,l,s0)
#define T_crossing_opt(s0,l) BuildTree(T_Co_crossing_opt,1,l,s0)
#define T_crossing_phase2(s0,l) BuildTree(T_Co_crossing_phase2,1,l,s0)
#define T_treefactor(s0,l) BuildTree(T_Co_treefactor,1,l,s0)
#define T_spreadlevel(s0,l) BuildTree(T_Co_spreadlevel,1,l,s0)
#define T_arrow_mode(s0,l) BuildTree(T_Co_arrow_mode,1,l,s0)
#define T_port_sharing(s0,l) BuildTree(T_Co_port_sharing,1,l,s0)
#define T_node_alignment(s0,l) BuildTree(T_Co_node_alignment,1,l,s0)
#define T_orientation(s0,l) BuildTree(T_Co_orientation,1,l,s0)
#define T_dummy(l) BuildCont(T_Co_dummy,UnionFake(),l)
#define T_nonearedges(l) BuildCont(T_Co_nonearedges,UnionFake(),l)
#define T_smanhatten(s0,l) BuildTree(T_Co_smanhatten,1,l,s0)
#define T_manhatten(s0,l) BuildTree(T_Co_manhatten,1,l,s0)
#define T_priophase(s0,l) BuildTree(T_Co_priophase,1,l,s0)
#define T_straightphase(s0,l) BuildTree(T_Co_straightphase,1,l,s0)
#define T_hidesingles(s0,l) BuildTree(T_Co_hidesingles,1,l,s0)
#define T_finetuning(s0,l) BuildTree(T_Co_finetuning,1,l,s0)
#define T_dirty_edge_label(s0,l) BuildTree(T_Co_dirty_edge_label,1,l,s0)
#define T_display_edge_label(s0,l) BuildTree(T_Co_display_edge_label,1,l,s0)
#define T_late_edge_label(s0,l) BuildTree(T_Co_late_edge_label,1,l,s0)
#define T_splinefactor(s0,l) BuildTree(T_Co_splinefactor,1,l,s0)
#define T_nearfactor(s0,l) BuildTree(T_Co_nearfactor,1,l,s0)
#define T_upfactor(s0,l) BuildTree(T_Co_upfactor,1,l,s0)
#define T_downfactor(s0,l) BuildTree(T_Co_downfactor,1,l,s0)
#define T_layoutfrequency(s0,l) BuildTree(T_Co_layoutfrequency,1,l,s0)
#define T_layoutalgorithm(s0,l) BuildTree(T_Co_layoutalgorithm,1,l,s0)
#define T_colentry(s0,s1,s2,s3,l) BuildTree(T_Co_colentry,4,l,s0,s1,s2,s3)
#define T_infoname(s0,s1,l) BuildTree(T_Co_infoname,2,l,s0,s1)
#define T_classname(s0,s1,l) BuildTree(T_Co_classname,2,l,s0,s1)
#define T_hidden(s0,l) BuildTree(T_Co_hidden,1,l,s0)
#define T_yraster(s0,l) BuildTree(T_Co_yraster,1,l,s0)
#define T_xlraster(s0,l) BuildTree(T_Co_xlraster,1,l,s0)
#define T_xraster(s0,l) BuildTree(T_Co_xraster,1,l,s0)
#define T_yspace(s0,l) BuildTree(T_Co_yspace,1,l,s0)
#define T_xlspace(s0,l) BuildTree(T_Co_xlspace,1,l,s0)
#define T_xspace(s0,l) BuildTree(T_Co_xspace,1,l,s0)
#define T_ybase(s0,l) BuildTree(T_Co_ybase,1,l,s0)
#define T_xbase(s0,l) BuildTree(T_Co_xbase,1,l,s0)
#define T_ymax(s0,l) BuildTree(T_Co_ymax,1,l,s0)
#define T_xmax(s0,l) BuildTree(T_Co_xmax,1,l,s0)
#define T_status(s0,l) BuildTree(T_Co_status,1,l,s0)
#define T_horizontal_order(s0,l) BuildTree(T_Co_horizontal_order,1,l,s0)
#define T_level(s0,l) BuildTree(T_Co_level,1,l,s0)
#define T_shape(s0,l) BuildTree(T_Co_shape,1,l,s0)
#define T_textmode(s0,l) BuildTree(T_Co_textmode,1,l,s0)
#define T_stretch(s0,l) BuildTree(T_Co_stretch,1,l,s0)
#define T_shrink(s0,l) BuildTree(T_Co_shrink,1,l,s0)
#define T_scaling(s0,l) BuildTree(T_Co_scaling,1,l,s0)
#define T_folding(s0,l) BuildTree(T_Co_folding,1,l,s0)
#define T_loc(s0,s1,l) BuildTree(T_Co_loc,2,l,s0,s1)
#define T_ydef(s0,l) BuildTree(T_Co_ydef,1,l,s0)
#define T_xdef(s0,l) BuildTree(T_Co_xdef,1,l,s0)
#define T_borderwidth(s0,l) BuildTree(T_Co_borderwidth,1,l,s0)
#define T_height(s0,l) BuildTree(T_Co_height,1,l,s0)
#define T_width(s0,l) BuildTree(T_Co_width,1,l,s0)
#define T_colorborder(s0,l) BuildTree(T_Co_colorborder,1,l,s0)
#define T_textcolor(s0,l) BuildTree(T_Co_textcolor,1,l,s0)
#define T_color(s0,l) BuildTree(T_Co_color,1,l,s0)
#define T_info3(s0,l) BuildTree(T_Co_info3,1,l,s0)
#define T_info2(s0,l) BuildTree(T_Co_info2,1,l,s0)
#define T_info1(s0,l) BuildTree(T_Co_info1,1,l,s0)
#define T_label(s0,l) BuildTree(T_Co_label,1,l,s0)
#define T_title(s0,l) BuildTree(T_Co_title,1,l,s0)
#define T_constraint(s0,l) BuildTree(T_Co_constraint,1,l,s0)
#define T_back_edge(s0,l) BuildTree(T_Co_back_edge,1,l,s0)
#define T_bent_near_edge(s0,l) BuildTree(T_Co_bent_near_edge,1,l,s0)
#define T_near_edge(s0,l) BuildTree(T_Co_near_edge,1,l,s0)
#define T_edge(s0,l) BuildTree(T_Co_edge,1,l,s0)
#define T_node(s0,l) BuildTree(T_Co_node,1,l,s0)
#define T_graph(s0,l) BuildTree(T_Co_graph,1,l,s0)
#define T_foldedge_defaults(s0,l) BuildTree(T_Co_foldedge_defaults,1,l,s0)
#define T_foldnode_defaults(s0,l) BuildTree(T_Co_foldnode_defaults,1,l,s0)
#define T_edge_defaults(s0,l) BuildTree(T_Co_edge_defaults,1,l,s0)
#define T_node_defaults(s0,l) BuildTree(T_Co_node_defaults,1,l,s0)
#define T_graph_attribute(s0,l) BuildTree(T_Co_graph_attribute,1,l,s0)
#define T_graph_entry(s0,s1,l) BuildTree(T_Co_graph_entry,2,l,s0,s1)




/*
 * Scan Parse macros
 */

#define REVERT(x) Revert(x)
#define COPY(x)   Copy(x)




#endif  /* SCANPARSE_H */

