/*--------------------------------------------------------------------*/
/*              VCG : Visualization of Compiler Graphs                */
/*--------------------------------------------------------------------*/
/*
 * Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                             the Compare Consortium
 * Copyright (C) 2015 Nikita S <nnlight@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
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
 *         a) it is used in the adjacency lists.
 *      or b) it is a direct neigbour edge in NCONNECT(v) for
 *            some node v.
 *    4)  maxindeg and maxoutdeg are the maximal indegree (number of
 *        incoming edges) and maximal outdegree (number of outgoing
 *        edges) occuring at visible nodes. Forward connections are
 *        not counted.
 *    5)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    6)  NTIEFE(node) is filled for all nodes. NINDEG and NOUTDEG are
 *        filled. Forward connections are not counted. But  NINDEG and
 *    NOUTDEG are also filled at nodes that have forward connections.
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
 *    in the TSUCC and TPRED lists, too.
 *    TANZ(layer[i]) and NINDEG(node) and NOUTDEG(node) are adapted
 *    to this situation. The connection edge still do not count.
 *    3)  The hierarchy in layer is proper.
 *    4)  nodelist, labellist and dummylist are not changed.
 *    5)  All pot. visible edges are in the lists edgelist or tmpedgelist,
 *    same as before.
 *    6)  maxindeg and maxoutdeg are not changed. By inserting nodes
 *    reacheable by connections, the maximal indegree and outdegree
 *    may shrink, thus maxindeg and maxoutdeg are upper estimations.
 *    7)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    8)  NTIEFE(node) is filled for all nodes. NINDEG and NOUTDEG are
 *        filled. Forward connections are not counted.
 *        NCONNECT(node) is filled as before.
 *    See point 2 !!!
 *    9)  Reverted edges are marked with EART(e)='R'.
 *        Self loops don't anymore exist.
 *    10) NPOS(v) gives the horizontal position of a node inside the
 *    layer. Adjacency edges are sorted according to these NPOS
 *    values. The ordering inside a layer is such that the number
 *    of cossings is small (but may be not optimal).
 *    11) NSUCCL(v) and NSUCCR(v) are the leftest and rightest successor
 *        edge of v, and NPREDL(v) and NPREDR(v) the leftest and rightest
 *        predecessor edge.
 *
 *
 * This file provides the following functions:
 * ------------------------------------------
 * step2_main       Main routine to reduce the number of crossings
 *
 * If DEBUG is switched on, we have further:
 * db_output_all_layers  prints all layers in tmp_layer.
 * db_output_tmp_layer   prints one tmp_layer[i]
 * db_check_proper   checks whether a nodes is proper in the
 *           hierarchy. Prints diagnostics.
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
#include "timing.h"
#include "graph.h"


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

static void     create_tmp_layer      _PP((void));
static void     tree_horizontal_order     _PP((void));
static void     unmerge_connected_parts   _PP((void));
static void prepare_horizontal_order  _PP((void));
static void     mark_all_nodes        _PP((GNODE node, int i));

static int  graph_crossings       _PP((void));
static void calc_all_layers_crossings _PP((void));
static int  layer_crossing        _PP((int level));
static void     finish_upper          _PP((GNODE v));
static void     finish_lower          _PP((GNODE v));
static void append_to_upper       _PP((GNODE n));
static void append_to_lower       _PP((GNODE n));
static void     delete_upper          _PP((DLLIST x));
static void     delete_lower          _PP((DLLIST x));

static void local_crossing_optimization _PP((int nearedges));
static void     prepare_local_optimization  _PP((int level));
static int      level_crossing_optimization _PP((int level,int nearedges));
static int  exchange_nodes_necessary    _PP((GNODE C,GNODE D));

static void     prepare_positions       _PP((void));
static int  is_complex          _PP((GNODE v));
static int  check_exchange          _PP((GNODE v1,GNODE v2,int dir));
static void do_exchange         _PP((GNODE v1,GNODE v2,int dir));
static void     local_unwind_crossings      _PP((void));

static void     init_barycentering    _PP((void));
static void     barycentering         _PP((void));
static void Phase1_down           _PP((void));
static void Phase1_up         _PP((void));
static void Phase2_down       _PP((void));
static void     Phase2_up         _PP((void));
static int  resort_down_layer     _PP((int i));
static int  resort_up_layer       _PP((int i));
static void resort_down_up_layer      _PP((int level));
static void resort_up_down_layer      _PP((int level));
static int  cycle_sort_array      _PP((int siz));
static float    succbary          _PP((GNODE node));
static float    predbary          _PP((GNODE node));
static float    succmedian        _PP((GNODE node));
static float    predmedian        _PP((GNODE node));
static void level_to_array        _PP((int i,int dir));
static void array_to_level        _PP((int i));
static void     save_level                _PP((int i));
static void     restore_level             _PP((int i));
static void     apply_horder          _PP((int i));

static  int     compare_bary          _PP((const GNODE *a, const GNODE *b));
static  int     compare_pos           _PP((const GNODE *a, const GNODE *b));

static void     copy_layers       _PP((DEPTH *l1, DEPTH *l2));

static void     insert_connects_in_layer  _PP((void));
static void     insert_left_right     _PP((int level, GNODE node));
static void     check_connect         _PP((int level, GNODE node));
static void     left_conn_list        _PP((GNODE v,GNODE w));
static void     right_conn_list       _PP((GNODE v,GNODE w));

static void     recreate_predlists    _PP((void));
static void     sort_adjedges         _PP((GNODE v));
static int  compare_srcpos        _PP((const GEDGE *a, const GEDGE *b));
static int  compare_tarpos        _PP((const GEDGE *a, const GEDGE *b));
#ifdef OWN_QUICKSORT
static void     myqsort           _PP((int l,int r));
#endif


/* Global variables
 * ----------------
 */

static DEPTH    *tmp_layer=NULL;    /* Temporary copy of layer table   */
static int size_of_tlayer = 0;      /* Size of temp.table of layers    */

/* maximal number of nodes in one layer */

int max_nodes_per_layer;

/* number of crossings of the graph layouted as in layer.
 */

int     nr_crossings;

/* array where we can temporary sort the nodes of one level.
 * Used for the barycenter method: All nodes of one level
 * should be sorted according to the bary-values.
 */

static GNODE    *sort_array = NULL;
static int  size_of_sortarray = 0;

/* array where we can temporary save one layer. It has always the
 * same size as the sort_array.
 */

static GNODE    *save_array = NULL;

