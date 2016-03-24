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
 *        They are distributed at the layer[i] lists and connected by the
 *        TSUCC list to allow to traverse the nodes of one layer[i].
 *        TANZ(layer[i])= 0.
 *        The hierarchy in layer is proper.
 *    2)  Further, all visible nodes are in nodelist, labellist and dummylist.
 *    3)  All pot. visible edges are in the lists edgelist or tmpedgelist.
 *        Visible edges can be detected by the EINVISIBLE flag (==0) in these
 *        lists. Note: invisible edges may also be in edgelist or tmpedgelist.
 *        An edge is visible iff
 *                 a) it is used in the adjacency lists.
 *              or b) it is a direct neigbour edge in NCONNECT(v) for
 *                    some node v.
 *    4)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    5)  NTIEFE(node) is filled for all nodes.
 *        NCONNECT(node) is filled for nodes that have direct neighbours
 *        in the layout.
 *    7)  Reverted edges are marked with EART(e)='R'.
 *        Self loops don't anymore exist.
 *
 * The task here is to calculate the (x,y) coordinates of a tree
 * by a specialized layout algorithm.
 * Crossing reduction is not necessary, because the layout algoritm
 * implies no crossings.
 * After that, the layout must be prepared to enter step4_main, i.e.
 * the TPRED lists must be created, the nodes must be sorted according
 * to their position within the levels, etc.
 *
 * At the end, the situation is this:
 *    1)  The array layer contains all visible nodes.
 *        They are distributed at the layer[i] lists and connected by two
 *        lists TPRED and TSUCC to allow to traverse the nodes of one
 *        layer[i] backwards and forwards.
 *    2)  Note that the nodes reacheable via forward connections are now
 *        in the TSUCC and TPRED lists, too.
 *        TANZ(layer[i]) is the number of nodes in layer[i].
 *    3)  The hierarchy in layer is proper.
 *    4)  nodelist, labellist and dummylist are not changed.
 *    5)  All pot. visible edges are in the lists edgelist or tmpedgelist,
 *        same as before.
 *    6)  (deleted)
 *    7)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    8)  NTIEFE(node) is filled for all nodes.
 *        NCONNECT(node) is filled as before.
 *    9)  Reverted edges are marked with EART(e)='R'.
 *        Self loops don't anymore exist.
 *    10) NPOS(v) gives the horizontal position of a node inside the
 *        layer. Adjacency edges are sorted according to these NPOS
 *        values. The ordering inside a layer is such that the number
 *        of cossings is 0.
 *    11) NSUCCL(v) and NSUCCR(v) are the leftest and rightest successor
 *        edge of v, and NPREDL(v) and NPREDR(v) the leftest and rightest
 *        predecessor edge.
 *    12) All nodes have filled NX, NY, NWIDTH and NHEIGHT such that
 *        they do not overlap. NX and NY are absolutely. NWIDTH and
 *        NHEIGHT are stretched or shrinked according to the local
 *        factors at the nodes.
 *
 *
 * This file provides the following functions:
 * ------------------------------------------
 * tree_main        Main routine to produce the tree layout.
 *
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
#include "graph.h"


/* Prototypes
 * ----------
 */

static int  is_tree     _PP((void));
static int  is_shared   _PP((GNODE v));
static void     create_tpred_lists  _PP((void));
static void     sort_tsucc_and_tpred    _PP((void));
static  int     compare_xpos        _PP((const GNODE *a, const GNODE *b));

static void tree_layout     _PP((void));
static int  find_position       _PP((GNODE v,int l));

static void     correct_position    _PP((GNODE conn,GNODE v));
static int  min_x_in_tree       _PP((GNODE v,GNODE conn));
static int  max_x_in_tree       _PP((GNODE v,int depth));
static void     correct_xpos        _PP((GNODE v,int diff));


/* Global variables
 * ----------------
 */

/* array where we can temporary sort the nodes of one level.
 */

static GNODE    *sort_array = NULL;

static int tree_factor1;
static int tree_factor2;

/*--------------------------------------------------------------------*/
/*  Tree Layout                                                       */
/*--------------------------------------------------------------------*/

/*   Main entry point of tree layout
 *   -------------------------------
 *   returns TREE_LAYOUT on success, otherwise 0.
 */

