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
 * into layers that represent a proper hierarchy. All nodes have already
 * their relative position inside the layers.
 * We want to calculate now the co-ordinated of the nodes.
 * We have:
 *    1)  The array layer contains all visible nodes.
 *        They are distributed at the layer[i] lists and connected by two
 *        lists TPRED and TSUCC to allow to traverse the nodes of one
 *        layer[i] backwards and forwards.
 *    2)  Note that the nodes reacheable via forward connections are now
 *        in the TSUCC and TPRED lists, too.
 *        TANZ(layer[i]) is the number of nodes in layer[i].
 *    3)  The hierarchy in layer is proper.
 *    4)  Further, all visible nodes are in nodelist, labellist and dummylist.
 *    5)  All pot. visible edges are in the lists edgelist or tmpedgelist,
 *        Visible edges can be detected by the EINVISIBLE flag (==0) in these
 *        lists. Note: invisible edges may also be in edgelist or tmpedgelist.
 *        An edge is visible iff
 *                 a) it is used in the adjacency lists.
 *              or b) it is a direct neigbour edge in NCONNECT(v) for
 *                    some node v.
 *    6)  maxindeg and maxoutdeg are upper estimations of NINDEG and
 *        NOUTDEG of nodes.
 *    7)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    8)  NTIEFE(node) is filled for all nodes. NINDEG and NOUTDEG are
 *        filled. Forward connections are not counted.
 *        NCONNECT(node) is filled for nodes that have direct neighbours
 *        in the layout. The edges in NCONNECT are not anymore in the
 *        adjacency lists, but still visible.
 *        See point 2 !!!
 *    9)  Reverted edges are marked with EART(e)='R'.
 *        Self loops don't anymore exist.
 *    10) NPOS(v) gives the horizontal position of a node inside the
 *        layer. Adjacency edges are sorted according to these NPOS
 *        values. The ordering inside a layer is such that the number
 *        of cossings is small (but may be not optimal).
 *    11) NSUCCL(v) and NSUCCR(v) are the leftest and rightest successor
 *        edge of v, and NPREDL(v) and NPREDR(v) the leftest and rightest
 *        predecessor edge.
 *
 * To calculate the y-coordinates, we must simply look which level
 * the node belongs to. All nodes of one level have the same y-coordinates.
 * But for the x-position, we must shift the nodes inside the layer
 * to calculate the correct space between the nodes. This is done
 * by two steps: first we try to assign positions to the most left
 * and right upper and lower node. The we center all nodes with respect
 * to their parent and child nodes. We use a special weight G here,
 * which has to be minimized.
 *
 * After this, we have the following situation:
 *    1-11) same as before
 *    12)   All nodes have filled NX, NY, NWIDTH and NHEIGHT such that
 *          they do not overlap. NX and NY are absolutely. NWIDTH and
 *          NHEIGHT are stretched or shrinked according to the local
 *          factors at the nodes.
 *
 * This file provides the following functions:
 * ------------------------------------------
 * step3_main       Main routine to calculate coordinates
 *
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "alloc.h"
#include "main.h"
#include "options.h"
#include "folding.h"
#include "steps.h"
#include "timing.h"
#include "graph.h"

/* Prototypes
 * ----------
 */

#undef DRAWDEBUG
#ifdef DRAWDEBUG
extern void     debug_display_part  _PP((void));
extern void     display_part        _PP((void));
static void     drawdebug_show_graph    _PP((char *m,int i,int j,int k));
#else
#define     drawdebug_show_graph(a,b,c,d)
#endif

static void init_xy_coordinates _PP((void));
static void center_layout       _PP((void));
static void shift_left_layout   _PP((void));

static void     shift_left_together_layout _PP((void));
static void     shift_intermixed_part      _PP((int t));
static void     shift_left_part        _PP((GNODE node, int i));
static void     mark_and_calc_maxx     _PP((GNODE node));

static void     shift_connect_together_layout  _PP((void));
static void     correct_connect_part           _PP((GNODE v));
static void     traverse_and_search_mindist    _PP((GNODE v));

static void     straight_line_tuning    _PP((void));
static int  do_straight_line    _PP((GNODE v));
static int  full_straight_possible  _PP((GNODE sw, int sxpos, int dir));
static void straight_part       _PP((GNODE sw, int sxpos, int dir));

static void iterate_dump_mediumshifts _PP((void));
static int  dumpmediumshift     _PP((int f));

static void iterate_centershifts    _PP((void));
static int  changed_nw_sum          _PP((void));

static int  nwsdump_mediumshift     _PP((int i,int dir));
static int  nwpdump_mediumshift     _PP((int i,int dir));
static int  nwdump_mediumshift  _PP((int i,int dir));
static int  summarize_dumpshift _PP((int i,int dir));
static int  priosummarize_dumpshift _PP((int i,int dir));
static int  correct_priority_nodes  _PP((int i));
static int  center_weight       _PP((void));
static int  center_layer        _PP((int i));
static int  do_leftshifts       _PP((int i));
static int  do_rightshifts      _PP((int i));

static int  nw      _PP((GNODE node));
static int  nwbend      _PP((GNODE node,GNODE lnode, GNODE rnode));
static int  ews     _PP((GEDGE edge));
static int  nws     _PP((GNODE node));
static int  ewp     _PP((GEDGE edge));
static int  nwp     _PP((GNODE node));

static void save_plevel _PP((int i));



/* Global variables
 * ----------------
 */

/* The following two arrays are used to store the constraints of the
 * nodes inside the levels.
 * levelshift gives the summary number of pixels a node should be shifted.
 * levelweight gives the number of constains.
 * Thus levelshift[i]/levelweight[i] is the number of real pixels
 * a node with position i should be shifted.
 */

static int  size_of_levelsw;    /* size of arrays levelshift */
                    /* and levelweight       */
static int  *levelshift =NULL;
static int  *levelweight=NULL;

/* array where we can temporary save one layer. It has always the
 * same size as the levelshift-array.
 * This is also used in step4.
 */

static GNODE    *slayer_array = NULL;


/* Two arrays that allow to find a node's list cell in the TPRED and TSUCC
 * lists of a layer. This is used in step 4 later.
 * Here, these arrays are overloaded with slayer_array and levelweight.
 */

GNLIST   *tpred_connection1 = NULL;
GNLIST   *tpred_connection2 = NULL;


/* Macros
 * ------
 */

#define TMINX(x) TCROSS(x)

/*--------------------------------------------------------------------*/
/*  Calculation of co-ordinates                       */
/*--------------------------------------------------------------------*/

#ifdef DRAWDEBUG
static int firstcall = 1;
#endif

/**
 * Layout phase 3: calculation of coordinates of nodes.
 */
void step3_main(void)
{
    start_time();
    debugmessage("step3_main","");
    assert((layer));

#ifdef DRAWDEBUG
    if (firstcall) {
        firstcall = 0;
        PRINTF("Enter ` ` into the drawwindow\n");
        display_part();
        return;
    }
#endif

        /* First we calculate how many up-down ports a node has, and how
         * the edges are scheduled among these ports. See step 4.
         */

    calc_all_ports(0);

    /* Calculate NWIDTH and NHEIGHT of all nodes */

    calc_all_node_sizes();

    /* Start with a minimal-distance-leftbound layout */

    init_xy_coordinates();

    /* Center the layout at the y-axis */

    center_layout();

    alloc_levelshift();


    /* Shift nodes of an edge by a medium measure, to become a
     * not too dense situation. We do until the leftest and
     * rightest upper and lower nodes do not change anymore.
     */

    iterate_dump_mediumshifts();


    /* Shift nodes such that they are centered between all edges
     * pointing to or from the nodes.
     */

    iterate_centershifts();

    if (straight_phase==1) straight_line_tuning();

    /* Shift the layout to the left such that the minimal x coordinate
     * is G_xbase.
     */

    shift_left_layout();
    shift_left_together_layout();
    shift_connect_together_layout();

    /* Once again: check the ports, because they may have changed
     * for some shapes.
     */
    calc_all_ports(1);

    stop_time("step3_main");

} /* step3_main */


/*--------------------------------------------------------------------*/
/*  Allocation of the arrays levelshift, etc.  */
/*--------------------------------------------------------------------*/


void alloc_levelshift(void)
{
    int len;

    debugmessage("alloc_levelshift","");

    if (sizeof(GNLIST)<sizeof(GNODE))
        Fatal_error("Assertion 1 about structs is wrong.","");
    if (sizeof(GNLIST)<sizeof(int))
        Fatal_error("Assertion 2 about structs is wrong.","");

#ifdef TESTLAYOUT
    PRINTF("Maximal nodes per layer: %d\n", max_nodes_per_layer);
    PRINTF("Maximal layers: %d\n", maxdepth);
#endif
    if (max_nodes_per_layer+2 > size_of_levelsw) {
        if (levelshift)        free(levelshift);
        if (tpred_connection1) free(tpred_connection1);
        if (tpred_connection2) free(tpred_connection2);
        len = max_nodes_per_layer+2;
        levelshift = (int *)libc_malloc(len * sizeof(int));
        tpred_connection1 = (GNLIST *)libc_malloc(len * sizeof(GNLIST));
        tpred_connection2 = (GNLIST *)libc_malloc(len * sizeof(GNLIST));
        levelweight  = (int *)tpred_connection1;
        slayer_array = (GNODE *)tpred_connection2;
        size_of_levelsw = max_nodes_per_layer+2;
#ifdef DEBUG
        PRINTF("Sizeof tables `levelshift',`levelweight',`slayer_array': %d Bytes\n",
            (max_nodes_per_layer+2)*sizeof(int));
#endif
    }
} /* alloc_levelshift */


/*--------------------------------------------------------------------*/
/*  Calculation of NWIDTH and NHEIGHT                     */
/*--------------------------------------------------------------------*/

/* Traverse all visible nodes and set NWIDTH and NHEIGHT of the nodes.
 * If NWIDTH and NHEIGHT are not already set, they are derived from
 * the size of the label.
 */
