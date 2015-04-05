/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         step2.c                                            */
/*   version:      1.00.00                                            */
/*   creation:     14.4.1993                                          */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */  
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Layout phase 2: reduction of crossings             */
/*   status:       in work                                            */
/*                                                                    */
/*--------------------------------------------------------------------*/


/*
 *   Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                               the Compare Consortium 
 *
 *  This program and documentation is free software; you can redistribute 
 *  it under the terms of the  GNU General Public License as published by
 *  the  Free Software Foundation;  either version 2  of the License,  or
 *  (at your option) any later version.
 *
 *  This  program  is  distributed  in  the hope that it will be useful,
 *  but  WITHOUT ANY WARRANTY;  without  even  the  implied  warranty of
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the
 *  GNU General Public License for more details.
 *
 *  You  should  have  received a copy of the GNU General Public License
 *  along  with  this  program;  if  not,  write  to  the  Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */



/************************************************************************
 * The situation here is the following:
 * -----------------------------------
 * We are still in the layout phase of the graph. The graph is in 
 * adjacency list representation available, further it is partitioned
 * into layers that represent a proper hierarchy.
 * We want to calculate now the relative position of the nodes inside
 * the layers. We have:
 *
 *    1)  The array layer contains all visible nodes in the TSUCC lists.
 *        They are distributed at the layer[i] lists and connected by two
 *        lists TPRED and TSUCC to allow to traverse the nodes of one 
 *        layer[i] backwards and forwards. However TSUCC and TPRED are
 *        different: TSUCC contains all nodes of the layer including those
 *        nodes reacheable by forward connections. TPRED excludes these
 *        nodes. TANZ(layer[i])= 0.
 *        The hierarchy in layer is proper.
 *    2)  Further, all visible nodes are in nodelist, labellist and dummylist.
 *    3)  All pot. visible edges are in the lists edgelist or tmpedgelist.
 *        Visible edges can be detected by the EINVISIBLE flag (==0) in these
 *        lists. Note: invisible edges may also be in edgelist or tmpedgelist.
 *        An edge is visible iff 
 *		   a) it is used in the adjacency lists.
 *	  	or b) it is a direct neigbour edge in NCONNECT(v) for 
 *		      some node v.
 *    4)  maxindeg and maxoutdeg are the maximal indegree (number of
 *        incoming edges) and maximal outdegree (number of outgoing
 *        edges) occuring at visible nodes. Forward connections are 
 *        not counted.
 *    5)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    6)  NTIEFE(node) is filled for all nodes. NINDEG and NOUTDEG are
 *        filled. Forward connections are not counted. But  NINDEG and 
 *	  NOUTDEG are also filled at nodes that have forward connections.
 *        NCONNECT(node) is filled for nodes that have direct neighbours
 *        in the layout. The edges in NCONNECT are not anymore in the
 *        adjacency lists, but still visible. The forward connection nodes
 *        are still in the layers in the TSUCC lists, but not in the
 *        TPRED lists. 
 *    7)  Reverted edges are marked with EART(e)='R'.
 *        Self loops don't anymore exist.
 *    8)  Nodes that are not in TPRED but in TSUCC are marked by NMARK.
 *
 * The task of calculating the relative position of the nodes inside the
 * layers is done by resorting the TSUCC(layer[i]) (and the TPRED(layer[i],
 * of course, too) such that the number of edge crossings between 
 * adjacent layers is minimized. However, we only use a heuristics,
 * because the optimal solution would be too time complex: the layer
 * table is copied into a temporary layer table and some changes are
 * done. If the number of crossings is reduced, we take the temporary
 * table as new table, if not, we stop and take the original layer table.
 * The changes we do, it the barycenter method. First, we work on the
 * TPRED-lists (that are compied into the TSUCC-lists) to exclude the
 * forward connections: i.e. directly neighboured nodes are dealed as
 * one summary node.
 * After that, we insert the forward connection nodes such that the number
 * of crossings does not increase too much. At last, the adjacency lists
 * are sorted according to the positions of TARGET(successor) or
 * SOURCE(predecessor).
 *
 * At the end, the situation is this:
 *    1)  The array layer contains all visible nodes. 
 *        They are distributed at the layer[i] lists and connected by two
 *        lists TPRED and TSUCC to allow to traverse the nodes of one 
 *        layer[i] backwards and forwards. 
 *    2)  Note that the nodes reacheable via forward connections are now
 *	  in the TSUCC and TPRED lists, too.
 *	  TANZ(layer[i]) and NINDEG(node) and NOUTDEG(node) are adapted
 * 	  to this situation. The connection edge still do not count.
 *    3)  The hierarchy in layer is proper.
 *    4)  nodelist, labellist and dummylist are not changed.
 *    5)  All pot. visible edges are in the lists edgelist or tmpedgelist,
 *	  same as before.
 *    6)  maxindeg and maxoutdeg are not changed. By inserting nodes
 *	  reacheable by connections, the maximal indegree and outdegree
 *	  may shrink, thus maxindeg and maxoutdeg are upper estimations.
 *    7)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    8)  NTIEFE(node) is filled for all nodes. NINDEG and NOUTDEG are
 *        filled. Forward connections are not counted.
 *        NCONNECT(node) is filled as before.
 *	  See point 2 !!!
 *    9)  Reverted edges are marked with EART(e)='R'.
 *        Self loops don't anymore exist.
 *    10) NPOS(v) gives the horizontal position of a node inside the
 *	  layer. Adjacency edges are sorted according to these NPOS
 *	  values. The ordering inside a layer is such that the number
 *	  of cossings is small (but may be not optimal).
 *    11) NSUCCL(v) and NSUCCR(v) are the leftest and rightest successor
 *        edge of v, and NPREDL(v) and NPREDR(v) the leftest and rightest
 *        predecessor edge.
 *
 *
 * This file provides the following functions:
 * ------------------------------------------
 * step2_main		Main routine to reduce the number of crossings
 *
 * If DEBUG is switched on, we have further:
 * db_output_all_layers  prints all layers in tmp_layer.
 * db_output_layer       prints one tmp_layer[i]
 * db_check_proper	 checks whether a nodes is proper in the 
 *			 hierarchy. Prints diagnostics.
 *
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "globals.h"
#include "alloc.h"
#include "main.h"
#include "options.h"
#include "folding.h"
#include "steps.h"
#include "timelim.h"
#include "timing.h"


/* Defines
 * -------
 * For debbuging: We can switch on CHECK_CROSSING, that
 * causes the checks of some additional assertions
 * and prints out the effects of the different phases
 * of the barycenter algorithm.
 */

/* #define CHECK_CROSSING  */
#undef CHECK_CROSSING  


/* Macros to sort the sort_array of size x with the bary_compare function.
 * We have written our own quicksort, which is randomized and fast.
 * Sometimes the quicksort `qsort' of the library is faster.
 */

#ifdef OWN_QUICKSORT
#define quicksort_sort_array(x) { if (x) myqsort(0,x-1); }
#else
#define quicksort_sort_array(x) { if (x) qsort(sort_array,x, \
	   sizeof(GNODE),(int (*) (const void *, const void *))compare_bary); }
#endif

#define quicksort_save_array(x) { if (x) qsort(save_array,x, \
	   sizeof(GNODE),(int (*) (const void *, const void *))compare_pos); }



/* Prototypes
 * ----------
 */

static void 	create_tmp_layer 	  _PP((void));
static void 	tree_horizontal_order	  _PP((void));
static void 	unmerge_connected_parts	  _PP((void));
static void	prepare_horizontal_order  _PP((void));
static void 	mark_all_nodes		  _PP((GNODE node, int i));

static int	graph_crossings 	  _PP((void));
static void	calc_all_layers_crossings _PP((void));
static int 	layer_crossing		  _PP((int level));
static void 	finish_upper		  _PP((GNODE v));
static void 	finish_lower		  _PP((GNODE v));
static void	append_to_upper		  _PP((GNODE n));
static void	append_to_lower		  _PP((GNODE n));
static void 	delete_upper		  _PP((DLLIST x));
static void 	delete_lower		  _PP((DLLIST x));

static void	local_crossing_optimization _PP((int nearedges));
static void    	prepare_local_optimization  _PP((int level));
static int  	level_crossing_optimization _PP((int level,int nearedges));
static int 	exchange_nodes_necessary    _PP((GNODE C,GNODE D));

static void 	prepare_positions	    _PP((void));
static int 	is_complex		    _PP((GNODE v));
static int 	check_exchange		    _PP((GNODE v1,GNODE v2,int dir));
static void	do_exchange		    _PP((GNODE v1,GNODE v2,int dir));
static void 	local_unwind_crossings	    _PP((void));

static void 	init_barycentering	  _PP((void));
static void     barycentering		  _PP((void));
static void	Phase1_down 		  _PP((void));
static void	Phase1_up 		  _PP((void));
static void	Phase2_down	 	  _PP((void));
static void 	Phase2_up 		  _PP((void));
static int	resort_down_layer 	  _PP((int i));
static int	resort_up_layer 	  _PP((int i));
static void	resort_down_up_layer	  _PP((int level));
static void	resort_up_down_layer	  _PP((int level));
static int 	cycle_sort_array	  _PP((int siz));
static float    succbary		  _PP((GNODE node));
static float    predbary		  _PP((GNODE node));
static float    succmedian		  _PP((GNODE node));
static float    predmedian		  _PP((GNODE node));
static void	level_to_array		  _PP((int i,int dir));
static void	array_to_level		  _PP((int i));
static void     save_level                _PP((int i)); 
static void     restore_level             _PP((int i));
static void 	apply_horder		  _PP((int i));

static  int 	compare_bary 	 	  _PP((const GNODE *a, const GNODE *b));
static  int 	compare_pos  	 	  _PP((const GNODE *a, const GNODE *b));

static void 	copy_layers		  _PP((DEPTH *l1, DEPTH *l2));

static void 	insert_connects_in_layer  _PP((void));
static void 	insert_left_right	  _PP((int level, GNODE node));
static void 	check_connect		  _PP((int level, GNODE node));
static void 	left_conn_list		  _PP((GNODE v,GNODE w));
static void 	right_conn_list		  _PP((GNODE v,GNODE w));

static void 	recreate_predlists	  _PP((void));
static void 	sort_adjedges		  _PP((GNODE v));
static int 	compare_srcpos		  _PP((const GEDGE *a, const GEDGE *b)); 
static int 	compare_tarpos		  _PP((const GEDGE *a, const GEDGE *b)); 
#ifdef OWN_QUICKSORT
static void 	myqsort			  _PP((int l,int r));
#endif
#ifdef DEBUG
void    db_output_all_layers  	_PP((void));
void    db_output_layer		_PP((int i));
int 	db_check_proper     	_PP((GNODE v,int level));
#endif


/* Global variables
 * ----------------
 */

static DEPTH	*tmp_layer=NULL;    /* Temporary copy of layer table   */
static int size_of_tlayer = 0;      /* Size of temp.table of layers    */

/* maximal number of nodes in one layer */

int	max_nodes_per_layer;

/* number of crossings of the graph layouted as in layer.
 */

int 	nr_crossings;

/* arrays where we can temporary sort the adjacency lists.
 * Used to calculate the number of crossings, e.g. to implement
 * the feature: for all NSUCC's of v in order of their NPOS ...
 * Both have the same size.
 */

static GNODE	*adjarray  = NULL;
static GEDGE    *adjarray2 = NULL;
static int	size_of_adjarray = 0;

/* array where we can temporary sort the nodes of one level.
 * Used for the barycenter method: All nodes of one level     
 * should be sorted according to the bary-values.
 */

static GNODE  	*sort_array = NULL;
static int	size_of_sortarray = 0;

/* array where we can temporary save one layer. It has always the
 * same size as the sort_array.
 */

static GNODE  	*save_array = NULL;

/* actual number of the iteration during the layout process.
 * We break after`max_baryiterations'.
 */

static int nr_bary_iterations;

/*--------------------------------------------------------------------*/
/*  Reducing the number of crossings                                  */
/*--------------------------------------------------------------------*/

