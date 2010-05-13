#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "grammar.h"




/*--------------------------------------------------------------------*/
/* Fatal error: exit with message                                     */
/*--------------------------------------------------------------------*/

void fatal_error(char *message)
{
	(void)fprintf(stderr,"Fatal error: %s !\n",message);
	(void)fprintf(stderr,"Aborted !\n");
	exit(-1);
}




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
long   table_size = 0L;

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
 * Освобождение памяти, выделенной под нужды хэш таблицы
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
		if ( strcmp(s,decode[new_node->num]) == 0L) {
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
 * Получить строчку по индексу
 */
char *Decode(long x)
{
	if (!hashtable) init_hash();
	if (x<0 || x>=hashtop)       return(decode[0]);
	return (decode[x]);
} /* Decode */

/*--------------------------------------------------------------------*/