/* actual number of the iteration during the layout process.
 * We break after`max_baryiterations'.
 */

static int nr_bary_iterations;

/*--------------------------------------------------------------------*/
/*  Reducing the number of crossings                                  */
/*--------------------------------------------------------------------*/

void step2_main(void)
{
    int len;
    int i;
    int old_nr_crossings;

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
        if (sort_array) free(sort_array);
        if (save_array) free(save_array);
        len = max_nodes_per_layer+2;
        sort_array = (GNODE *)libc_malloc(len * sizeof(GNODE));
        save_array = (GNODE *)libc_malloc(len * sizeof(GNODE));
        size_of_sortarray = max_nodes_per_layer+2;
#ifdef DEBUG
        PRINTF("Sizeof tables `sort_array',`save_array': %d Bytes\n",
                (max_nodes_per_layer+2)*sizeof(GNODE));
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

    copy_layers(layer, tmp_layer);

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
static void create_tmp_layer(void)
{
    int    i, j;
    GNLIST h1, h2;


    /* Allocate the array tmp_layer */

    if (maxdepth+2 > size_of_tlayer) {
        if (tmp_layer) free(tmp_layer);
        tmp_layer = (DEPTH *)libc_malloc((maxdepth+2)*sizeof(struct depth_entry));
        size_of_tlayer = maxdepth+2;
#ifdef DEBUG
        PRINTF("Sizeof table `tmp_layer': %d Bytes\n",
                (maxdepth+2)*sizeof(struct depth_entry));
#endif
    }

    /* Copy layer into tmp_layer */

    max_nodes_per_layer = 0;
    for (i=0; i<=maxdepth+1; i++) {
        TCROSS(tmp_layer[i])    = 0;
        TPRED(tmp_layer[i]) = NULL;
        TSUCC(tmp_layer[i])     = NULL;

        /* TSUCC contains more nodes as TPRED. Thus, to calculate
         * max_nodes_per_layer, we use TSUCC.
         */
        j = 0;
        for (h1 = TSUCC(layer[i]); h1; h1 = GNNEXT(h1))
        {
            j++;
        }
        if (j > max_nodes_per_layer) max_nodes_per_layer = j;

        /* Copying with this method reverts the list,
         * thus we copy revert.TPRED(layer[i]) -> TSUCC(tmp_layer[i])
         * TPRED(tmp_layer) is not needed for barycentering.
         */
        j = 0;
        for (h1 = TPRED(layer[i]); h1; h1 = GNNEXT(h1))
        {
            j++;
            h2 = tmpnodelist_alloc();
            GNNEXT(h2) = TSUCC(tmp_layer[i]);
            TSUCC(tmp_layer[i]) = h2;
            GNNODE(h2) = GNNODE(h1);
        }

        TANZ(tmp_layer[i])      = TANZ(layer[i]) = j;
        TRESNEEDED(tmp_layer[i])= TRESNEEDED(layer[i]);

        /* for security only */
        assert(j <= max_nodes_per_layer);

        /*
         * Now we adapt the TSUCC-fields of layer in the same way.
         */
        TSUCC(layer[i]) = NULL;
        for (h1 = TPRED(layer[i]); h1; h1 = GNNEXT(h1))
        {
            h2 = tmpnodelist_alloc();
            GNNEXT(h2) = TSUCC(layer[i]);
            TSUCC(layer[i]) = h2;
            GNNODE(h2) = GNNODE(h1);
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
    GEDGE  e;
    double maxbary;

    debugmessage("tree_horizontal_order","");

    /* Not necessary, but for security reason: unmark all nodes
     * and edges.
     */

    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
        {
            assert(NMARK(GNNODE(li)) == 0);
            for (e = FirstSucc(GNNODE(li)); e; e = NextSucc(e))
            {
                EWEIGHTS(e) = 0;
            }
        }
    }

    /* Mark the spanning tree edges by EWEIGHTS = 1 */

    for (i=0; i<=maxdepth+1; i++) {
        for (prio=max_eprio; prio >= 0; prio--) {
            for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
            {
                for (e = FirstSucc(GNNODE(li)); e; e = NextSucc(e))
                {
                    if (EPRIO(e)==prio) {
                        node = ETARGET(e);
                        if (!NMARK(node)) {
                            NMARK(node) = 1;
                            EWEIGHTS(e) = 1;
                        }
                    }
                }
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
        for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
        {
            node = GNNODE(li);
            NBARY(node) = predbary(node);
            if (NBARY(node) > maxbary) maxbary = NBARY(node);
        }
        maxbary = maxbary + 1.0;

        for (li = TSUCC(tmp_layer[i-1]); li; li = GNNEXT(li))
        {
            for (e = FirstSucc(GNNODE(li)); e; e = NextSucc(e))
            {
                if (EWEIGHTS(e)) {
                    node = ETARGET(e);
                    NBARY(node) = (double)(NPOS(GNNODE(li)))
                        + NBARY(node)/maxbary;
                }
            }
        }

        quicksort_sort_array(TANZ(layer[i]));

        /* Copy sort_array -> tmp_layer */
        array_to_level(i);
    }

    /* Unmark all nodes again */

    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
        {
            NMARK(GNNODE(li)) = 0;
        }
    }

    /* layer = tmp_layer */
    copy_layers(layer, tmp_layer);

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
        reorder_necessary = 0;
        for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
        {
            node = GNNODE(li);
            if (NHORDER(node) >= 0) reorder_necessary = 1;
            if (NHORDER(node)>max_horder_num)
                max_horder_num = NHORDER(node);
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
    int    i, j;
    int    part_is_missing;
    GNODE  node;
    int act_graph;

    debugmessage("unmerge_connected_parts","");

    /* first, set the NBARY field to 0 */
    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
        {
            NBARY(GNNODE(li)) = 0;
        }
    }

    act_graph = 1;

    part_is_missing = 1;
    while (part_is_missing) {

        gs_wait_message('u');
        part_is_missing =0;
        node = NULL;
        act_graph++;

        /* look for an untouched connected component
         */
        for (i=0; i<=maxdepth+1; i++) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                if (NBARY(GNNODE(li)) == 0) {
                    node = GNNODE(li);
                    break;
                }
            }
            if (node) break;
        }
        if (node) {
            assert(NBARY(node)==0);
            part_is_missing = 1;
            mark_all_nodes(node, act_graph);
        }
    }


    for (i=0; i<=maxdepth+1; i++) {
        /* Copy tmp_layer -> sort_array */
        j = 0;
        for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
        {
            sort_array[j++] = GNNODE(li);
        }
        quicksort_sort_array(TANZ(layer[i]));
        /* Copy sort_array -> tmp_layer */
        array_to_level(i);
    }

    /* layer = tmp_layer */
    copy_layers(layer, tmp_layer);
} /* unmerge_connected_parts */




/* Mark all nodes with its graph number
 * ------------------------------------
 */
static void mark_all_nodes(GNODE node, int i)
{
    GEDGE e;

    debugmessage("mark_all_nodes","");

    if (NBARY(node)) return;
    if (NHORDER(node)>=0)
        NBARY(node) = (float)i*(float)(max_horder_num+1)
                    +(float)NHORDER(node);
    else    NBARY(node) = (float)i*(float)(max_horder_num+1);

    for (e = FirstSucc(node); e; e = NextSucc(e))
    {
        mark_all_nodes(ETARGET(e), i);
    }
    for (e = FirstPred(node); e; e = NextPred(e))
    {
        mark_all_nodes(ESOURCE(e), i);
    }
} /* mark_all_nodes */



/*--------------------------------------------------------------------*/
/*  Calculate the number of crossings                                 */
/*--------------------------------------------------------------------*/

/*  Number of crossings in the whole graph in tmp_layer
 *  ===================================================
 *  The precondition is that the TCROSS-entries are already filled.
 */
static int graph_crossings(void)
{
    int i;
    int sumC;

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
static void calc_all_layers_crossings(void)
{
    int i;

    debugmessage("calc_all_layers_crossings","");
    assert(tmp_layer);
    for (i=0; i<=maxdepth; i++)
        TCROSS(tmp_layer[i]) = layer_crossing(i);
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

static  int     size_lower_list;    /* nr. of elements in lower_list */
static  int     size_upper_list;    /* nr. of elements in upper_list */
static  DLLIST  lower_list     = NULL;  /* the lower list                */
static  DLLIST  lower_list_end = NULL;  /* and its end                   */
static  DLLIST  upper_list     = NULL;  /* the upper list                */
static  DLLIST  upper_list_end = NULL;  /* and its end                   */
static  int nr_tcrossings;      /* actual number of crossings    */


static int layer_crossing(int level)
{
    GNLIST vl1, vl2;
    GEDGE e;
    int i;
    ADJEDGE a;

    debugmessage("layer_crossings","");
    assert((tmp_layer));
    assert((lower_list==NULL));
    assert((upper_list==NULL));
    assert((level>=0));
    assert((level<=maxdepth));  /* we access to level+1
                                 * and tmp_layer[maxdepth+1] exists
                                 */

#if 0
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
#else
/*
 * Данные циклы горячие и из-за них VCG_USE_MACROS и NDEBUG (assert)
 * имеют сильное влияние (11 сек -> 7.4 сек).
 * Следующий трюк тоже дает привар (7.4->6.7), хотя, возможно, когда
 * инициализации DEAD_PTR вырубить, он перестанет быть заметным.
 */
#define LC_HACK_FOR_PERF 0
#if LC_HACK_FOR_PERF
    for (vl2 = TSUCC(tmp_layer[level+1]); vl2; vl2 = GNNEXT(vl2))
    {
        LastPred(GNNODE(vl2)) = NULL;
    }
#endif
    i = 1;
    for (vl1 = TSUCC(tmp_layer[level]); vl1; vl1 = GNNEXT(vl1))
    {
        NVPTR(GNNODE(vl1)) = NULL;
        NPOS(GNNODE(vl1))  = i;
        for (e = FirstSucc(GNNODE(vl1)); e; e = NextSucc(e))
        {
#if !LC_HACK_FOR_PERF
            relink_node_edge_as_last(ETARGET(e), e, GD_PRED);
#else
            GNODE n = ETARGET(e);
            if (LastPred(n)) {
                EADJNEXT(LastPred(n),GD_PRED) = e;
                EADJPREV(e,GD_PRED) = LastPred(n);
                EADJNEXT(e,GD_PRED) = NULL;
                NADJLAST(n,GD_PRED) = e;
            } else {
                EADJPREV(e,GD_PRED) = NULL;
                EADJNEXT(e,GD_PRED) = NULL;
                NADJFIRST(n,GD_PRED) = e;
                NADJLAST(n,GD_PRED) = e;
            }
#endif
        }
        i = i+2;
    }

#if LC_HACK_FOR_PERF
    for (vl1 = TSUCC(tmp_layer[level]); vl1; vl1 = GNNEXT(vl1))
    {
        LastSucc(GNNODE(vl1)) = NULL;
    }
#endif
    i = 2;
    for (vl2 = TSUCC(tmp_layer[level+1]); vl2; vl2 = GNNEXT(vl2))
    {
        NVPTR(GNNODE(vl2)) = NULL;
        NPOS(GNNODE(vl2))  = i;
        for (e = FirstPred(GNNODE(vl2)); e; e = NextPred(e))
        {
#if !LC_HACK_FOR_PERF
            relink_node_edge_as_last(ESOURCE(e), e, GD_SUCC);
#else
            GNODE n = ESOURCE(e);
            if (LastSucc(n)) {
                EADJNEXT(LastSucc(n),GD_SUCC) = e;
                EADJPREV(e,GD_SUCC) = LastSucc(n);
                EADJNEXT(e,GD_SUCC) = NULL;
                NADJLAST(n,GD_SUCC) = e;
            } else {
                EADJPREV(e,GD_SUCC) = NULL;
                EADJNEXT(e,GD_SUCC) = NULL;
                NADJFIRST(n,GD_SUCC) = e;
                NADJLAST(n,GD_SUCC) = e;
            }
#endif
        }
        i = i+2;
    }
#endif

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
static void finish_upper(GNODE v)
{
    GEDGE e;
    DLLIST n, m;
    int k1;     /* number occurences of v in upper_list */
    int k2;     /* number of nodes!=v in upper_list that*/
                /* precede an occurence of v            */
    int k3;     /* number of edges that start before v  */
                /* to nonfinished nodes.        */
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
    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        if (NPOS(ETARGET(e)) > NPOS(v)) {
            /* nonfinished: touch it */
            append_to_lower(ETARGET(e));
            NVPTR(ETARGET(e)) = lower_list_end;
        }
    }
} /* finish_upper */


/* Finish a node at the lower level
 * --------------------------------
 * and touch all nonfinished nodes.
 * This is symmetrical to finish_upper.
 */
static void finish_lower(GNODE v)
{
    GEDGE e;
    DLLIST n,m;
    int k1;     /* number occurences of v in lower_list */
    int k2;     /* number of nodes!=v in lower_list that*/
                /* precede an occurence of v            */
    int k3;     /* number of edges that start before v  */
                /* to nonfinished nodes.        */
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
    for (e = FirstPred(v); e; e = NextPred(e))
    {
        if (NPOS(ESOURCE(e)) > NPOS(v)) {
            /* nonfinished: touch it */
            append_to_upper(ESOURCE(e));
            NVPTR(ESOURCE(e)) = upper_list_end;
        }
    }
} /* finish_lower */


/*--------------------------------------------------------------------*/
/*  Management of upper_list and lower_list                           */
/*--------------------------------------------------------------------*/

/*  Insert node n at the end of upper_list
 *  --------------------------------------
 */
static void append_to_upper(GNODE n)
{
    DLLIST  d;

    assert((n));
    debugmessage("append_to_upper", (NTITLE(n)?NTITLE(n):""));

    d = dllist_alloc(n,upper_list_end);
    if (!upper_list)    upper_list = d;
    if (upper_list_end) DSUCC(upper_list_end) = d;
    upper_list_end  = d;
    size_upper_list++;
} /* append_to_upper */


/*  Insert node n at the end of lower_list
 *  --------------------------------------
 */
static void append_to_lower(GNODE n)
{
    DLLIST  d;

    assert((n));
    debugmessage("append_to_lower", (NTITLE(n)?NTITLE(n):""));

    d = dllist_alloc(n,lower_list_end);
    if (!lower_list)    lower_list = d;
    if (lower_list_end) DSUCC(lower_list_end) = d;
    lower_list_end  = d;
    size_lower_list++;
} /* append_to_lower */


/*  Delete node x from upper_list
 *  -----------------------------
 */
static void delete_upper(DLLIST x)
{
    assert((x));
    assert((DNODE(x)));
    debugmessage("delete_upper",
        (NTITLE(DNODE(x))?NTITLE(DNODE(x)):""));
    if (DPRED(x)) DSUCC(DPRED(x)) = DSUCC(x);
    else          upper_list      = DSUCC(x);
    if (DSUCC(x)) DPRED(DSUCC(x)) = DPRED(x);
    else          upper_list_end  = DPRED(x);
    dllist_free(x);
    size_upper_list--;
}


/*  Delete node x from lower_list
 *  -----------------------------
 */
static void delete_lower(DLLIST x)
{
    assert((x));
    assert((DNODE(x)));
    debugmessage("delete_lower",
        (NTITLE(DNODE(x))?NTITLE(DNODE(x)):""));
    if (DPRED(x)) DSUCC(DPRED(x)) = DSUCC(x);
    else          lower_list      = DSUCC(x);
    if (DSUCC(x)) DPRED(DSUCC(x)) = DPRED(x);
    else          lower_list_end  = DPRED(x);
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
    GEDGE e;
    int Sum1, Sum2;

    debugmessage("exchange_nodes_necessary","");

    Sum1 = Sum2 = 0;

    /* First, build predC. We use actlistC to represent predC */

    actlistC = NULL;
    for (e = FirstPred(C); e; e = NextPred(e))
    {
        actlistC = dllist_alloc(ESOURCE(e), actlistC);
        DSUCC(actlistC) = DPRED(actlistC);  /* see dllist_free_all */
    }

    /* We calculate the part of Sum1 and Sum2 originated by predC */

    for (e = FirstPred(D); e; e = NextPred(e))
    {
        n = ESOURCE(e);
        h = actlistC;
        while (h) {
            if (NPOS(n)<NPOS(DNODE(h)))      Sum1++;
            else if (NPOS(n)>NPOS(DNODE(h))) Sum2++;
            h = DSUCC(h);
        }
    }
    dllist_free_all(actlistC);

    /* Next, build succC. We use actlistC to represent succC */

    actlistC = NULL;
    for (e = FirstSucc(C); e; e = NextSucc(e))
    {
        actlistC = dllist_alloc(ETARGET(e), actlistC);
        DSUCC(actlistC) = DPRED(actlistC);  /* see dllist_free_all */
    }

    /* We calculate the part of Sum1 and Sum2 originated by succC */

    for (e = FirstSucc(D); e; e = NextSucc(e))
    {
        n = ETARGET(e);
        h = actlistC;
        while (h) {
            if (NPOS(n)<NPOS(DNODE(h)))      Sum1++;
            else if (NPOS(n)>NPOS(DNODE(h))) Sum2++;
            h = DSUCC(h);
        }
    }
    dllist_free_all(actlistC);

    return(Sum1>Sum2);
} /* exchange_nodes_necessary */



/* Optimization of the crossings of one level
 * ------------------------------------------
 * return 1, if something has changed.
 */
static int level_crossing_optimization(int level, int nearedges)
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
    for (vl2 = TSUCC(tmp_layer[level]); vl2; vl2 = GNNEXT(vl2))
    {
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
    }
    return(changed);
} /* level_crossing_optimization */



/* Local optimization of crossings for all levels
 * ----------------------------------------------
 */
static void local_crossing_optimization(int nearedges)
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
static void prepare_positions(void)
{
    GNLIST h1;
    int i, j;

    debugmessage("prepare_positions","");

    /* First, give all nodes their position numbers */
    for (i=0; i<=maxdepth+1; i++) {
        j = 0;
        for (h1 = TSUCC(tmp_layer[i]); h1; h1 = GNNEXT(h1))
        {
            NPOS(GNNODE(h1)) = j++;
        }
    }
}


/* Check whether a node is complex
 * -------------------------------
 * A node is simple, if it has exactly an predecessor and one
 * successor. This holds for the most dummy nodes that are used
 * to split edges that cross levels.
 */
static int is_complex(GNODE v)
{
    if (NCONNECT(v)) return 1;
    if (!FirstSucc(v) || NextSucc(FirstSucc(v)))
        return 1;
    if (!FirstPred(v) || NextPred(FirstPred(v)))
        return 1;
    return 0;
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
static int check_exchange(GNODE v1, GNODE v2, int dir)
{
    int d1, d2;
    GNODE n1, n2;

    debugmessage("check_exchange","");

    if (is_complex(v1)) return(0);
    if (is_complex(v2)) return(0);
    d1 = NPOS(v1)-NPOS(v2);
    if (d1*d1!=1) return(0);

    if (dir=='S') {
        n1 = ETARGET(FirstSucc(v1));
        n2 = ETARGET(FirstSucc(v2));
    }
    else { /* dir == 'P' */
        n1 = ESOURCE(FirstPred(v1));
        n2 = ESOURCE(FirstPred(v2));
    }
    d2 = NPOS(n1)-NPOS(n2);
    if (d1*d2<0) return(1);

    return check_exchange(n1, n2, dir);
}

/* Exchange two chains of simple nodes
 * -----------------------------------
 * Same as before, but this is not a check. We do the exchanging.
 * The NPOS-numbers are updated, too.
 */
static void do_exchange(GNODE v1, GNODE v2, int dir)
{
    int    d1, d2, h;
    GNODE  n1, n2;
    GNLIST vl1, vl2, vl3;

    debugmessage("do_exchange","");

    if (is_complex(v1)) return;
    if (is_complex(v2)) return;
    d1 = NPOS(v1)-NPOS(v2);
    if (d1*d1!=1) return;

    assert(NTIEFE(v1) == NTIEFE(v2));
    vl1 = vl2 = NULL;
    for (vl3 = TSUCC(tmp_layer[NTIEFE(v1)]); vl3; vl3 = GNNEXT(vl3))
    {
        if (GNNODE(vl3)==v1) vl1 = vl3;
        if (GNNODE(vl3)==v2) vl2 = vl3;
    }
    assert(vl1);
    assert(vl2);
    GNNODE(vl1) = v2;
    GNNODE(vl2) = v1;
    h = NPOS(v1);
    NPOS(v1) = NPOS(v2);
    NPOS(v2) = h;

    if (dir=='S') {
        n1 = ETARGET(FirstSucc(v1));
        n2 = ETARGET(FirstSucc(v2));
    }
    else { /* dir == 'P' */
        n1 = ESOURCE(FirstPred(v1));
        n2 = ESOURCE(FirstPred(v2));
    }
    d2 = NPOS(n1)-NPOS(n2);
    if (d1*d2<0) return;
    do_exchange(n1, n2, dir);
}

/* Check chain exchange for all pair of edges of a node v
 * ------------------------------------------------------
 */
static void unwind_crossed_edges(GNODE v)
{
    GEDGE e1, e2;

    debugmessage("unwind_crossed_edges","");

    for (e1 = FirstSucc(v); e1; e1 = NextSucc(e1))
    {
        for (e2 = FirstSucc(v); e2; e2 = NextSucc(e2))
        {
            if (check_exchange(ETARGET(e1), ETARGET(e2), 'S')) {
                do_exchange(ETARGET(e1), ETARGET(e2), 'S');
            }
        }
    }
    for (e1 = FirstPred(v); e1; e1 = NextPred(e1))
    {
        for (e2 = FirstPred(v); e2; e2 = NextPred(e2))
        {
            if (check_exchange(ESOURCE(e1), ESOURCE(e2), 'P')) {
                do_exchange(ESOURCE(e1), ESOURCE(e2), 'P');
            }
        }
    }
}


/* The entry point of the local unwind phase
 * -----------------------------------------
 */
static void local_unwind_crossings(void)
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
    for (v = nodelist; v; v = NNEXT(v))
    {
        unwind_crossed_edges(v);
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
static void init_barycentering(void)
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
    changed = 1;                 \
    copy_tmp_layer_to_layer();       \
    nr_crossings = cross;            \
    alt_startlevel = tmp_startlevel; \
}

#define tmp_layer_is_worser() {          \
    copy_layer_to_tmp_layer();       \
    tmp_startlevel = alt_startlevel; \
}

#define copy_tmp_layer_to_layer()  { copy_layers(layer,tmp_layer); }
#define copy_layer_to_tmp_layer()  { copy_layers(tmp_layer,layer); }

/*
 *  Note that Phase[1,2][_down,_up] fill all TCROSS(tmp_layer[i])
 *  such the access graph_crossings() is allowed.
 *  Phase 2 needs the level where to start.
 */

static int phase2_startlevel;

static void barycentering(void)
{
    int alt;    /* temporary flag: exist an alternative ? */
    int cross;  /* number crossings of tmp_layer          */
    int changed;
    int tmp_startlevel, alt_startlevel;

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
        if ( (cross < nr_crossings)
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
        else if (cross == nr_crossings)        alt = 1;
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
} /* barycentering */

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

static void    Phase1_down(void)
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

static void    Phase1_up(void)
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

static int     resort_down_layer(int i)
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

    save_level(i+1);    /* save old level temporary */

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

static int     resort_up_layer(int i)
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

    save_level(i);      /* save old level temporary */

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

static void    Phase2_down(void)
{
    int     i,j;
    int     cross;

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
} /* Phase2_down */


/*  Phase 2 upwards
 *  ---------------
 *  traverses all levels upwards, look where bary-values are equal,
 *  cycles these regions in the layers, and starts a resorting
 *  traversal from the layer locally downwards and upwards through the layers.
 */

static void    Phase2_up(void)
{
    int     i,j;
    int     cross;

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
} /* Phase2_up */


/*  Resort up and down
 *  ------------------
 *  At level, the ordering has just changed.
 *  Starting at level, we traverse the layers downwards
 *  and resort until nothing changes anymore.
 *  Then we revert the direction, starting at level, we
 *  traverse the layers upwards and resort until nothing
 *  changes anymore.
 */

static void resort_up_down_layer(int level)
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

static void resort_down_up_layer(int level)
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

static int  cycle_sort_array(int siz)
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
            if (start_region == -1) start_region = j;
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
static float succbary(GNODE node)
{
    int Sum;
    int i;
    GEDGE e;

    assert(node);
    debugmessage("succbary","");
    /* Assertion: The NPOS-values are set before by level_to_array */

    Sum = 0;
    i = 0;
    for (e = FirstSucc(node); e; e = NextSucc(e))
    {
        Sum += NPOS(ETARGET(e));
        i++;
    }
    if (i==0)
        return 0.0;
    else
        return ((float) Sum) / ((float) i);
}


/*  Predbary-Value of a node
 *  ------------------------
 *  predbary(v) = ( Sum pos(predecessors(v)) ) / indegree(v) )
 */
static float predbary(GNODE node)
{
    int Sum;
    int i;
    GEDGE e;

    assert(node);
    debugmessage("predbary","");
    /* Assertion: The NPOS-values are set before by level_to_array */

    Sum = 0;
    i = 0;
    for (e = FirstPred(node); e; e = NextPred(e))
    {
        Sum += NPOS(ESOURCE(e));
        i++;
    }
    if (i==0)
        return 0.0;
    else
        return ((float) Sum) / ((float) i);
}



/*  Succmedian-Value of a node
 *  --------------------------
 *  succmedian(v) = interpolated median value
 *
 *  For instance, if a node has successors with positions
 *          5  7  13  49  57
 *  then 13 is the real median.
 *  If the outdegree is even, then the median is not unique,
 *  e.g.      5  7  13  49  57  62
 *  the left median is 13 and the right median is 49. In this
 *  case we interpolate, i.e.
 *  median = ((13-5) * 49 + (62-57) * 13) /  (13-5+62-57) = 35.153
 */
static float succmedian(GNODE node)
{
    int i, sum, leftpart, rightpart;
    GEDGE e;

    assert((node));
    /* Assertion: the save_array must be unused at that time */

    debugmessage("succmedian","");
    /* Assertion: The NPOS-values are set before by level_to_array */

    i = 0;
    for (e = FirstSucc(node); e; e = NextSucc(e))
    {
        save_array[i++] = ETARGET(e);
    }
    switch (i) {
    case 0: return(0.0);
    case 1: return((float) NPOS(save_array[0]));
    case 2:
        sum = NPOS(save_array[0]) + NPOS(save_array[1]);
        return ( ((float) sum) / 2.0);
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
static float predmedian(GNODE node)
{
    int i, sum, leftpart, rightpart;
    GEDGE e;

    assert((node));
    /* Assertion: the save_array must be unused at that time */

    debugmessage("predmedian","");
    /* Assertion: The NPOS-values are set before by level_to_array */

    i = 0;
    for (e = FirstPred(node); e; e = NextPred(e))
    {
        save_array[i++] = ESOURCE(e);
    }
    switch (i) {
    case 0: return(0.0);
    case 1: return((float) NPOS(save_array[0]));
    case 2:
        sum = NPOS(save_array[0]) + NPOS(save_array[1]);
        return ( ((float) sum) / 2.0);
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

static int  compare_bary(const GNODE *a, const GNODE *b)
{
    /*if ((NBARY(*a)==0.0) || (NBARY(*b)==0.0)) return(0);*/
    if (NBARY(*a) > NBARY(*b))            return(1);
    if (NBARY(*a) < NBARY(*b))            return(-1);
    return(0);
}



/*  Compare function for sorting according positions
 *  ------------------------------------------------
 *  returns 1 if NPOS(*a) > NPOS(*b), 0 if equal, -1 otherwise.
 */

static int  compare_pos(const GNODE *a, const GNODE *b)
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
 *          rand_num = rand_num + 10891;
 *  Note that 10891 is a prime number.
 */

#define exchange(a,b) { t=a; a=b; b=t; }

static unsigned int rand_num = 211;

static void     myqsort(int l, int r)
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

#endif /* OWN_QUICKSORT */


/*--------------------------------------------------------------------*/
/*  level and array exchange routines                                 */
/*--------------------------------------------------------------------*/


/*  Put level tmp_layer[i] into the sort_array
 *  ------------------------------------------
 *  and set all NPOS of the nodes of the next or previous layer.
 *  This is done before sorting.
 *  Note the size of the layer is as long as the size of the array.
 */
static void level_to_array(int i, int dir)
{
    int j;
    GNLIST hn;

    debugmessage("level_to_array","");
    j = 0;
    for (hn = TSUCC(tmp_layer[i]); hn; hn = GNNEXT(hn))
    {
        sort_array[j++] = GNNODE(hn);
    }
    if (dir=='d') {
        assert((i>0));
        hn = TSUCC(tmp_layer[i-1]);
    }
    else /* dir='u' */ {
        assert((i<maxdepth+1));
        hn = TSUCC(tmp_layer[i+1]);
    }
    j = 1;
    for ( ; hn; hn = GNNEXT(hn))
    {
        NPOS(GNNODE(hn)) = j++;
    }
}


/*  Put the sort_array back into tmp_layer[i]
 *  -----------------------------------------
 *  This is done after sorting.
 *  Note the size of the layer is as long as the size of the array.
 */
static void array_to_level(int i)
{
    int j;
    GNLIST hn;

    debugmessage("array_to_level","");
    j  = 0;
    for (hn = TSUCC(tmp_layer[i]); hn; hn = GNNEXT(hn))
    {
        GNNODE(hn) = sort_array[j++];
    }
    assert(j == TANZ(tmp_layer[i]));
}


/*  Put level tmp_layer[i] into the save_array
 *  ------------------------------------------
 *  This is done to store the tmp_layer[i] temporary.
 */
static void save_level(int i)
{
    int j;
    GNLIST hn;

    debugmessage("save_level","");
    j  = 0;
    for (hn = TSUCC(tmp_layer[i]); hn; hn = GNNEXT(hn))
    {
        save_array[j++] = GNNODE(hn);
    }
    assert(j==TANZ(tmp_layer[i]));
}

/*  Put the save_array back into tmp_layer[i]
 *  -----------------------------------------
 *  This is done to restore the tmp_layer[i].
 */
static void restore_level(int i)
{
    int j;
    GNLIST hn;

    debugmessage("restore_level","");
    j  = 0;
    for (hn = TSUCC(tmp_layer[i]); hn; hn = GNNEXT(hn))
    {
        GNNODE(hn) = save_array[j++];
    }
    assert(j==TANZ(tmp_layer[i]));
}


/*  Reorder the tmp_layer[i] according to the HORDER
 *  ------------------------------------------------
 *  This is done by storing tmp_layer[i] to the sort_array,
 *  sorting it according to NHORDER and storing it back.
 */
static void apply_horder(int i)
{
    GNLIST hn;
    int j;

    debugmessage("apply_horder","");

    /* Assertions:
     * NBARY(nodes) is currently not needed.
     * The sort_array is free.
     * The actual layer is tmp_Layer.
     */

    j = 0;
    for (hn = TSUCC(tmp_layer[i]); hn; hn = GNNEXT(hn))
    {
        if (NHORDER(GNNODE(hn)) >= 0) {
            sort_array[j++] = GNNODE(hn);
            NBARY(GNNODE(hn)) = NHORDER(GNNODE(hn));
        }
    }

    quicksort_sort_array(j);

    j = 0;
    for (hn = TSUCC(tmp_layer[i]); hn; hn = GNNEXT(hn))
    {
        if (NHORDER(GNNODE(hn)) >= 0)
            GNNODE(hn) = sort_array[j++];
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
static void copy_layers(DEPTH *l1, DEPTH *l2)
{
        int    i;
    GNLIST h1, h2;

    debugmessage("copy_layers","");
    for (i=0; i<=maxdepth+1; i++) {
        TCROSS(l1[i]) = TCROSS(l2[i]);
        h1 = TSUCC(l1[i]);
        h2 = TSUCC(l2[i]);
        if (TANZ(l1[i])==TANZ(l2[i])) {
            while (h2) {
                assert((h1));   /* both lenght are equal */
                GNNODE(h1) = GNNODE(h2);
                h1 = GNNEXT(h1);
                h2 = GNNEXT(h2);
            }
        }
        else {
            assert(TANZ(l1[i]) < TANZ(l2[i]));
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
static void insert_connects_in_layer(void)
{
    int i, j;
    GNLIST  hl, hln;
    CONNECT c;
    int     forward_conn;
    int     changed;

    debugmessage("insert_connects_in_layer","");
    for (i=0; i<=maxdepth+1; i++) {
        changed = 0;
        for (hl = TSUCC(tmp_layer[i]); hl; hl = hln)
        {
            hln = GNNEXT(hl);
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
                check_connect(i, GNNODE(hl));
            }
        }
        if (changed) {
            if (i<=maxdepth)
                TCROSS(tmp_layer[i]) = layer_crossing(i);
                for (j=i; j<=maxdepth; j++)
                    (void)resort_down_layer(j);
        }
    }
} /* insert_connects_in_layer */

/**
 * Restore the edges that were in the adjacency lists
 * of the connection nodes.
 */
static void revive_conn_edges(GNODE v, GNODE w, GNODE predw)
{
    ADJEDGE sv_succs, sv_preds, a;
    GEDGE e;
    CONNECT c = NCONNECT(w);

    assert(c);

    /* restore the adjacency lists of the connection
     * Remind: The adjacency lists were destroyed in step1 in
     * calc_connect_adjlists.
     */
    sv_succs = reverse_adjlist(CSVSUCC(c));
    sv_preds = reverse_adjlist(CSVPRED(c));
    CSVSUCC(c) = DEAD_GELIST;
    CSVPRED(c) = DEAD_GELIST;

    if (v!=w) {
        assert(FirstSucc(w) == NULL);
        assert(FirstPred(w) == NULL);
    }

    for (a = sv_succs; a; a = ANEXT(a))
    {
        e = AKANTE(a);
        assert(ESOURCE(e) == v);
        change_edge_src(e, ESOURCE(e), w);
    }

    for (a = sv_preds; a; a = ANEXT(a))
    {
        e = AKANTE(a);
        assert(ETARGET(e) == v);
        change_edge_dst(e, ETARGET(e), w);
    }

    if (v==w) {
        if (forward_connection1(c)) revive_conn_edges(v, CTARGET(c), v);
        if (forward_connection2(c)) revive_conn_edges(v, CTARGET2(c), v);

        /* для основного узла после переноса всех дуг еще проверяем,
         * что не осталось висеть лишних дуг */
        if (sv_succs) {
            e = AKANTE(sv_succs);
            assert(NextSucc(e) == NULL); /* что нет лишних дуг */
        } else {
            assert(FirstSucc(v) == NULL);
        }
        if (sv_preds) {
            e = AKANTE(sv_preds);
            assert(NextPred(e) == NULL); /* что нет лишних дуг */
        } else {
            assert(FirstPred(v) == NULL);
        }
    } else {
        if (c && CTARGET(c) && (CTARGET(c)!=predw))   revive_conn_edges(v, CTARGET(c), w);
        if (c && CTARGET2(c) && (CTARGET2(c)!=predw)) revive_conn_edges(v, CTARGET2(c), w);
    }
    delete_adjlist(sv_succs);
    delete_adjlist(sv_preds);
} /* revive_conn_edges */

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

static GNLIST leftlist;     /* the list of nodes that are left  */
static GNLIST leftlistend;      /* neighbours of node               */
static GNLIST rightlist;    /* the list of nodes that are right */
static GNLIST rightlistend; /* neighbours of node               */

/* Because we insert leftlist and rightlist for test reason, we
 * must be able to restore the original layer[level]-list. Thus, we
 * store the points that we have changes.
 */

static GNLIST *insertpoint; /* *insertpoint becomes leftlist           */
static GNLIST middlepoint;      /* GNNEXT(middlepoint) becomes rightlist   */
static GNLIST endpoint;     /* GNNEXT(rightlistend) will point to this */

static void check_connect(int level, GNODE node)
{
    CONNECT c = NCONNECT(node);
    int     clr, crl;

    debugmessage("check_connect","");

    revive_conn_edges(node, node, NULL);


    /* First we try to insert left first:
     *   -> Cl -> Cl -> A -> Cr -> Cr -> ...
     * and calculate the number of crossings, that now exist.
     */
    leftlist = leftlistend = rightlist = rightlistend = NULL;
    if (forward_connection1(c)) left_conn_list(CTARGET(c), node);
    if (forward_connection2(c)) right_conn_list(CTARGET2(c), node);

    insert_left_right(level, node);

    /* Calculate the actual number of crossings */
    clr = 0;
    if (level>0)         clr += layer_crossing(level-1);
    if (level<=maxdepth) clr += layer_crossing(level);

    /* Now we do the reverse inserting,
     *   -> Cr -> Cr -> A -> Cl -> Cl -> ...
     * and calculate the number of crossings, that now exist.
     */

    *insertpoint = middlepoint;
    GNNEXT(middlepoint) = endpoint;

    leftlist = leftlistend = rightlist = rightlistend = NULL;
    if (forward_connection1(c)) right_conn_list(CTARGET(c), node);
    if (forward_connection2(c)) left_conn_list(CTARGET2(c), node);

    insert_left_right(level, node);


    /* Calculate the actual number of crossings */
    crl = 0;
    if (level>0)         crl += layer_crossing(level-1);
    if (level<=maxdepth) crl += layer_crossing(level);

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
    if (forward_connection1(c)) left_conn_list(CTARGET(c), node);
    if (forward_connection2(c)) right_conn_list(CTARGET2(c), node);

    insert_left_right(level, node);

} /* check_connect */


/* Insert left_list and right_list
 * -------------------------------
 * The insertion point is the cons-cell at TSUCC(tmp_layer[level])
 * that just points to node. After insertion, TANZ(tmp_layer[level])
 * is recalculated.
 */

static void insert_left_right(int level,GNODE node)
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
    assert((hl));   /* node should be in the list */

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
    for (hl = TSUCC(tmp_layer[level]); hl; hl = GNNEXT(hl))
    {
        j++;
    }
    TANZ(tmp_layer[level]) = j;
}


/* Calculate left_list
 * -------------------
 * Traverse the connections and add them to left_list.
 * Further, restore the edges that were in the adjacency lists
 * of the connection nodes.
 */
static void left_conn_list(GNODE v,GNODE w)
{
    GNLIST h;
    CONNECT c;

    debugmessage("left_conn_list","");

    h = tmpnodelist_alloc();
    GNNODE(h) = v;
    GNNEXT(h) = leftlist;
    leftlist = h;
    if (!leftlistend) leftlistend = h;

    c = NCONNECT(v);
    if (c && CTARGET(c) && (CTARGET(c)!=w))   left_conn_list(CTARGET(c), v);
    if (c && CTARGET2(c) && (CTARGET2(c)!=w)) left_conn_list(CTARGET2(c), v);
}

/* Calculate right_list
 * -------------------
 * Traverse the connections and add them to left_list.
 * Further, restore the edges that were in the adjacency lists
 * of the connection nodes.
 */
static void right_conn_list(GNODE v,GNODE w)
{
    GNLIST h;
    CONNECT c;

    debugmessage("right_conn_list","");

    h = tmpnodelist_alloc();
    GNNODE(h) = v;
    if (rightlistend) GNNEXT(rightlistend) = h;
    GNNEXT(h) = NULL;
    rightlistend = h;
    if (!rightlist) rightlist = h;

    c = NCONNECT(v);
    if (c && CTARGET(c) && (CTARGET(c)!=w))   right_conn_list(CTARGET(c), v);
    if (c && CTARGET2(c) && (CTARGET2(c)!=w)) right_conn_list(CTARGET2(c), v);
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
static void recreate_predlists(void)
{
    GNLIST h1, h2;
    int i,j,k;

    debugmessage("recreate_predlists","");

    /* First, give all nodes their position numbers */
    for (i=0; i<=maxdepth+1; i++) {
        j = 0;
        for (h1 = TSUCC(layer[i]); h1; h1 = GNNEXT(h1))
        {
            NPOS(GNNODE(h1)) = j++;
        }
    }
    for (i=0; i<=maxdepth+1; i++) {
        TPRED(layer[i]) = NULL;
        k = 0;
        for (h1 = TSUCC(layer[i]); h1; h1 = GNNEXT(h1))
        {
            k++;
            h2 = tmpnodelist_alloc();
            GNNEXT(h2) = TPRED(layer[i]);
            TPRED(layer[i]) = h2;
            GNNODE(h2) = GNNODE(h1);
        }
        assert(TANZ(layer[i]) == k);
    }
    /*  Sort adjacencies according NPOS
     *  -------------------------------
     *  This gives the fine layout the last touch, even if the effect
     *  is very small.
     */
    sort_all_adjacencies();
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

void    db_output_all_layers(void)
{
    int i;
    for (i=0; i<=maxdepth+1; i++) db_output_tmp_layer(i);
    PRINTF("\n");
}

#endif


/* Output one layer textually
 * --------------------------
 */

#ifdef DEBUG

void    db_output_tmp_layer(int i)
{
    GNLIST li;
    GEDGE e;

    PRINTF("layer[%d]: ", i);
    for (li = TSUCC(tmp_layer[i]); li; li = GNNEXT(li))
    {
        if (NTITLE(GNNODE(li))[0])
            PRINTF("%s[", NTITLE(GNNODE(li)));
        else    PRINTF("?[");
        for (e = FirstPred(GNNODE(li)); e; e = NextPred(e))
        {
            if (NTITLE(ESOURCE(e))[0])
                PRINTF("%s,", NTITLE(ESOURCE(e)));
            else    PRINTF("?,");
        }
        PRINTF("][");
        for (e = FirstSucc(GNNODE(li)); e; e = NextSucc(e))
        {
            if (NTITLE(ETARGET(e))[0])
                PRINTF("%s,", NTITLE(ETARGET(e)));
            else    PRINTF("?,");
        }
        PRINTF("]");
        PRINTF("b(%f)p(%d) ",NBARY(GNNODE(li)),NPOS(GNNODE(li)));
    }
    PRINTF("\n");
}

#endif


/* Check whether the adjacencies of a node v are proper
 * ----------------------------------------------------
 * level is the level v has to be;
 */

#ifdef DEBUG

int db_check_proper(GNODE v, int level)
{
    int t;
    char *title,*st,*tt;
    GEDGE e;

    if (NTITLE(v)[0])
        title = NTITLE(v);
    else    title = "?";
    t = NTIEFE(v);
    if (level!=t)
        PRINTF("%s at level %d, expected %d\n",title,t,level);
    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        if (!ESOURCE(e)) {
            PRINTF("Succedge at %s missing source\n",title);
            break;
        }
        if (!ETARGET(e)) {
            PRINTF("Succedge at %s missing source\n",title);
            break;
        }
        if (NTITLE(ESOURCE(e))[0])
            st = NTITLE(ESOURCE(e));
        else    st = "?";
        if (NTITLE(ETARGET(e))[0])
            tt = NTITLE(ETARGET(e));
        else    tt = "?";
        if (ESOURCE(e)!=v) {
            PRINTF("Succedge (%s,%s) at %s wrong source\n",
                    st,tt,title);
            break;
        }
        if (NTIEFE(ETARGET(e))!=t+1) {
            PRINTF("Succedge (%s,%s) depth %d (%d expected)\n",
                    st,tt,NTIEFE(ETARGET(e)),t+1);
            break;
        }
    }
    for (e = FirstPred(v); e; e = NextPred(e))
    {
        if (!ESOURCE(e)) {
            PRINTF("Prededge at %s missing source\n",title);
            break;
        }
        if (!ETARGET(e)) {
            PRINTF("Prededge at %s missing source\n",title);
            break;
        }
        if (NTITLE(ESOURCE(e))[0])
            st = NTITLE(ESOURCE(e));
        else    st = "?";
        if (NTITLE(ETARGET(e))[0])
            tt = NTITLE(ETARGET(e));
        else    tt = "?";
        if (ETARGET(e)!=v) {
            PRINTF("Prededge (%s,%s) at %s wrong target\n",
                    st,tt,title);
            break;
        }
        if (NTIEFE(ESOURCE(e))!=t-1) {
            PRINTF("Succedge (%s,%s) depth %d (%d expected)\n",
                    st,tt,NTIEFE(ESOURCE(e)),t-1);
            break;
        }
    }
    return(1);
}

#endif