void step2_main(void)
{
        int     i;
	int 	old_nr_crossings;

	start_time();
	debugmessage("step2_main","");
	assert((layer));


        /* Allocate tmp_layer and copy layer into tmp_layer.
	 * Calculate max_nodes_per_layer.
	 */ 

	create_tmp_layer();

	/* Allocate the sort_array and the save_array, that are used 
	 * for barycentering
	 */

	if (max_nodes_per_layer+2 > size_of_sortarray) {
		if (sort_array)  free(sort_array);
		if (save_array)  free(save_array);
		sort_array = (GNODE *)malloc((max_nodes_per_layer+2)
					* sizeof(GNODE));
		save_array = (GNODE *)malloc((max_nodes_per_layer+2)
					* sizeof(GNODE));
		if ((!sort_array)||(!save_array)) 
				Fatal_error("memory exhausted","");
		size_of_sortarray = max_nodes_per_layer+2;
#ifdef DEBUG
		PRINTF("Sizeof tables `sort_array',`save_array': %d Bytes\n",
			(max_nodes_per_layer+2)*sizeof(GNODE));
#endif
	}

	/* Allocate the adjarrays, that are used for crossing calculation */

	i = (maxindeg > maxoutdeg ? maxindeg : maxoutdeg);
	if (i+2 > size_of_adjarray) {
		if (adjarray)  free(adjarray);
		if (adjarray2) free(adjarray2);
		adjarray  = (GNODE *)malloc((i+2)*sizeof(GNODE));
		adjarray2 = (GEDGE *)malloc((i+2)*sizeof(GEDGE));
		if ((!adjarray)||(!adjarray2)) 
			Fatal_error("memory exhausted","");
		size_of_adjarray = i+2;
#ifdef DEBUG
		PRINTF("Sizeof table `adjarray[12]': %d Bytes\n",
			(i+2)*sizeof(GNODE));
#endif
	}


	/* Presort layers according to the tree traversal. */

	tree_horizontal_order();

	/* Presort layers according to horizontal order */

	prepare_horizontal_order();


	/* Unmerge intermixed connected parts */

	unmerge_connected_parts();


	/* Calculate the number of crossings of the actual graph */

	calc_all_layers_crossings();
	for (i=0; i<=maxdepth+1; i++)
		TCROSS(layer[i]) = TCROSS(tmp_layer[i]);
	nr_crossings = graph_crossings();

#ifdef CHECK_CROSSING
	PRINTF("Start: nr_crossings %d \n",nr_crossings); 
#endif

	/* Layout the graph until the number of crossings does not
	 * change anymore. Barycentering expects the graph in 
	 * tmp_layer.
	 */

	init_barycentering();
	old_nr_crossings = -1;
	nr_bary_iterations = 0;
        while (nr_crossings != old_nr_crossings) {
		if (nr_bary_iterations>=max_baryiterations) {
                       	gs_wait_message('t');
			break;
		}
	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
		old_nr_crossings = nr_crossings;
                barycentering();
		nr_bary_iterations++;
		if (nr_bary_iterations<min_baryiterations) 
			nr_crossings = old_nr_crossings+1; 
        }

#ifdef CHECK_CROSSING
	PRINTF("After barycentering: nr_crossings %d \n",nr_crossings); 
#endif

	/* Now we examine the local neighboured nodes to optimize the
 	 * crossings.
	 */

	old_nr_crossings = nr_crossings;
	if (old_nr_crossings>0) local_crossing_optimization(0);

	/* Now we try to insert the connections such that we
	 * have minimal crossings.
	 */

	gs_wait_message('b');
 	insert_connects_in_layer();
	gs_wait_message('b');

	/* Again, we examine the local neighboured nodes to optimize the
 	 * crossings. Now, we do not optimize nodes with nearedges.
	 */

	if (old_nr_crossings>0) local_crossing_optimization(1);

#ifdef CHECK_CROSSING
	calc_all_layers_crossings();
	nr_crossings = graph_crossings();
	PRINTF("After local optimization: nr_crossings %d \n",nr_crossings); 
#endif

	if (local_unwind && (old_nr_crossings>0)) local_unwind_crossings();

	calc_all_layers_crossings();
	nr_crossings = graph_crossings();

#ifdef CHECK_CROSSING
	PRINTF("Final: nr_crossings %d \n",nr_crossings); 
#endif

	/* The result after barycentering is in tmp_layer.
	 * Copy this to layer.
	 */

	copy_layers(layer,tmp_layer); 

	/* Barycentering ignores the TPRED field and works only
	 * with TSUCC. Thus we recreate the TPRED fields of layer now.
	 */

	recreate_predlists();

	stop_time("step2_main");
} /* step2_main */


/*--------------------------------------------------------------------*/
/*  Copy layer into tmp_layer                                         */
/*--------------------------------------------------------------------*/

/*  Initialize the array tmp_layer
 *  ==============================
 *  tmp_layer is a copy of the actual graph. All position changes on 
 *  the graph are done in tmp_layer, because if they fail, the original
 *  graph is used.
 *  create_tmp_layer allocates tmp_layer and copies layer into tmp_layer.
 *  Further, max_nodes_per_layer is calculated, and the field TCROSS
 *  is initialized.
 *  We use the TPRED-fields of layer here. As side effect, all connection
 *  nodes disppear  because they are not in the TPRED-lists. We adapt
 *  the TSUCC-lists of layer to the TPRED-lists.
 *  Further TANZ(layer[i]) is calculated here.
 */

static void	create_tmp_layer(void)
{
	int	i,j;
	GNLIST	h1, h2;


	/* Allocate the array tmp_layer */

	if (maxdepth+2 > size_of_tlayer) {
		if (tmp_layer) free(tmp_layer);
		tmp_layer = (DEPTH *)malloc((maxdepth+2)*sizeof(struct depth_entry));
		if (!tmp_layer) Fatal_error("memory exhausted","");
		size_of_tlayer = maxdepth+2;
#ifdef DEBUG
		PRINTF("Sizeof table `tmp_layer': %d Bytes\n",
			(maxdepth+2)*sizeof(struct depth_entry));
#endif
	}

	/* Copy layer into tmp_layer */

	max_nodes_per_layer = 0;
	for (i=0; i<=maxdepth+1; i++) {
		TCROSS(tmp_layer[i])	= 0;
		TPRED(tmp_layer[i])	= NULL;
		TSUCC(tmp_layer[i]) 	= NULL;

		/* TSUCC contains more nodes as TPRED. Thus, to calculate
		 * max_nodes_per_layer, we use TSUCC.
		 */
		j = 0;
                h1  =  TSUCC(layer[i]);
		while (h1) {
			j++;
			h1 = GNNEXT(h1);
		}
		if (j > max_nodes_per_layer) max_nodes_per_layer = j;

		/* Copying with this method reverts the list,
		 * thus we copy revert.TPRED(layer[i]) -> TSUCC(tmp_layer[i])
		 * TPRED(tmp_layer) is not needed for barycentering.
 		 */

		j = 0;
                h1  =  TPRED(layer[i]);
		while (h1) {
			j++;
			h2 = tmpnodelist_alloc();
			GNNEXT(h2) = TSUCC(tmp_layer[i]);
			TSUCC(tmp_layer[i]) = h2;
			GNNODE(h2) = GNNODE(h1);
			h1 = GNNEXT(h1);
		}

		TANZ(tmp_layer[i])      = TANZ(layer[i]) = j;
		TRESNEEDED(tmp_layer[i])= TRESNEEDED(layer[i]);

		/* for security only */
		if (j > max_nodes_per_layer) max_nodes_per_layer = j;

		/*
		 * Now we adapt the TSUCC-fields of layer in the same way.
		 */
		TSUCC(layer[i]) = NULL;
                h1  =  TPRED(layer[i]);
		while (h1) {
			h2 = tmpnodelist_alloc();
			GNNEXT(h2) = TSUCC(layer[i]);
			TSUCC(layer[i]) = h2;
			GNNODE(h2) = GNNODE(h1);
			h1 = GNNEXT(h1);
		}
	}
} /* create_tmp_layer */



/*--------------------------------------------------------------------*/
/*  Prepare horizontal order according to the spanning tree           */
/*--------------------------------------------------------------------*/

/*  Spanning tree order
 *  ===================
 *  A good initial order is the one where the spanning tree has already
 *  no crossings. 
 *  Thus we calculate the spanning tree using the edges with the highest
 *  priorities. We reorder the layers such that this spanning tree is
 *  planar embedded.
 */

static void tree_horizontal_order(void)
{
	GNLIST li;
	int    i, prio;
	GNODE  node;
	ADJEDGE a;
	GEDGE   e;
	double maxbary;

	debugmessage("tree_horizontal_order","");

	/* Not necessary, but for security reason: unmark all nodes
	 * and edges.
	 */

	for (i=0; i<=maxdepth+1; i++) {
		li	= TSUCC(tmp_layer[i]);
		while (li) { 
			assert((NMARK(GNNODE(li))==0)); 
			a = NSUCC(GNNODE(li));
			while (a) {
				EWEIGHTS(AKANTE(a)) = 0;
				a = ANEXT(a);
			}
			li = GNNEXT(li); 
		}
	}

	/* Mark the spanning tree edges by EWEIGHTS = 1 */

	for (i=0; i<=maxdepth+1; i++) {
		for (prio=max_eprio; prio >= 0; prio--) {
			li	= TSUCC(tmp_layer[i]);
			while (li) { 
				a = NSUCC(GNNODE(li));
				while (a) {
					e = AKANTE(a);
					if (EPRIO(e)==prio) {
						node = TARGET(a);	
						if (!NMARK(node)) {
							NMARK(node) = 1;
							EWEIGHTS(e) = 1;
						}
					}			
					a = ANEXT(a);
				}
				li = GNNEXT(li); 
			}
		}
	}

	/* The sons in the spanning tree are subsequent in the layer.
	 * Internally, they are ordered according to the predbary.
	 */

	for (i=1; i<=maxdepth+1; i++) {
		/* Copy tmp_layer -> sort_array. This initializes NPOS. */
		level_to_array(i,'d');

		maxbary = 0.1;
		li = TSUCC(tmp_layer[i]);
		while (li) {
			node = GNNODE(li);
			NBARY(node) =  predbary(node);	
			if (NBARY(node) > maxbary) maxbary = NBARY(node);
			li = GNNEXT(li);
		}
		maxbary = maxbary + 1.0;

		li = TSUCC(tmp_layer[i-1]);
		while (li) {
			a = NSUCC(GNNODE(li));
			while (a) {
				e = AKANTE(a);
				if (EWEIGHTS(e)) {
					node = TARGET(a);
					NBARY(node)=(double)(NPOS(GNNODE(li)))
						 + NBARY(node)/maxbary;
				}
				a = ANEXT(a);
			}
			li = GNNEXT(li);
		}

		quicksort_sort_array(TANZ(layer[i]));

		/* Copy sort_array -> tmp_layer */
        	array_to_level(i);
	}

	/* Unmark all nodes again */

	for (i=0; i<=maxdepth+1; i++) {
		li	= TSUCC(tmp_layer[i]);
		while (li) { NMARK(GNNODE(li)) = 0; li = GNNEXT(li); }
	}

	/* layer = tmp_layer */
	copy_layers(layer,tmp_layer); 

} /* tree_horizontal_order */


/*--------------------------------------------------------------------*/
/*  Prepare horizontal order                                          */
/*--------------------------------------------------------------------*/

/*  Horizontal order analysis
 *  =========================
 *  We want to have the nodes of a level in the increasing order
 *  of its NHORDER-attribute. Thus, we calculate whether some nodes
 *  of a level have appropriate NHORDER-attributes (not -1)   and
 *  we set TRESNEEDED(layer) appropriate. Further, we calculate
 *  the maximal NHORDER-number that occurs.
 */

static int max_horder_num;

static void prepare_horizontal_order(void)
{
	GNLIST li;
	int    i, reorder_necessary;
	GNODE  node;

	debugmessage("prepare_horizontal_order","");

	max_horder_num = 0;

	for (i=0; i<=maxdepth+1; i++) {
		li	= TSUCC(tmp_layer[i]);
		reorder_necessary = 0;
		while (li) {
			node = GNNODE(li);
			if (NHORDER(node) >= 0) reorder_necessary = 1; 
			if (NHORDER(node)>max_horder_num)
				max_horder_num = NHORDER(node);
			li = GNNEXT(li);
		}
		TRESNEEDED(tmp_layer[i]) = 
		    TRESNEEDED(layer[i]) = reorder_necessary;
	}
} /* prepare_horizontal_order */


/*--------------------------------------------------------------------*/
/*  Unmerge connected parts                                           */
/*--------------------------------------------------------------------*/

/*  Intermixed connected parts
 *  ==========================
 *  The following situation is an unfortunately intermixing of two
 *  parts.
 *               A                               A
 *              / \                             / \
 *             /   \                           /   \
 *         C  D  E  F     We want to unmerge  D     F  C     E
 *          \   /          this into:                   \   /
 *           \ /                                         \ /
 *            G                                           G
 *
 *  Of course, barycentering may reintroduce such intermixings, but if
 *  we start with a clean situation, the intermixing is very seldom.
 */

static void unmerge_connected_parts(void)
{
	GNLIST li;
	int    i,j;
	int    part_is_missing; 
	GNODE  node;
	int act_graph;

	debugmessage("unmerge_connected_parts","");

	/* first, set the NBARY field to 0 */
	for (i=0; i<=maxdepth+1; i++) {
		li	= TSUCC(tmp_layer[i]);
		while (li) {
			NBARY(GNNODE(li)) = 0;
			li = GNNEXT(li);
		}
	}

	act_graph = 1;

	part_is_missing = 1;
	while (part_is_missing) {

		gs_wait_message('u');
		part_is_missing =0;
		node = (GNODE)0;
		act_graph++;

		/* look for an untouched connected component
		 */
		for (i=0; i<=maxdepth+1; i++) {
			li	= TSUCC(layer[i]);
			while (li) {
				if (NBARY(GNNODE(li)) == 0) {
					node = GNNODE(li);
					break;
				}
				li = GNNEXT(li);
			}
			if (node) break;
		}
		if (node) {
			assert((NBARY(node)==0));
			part_is_missing =1;
			mark_all_nodes(node, act_graph);
		}
	}


	for (i=0; i<=maxdepth+1; i++) {
		/* Copy tmp_layer -> sort_array */
		li = TSUCC(tmp_layer[i]);
		j = 0;
		while (li) {
			sort_array[j++] = GNNODE(li);
			li = GNNEXT(li);
		}
		quicksort_sort_array(TANZ(layer[i]));
		/* Copy sort_array -> tmp_layer */
        	array_to_level(i);
	}

	/* layer = tmp_layer */
	copy_layers(layer,tmp_layer); 
} /* unmerge_connected_parts */




/* Mark all nodes with its graph number 
 * ------------------------------------
 */

static void mark_all_nodes(GNODE node,int i)
{
	ADJEDGE e;

	debugmessage("mark_all_nodes","");

	if (NBARY(node)) return;
	if (NHORDER(node)>=0) 
		NBARY(node) = (float)i*(float)(max_horder_num+1)
					+(float)NHORDER(node);
	else	NBARY(node) = (float)i*(float)(max_horder_num+1);

	e = NSUCC(node);
	while (e) {
		mark_all_nodes(TARGET(e),i);
		e = ANEXT(e);
	}
	e = NPRED(node);
	while (e) {
		mark_all_nodes(SOURCE(e),i);
		e = ANEXT(e);
	}
} /* mark_all_nodes */



