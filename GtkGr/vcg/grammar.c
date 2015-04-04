#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include "grammar.h"
#include "globals.h" /* for MEMBLOCKSIZE */



/*--------------------------------------------------------------------*/
/* Итоговое дерево, которое строится парсером (parse())               */
/*--------------------------------------------------------------------*/

yysyntaxtree  Syntax_Tree;



/*--------------------------------------------------------------------*/
/* Fatal error: exit with message                                     */
/*--------------------------------------------------------------------*/

void fatal_error(char *message)
{
	(void)fprintf(stderr,"Fatal error: %s !\n",message);
	(void)fprintf(stderr,"Aborted !\n");
	exit(-1);
}


/*====================================================================*/
/*   Errors and Warnings                                              */
/*====================================================================*/

/*  File Specifics 
 *  --------------
 *  Because of C-stile line directives, the file name used by the
 *  parser for error messages is not always the same as the filename
 *  of the actal input file. 
 */
char    filename[1024];     /* Filename from the view of the parser */ 

static int nr_max_errors = 16;
int nr_errors;

/*
 *   syntaxerror prints an error message with position of the
 *   error place, and increments the counter of errors.
 */
void syntaxerror(int line, int pos, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	(void)fprintf(stderr,"Syntax error (%s: l:%d p:%d): ",filename,line,pos);
	(void)vfprintf(stderr, fmt, args);
	(void)fprintf(stderr," !\n");
	va_end(args);
	nr_errors++;
	if (nr_errors>nr_max_errors)
		fatal_error("Too many syntax errors");
} /* syntaxerror */


/*--------------------------------------------------------------*/



/*--------------------------------------------------------------*/
/*   Line directives                                            */
/*--------------------------------------------------------------*/

int line_nr;
int pos_nr;

/**
 * Сработало правило сканнера.
 * Обновляем line_nr, pos_nr и определяем yylloc для парсера.
 */
void lex_rule_match( char *text)
{
	char *c;
	yylloc.first_line = line_nr;
	yylloc.first_column = pos_nr;

	for ( c = text; *c != '\0'; c++ )
	{
		if (c[1] == '\0') {
			yylloc.last_line = line_nr;
			yylloc.last_column = pos_nr;
		}
		if (*c == '\n') { line_nr++; pos_nr = 1; }
                else pos_nr++;

	}
}

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
 
        if (d<c) { line_nr = atoi(d); pos_nr = 1; }
 
        if (*c) {
                c++;
                d = c;
                while ((*d) && (*d!='"')) d++;
                *d=0;
                strcpy(filename, c);
        }
} /* line_directive */


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
} /* escape_transl */

/*--------------------------------------------------------------*/




/*--------------------------------------------------------------------*/
/* Memory allocation for decode table and hash table                  */
/*--------------------------------------------------------------------*/

#define hash_size 22079
#define hashtable_size 10000L

/**
 * We use a hash table with linked lists
 */
typedef struct hash_node {
        long              num;    /* номер строчки в таблице decode */
        struct hash_node *next;
} *hashentry;


/* The hash table */

static hashentry *hashtable = NULL;

static char **decode = NULL;
static long hashtop = 1;

/* текущий размер хэш таблицы */
static long   table_size = 0L;

/**
 * Выделение памяти под нужды хэш таблицы
 */
static void init_hash(void)
{
	char *h;

	if (!hashtable)
	{
		/* no hash table present -> allocate table */

		hashtable  = (hashentry *) 
			calloc((size_t) (hash_size+1),
					sizeof(hashentry));
		if (!hashtable) fatal_error("memory exhausted");
	}

	if (!decode)
	{
		/* no decode table present -> allocate table */

		hashtop = 1;
		table_size = hashtable_size;
		decode   = (char **) 
			calloc((size_t) (table_size+1L),
					sizeof(char *));
		if (!decode) fatal_error("memory exhausted");
		h = ParseMalloc(strlen("$!+-#Start of Hash#+-!$")+1);
		decode[0]    = strcpy(h,"$!+-#Start of Hash#+-!$");
	}
} /* init_hash */

/**
 *
 */
static void increase_decode(void)
{
	char **help;
	long   mysize;

	mysize = table_size * 2L;
	help = (char **) calloc((size_t) (mysize+1), sizeof(char *));
	if (!help) fatal_error("memory exhausted");

	memcpy( help, decode, table_size*sizeof(char *));
	if (decode) (void)free((char *)decode);
	
	decode   = help; 
	table_size = mysize;
} /* increase_decode */

/**
 * Освобождение памяти, выделенной под нужды хэш таблицы.
 */