int tree_main(void)
{
    debugmessage("tree_main","");
    assert((layer));

    if (tree_factor<0) tree_factor = -tree_factor;
    if (tree_factor>1.0) {
        tree_factor1 = 10;
        tree_factor2 = (int)(10.0/tree_factor);
    }
    else {
        tree_factor1 = (int)(tree_factor*10.0);
        tree_factor2 = 10;
    }
    if ((tree_factor1==0)||(tree_factor2==0)) {
        tree_factor = 0.5;
        tree_factor1 = 1;
        tree_factor2 = 2;
    }

    gs_wait_message('T');

    /* Check whether it is a forest of trees */

    if (!is_tree()) {
        return(0);
    }

    /* Create the TPRED lists
     * This also initializes max_nodes_per_layer.
     */

    create_tpred_lists();

    /* Calculate width and height of the nodes
     */

    calc_all_node_sizes();

    /* Allocate the sort_array and the tpred_connections.
     */

    alloc_levelshift();  /* This allocates tpred_connections. */
    sort_array = (GNODE *)tpred_connection1;

    /* Layout the tree */

    tree_layout();

    /*  Order the nodes within the levels according to their
     *  X-coordinate. This initializes also NPOS.
     */

    sort_tsucc_and_tpred();

    /*  Order the adjacency lists of the nodes and initialize
     *  NPREDL, NPREDR, NSUCCL, NSUCCR.
     */

    gs_wait_message('T');
    sort_all_adjacencies();
    calc_all_ports(1);

    return(TREE_LAYOUT);
} /* tree_main */




/*--------------------------------------------------------------------*/
/*  Check whether it is a forest of tree's                            */
/*--------------------------------------------------------------------*/

/* Tree check main function
 * ------------------------
 * return 1 if it is a downward tree.
 */
static int is_tree(void)
{
    int i;
    int result;
    int fresh;

    GNODE v;
    GNLIST li;
    CONNECT c;

    debugmessage("is_tree","");

    for (v = nodelist; v; v = NNEXT(v)) { NMARK(v) = 1; }
    for (v = labellist; v; v = NNEXT(v)) { NMARK(v) = 1; }
    for (v = dummylist; v; v = NNEXT(v)) { NMARK(v) = 1; }

    /* Check whether a node is shared */

    for (i=0; i<= maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            v = GNNODE(li);
            fresh = NMARK(v);
            c = NCONNECT(v);
            if (c) {
                if (backward_connection1(c)) fresh = 0;
                if (backward_connection2(c)) fresh = 0;
            }
            result = 0;
            if (fresh) result = is_shared(v);
            if (result) return(0);
        }
    }

    return(1);
}


/* Traverse a tree by dfs and check whether a node is shared
 * ---------------------------------------------------------
 * returns 1 if a node is shared.
 */
static int is_shared(GNODE v)
{
    GEDGE e;
    CONNECT c;

    if (!NMARK(v)) return(1);

    NMARK(v) = 0;

    if (FirstPred(v) && NextPred(FirstPred(v)))
    {
        return(1);
    }

    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        if (is_shared(ETARGET(e))) return(1);
    }

    c = NCONNECT(v);
    if (c) {
        if (forward_connection1(c)) {
            if (is_shared(EEND(CEDGE(c)))) return(1);
        }
        if (forward_connection2(c)) {
            if (is_shared(EEND(CEDGE2(c)))) return(1);
        }
    }
    return(0);
}


/*--------------------------------------------------------------------*/
/*  Create the TPRED lists of the layers                              */
/*--------------------------------------------------------------------*/

static void create_tpred_lists(void)
{
    int i, k;
    GNLIST h1, h2;

    debugmessage("create_tpred_lists","");

    max_nodes_per_layer = 0;
    for (i=0; i<=maxdepth+1; i++) {
        TPRED(layer[i]) = NULL;
        k = 0;
        for (h1 = TSUCC(layer[i]); h1; h1 = GNNEXT(h1))
        {
            k++;
            TPRED(layer[i]) = cons_node_tmp(GNNODE(h1), TPRED(layer[i]));
        }
        TANZ(layer[i]) = k;
        if (k>max_nodes_per_layer) max_nodes_per_layer = k;
    }
}


/*--------------------------------------------------------------------*/
/*  Sort the layers according to the X-Postion                        */
/*--------------------------------------------------------------------*/