/*--------------------------------------------------------------------*/
/*  Calculate the number of crossings                                 */
/*--------------------------------------------------------------------*/

/*  Number of crossings in the whole graph in tmp_layer
 *  ===================================================
 *  The precondition is that the TCROSS-entries are already filled. 
 */

static int	graph_crossings(void)
{
	int	i;
	int	sumC;

	debugmessage("graph_crossings","");
	assert((tmp_layer));
	sumC = 0;
	for (i=0; i<=maxdepth; i++) sumC += TCROSS(tmp_layer[i]);
	assert((TCROSS(tmp_layer[maxdepth+1])==0));
	return(sumC);
} /* graph_crossings */



/*  Number of crossings of all layers
 *  ---------------------------------
 *  Before graph_crossings, we have to fill the TCROSS-entries of
 *  tmp_layer. This is done in the following function.
 */

static void	calc_all_layers_crossings(void)
{
	int	i;

	debugmessage("calc_all_layers_crossings","");
	assert((tmp_layer));
	for (i=0; i<=maxdepth; i++) 
		TCROSS(tmp_layer[i])=layer_crossing(i);
} /* calc_all_layers_crossings */


/*  Number of crossings of one layer
 *  --------------------------------
 *  returns the number of crossings between level and level+1.
 *  We analyse tmp_layer, not layer. Note that the hierarchy is
 *  proper, i.e. all edges go from nodes of level to level+1.
 *
 *  The algorithm is a kind of plan sweep. Preconditions: 
 *  We use two double linked, ordered lists lower_list and upper_list. 
 *  1)  A node is called `finished' if we have completely inspect
 *      its position.
 *  2)  A node is called `touched' if it is not finished, but there 
 *      is an edge from a finished node to it, or an edge from it
 *      to a finished node. 
 *  3)  The upper list contains all touched nodes of level.
 *  4)  The lower list contains all touched nodes of level+1.
 *  5)  Note the there may be more than one edge to a node, thus the
 *      node may be several times touched, i.e. several times in 
 *      lower_list or upper_list.
 *  6)  NVPTR(v) points to the backmost instance of v in the 
 *      lower_list or upper_list.
 *  7)  Nodes are alternating numbered: nodes of level have even
 *      numbers, nodes at level+1 have odd numbers in NPOS.
 *  See the algorithmus of `finishing' for more details.
 */

static	int 	size_lower_list;	/* nr. of elements in lower_list */
static	int 	size_upper_list;	/* nr. of elements in upper_list */
static  DLLIST	lower_list     = NULL;	/* the lower list                */
static  DLLIST	lower_list_end = NULL;  /* and its end                   */
static 	DLLIST	upper_list     = NULL;  /* the upper list                */
static	DLLIST	upper_list_end = NULL;  /* and its end                   */
static	int	nr_tcrossings;		/* actual number of crossings    */


static int 	layer_crossing(int level)
{
	GNLIST  vl1,vl2;
	ADJEDGE a;
	int i;

	debugmessage("layer_crossings","");
	assert((tmp_layer));
	assert((lower_list==NULL));
	assert((upper_list==NULL));
	assert((level>=0));
	assert((level<=maxdepth));	/* we access to level+1
					 * and tmp_layer[maxdepth+1] exists 
					 */

	/* Later, we need to traverse the adjacency lists in the order
	 * of their source or target nodes. We prepare this here, by
	 * refilling the NPREDs of level+1 and NSUCCs of level. Therefore
	 * we use a pointer NTMPADJ pointing to the next adjlist-cons cell
	 * that must be filled. See finish_upper/lower for more.
	 * Init NTMPADJ of all nodes in level and level+1 */

	vl1 = TSUCC(tmp_layer[level]);
	while (vl1) {
		NTMPADJ(GNNODE(vl1)) = NSUCC(GNNODE(vl1));
		vl1 = GNNEXT(vl1);
	}
	vl2 = TSUCC(tmp_layer[level+1]);
	while (vl2) {
		NTMPADJ(GNNODE(vl2)) = NPRED(GNNODE(vl2));
		vl2 = GNNEXT(vl2);
	}

	/* Init NVPTR of all nodes in level and level+1.
	 * Further, sort the adjacency lists here. When traversing level
	 * i, we sort the PREDs of level i+1, and when traversing level
	 * i+1, we sort the SUCCs of level i.
	 */

	i = 1;
	vl1 = TSUCC(tmp_layer[level]);
	while (vl1) {
		NVPTR(GNNODE(vl1)) = NULL;
		NPOS(GNNODE(vl1))  = i;
		a = NSUCC(GNNODE(vl1));
		while (a) {
			assert((NTMPADJ(TARGET(a))));
			AKANTE(NTMPADJ(TARGET(a))) = AKANTE(a);
			NTMPADJ(TARGET(a)) = ANEXT(NTMPADJ(TARGET(a)));
			a = ANEXT(a);
		}
		vl1 = GNNEXT(vl1);
		i = i+2;
	}

	i = 2;
	vl2 = TSUCC(tmp_layer[level+1]);
	while (vl2) {
		NVPTR(GNNODE(vl2)) = NULL;
		NPOS(GNNODE(vl2))  = i;
		a = NPRED(GNNODE(vl2));
		while (a) {
			assert((NTMPADJ(SOURCE(a))));
			AKANTE(NTMPADJ(SOURCE(a))) = AKANTE(a);
			NTMPADJ(SOURCE(a)) = ANEXT(NTMPADJ(SOURCE(a)));
			a = ANEXT(a);
		}
		vl2 = GNNEXT(vl2);
		i = i+2;
	}

	/* Init the rest */

	nr_tcrossings = 0;
	size_upper_list = size_lower_list = 0;
	lower_list = lower_list_end = NULL;
	upper_list = upper_list_end = NULL;

	/* Traverse the layers alternating and finish the nodes */

	vl1 = TSUCC(tmp_layer[level]);
	vl2 = TSUCC(tmp_layer[level+1]);
	while ((vl1)||(vl2)) {
		if (vl1) {
			finish_upper(GNNODE(vl1));
			vl1 = GNNEXT(vl1);
		}
		if (vl2) {
			finish_lower(GNNODE(vl2));
			vl2 = GNNEXT(vl2);
		}
	}
	assert((size_upper_list==0));
	assert((size_lower_list==0));
	assert((lower_list==NULL));
	assert((upper_list==NULL));
	return(nr_tcrossings);
} /* layer_crossing */


/* Finish a node at the upper level
 * --------------------------------
 * and touch all nonfinished nodes.
 * If an occurence of v is found in the upper list, there exist an
 * edge e = (v,w) with NPOS(w) < NPOS(v). In this case, elements in 
 * upper_list before that occurence indicate edges whose target
 * have lower NPOS then NPOS(w), but they are not yet finished,
 * i.e. they cross of e.
 *           
 *                ,--v   z      upper_list = z z v v 
 *        _______/__/___/       Predecessors!=v of first  v : 2
 *       /  ____/__/___/        Predecessors!=v of second v : 2
 *      /  /   /  /             => Crossings 4 (calculated in k3)
 *     a  b   c  d
 *
 * Nodes y in the lower_list indicate edges (x,y) that are not yet 
 * finished, i.e. NPOS(x) < NPOS(v) < NPOS(y).
 * Thus they cross each edge starting at v to nodes with NPOS<NPOS(v).
 *
 *    x1  x2   v
 *     \    \/_|__       lower_list = y1 y2   upper_list = ... v v
 *      \___/__|_ \      c and d are finished if v is twice in upper_list
 *         /   | \ \     Size of lower_list: 2   Nr.occurences of v: 2
 *   ...  c    d  y1 y2    => Crossings 2*2 = 4 (i.e. k1 * size_lower_list)
 *
 * Crossings of edges that start at v to nodes n with NPOS(n)>NPOS(v) 
 * are calculated if we finish these nodes n. Thus we touch them here
 * only.
 */

static void 	finish_upper(GNODE v)
{
	ADJEDGE a;
	DLLIST n,m;
	int k1;  	/* number occurences of v in upper_list */
	int k2;		/* number of nodes!=v in upper_list that*/
			/* precede an occurence of v            */
	int k3;		/* number of edges that start before v  */
			/* to nonfinished nodes.		*/
	debugmessage("finish_upper","");
	assert((v));
	k1 = k2 = k3 = 0;
	if (NVPTR(v)) {
		n = upper_list;
		/* for n in upper_list up to NVPTR(v) including ... */
		while (n) {
			m = DSUCC(n);
			if (DNODE(n) == v) {
				k1++;
				k3 += k2;
				delete_upper(n);
			}	
			else k2++;
			if (n==NVPTR(v)) break;
			n = m;
		}
		nr_tcrossings += (k1 * size_lower_list + k3);
	}
	/* for all NSUCC's of v in order of their NPOS ... 
	 * The adjacency lists are already sorted according to the
	 * NPOS, see initialization phase in layer_crossing.
	 */
	a = NSUCC(v);
	while (a) {
		if (NPOS(TARGET(a)) > NPOS(v)) {
			/* nonfinished: touch it */
			append_to_lower(TARGET(a));
			NVPTR(TARGET(a)) = lower_list_end;
		}
		a = ANEXT(a);
	}
} /* finish_upper */


/* Finish a node at the lower level
 * --------------------------------
 * and touch all nonfinished nodes.
 * This is symmetrical to finish_upper.
 */

static void 	finish_lower(GNODE v)
{
	ADJEDGE a;
	DLLIST n,m;
	int k1;  	/* number occurences of v in lower_list */
	int k2;		/* number of nodes!=v in lower_list that*/
			/* precede an occurence of v            */
	int k3;		/* number of edges that start before v  */
			/* to nonfinished nodes.		*/
	debugmessage("finish_lower","");
	assert((v));
	k1 = k2 = k3 = 0;
	if (NVPTR(v)) {
		n = lower_list;
		/* for n in lower_list up to NVPTR(v) including ... */
		while (n) {
			m = DSUCC(n);
			if (DNODE(n) == v) {
				k1++;
				k3 += k2;
				delete_lower(n);
			}	
			else k2++;
			if (n==NVPTR(v)) break;
			n = m;
		}
		nr_tcrossings += (k1 * size_upper_list + k3);
	}

	/* for all NPRED's of v in order of their NPOS ... 
	 * The adjacency lists are already sorted according to the
	 * NPOS, see initialization phase in layer_crossing.
	 */
	a = NPRED(v);
	while (a) {
		if (NPOS(SOURCE(a)) > NPOS(v)) {
			/* nonfinished: touch it */
			append_to_upper(SOURCE(a));
			NVPTR(SOURCE(a)) = upper_list_end;
		}
		a = ANEXT(a);
	}
} /* finish_lower */


/*--------------------------------------------------------------------*/
/*  Management of upper_list and lower_list                           */
/*--------------------------------------------------------------------*/

/*  Insert node n at the end of upper_list
 *  --------------------------------------
 */

static void	append_to_upper(GNODE n)
{
	DLLIST	d;

	assert((n));
	debugmessage("append_to_upper", (NTITLE(n)?NTITLE(n):""));

	d = dllist_alloc(n,upper_list_end);
	if (!upper_list)	upper_list = d;
	if (upper_list_end)	DSUCC(upper_list_end) = d;
	upper_list_end 	= d;
	size_upper_list++;
} /* append_to_upper */


/*  Insert node n at the end of lower_list
 *  --------------------------------------
 */

static void	append_to_lower(GNODE n)
{
	DLLIST	d;

	assert((n));
	debugmessage("append_to_lower", (NTITLE(n)?NTITLE(n):""));

	d = dllist_alloc(n,lower_list_end);
	if (!lower_list)	lower_list = d;
	if (lower_list_end)	DSUCC(lower_list_end) = d;
	lower_list_end 	= d;
	size_lower_list++;
} /* append_to_lower */


/*  Delete node x from upper_list
 *  -----------------------------
 */

static void 	delete_upper(DLLIST x)
{
	assert((x));
	assert((DNODE(x)));
	debugmessage("delete_upper",
		(NTITLE(DNODE(x))?NTITLE(DNODE(x)):""));
	if (DPRED(x)) DSUCC(DPRED(x)) = DSUCC(x);
	else	      upper_list      = DSUCC(x);
	if (DSUCC(x)) DPRED(DSUCC(x)) = DPRED(x);
	else	      upper_list_end  = DPRED(x);
	dllist_free(x);
	size_upper_list--;
}


/*  Delete node x from lower_list
 *  -----------------------------
 */

static void 	delete_lower(DLLIST x)
{
	assert((x));
	assert((DNODE(x)));
	debugmessage("delete_lower",
		(NTITLE(DNODE(x))?NTITLE(DNODE(x)):""));
	if (DPRED(x)) DSUCC(DPRED(x)) = DSUCC(x);
	else	      lower_list      = DSUCC(x);
	if (DSUCC(x)) DPRED(DSUCC(x)) = DPRED(x);
	else	      lower_list_end  = DPRED(x);
	dllist_free(x);
	size_lower_list--;
}


/*--------------------------------------------------------------------*/
/*  Calculate local number of crossings                               */
/*--------------------------------------------------------------------*/

/* Different than before, we now are interested on the crossings
 * that are produced only by to selected neighboured nodes.
 * We want to check what happens if if exchange these two nodes.
 *
 * Example:     A   B                      A   B
 *               \ /                       |   |
 *                X                        |   |
 *               / \      should become    |   |
 *              C   D                      D   C
 *               \ /                        \ /
 *                E                          E
 *
 * Thus we calculate the crossings produced by C and D. This problem
 * is much simpler than to calculate the crossings of a whole layer.
 * First, we assume, that the nodes in the adjacent layers of C and D
 * are numbered in increasing order.
 * We build four lists ordered according to that number:
 *     1) the list of predecessors of C = predC
 *     2) the list of successors   of C = succC
 *     3) the list of predecessors of D = predD
 *     4) the list of successors   of D = succD
 *
 * Now, we count for each element of predD how many element of predC
 * are greater. Dito for each element of succC with succD. This gives Sum1.
 * Next, we count for each element of predD how many elements of predC
 * are smaller. Dito for each element of succC with succD. This gives Sum2.
 *
 * If Sum1 <= Sum2, then C should come before D, else C should come after D.
 *
 * In fact, we dont need to construct predD and succD really.
 */ 