void calc_all_node_sizes(void)
{
    GNODE v;
    int   h, hh, hhh;

    debugmessage("calc_all_node_sizes","");
    for (v = nodelist; v; v = NNEXT(v))
    {
        calc_node_size(v);

        if (  (G_orientation==LEFT_TO_RIGHT)
            ||(G_orientation==RIGHT_TO_LEFT)) {
            h          = NWIDTH(v);
            NWIDTH(v)  = NHEIGHT(v);
            NHEIGHT(v) = h;
        }
    }

    /* For the labellist, width and height are already set.
     * See folding.c: adapt_labelpos.
     * But if orientation is left to right or converse, we must
     * turn here.
     */
    if (  (G_orientation==LEFT_TO_RIGHT)
        ||(G_orientation==RIGHT_TO_LEFT)) {
        for (v = labellist; v; v = NNEXT(v))
        {
            h          = NWIDTH(v);
            NWIDTH(v)  = NHEIGHT(v);
            NHEIGHT(v) = h;
        }
    }

    for (v = dummylist; v; v = NNEXT(v))
    {
        if (NANCHORNODE(v)) {
            h = get_node_preds_num(v);
            hh = get_node_succs_num(v);
            hhh = (h > hh) ? h : hh;
            assert(NCONNECT(v));
            NHEIGHT(v) = NHEIGHT(CTARGET(NCONNECT(v)));
            NWIDTH(v) =  (hhh+1)*G_xspace;
            if (manhatten_edges==1) {
                if ((h<=1)&&(hh==0)) NWIDTH(v) = 0;
                if ((h==0)&&(hh<=1)) NWIDTH(v) = 0;
            }
        }
        else {
            NWIDTH(v)   = 0;
            NHEIGHT(v)  = 0;
        }
        if (  (G_orientation==LEFT_TO_RIGHT)
            ||(G_orientation==RIGHT_TO_LEFT)) {
            h          = NWIDTH(v);
            NWIDTH(v)  = NHEIGHT(v);
            NHEIGHT(v) = h;
        }
    }
} /* calc_all_node_sizes */


/*--------------------------------------------------------------------*/
/*  Initialize x and y coordinates                    */
/*--------------------------------------------------------------------*/

/*  Initialize with minimal x-y coordinates
 *  ---------------------------------------
 *  The initial layout is the leftbound. All nodes have minimal
 *  distance between, ignoring constraints because of edges.
 *  Something like this (Example)
 *    A B C
 *    D E F G H
 *    I J
 *    K L M
 */

#define xralign(a)  ((((a)+G_xraster-1)/G_xraster)*G_xraster)
#define yralign(a)  ((((a)+G_yraster-1)/G_yraster)*G_yraster)
#define dxralign(a) ((((a)+G_dxraster-1)/G_dxraster)*G_dxraster)
#define dyralign(a) ((((a)+G_dyraster-1)/G_dyraster)*G_dyraster)

#define xllalign(a)  ((((a)-G_xraster+1)/G_xraster)*G_xraster)
#define xlalign(a)  ((((a))/G_xraster)*G_xraster)
#define ylalign(a)  ((((a))/G_yraster)*G_yraster)
#define dxlalign(a) ((((a))/G_dxraster)*G_dxraster)
#define dylalign(a) ((((a))/G_dyraster)*G_dyraster)

static void init_xy_coordinates(void)
{
    int actxpos, actypos;
    int maxboxheight;
    GNLIST  li;
    GNODE   v;
    int i;

    debugmessage("init_xy_coordinates","");
    if (G_yspace<5)  G_yspace=5;
    if (G_xspace<5)  G_xspace=5;
    if (G_dspace==0) {
        if (G_spline) G_dspace = 4*G_xspace/5;
        else          G_dspace = G_xspace/2;
    }
    if (G_flat_factor<1)   G_flat_factor = 1;
    if (G_flat_factor>100) G_flat_factor = 100;

    actypos = G_ybase;
    actypos = yralign(actypos);

    for (i=0; i<=maxdepth+1; i++) {
        actxpos = G_xbase;
        maxboxheight = 0;
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            v = GNNODE(li);

            if ((NWIDTH(v)==0)&&(NHEIGHT(v)==0))
                  NX(v) = dxralign(actxpos+NWIDTH(v)/2)-NWIDTH(v)/2;
            else  NX(v) =  xralign(actxpos+NWIDTH(v)/2)-NWIDTH(v)/2;
            NY(v) = actypos;

            actxpos = NX(v) + NWIDTH(v) + G_xspace;
            if (maxboxheight<NHEIGHT(v))
                maxboxheight = NHEIGHT(v);
        }

        if (G_yalign==AL_CENTER) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                NY(GNNODE(li)) += (maxboxheight-NHEIGHT(GNNODE(li)))/2;
            }
        }
        else if (G_yalign==AL_BOTTOM) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                NY(GNNODE(li)) += (maxboxheight-NHEIGHT(GNNODE(li)));
            }
        }
        actypos += (maxboxheight + G_yspace);
        actypos = yralign(actypos);
    }
} /* init_xy_coordinates */


/*--------------------------------------------------------------------*/
/*  Center the layout                             */
/*--------------------------------------------------------------------*/

/*  Center the layers
 *  -----------------
 *  This moves the layers independently, such that the layers are
 *  centered at the y-axis. The result is symmetrical.
 *  Something like this (Example)
 *      A B C
 *    D E F G H    (the x==0 axis goes though B, F and L)
 *       I J
 *      K L M
 */
static void center_layout(void)
{
    GNLIST  li;
    int i;
    int shift_value;

    debugmessage("center_layout","");
    for (i=0; i<=maxdepth+1; i++) {
        li = TPRED(layer[i]); /* rightest node */
        if (li) {
            assert(TSUCC(layer[i]));
            shift_value = (NX(GNNODE(li))+NWIDTH(GNNODE(li)))/2;
            shift_value = xlalign(shift_value);
            while (li) {
                NX(GNNODE(li)) -= shift_value;
                li = GNNEXT(li);
            }
        }
    }
}

/*--------------------------------------------------------------------*/
/*  Shift the layout to the left x-position               */
/*--------------------------------------------------------------------*/

/*  Shift the complete layout
 *  -------------------------
 *  This moves all layers together, such that the minimal x value
 *  is G_xbase, and the minimal y value is G_ybase. With other words,
 *  the layout is shifted to the left upper part of the coordinate
 *  system.
 */
static void shift_left_layout(void)
{
    GNLIST li;
    int i;
    int minx,miny;

    debugmessage("shift_left_layout","");

    /* first, calculate minx and miny */
    minx = miny = MAXINT;
    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            if (NX(GNNODE(li))<minx) minx = NX(GNNODE(li));
            if (NY(GNNODE(li))<miny) miny = NY(GNNODE(li));
        }
    }

    minx = minx - G_xbase;
    miny = miny - G_ybase;
    minx = xlalign(minx);
    miny = xlalign(miny);
    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            NX(GNNODE(li)) -= minx;
            NY(GNNODE(li)) -= miny;
        }
    }

} /* shift_left_layout */



/*--------------------------------------------------------------------*/
/*  Shift the layout together                         */
/*--------------------------------------------------------------------*/

/*  Shift unconnected parts together
 *  --------------------------------
 *  It may happen that the graph is splitted into several connected parts,
 *  and that only the leftest part is shifted to the left upper part of
 *  the coordinate system, because dump shifting has moved the other
 *  parts to much to the right.
 *  This is now corrected here. We move the layout together such that
 *  the parts have at most a minimal x - distance G_xspace.
 *  In y - direction, this is not necessary.
 */

/*  graph_maxx is the maximal x-coordinate of the actual inspected
 *  graph.
 */

static int graph_maxx;

static void shift_left_together_layout(void)
{
    GNLIST li;
    int i;
    int    minx, part_is_missing;
    GNODE  node;

    debugmessage("shift_left_together_layout","");

    /* first, set the NMARK field to 0 */
    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            NMARK(GNNODE(li)) = 0;
        }
    }

    graph_maxx = G_xbase-G_xspace-5;

    part_is_missing = 1;
    while (part_is_missing) {

        part_is_missing =0;
        minx = MAXINT;
        node = (GNODE)0;
        /* look for an untouched connected component, i.e.
         * for the node with minimal x-co-ordinate.
         */
        for (i=0; i<=maxdepth+1; i++) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                if (NMARK(GNNODE(li)) == 0) {
                    if (minx > NX(GNNODE(li))) {
                        node = GNNODE(li);
                        minx = NX(node);
                    }
                    break;
                }
            }
        }
        if (node) {
            assert((NMARK(node)==0));
            part_is_missing =1;
            if (minx>graph_maxx+G_xspace+5) {
                i = minx-graph_maxx-G_xspace-5;
                i = xlalign(i);
                shift_left_part(node,i);
                shift_intermixed_part(i);
            }
            mark_and_calc_maxx(node);
        }
#ifdef DRAWDEBUG
#ifdef NEVER
        PRINTF("After one shift left together\n");
        PRINTF("Enter CR into the text window\n");
        step4_main();
        debug_display_part();
        fgetc(stdin);
#endif
#endif
    }
} /* shift_left_together_layout */


/* Search and shift left a subgraph that is intermixed with another
 * ----------------------------------------------------------------
 * E.g. the following situation:
 *
 *              A             We should consider this as one part, i.e.
 *             / \            the subgraph A-C,E must be shifted by the
 *            /   \           same amount as B,D-F.
 *        B  C  D  E
 *         \   /
 *          \ /
 *           F
 */

static void shift_intermixed_part(int t)
{
    int i, intermixed_part_found;
    GNLIST li;
    GNODE node;

    intermixed_part_found = 1;

    while (intermixed_part_found) {

        intermixed_part_found = 0;
        node = NULL;

        /* look for an untouched intermixed component, i.e.
         * for the untouched node whose successor is touched.
         */
        for (i=0; i<=maxdepth+1; i++) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                if (NMARK(GNNODE(li)) == 0) {
                    if (  (GNNEXT(li))
                        &&(NMARK(GNNODE(GNNEXT(li))))) {
                        node = GNNODE(li);
                        intermixed_part_found = 1;
                        break;
                    }
                }
            }
            if (intermixed_part_found) break;
        }

        if (intermixed_part_found) {
            assert((NMARK(node)==0));
            shift_left_part(node,t);
            mark_and_calc_maxx(node);
        }
    }
} /* shift_intermixed_part */



