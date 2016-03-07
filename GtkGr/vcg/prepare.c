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
 * We are not in the layout phase, because all nodes have locations.
 * However, not all nodes have correct width and height, and the edges
 * have no position nor orientation.
 * This is calculated here.
 *
 * We have:
 *  1) We have proper adjacency lists.
 *  2) All visible nodes are in the nodelist or in the labellist.
 *     More exactly: all visible nodes originated directly by the
 *     specification are in the nodelist, and all visible edge label nodes
 *     are in the labellist.
 *  3) All nodes from the nodelist that are invisible because of
 *     edge class hiding are in the list invis_nodes.
 *  4) All potentially visible edges are in the lists edgelist or tmpedgelist.
 *     Visible edges can be detected by the EINVISIBLE flag (==0) in these
 *     lists. Note: invisible edges may also be in edgelist or tmpedgelist.
 *     Except the INVISIBLE flag, the edgelist IS NEVER CHANGED !!!
 *  5) An edge is visible iff it is used in the adjacency lists. For some
 *     edges, we create substeds; then, the substed is visible but the original
 *     edge is not visible.
 *  6) The locFlag is 1, if all visible nodes have positions (x,y).
 *
 * We calculate NINDEG and NOUTDEG of every node, reset width and height
 * of nodes, sort outgoing edges according to the gradient, calculate
 * ports and anchorpoints, and assign co-ordinates to the edges.
 *
 * After that, the following invariants hold:
 *    1)  NINDEG, NOUTDEG, NWIDTH and NHEIGHT are proper filled for nodes.
 *        maxindeg and maxoutdeg are upper estimations of NINDEG and
 *    NOUTDEG of nodes.
 *    2)  The adjacency lists NPRED(v) and the connection fields
 *        NCONNECT(v) contain all visible nodes.
 *    3)  Reverted edges are marked with EART(e)='R'.
 *    Edges to the left are marked with EART(e)='l', and to
 *    the right with EART(e)='r'.
 *        Self loops or double edges don't anymore exist.
 *    4)  All nodes have filled NX, NY, NWIDTH and NHEIGHT.
 *        NX and NY are absolutely.
 *        NWIDTH and NHEIGHT are stretched or shrinked according to the
 *        local factors at the nodes.
 *    5)  For all visible edges, ESTARTX(e), ESTARTY(e), EENDX(e),
 *        EENDY(e), ETBENDX(e), ETBENDY(e), EBBENDX(e), EBBENDY(e)are filled.
 *    6)  EORI(e) is filled. If the edge has two arrows, EORI2(e)
 *        is filled, too.
 *
 * This file provides the following functions:
 * ------------------------------------------
 * prepare_nodes    Preparation of all nodes
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
#include "drawlib.h"
#include "steps.h"
#include "timing.h"
#include "graph.h"

/* Prototypes
 * ----------
 */

static void     calc_node_degree    _PP((GNODE v));
static void     calc_node_anchor    _PP((GNODE v));
static void     sort_adjacencies    _PP((GNODE v));
static int  compare_ppos        _PP((const GEDGE *a,const GEDGE *b));
static int  compare_spos        _PP((const GEDGE *a,const GEDGE *b));


/* Global variables
 * ----------------
 */

/* arrays where we can temporary sort the adjacency lists.
 */

static GEDGE    *adjarray2 = NULL;
static int      size_of_adjarray = 0;

/*--------------------------------------------------------------------*/
/*  Preparation on fixed layout of nodes                              */
/*--------------------------------------------------------------------*/