/* This initializes also NPOS.
 */
static void sort_tsucc_and_tpred(void)
{
    int i, k, max;
    GNLIST h1;

    debugmessage("sort_tsucc_and_tpred","");

    for (i=0; i<=maxdepth+1; i++) {
        k = 0;
        for (h1 = TSUCC(layer[i]); h1; h1 = GNNEXT(h1))
        {
            sort_array[k++] = GNNODE(h1);
        }
        max = k;

        if (k) {
            qsort(sort_array, k, sizeof(GNODE),
                (int (*) (const void *, const void *))compare_xpos);
        }

        k = 0;
        for (h1 = TSUCC(layer[i]); h1; h1 = GNNEXT(h1))
        {
            NPOS(sort_array[k]) = k;
            GNNODE(h1) = sort_array[k++];
        }
        assert((k==max));
        k--;
        for (h1 = TPRED(layer[i]); h1; h1 = GNNEXT(h1))
        {
            GNNODE(h1) = sort_array[k--];
        }
        assert((k== -1));
    }
} /* sort_tsucc_and_tpred */



/*  Compare function for sorting according NX
 *  -----------------------------------------
 *  returns 1 if NX(*a) > NX(*b), 0 if equal, -1 otherwise.
 */
static int compare_xpos(const GNODE *a, const GNODE *b)
{
    if (NX(*a) > NX(*b)) return(1);
    if (NX(*a) < NX(*b)) return(-1);
    return(0);
}


/*--------------------------------------------------------------------*/
/*  Sort adjacency lists                                              */
/*--------------------------------------------------------------------*/

/* Sort the adjacency lists
 * ------------------------
 */
void sort_all_adjacencies(void)
{
    int i;
    GNLIST li;
    GEDGE e;

    debugmessage("sort_all_adjacencies","");

    for (i=0; i<=maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            if (i == 0)          assert(!FirstPred(GNNODE(li)));
            if (i == maxdepth+1) assert(!FirstSucc(GNNODE(li)));
            for (e = FirstPred(GNNODE(li)); e; e = NextPred(e))
            {
                relink_node_edge_as_last(ESOURCE(e), e, GD_SUCC);
            }
            for (e = FirstSucc(GNNODE(li)); e; e = NextSucc(e))
            {
                relink_node_edge_as_last(ETARGET(e), e, GD_PRED);
            }
        }
    }
} /* sort_all_adjacencies */


/*--------------------------------------------------------------------*/
/*  The real tree layout algorithm                                    */
/*--------------------------------------------------------------------*/

static int horder_warn; /* 1 if the horder-warning was already printed */

#define xralign(a)  ((((a)+G_xraster-1)/G_xraster)*G_xraster)
#define xlalign(a)  ((((a)            )/G_xraster)*G_xraster)
#define dxralign(a) ((((a)+G_dxraster-1)/G_dxraster)*G_dxraster)
#define yralign(a)  ((((a)+G_yraster-1)/G_yraster)*G_yraster)


static void tree_layout(void)
{
    int actypos;
    int maxboxheight;
    int i, fresh;
    GNLIST li;
    GNODE v;
    CONNECT c;

    debugmessage("tree_layout","");

    horder_warn = 0;

    /* First, check the layout factors */

    if (G_yspace<5)  G_yspace=5;
    if (G_xspace<5)  G_xspace=5;
    if (G_dspace==0) {
        if (G_spline) G_dspace = 4*G_xspace/5;
        else          G_dspace = G_xspace/2;
    }

    if (G_flat_factor<1)   G_flat_factor = 1;
    if (G_flat_factor>100) G_flat_factor = 100;

    /* Now calculate the y positions */

    actypos = yralign(G_ybase);

    for (i=0; i<=maxdepth+1; i++) {
        TACTX(layer[i]) = G_xbase;
        maxboxheight = 0;
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            NY(GNNODE(li))   = actypos;
            if (maxboxheight < NHEIGHT(GNNODE(li)))
                maxboxheight = NHEIGHT(GNNODE(li));
        }
        if (G_yalign==AL_CENTER) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                NY(GNNODE(li)) += (maxboxheight-
                              NHEIGHT(GNNODE(li)))/2;
            }
        }
        else if (G_yalign==AL_BOTTOM) {
            for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
            {
                NY(GNNODE(li)) += (maxboxheight-
                              NHEIGHT(GNNODE(li)));
            }
        }
        actypos += (maxboxheight + G_yspace);
        actypos = yralign(actypos);
    }

    for (v = nodelist; v; v = NNEXT(v)) { NMARK(v) = 1; }
    for (v = labellist; v; v = NNEXT(v)) { NMARK(v) = 1; }
    for (v = dummylist; v; v = NNEXT(v)) { NMARK(v) = 1; }

    /* Now the real tree layout */

    for (i=0; i<= maxdepth+1; i++) {
        for (li = TSUCC(layer[i]); li; li = GNNEXT(li))
        {
            v = GNNODE(li);
            fresh = NMARK(v);
            c = NCONNECT(v);
            if (c) {
                if (backward_connection1(c)) fresh = 0;
                if (backward_connection2(c)) fresh = 0;
            }
            if (fresh) {
                gs_wait_message('T');
                (void)find_position(v, G_xbase+NWIDTH(v)/2);
            }
        }
    }
} /* tree_layout */