/* Shift left a connected part by i pixels
 * ---------------------------------------
 * this is done for all nodes reachable from node.
 */

static void shift_left_part(GNODE node, int i)
{
    GEDGE e;

    debugmessage("shift_left_part","");

    if (NMARK(node)) return;
    NMARK(node) = 1;

    NX(node) -= i;

    if (NCONNECT(node)) {
        if (CTARGET(NCONNECT(node)))
            shift_left_part(CTARGET(NCONNECT(node)), i);
        if (CTARGET2(NCONNECT(node)))
            shift_left_part(CTARGET2(NCONNECT(node)), i);
    }
    for (e = FirstSucc(node); e; e = NextSucc(e))
    {
        shift_left_part(ETARGET(e), i);
    }
    for (e = FirstPred(node); e; e = NextPred(e))
    {
        shift_left_part(ESOURCE(e), i);
    }
} /* shift_left_part */


/* Mark and update graph_maxx
 * --------------------------
 * this is done for all nodes reachable from node.
 */

static void mark_and_calc_maxx(GNODE node)
{
    GEDGE e;

    debugmessage("mark_and_calc_maxx","");

    if (NMARK(node)==2) return;
    NMARK(node) = 2;

    if (NX(node)+NWIDTH(node)>graph_maxx)
        graph_maxx = NX(node)+NWIDTH(node);

    if (NCONNECT(node)) {
        if (CTARGET(NCONNECT(node)))
            mark_and_calc_maxx(CTARGET(NCONNECT(node)));
        if (CTARGET2(NCONNECT(node)))
            mark_and_calc_maxx(CTARGET2(NCONNECT(node)));
    }
    for (e = FirstSucc(node); e; e = NextSucc(e))
    {
        mark_and_calc_maxx(ETARGET(e));
    }
    for (e = FirstPred(node); e; e = NextPred(e))
    {
        mark_and_calc_maxx(ESOURCE(e));
    }
} /* mark_and_calc_maxx */


/*--------------------------------------------------------------------*/
/*  Shift nearedges without priority together                 */
/*--------------------------------------------------------------------*/

/*  Shift parts only connected by near edges together
 *  -------------------------------------------------
 *  If we have near edge connections of priority 0, the graph tends
 *  to move apart of each other, if the parts are not connected by
 *  additional edges. In order to mprove this situation, we look
 *  for such nodes and determine, how much its connected part (without
 *  these edges) can be moved to the left. Therefore, we basically need
 *  the leftest node of the part at each level, and its distance to
 *  its left neighbor. The minimal distance gives the amount we can
 *  move.
 */

static void shift_connect_together_layout(void)
{
    GNLIST li;
    int i;
    int weight, found;
    GNODE  node;
    CONNECT c;

    debugmessage("shift_connect_together_layout","");

    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            weight = 0;
            node = GNNODE(li);
            c = NCONNECT(node);
            found = 0;
            if (c) {
                if (CTARGET(c)) {
                    if (NX(CTARGET(c))<NX(node)) {
                        weight = EPRIO(CEDGE(c));
                        weight *= layout_nearfactor;
                        if (weight==0) found = 1;
                    }
                }
                if (CTARGET2(c)) {
                    if (NX(CTARGET2(c))<NX(node)) {
                        weight = EPRIO(CEDGE2(c));
                        weight *= layout_nearfactor;
                        if (weight==0) found = 1;
                    }
                }
            }
            if (found) correct_connect_part(node);
        }
    }
} /* shift_connect_together_layout */


/*  Calculate the correction of the nearedge going left to v
 *  --------------------------------------------------------
 */

static void correct_connect_part(GNODE v)
{
    GNLIST  li;
    int i;
    int     mindist, found;

    debugmessage("correct_connect_part","");

    /* first, set the NMARK field to 0 */
    for (i=0; i<=maxdepth+1; i++) {
        TACTX(layer[i]) = MAXINT;
        TMINX(layer[i]) = MAXINT;
        TREFN(layer[i]) = NULL;
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            NMARK(GNNODE(li)) = 0;
        }
    }

    traverse_and_search_mindist(v);

    mindist = MAXINT;
    for (i=0; i<=maxdepth+1; i++) {
        if (TACTX(layer[i])<mindist) mindist = TACTX(layer[i]);
    }

    if ((mindist>0) && (mindist!=MAXINT)) {
        for (i=0; i<=maxdepth+1; i++) {
            found = 0;
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                if (GNNODE(li)==TREFN(layer[i])) found = 1;
                if (found) NX(GNNODE(li)) -= mindist;
            }
        }
    }
} /* correct_connect_part */


/*  Traverse the graph and calculate the minimal distance per layer
 *  ---------------------------------------------------------------
 */

static void traverse_and_search_mindist(GNODE v)
{
    int level, weight;
    GNLIST  li;
    GEDGE e;
    CONNECT c;

    debugmessage("traverse_and_search_mindist","");
    if (!v) return;
    if (NMARK(v)) return;
    NMARK(v) = 1;

    level = NTIEFE(v);
    if (NX(v) < TMINX(layer[level])) {
        TREFN(layer[level]) = v;
        TMINX(layer[level]) = NX(v);
        li = TPRED(layer[level]);
        while (li) {
            if (GNNODE(li)==v) break;
            li = GNNEXT(li);
        }
        assert((li));
        li = GNNEXT(li);
        if (li) {
            TACTX(layer[level]) = NX(v) - G_xspace
                - NX(GNNODE(li)) - NWIDTH(GNNODE(li));
        }
        else TACTX(layer[level]) = NX(v) - G_xbase;
    }
    c = NCONNECT(v);
    if (c) {
        if (CTARGET(c)) {
            if (NX(CTARGET(c))>NX(v)) weight = 1;
            else weight = EPRIO(CEDGE(c)) * layout_nearfactor;
            if (weight!=0)
                traverse_and_search_mindist(CTARGET(c));
        }
        if (CTARGET2(c)) {
            if (NX(CTARGET2(c))>NX(v)) weight = 1;
            else weight = EPRIO(CEDGE2(c)) * layout_nearfactor;
            if (weight!=0)
                traverse_and_search_mindist(CTARGET2(c));
        }
    }
    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        traverse_and_search_mindist(ETARGET(e));
    }
    for (e = FirstPred(v); e; e = NextPred(e))
    {
        traverse_and_search_mindist(ESOURCE(e));
    }
} /* traverse_and_search_mindist */





/*--------------------------------------------------------------------*/
/*  Dump medium shifts                                */
/*--------------------------------------------------------------------*/

/* Stretch the layout by medium shifts (dumping)
 * ===================================
 * This task is to stretch a dense layout into a reasonable nondense
 * layout. The most important effect is that the leftest and rightest
 * node of the most lower and most upper level are moved into useful
 * positions:
 * Example:
 *        ABC            A   B  C
 *      DEFGHIJK         DEFGHIJK
 *        LMNO     =>     L M N O
 *       PQRSTU           PQR STU
 *         VW              V   W
 *
 * We use the positions of the leftest and rightest node of the levels
 * as stop criterium of the iteration. The x-position of the leftest nodes
 * should decrease, the x-position of the rightest nodes should increase.
 * Thus the iteration stops, if no decrease or increase is recognizable.
 */

/* The leftest, rightest upper and lower nodes */

static int dumpfactor;
static int old_nw1 = MAXINT;
static int min_nw1 = MAXINT;
static int old_nw2 = MAXINT;
static int min_nw2 = MAXINT;
static int jump_done1 = 0;
static int jump_done2 = 0;

static void iterate_dump_mediumshifts(void)
{
    int count;
    int changed;
    int tryout;

    debugmessage("iterate_dump_mediumshifts","");

    /* initialize leftupper_node, rightupper_node, ... */

    old_nw1 = MAXINT;
    min_nw1 = MAXINT;
    old_nw2 = MAXINT;
    min_nw2 = MAXINT;
    jump_done1 = 0;
    jump_done2 = 0;

    count = 0;
    tryout = 2;
    dumpfactor = 2;
    while (1) {
        if (count%5==0) {
            gs_wait_message('m');
            shift_left_layout();
        }
        count++;
        drawdebug_show_graph("dump mediumshift",count,0,tryout);
        changed = dumpmediumshift(0);
        changed += dumpmediumshift(1);

        dumpfactor = 1;

        if ((!changed)&&(count>=min_mediumshifts)) break;
        if (count>=max_mediumshifts) {
            gs_wait_message('t');
            break;
        }
            if (G_timelimit>0)
                    if (test_timelimit(85)) {
                            gs_wait_message('t');
                            break;
                    }

        if (count>=min_mediumshifts) {
            if (!changed_nw_sum()) {
                tryout--;
                if (tryout==0) break;
            }
            else tryout = 2;
        }
    }
}


/* Check whether some position has changed
 * ----------------------------------------
 * return 1 if there is a leftest x-position of a level that has decreased,
 * or a rightest x-position of a level that has increased.
 */

static int changed_nw_sum(void)
{
    int i, nwval1, nwval2, k;
    int changed;
    GNLIST li;

    debugmessage("changed_nw_sum","");

    changed = nwval1 = nwval2 = 0;
    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            k = nws(GNNODE(li));
            if (k<0) k = -k;
            nwval1 += k;
            k = nwp(GNNODE(li));
            if (k<0) k = -k;
            nwval2 += k;
        }
    }

    if (nwval1<min_nw1) {
        changed = 1;
        old_nw1 = min_nw1 = nwval1;
        jump_done1 = 0;
    }
    else {
        if (nwval1<old_nw1) {
            changed = 1;
            old_nw1 = nwval1;
        }
        if (jump_done1 < 1) {
            old_nw1 = nwval1;
            jump_done1++;
        }
    }

    if (nwval2<min_nw2) {
        changed = 1;
        old_nw2 = min_nw2 = nwval2;
        jump_done2 = 0;
    }
    else {
        if (nwval2<old_nw2) {
            changed = 1;
            old_nw2 = nwval2;
        }
        if (jump_done2 < 1) {
            old_nw2 = nwval2;
            jump_done2++;
        }
    }