void prepare_nodes(void)
{
    int i,h,hh, xp, na;
    GNODE v,w;
    GEDGE e;

    start_time();
    debugmessage("prepare_nodes","");

    /* First, calculate width and height of each node */
    maxindeg = maxoutdeg = 0;
    for (v = nodelist; v; v = NNEXT(v)) { calc_node_size(v); }
    for (v = labellist; v; v = NNEXT(v)) { calc_node_size(v); }

    /* prepare back edges, i.e. revert all back edges initially
     */

    prepare_back_edges();

    /*  insert anchor edges: we blindly set the anchor node
     *  G_xspace pixels right or left of the anchored node.
     */

    insert_anchor_edges();
    for (v = dummylist; v; v = NNEXT(v))
    {
        if (NANCHORNODE(v)) {
            assert(NCONNECT(v));
            w = CTARGET(NCONNECT(v));
            NY(v) = NY(w);
            NHEIGHT(v) = NHEIGHT(w);
            xp = 0;
            na = 0;
            h = 0;
            for (e = FirstPred(v); e; e = NextPred(e))
            {
                h++;
                na++;
                xp += NX(ESOURCE(e));
            }
            hh = 0;
            for (e = FirstSucc(v); e; e = NextSucc(e))
            {
                hh++;
                na++;
                xp += NX(ETARGET(e));
            }
            if (hh>h) h = hh;
            NWIDTH(v) =  (h+1)*G_xspace;
            if (xp/na >= NX(w)+NWIDTH(w))
                NX(v) = NX(w)+NWIDTH(w)+G_xspace;
            else
                NX(v) = NX(w)-NWIDTH(v)-G_xspace;
        }
    }

    /* Now, check the anchor points */
    maxindeg = maxoutdeg = 0;
    for (v = nodelist; v; v = NNEXT(v)) { calc_node_anchor(v); }
    for (v = labellist; v; v = NNEXT(v)) { calc_node_anchor(v); }
    for (v = dummylist; v; v = NNEXT(v)) { calc_node_anchor(v); }

    /* Then, calculate the node in/outdegree */
    for (v = nodelist; v; v = NNEXT(v)) { calc_node_degree(v); }
    for (v = labellist; v; v = NNEXT(v)) { calc_node_degree(v); }
    for (v = dummylist; v; v = NNEXT(v)) { calc_node_degree(v); }

        i = (maxindeg > maxoutdeg ? maxindeg : maxoutdeg);
        if (i+2 > size_of_adjarray) {
                if (adjarray2) free(adjarray2);
                adjarray2 = (GEDGE *)libc_malloc((i+2)*sizeof(GEDGE));
                size_of_adjarray = i+2;
#ifdef DEBUG
                PRINTF("Sizeof table `adjarray2': %d Bytes\n",
                        (i+2)*sizeof(GEDGE));
#endif
        }


    /* Now, sort the adjacency lists */
    for (v = nodelist; v; v = NNEXT(v)) { sort_adjacencies(v); }
    for (v = labellist; v; v = NNEXT(v)) { sort_adjacencies(v); }
    for (v = dummylist; v; v = NNEXT(v)) { sort_adjacencies(v); }

    /* Now, calculate the node ports */
    for (v = nodelist; v; v = NNEXT(v)) { calc_node_ports(v,1); }
    for (v = labellist; v; v = NNEXT(v)) { calc_node_ports(v,1); }
    for (v = dummylist; v; v = NNEXT(v)) { calc_node_ports(v,1); }

    /* Now, calculate the node ports */
    for (v = nodelist; v; v = NNEXT(v)) { calc_edge_xy(v); }
    for (v = labellist; v; v = NNEXT(v)) { calc_edge_xy(v); }
    for (v = dummylist; v; v = NNEXT(v)) { calc_edge_xy(v); }

    /* Now, calculate the arrow orientation */
    for (v = nodelist; v; v = NNEXT(v)) { calc_edgearrow(v); }
    for (v = dummylist; v; v = NNEXT(v)) { calc_edgearrow(v); }
    /* Labels and dummy nodes have no arrows */

    /* calculate maximal x-y position
     */
    calc_max_xy_pos();

    stop_time("prepare_nodes");
    debugmessage("end of prepare_nodes","");
} /* prepare_nodes */



/* Set NINDEG and NOUTDEG of the node
 * ----------------------------------
 */

static void calc_node_degree(GNODE v)
{
    int k;

    k = get_node_succs_num(v);
    if (k>maxoutdeg) maxoutdeg = k;

    k = get_node_preds_num(v);
    if (k>maxindeg) maxindeg = k;
} /* calc_node_degree */


/* Set NWIDTH and NHEIGHT of the node
 * ----------------------------------
 * If NWIDTH and NHEIGHT are not already set, they are derived from
 * the size of the label.
 */

void calc_node_size(GNODE v)
{
    debugmessage("calc_node_size","");

    if ((NWIDTH(v) == -1)||(NHEIGHT(v) == -1)) {
        gs_setshrink(NSTRETCH(v),NSHRINK(v));
        switch (NSHAPE(v)) {
        case RHOMB:    gs_calcrhombsize(v);    break;
        case TRIANGLE: gs_calctrianglesize(v); break;
        case ELLIPSE:  gs_calcellipsesize(v);  break;
        default:    gs_calctextboxsize(v);
        }
    }
    if (NWIDTH(v)  == -1) NWIDTH(v)   = gs_boxw;
    if (NHEIGHT(v) == -1) NHEIGHT(v)  = gs_boxh;
} /* calc_node_size */


/* Calculate anchor point
 * ----------------------
 * Further are edges reverted, if they are upwards, and the gradient
 * is calculated.
 * EWEIGHTP(e) is used to contain the gradient of incoming edges,
 * and EWEIGHTS(e) is used to contain the gradient of outgoing
 * edges. Note that edges with anchor left-on have gradient MININT, and
 * edges with anchor right-on have gradient MAXINT.
 */