/* Find position of a node
 * -----------------------
 * leftest_pos is the leftest position allowed as center of the node.
 * It returns the center of the positioned node.
 */
static int find_position(GNODE v, int leftest_pos)
{
    int xpos, minpos, maxpos, l, num, i;
    GEDGE e;
    int minhorder, maxhorder;
    GNODE w, actl, actr, actn;
    GNODE conn1, conn2, leftconn;
    CONNECT c;
    int outdeg = get_node_succs_num(v);

    debugmessage("find_position","");

    if (leftest_pos<0) leftest_pos = 0;
    if (!v) return(leftest_pos);
    xpos = leftest_pos;

    NMARK(v) = 0;
    if ((NHORDER(v)!= -1)&&(!horder_warn)) {
        horder_warn = 1;
        if (!silent) {
            FPRINTF(stderr,"Note: On tree layout ");
            FPRINTF(stderr,"the attribute `horizontal_order' ");
            FPRINTF(stderr,"sorts only the child nodes\n");
            FPRINTF(stderr,"of a node locally, ");
            FPRINTF(stderr,"but not the whole layer.");
            FPRINTF(stderr,"\n");
        }
    }

    l = NTIEFE(v);

    conn1 = conn2 = 0;
    c = NCONNECT(v);
    if (c) {
        if (forward_connection1(c)) { conn1 = EEND(CEDGE(c)); }
        if (forward_connection2(c)) { conn2 = EEND(CEDGE2(c)); }
    }


    if (conn1 && conn2) {

        if (NHORDER(conn1)== -1) {
            if (NHORDER(conn2)<NHORDER(v))
                find_position(conn2,
                   xpos-NWIDTH(v)/2-NWIDTH(conn2)/2-G_xspace);
            else
                find_position(conn1,
                   xpos-NWIDTH(v)/2-NWIDTH(conn1)/2-G_xspace);
        }
        else if (NHORDER(conn1)==NHORDER(v)) {
            if (NHORDER(conn2)<NHORDER(v))
                find_position(conn2,
                   xpos-NWIDTH(v)/2-NWIDTH(conn2)/2-G_xspace);
            else
                find_position(conn1,
                   xpos-NWIDTH(v)/2-NWIDTH(conn1)/2-G_xspace);
        }
        else if (NHORDER(conn1)<NHORDER(v)) {
            find_position(conn1,
                   xpos-NWIDTH(v)/2-NWIDTH(conn1)/2-G_xspace);
        }
        else {
            if (NHORDER(conn2)<NHORDER(conn1))
                find_position(conn2,
                   xpos-NWIDTH(v)/2-NWIDTH(conn2)/2-G_xspace);
            else
                find_position(conn1,
                   xpos-NWIDTH(v)/2-NWIDTH(conn1)/2-G_xspace);
        }

    }
    else if (conn1) {
        if (NHORDER(conn1)!= -1) {
            if (NHORDER(conn1)<NHORDER(v)) {
                find_position(conn1,
                   xpos-NWIDTH(v)/2-NWIDTH(conn1)/2-G_xspace);
            }
        }
    }
    else if (conn2) {
        if (NHORDER(conn2)!= -1) {
            if (NHORDER(conn2)<NHORDER(v)) {
                find_position(conn2,
                   xpos-NWIDTH(v)/2-NWIDTH(conn2)/2-G_xspace);
            }
        }
    }

    xpos = leftest_pos;
    if (xpos-NWIDTH(v)/2 < TACTX(layer[l]))
        xpos = TACTX(layer[l]) + NWIDTH(v)/2;

    switch (outdeg) {
    case 0:
        break;
    case 1:
        e = FirstSucc(v);
        assert(e);
        minpos = find_position(ETARGET(e), xpos);
        xpos = minpos;
        break;
    default:
        actl = NULL;
        minhorder = MAXINT;
        for (e = FirstSucc(v); e; e = NextSucc(e))
        {
            w = ETARGET(e);
            if (NHORDER(w)<minhorder) {
                minhorder = NHORDER(w);
                actl = w;
            }
        }

        actr = NULL;
        maxhorder = MININT;
        for (e = FirstSucc(v); e; e = NextSucc(e))
        {
            w = ETARGET(e);
            if ((w!=actl) && (NHORDER(w)>maxhorder)) {
                maxhorder = NHORDER(w);
                actr = w;
            }
        }

        assert((actl)&&(actr)&&(actl!=actr));

        num = 0;
        for (e = FirstSucc(v); e; e = NextSucc(e))
        {
            num += NWIDTH(ETARGET(e));
        }
        num += ((outdeg-1) * G_xspace);
        num -= NWIDTH(actl)/2;
        num -= NWIDTH(actr)/2;

        minpos = find_position(actl, xpos - (num*tree_factor1)/tree_factor2);

        actn = NULL;
        minhorder = MAXINT;
        for (e = FirstSucc(v); e; e = NextSucc(e))
        {
            w = ETARGET(e);
            if ((w!=actr) && (NMARK(w)) && (NHORDER(w)<minhorder)) {
                minhorder = NHORDER(w);
                actn = w;
            }
        }

        i = 2;
        while (actn) {
            if (l>spread_level)
                maxpos = find_position(actn,
                    minpos + (2*(i-1)*(xpos-minpos))/
                        (outdeg-1));
            else {
                if (i<=(outdeg+1)/2)
                    maxpos = find_position(actn,
                       minpos + (2*(i-1)*(xpos-minpos))/
                        (outdeg-1));
                else
                    maxpos = find_position(actn, xpos);
            }
            if (l>spread_level) {
                xpos = minpos + (outdeg-1)*(maxpos-minpos)/
                        (2*(i-1));
                if (xpos<leftest_pos) xpos = leftest_pos;
            }
            i++;
            actn = NULL;
            minhorder = MAXINT;
            for (e = FirstSucc(v); e; e = NextSucc(e))
            {
                w = ETARGET(e);
                if (   (w!=actr) && (NMARK(w))
                    && (NHORDER(w)<minhorder)) {
                    minhorder = NHORDER(w);
                    actn = w;
                }
            }

        }
        if (l>spread_level)
            maxpos = find_position(actr,
                minpos + (2*(i-1)*(xpos-minpos))/
                    (outdeg-1));
        else {
            if (i<=(outdeg+1)/2)
                maxpos = find_position(actr,
                   minpos + (2*(i-1)*(xpos-minpos))/
                    (outdeg-1));
            else
                maxpos = find_position(actr, xpos);
        }
        if (l>spread_level) {
            xpos = minpos + (outdeg-1)*(maxpos-minpos)/
                    (2*(i-1));
            if (xpos<leftest_pos) xpos = leftest_pos;
        }

        xpos = (minpos+maxpos)/2;
        if (xpos-NWIDTH(v)/2 < TACTX(layer[l]))
            xpos = TACTX(layer[l]) + NWIDTH(v)/2;
    }

    NX(v) = xpos - NWIDTH(v)/2;

    if ((NWIDTH(v)==0) && (NHEIGHT(v)==0))
        NX(v) = dxralign(NX(v)+NWIDTH(v)/2) - NWIDTH(v)/2;
    else
        NX(v) = xralign(NX(v) +NWIDTH(v)/2) - NWIDTH(v)/2;
    TACTX(layer[l]) = NX(v) + NWIDTH(v) + G_xspace;

    leftconn = 0;
    if (conn1 && (!NMARK(conn1))) leftconn = conn1;
    if (conn2 && (!NMARK(conn2))) leftconn = conn2;

    if (conn1 && (NMARK(conn1))) (void)find_position(conn1,TACTX(layer[l]));
    if (conn2 && (NMARK(conn2))) (void)find_position(conn2,TACTX(layer[l]));

    if (leftconn) correct_position(leftconn,v);

    return(xpos);
} /* find_position */