static void prepare_local_optimization(int level)
{
	int i;
	GNLIST  vl;

	debugmessage("prepare_local_optimization","");
	
	if (level>0) {
		i = 1;
		vl = TSUCC(tmp_layer[level-1]);
		while (vl) {
			NPOS(GNNODE(vl))  = i++;
			vl = GNNEXT(vl);
		}
	}
	if (level<=maxdepth) {
		i = 1;
		vl = TSUCC(tmp_layer[level+1]);
		while (vl) {
			NPOS(GNNODE(vl))  = i++;
			vl = GNNEXT(vl);
		}
	}
}

/*  Crossing comparision of two neighboured nodes 
 *  =============================================
 *  This function returns 1, if the nodes C and D have less crossings
 *  if they are exchanged.
 */

static int exchange_nodes_necessary(GNODE C, GNODE D)
{
	GNODE n;
	DLLIST actlistC, h;
	ADJEDGE a;
	int Sum1, Sum2;

	debugmessage("exchange_nodes_necessary","");

	Sum1 = Sum2 = 0;

	/* First, build predC. We use actlistC to represent predC */

	actlistC = NULL;
	a = NPRED(C);
	while (a) {
		actlistC = dllist_alloc(SOURCE(a),actlistC);
		DSUCC(actlistC) = DPRED(actlistC);  /* see dllist_free_all */
		a = ANEXT(a);
	}

	/* We calculate the part of Sum1 and Sum2 originated by predC */

	a = NPRED(D);
	while (a) {
		n = SOURCE(a);
		h = actlistC;
		while (h) {
			if (NPOS(n)<NPOS(DNODE(h)))      Sum1++;
			else if (NPOS(n)>NPOS(DNODE(h))) Sum2++;
			h = DSUCC(h);
		}
		a = ANEXT(a);
	}
	dllist_free_all(actlistC);

	/* Next, build succC. We use actlistC to represent succC */

	actlistC = NULL;
	a = NSUCC(C);
	while (a) {
		actlistC = dllist_alloc(TARGET(a),actlistC);
		DSUCC(actlistC) = DPRED(actlistC);  /* see dllist_free_all */
		a = ANEXT(a);
	}

	/* We calculate the part of Sum1 and Sum2 originated by succC */

	a = NSUCC(D);
	while (a) {
		n = TARGET(a);
		h = actlistC;
		while (h) {
			if (NPOS(n)<NPOS(DNODE(h)))      Sum1++;
			else if (NPOS(n)>NPOS(DNODE(h))) Sum2++;
			h = DSUCC(h);
		}
		a = ANEXT(a);
	}
	dllist_free_all(actlistC);

	return(Sum1>Sum2);
} /* exchange_nodes_necessary */



/* Optimization of the crossings of one level
 * ------------------------------------------
 * return 1, if something has changed.
 */

static int level_crossing_optimization(int level,int nearedges)
{
	int changed, possible;
	GNLIST  vl1, vl2;
	GNODE n1,n2;

	debugmessage("level_crossing_optimization","");

	assert((level>=0));
	assert((level<=maxdepth+1));

	prepare_local_optimization(level);

	changed = 0;
	vl1 = NULL;
	vl2 = TSUCC(tmp_layer[level]);
	while (vl2) {
		n2 = GNNODE(vl2);
		if (vl1) {
			n1 = GNNODE(vl1);
			possible = 1;
			if (nearedges) {
				if ((NCONNECT(n1))||(NCONNECT(n2)))
					possible = 0;
			}
			if (   (possible)
			     &&(exchange_nodes_necessary(n1,n2))) {
				GNNODE(vl1) = n2;
				GNNODE(vl2) = n1; 
				changed = 1;
			}
		}
		vl1 = vl2;
		vl2 = GNNEXT(vl2);
	}
	return(changed);
} /* level_crossing_optimization */



/* Local optimization of crossings for all levels
 * ----------------------------------------------
 */

#ifdef ANSI_C
static void local_crossing_optimization(int nearedges)
#else
static void local_crossing_optimization(nearedges)
int nearedges;
#endif
{
	int i;

	debugmessage("local_crossing_optimization","");

	if (!local_unwind) return;
	if (G_timelimit>0)
              	if (test_timelimit(60)) {
               		gs_wait_message('t');
			return;
       		}
	gs_wait_message('l');
	for (i=0; i<=maxdepth+1; i++) 
		while (level_crossing_optimization(i, nearedges));
}

/*--------------------------------------------------------------------*/
/*  Crossing unwinding of pairs of edges                              */
/*--------------------------------------------------------------------*/

/* Pairs of edges may produce crossings, if they are adjacent to the same
 * node. This is really ugly, and sometimes it is not resolved by 
 * barycentering or local optimization.
 *
 * Example:     A   B                      A   B
 *               \ /                       |   |
 *                X                        |   |
 *               / \      should become    |   |
 *              C   D                      D   C
 *              |   |                      |   |
 *              |   |                      |   |
 *              E   F                      F   E
 *               \ /                        \ /
 *                G                          G
 *
 * The succBary of E and F have the same value, and exchanging of E and F
 * alone is not appropriate. We have to follow the edges E and F until
 * the crossing is found.
 */



/* Assign position numbers to the nodes at each level
 * --------------------------------------------------
 */

#ifdef ANSI_C
static void prepare_positions(void)
#else
static void prepare_positions()
#endif
{
	GNLIST h1;
	int i,j;

	debugmessage("prepare_positions","");

	/* First, give all nodes their position numbers */
	for (i=0; i<=maxdepth+1; i++) { 
		h1 = TSUCC(tmp_layer[i]);
		j = 0;
		while (h1) {
			NPOS(GNNODE(h1)) = j++;
			h1 = GNNEXT(h1);
		}
	}
}


/* Check whether a node is complex
 * -------------------------------
 * A node is simple, if it has exactly an predecessor and one
 * successor. This holds for the most dummy nodes that are used
 * to split edges that cross levels.
 */

#ifdef ANSI_C
static int is_complex(GNODE v)
#else
static int is_complex(v)
GNODE v;
#endif
{
	ADJEDGE a;

	debugmessage("is_complex","");

	if (NCONNECT(v)) return(1);
	a = NSUCC(v);
	if (!a) return(1);
	if (ANEXT(a)) return(1);
	a = NPRED(v);
	if (!a) return(1);
	if (ANEXT(a)) return(1);
	return(0);
}


/* Check whether two chains of simple nodes must be exchanged.
 * ----------------------------------------------------------
 * The chain starts at v1 and v2, which should be neighboured at the
 * same level.
 * If dir='S', the chain goes in successor direction, otherwise in
 * predecessor direction.
 * Exchange is necessary, if both chains cross anywhere, but not
 * necessary, if they stop at complex nodes, or if they are not
 * neighboured.
 */

#ifdef ANSI_C
static int check_exchange(GNODE v1,GNODE v2,int dir)
#else
static int check_exchange(v1,v2,dir)
GNODE v1;
GNODE v2;
int dir;
#endif
{
	int d1,d2;
	GNODE n1,n2;

	debugmessage("check_exchange","");

	if (is_complex(v1)) return(0);
	if (is_complex(v2)) return(0);
	d1 = NPOS(v1)-NPOS(v2);
	if (d1*d1!=1) return(0);

	if (dir=='S') {
		n1 = TARGET(NSUCC(v1));
		n2 = TARGET(NSUCC(v2));
	}
	else { /* dir == 'P' */
		n1 = SOURCE(NPRED(v1));
		n2 = SOURCE(NPRED(v2));
 	}
	d2 = NPOS(n1)-NPOS(n2);
	if (d1*d2<0) return(1);

	return(check_exchange(n1,n2,dir));
}


/* Exchange two chains of simple nodes
 * -----------------------------------
 * Same as before, but this is not a check. We do the exchanging.
 * The NPOS-numbers are updated, too.
 */

#ifdef ANSI_C
static void do_exchange(GNODE v1,GNODE v2,int dir)
#else
static void do_exchange(v1,v2,dir)
GNODE v1;
GNODE v2;
int dir;
#endif
{
	int 	d1,d2,h;
	GNODE 	n1,n2;
	GNLIST  vl1, vl2, vl3;

	debugmessage("do_exchange","");

	if (is_complex(v1)) return;
	if (is_complex(v2)) return;
	d1 = NPOS(v1)-NPOS(v2);
	if (d1*d1!=1) return;

	assert((NTIEFE(v1)==NTIEFE(v2)));
	vl1 = vl2 = NULL;
	vl3 = TSUCC(tmp_layer[NTIEFE(v1)]);
	while (vl3) {
		if (GNNODE(vl3)==v1) vl1 = vl3;
		if (GNNODE(vl3)==v2) vl2 = vl3;
		vl3 = GNNEXT(vl3);
	}
	assert((vl1));
	assert((vl2));
	GNNODE(vl1) = v2;
	GNNODE(vl2) = v1; 
	h = NPOS(v1); 
	NPOS(v1) = NPOS(v2);
	NPOS(v2) = h; 

	if (dir=='S') {
		n1 = TARGET(NSUCC(v1));
		n2 = TARGET(NSUCC(v2));
	}
	else { /* dir == 'P' */
		n1 = SOURCE(NPRED(v1));
		n2 = SOURCE(NPRED(v2));
 	}
	d2 = NPOS(n1)-NPOS(n2);
	if (d1*d2<0) return;
	do_exchange(n1,n2,dir);
}


/* Check chain exchange for all pair of edges of a node v
 * ------------------------------------------------------
 */

#ifdef ANSI_C
static void unwind_crossed_edges(GNODE v)
#else
static void unwind_crossed_edges(v)
GNODE v;
#endif
{
	ADJEDGE a1,a2;

	debugmessage("unwind_crossed_edges","");

	a1 = NSUCC(v);
	while (a1) {
		a2 = NSUCC(v);
		while (a2) {
			if (check_exchange(TARGET(a1),TARGET(a2),'S')) {
				do_exchange(TARGET(a1),TARGET(a2),'S');
			}
			a2 = ANEXT(a2);
		}
		a1 = ANEXT(a1);
	}
	a1 = NPRED(v);
	while (a1) {
		a2 = NPRED(v);
		while (a2) {
			if (check_exchange(SOURCE(a1),SOURCE(a2),'P')) {
				do_exchange(SOURCE(a1),SOURCE(a2),'P');
			}
			a2 = ANEXT(a2);
		}
		a1 = ANEXT(a1);
	}
}


/* The entry point of the local unwind phase
 * -----------------------------------------
 */

#ifdef ANSI_C
static void local_unwind_crossings(void)
#else
static void local_unwind_crossings()
#endif
{
	GNODE v;

	debugmessage("local_unwind_crossings","");

	if (G_timelimit>0)
              	if (test_timelimit(60)) {
               		gs_wait_message('t');
			return;
       		}
	gs_wait_message('l');
	prepare_positions();
	v = nodelist;
	while (v) {
		unwind_crossed_edges(v);
		v = NNEXT(v);
	}
}

/*--------------------------------------------------------------------*/
/*  Barycenter layout                                                 */
/*--------------------------------------------------------------------*/

/* Barycenter works with two graphs: one in layer and one in tmp_layer.
 * It consists of several phases. If the result of one phase is not
 * better nor worser than before, we can do the next phase with both
 * layers. In this case we do the next phase with tmp_layer but 
 * set the flag `have_alternative' to indicate that layer is also
 * a possible alternative.
 * In this case, if the next step was not succesful, we try it with
 * the alternative layer.
 */

static int have_alternative;
static int phase1_allowed;

/* Initialize the barycenter method 
 * --------------------------------
 */

#ifdef ANSI_C
static void init_barycentering(void)
#else
static void init_barycentering()
#endif
{
	have_alternative = 0;
	phase1_allowed   = 1;
}


/*  The barycentering main routine
 *  ==============================
 *  traverses the levels is for steps:
 *  Each step changes the tmp_layer. If the number of crossings is
 *  reduced, the tmp_layer is copied into the layer. If the number
 *  of crossings is worser than before, the original tmp_layer
 *  is produced by copying layer again into tmp_layer.
 *  If both numbers of crossings are equal, we denote layer as an
 *  alternation backtrack point, but continue to work with tmp_layer.
 *  Before we create the original layer in the next step that has
 *  produced a worser layer, we try this backtrack point.
 */ 


/* Some abbreviatons */

#define tmp_layer_is_better() {          \
	changed = 1;		         \
	copy_tmp_layer_to_layer();       \
	nr_crossings = cross;            \
	alt_startlevel = tmp_startlevel; \
} 

#define tmp_layer_is_worser() {          \
	copy_layer_to_tmp_layer();       \
	tmp_startlevel = alt_startlevel; \
}

#define	copy_tmp_layer_to_layer()  { copy_layers(layer,tmp_layer); }
#define	copy_layer_to_tmp_layer()  { copy_layers(tmp_layer,layer); }

/*
 *  Note that Phase[1,2][_down,_up] fill all TCROSS(tmp_layer[i])
 *  such the access graph_crossings() is allowed.
 *  Phase 2 needs the level where to start.
 */

static int phase2_startlevel;