#ifdef DRAWDEBUG
    printf("Graph Weight: minimal: %d %d old: %d %d actual: %d %d\n",
            min_nw1, min_nw2, old_nw1, old_nw2, nwval1, nwval2);
#endif
    return(changed);
}



/* Apply a sequence of dumpmedium_shifts to all layers
 * ---------------------------------------------------
 * dumpnwpdump_mediumshift and nwsdump_mediumshift is applied to all layers.
 * Warning: Iterations of this routine need not to be convergent !!!
 * We can go into a endless loop, if we do
 *     while (changed) changed = dumpmediumshift();
 *
 * This is done initially, to stretch the layers such that they
 * become nearly the width of the most dense layer.
 */

static int dumpmediumshift(int first)
{
    int i;
    int layer_changed;
    int dir;

    debugmessage("dumpmediumshift","");

    layer_changed = 0;
    dir = 0;

    if (first) {
        for (i=1; i<=maxdepth+1; i++) {
            dir = 1-dir;
            layer_changed += nwpdump_mediumshift(i,dir);
            dir = 1-dir;
            layer_changed += nwpdump_mediumshift(i,dir);
        }
        if (prio_phase==1) {
            for (i=0; i<=maxdepth+1; i++)
                layer_changed += correct_priority_nodes(i);
        }
    }
    for (i=maxdepth; i>=0; i--) {
        dir = 1-dir;
        layer_changed += nwsdump_mediumshift(i,dir);
        dir = 1-dir;
        layer_changed += nwsdump_mediumshift(i,dir);
    }
    if (prio_phase==1) {
        for (i=0; i<=maxdepth+1; i++)
            layer_changed += correct_priority_nodes(i);
    }
    if (!first) {
        for (i=1; i<=maxdepth+1; i++) {
            dir = 1-dir;
            layer_changed += nwpdump_mediumshift(i,dir);
            dir = 1-dir;
            layer_changed += nwpdump_mediumshift(i,dir);
        }
        if (prio_phase==1) {
            for (i=0; i<=maxdepth+1; i++)
                layer_changed += correct_priority_nodes(i);
        }
    }

    if (nwdumping_phase) {
        for (i=maxdepth; i>=0; i--) {
            dir = 1-dir;
            layer_changed += nwdump_mediumshift(i,dir);
            dir = 1-dir;
            layer_changed += nwdump_mediumshift(i,dir);
        }
        if (prio_phase==1) {
            for (i=0; i<=maxdepth+1; i++)
                layer_changed += correct_priority_nodes(i);
        }
    }

    if (layer_changed) return(1);
    return(0);
}



/*--------------------------------------------------------------------*/
/*  Treatment of one layer: Dump pendling method                  */
/*--------------------------------------------------------------------*/


/*  High priority nodes
 *  -------------------
 */

static int is_fullprio_node(GNODE v)
{
    int nr_edges;
    int     pr;

    if (!v) return(0);
    /* if (NCONNECT(v)&&(layout_nearfactor!=0)) return(0); */

    pr = 1;
    nr_edges = get_node_succs_num(v);
    if (nr_edges>1) pr = 0;
    nr_edges = get_node_preds_num(v);
    if (nr_edges>1) pr = 0;
    return(pr);
}


/*  High successor priority nodes
 *  -----------------------------
 */

static int is_prio_snode(GNODE v)
{
    int nr_edges;

    if (!v) return(0);
    /* if (NCONNECT(v)&&(layout_nearfactor!=0)) return(0); */
    nr_edges = get_node_succs_num(v);
    return((nr_edges == 1));
}

/*  High predecressor priority nodes
 *  --------------------------------
 */

static int is_prio_pnode(GNODE v)
{
    int nr_edges;

    if (!v) return(0);
    /* if (NCONNECT(v)&&(layout_nearfactor!=0)) return(0); */
    nr_edges = get_node_preds_num(v);
    return((nr_edges == 1));
}



/*  Shift layer i by respecting its nws values:
 *  -------------------------------------------
 *  We try to shift the nodes by the nws-value itself.
 *  nws > 0 => shift to the right, nws < 0 => shift to the left
 *  The return value is 1, if something has changed.
 *
 *  Example:   nws=5                     nws=0
 *                N                        N (shifted 5 pixels to the right)
 *                |\         ==>           /\
 *          ews=0 | \ ews=10       ews=-5 /  \ ews=5
 *                X  Y                   X    Y
 *
 *  The difference to nws_mediumshift is, that we respect neighbor constraints.
 *  If a neighbour block must be shifted by n pixels, then we shift the nodes
 *  even if their nws values do not need a shift.
 *
 *  Example:  Y  X  Z   K           Y X Z  K   (K is shifted even if nws(K)=0)
 *             \_ \ |   |     ==>   | | |  |
 *               \_\|   |            \|/  /
 *                  N   M             N   M
 */

static int nwsdump_mediumshift(int i, int dir)
{
    GNLIST  li;
    int j;
    int     sign;

    debugmessage("nwsdump_mediumshift","");

    assert((i<=maxdepth));

    save_plevel(i);

    /* calculate shift weights */

    j = 0;
    sign = 1;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        levelshift[j] = nws(GNNODE(li));
        if ((sign<0) && (levelshift[j]>=0)) levelweight[j]=1;
        else levelweight[j]= MAXINT;
        if (levelshift[j]<0) sign = -1; else sign = 1;
        j++;
    }
    levelweight[0]= 1;

    if (prio_phase==1) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            NHIGHPRIO(GNNODE(li)) = is_prio_snode(GNNODE(li));
        }
        return(priosummarize_dumpshift(i,dir));
    }

    return(summarize_dumpshift(i,dir));
}

/*  Shift layer i by respecting its nwp values:
 *  -------------------------------------------
 *  We try to shift the nodes by the nwp-value itself.
 *  nwp > 0 => shift to the right, nwp < 0 => shift to the left
 *  The return value is 1, if something has changed.
 *
 *  Example:      X  Y                 X      Y
 *                |  /                  \    /
 *                | /        ==>   ewp=-5\  /ewp=5
 *          ewp=0 |/ewp=10                \/
 *                N                       N   (shifted 5 pixels to the right)
 *              nwp=5                   nwp=0
 *
 *  The difference to nws_mediumshift is, that we respect neighbor constraints.
 *  If a neighbour block must be shifted by n pixels, then we shift the nodes
 *  even if their nws values do not need a shift.
 *
 *  Example:    __N   M             N   M
 *            _/ /|   |            /|\  \
 *           /  / |   |     ==>   | | |  |
 *          Y  X  Z   K           Y X Z  K   (K is shifted even if nwp(K)=0)
 */

static int nwpdump_mediumshift(int i, int dir)
{
    GNLIST  li;
    int j;
    int sign;

    debugmessage("nwpdump_mediumshift","");

    assert((i>0));

    save_plevel(i);

    /* calculate shift weights */

    j = 0;
    sign = 1;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        levelshift[j] = nwp(GNNODE(li));
        if ((sign<0) && (levelshift[j]>=0)) levelweight[j]=1;
        else levelweight[j]= MAXINT;
        if (levelshift[j]<0) sign = -1; else sign = 1;
        j++;
    }
    levelweight[0]= 1;

    if (prio_phase==1) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            NHIGHPRIO(GNNODE(li)) = is_prio_pnode(GNNODE(li));
        }
        return(priosummarize_dumpshift(i,dir));
    }

    return(summarize_dumpshift(i,dir));
}


/*  Shift layer i by respecting its nw values:
 *  ------------------------------------------
 *  We try to shift the nodes by the nw-value itself.
 *  nw > 0 => shift to the right, nw < 0 => shift to the left
 *  The return value is 1, if something has changed.
 *
 *  nw is (nearly) the sum of nws and nwp, i.e. we respect now the
 *  predecessors and the successors in the same step.
 *
 */

static int nwdump_mediumshift(int i, int dir)
{
    GNLIST  li, li1;
    int j;
    int     sign;
    GNODE lnode, node, rnode;

    debugmessage("nwdump_mediumshift","");

    assert((i<=maxdepth));

    save_plevel(i);

    /* calculate shift weights */

    j = 0;
    sign = 1;
    lnode = NULL;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        node = GNNODE(li);
        if (NWIDTH(node)==0) {
            rnode = NULL;
            for (li1 = GNNEXT(li); li1; li1 = GNNEXT(li1))
            {
                if (NWIDTH(GNNODE(li1))!=0) {
                    rnode = GNNODE(li1);
                    break;
                }
            }
            levelshift[j] = nwbend(node,lnode,rnode);
        }
        else {
            levelshift[j] = nw(node);
            lnode = node;
        }
        if ((sign<0) && (levelshift[j]>=0)) levelweight[j]=1;
        else levelweight[j]= MAXINT;
        if (levelshift[j]<0) sign = -1; else sign = 1;
        j++;
    }
    levelweight[0]= 1;

    return(summarize_dumpshift(i,dir));
}




/* Common part of all dump_mediumshifts
 * ------------------------------------
 */


#define touching(v,w)  (  (NX(v)+NWIDTH(v)+G_xspace >= NX(w)) \
                ||(NX(w)-NX(v)-NWIDTH(v)<=2*G_xraster))