void FreeHash(void)
{
	if (decode) (void)free((char *)decode);
	decode   = (char **)0; 
	if (hashtable) (void)free((char *)hashtable);
	hashtable  = (hashentry *)0;
} /* FreeHash */


/*--------------------------------------------------------------------*/
/* Hash table for identifers and strings                              */
/*--------------------------------------------------------------------*/

/**
 * хэш-функция
 */
static int hash( char *string)
{
	int   hashval = 0 ;
	char *s = string;

	for(hashval=0; *s != '\0'; s++)
		hashval = (10*hashval - 48 + *s) % hash_size;
	hashval = ((hashval<0 ) ? -hashval : hashval);

	/* Modulo calculation because of table range */
	return (hashval % hash_size);
} /* hash */


/*--------------------------------------------------------------------*/
/* Hash table: visible from outside                                   */
/*--------------------------------------------------------------------*/

/**
 * Добавить в хэш строчку, возвращается присвоенный строчке индекс.
 */
long HashInsert( char *s)
{
	hashentry new_node;
	int       hashval;
	long      look;
	char *h;

	if (!hashtable) init_hash();

	hashval  = hash(s);
	new_node = hashtable[hashval];
	look = -1L;
	while (new_node != NULL) {
		if ( strcmp(s,decode[new_node->num]) == 0 ) {
			look = new_node->num;
			break;
		}
		new_node=new_node->next;
	}

	if (look == -1L) {
		if (hashtop == table_size-1) increase_decode();
		new_node = (hashentry)ParseMalloc(sizeof(struct hash_node));
		look = hashtop++;
		new_node->num  = look;
		new_node->next = hashtable[hashval];

		h = ParseMalloc(strlen(s)+1);
		decode[look]    = strcpy(h,s);
		hashtable[hashval] = new_node;
	}
	return(look);
} /* HashInsert */

/**
 *
 */
long HashTableSize(void)
{
	if (!hashtable) init_hash();
	return (hashtop);
} /* HashTableSize */

/**
 * Получить строчку по индексу.
 */
char *Decode(long x)
{
	if (!hashtable) init_hash();
	if (x<0 || x>=hashtop)       return(decode[0]);
	return (decode[x]);
} /* Decode */

/*--------------------------------------------------------------------*/




/*--------------------------------------------------------------------*/
/*  Standard Tree Construction Routines                               */
/*--------------------------------------------------------------------*/

/* Memory Management */

#ifdef MEMBLOCKSIZE
#define PARSEBLOCKSIZE (MEMBLOCKSIZE/sizeof(struct stree_node)+1)
#endif
/*--------------------------------------------------------------------*/
/* Memory allocation for syntax tree nodes                            */
/*--------------------------------------------------------------------*/

#ifndef ALIGN
#define ALIGN 8
#define IALIGN (ALIGN-1)
#endif

/*   The following in invisible from outside:        
 *   The heap consists of a list of memory blocks of size parseheapsize.
 *   The parseheap is the actual memory block.
 */ 

static char *parseheap = NULL;             /* the heap */ 
static char *parseheapstart;               /* the base */
static char *parseheaptop;                 /* the top  */
static char *parseheapend;                 /* the end  */
static int parseheapsize = PARSEBLOCKSIZE; /* the size of one block */


static void alloc_block(void)
{
	char *new_block;

	new_block = (char *)malloc(parseheapsize*sizeof(struct stree_node));
	if (!new_block) fatal_error("memory exhausted");
	(*(char**)new_block) = parseheap; /* в начале блока хранится ссылка ны предыдущий блок */
	parseheap = new_block;
	parseheapstart = parseheaptop = (parseheap + sizeof(char *));
	parseheapend = parseheap + (parseheapsize-2) * sizeof(struct stree_node);
	/* выравнивание */
	parseheaptop = (char *)((size_t)(parseheaptop+IALIGN)&(~IALIGN));
	return;
} /* alloc_block */

/**
 * allocate x bytes
 */
static void *parsemalloc(int x)
{
	char *help;
	int  y = (x+IALIGN)&(~IALIGN);

	if (!parseheap) alloc_block();

	help = parseheaptop;
	parseheaptop = parseheaptop + y;

	if (parseheaptop > parseheapend)
	{
		/* heap too small -> allocate new heap block */

		alloc_block();
		help = parseheaptop;
		parseheaptop = parseheaptop + y;
		if (parseheaptop > parseheapend) 
			fatal_error("syntax tree node too large");
	}
	return (help);
} /* parsemalloc */