#ifdef ANSI_C
static void    barycentering(void)
#else
static void    barycentering()
#endif
{
	int alt;	/* temporary flag: exist an alternative ? */
	int cross;	/* number crossings of tmp_layer          */
	int changed;
	int tmp_startlevel,alt_startlevel;

	debugmessage("barycentering","");

	assert(tmp_layer);

	tmp_startlevel = alt_startlevel = 0;
	changed = 1;
	while (phase1_allowed && changed) {
		nr_bary_iterations++;
		if (nr_crossings==0) return; /* is already optimal */
		if (nr_bary_iterations>=max_baryiterations) {
                       	gs_wait_message('t');
			break;
		}
	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
		changed = 0;
	        Phase1_down();
	       	cross = graph_crossings();
	       	alt = 0;
	        if (  (cross < nr_crossings) 
		    ||(nr_bary_iterations<min_baryiterations)) { 
			tmp_layer_is_better(); 
		} 
	        else if (have_alternative) {
	                copy_layer_to_tmp_layer();
	                Phase1_down();
	                cross = graph_crossings();
	                if      (cross < nr_crossings)     { tmp_layer_is_better(); }
	                else if (cross > nr_crossings)     { tmp_layer_is_worser(); }
	                else  /* cross == nr_crossings */  alt = 1;
		}
		else if     (cross == nr_crossings)        alt = 1;
	        else      /*(cross > nr_crossings) */      { tmp_layer_is_worser(); }
		have_alternative = alt;

		if (nr_crossings==0) return;

       		Phase1_up();
        	cross = graph_crossings();
        	alt = 0;
	        if (  (cross < nr_crossings) 
		    ||(nr_bary_iterations<min_baryiterations)) { 
			tmp_layer_is_better(); 
		} 
        	else if (have_alternative) {
                	copy_layer_to_tmp_layer();
                	Phase1_up();
                	cross = graph_crossings();
                	if      (cross < nr_crossings)     { tmp_layer_is_better(); }
                	else if (cross > nr_crossings)     { tmp_layer_is_worser(); }
                	else  /* cross == nr_crossings */  alt = 1;
		}
		else if     (cross == nr_crossings)        alt = 1;
        	else      /*(cross > nr_crossings) */      { tmp_layer_is_worser(); }
		have_alternative = alt;
	}

	if (nr_crossings==0) return; /* is already optimal */
	if (skip_baryphase2) return;

	phase1_allowed   = 0; 

	tmp_startlevel = alt_startlevel = 0;
	changed = 1;
	while (changed) {
		nr_bary_iterations++;
		if (nr_bary_iterations>=max_baryiterations) {
                       	gs_wait_message('t');
			break;
		}
	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
		changed = 0;
		phase2_startlevel = tmp_startlevel;
	        Phase2_down();
		tmp_startlevel = phase2_startlevel;
		if (tmp_startlevel > maxdepth) tmp_startlevel = 0;
       		cross = graph_crossings();
        	alt = 0;
	        if (cross < nr_crossings) { tmp_layer_is_better(); } 
        	else if (have_alternative) {
                	copy_layer_to_tmp_layer();
			phase2_startlevel = tmp_startlevel = alt_startlevel;
                	Phase2_down();
			tmp_startlevel = phase2_startlevel;
			if (tmp_startlevel > maxdepth) tmp_startlevel = 0;
                	cross = graph_crossings();
                	if      (cross < nr_crossings)     { tmp_layer_is_better(); }
                	else if (cross > nr_crossings)     { tmp_layer_is_worser(); }
                	else  /* cross == nr_crossings */  alt = 1;
		}
		else if     (cross == nr_crossings)        alt = 1;
        	else      /*(cross > nr_crossings) */      { tmp_layer_is_worser(); }
		have_alternative = alt;
		if (nr_crossings==0) return;
	}


	tmp_startlevel = alt_startlevel = maxdepth+1;
	changed = 1;
	while (changed) {
		nr_bary_iterations++;
		if (nr_bary_iterations>=max_baryiterations) {
                       	gs_wait_message('t');
			break;
		}
	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
		changed = 0;
		phase2_startlevel = tmp_startlevel;
        	Phase2_up();
		tmp_startlevel = phase2_startlevel;
		if (tmp_startlevel < 0) tmp_startlevel = maxdepth;
        	cross = graph_crossings();
        	alt = 0;
	        if (cross < nr_crossings) { tmp_layer_is_better(); } 
        	else if (have_alternative) {
               		copy_layer_to_tmp_layer();
			phase2_startlevel = tmp_startlevel = alt_startlevel;
                	Phase2_up();
			tmp_startlevel = phase2_startlevel;
			if (tmp_startlevel < 0) tmp_startlevel = maxdepth;
                	cross = graph_crossings();
                	if      (cross < nr_crossings)     { tmp_layer_is_better(); }
                	else if (cross > nr_crossings)     { tmp_layer_is_worser(); }
                	else  /* cross == nr_crossings */  alt = 1;
		}
		else if     (cross == nr_crossings)        alt = 1;
        	else      /*(cross > nr_crossings) */      { tmp_layer_is_worser(); }
		have_alternative = alt;
		if (nr_crossings==0) return;
	}
}

/*--------------------------------------------------------------------*/
/*  Phases 1                                                          */
/*--------------------------------------------------------------------*/

/*  The phases 1 simply traverse all layers, calculate the predbary's
 *  or succbary's and sort the nodes of the layers according
 *  to the bary-values. 
 */


/*  Phase 1 downwards
 *  -----------------
 *  traverses all levels downwards, calculates the predbary values
 *  and sorts a the nodes of each level according to the predbary values.
 */


#ifdef ANSI_C
static void    Phase1_down(void)
#else
static void    Phase1_down()
#endif
{
        int     i;

	debugmessage("Phase1_down","");
	gs_wait_message('b');
        for (i=0; i<=maxdepth; i++) (void)resort_down_layer(i);
#ifdef CHECK_CROSSING
        i = graph_crossings();
	calc_all_layers_crossings();
	assert((i==graph_crossings()));
	PRINTF("Phase1_down: nr_crossings old: %d new: %d\n",nr_crossings,i); 
#endif
}


/*  Phase 1 upwards
 *  ---------------
 *  traverses all levels upwards, calculates the succbary values
 *  and sorts a the nodes of each level according to the succbary values.
 */

#ifdef ANSI_C
static void    Phase1_up(void)
#else
static void    Phase1_up()
#endif
{
        int     i;

	debugmessage("Phase1_up","");
	gs_wait_message('b');
        for (i=maxdepth; i>=0; i--) (void)resort_up_layer(i);
#ifdef CHECK_CROSSING
        i = graph_crossings();
	calc_all_layers_crossings();
	assert((i==graph_crossings()));
	PRINTF("Phase1_up: nr_crossings old: %d new: %d\n",nr_crossings,i); 
#endif
}


/*  Resort a layer according to the predbary-values
 *  -----------------------------------------------
 *  The layer i+1 is stored into an array. This array is sorted
 *  according to the predbary-values. If after sorting the 
 *  number of crossings between level i and i+1 is reduced,
 *  we use the new ordering of the level i+1, otherwise we use
 *  the old ordering.
 *  We return 1 if this step has changed level i+1.
 */

#ifdef ANSI_C
static int     resort_down_layer(int i)
#else
static int     resort_down_layer(i)
int     i;
#endif
{
	int c;
	int j;

	debugmessage("resort_down_layer","");
	assert((i>=0));
	assert((i<=maxdepth));   /* we access to maxdepth+1 which exists */
	/* Assertion: TCROSS(tmp_layer[i]) is correctly initialized */

        level_to_array(i+1,'d');
	switch (crossing_heuristics) {
	case 0: for (j=0; j<TANZ(layer[i+1]); j++)
			NBARY(sort_array[j]) = predbary(sort_array[j]);
		break;
	case 1: for (j=0; j<TANZ(layer[i+1]); j++)
			NBARY(sort_array[j]) = predmedian(sort_array[j]);
		break;
	case 2: for (j=0; j<TANZ(layer[i+1]); j++)
			NBARY(sort_array[j]) = predbary(sort_array[j])
				+ predmedian(sort_array[j])/10000.0;
		break;
	case 3: for (j=0; j<TANZ(layer[i+1]); j++)
			NBARY(sort_array[j]) = predmedian(sort_array[j])
				+ predbary(sort_array[j])/10000.0;
		break;
	}
	quicksort_sort_array(TANZ(layer[i+1]));

	save_level(i+1);	/* save old level temporary */

        array_to_level(i+1);
	if (TRESNEEDED(layer[i+1])) apply_horder(i+1); 

	c = layer_crossing(i);

        if (c <= TCROSS(tmp_layer[i])) {
		/* the new level+1 is better than the old one */

		TCROSS(tmp_layer[i]) = c;
		if (i<maxdepth) TCROSS(tmp_layer[i+1])= layer_crossing(i+1);	
                return(1);
        }
	/* the old level+1 is better than the new one. Thus restore it. */

	restore_level(i+1);
        return(0);
}

/*  Resort a layer according to the succbary-values
 *  -----------------------------------------------
 *  The layer i is stored into an array. This array is sorted
 *  according to the succbary-values. If after sorting the 
 *  number of crossings between level i and i+1 is reduced,
 *  we use the new ordering of the level i, otherwise we use
 *  the old ordering.
 *  We return 1 if this step has changed level i.
 */

#ifdef ANSI_C
static int     resort_up_layer(int i)
#else
static int     resort_up_layer(i)
int     i;
#endif
{
	int c;
	int j;

	debugmessage("resort_up_layer","");
	assert((i>=0));
	assert((i<=maxdepth));   /* we access to maxdepth+1 which exists */
	/* Assertion: TCROSS(tmp_layer[i]) is correctly initialized */

        level_to_array(i,'u');
	switch (crossing_heuristics) {
	case 0: for (j=0; j<TANZ(layer[i]); j++)
			NBARY(sort_array[j]) = succbary(sort_array[j]);
		break;
	case 1: for (j=0; j<TANZ(layer[i]); j++)
			NBARY(sort_array[j]) = succmedian(sort_array[j]);
		break;
	case 2: for (j=0; j<TANZ(layer[i]); j++)
			NBARY(sort_array[j]) = succbary(sort_array[j])
				+ succmedian(sort_array[j])/10000.0;
		break;
	case 3: for (j=0; j<TANZ(layer[i]); j++)
			NBARY(sort_array[j]) = succmedian(sort_array[j])
				+ succbary(sort_array[j])/10000.0;
		break;
	}
	quicksort_sort_array(TANZ(layer[i]));

	save_level(i);		/* save old level temporary */

        array_to_level(i);
	if (TRESNEEDED(layer[i])) apply_horder(i); 

	c = layer_crossing(i);

        if (c <= TCROSS(tmp_layer[i])) {
		/* the new level is better than the old one */

		TCROSS(tmp_layer[i]) = c;
		if (i>0) TCROSS(tmp_layer[i-1])= layer_crossing(i-1);	
                return(1);
        }
	/* the old level is better than the new one. Thus restore it. */

	restore_level(i);
        return(0);
}


/*--------------------------------------------------------------------*/
/*  Phases 2                                                          */
/*--------------------------------------------------------------------*/

/*  The phases 2 search the points where bary-values are equal, and
 *  change these orders cyclic. Starting from these points, it calculates
 *  the predbary's or succbary's and sort the nodes of the layers according
 *  to the bary-values.
 *  E.g. an situation where this is necessary:
 *      
 *         A    B    C    D                        A    D   B    C
 *         |\___|\__/|_  /|                        |\  /|   |\  /|
 *         |    | \/ | \/ |  should be layouted as | \/ |   | \/ |
 *         | ___|_/\_|_/\ |                        | /\ |   | /\ |
 *         |/   |/  \|   \|                        |/  \|   |/  \|
 *         E    F    G    H                        E    H   F    G
 */

/*  Phase 2 downwards
 *  -----------------
 *  traverses all levels downwards, look where bary-values are equal,
 *  cycles these regions in the layers, and starts a resorting 
 *  traversal from the layer locally downwards and upwards through the layers.
 */


#ifdef ANSI_C
static void    Phase2_down(void)
#else
static void    Phase2_down()
#endif
{
        int     i,j;
	int 	cross;

	debugmessage("Phase2_down","");

	gs_wait_message('B');
	if (phase2_startlevel <= maxdepth)
            for (i=phase2_startlevel; i<=maxdepth; i++) {

	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
        	level_to_array(i,'u');
		switch (crossing_heuristics) {
		case 0: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = succbary(sort_array[j]);
			break;
		case 1: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = succmedian(sort_array[j]);
			break;
		case 2: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = succbary(sort_array[j])
					+ succmedian(sort_array[j])/10000.0;
			break;
		case 3: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = succmedian(sort_array[j])
					+ succbary(sort_array[j])/10000.0;
			break;
		}
		quicksort_sort_array(TANZ(layer[i]));
                if (cycle_sort_array(TANZ(layer[i]))) {
        		array_to_level(i);
			if (TRESNEEDED(layer[i])) apply_horder(i); 

			if (i>0) TCROSS(tmp_layer[i-1]) = layer_crossing(i-1);
			if (i<=maxdepth) TCROSS(tmp_layer[i]) = layer_crossing(i);
                        resort_up_down_layer(i);
        		cross = graph_crossings();
        		if (cross < nr_crossings) { 
#ifdef CHECK_CROSSING
			        j = graph_crossings();
				calc_all_layers_crossings();
				assert((j==graph_crossings()));
				PRINTF("Phase2_down: nr_crossings old: %d new: %d\n",nr_crossings,j); 
#endif
				phase2_startlevel = i+1;
				return; 
			} 
                }
        }
        for (i=0; (i<phase2_startlevel) && (i<=maxdepth); i++) {

	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
        	level_to_array(i,'u');
		switch (crossing_heuristics) {
		case 0: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = predbary(sort_array[j]);
			break;
		case 1: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = predmedian(sort_array[j]);
			break;
		case 2: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = predbary(sort_array[j])
					+ predmedian(sort_array[j])/10000.0;
			break;
		case 3: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = predmedian(sort_array[j])
					+ predbary(sort_array[j])/10000.0;
			break;
		}
		quicksort_sort_array(TANZ(layer[i]));
                if (cycle_sort_array(TANZ(layer[i]))) {
        		array_to_level(i);
			if (TRESNEEDED(layer[i])) apply_horder(i); 

			if (i>0) TCROSS(tmp_layer[i-1]) = layer_crossing(i-1);
			if (i<=maxdepth) TCROSS(tmp_layer[i]) = layer_crossing(i);
                        resort_up_down_layer(i);
        		cross = graph_crossings();
        		if (cross < nr_crossings) { 
#ifdef CHECK_CROSSING
			        j = graph_crossings();
				calc_all_layers_crossings();
				assert((j==graph_crossings()));
				PRINTF("Phase2_down: nr_crossings old: %d new: %d\n",nr_crossings,j); 
#endif
				phase2_startlevel = i+1;
				return; 
			} 
                }
        }
#ifdef CHECK_CROSSING
        i = graph_crossings();
	calc_all_layers_crossings();
	assert((i==graph_crossings()));
	PRINTF("Phase2_down: nr_crossings old: %d new: %d\n",nr_crossings,i); 
#endif
}