static int summarize_dumpshift(int i, int dir)
{
    GNLIST  li;
    int j;
    int changed;
    int     oldpos,sum,nrnodes;
    GNODE   v,w;

    debugmessage("summarize_dumpshift","");

    /* First we look whether there is space between nodes on one
     * region. If yes, we must split the region at this point.
     */

    j = 0;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        v = GNNODE(li);
        if (GNNEXT(li)) {
            w = GNNODE(GNNEXT(li));
            if (!touching(v,w))
                levelweight[j+1]=1; /* space => new region */
        }
        j++;
    }

    /* Now, the level is partitioned into regions of directly neigboured
     * nodes. If we traverse levelweight, then levelweight[j]=1 indicates
     * a new region.
     * We calculate now the shift values of the regions and store them
     * at the region indicator levelweight[j]. Because the shift values
     * should be < MAXINT, we can still use them as region indicators.
     */

    changed = 1;
    while (changed) {
        changed = 0;

        j = 0;
        oldpos = -1;
        nrnodes = 1;  /* not needed, only for the compiler */
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            if (levelweight[j]!=MAXINT) {
                if (oldpos!= -1) levelweight[oldpos] = sum/nrnodes;
                sum = levelshift[j];
                nrnodes = 1;
                oldpos = j;
            }
            else { sum += levelshift[j]; nrnodes++; }
            j++;
        }
        if (oldpos!= -1) levelweight[oldpos] = sum/nrnodes;

        /* check whether two regions must be combined: This occurs
         * if both regions are neighboured and have a tendence in
         * the same direction.
         */

        j = 0;
        sum = 0;
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            if (levelweight[j]!=MAXINT) sum = levelweight[j];
            if (GNNEXT(li)&&(levelweight[j+1]!=MAXINT)) {
                v = GNNODE(li);
                w = GNNODE(GNNEXT(li));
                if (touching(v,w)) {
                    if (  (sum>=0)&&(levelweight[j+1]>=0)
                        &&(sum>=levelweight[j+1])) {
                        levelweight[j+1] = MAXINT;
                        changed = 1;
                    }
                    if (  (sum<0)&&(levelweight[j+1]<0)
                        &&(sum>=levelweight[j+1])) {
                        levelweight[j+1] = MAXINT;
                        changed = 1;
                    }
                    if (  (sum>0)&&(levelweight[j+1]<0)) {
                        levelweight[j+1] = MAXINT;
                        changed = 1;
                    }
                }
            }
            j++;
        }
    }

    /* Now, we have the final region partitioning, and levelweight
     * contains appropriate shift values. These must be moved to
     * levelshift now.
     */

    j = 0;
    assert((levelweight[0]!=MAXINT));
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        if (levelweight[j]!=MAXINT) sum = dumpfactor * levelweight[j];
        levelshift[j] = sum;
        levelweight[j]= 1;
        j++;
    }
    if (dir) {
        changed  = do_rightshifts(i);
        changed += do_leftshifts(i);
    }
    else {
        changed  = do_leftshifts(i);
        changed += do_rightshifts(i);
    }

    return(changed);
} /* summarize_dumpshift */




/* Common part of all prio_phase dump_mediumshifts
 * -----------------------------------------------
 * prio_dump mediumshifts force priority nodes to be moved such that
 * their nwp or nws value becomes 0. This means they are treated with
 * first priority. Hence a priority node with levelshift value 0 must
 * form its own single-node region, since otherwise the whole region is
 * blocked.
 */

static int priosummarize_dumpshift(int i, int dir)
{
    GNLIST  li;
    int j;
    int changed;
    int     oldpos,sum,nrnodes;
    int priosum, nrprionodes, lprio, rprio;
    GNODE   v,w;

    debugmessage("summarize_dumpshift","");

    /* First we look whether there is space between nodes on one
     * region. If yes, we must split the region at this point.
     */

    j = 0;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        v = GNNODE(li);
        if (GNNEXT(li)) {
            w = GNNODE(GNNEXT(li));
            if (!touching(v,w))
                levelweight[j+1]=1; /* space => new region */
        }
        j++;
    }

    /* Now, the level is partitioned into regions of directly neigboured
     * nodes. If we traverse levelweight, then levelweight[j]=1 indicates
     * a new region.
     * We calculate now the shift values of the regions and store them
     * at the region indicator levelweight[j]. Because the shift values
     * should be < MAXINT, we can still use them as region indicators.
     */

    changed = 1;
    while (changed) {
        changed = 0;

        /* High priority nodes with levelshift=0
         * form there own region.
         */
        j = 0;
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            if (NHIGHPRIO(GNNODE(li))&&(levelshift[j]==0)) {
                levelweight[j]   = 1;
                levelweight[j+1] = 1;
            }
            j++;
        }

        j = 0;
        oldpos = -1;
        nrnodes = 1;  /* not needed, only for the compiler */
        priosum = 0;
        lprio = 0;
        rprio = 0;
        nrprionodes = 0;
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            if (levelweight[j]!=MAXINT) {
                if (oldpos!= -1) {
                    levelweight[oldpos] = sum/nrnodes;
                    if (nrprionodes !=0) {
                        if (priosum>0) {
                            levelweight[oldpos]=lprio;
                        }
                        else {
                            levelweight[oldpos]=rprio;
                        }
                    }
                }
                lprio = 0;
                rprio = 0;
                priosum = 0;
                nrprionodes = 0;
                sum = 0;
                nrnodes = 0;
                oldpos = j;
            }
            if (NHIGHPRIO(GNNODE(li))) {
                priosum += levelshift[j];
                if (lprio==0) lprio = levelshift[j];
                rprio = levelshift[j];
                nrprionodes++;
            }
            sum += levelshift[j];
            nrnodes++;
            j++;
        }
        if (oldpos!= -1) levelweight[oldpos] = sum/nrnodes;

        /* check whether two regions must be combined: This occurs
         * if both regions are neighboured and have a tendence in
         * the same direction.
         */

        j = 0;
        sum = 0;
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            if (levelweight[j]!=MAXINT) sum = levelweight[j];
            if (GNNEXT(li)&&(levelweight[j+1]!=MAXINT)) {
                v = GNNODE(li);
                w = GNNODE(GNNEXT(li));
                if (   (touching(v,w))
                    && (!NHIGHPRIO(v)) && (!NHIGHPRIO(w))) {
                    if (  (sum>=0)&&(levelweight[j+1]>=0)
                        &&(sum>=levelweight[j+1])) {
                        levelweight[j+1] = MAXINT;
                        changed = 1;
                    }
                    if (  (sum<0)&&(levelweight[j+1]<0)
                        &&(sum>=levelweight[j+1])) {
                        levelweight[j+1] = MAXINT;
                        changed = 1;
                    }
                    if (  (sum>0)&&(levelweight[j+1]<0)) {
                        levelweight[j+1] = MAXINT;
                        changed = 1;
                    }
                }
            }
            j++;
        }
    }

    /* Now, we have the final region partitioning, and levelweight
     * contains appropriate shift values. These must be moved to
     * levelshift now.
     */

    j = 0;
    assert((levelweight[0]!=MAXINT));
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        if (levelweight[j]!=MAXINT) sum = dumpfactor * levelweight[j];
        levelshift[j] = sum;
        levelweight[j]= 1;
        j++;
    }
    if (dir) {
        changed  = do_rightshifts(i);
        changed += do_leftshifts(i);
    }
    else {
        changed  = do_leftshifts(i);
        changed += do_rightshifts(i);
    }

    return(changed);
} /* priosummarize_dumpshift */



static int correct_priority_nodes(int i)
{
    GNLIST  li;
    int j, k, changed;
    int     ldiff, rdiff;
    int smove, pmove;
    GNODE   v,w,oldv;
    int dist;

    debugmessage("correct_priority_nodes","");

    assert((i>=0));
#ifdef DRAWDEBUG
#ifdef NEVER
    PRINTF("Before correct priority nodes line %d\n", i);
    PRINTF("Enter CR into the text window\n");
    step4_main();
    debug_display_part();
    fgetc(stdin);
#endif
#endif

    save_plevel(i);
    k = 5;

    /* calculate shift weights */
    changed = 1;
    while (changed) {
    k--;
    if (k<0) break;
    changed = 0;
    j = 0;
    oldv = NULL;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        levelshift[j]  = 0;
        levelweight[j] = 1;
        v = GNNODE(li);
        rdiff = ldiff = MAXINT;
        if (GNNEXT(li)) {
            dist = G_xspace;
            if ((NWIDTH(v)==0) && (NHEIGHT(v)==0)) dist = G_dspace;
            w = GNNODE(GNNEXT(li));
            if ((NWIDTH(w)==0) && (NHEIGHT(w)==0)) dist = G_dspace;
            rdiff = NX(w) - (NX(v) + NWIDTH(v) + dist);
        }
        if (oldv) {
            dist = G_xspace;
            if ((NWIDTH(v)==0) && (NHEIGHT(v)==0)) dist = G_dspace;
            w = oldv;
            if ((NWIDTH(w)==0) && (NHEIGHT(w)==0)) dist = G_dspace;
            ldiff = NX(v) - (NX(w) + NWIDTH(w) + dist);
        }
        if (is_prio_snode(v) && is_prio_pnode(v)) {
            smove = nws(v);
            pmove = nwp(v);
            if ((pmove > 0) && (smove!=0) && (pmove<=rdiff)) {
                levelshift[j]  = pmove;
            }
            else if ((pmove < 0) && (smove!=0) && (-pmove<=ldiff)) {
                levelshift[j]  = pmove;
            }
            else if ((smove > 0) && (pmove!=0) && (smove<=rdiff)) {
                levelshift[j]  = smove;
            }
            else if ((smove < 0) && (pmove!=0) && (-smove<=ldiff)) {
                levelshift[j]  = smove;
            }
        }
        else if (is_prio_snode(v) && (FirstPred(v)==NULL)) {
            smove = nws(v);
            if ((smove > 0) && (smove<=rdiff)) {
                levelshift[j]  = smove;
            }
            else if ((smove < 0) && (-smove<=ldiff)) {
                levelshift[j]  = smove;
            }
        }
        else if (is_prio_pnode(v) && (FirstSucc(v)==NULL)) {
            pmove = nwp(v);
            if ((pmove > 0) && (pmove<=rdiff)) {
                levelshift[j]  = pmove;
            }
            else if ((pmove < 0) && (-pmove<=ldiff)) {
                levelshift[j]  = pmove;
            }
        }
        oldv = v;
        j++;
    }
    changed += do_rightshifts(i);
    changed += do_leftshifts(i);
    }

#ifdef DRAWDEBUG
#ifdef NEVER
    PRINTF("After correct priority nodes line %d\n", i);
    PRINTF("Enter CR into the text window\n");
    step4_main();
    debug_display_part();
    fgetc(stdin);