static void calc_node_anchor(GNODE v)
{
    GEDGE e, nxt_e;
    int   x1,y1,x2,y2;

    debugmessage("calc_node_anchor","");
    for (e = FirstSucc(v); e; e = nxt_e)
    {
        nxt_e = NextSucc(e);
        x1 = NX(ESOURCE(e))+NWIDTH( ESOURCE(e))/2;
        y1 = NY(ESOURCE(e))+NHEIGHT(ESOURCE(e))/2;
        x2 = NX(ETARGET(e))+NWIDTH( ETARGET(e))/2;
        y2 = NY(ETARGET(e))+NHEIGHT(ETARGET(e))/2;
        if (NY(ESOURCE(e))>NY(ETARGET(e))+NHEIGHT(ETARGET(e))) {
            (void)revert_edge(e);
            if (y1==y2) EWEIGHTS(e) = MININT;
            else        EWEIGHTS(e) = 1000*(x1-x2)/(y1-y2);
            if (y1==y2) EWEIGHTP(e) = MAXINT;
            else        EWEIGHTP(e) = -1000*(x1-x2)/(y1-y2);
        }
        else if (NY(ESOURCE(e))+NHEIGHT(ESOURCE(e))<NY(ETARGET(e))) {
            if (y1==y2) EWEIGHTS(e) = MININT;
            else        EWEIGHTS(e) = 1000*(x1-x2)/(y1-y2);
            if (y1==y2) EWEIGHTP(e) = MAXINT;
            else        EWEIGHTP(e) = -1000*(x1-x2)/(y1-y2);
        }
        else if (NX(ESOURCE(e))>NX(ETARGET(e))+NWIDTH(ETARGET(e))) {
            EWEIGHTS(e) = MININT;
            EWEIGHTP(e) = MAXINT;
            /* 'l' и 'r' не должны перетирать инфу о том, что дуга обращена */
            assert(EART(e) != 'R');
            EART(e) = 'l';
        }
        else if (NX(ESOURCE(e))+NWIDTH(ESOURCE(e))<NX(ETARGET(e))) {
            EWEIGHTS(e) = MAXINT;
            EWEIGHTP(e) = MININT;
            /* 'l' и 'r' не должны перетирать инфу о том, что дуга обращена */
            assert(EART(e) != 'R');
            EART(e) = 'r';
        }
        else delete_adjedge(e); /* Edge is not drawable */
    }
} /* calc_node_anchor */

/* Sort the adjacency lists of node v
 * ----------------------------------
 * This gives the layout the final touch.
 */

static void sort_adjacencies(GNODE v)
{
        int i, len;
    GEDGE e;

        debugmessage("sort_adjacencies","");
        assert((v));

    i = 0;
    for (e = FirstPred(v); e; e = NextPred(e))
    {
                adjarray2[i++] = e;
        }
    len = i;

        qsort(adjarray2, len, sizeof(GEDGE),
        (int (*) (const void *, const void *))compare_ppos);
    for ( i = 0;
        i < len;
        i++ )
    {
        e = adjarray2[i];
        if (i > 0) {
            EADJPREV(e,GD_PRED) = adjarray2[i-1];
        } else {
            EADJPREV(e,GD_PRED) = NULL;
            NADJFIRST(v,GD_PRED) = e;
        }
        if (i < len-1) {
            EADJNEXT(e,GD_PRED) = adjarray2[i+1];
        } else {
            EADJNEXT(e,GD_PRED) = NULL;
            NADJLAST(v,GD_PRED) = e;
        }
    }

    i = 0;
    for (e = FirstSucc(v); e; e = NextSucc(e))
    {
        adjarray2[i++] = e;
    }
    len = i;

        qsort(adjarray2, len, sizeof(GEDGE),
        (int (*) (const void *, const void *))compare_spos);
    for ( i = 0;
        i < len;
        i++ )
    {
        e = adjarray2[i];
        if (i > 0) {
            EADJPREV(e,GD_SUCC) = adjarray2[i-1];
        } else {
            EADJPREV(e,GD_SUCC) = NULL;
            NADJFIRST(v,GD_SUCC) = e;
        }
        if (i < len-1) {
            EADJNEXT(e,GD_SUCC) = adjarray2[i+1];
        } else {
            EADJNEXT(e,GD_SUCC) = NULL;
            NADJLAST(v,GD_SUCC) = e;
        }
    }
} /* sort_adjacencies */


/*  Compare functions for sort_adjedges
 *  ------------------------------------
 *  returns 1 if EWEIGHT(*a) > EWEIGHT(*b), 0 if equal, -1 otherwise
 */

static int compare_ppos(const GEDGE *a, const GEDGE *b)
{
        if (EWEIGHTP(*a) > EWEIGHTP(*b))        return(1);
        if (EWEIGHTP(*a) < EWEIGHTP(*b))        return(-1);
        return(0);
}

static int compare_spos(const GEDGE *a,const GEDGE *b)
{
        if (EWEIGHTS(*a) > EWEIGHTS(*b))        return(1);
        if (EWEIGHTS(*a) < EWEIGHTS(*b))        return(-1);
        return(0);
}