/*  Phase 2 upwards
 *  ---------------
 *  traverses all levels upwards, look where bary-values are equal,
 *  cycles these regions in the layers, and starts a resorting 
 *  traversal from the layer locally downwards and upwards through the layers.
 */


#ifdef ANSI_C
static void    Phase2_up(void)
#else
static void    Phase2_up()
#endif
{
        int     i,j;
	int 	cross;

	debugmessage("Phase2_up","");

	gs_wait_message('B');
	if (phase2_startlevel > 0)
            for (i=phase2_startlevel; i>0; i--) {

	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
        	level_to_array(i,'d');
		switch (crossing_heuristics) {
		case 0: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = predbary(sort_array[j]);
			break;
		case 1: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = predmedian(sort_array[j]);
			break;
		case 2: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = predbary(sort_array[j])
					+ predmedian(sort_array[j])/10000.0;
			break;
		case 3: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = succmedian(sort_array[j])
					+ predbary(sort_array[j])/10000.0;
			break;
		}
		quicksort_sort_array(TANZ(layer[i]));
                if (cycle_sort_array(TANZ(layer[i]))) {
        		array_to_level(i);
			if (TRESNEEDED(layer[i])) apply_horder(i); 

			if (i>0) TCROSS(tmp_layer[i-1]) = layer_crossing(i-1);
			if (i<=maxdepth) TCROSS(tmp_layer[i]) = layer_crossing(i);
                        resort_down_up_layer(i);
        		cross = graph_crossings();
        		if (cross < nr_crossings) { 
#ifdef CHECK_CROSSING
			        j = graph_crossings();
				calc_all_layers_crossings();
				assert((j==graph_crossings()));
				PRINTF("Phase2_up: nr_crossings old: %d new: %d\n",nr_crossings,j); 
#endif
				phase2_startlevel = i-1;
				return; 
			} 
                }
        }
        for (i=maxdepth+1; (i>phase2_startlevel) && (i>0); i--) {

	        if (G_timelimit>0)
                	if (test_timelimit(60)) {
                       		gs_wait_message('t');
				break;
        		}
        	level_to_array(i,'d');
		switch (crossing_heuristics) {
		case 0: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = predbary(sort_array[j]);
			break;
		case 1: for (j=0; j<TANZ(layer[i]); j++)
			     NBARY(sort_array[j]) = predmedian(sort_array[j]);
			break;
		case 2: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = predbary(sort_array[j])
					+ predmedian(sort_array[j])/10000.0;
			break;
		case 3: for (j=0; j<TANZ(layer[i]); j++)
				NBARY(sort_array[j]) = predmedian(sort_array[j])
					+ predbary(sort_array[j])/10000.0;
			break;
		}
		quicksort_sort_array(TANZ(layer[i]));
                if (cycle_sort_array(TANZ(layer[i]))) {
        		array_to_level(i);
			if (TRESNEEDED(layer[i])) apply_horder(i); 

			if (i>0) TCROSS(tmp_layer[i-1])= layer_crossing(i-1);	
			if (i<=maxdepth) TCROSS(tmp_layer[i]) = layer_crossing(i);
                        resort_down_up_layer(i);
        		cross = graph_crossings();
        		if (cross < nr_crossings) { 
#ifdef CHECK_CROSSING
			        j = graph_crossings();
				calc_all_layers_crossings();
				assert((j==graph_crossings()));
				PRINTF("Phase2_up: nr_crossings old: %d new: %d\n",nr_crossings,j); 
#endif
				phase2_startlevel = i-1;
				return; 
			} 
                }
        }
#ifdef CHECK_CROSSING
        i = graph_crossings();
	calc_all_layers_crossings();
	assert((i==graph_crossings()));
	PRINTF("Phase2_up: nr_crossings old: %d new: %d\n",nr_crossings,i); 
#endif
}


/*  Resort up and down 
 *  ------------------
 *  At level, the ordering has just changed.
 *  Starting at level, we traverse the layers downwards
 *  and resort until nothing changes anymore.
 *  Then we revert the direction, starting at level, we 
 *  traverse the layers upwards and resort until nothing 
 *  changes anymore.
 */

#ifdef ANSI_C
static void	resort_up_down_layer(int level)
#else
static void	resort_up_down_layer(level)
int level;
#endif
{
	int change;
	int i;

	debugmessage("resort_up_down","");

	change = 1;
	if (level>0) {
        	for (i=level-1;i>=0; i--) {
			change = resort_up_layer(i);
			if (!change) break; 
		}
	}
	if (level<=maxdepth) {
        	for (i=level; i<=maxdepth; i++) {
			change = resort_down_layer(i);
			if (!change) break; 
		}
	}
}


/*  Resort down and up 
 *  ------------------
 *  At level, the ordering has just changed.
 *  Starting at level, we traverse the layers downwards
 *  and resort until nothing changes anymore.
 *  Then we revert the direction, starting at level, we 
 *  traverse the layers upwards and resort until nothing 
 *  changes anymore.
 */

#ifdef ANSI_C
static void	resort_down_up_layer(int level)
#else
static void	resort_down_up_layer(level)
int level;
#endif
{
	int change;
	int i;

	debugmessage("resort_down_up","");

	change = 1;
	if (level<=maxdepth) {
        	for (i=level; i<=maxdepth; i++) {
			change = resort_down_layer(i);
			if (!change) break; 
		}
	}
	if (level>0) {
        	for (i=level-1;i>=0; i--) {
			change = resort_up_layer(i);
			if (!change) break; 
		}
	}
}



/*  Cycle the sort_array
 *  --------------------
 *  cycle the parts of the sort array where the NBARY-s are equal.
 *  We expect the size of the used part of sort_array in siz.
 *  The return value is 1 (true) if after cycling, the sort_array
 *  is NOT in the original situation.
 */

#ifdef ANSI_C
static int 	cycle_sort_array(int siz)
#else
static int 	cycle_sort_array(siz)
int siz;
#endif
{
	int j,k;
	int original_sit;  /* flag: original situation reached      */
	int start_region;  /* index of the start of the part        */
			   /* of sort_array where NBARY's are equal */
	GNODE h;

	debugmessage("cycle_sort_array","");

	original_sit = 1;
	start_region = -1;
	for (j=0; j<siz-1; j++) {
		if ( NBARY(sort_array[j]) == NBARY(sort_array[j+1]) ) {
			if (start_region == -1)	start_region = j;	
		}
		else if (start_region != -1) {

			/* cyclic shift to the right */
			h = sort_array[j];
			for (k=j; k>start_region; k--)
				sort_array[k] = sort_array[k-1];
			sort_array[start_region] = h;

			start_region = -1;
			original_sit =  0;
		}
	}	
	if (start_region != -1) {

		/* cyclic shift to the right */
		h = sort_array[j];
		for (k=j; k>start_region; k--)
			sort_array[k] = sort_array[k-1];
		sort_array[start_region] = h;
		original_sit =  0;
	}

	return(!original_sit);
}



/*  Succbary-Value of a node
 *  ------------------------
 *  succbary(v) = ( Sum pos(successors(v)) ) / outdegree(v) )
 */

#ifdef ANSI_C
static float   succbary(GNODE node)
#else
static float   succbary(node)
GNODE	node;
#endif
{
	int	Sum;
	ADJEDGE	w;

	assert((node));
	debugmessage("succbary","");
	/* Assertion: The NPOS-values are set before by level_to_array */

	if (NOUTDEG(node)==0) return(0.0);
	Sum = 0;
	w = NSUCC(node);
	while (w) {
		Sum += NPOS(TARGET(w));
		w = ANEXT(w);
	}
	return ( ((float) Sum) / ((float) NOUTDEG(node)) );
}

	
/*  Predbary-Value of a node
 *  ------------------------
 *  predbary(v) = ( Sum pos(predecessors(v)) ) / indegree(v) )
 */

#ifdef ANSI_C
static float	predbary(GNODE node)
#else
static float	predbary(node)
GNODE	node;
#endif
{
	int	Sum;
	ADJEDGE	w;

	assert((node));
	debugmessage("predbary","");
	/* Assertion: The NPOS-values are set before by level_to_array */

	if (NINDEG(node)==0) return(0.0);
	Sum = 0;
	w = NPRED(node);
	while (w) {
		Sum += NPOS(SOURCE(w));
		w = ANEXT(w);
	}
	return ( ((float) Sum) / ((float) NINDEG(node)) );
}



/*  Succmedian-Value of a node
 *  --------------------------
 *  succmedian(v) = interpolated median value 
 *
 *  For instance, if a node has successors with positions
 *   		5  7  13  49  57 
 *  then 13 is the real median.
 *  If the outdegree is even, then the median is not unique,
 *  e.g.      5  7  13  49  57  62   
 *  the left median is 13 and the right median is 49. In this
 *  case we interpolate, i.e.
 *  median = ((13-5) * 49 + (62-57) * 13) /  (13-5+62-57) = 35.153
 */

#ifdef ANSI_C
static float   succmedian(GNODE	node)
#else
static float   succmedian(node)
GNODE	node;
#endif
{
	int	i, leftpart, rightpart;
	ADJEDGE	w;

	assert((node));
	/* Assertion: the save_array must be unused at that time */

	debugmessage("succmedian","");
	/* Assertion: The NPOS-values are set before by level_to_array */

	switch (NOUTDEG(node)) {
	case 0: return(0.0);
	case 1: return((float) NPOS(TARGET(NSUCC(node))));
	case 2: 
		w = NSUCC(node);
		i = NPOS(TARGET(w));
		w = ANEXT(w);
		i += NPOS(TARGET(w));
		return ( ((float) i) / 2.0);
	}
 
	i = 0;
	w = NSUCC(node);
	while (w) {
		save_array[i++] = TARGET(w);
		w = ANEXT(w);
	}
	quicksort_save_array(i);

	if (i % 2) return((float)NPOS(save_array[i/2]));

	leftpart  = NPOS(save_array[i/2-1])-NPOS(save_array[0]);
	rightpart = NPOS(save_array[i-1])-NPOS(save_array[i/2]);
	return ( ((float) 
		  (NPOS(save_array[i/2-1]) * rightpart + 
		   NPOS(save_array[i/2]) * leftpart)) 
		  / ((float) (leftpart+rightpart)) );
}

/*  Predmedian-Value of a node
 *  --------------------------
 *  predmedian(v) = interpolated median value 
 *
 *  See Succmedian for explanations.
 */

#ifdef ANSI_C
static float   predmedian(GNODE	node)
#else
static float   predmedian(node)
GNODE	node;
#endif
{
	int	i, leftpart, rightpart;
	ADJEDGE	w;

	assert((node));
	/* Assertion: the save_array must be unused at that time */

	debugmessage("predmedian","");
	/* Assertion: The NPOS-values are set before by level_to_array */

	switch (NINDEG(node)) {
	case 0: return(0.0);
	case 1: return((float) NPOS(SOURCE(NPRED(node))));
	case 2: 
		w = NPRED(node);
		i = NPOS(SOURCE(w));
		w = ANEXT(w);
		i += NPOS(SOURCE(w));
		return ( ((float) i) / 2.0);
	}
 
	i = 0;
	w = NPRED(node);
	while (w) {
		save_array[i++] = SOURCE(w);
		w = ANEXT(w);
	}
	quicksort_save_array(i);

	if (i % 2) return((float)NPOS(save_array[i/2]));

	leftpart  = NPOS(save_array[i/2-1])-NPOS(save_array[0]);
	rightpart = NPOS(save_array[i-1])-NPOS(save_array[i/2]);
	return ( ((float) 
		  (NPOS(save_array[i/2-1]) * rightpart + 
		   NPOS(save_array[i/2]) * leftpart)) 
		  / ((float) (leftpart+rightpart)) );
}



/*  Compare function for sorting according barycenters
 *  --------------------------------------------------
 *  returns 1 if NBARY(*a) > NBARY(*b), 0 if equal, -1 otherwise.
 *  A further problem is, if a node v has no successor. Then
 *  its succbary(v) is 0, but its position does not influence
 *  the number of crossings at successor edges. We make  
 *  these nodes artificially equal to all nodes.
 *  Dito may happen with predecessors and predbary.
 */
 
#ifdef ANSI_C
static int 	compare_bary(const GNODE *a, const GNODE *b)
#else
static int 	compare_bary(a,b)
GNODE	*a;
GNODE   *b;
#endif
{ 
        if ((NBARY(*a)==0.0) || (NBARY(*b)==0.0)) return(0);
	if (NBARY(*a) > NBARY(*b))	          return(1);
	if (NBARY(*a) < NBARY(*b))	          return(-1);
	return(0);
}