#endif
#endif

    return(changed);
}



/*--------------------------------------------------------------------*/
/*  Straight line fine tuning                             */
/*--------------------------------------------------------------------*/


static void straight_line_tuning(void)
{
    int i, count;
    int changed;
    GNLIST  li;

    debugmessage("straight_line_tuning","");

    count = 0;
    changed = 1;
    while (changed) {
        count++;
        gs_wait_message('S');
        if (count>max_straighttune) {
            gs_wait_message('t');
            return;
        }
        if (G_timelimit>0)
            if (test_timelimit(90)) {
                gs_wait_message('t');
                return;
            }
        changed = 0;
        for (i=0; i<=maxdepth+1; i++) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                if (is_fullprio_node(GNNODE(li))) {
                    changed += do_straight_line(GNNODE(li));
                }
            }
        }

    }

}

static GNODE get_pred(GNODE v)
{
    GEDGE e = FirstPred(v);
    GNODE res;

    if (e) res = ESOURCE(e);
    else res = NULL;
    return res;
}

static GNODE get_succ(GNODE v)
{
    GEDGE e = FirstSucc(v);
    GNODE res;

    if (e) res = ETARGET(e);
    else res = NULL;
    return res;
}

static int do_straight_line(GNODE v)
{
    GNODE   w, sw, tw, minw, maxw;
    int     sx, tx, sminx, smaxx, diff;
    int     possible, allzero, h2;
    int     changed;

    debugmessage("straight_line","");

    sminx = smaxx = sx = tx = NX(v) + NWIDTH(v)/2;
    sw = tw = minw = maxw = v;
    w = v;
    while (w && is_fullprio_node(w)) {
        TACTX(layer[NTIEFE(w)]) = NX(w);
        sw = w;
        sx = NX(w) + NWIDTH(w)/2;
        if (sx<sminx) { sminx = sx; minw = w; }
        if (sx>smaxx) { smaxx = sx; maxw = w; }
        w = get_pred(w);
    }
    w = v;
    while (w && is_fullprio_node(w)) {
        TACTX(layer[NTIEFE(w)]) = NX(w);
        tw = w;
        tx = NX(w) + NWIDTH(w)/2;
        if (tx<sminx) { sminx = tx; minw = w; }
        if (tx>smaxx) { smaxx = tx; maxw = w; }
        w = get_succ(w);
    }

    if (sw==tw) return(0);

    /*---------- first, try to set the line to sx ------------------*/

    possible  = full_straight_possible(sw, sx, 0);
    possible &= full_straight_possible(sw, sx, 1);

    if (possible) {
        allzero = 1;
        w = sw;
        while (w && is_fullprio_node(w)) {
            diff = sx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_pred(w);
        }
        w = sw;
        while (w && is_fullprio_node(w)) {
            diff = sx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_succ(w);
        }
        return(1-allzero);
    }


    /*---------- and now the same with tx --------------------------*/

    possible  = full_straight_possible(tw, tx, 0);
    possible &= full_straight_possible(tw, tx, 1);

    if (possible) {
        allzero = 1;
        w = tw;
        while (w && is_fullprio_node(w)) {
            diff = tx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_pred(w);
        }
        w = tw;
        while (w && is_fullprio_node(w)) {
            diff = tx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_succ(w);
        }
        return(1-allzero);
    }


    /*---------- and now the same with sminx --------------------------*/

    possible  = full_straight_possible(minw, sminx, 0);
    possible &= full_straight_possible(minw, sminx, 1);

    if (possible) {
        allzero = 1;
        w = minw;
        while (w && is_fullprio_node(w)) {
            diff = sminx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_pred(w);
        }
        w = minw;
        while (w && is_fullprio_node(w)) {
            diff = sminx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_succ(w);
        }
        return(1-allzero);
    }

    /*---------- and now the same with smaxx --------------------------*/

    possible  = full_straight_possible(maxw, smaxx, 0);
    possible &= full_straight_possible(maxw, smaxx, 1);

    if (possible) {
        allzero = 1;
        w = maxw;
        while (w && is_fullprio_node(w)) {
            diff = smaxx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_pred(w);
        }
        w = maxw;
        while (w && is_fullprio_node(w)) {
            diff = smaxx - NX(w) - NWIDTH(w)/2;
            h2 = NX(w)+diff;
            if (diff < 0) {
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            else if (diff > 0) {
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                NX(w) = h2;
                allzero = 0;
            }
            w = get_succ(w);
        }
        return(1-allzero);
    }

    /*---------- now, try to set the line partially to minx -------------*/

    straight_part(minw, sminx, 0);
    straight_part(minw, sminx, 1);

    /*---------- now, try to set the line partially to maxx -----------*/

    straight_part(maxw, smaxx, 0);
    straight_part(maxw, smaxx, 1);

    /*---------- now, try to set the line partially to tx -------------*/

    straight_part(tw, tx, 0);
    straight_part(tw, tx, 1);

    /*---------- now, try to set the line partially to sx -------------*/

    straight_part(sw, sx, 0);
    straight_part(sw, sx, 1);

    changed = 0;
    w = v;
    while (w && is_fullprio_node(w)) {
        if (TACTX(layer[NTIEFE(w)]) != NX(w)) changed = 1;
        w = get_pred(w);
    }
    w = v;
    while (w && is_fullprio_node(w)) {
        if (TACTX(layer[NTIEFE(w)]) != NX(w)) changed = 1;
        w = get_succ(w);
    }

    return(changed);
}



static int full_straight_possible(GNODE sw, int sxpos, int dir)
{
    GNODE   w, nw;
    GNLIST  li;
    int     diff;
    int     sxpos_possible, h1, h2;

    debugmessage("full_straight_possible","");

    sxpos_possible = 1;
    w = sw;
    while (w && is_fullprio_node(w)) {
        if (!sxpos_possible) break;
        diff = sxpos - NX(w) - NWIDTH(w)/2;
        h2 = NX(w)+diff;
        if (diff < 0) li = TPRED(layer[NTIEFE(w)]);
        else          li = TSUCC(layer[NTIEFE(w)]);
        while (li) {
            if (GNNODE(li)==w) break;
            li = GNNEXT(li);
        }
        assert((li));
        li = GNNEXT(li);
        if (diff < 0) {
            if (li) {
                nw = GNNODE(li);
                if ((NWIDTH(w)==0)||(NWIDTH(nw)==0))
                     h1 = NX(nw) + NWIDTH(nw) + G_dspace;
                else h1 = NX(nw) + NWIDTH(nw) + G_xspace;
                if (NWIDTH(w)==0)
                     h2 = dxlalign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                if (h2<h1) sxpos_possible = 0;
            }
        }
        else if (diff > 0) {
            if (li) {
                nw = GNNODE(li);
                if ((NWIDTH(w)==0)||(NWIDTH(nw)==0))
                     h1 = NX(nw) - G_dspace - NWIDTH(w);
                else h1 = NX(nw) - G_xspace - NWIDTH(w);
                if (NWIDTH(w)==0)
                     h2 = dxralign(h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
                if (h2>h1) sxpos_possible = 0;
            }
        }
        if (dir) {
            w = get_pred(w);
        } else {
            w = get_succ(w);
        }
    }
    return(sxpos_possible);
}



static void straight_part(GNODE sw, int sxpos, int dir)
{
    GNODE   w, nw;
    GNLIST  li;
    int     diff;
    int     sxpos_possible, h1, h2;

    debugmessage("straight_part","");

    sxpos_possible = 1;
    w = sw;
    while (w && is_fullprio_node(w)) {
        if (!sxpos_possible) break;
        diff = sxpos - NX(w) - NWIDTH(w)/2;
        h2 = NX(w)+diff;
        if (diff < 0) li = TPRED(layer[NTIEFE(w)]);
        else          li = TSUCC(layer[NTIEFE(w)]);
        while (li) {
            if (GNNODE(li)==w) break;
            li = GNNEXT(li);
        }
        assert((li));
        li = GNNEXT(li);
        if (diff < 0) {
            if (NWIDTH(w)==0)
                 h2 = dxlalign(h2);
            else h2 = xlalign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
            if (!li) { NX(w) = h2; }
            else {
                nw = GNNODE(li);
                if ((NWIDTH(w)==0)||(NWIDTH(nw)==0))
                     h1 = NX(nw) + NWIDTH(nw) + G_dspace;
                else h1 = NX(nw) + NWIDTH(nw) + G_xspace;
                if (h2>=h1) { NX(w) = h2; }
                else sxpos_possible = 0;
            }
        }
        else if (diff > 0) {
            if (NWIDTH(w)==0)
                 h2 = dxralign(h2);
            else h2 = xralign( h2+NWIDTH(w)/2)-NWIDTH(w)/2;
            if (!li) { NX(w) = h2;  }
            else {
                nw = GNNODE(li);
                if ((NWIDTH(w)==0)||(NWIDTH(nw)==0))
                     h1 = NX(nw) - G_dspace - NWIDTH(w);
                else h1 = NX(nw) - G_xspace - NWIDTH(w);
                if (h2<=h1) { NX(w) = h2; }
                else sxpos_possible = 0;
            }
        }
        if (dir) {
            w = get_pred(w);
        } else {
            w = get_succ(w);
        }
    }
}


/*--------------------------------------------------------------------*/
/*  Centering method                              */
/*--------------------------------------------------------------------*/

/*  If we have the leftest and rightest node of the upperst and
 *  the lowerst level in useful positions (not too dense, and pendled),
 *  we can try to minimize the constraints of a node.
 *  As constrains, we define:
 *     G = sum( ewp(predecessors) )  + sum ( ews(successors) )
 *
 *  To minimize this value, we successively move a node by
 *     nw(node) = G(node) / number(edges to n)
 */

static void iterate_centershifts(void)
{
    int i,count;
    int weight,h;
    int second_try;

    debugmessage("iterate_centershifts","");

    if (prio_phase==1) return;

    weight = center_weight();
    second_try = 2;
    count=0;
    while (1) {
        if (count%5==0) gs_wait_message('c');
        count++;
        if (count>=max_centershifts) {
            gs_wait_message('t');
            break;
        }
        if (G_timelimit>0)
            if (test_timelimit(90)) {
                gs_wait_message('t');
                break;
            }
        for (i=0; i<=maxdepth+1; i++) center_layer(i);
        for (i=maxdepth+1; i>=0; i--) center_layer(i);

        h = center_weight();
        drawdebug_show_graph("centershift",count,h,weight);

        if (count>=min_centershifts) {
            if (h<weight) {
                weight = h;
                second_try = 2;
            }
            else if (h==weight) {
                second_try--;
                if (second_try<=0) break;
            }
            else break;  /* should never occur */
        }
    }
}



/*  Calculate the summary weight of all layers
 *  ------------------------------------------
 *  This is  sum( abs(G(all nodes)) )
 */

static int center_weight(void)
{
    int i;
    GNLIST  li;
    GEDGE e;
    int weight,h;

    debugmessage("center_weight","");

    weight = 0;
    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            h = 0;
            for (e = FirstSucc(GNNODE(li)); e; e = NextSucc(e))
            {
                h += ews(e);
            }
            for (e = FirstPred(GNNODE(li)); e; e = NextPred(e))
            {
                h += ewp(e);
            }
            if (h<0) h = -h;
            weight += h;
        }
    }
    return(weight);
}


