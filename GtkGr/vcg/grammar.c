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

void fatal_error(const char *message)
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
char    filename[FILENAME_BUF_SIZE];     /* Filename from the view of the parser */

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
    (void)fprintf(stderr,"Syntax error (%s: l:%d c:%d): ",filename,line,pos);
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
} /* lex_rule_match */

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
                strncpy( filename, c, FILENAME_BUF_SIZE-1);
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
    return (yysyntaxtree)parsemalloc(sizeof(struct stree_node) + x*sizeof(yysyntaxtree));
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

/* without sons
 */
yysyntaxtree BuildCont(int mtag, union special x, YYLTYPE *l)
{
    yysyntaxtree help = st_malloc(0);

    assert(ConstructorArity(mtag) == 0);

    help->contents = x;
    tag(help)           = mtag;
    xfirst_line(help)   = l->first_line;
    xfirst_column(help) = l->first_column;
    xlast_line(help)    = l->last_line;
    xlast_column(help)  = l->last_column;
    xfather(help)       = (yysyntaxtree)0;

    return(help);
}

/* with sons
 */
yysyntaxtree BuildTree(int mtag, int len, YYLTYPE *l, ...)
{
    int i;
    va_list pvar;
    yysyntaxtree help = st_malloc(len);

    assert(ConstructorArity(mtag) == len);

    help->contents = UnionFake();
    tag(help)           = mtag;
    xfirst_line(help)   = l->first_line;
    xfirst_column(help) = l->first_column;
    xlast_line(help)    = l->last_line;
    xlast_column(help)  = l->last_column;
    xfather(help)       = (yysyntaxtree)0;

    va_start(pvar,l);
    for (i = 1; i <= len; i++)
    {
        yysyntaxtree tree_son = va_arg(pvar, yysyntaxtree);
        son(help,i) = tree_son;
        if (tree_son)
            xfather(tree_son) = help;
    }
    va_end(pvar);

    return(help);
}


/* copy syntax tree */

yysyntaxtree Copy(yysyntaxtree x)
{
    int i,len;
    yysyntaxtree help;

    if (!x) return(x);
    len = nr_of_sons(x);
    help = st_malloc(len);

    help->contents = x->contents;
    tag(help)           = tag(x);
    xfirst_line(help)   = xfirst_line(x);
    xfirst_column(help) = xfirst_column(x);
    xlast_line(help)    = xlast_line(x);
    xlast_column(help)  = xlast_column(x);
    xfather(help)       = (yysyntaxtree)0;

    for (i=1; i<=len; i++) {
        son(help,i) = Copy(son(x,i));
        if (son(help,i))
            xfather(son(help,i)) = help;
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
        assert(nr_of_sons(act) >= 2);
        next = son2(act);
        son2(act) = last;
        if (last) xfather(last) = act;
        last = act;
        act = next;
    }
    if (last) xfather(last) = (yysyntaxtree)0;
    return(last);
}

/*--------------------------------------------------------------------*/


/* Constructor Names & Arities */

struct yyconst_descr_unit {
    const char *name;
    int arity;
};

static const struct yyconst_descr_unit yyconst_descr[] =
{
    #define ST_NODE(name, arity, contents_type) { #name, arity },
    #include "parse_gdl.mac.h"
};

/*--------------------------------------------------------------------*/
/* yield constructor name                                             */
/*--------------------------------------------------------------------*/

const char *ConstructorName(int mtag)
{
    return yyconst_descr[mtag].name;
}

/*--------------------------------------------------------------------*/
/* yield constructor arity                                            */
/*--------------------------------------------------------------------*/

int ConstructorArity(int mtag)
{
    return yyconst_descr[mtag].arity;
}


/*--------------------------------------------------------------------*/