/*  Compare function for sorting according positions 
 *  ------------------------------------------------
 *  returns 1 if NPOS(*a) > NPOS(*b), 0 if equal, -1 otherwise.
 */
 
#ifdef ANSI_C
static int 	compare_pos(const GNODE *a, const GNODE *b)
#else
static int 	compare_pos(a,b)
GNODE	*a;
GNODE     *b;
#endif
{ 
	if (NPOS(*a) > NPOS(*b))   return(1);
	if (NPOS(*a) < NPOS(*b))   return(-1);
	return(0);
}


/*--------------------------------------------------------------------*/
/*  Sort the sort_array according the the BARY-values                 */
/*--------------------------------------------------------------------*/


#ifdef OWN_QUICKSORT

/*  Quicksort of sort_array
 *  =======================
 *  This is a randomnized quicksort that can be used if the qsort of
 *  the library is bad. E.g., myqsort( 0, 49) sorts the first 50 elements
 *  of the sort_array.
 *  Randomizing is done by a simple pseudorandom function:
 *     		rand_num = rand_num + 10891;
 *  Note that 10891 is a prime number. 
 */

#define exchange(a,b) { t=a; a=b; b=t; }

static unsigned int rand_num = 211;

#ifdef ANSI_C
static void 	myqsort(int l, int r)
#else
static void 	myqsort(l, r)
int l, r;
#endif
{
	int   i, j, k;
	GNODE v, t;

	/* assert(l<r) */
	k = r-l;
	if (k>5) { /* randomized pivot element */
		j = l + (rand_num % k); 
		assert( l <=j && j<=r );
		rand_num = (rand_num + 10891) % MAXINT;
		exchange(sort_array[l],sort_array[j]); 
	}
        v=sort_array[l]; i=l; j=r+1;
	if (NBARY(v)==0.0) j=l+k/2; 
	else {
		while (i<j) {
       	     		do j--; while (  (NBARY(sort_array[j])!=0.0)
			               &&(NBARY(sort_array[j])>NBARY(v)));
			/* 
			 * j loops maximal until l, because then:
			 * NBARY(sort_array[j])==NBARY(v)
			 */
        	    	do i++; while ((i<=j)&&(NBARY(sort_array[i])!=0.0) 
			       	       &&(NBARY(sort_array[i])<NBARY(v)));
			/* 
			 * i loops maximal until r, because j<=r holds now
			 */
			if (i<j) exchange(sort_array[j],sort_array[i]); 
       	 	};
	}
	exchange(sort_array[l],sort_array[j]); 
        if (l<j-1) myqsort(l,j-1);
        if (j+1<r) myqsort(j+1,r);
}

#endif


/*--------------------------------------------------------------------*/
/*  level and array exchange routines                                 */
/*--------------------------------------------------------------------*/


/*  Put level tmp_layer[i] into the sort_array
 *  ------------------------------------------
 *  and set all NPOS of the nodes of the next or previous layer.
 *  This is done before sorting.
 *  Note the size of the layer is as long as the size of the array.
 */

#ifdef ANSI_C
static void	level_to_array(int i,int dir)
#else
static void	level_to_array(i,dir)
int i;
int dir;
#endif
{
	int j;
	GNLIST hn;

	debugmessage("level_to_array","");
	hn = TSUCC(tmp_layer[i]);
	j = 0;
	while (hn) {
		sort_array[j++] = GNNODE(hn);
		hn = GNNEXT(hn);
	}
	if (dir=='d') {
		assert((i>0));
		hn = TSUCC(tmp_layer[i-1]);
	}
	else /* dir='u' */ {
		assert((i<maxdepth+1));
		hn = TSUCC(tmp_layer[i+1]);
	}
	j  = 1;
	while (hn) {
		NPOS(GNNODE(hn)) = j++;
		hn = GNNEXT(hn);
	}
}


/*  Put the sort_array back into tmp_layer[i]
 *  -----------------------------------------
 *  This is done after sorting.
 *  Note the size of the layer is as long as the size of the array.
 */

#ifdef ANSI_C
static void	array_to_level(int i)
#else
static void	array_to_level(i)
int i;
#endif
{
	int j;
	GNLIST hn;

	debugmessage("array_to_level","");
	j  = 0;
	hn = TSUCC(tmp_layer[i]);
	while (hn) {
		GNNODE(hn) = sort_array[j++];
		hn = GNNEXT(hn);
	}
	assert(j==TANZ(tmp_layer[i]));
}


/*  Put level tmp_layer[i] into the save_array
 *  ------------------------------------------
 *  This is done to store the tmp_layer[i] temporary.
 */

#ifdef ANSI_C
static void	save_level(int i)
#else
static void	save_level(i)
int i;
#endif
{
	int j;
	GNLIST hn;

	debugmessage("save_level","");
	j  = 0;
	hn = TSUCC(tmp_layer[i]);
	while (hn) {
		save_array[j++] = GNNODE(hn);
		hn = GNNEXT(hn);
	}
	assert(j==TANZ(tmp_layer[i]));
}

/*  Put the save_array back into tmp_layer[i]
 *  -----------------------------------------
 *  This is done to restore the tmp_layer[i].
 */

#ifdef ANSI_C
static void	restore_level(int i)
#else
static void	restore_level(i)
int i;
#endif
{
	int j;
	GNLIST hn;

	debugmessage("restore_level","");
	j  = 0;
	hn = TSUCC(tmp_layer[i]);
	while (hn) {
		GNNODE(hn) = save_array[j++];
		hn = GNNEXT(hn);
	}
	assert(j==TANZ(tmp_layer[i]));
}


/*  Reorder the tmp_layer[i] according to the HORDER
 *  ------------------------------------------------
 *  This is done by storing tmp_layer[i] to the sort_array,
 *  sorting it according to NHORDER and storing it back. 
 */


#ifdef ANSI_C
static void apply_horder(int i)
#else
static void apply_horder(i)
int i;
#endif
{
	GNLIST hn;
	int j;

	debugmessage("apply_horder","");

	/* Assertions:
	 * NBARY(nodes) is currently not needed.
	 * The sort_array is free.
	 * The actual layer is tmp_Layer.
	 */

	hn = TSUCC(tmp_layer[i]);
	j = 0;
	while (hn) {
		if (NHORDER(GNNODE(hn)) >=0) { 
			sort_array[j++] = GNNODE(hn);
			NBARY(GNNODE(hn)) = NHORDER(GNNODE(hn));
		}
		hn = GNNEXT(hn);
	}

	quicksort_sort_array(j);

	hn = TSUCC(tmp_layer[i]);
	j = 0;
	while (hn) {
		if (NHORDER(GNNODE(hn)) >=0) 
			GNNODE(hn) = sort_array[j++];
		hn = GNNEXT(hn);
	}
}


/*--------------------------------------------------------------------*/
/*  Copy layer tables                                                 */
/*--------------------------------------------------------------------*/

/*  Copy layer tables
 *  =================
 *  copies table l2 to table l1.
 *  Note: it is not enough to exchange the tables. They must be really
 *  copied because we need l2 twice. The layers of l2 may be longer
 *  but not shorter than the layers of l1.
 *  Further: the TPRED lists are not important here.
 *  They are recreated later.
 */

#ifdef ANSI_C
static void copy_layers(DEPTH *l1,DEPTH *l2)
#else
static void copy_layers(l1,l2)
DEPTH *l1;
DEPTH *l2;
#endif
{
        int     i;
	GNLIST  h1,h2;

	debugmessage("copy_layers","");
	for (i=0; i<=maxdepth+1; i++) { 
		TCROSS(l1[i]) = TCROSS(l2[i]);
		h1 = TSUCC(l1[i]);
		h2 = TSUCC(l2[i]);
		if (TANZ(l1[i])==TANZ(l2[i])) {
			while (h2) {
				assert((h1));	/* both lenght are equal */
				GNNODE(h1) = GNNODE(h2);
				h1 = GNNEXT(h1);
				h2 = GNNEXT(h2);
			}
		}
		else {
			assert ((TANZ(l1[i])<TANZ(l2[i])));
			while (h2) {
				assert((h1));	
				GNNODE(h1) = GNNODE(h2);
				h2 = GNNEXT(h2);
				if (h2 && !GNNEXT(h1)) 
					GNNEXT(h1) = tmpnodelist_alloc();
				h1 = GNNEXT(h1);
			}
			TANZ(l1[i]) = TANZ(l2[i]);
		}
	}
}


/*--------------------------------------------------------------------*/
/*  Insert the connections                                            */
/*--------------------------------------------------------------------*/

/* Connection layout
 * =================
 * In barycentering(), nodes that are directly neighboured are
 * regarded as one nodes. For this reason, we had deleted the
 * nodes reacheable by forward connections from the layers.
 * This was calc_connect_adjlists in step1.c
 * Now, we insert these nodes again. They must be directly
 * neighboured to their reference node. But a reference node
 * may have a left neighbour chain and a right neigbour chain,
 * and we must calculate which chain comes left or right.
 * This is done here. Further, the original adjacency lists
 * of these connections are recalculated, and NINDEG and NOUTDEG
 * of nodes and TANZ of the layers is updated, if connections
 * are found.
 */

/* Abreviations to check whether a connection is a forward connection.
 */

#define forward_connection1(c)  ((CEDGE(c))&& (EEND(CEDGE(c))==CTARGET(c)))
#define forward_connection2(c)  ((CEDGE2(c))&&(EEND(CEDGE2(c))==CTARGET2(c)))

#ifdef ANSI_C
static void insert_connects_in_layer(void)
#else
static void insert_connects_in_layer()
#endif
{
	int i,j;
	GNLIST 	hl,hln;
	CONNECT c;
	int 	forward_conn;
	int 	changed;

	debugmessage("insert_connects_in_layer","");
	for (i=0; i<=maxdepth+1; i++) { 
		hl  = TSUCC(tmp_layer[i]);
		changed = 0;
		while (hl) {
			hln  = GNNEXT(hl);
			c = NCONNECT(GNNODE(hl));
			forward_conn = 0;
			if (c) {
				if (forward_connection1(c))
                                        forward_conn = 1;
                                if (forward_connection2(c))
                                        forward_conn = 1;
			}
			if (forward_conn&&(NMARK(GNNODE(hl))==0)) {
				changed = 1;
				check_connect(i,GNNODE(hl)); 
			}
			hl  = hln;
		}
		if (changed) {
			if (i<=maxdepth) 
				TCROSS(tmp_layer[i]) = layer_crossing(i);
        		for (j=i; j<=maxdepth; j++) 
					(void)resort_down_layer(j);
		}
	}
}

/* Check one connection node
 * -------------------------
 * The connection node at level is analyzed.
 * Its lists of direct neigbours are created first as left neighbour
 * and right neighbour, then as right neighbour and left neighbour.
 * The number of crossings decide which layout we take. 
 * As side effect, the adjacency lists of all nodes that participate 
 * on the connection are restored and the degree numbers are recalculated.
 * TANZ is updated.
 */

static GNLIST leftlist;		/* the list of nodes that are left  */
static GNLIST leftlistend;      /* neighbours of node               */
static GNLIST rightlist;	/* the list of nodes that are right */
static GNLIST rightlistend;	/* neighbours of node               */

/* Because we insert leftlist and rightlist for test reason, we
 * must be able to restore the original layer[level]-list. Thus, we
 * store the points that we have changes.
 */
 
static GNLIST *insertpoint;	/* *insertpoint becomes leftlist           */
static GNLIST middlepoint;      /* GNNEXT(middlepoint) becomes rightlist   */
static GNLIST endpoint;		/* GNNEXT(rightlistend) will point to this */

#ifdef ANSI_C
static void check_connect(int level,GNODE node)
#else
static void check_connect(level,node)
int    level;
GNODE  node;
#endif
{
	CONNECT c;
	int 	j,clr, crl;
	ADJEDGE a;

	debugmessage("check_connect","");

	c = NCONNECT(node);
	NSUCC(node) = NSVSUCC(node);
	NPRED(node) = NSVPRED(node);

	/* recalculate INDEGREE and OUTDEGREE */
	j = 0;
	a = NSUCC(node);
	while (a) { j++; a = ANEXT(a); }
	NOUTDEG(node) = j;
	j = 0;
	a = NPRED(node);
	while (a) { j++; a = ANEXT(a); }
	NINDEG(node) = j;

	/* First we try to insert left first:
 	 *   -> Cl -> Cl -> A -> Cr -> Cr -> ...
	 * and calculate the number of crossings, that now exist.
	 */ 
	leftlist = leftlistend = rightlist = rightlistend = NULL;
	if (forward_connection1(c)) left_conn_list(CTARGET(c),node);
	if (forward_connection2(c)) right_conn_list(CTARGET2(c),node);

	insert_left_right(level,node);

	/* Calculate the actual number of crossings */
 	clr = 0;
	if (level>0) 	     clr+=layer_crossing(level-1);
	if (level<=maxdepth) clr+=layer_crossing(level);

	/* Now we do the reverse inserting, 
 	 *   -> Cr -> Cr -> A -> Cl -> Cl -> ...
	 * and calculate the number of crossings, that now exist.
	 */ 

	*insertpoint = middlepoint;
	GNNEXT(middlepoint) = endpoint;

	leftlist = leftlistend = rightlist = rightlistend = NULL;
	if (forward_connection1(c)) right_conn_list(CTARGET(c),node);
	if (forward_connection2(c)) left_conn_list(CTARGET2(c),node);

	insert_left_right(level,node);


	/* Calculate the actual number of crossings */
 	crl = 0;
	if (level>0) 	     crl+=layer_crossing(level-1);
	if (level<=maxdepth) crl+=layer_crossing(level);

	/* If the the second try is better, we finish. If the first
	 * try was better, we recreate the first situation.
	 */

	if (crl<=clr) return;

	*insertpoint = middlepoint;
	GNNEXT(middlepoint) = endpoint;

	/* This was better:
 	 *   -> Cl -> Cl -> A -> Cr -> Cr -> ...
	 */ 
	leftlist = leftlistend = rightlist = rightlistend = NULL;
	if (forward_connection1(c)) left_conn_list(CTARGET(c),node);
	if (forward_connection2(c)) right_conn_list(CTARGET2(c),node);

	insert_left_right(level,node);

}