/* Correct the position of a left neighbor node
 * --------------------------------------------
 * If the tree at v is wide, the left neighbor node conn is far away
 * from v. To avoid this situation, we calculate how much the tree with
 * root conn can be shifted to the right. This works only, if the tree
 * at conn has a smaller depth than the tree at v, since otherwise
 * the shifting to the right would influence the TACTX-values which
 * indicate the leftest available position.
 */

#define TMINX(x) TCROSS(x)

static void correct_position(GNODE conn, GNODE v)
{
    int i, depth, diff;
    debugmessage("correct_position","");

    gs_wait_message('T');
    assert((NX(v)>NX(conn)));
    for (i=0; i<=maxdepth+1; i++)
        TMINX(layer[i]) = MAXINT;

    depth = min_x_in_tree(v, conn);

    diff  = max_x_in_tree(conn, depth);

    if (diff-G_xspace>0) correct_xpos(conn, xlalign(diff-G_xspace));
}



/* Traverse the tree v except conn and calculate the minimal x-positions
 * ---------------------------------------------------------------------
 * The minimal x-positions at each level are stored in TMINX of the levels.
 * We return the maximal depth of the tree.
 */
static int min_x_in_tree(GNODE v, GNODE conn)
{
    int maxlevel, l, h;
    GEDGE e;
    CONNECT c;

    debugmessage("min_x_in_tree","");

    if (v==conn) return(0);

    maxlevel = l = NTIEFE(v);
    if (NX(v) < TMINX(layer[l])) TMINX(layer[l]) = NX(v);

    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        h = min_x_in_tree(ETARGET(e), conn);
        if (h>maxlevel) maxlevel = h;
    }

    c = NCONNECT(v);
    if (c) {
        if (forward_connection1(c)) {
            h = min_x_in_tree(EEND(CEDGE(c)), conn);
            if (h>maxlevel) maxlevel = h;
        }
        if (forward_connection2(c)) {
            h = min_x_in_tree(EEND(CEDGE2(c)), conn);
            if (h>maxlevel) maxlevel = h;
        }
    }
    return(maxlevel);
}