/*  Shift layer i by respecting its nw values:
 *  ------------------------------------------
 *  We try to shift the nodes by the nw-value itself.
 *  Example:
 *     A  B       A    B
 *      \ |        \  /
 *       \|         \/
 *        N    =>   N  (shifted to the left)
 *       /|         /\
 *      / |        /  \
 *     C  D       C    D
 */

static int center_layer(int i)
{
    GNLIST  li, li1;
    int j;
    int changed;
    int dir;
    GNODE   lnode, node, rnode;
    int is_rnode;

    debugmessage("center_layer","");

    save_plevel(i);

    /* calculate shift weights */

    j = 0;
    dir = 0;
    lnode = NULL;
    is_rnode = 0;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        node = GNNODE(li);
        if (NWIDTH(node)==0) {
            if (!is_rnode) {
                rnode = NULL;
                for (li1 = GNNEXT(li); li1; li1 = GNNEXT(li1))
                {
                    if (NWIDTH(GNNODE(li1))!=0) {
                        rnode = GNNODE(li1);
                        break;
                    }
                }
                is_rnode = 1;
            }
            levelshift[j] = nwbend(node,lnode,rnode);
        }
        else {
            levelshift[j] = nw(node);
            lnode = node;
            is_rnode = 0;
        }
        dir += levelshift[j];
        levelweight[j++]= 1;
    }
    if (dir>=0) {
        changed  = do_rightshifts(i);
        changed += do_leftshifts(i);
    }
    else {
        changed  = do_leftshifts(i);
        changed += do_rightshifts(i);
    }
    return(changed);
}


/*--------------------------------------------------------------------*/
/*  Layer-Shifts                              */
/*--------------------------------------------------------------------*/

/*  Leftshifts of a layer
 *  ---------------------
 *  The nodes of a layer i are in slayer_array. The negative values
 *  levelshift[j]/levelweight[j] give the numbers of
 *  pixels we want to shift. We start just at the left side
 *  of the level and shift to the left.
 *  The return value is 1, if something has changed.
 */
static int do_leftshifts(int i)
{
    int   j;
    int   diff;
    int   oldx;
    GNODE node,lnode;
    int   changed;

    assert((i>=0) && (i<=maxdepth+1));
    lnode = NULL;
    changed = 0;
    for (j=0; j<TANZ(layer[i]); j++) {
        if (levelweight[j]) diff = levelshift[j]/levelweight[j];
        else diff = 0;
        node  = slayer_array[j];
        if (diff<0) {
            oldx = NX(node);
            NX(node) = oldx + diff;
            if (lnode) {
                if ((NWIDTH(node)==0)||(NWIDTH(lnode)==0)) {
                    if (NX(node)<NX(lnode)
                            +NWIDTH(lnode)+G_dspace)
                        NX(node) = NX(lnode)
                            +NWIDTH(lnode)+G_dspace;
                }
                else {
                    if (NX(node)<NX(lnode)
                            +NWIDTH(lnode)+G_xspace)
                        NX(node) = NX(lnode)
                            +NWIDTH(lnode)+G_xspace;
                }
            }

            if ((NWIDTH(node)==0)&&(NHEIGHT(node)==0))
                NX(node) = dxralign(NX(node)+NWIDTH(node)/2) - NWIDTH(node)/2;
            else
                NX(node) =  xralign(NX(node)+NWIDTH(node)/2) - NWIDTH(node)/2;
            if (NX(node)<oldx) changed = 1;
        }
        lnode = node;
    }
    return(changed);
} /* do_leftshifts */

/*  Rightshifts of a layer
 *  ----------------------
 *  The nodes of a layer i are in slayer_array. The positive values
 *  levelshift[j]/levelweight[j] give the numbers of
 *  pixels we want to shift. We start just at the right side
 *  of the level and shift to the right.
 *  The return value is 1, if something has changed.
 */
static int do_rightshifts(int i)
{
    int   j;
    int   diff;
    int   oldx;
    GNODE node,rnode;
    int   changed;

    assert((i>=0) && (i<=maxdepth+1));
    rnode = NULL;
    changed = 0;
    for (j=TANZ(layer[i])-1; j>=0; j--) {
        if (levelweight[j]) diff = levelshift[j]/levelweight[j];
        else diff = 0;
        node  = slayer_array[j];
        if (diff>0) {
            oldx = NX(node);
            NX(node) = oldx + diff;
            if (rnode) {
                if ((NWIDTH(node)==0)||(NWIDTH(rnode)==0)) {
                    if (NX(node)+NWIDTH(node)+G_dspace
                            >NX(rnode))
                        NX(node) = NX(rnode)
                            -NWIDTH(node)-G_dspace;
                }
                else {
                    if (NX(node)+NWIDTH(node)+G_xspace
                            >NX(rnode))
                        NX(node) = NX(rnode)
                            -NWIDTH(node)-G_xspace;
                }
            }
            if ((NWIDTH(node)==0)&&(NHEIGHT(node)==0))
                NX(node) = dxlalign(NX(node)+NWIDTH(node)/2) - NWIDTH(node)/2;
            else
                NX(node) =  xlalign(NX(node)+NWIDTH(node)/2) - NWIDTH(node)/2;
            if (NX(node)>oldx) changed = 1;
        }
        rnode = node;
    }
    return(changed);
} /* do_rightshifts */



/*--------------------------------------------------------------------*/
/*  Calculation of layout weights                     */
/*--------------------------------------------------------------------*/

/* Edge wights ews and ewp
 * -----------------------
 *    Examples:
 *        /  here is ews<0     \  here is ews>0
 *       /   and     ewp>0      \ and     ewp<0
 *     |/_                      _\|
 */

static int ews(GEDGE edge)
{
    GNODE   start, ende;
    int     x1,x2;

    start = ESTART(edge);
    ende  = EEND(edge);
    x1 = NX(ende) + (NWIDTH(ende)*EWEIGHTP(edge)/(NWEIGHTP(ende)+1));
    x2 = NX(start) + (NWIDTH(start)*EWEIGHTS(edge)/(NWEIGHTS(start)+1));
    return(x1-x2);
}


static int ewp(GEDGE edge)
{
    GNODE   start, ende;
    int     x1, x2;

    ende  = EEND(edge);
    start = ESTART(edge);
    x1 = NX(ende) + (NWIDTH(ende)*EWEIGHTP(edge)/(NWEIGHTP(ende)+1));
    x2 = NX(start) + (NWIDTH(start)*EWEIGHTS(edge)/(NWEIGHTS(start)+1));
    return(x2-x1);
}


/* Node weights nws and nwp
 * ------------------------
 * nws(n) is the sum of ews(successor of n) / number successors
 * nwp(n) is the sum of ewp(predecessor of n) / number predecessors
 *
 * nws(n) < 0  => shift n to the left
 * nws(n) > 0  => shift n to the right
 * nwp(n) < 0  => shift n to the left
 * nwp(n) > 0  => shift n to the right
 */

static int nws(GNODE node)
{
    int     h;
    int weight;
    int nr_edges;
    GEDGE e;
    CONNECT c;

    weight = 0;
    nr_edges = 0;
    for (e = FirstSucc(node); e; e = NextSucc(e))
    {
        h = (EPRIO(e)*layout_downfactor) * ews(e);
        weight += h;
        nr_edges += (EPRIO(e)*layout_downfactor);
    }
    c = NCONNECT(node);
    if (c) {
        if (CTARGET(c)) {
            if (NX(CTARGET(c))<NX(node))
                weight -= (EPRIO(CEDGE(c))*layout_nearfactor *
                         (NX(node) - (NX(CTARGET(c))+NWIDTH(CTARGET(c))+G_xspace)));
            else
                weight += (EPRIO(CEDGE(c))*layout_nearfactor *
                         (NX(CTARGET(c)) - (NX(node)+NWIDTH(node)+G_xspace)));
            nr_edges += (EPRIO(CEDGE(c))*layout_nearfactor);
        }
        if (CTARGET2(c)) {
            if (NX(CTARGET2(c))<NX(node))
                weight -= (EPRIO(CEDGE2(c))*layout_nearfactor *
                        (NX(node) - (NX(CTARGET2(c))+NWIDTH(CTARGET2(c))+G_xspace)));
            else
                weight += (EPRIO(CEDGE2(c))*layout_nearfactor *
                        (NX(CTARGET2(c)) - (NX(node)+NWIDTH(node)+G_xspace)));
            nr_edges += (EPRIO(CEDGE2(c))*layout_nearfactor);
        }
    }

    if (nr_edges) return(weight/nr_edges);
    else          return(0);
} /* nws */