/* Insert left_list and right_list 
 * -------------------------------
 * The insertion point is the cons-cell at TSUCC(tmp_layer[level]) 
 * that just points to node. After insertion, TANZ(tmp_layer[level]) 
 * is recalculated.
 */

#ifdef ANSI_C
static void insert_left_right(int level,GNODE node)
#else
static void insert_left_right(level,node)
int level;
GNODE node;
#endif
{
	GNLIST hl,*hlp;
	int j;

	debugmessage("insert_left_right","");

	/* search insertion point */
	hlp = &TSUCC(tmp_layer[level]);
	hl  =  TSUCC(tmp_layer[level]);
	while (hl) {
		if (GNNODE(hl)==node) break;
		hlp = &GNNEXT(hl);
		hl  =  GNNEXT(hl);
	}
	assert((hl)); 	/* node should be in the list */

	/* Insert leftlist and rightlist around the connection */

	insertpoint = hlp;
	middlepoint = hl;
	endpoint    = NNEXT(hl);

	if (leftlist) { 
		*hlp = leftlist;
		GNNEXT(leftlistend) = hl;
	}
	hlp = &GNNEXT(hl);
	hl  =  GNNEXT(hl);
	if (rightlist) { 
		*hlp = rightlist;
		GNNEXT(rightlistend) = hl;
	}

	/* and recalculate TANZ */

	j = 0;
        hl  =  TSUCC(tmp_layer[level]);
	while (hl) {
		j++;
		hl = GNNEXT(hl);
	}
	TANZ(tmp_layer[level]) = j;
}


/* Calculate left_list
 * -------------------
 * Traverse the connections and add them to left_list.
 * Further, restore the edges that were in the adjacency lists
 * of the connection nodes.
 */

#ifdef ANSI_C
static void left_conn_list(GNODE v,GNODE w)
#else
static void left_conn_list(v,w)
GNODE v;
GNODE w;
#endif
{
	GNLIST h;
	ADJEDGE e;
	CONNECT c;

	debugmessage("left_conn_list","");

	/* restore the adjacency lists of the connection 
	 * Remind: The adjacency lists were destroyed in step1 in
    	 * calc_connect_adjlists.
	 */
	e = NSUCC(v);
	while (e) { SOURCE(e) = v; e = ANEXT(e); }
	e = NPRED(v);
	while (e) { TARGET(e) = v; e = ANEXT(e); }

	h = tmpnodelist_alloc();
	GNNODE(h) = v;
	GNNEXT(h) = leftlist;
	leftlist = h;
	if (!leftlistend) leftlistend = h;	

	c = NCONNECT(v);
	if (!c) return;

        if (CTARGET(c) && (CTARGET(c)!=w))   left_conn_list(CTARGET(c),v);
        if (CTARGET2(c) && (CTARGET2(c)!=w)) left_conn_list(CTARGET2(c),v);
}

/* Calculate right_list
 * -------------------
 * Traverse the connections and add them to left_list.
 * Further, restore the edges that were in the adjacency lists
 * of the connection nodes.
 */

#ifdef ANSI_C
static void right_conn_list(GNODE v,GNODE w)
#else
static void right_conn_list(v,w)
GNODE v;
GNODE w;
#endif
{
	GNLIST h;
	ADJEDGE e;
	CONNECT c;

	debugmessage("right_conn_list","");

	/* restore the adjacency lists of the connection 
	 * Remind: The adjacency lists were destroyed in step1 in
    	 * calc_connect_adjlists.
	 */
	e = NSUCC(v);
	while (e) { SOURCE(e) = v; e = ANEXT(e); }
	e = NPRED(v);
	while (e) { TARGET(e) = v; e = ANEXT(e); }

	h = tmpnodelist_alloc();
	GNNODE(h) = v;
	if (rightlistend) GNNEXT(rightlistend) = h;
	GNNEXT(h) = NULL;
	rightlistend = h;
	if (!rightlist) rightlist = h;	

	c = NCONNECT(v);
	if (!c) return;

        if (CTARGET(c) && (CTARGET(c)!=w))   right_conn_list(CTARGET(c),v);
        if (CTARGET2(c) && (CTARGET2(c)!=w)) right_conn_list(CTARGET2(c),v);
}


/*--------------------------------------------------------------------*/
/*  Recreate the TPRED fields of layer                                */
/*--------------------------------------------------------------------*/

/* Note: we cannot reuse the existing list of TPRED, because it
 * may be too short, since we have inserted connections. To recreate 
 * is faster than to analyse how to reuse existing nodelist cells.
 * Further we sort the adjacencies here, and fill NSUCCL, NSUCCR,
 * NPREDL, NPREDR.
 */

#ifdef ANSI_C
static void 	recreate_predlists(void)
#else
static void 	recreate_predlists()
#endif
{
	GNLIST h1,h2;
	int i,j,k;
	
	debugmessage("recreate_predlists","");

	/* First, give all nodes their position numbers */
	for (i=0; i<=maxdepth+1; i++) { 
		h1 = TSUCC(layer[i]);
		j = 0;
		while (h1) {
			NPOS(GNNODE(h1)) = j++;
			h1 = GNNEXT(h1);
		}
	}
	for (i=0; i<=maxdepth+1; i++) { 
		h1 = TSUCC(layer[i]);
		TPRED(layer[i]) = NULL;
		k = 0;
		while (h1) {
			k++;
			h2 = tmpnodelist_alloc();
			GNNEXT(h2) = TPRED(layer[i]);
			TPRED(layer[i]) = h2;
			GNNODE(h2) = GNNODE(h1);
			sort_adjedges(GNNODE(h1)); 
			h1 = GNNEXT(h1);
		}
		assert((TANZ(layer[i]) == k));
	}
}

/*--------------------------------------------------------------------*/
/*  Sort the adjacency edges of a node                                */
/*--------------------------------------------------------------------*/

/*  Sort adjacencies according NPOS
 *  -------------------------------
 *  To allow to access to the predecessor or successor nodes 
 *  of a node v in the order of their NPOS-values, we traverse
 *  the corresponding adjacency list of v and put the target nodes
 *  sorted into the array adjarray2.
 *  This gives the fine layout the last touch, even if the effect 
 *  is very small.
 */

#ifdef ANSI_C
static void sort_adjedges(GNODE	v)
#else
static void sort_adjedges(v)
GNODE 	v;
#endif
{
	int i;
	ADJEDGE a;

	debugmessage("sort_adjedges","");
	assert((v));
	i = 0;
	a = NPRED(v);
	while (a) {
		adjarray2[i++] = AKANTE(a);
		a = ANEXT(a);
	}

	qsort(adjarray2,NINDEG(v),sizeof(GNODE),
		(int (*) (const void *, const void *))compare_srcpos);

	i = 0;
	a = NPRED(v);
	while (a) {
		AKANTE(a) = adjarray2[i++];
		a = ANEXT(a);
	}
	NPREDL(v) = NPREDR(v) = 0;
	if (i) { /* at least one predecessor */
		NPREDL(v) = adjarray2[0];
		NPREDR(v) = adjarray2[i-1];
	}
	i = 0;
	a = NSUCC(v);
	while (a) {
		adjarray2[i++] = AKANTE(a);
		a = ANEXT(a);
	}

	qsort(adjarray2,NOUTDEG(v),sizeof(GNODE),
		(int (*) (const void *, const void *))compare_tarpos);

	i = 0;
	a = NSUCC(v);
	while (a) {
		AKANTE(a) = adjarray2[i++];
		a = ANEXT(a);
	}
	NSUCCL(v) = NSUCCR(v) = 0;
	if (i) { /* at least one successor */
		NSUCCL(v) = adjarray2[0];
		NSUCCR(v) = adjarray2[i-1];
	}
}


/*  Compare functions for sort_adjedges
 *  ------------------------------------
 *  returns 1 if NPOS(*a) > NPOS(*b), 0 if equal, -1 otherwise
 */
 
#ifdef ANSI_C
static int compare_srcpos(const GEDGE *a, const GEDGE *b)
#else
static int compare_srcpos(a,b)
GEDGE	*a;
GEDGE     *b;
#endif
{ 
	if (NPOS(ESTART(*a)) > NPOS(ESTART(*b)))	return(1);
	if (NPOS(ESTART(*a)) < NPOS(ESTART(*b)))	return(-1);
	return(0);
}

#ifdef ANSI_C
static int compare_tarpos(const GEDGE *a, const GEDGE *b)
#else
static int compare_tarpos(a,b)
GEDGE	*a;
GEDGE     *b;
#endif
{ 
	if (NPOS(EEND(*a)) > NPOS(EEND(*b)))	return(1);
	if (NPOS(EEND(*a)) < NPOS(EEND(*b)))	return(-1);
	return(0);
}



/*--------------------------------------------------------------------*/
/*  Debugging routines                                                */
/*--------------------------------------------------------------------*/

/*  The following routines are normally not used, but are provided
 *  to simplify debugging later on.
 */

/* Output all layers textually
 * ---------------------------
 */

#ifdef DEBUG

#ifdef ANSI_C
void    db_output_all_layers(void)
#else
void    db_output_all_layers()
#endif
{
	int i;
	for (i=0; i<=maxdepth+1; i++) db_output_layer(i); 
	PRINTF("\n");
}

#endif


/* Output one layer textually
 * --------------------------
 */

#ifdef DEBUG

#ifdef ANSI_C
void    db_output_layer(int i)
#else
void    db_output_layer(i)
int     i;
#endif
{
        GNLIST	li;
	ADJEDGE li2;

        PRINTF("layer[%d]: ", i);
        li = TSUCC(tmp_layer[i]);
        while (li) {
		if (NTITLE(GNNODE(li))[0])
                	PRINTF("%s[", NTITLE(GNNODE(li)));
		else 	PRINTF("?[");
		li2 = NPRED(GNNODE(li));
		while (li2) {
               		if (NTITLE(SOURCE(li2))[0])
				PRINTF("%s,", NTITLE(SOURCE(li2)));
			else 	 PRINTF("?,");
			li2 = ANEXT(li2);
		}
		PRINTF("][");
		li2 = NSUCC(GNNODE(li));
		while (li2) {
               		if (NTITLE(TARGET(li2))[0])
               			PRINTF("%s,", NTITLE(TARGET(li2)));
			else 	 PRINTF("?,");
			li2 = ANEXT(li2);
		}
		PRINTF("]");
		PRINTF("b(%f)p(%d) ",NBARY(GNNODE(li)),NPOS(GNNODE(li)));
                li = GNNEXT(li);
        }
	PRINTF("\n");
}

#endif


/* Check whether the adjacencies of a node v are proper
 * ----------------------------------------------------
 * level is the level v has to be;
 */

#ifdef DEBUG

#ifdef ANSI_C
int db_check_proper(GNODE v,int level)
#else
int db_check_proper(v,level)
GNODE v;
int level;
#endif
{
	int t;
	char *title,*st,*tt;
	ADJEDGE li;

	if (NTITLE(v)[0])
                title = NTITLE(v);
	else 	title = "?";
	t = NTIEFE(v);
	if (level!=t)
		PRINTF("%s at level %d, expected %d\n",title,t,level);
	li = NSUCC(v);
	while (li) {
		if (!AKANTE(li)) { PRINTF("%s missing edge\n",title); break; }
		if (!SOURCE(li)) { 	
			PRINTF("Succedge at %s missing source\n",title); 
			break; 
		}
		if (!TARGET(li)) { 	
			PRINTF("Succedge at %s missing source\n",title); 
			break; 
		}
		if (NTITLE(SOURCE(li))[0])
               		st = NTITLE(SOURCE(li));
		else 	st = "?";
		if (NTITLE(TARGET(li))[0])
               		tt = NTITLE(TARGET(li));
		else 	tt = "?";
		if (SOURCE(li)!=v) { 	
			PRINTF("Succedge (%s,%s) at %s wrong source\n",
				st,tt,title); 
			break; 
		}
		if (NTIEFE(TARGET(li))!=t+1) { 	
			PRINTF("Succedge (%s,%s) depth %d (%d expected)\n",
				st,tt,NTIEFE(TARGET(li)),t+1); 
			break; 
		}
		li = ANEXT(li);
	}
	li = NPRED(v);
	while (li) {
		if (!AKANTE(li)) { PRINTF("%s missing edge\n",title); break; }
		if (!SOURCE(li)) { 	
			PRINTF("Prededge at %s missing source\n",title); 
			break; 
		}
		if (!TARGET(li)) { 	
			PRINTF("Prededge at %s missing source\n",title); 
			break; 
		}
		if (NTITLE(SOURCE(li))[0])
               		st = NTITLE(SOURCE(li));
		else 	st = "?";
		if (NTITLE(TARGET(li))[0])
               		tt = NTITLE(TARGET(li));
		else 	tt = "?";
		if (TARGET(li)!=v) { 	
			PRINTF("Prededge (%s,%s) at %s wrong target\n",
				st,tt,title); 
			break; 
		}
		if (NTIEFE(SOURCE(li))!=t-1) { 	
			PRINTF("Succedge (%s,%s) depth %d (%d expected)\n",
				st,tt,NTIEFE(SOURCE(li)),t-1); 
			break; 
		}
		li = ANEXT(li);
	}
	return(1);
}

#endif