/* Traverse the tree v and calculate the maximal x-positions
 * ---------------------------------------------------------
 * We compare the maximal x-positions with the minimal x-positions
 * of the right neighbor tree (which are in TMINX) and return the
 * minimal difference.
 * If the tree at v is deeper than the right neigbour tree, we return 0.
 */
static int max_x_in_tree(GNODE v, int depth)
{
    int mindiff, l, h;
    GEDGE e;
    CONNECT c;

    debugmessage("max_x_in_tree","");

    l = NTIEFE(v);
    if (l>depth) return(0);
    mindiff = TMINX(layer[l]) - NX(v) - NWIDTH(v);

    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        h = max_x_in_tree(ETARGET(e), depth);
        if (h<mindiff) mindiff = h;
    }

    c = NCONNECT(v);
    if (c) {
        if (forward_connection1(c)) {
            h = max_x_in_tree(EEND(CEDGE(c)), depth);
            if (h<mindiff) mindiff = h;
        }
        if (forward_connection2(c)) {
            h = max_x_in_tree(EEND(CEDGE2(c)), depth);
            if (h<mindiff) mindiff = h;
        }
    }
    return(mindiff);
}


/* Correct the x-position of the tree at v by diff
 * -----------------------------------------------
 */
static void correct_xpos(GNODE v, int diff)
{
    GEDGE e;
    CONNECT c;

    debugmessage("correc_xpos","");

    NX(v) += diff;

    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        correct_xpos(ETARGET(e), diff);
    }

    c = NCONNECT(v);
    if (c) {
        if (forward_connection1(c)) {
            correct_xpos(EEND(CEDGE(c)), diff);
        }
        if (forward_connection2(c)) {
            correct_xpos(EEND(CEDGE2(c)), diff);
        }
    }
}