/**
 * allocate yysyntaxtree node with x sons
 */
static yysyntaxtree st_malloc(int x)
{
	yysyntaxtree help;

	help = parsemalloc(sizeof(struct stree_node)+x*sizeof(yysyntaxtree));
	return (help);
}


/**
 * global allocate x bytes
 */
char * ParseMalloc(int x)
{
	return ((char *)parsemalloc(x));
}

/**
 * global deallocate the complete syntax tree heap
 */
void ParseFree(void)
{
	char *help, *next_help;


	help = parseheap;
	while (help) {
		next_help = *(char **)help;
		(void)free(help);
		help = next_help;
	}	

	parseheap = NULL;
	return;
} /* ParseFree */
 
/*--------------------------------------------------------------------*/
/* Create unions for syntax tree nodes                                */
/*--------------------------------------------------------------------*/


union special UnionByte(unsigned char x)
{
	union special help;
	help.byte = x;
	return(help);
}

union special UnionNum(int x)
{
	union special help;
	help.num = x;
	return(help);     
}

union special UnionLnum(long int x)
{
	union special help;
	help.lnum = x;
	return(help);     
}

union special UnionLrealnum(double x)
{
	union special help;
	help.lrealnum = x;
	return(help);     
}

union special UnionString(char *x)
{
	union special help;
	help.string = x;
	return(help);     
}


/*--------------------------------------------------------------------*/
/* Build syntax tree nodes                                            */
/*--------------------------------------------------------------------*/

/* Table for varargs */

static yysyntaxtree TreeTab[1024];

/* without sons */

yysyntaxtree BuildCont(int mtag,union special x,YYLTYPE *l)
{
        yysyntaxtree help;
        help = st_malloc(1);

	help->contents = x;
	tag(help)          = mtag;
	xfirst_line(help)   =l->first_line;
	xfirst_column(help) =l->first_column;
	xlast_line(help)    =l->last_line;
	xlast_column(help)  =l->last_column;
	xfather(help)       =(yysyntaxtree)0;
	son1(help) = (yysyntaxtree)0; 

        return(help);
}

/* with sons */

yysyntaxtree BuildTree(int mtag,int len, YYLTYPE *l, ...)
{
	int i,j;
	va_list pvar;
	yysyntaxtree help;

	va_start(pvar,l);

	i = 0;
	help = va_arg(pvar, yysyntaxtree);
	while (i < len) {
		TreeTab[i++] = help;
		help = va_arg(pvar, yysyntaxtree);
	}
	va_end(pvar);

        help = st_malloc((i<1?1:i));

	help->contents = UnionNum(0);
	tag(help)          = mtag;
	xfirst_line(help)   =l->first_line;
	xfirst_column(help) =l->first_column;
	xlast_line(help)    =l->last_line;
	xlast_column(help)  =l->last_column;
	xfather(help)       =(yysyntaxtree)0;

	for (j=1; j<=i; j++) {
		son(help,j) = TreeTab[j-1]; 
		if (TreeTab[j-1]!=(yysyntaxtree)0)
			xfather(TreeTab[j-1]) = help;
	}
        return(help);
}


/* copy syntax tree */

yysyntaxtree Copy(yysyntaxtree x)
{
	register int j,len;
        yysyntaxtree help; 

	if (!x) return(x);
	len = nr_of_sons(x);
        help = st_malloc((len<1?1:len));

	help->contents = x->contents;
	tag(help)          = tag(x);
	xfirst_line(help)   = xfirst_line(x);
	xfirst_column(help) = xfirst_column(x);
	xlast_line(help)    = xlast_line(x);
	xlast_column(help)  = xlast_column(x);
	xfather(help)       = (yysyntaxtree)0;
	son1(help) = (yysyntaxtree)0; 

	for (j=1; j<=len; j++) {
		son(help,j) = Copy(son(x,j)); 
		if (son(help,j))
			xfather(son(help,j)) = help; 
	}
        return(help);
}

/* revert list */

yysyntaxtree Revert(yysyntaxtree list)
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


/* Constructor Names */

static const char * const yyconst_name[] =
{
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

static const int yyconst_arity[] =
{
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

/*--------------------------------------------------------------------*/
/* yield constructor name                                             */
/*--------------------------------------------------------------------*/

const char *ConstructorName(int i)
{
	return(yyconst_name[i]);
}

/*--------------------------------------------------------------------*/
/* yield constructor arity                                            */
/*--------------------------------------------------------------------*/

int ConstructorArity(int i)
{
	return(yyconst_arity[i]);
}


/*--------------------------------------------------------------------*/