static int nwp(GNODE node)
{
    int     h;
    int weight;
    int nr_edges;
    GEDGE e;
    CONNECT c;

    weight = 0;
    nr_edges = 0;
    for (e = FirstPred(node); e; e = NextPred(e))
    {
        h = (EPRIO(e)*layout_upfactor) * ewp(e);
        weight += h;
        nr_edges += (EPRIO(e)*layout_upfactor);
    }
    c = NCONNECT(node);
    if (c) {
        if (CTARGET(c)) {
            if (NX(CTARGET(c))<NX(node))
                weight -= (EPRIO(CEDGE(c))*layout_nearfactor *
                         (NX(node) - (NX(CTARGET(c))+NWIDTH(CTARGET(c))+G_xspace)));
            else
                weight += (EPRIO(CEDGE(c))*layout_nearfactor *
                         (NX(CTARGET(c)) - (NX(node)+NWIDTH(node)+G_xspace)));
            nr_edges += (EPRIO(CEDGE(c))*layout_nearfactor);
        }
        if (CTARGET2(c)) {
            if (NX(CTARGET2(c))<NX(node))
                weight -= (EPRIO(CEDGE2(c))*layout_nearfactor *
                        (NX(node) - (NX(CTARGET2(c))+NWIDTH(CTARGET2(c))+G_xspace)));
            else
                weight += (EPRIO(CEDGE2(c))*layout_nearfactor *
                        (NX(CTARGET2(c)) - (NX(node)+NWIDTH(node)+G_xspace)));
            nr_edges += (EPRIO(CEDGE2(c))*layout_nearfactor);
        }
    }

    if (nr_edges) return(weight/nr_edges);
    else          return(0);
} /* nwp */


/* Node weights nw
 * ---------------
 * If we have multiple predecessors and successors:
 * nw(n) is the sum of ews(successor) + ewp(predecessors) / number edges
 * If we have 1 predecessor and 1 successors:
 * nw(n) is (dx2 * dy1 - dx1 * dy2) / (dy1+dy2) in
 *
 *              dx1  dx2
 *           A -----|--|
 *            \     |  |
 *             \    |  |dy1
 *              B'--B--|        nw(B) is the distance to shift B to B'.
 *               \     |
 *                \    |
 *                 \   |dy2
 *                  \  |
 *                   \ |
 *                     C
 *
 * nw(n) < 0  => shift n to the left
 * nw(n) > 0  => shift n to the right
 */

static int nw(GNODE node)
{
    int weight;
    int     dx1, dx2, dy1, dy2;
    int nr_edges,p1,p2;
    GEDGE   edge;
    GNODE   v;
    CONNECT c;
    GEDGE pred_edge = FirstPred(node);
    GEDGE succ_edge = FirstSucc(node);

    weight = 0;
    nr_edges = 0;
    if ( (layout_downfactor==1)&&(layout_upfactor==1)
         && succ_edge && !NextSucc(succ_edge)
         && pred_edge && !NextPred(pred_edge)
         && EPRIO(succ_edge)==EPRIO(pred_edge)
         && NCONNECT(node)==NULL )
    {
        edge = pred_edge;
        v    = ESTART(edge);
        p1 = NX(node)+(NWIDTH(node)*EWEIGHTP(edge)/(NWEIGHTP(node)+1));
        p2 = NX(v) + (NWIDTH(v)*EWEIGHTS(edge)/(NWEIGHTS(v)+1));
        dx1 = p1 - p2;

        edge = succ_edge;
        v    = EEND(edge);
        p1 = NX(node)+(NWIDTH(node)*EWEIGHTS(edge)/(NWEIGHTS(node)+1));
        p2 = NX(v) + (NWIDTH(v)*EWEIGHTP(edge)/(NWEIGHTP(v)+1));
        dx2 = p2 - p1;

        p1  = NY(node)+NHEIGHT(node)/2;
        p2  = NY(ESOURCE(pred_edge))+NHEIGHT(ESOURCE(pred_edge))/2;
        dy1 = p1 - p2;
        p2  = NY(ETARGET(succ_edge))+NHEIGHT(ETARGET(succ_edge))/2;
        dy2 = p2 - p1;
        weight = (dx2*dy1-dx1*dy2)/(dy1+dy2);
        return(weight);
    }

    for (edge = FirstSucc(node); edge; edge = NextSucc(edge))
    {
        weight += (ews(edge)*EPRIO(edge)*layout_downfactor);
        nr_edges += (EPRIO(edge)*layout_downfactor);
    }
    for (edge = FirstPred(node); edge; edge = NextPred(edge))
    {
        weight += (ewp(edge)*EPRIO(edge)*layout_upfactor);
        nr_edges += (EPRIO(edge)*layout_upfactor);
    }
    c = NCONNECT(node);
    if (c) {
        if (CTARGET(c)) {
            if (NX(CTARGET(c))<NX(node))
                weight -= (EPRIO(CEDGE(c))*layout_nearfactor *
                         (NX(node) - (NX(CTARGET(c))+NWIDTH(CTARGET(c))+G_xspace)));
            else
                weight += (EPRIO(CEDGE(c))*layout_nearfactor *
                         (NX(CTARGET(c)) - (NX(node)+NWIDTH(node)+G_xspace)));
            nr_edges += (EPRIO(CEDGE(c))*layout_nearfactor);
        }
        if (CTARGET2(c)) {
            if (NX(CTARGET2(c))<NX(node))
                weight -= (EPRIO(CEDGE2(c))*layout_nearfactor *
                        (NX(node) - (NX(CTARGET2(c))+NWIDTH(CTARGET2(c))+G_xspace)));
            else
                weight += (EPRIO(CEDGE2(c))*layout_nearfactor *
                        (NX(CTARGET2(c)) - (NX(node)+NWIDTH(node)+G_xspace)));
            nr_edges += (EPRIO(CEDGE2(c))*layout_nearfactor);
        }
    }


    if (nr_edges) return(weight/nr_edges);
    else          return(0);
} /* nw */


/* Node weights nw on dummy nodes
 * ------------------------------
 *
 *  This is the same as nw, except that we want to avoid bendings
 *  on dummy nodes in certain situations.
 *  If the direction of an edge changes completely, it is not
 *  critical, but if the direction is nearly the same, we must
 *  calculate the position on the node such that just no bending
 *  occurs. To simplify, we observe only the direct left and right
 *  neighbour of the dummy node.
 *
 *  Example: we want to avoid such situations:
 *
 *  (ax,ay)
 *        \           __(kx,ky)_
 *          \        |
 *         (mx,my)   |
 *            |      |
 *            |      |   k
 *             \     |
 *               \   |_______
 *                 \
 *                   \
 *                (bx,by)
 *
 *  The corrected situation is:
 *
 *  (ax,ay)
 *        \
 *          \       --(kx,ky)-
 *       (mx,my)   |
 *            \    |               where obviously holds:
 *             \   |   k
 *              \  |               (kx-mx)       ky+HEIGHT(k)-my
 *               \ |_______        -------   =   ---------------
 *                \                (bx-mx)       (by-my)
 *                 \
 *                (bx,by)
 *
 *
 *  nwbend(n) < 0  => shift n to the lefti, etc.
 */

static int nwbend(GNODE node, GNODE lnode, GNODE rnode)
{
    GNODE pred, succ;
    int ax,ay,mx,my,bx,by,kx,h, dist;
    int act_nw;

    act_nw = nw(node);

    if (FirstPred(node)) pred = ESOURCE(FirstPred(node));
    else return(act_nw);
    if (FirstSucc(node)) succ = ETARGET(FirstSucc(node));
    else return(act_nw);

    ax = NX(pred);
    ay = NY(pred);
    mx = NX(node);
    my = NY(node);
    bx = NX(succ);
    by = NY(succ);

    if ((ax<mx)&&(bx<mx)) return(act_nw);
    if ((ax>mx)&&(bx>mx)) return(act_nw);

    if (ax<bx) {
        if (rnode && act_nw>0) {
            kx = NX(rnode);
            h  = NY(rnode)+NHEIGHT(rnode)-my;
            if (h-by+my==0) return(act_nw);
            dist = (bx*h - kx*(by-my))/(h-by+my) - mx;
            if (dist<act_nw) act_nw = dist;
        }
        else if (lnode && act_nw<0) {
            kx = NX(lnode)+NWIDTH(lnode);
            h  = NY(lnode)-my;
            if (h-ay+my==0) return(act_nw);
            dist = (ax*h - kx*(ay-my))/(h-ay+my) - mx;
            if (dist>act_nw) act_nw = dist;
        }
    }
    else if (ax>bx) {
        if (lnode && act_nw<0) {
            kx = NX(lnode)+NWIDTH(lnode);
            h  = NY(lnode)+NHEIGHT(lnode)-my;
            if (h-by+my==0) return(act_nw);
            dist = (bx*h - kx*(by-my))/(h-by+my) - mx;
            if (dist>act_nw) act_nw = dist;
        }
        else if (rnode && act_nw>0) {
            kx = NX(rnode);
            h  = NY(rnode)-my;
            if (h-ay+my==0) return(act_nw);
            dist = (ax*h - kx*(ay-my))/(h-ay+my) - mx;
            if (dist<act_nw) act_nw = dist;
        }
    }

    return(act_nw);
}


/*--------------------------------------------------------------------*/
/*  Treatement of the save array                      */
/*--------------------------------------------------------------------*/


/*  Put level layer[i] into the slayer_array
 *  ----------------------------------------
 *  This is done to calculate levelshift and levelweight.
 *  Both arrays are initialized, further.
 */
static void save_plevel(int i)
{
    int j;
    GNLIST li;

    debugmessage("save_plevel","");
    j  = 0;
    for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
    {
        assert(NPOS(GNNODE(li)) == j);
        slayer_array[j]  = GNNODE(li);
        levelshift[j]    = 0;
        levelweight[j++] = 0;
    }
    assert(j==TANZ(layer[i]));
}


/*--------------------------------------------------------------------*/
/*  For debugging only                            */
/*--------------------------------------------------------------------*/

#ifdef DRAWDEBUG

static void drawdebug_show_graph(char *m, int i, int j, int k)
{
    PRINTF("%s %d (%d %d)\n",m,i,j,k);
    PRINTF("Enter CR into the text window\n");
    shift_left_layout();
    step4_main();
    debug_display_part();
    fgetc(stdin);
}

#endif

