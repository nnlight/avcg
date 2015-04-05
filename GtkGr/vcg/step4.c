/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         step4.c                                            */
/*   version:      1.00.00                                            */
/*   creation:     14.4.93                                            */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Layout phase 4: calculation of coordinates         */
/*                 of edges                                           */
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
 *
 *  The software is available per anonymous ftp at ftp.cs.uni-sb.de.
 *  Contact  sander@cs.uni-sb.de  for additional information.
 */



/************************************************************************
 * The situation here is the following:
 * -----------------------------------
 * We are still in the layout phase of the graph. The graph is in
 * adjacency list representation available, further it is partitioned
 * into layers that represent a proper hierarchy. All nodes have already
 * their relative position inside the layers and their co-ordinates.
 * We want to calculate now the co-ordinated of the edges.
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
 *    12) All nodes have filled NX, NY, NWIDTH and NHEIGHT such that
 *	  they do not overlap. NX and NY are absolutely. NWIDTH and
 *	  NHEIGHT are stretched or shrinked according to the local
 *	  factors at the nodes.
 *
 * We summarize all subsequential edges that point to the same node into
 * ports. This has the advantage that we need only to draw one arrow
 * per port, and the arrows of incoming edges have different locations
 * than the outgoing edges. This improves the readability. Then we
 * assign xy-co-ordinates to the edges. If an edge goes though a very 
 * large neigbour node, we need to bend the edges. We calculate the
 * bendpoints. At last, we calculate the orientation of the arrows.
 * After this, we have the following situation:
 *    1-12) same as before
 *    13)   For all visible edges, ESTARTX(e), ESTARTY(e), EENDX(e),
 *          EENDY(e), ETBENDX(e), ETBENDY(e), EBBENDX(e), EBBENDY(x) 
 *          are filled.
 *    14)   EORI(e) is filled. If the edge has two arrows, EORI2(e)
 *          is filled, too.
 *
 * This file provides the following functions:
 * ------------------------------------------
 * step4_main           Main routine to calculate edge co-ordinates
 * calc_node_ports      Calculate the ports where the edges come
 *  			in or out 
 * calc_edge_xy         Calculate the coordinates of edges
 * calc_edgearrow       Calculate the orientation of edges
 * calc_max_xy_pos	Calculate the maximal x/y-positions in the
 *			layout.
 * statistics           Calculate a statistic of the layout (not needed
 *			in this step, but later, see menu point statistics). 
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

/* Prototypes
 * ----------
 */

static void 	calc_manhatten		_PP((void));
static void 	evaluate_row_indicators _PP((int le,int mr,int miy,int may));
static int 	fill_row_indicators	_PP((int level));
static void 	finish_upper		_PP((GNODE v, int xpos));
static void 	finish_lower		_PP((GNODE v, int xpos));
static void 	delete_upper		_PP((DLLIST x));
static void 	delete_lower		_PP((DLLIST x));
static void	append_to_upper		_PP((GEDGE e, GNODE n));
static void	append_to_lower		_PP((GEDGE e, GNODE n));
static void 	calc_all_edge_xy	_PP((void));
static void 	calc_all_bendpoints	_PP((void));
static void 	tune_dummy_bendings	_PP((void));

static int 	calc_topbendpoint	_PP((GNLIST li));
static int 	calc_botbendpoint	_PP((GNLIST li));
static int 	calc_edgetopbendpoint	_PP((GEDGE e,GNLIST li));
static int 	calc_edgebotbendpoint	_PP((GEDGE e,GNLIST li));

static int  	set_topbendpoint	_PP((GNLIST li, int bendp));
static int  	set_botbendpoint	_PP((GNLIST li, int bendp));

static void	calc_all_edgearrows	_PP((void));
static void 	check_horizontal	_PP((GEDGE e));
static void 	check_up_port		_PP((ADJEDGE a));
static void 	check_down_port		_PP((ADJEDGE a));

static void 	flip_mirror		_PP((void));
static void 	flip_all_nodes		_PP((GNODE v)); 
static void 	flip_edge		_PP((GEDGE e)); 
static void 	flip_ver_mirror		_PP((void));
static void 	flip_ver_all_nodes	_PP((GNODE v)); 
static void 	flip_ver_edge		_PP((GEDGE e)); 

/*--------------------------------------------------------------------*/
/*  Calculation of co-ordinates of edges                              */
/*--------------------------------------------------------------------*/

void	step4_main(void)
{
        start_time();
	debugmessage("step4_main","");
	assert((layer));

	/* Now, we assign the xy-coordinates to the edges. 
	 */
	calc_all_edge_xy();

	if (manhatten_edges!=1) {
		/* Then, we check whether the edges of a layer must be bend.
		 * To stay as readeable as possible, we bend all edges at the
		 * same bending point if one edge of a layer must be bend.
		 */
		calc_all_bendpoints();
	
		/* On dummy nodes, bendpoints are very ugly. Thus, we change
		 * the coordinates of an edge to a dummy node where a
		 * bended edge starts, and of this edge, too.
		 */
		tune_dummy_bendings(); 
	}
	else {
		/* For manhatten layout, the edges are bend to form a grid.
		 * All edges must be bend to consists of either horizontal
		 * or vertical segments. This is a special form of bend point
		 * calculation.
		 */
		calc_manhatten();
	}

	/* At last, we check calculate the orientation of the edge
	 * arrows.
	 */
	calc_all_edgearrows();

	/* flip all coordinates, if the orientation is not top_to_bottom
	 */
	if (  (G_orientation==BOTTOM_TO_TOP)
	    ||(G_orientation==RIGHT_TO_LEFT)) flip_ver_mirror();
	if (  (G_orientation==LEFT_TO_RIGHT)
	    ||(G_orientation==RIGHT_TO_LEFT)) flip_mirror();

	/* calculate maximal x-y position
	 */
	calc_max_xy_pos();

        stop_time("step4_main");

}


/*--------------------------------------------------------------------*/
/*  Calculation of ports                                              */
/*--------------------------------------------------------------------*/

/*  Nodes have potentially many ports, i.e. anchor points for the
 *  edges. We allow:
 *             1  anchorpoint  to the direct left  neighbour
 *             1  anchorpoint  to the direct right neighbour
 *	     many anchorpoints to upper nodes 
 *	     many anchorpoints to lower nodes 
 *  Hereby, we summarize all anchorpoints that have edges in the same
 *  direction. E.g. here we have 4 anchorpoints. 
 *              
 *         --------------------        --------------------
 *        |                    |  =>  |                    |
 *         --------------------        --------------------
 *          ^ ^ ^ | | ^ ^ ^ |            ^    /\    ^    |
 *          | | | v v | | | V           /|\  |  |  /|\   |
 *                                     | | | v  v | | |  v
 */


/* Calculate all node ports of all nodes 
 * -------------------------------------
 *   NWEIGHTS and NWEIGHTP become the number of ports of a node 
 *   EWEIGHTS and EWEIGHTP become the port number of an edge 
 */

void	calc_all_ports(int xypos_avail)
{
	GNODE   v;

	debugmessage("calc_all_ports","");

	v = nodelist;
        while (v) {
		calc_node_ports(v,xypos_avail);
                v = NNEXT(v);
        }
        v = labellist;
        while (v) {
		calc_node_ports(v,xypos_avail);
                v = NNEXT(v);
        }
        v = dummylist;
        while (v) {
		calc_node_ports(v,xypos_avail);
                v = NNEXT(v);
        }
}


/* Calculate the node ports of one node
 * ------------------------------------
 *                     |          EWEIGHTP(e) = 1
 *                     |
 *                ------------    NWEIGHTP(N) = 1
 *               |     N      |
 *                ------------    NWEIGHTS(N) = 2
 *                  ^    |        
 *                  |    |        EWEIGHTS(e) = 1 .. 2
 */

void calc_node_ports(GNODE v,int xypos_avail)
{
	int act_port;
	int act_pcol, act_pstyle, act_psize;
	int pcol, pstyle, psize;
	int midport,nullport,portpos;
	ADJEDGE a;

	debugmessage("calc_node_ports","");

	/* First, inspect the successors */

	act_port = 1;
	if (G_portsharing!=YES) act_port = 0;
	if (NANCHORNODE(v))     act_port = 0;

	a = NSUCC(v);
	if (a) {
		act_pstyle = EARROWBSTYLE(AKANTE(a));
		act_pcol   = EARROWBCOL(AKANTE(a));
		act_psize  = EARROWBSIZE(AKANTE(a));
		if (EKIND(a)=='r')      act_pstyle = -1;
		else if (EKIND(a)=='l') act_pstyle = -1;
	}
	while (a) {
		pstyle = EARROWBSTYLE(AKANTE(a));
		pcol   = EARROWBCOL(AKANTE(a));
		psize  = EARROWBSIZE(AKANTE(a));
		if (EKIND(a)=='r')      pstyle = -1;
		else if (EKIND(a)=='l') pstyle = -1;

		/* For anchornodes, pstyle must change always ! Dito, if ports
		 * are not shared but separated. 
		 */
		if (NANCHORNODE(v)) pstyle= 5+(act_pstyle+1)%20;
		if (G_portsharing!=YES) pstyle= 5+(act_pstyle+1)%20;

		if ((psize==act_psize)&&(pcol==act_pcol)&&(pstyle==act_pstyle)){
			if (pstyle!= -1) EWEIGHTS(AKANTE(a)) = act_port;
		}
		else { 	act_pstyle = pstyle;
			act_psize  = psize;
			act_pcol   = pcol;
			if (pstyle!= -1) { 
				act_port++;
				EWEIGHTS(AKANTE(a)) = act_port;
			}
		}
		a = ANEXT(a);
	}
	NWEIGHTS(v) = act_port;
	
	/* Some shapes need corrections of ports */

	if (xypos_avail) {
		switch (NSHAPE(v)) {
  		case ELLIPSE:
	  	case RHOMB:
			midport = 1;
			nullport = 0;
			a = NSUCC(v);
			while (a) {
				if (NX(TARGET(a))+NWIDTH(TARGET(a))<NX(v)) 
					portpos = -1;
				else if (NX(TARGET(a))>NX(v)+NWIDTH(v)) 
					portpos = 1;
				else 	portpos = 0;
				if (portpos<0) midport = EWEIGHTS(AKANTE(a));
				if (portpos==0) {
					if (!nullport) 
						midport = 
						nullport = EWEIGHTS(AKANTE(a));
					else 	midport = (EWEIGHTS(AKANTE(a))
								+nullport)/2; 
				}
				else nullport = 0;
				a = ANEXT(a);
			}
			if (act_port-midport > midport-1) 
				portpos = 2*act_port-3*midport+1;
			else 	portpos = midport-1;
			NWEIGHTS(v) = 2*(midport+portpos)-1; 
			if (portpos) {
				a = NSUCC(v);
				while (a) { 
					EWEIGHTS(AKANTE(a))+=portpos; 
					a=ANEXT(a); 
				}
			}	
			break;
		}
	}

	/* Now, inspect the predecessors */

	act_port = 1;
	if (NANCHORNODE(v)) act_port = 0;
	if (G_portsharing!=YES) act_port = 0;

	a = NPRED(v);
	if (a) {
		act_pstyle = EARROWSTYLE(AKANTE(a));
		act_pcol   = EARROWCOL(AKANTE(a));
		act_psize  = EARROWSIZE(AKANTE(a));
		if (EKIND(a)=='r')      act_pstyle = -1;
		else if (EKIND(a)=='l') act_pstyle = -1;
	}
	while (a) {
		pstyle = EARROWSTYLE(AKANTE(a));
		pcol   = EARROWCOL(AKANTE(a));
		psize  = EARROWSIZE(AKANTE(a));
		if (EKIND(a)=='r')      pstyle = -1;
		else if (EKIND(a)=='l') pstyle = -1;

		/* For anchornodes, pstyle must change always ! Dito, if ports
		 * are not shared but separated. 
		 */
		if (NANCHORNODE(v)) pstyle= 5+(act_pstyle+1)%20;
		if (G_portsharing!=YES) pstyle= 5+(act_pstyle+1)%20;

		if ((psize==act_psize)&&(pcol==act_pcol)&&(pstyle==act_pstyle)){
			if (pstyle!= -1) EWEIGHTP(AKANTE(a)) = act_port;
		}
		else { 	act_pstyle = pstyle;
			act_psize  = psize;
			act_pcol   = pcol;
			if (pstyle!= -1) { 
				act_port++;
				EWEIGHTP(AKANTE(a)) = act_port;
			}
		}
		a = ANEXT(a);
	}
	NWEIGHTP(v) = act_port;

	/* Some shapes need corrections of ports */

	if (xypos_avail) {
		switch (NSHAPE(v)) {
  		case ELLIPSE:
  		case TRIANGLE:
  		case RHOMB:
			midport = 1;
			nullport = 0;
			a = NPRED(v);
			while (a) {
				if (NX(SOURCE(a))+NWIDTH(SOURCE(a))<NX(v)) 
					portpos = -1;
				else if (NX(SOURCE(a))>NX(v)+NWIDTH(v)) 
					portpos = 1;
				else 	portpos = 0;
				if (portpos<0) midport = EWEIGHTP(AKANTE(a));
				if (portpos==0) {
					if (!nullport) 
						midport =
						nullport = EWEIGHTP(AKANTE(a));
					else 	midport = (EWEIGHTP(AKANTE(a))
								+nullport)/2; 
				}
				else nullport = 0;
				a = ANEXT(a);
			}
			if (act_port-midport > midport-1) 
				portpos = 2*act_port-3*midport+1;
			else 	portpos = midport-1;
			NWEIGHTP(v) = 2*(midport+portpos)-1; 
			if (portpos) {
				a = NPRED(v);
				while (a) { 
					EWEIGHTP(AKANTE(a))+=portpos; 
					a=ANEXT(a); 
				}
			}	
			break;
		}
	}
}


/*--------------------------------------------------------------------*/
/*  Calculation of xy coordinates of edges                            */
/*--------------------------------------------------------------------*/

/*  Set xy-coordinates for all visible edges
 *  ----------------------------------------
 *  Here we initialize ESTARTX, ESTARTY, EENDX, EENDY. Note
 *  that ETBENDY and EBBENDY are corrected later.
 */

static void calc_all_edge_xy(void)
{
	GNODE   v;

	debugmessage("calc_all_edge_xy","");

	v = nodelist;
        while (v) {
		calc_edge_xy(v);
                v = NNEXT(v);
        }
        v = labellist;
        while (v) {
		calc_edge_xy(v);
                v = NNEXT(v);
        }
        v = dummylist;
        while (v) {
		calc_edge_xy(v);
                v = NNEXT(v);
        }
}

/*  Calculate coordinates of all edges that start at node v
 *  -------------------------------------------------------
 *  Depending on the port number, we calculate the co-ordinates,
 *  because the ports should be distingishable in the drawing.
 */

void calc_edge_xy(GNODE v)
{
	int node_x,node_y,node_width,node_height;
	int node_predports,node_succports, dist, dist1;
	CONNECT c;
	GEDGE   e;
	ADJEDGE a;

	debugmessage("calc_edge_xy","");
	assert((v));
	node_width  = NWIDTH(v);
	node_height = NHEIGHT(v);
	node_x      = NX(v);
	node_y      = NY(v);
	node_predports = NWEIGHTP(v);
	node_succports = NWEIGHTS(v);

	switch (NSHAPE(v)) {
	case TRIANGLE:  dist1 = node_width/4;
			break;
	default: 	dist1 = 0;
	}

	/* First: edge coordinates of connections */
	c = NCONNECT(v);
	if (c) {
		if (CTARGET(c)) {
			e = CEDGE(c);
			if (ESTART(e)==v) {
				ETBENDY(e) = ESTARTY(e) = node_y+node_height/2;
				if (NX(EEND(e))<node_x) 
					ETBENDX(e) = ESTARTX(e) = node_x+dist1;
				else 	ETBENDX(e) = ESTARTX(e) = node_x-dist1+node_width;
			}
			else { /* EEND(e)==v */
				EBBENDY(e) = EENDY(e) = node_y+node_height/2;
				if (NX(ESTART(e))<node_x) 
					EBBENDX(e) = EENDX(e) = node_x+dist1;
				else 	EBBENDX(e) = EENDX(e) = node_x-dist1+node_width;
			}
		}
		if (CTARGET2(c)) {
			e = CEDGE2(c);
			if (ESTART(e)==v) {
				ETBENDY(e) = ESTARTY(e) = node_y+node_height/2;
				if (NX(EEND(e))<node_x) 
					ETBENDX(e) = ESTARTX(e) = node_x+dist1;
				else 	ETBENDX(e) = ESTARTX(e) = node_x-dist1+node_width;
			}
			else { /* EEND(e)==v */
				EBBENDY(e) = EENDY(e) = node_y+node_height/2;
				if (NX(ESTART(e))<node_x) 
					EBBENDX(e) = EENDX(e) = node_x+dist1;
				else 	EBBENDX(e) = EENDX(e) = node_x-dist1+node_width;
			}
		}
	}

	/* Now: check all successors */
	a = NSUCC(v);
	while (a) {
		e = AKANTE(a);
		switch (EART(e)) {
		case 'l': ETBENDY(e) = ESTARTY(e) = node_y+node_height/2;
			  ETBENDX(e) = ESTARTX(e) = node_x+dist1;
			  break;
		case 'r': ETBENDY(e) = ESTARTY(e) = node_y+node_height/2;
			  ETBENDX(e) = ESTARTX(e) = node_x+node_width-dist1;
			  break;
		default:  
			  ETBENDX(e) = ESTARTX(e) = node_x + 
				node_width * EWEIGHTS(e) / (node_succports+1);

			  switch (NSHAPE(v)) {
			  case RHOMB:
				if (ESTARTX(e)-node_x < node_width/2)
					dist = ((node_width+1)/2 - ESTARTX(e)
						+ node_x) * node_height
						/ node_width;
				else 	dist = (-(node_width+1)/2 + ESTARTX(e)
						- node_x) * node_height
						/ node_width;
				ESTARTY(e) = node_y + node_height - dist;
				break;	
			  case ELLIPSE:
				dist = node_height/2 -
					gstoint(sqrt(
					  (double)(node_height*node_height)/4.0
					   - (double)(node_height*node_height)
					    /(double)(node_width*node_width)
					    *(ESTARTX(e)-node_x-(double)node_width/2.0)
					    *(ESTARTX(e)-node_x-(double)node_width/2.0)));
				ESTARTY(e) = node_y + node_height - dist;
				break;	
			  default:  
				ESTARTY(e) = node_y + node_height;
			  }
			  ETBENDY(e) = ESTARTY(e);
		}
		a = ANEXT(a);
	}
	/* Now: check all predecessors */
	a = NPRED(v);
	while (a) {
		e = AKANTE(a);
		switch (EART(e)) {
		case 'l': EBBENDY(e) = EENDY(e) = node_y+node_height/2;
			  EBBENDX(e) = EENDX(e) = node_x+node_width-dist1;
			  break;
		case 'r': EBBENDY(e) = EENDY(e) = node_y+node_height/2;
			  EBBENDX(e) = EENDX(e) = node_x+dist1;
			  break;
		default:  
			  EBBENDX(e) = EENDX(e) = node_x + 
				node_width * EWEIGHTP(e) / (node_predports+1);

			  switch (NSHAPE(v)) {
			  case RHOMB:
				if (EENDX(e)-node_x < node_width/2)
					dist = ((node_width+1)/2 - EENDX(e)
						+ node_x) * node_height
						/ node_width;
				else 	dist = (-(node_width+1)/2 + EENDX(e)
						- node_x) * node_height
						/ node_width;
				EENDY(e) = node_y + dist;
				break;	
			  case ELLIPSE:
				dist = node_height/2 -
					gstoint(sqrt(
					  (double)(node_height*node_height)/4.0
					   - (double)(node_height*node_height)
					    /(double)(node_width*node_width)
					    *(EENDX(e)-node_x-(double)node_width/2.0)
					    *(EENDX(e)-node_x-(double)node_width/2.0)));
				EENDY(e) = node_y + dist;
				break;	
			  case TRIANGLE:
				if (EENDX(e)-node_x < node_width/2)
					dist = ((node_width+1)/2 - EENDX(e)
						+ node_x) * node_height * 2
						/ node_width;
				else 	dist = (-(node_width+1)/2 + EENDX(e)
						- node_x) * node_height * 2
						/ node_width;
				EENDY(e) = node_y + dist;
				break;	
			  default:  
				EENDY(e) = node_y;
			  }
			  EBBENDY(e) = EENDY(e);
		}
		a = ANEXT(a);
	}
}


/*--------------------------------------------------------------------*/
/*  Calculation of manhatten layout                                   */
/*--------------------------------------------------------------------*/

/* Manhatten layout
 * ================
 * Manhatten layout consists of horizontal and vertical edge segments.
 * Thus we calculate ETBENDY and EBBENDY, but since it is manhatten
 * layout, it holds ETBENDY==EBBENDY.
 */

static void 	calc_manhatten(void)
{
	GNLIST li;
	int topbendp, botbendp;
	int maxrow, i;

	debugmessage("calc_manhatten","");
	assert((layer));
	for (i=0; i<=maxdepth; i++) {
 		if (i%20==0) gs_wait_message('e');

		/* First, calculate the minimal upper bendpoint `topbendp'
		 * and the maximal lower bendpoint `botbendp'.
		 */
		topbendp = 0;
		botbendp = MAXINT;
		li = TPRED(layer[i]);
		while (li) {
			tpred_connection1[NPOS(GNNODE(li))] = li;
			if (NY(GNNODE(li))+NHEIGHT(GNNODE(li))>topbendp)
				topbendp = NY(GNNODE(li))+NHEIGHT(GNNODE(li));
			li = GNNEXT(li);
		}
		li = TPRED(layer[i+1]);
		while (li) {
			tpred_connection2[NPOS(GNNODE(li))] = li;
			if (NY(GNNODE(li))<botbendp)
				botbendp = NY(GNNODE(li));
			li = GNNEXT(li);
		}

		/* Now we fill ETBENDY by the indicator in which row
		 * the edge must go. The function returns the maximal
		 * number of used rows.
		 */

		maxrow = fill_row_indicators(i);

		/* Now, we assign ETBENDY and EBBENDY according to
		 * the row indicators.
		 */

		if (botbendp-topbendp > 20) {
			botbendp = botbendp-10;
			topbendp = topbendp+10;
		}

		evaluate_row_indicators(i, maxrow, topbendp, botbendp);

	}
	evaluate_row_indicators(maxdepth+1, maxrow, topbendp, botbendp);
}


/* Calculate the row indicators
 * -----------------------------
 * The row indicators denote in which relative row the edge must be bend.
 * We return the maximal number of rows that must exist.
 * In order to avoid double crossings or confusion, all edges that point
 * to the left at a fixed time point must have a different row.
 * Dito for all edges that point to the right. Thus we use a plane sweep
 * algorithm to find the rows.
 */

static  int     size_lower_list;        /* nr. of elements in lower_list */
static  int     size_upper_list;        /* nr. of elements in upper_list */
static  int     maxr_lower_list;        /* maximal row     in lower_list */
static  int     maxr_upper_list;        /* maximal row     in upper_list */
static  int     maxr_sum;		/* sum of maximal rows           */
static  DLLIST  lower_list     = NULL;  /* the lower list                */
static  DLLIST  lower_list_end = NULL;  /* and its end                   */
static  DLLIST  upper_list     = NULL;  /* the upper list                */
static  DLLIST  upper_list_end = NULL;  /* and its end                   */
static  int 	upperxpos;
static  int 	lowerxpos;

static int 	fill_row_indicators(int level)
{
	GNLIST li1, li2;
	ADJEDGE a1, a2;

	debugmessage("fill_row_indicator","");
        assert((level>=0));
        assert((level<=maxdepth));      /* we access to level+1
                                         * and tmp_layer[maxdepth+1] exists
                                         */
	size_upper_list = size_lower_list = 0;
	maxr_lower_list = maxr_upper_list = 0;
	lower_list = lower_list_end = NULL;
        upper_list = upper_list_end = NULL;
	maxr_sum = 0;
	upperxpos = -1;
	lowerxpos = -1;

	li1 = TSUCC(layer[level]);
	li2 = TSUCC(layer[level+1]);
	a1 = NULL;
	while (li1 && (!a1)) {
		a1 = NSUCC(GNNODE(li1));
		if (!a1) li1 = GNNEXT(li1); 
	}
	a2 = NULL;
	while (li2 && (!a2)) {
		a2 = NPRED(GNNODE(li2));
		if (!a2) li2 = GNNEXT(li2); 
	}
	while ((li1)||(li2)) {
		if (a1 && a2) {
			if (ESTARTX(AKANTE(a1)) < EENDX(AKANTE(a2))) {
				finish_upper(GNNODE(li1),ESTARTX(AKANTE(a1)));
				a1 = ANEXT(a1);
			}
			else {  finish_lower(GNNODE(li2),EENDX(AKANTE(a2)));
				a2 = ANEXT(a2);
			}
		}
		else if (a1) {
			finish_upper(GNNODE(li1),ESTARTX(AKANTE(a1)));
			a1 = ANEXT(a1);
		}
		else if (a2) {
			finish_lower(GNNODE(li2),EENDX(AKANTE(a2)));
			a2 = ANEXT(a2);
		}
		if (!a1) { 
			if (li1) li1 = GNNEXT(li1); 
			while (li1 && (!a1)) {
				a1 = NSUCC(GNNODE(li1));
				if (!a1) li1 = GNNEXT(li1); 
			}
		}
		if (!a2) { 
			if (li2) li2 = GNNEXT(li2); 
			while (li2 && (!a2)) {
				a2 = NPRED(GNNODE(li2));
				if (!a2) li2 = GNNEXT(li2); 
			}
		}
	}

        return(maxr_sum);
}
	

/* Finish a node at the upper level
 * --------------------------------
 * and touch all nonfinished edges.
 * All edges that start before or at this node (check NX) but
 * finish before are untouched, i.e. deleted from the upper list.
 * They were touched before. Since they do not require rows anymore,
 * the maximal number of rows is recalculated.
 */

static void 	finish_upper(GNODE v, int xpos)
{
	ADJEDGE a;
	GEDGE ee;
	DLLIST n,m;
	int k;

	debugmessage("finish_upper","");
	assert((v));

	k = maxr_upper_list + maxr_lower_list;
	if (k>maxr_sum) maxr_sum = k;
	if (xpos<=upperxpos) return;
	upperxpos = xpos;

	/* for all NSUCC's of v in order of their gradient... 
 	 * the edges, that are not yet threated are now inserted.
	 */
	a = NSUCC(v);
	while (a) {
		ee = AKANTE(a);
		if ((ESTARTX(ee)==xpos) && (EENDX(ee) > ESTARTX(ee))) {
			/* nonfinished: touch it */
			append_to_lower(ee,TARGET(a));
		}
		a = ANEXT(a);
	}
	k = maxr_upper_list + maxr_lower_list;
	if (k>maxr_sum) maxr_sum = k;

	n = upper_list;
	while (n) {
		m = DSUCC(n);
		if (DNX(n) <= xpos) delete_upper(n);
		n = m;
	}
	maxr_upper_list = 0;
	n = upper_list;
	while (n) {
		if (DINFO(n)> maxr_upper_list) maxr_upper_list = DINFO(n);
		n = DSUCC(n);
	}
}


/* Finish a node at the lower level
 * --------------------------------
 * and touch all nonfinished edges 
 * This is symmetrical to finish_upper.
 */

static void 	finish_lower(GNODE v, int xpos)
{
	ADJEDGE a;
	GEDGE ee;
	DLLIST n,m;
	int k;

	debugmessage("finish_lower","");
	assert((v));

	k = maxr_upper_list + maxr_lower_list;
	if (k>maxr_sum) maxr_sum = k;
	if (xpos<=lowerxpos) return;
	lowerxpos = xpos;

	/* for all NPRED's of v in order of their gradient... 
 	 * the edges, that are not yet threated are now inserted.
	 */
	a = NPRED(v);
	while (a) {
		ee = AKANTE(a);
		if ((EENDX(ee)==xpos) && (EENDX(ee) <= ESTARTX(ee))) {
			/* nonfinished: touch it */
			append_to_upper(ee,SOURCE(a));
		}
		a = ANEXT(a);
	}
	k = maxr_upper_list + maxr_lower_list;
	if (k>maxr_sum) maxr_sum = k;

	n = lower_list;
	while (n) {
		m = DSUCC(n);
		if (DNX(n) <= xpos) delete_lower(n);
		n = m;
	}

	maxr_lower_list = 0;
	n = lower_list;
	while (n) {
		if (DINFO(n)> maxr_lower_list) maxr_lower_list = DINFO(n);
		n = DSUCC(n);
	}
}


/*  Management of upper_list and lower_list */
/*==========================================*/

/*  Insert node n at the end of upper_list
 *  --------------------------------------
 */

static void	append_to_upper(GEDGE e, GNODE n)
{
	DLLIST	d;

	assert((n));
	debugmessage("append_to_upper", (NTITLE(n)?NTITLE(n):""));

	d = dllist_alloc(n,upper_list_end);
	if (!upper_list)	upper_list = d;
	if (upper_list_end)	DSUCC(upper_list_end) = d;
	upper_list_end 	= d;
	size_upper_list++;
	if (ESTARTX(e)!=EENDX(e)) maxr_upper_list++;
	DINFO(d) = maxr_upper_list;
	ETBENDY(e) = maxr_upper_list;
	DNX(d) = ESTARTX(e); 
}


/*  Insert node n at the end of lower_list
 *  --------------------------------------
 */

static void	append_to_lower(GEDGE e, GNODE n)
{
	DLLIST	d;

	assert((n));
	debugmessage("append_to_lower", (NTITLE(n)?NTITLE(n):""));

	d = dllist_alloc(n,lower_list_end);
	if (!lower_list)	lower_list = d;
	if (lower_list_end)	DSUCC(lower_list_end) = d;
	lower_list_end 	= d;
	size_lower_list++;
	if (ESTARTX(e)!=EENDX(e)) maxr_lower_list++;
	DINFO(d) = maxr_lower_list;
	ETBENDY(e) = maxr_lower_list;
	DNX(d) = EENDX(e); 
}


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




/* Set the top and bottom bendpoint according to the row indicator
 * ---------------------------------------------------------------
 */

static void 	evaluate_row_indicators(int level, int maxr, int miny, int maxy)
{
	GNLIST li;
        ADJEDGE a;
	GEDGE ee;
	GNODE node;
	int k, k1, k2;
	debugmessage("evaluate_row_indicators","");

	li = TSUCC(layer[level]);
	while (li) {
        	a = NSUCC(GNNODE(li));
        	while (a) {
			ee = AKANTE(a);
			if (one_line_manhatten==1) {
				k = maxy- (maxy-miny) / 2; 
			}
			else {
				if (ESTARTX(ee) >= EENDX(ee))
					k = maxy- (maxy-miny) * 
					       (maxr+1-ETBENDY(ee)) / (maxr+1);
				else    k = miny+ (maxy-miny) *
					       (maxr+1-ETBENDY(ee)) / (maxr+1);
			}
			ETBENDY(ee) = k;
			EBBENDY(ee) = k;
                	a = ANEXT(a);
        	}
		li = GNNEXT(li);
	}

	li = TSUCC(layer[level]);
	while (li) {
		node = GNNODE(li);

		if (  (NWIDTH(node)==0) && (NHEIGHT(node)==0)
		    &&(NSUCC(node)) && (NNEXT(NSUCC(node)))
		    &&(NNEXT(NNEXT(NSUCC(node)))==NULL)
		    &&((NPRED(node))==NULL)) {

			/* It is a dummy node with two successors
			 * and no predecessor.
			 */
				
			k1 = ETBENDY(AKANTE(NSUCC(node)));
			k2 = ETBENDY(AKANTE(NNEXT(NSUCC(node))));
			if (k1<k2) k = k1;
			else k = k2;
			NY(node)  = k;
			ESTARTY(AKANTE(NSUCC(node))) = k; 
			ESTARTY(AKANTE(NNEXT(NSUCC(node)))) = k;
		}
		if (  (NWIDTH(node)==0) && (NHEIGHT(node)==0)
		    &&(NPRED(node)) && (NNEXT(NPRED(node)))
		    &&(NNEXT(NNEXT(NPRED(node)))==NULL)
		    &&((NSUCC(node))==NULL)) {

			/* It is a dummy node with two predecessors 
			 * and no succecessor.
			 */

			k1 = EBBENDY(AKANTE(NPRED(node)));
			k2 = EBBENDY(AKANTE(NNEXT(NPRED(node))));
			if (k1>k2) k = k1;
			else k = k2;
			NY(node) = k;
			EENDY(AKANTE(NPRED(node))) = k;
			EENDY(AKANTE(NNEXT(NPRED(node)))) = k;
		}
		li = GNNEXT(li);
	}
}



/*--------------------------------------------------------------------*/
/*  Calculation of bend points                                        */
/*--------------------------------------------------------------------*/

/* Bendpoints of edges
 * ===================
 *  For the x,y co-ordinates, there is a problem if neighoured nodes
 *  are to large. To avoid (1), we allow that edges are bend, see (2)
 *
 *             -   --------                 -   --------
 *            |A| |        |               |A| |        |
 *             -  |        |                -  |        |
 *              \ |   M    |                |  |   M    |
 *   (1)         \|        |          (2)   |  |        |
 *                \        |                |  |        |
 *                |\       |                |  |        |
 *                 -\------                  \  --------
 *                   \                        \
 *
 *  The x-co-ordinate of a bend point is the same as the x-co-ordinate
 *  of the startpoint. We calculate the y-co-ordinate of the bend-point
 *  now. To ensure that bending introduce new crossings, we must iterate
 *  this process a second time to change (3) to (4).
 * 
 *             -   -                        -   -         
 *            |A| |B|                      |A| |B|   
 *             -   -                        -   -     
 *              \  |                        |   |         
 *   (3)         \ |                  (4)   |   |        
 *                \|                        |   |       
 *                 \                        |   |      
 *                 |\                        \  |           
 *                 \ \                        \ \
 *
 *  The iteratation stops because the bending is always increase, and cannot
 *  be more the the height of the largest node of the level.
 */

static void calc_all_bendpoints(void)
{
	int i,j;
	GNLIST li;
	GNODE node;
	int h,changed;
	int topbendp;
	int botbendp;
	int minx, maxx;

	debugmessage("calc_all_bendpoints","");
	assert((layer));
	for (i=0; i<=maxdepth; i++) {
 		if (i%10==0) gs_wait_message('e');

		/* First, we correct the position of dummy nodes in
		 * order to avoid such situations:
		 *           \   /
		 *            \ /
		 *             |  <- Bending
		 *             d
		 */

		li = TSUCC(layer[i]);
		minx = MAXINT;
		maxx = 0;
		while (li) {
			j = NY(GNNODE(li));
			if (j<minx) minx = j;
			j = j + NHEIGHT(GNNODE(li));
			if (j>maxx) maxx = j;
			li = GNNEXT(li);
		}
		li = TSUCC(layer[i]);
		while (li) {
			node = GNNODE(li);

			if (  (NWIDTH(node)==0) && (NHEIGHT(node)==0)
			    &&(NSUCC(node)) && (NNEXT(NSUCC(node)))
			    &&(NNEXT(NNEXT(NSUCC(node)))==NULL)
			    &&((NPRED(node))==NULL)) {

				/* It is a dummy node with two successors
				 * and no predecessor.
				 */
				
				NY(node)  = maxx;
				ESTARTY(AKANTE(NSUCC(node))) = maxx;
				ETBENDY(AKANTE(NSUCC(node))) = maxx;
				ESTARTY(AKANTE(NNEXT(NSUCC(node)))) = maxx;
				ETBENDY(AKANTE(NNEXT(NSUCC(node)))) = maxx;
			}
			if (  (NWIDTH(node)==0) && (NHEIGHT(node)==0)
			    &&(NPRED(node)) && (NNEXT(NPRED(node)))
			    &&(NNEXT(NNEXT(NPRED(node)))==NULL)
			    &&((NSUCC(node))==NULL)) {

				/* It is a dummy node with two successors
				 * and no predecessor.
				 */
				
				NY(node) = minx;
				EENDY(AKANTE(NPRED(node))) = minx;
				EBBENDY(AKANTE(NPRED(node))) = minx;
				EENDY(AKANTE(NNEXT(NPRED(node)))) = minx;
				EBBENDY(AKANTE(NNEXT(NPRED(node)))) = minx;
			}
			li = GNNEXT(li);
		}

		/*  And now the real bendpoint calculation.
		 */

		topbendp = 0;
		botbendp = MAXINT;
		li = TPRED(layer[i]);
		while (li) {
			tpred_connection1[NPOS(GNNODE(li))] = li;
			if (NY(GNNODE(li))+NHEIGHT(GNNODE(li))>topbendp)
				topbendp = NY(GNNODE(li))+NHEIGHT(GNNODE(li));
			li = GNNEXT(li);
		}
		li = TPRED(layer[i+1]);
		while (li) {
			tpred_connection2[NPOS(GNNODE(li))] = li;
			if (NY(GNNODE(li))<botbendp)
				botbendp = NY(GNNODE(li));
			li = GNNEXT(li);
		}

		j = 1;
		changed = 1;

		while (changed) {
			j++;
			if (j>max_edgebendings) { 
                      		gs_wait_message('t');
                       		break;
                	}
        		if (G_timelimit>0)
                		if (test_timelimit(100)) {
                        		gs_wait_message('t');
                        		break;
                		}

			changed = 0;
			li = TSUCC(layer[i]);
			while (li) {
				h = calc_topbendpoint(li);
				changed += set_topbendpoint(li,h);
				li = GNNEXT(li);
			}
			li = TSUCC(layer[i+1]);
			while (li) {
				h = calc_botbendpoint(li);
				changed += set_botbendpoint(li,h);
				li = GNNEXT(li);
			}
		}

		/* If we do not come to a success in reasonable time,
		 * we simply set the bendpoints to the minimal resp. maximal
		 * values.
		 */

		if (changed) {
			li = TSUCC(layer[i]);
			while (li) {
				(void)set_topbendpoint(li,topbendp);
				li = GNNEXT(li);
			}
			li = TSUCC(layer[i+1]);
			while (li) {
				(void)set_botbendpoint(li,botbendp);
				li = GNNEXT(li);
			}
		}
	}
}



/*  Set the bendpoint of all succ edges of GNNODE(li)
 *  -------------------------------------------------
 *  bendp is the value the y-co-ordinate of the bendpoint is set to.
 *  If something has changed, we return 1.
 */

static int set_topbendpoint(GNLIST li, int bendp)
{
        ADJEDGE a;
	int changed;
        
        debugmessage("set_topbendpoint","");
        assert((li));
        assert((GNNODE(li)));
	changed = 0;
        a = NSUCC(GNNODE(li));
        while (a) {
                if (ETBENDY(AKANTE(a))<bendp) {
                        ETBENDY(AKANTE(a)) = bendp;
			changed = 1;
		}
                a = ANEXT(a);
        }
	return(changed);
}


/*  Set the bendpoint of all pred edges of GNNODE(li)
 *  -------------------------------------------------
 *  bendp is the value the y-co-ordinate of the bendpoint is set to.
 *  If something has changed, we return 1.
 */

static int set_botbendpoint(GNLIST li, int bendp)
{
        ADJEDGE a;
	int changed;
        
        debugmessage("set_botbendpoint","");
        assert((li));
        assert((GNNODE(li)));
	changed = 0;
        a = NPRED(GNNODE(li));
        while (a) {
                if (EBBENDY(AKANTE(a))>bendp) {
                        EBBENDY(AKANTE(a)) = bendp;
			changed = 1;
		}
                a = ANEXT(a);
        }
	return(changed);
}
 



/*  Calculate the top bendpoint of edges of GNNODE(li)
 *  --------------------------------------------------
 *  li is part of the TSUCC-list.
 *  Because the outgoing edges of GNNODE(li) are sorted, we need
 *  only to inspect the leftest end rightest successor edge.
 *  We return the y-co-ordinate of the bendpoint, or 0, if bending
 *  is not necessary.
 *
 *  The main idea is to calculate the bend-y point h:
 *        -     --------
 *       |s|   |        |
 * (sx,sy)-    |        |
 *        |.   |    k   |    Clearly:  (ty-h )     (tx-sx)
 *        |  . |        |              -------  =   ------
 *        |    .        |              (ty-ky)     (tx-kx)
 *  (sx,h) .   | .      |
 *        .   .|   .    |    implies:
 *        .     -.---.--              h = (sx-tx)(ky-ty)/(kx-tx)+ty
 *        . (kx,ky) .  .
 *        .            . .
 *        .               ..(tx,ty)
 *         -  -  -  -  - -----
 *                      |  t  |
 *                       -----
 */

static int calc_topbendpoint(GNLIST li)
{
	int    bendp, h;
	GEDGE  e;
	
	debugmessage("calc_topbendpoint","");
	assert((li));
	assert((GNNODE(li)));

	/* First, we examine the leftest successor */

	bendp = 0;
	e = NSUCCL(GNNODE(li));
	if (!e) return(bendp);
	bendp = calc_edgetopbendpoint(e,li);

	/* Now, we examine the rightest successor */

	e = NSUCCR(GNNODE(li));
	assert((e));	/* because NSUCCL(GNNODE(li)) exists */
	if (e==NSUCCL(GNNODE(li))) return(bendp);
	h = calc_edgetopbendpoint(e,li);
	if (h>bendp) bendp = h;
	return(bendp);
}



/*  This function does the work for one edge 
 *  ----------------------------------------
 */

#define bendformula  ((sx-tx)*(ky-ty))/(kx-tx)+ty

static int calc_edgetopbendpoint(GEDGE e, GNLIST li)
{
	int    bendp, h;
	GEDGE  e2;
	int sx,sy,tx,ty; /* co-ordinates of source and target        */
	int kx,ky;	 /* co-ordinates of node that causes bending */
	GNODE  node;
	int offset, myoff, cross;
	GNLIST li2;
	
	debugmessage("calc_edgetopbendpoint","");
	bendp = 0;

	sx = ESTARTX(e);
	sy = ESTARTY(e);
	tx = EBBENDX(e);
	ty = EBBENDY(e);
	offset = 7*EARROWBSIZE(e)/10+2;
	if (sx<tx) {
		li2 = GNNEXT(li);
		while (li2) {
			node = GNNODE(li2);
			if (!NANCHORNODE(node)) { 
				kx = NX(node);
				if (kx>=tx) break;
				if (NSHAPE(node) == RHOMB) {
					kx -= 3;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node)/2 + myoff;
					if (kx>=sx) {
						h = bendformula;
						if (h>bendp) bendp = h;
					}
					kx += NWIDTH(node)/2;
					if (kx>=tx) break;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node) + myoff;
					if (kx>=sx) {
						h = bendformula;
						if (h>bendp) bendp = h;
					}
				}
				else {
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node) + myoff;
					if (kx>=sx) {
						h = bendformula;
						if (h>bendp) bendp = h;
					}
				}
			}
			e2 = NSUCCL(node);
			if ((e2)&&(ETBENDY(e2)!=ESTARTY(e2))) {
				cross = 0;
				if (  (NPOS(ESTART(e2))<NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))>NPOS(EEND(e))) ) cross=1;
				if (  (NPOS(ESTART(e2))>NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))<NPOS(EEND(e))) ) cross=1;
				kx = ETBENDX(e2) - G_dspace+3;
				if (kx>=tx) break;
				ky = ETBENDY(e2);
				if ((kx>=sx) && (!cross)) {
					h = bendformula;
					if (h>bendp) bendp = h;
				}
                        }
			li2 = GNNEXT(li2);
		}
	}
	else if (tx<sx) { 
		li2 = tpred_connection1[NPOS(GNNODE(li))]; 
		/* li is now the corresponding in the TPRED list */
		li2 = GNNEXT(li2);
		while (li2) {
			node = GNNODE(li2);
			if (!NANCHORNODE(node)) { 
				kx = NX(node) + NWIDTH(node);
				if (kx<=tx) break;
				if (NSHAPE(node) == RHOMB) {
					kx += 3;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node)/2 + myoff;
					if (kx<=sx) { 
						h = bendformula;
						if (h>bendp) bendp = h;
					}
					kx -= NWIDTH(node)/2;
					if (kx<=tx) break;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node) + myoff;
					if (kx<=sx) { 
						h = bendformula;
						if (h>bendp) bendp = h;
					}
				}
				else {
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node) + myoff;
					if (kx<=sx) { 
						h = bendformula;
						if (h>bendp) bendp = h;
					}
				}
			}
			e2 = NSUCCR(node);
			if ((e2)&&(ETBENDY(e2)!=ESTARTY(e2))) {
				cross = 0;
				if (  (NPOS(ESTART(e2))<NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))>NPOS(EEND(e))) ) cross=1;
				if (  (NPOS(ESTART(e2))>NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))<NPOS(EEND(e))) ) cross=1;
				kx = ETBENDX(e2) + G_dspace-3;
				if (kx<=tx) break;
				ky = ETBENDY(e2);
				if ((kx<=sx) && (!cross)) {
					h = bendformula;
					if (h>bendp) bendp = h;
				}
			}
			li2 = GNNEXT(li2);
		}
	}
	return(bendp);
}



/*  Calculate the bot bendpoint of edges of GNNODE(li)
 *  --------------------------------------------------
 *  li is part of the TSUCC-list.
 *  Because the outgoing edges of GNNODE(li) are sorted, we need
 *  only to inspect the leftest end rightest predecessor edge.
 *  We return the y-co-ordinate of the bendpoint, or MAXINT, if bending
 *  is not necessary.
 *
 *  The main idea is to calculate the bend-y point h:
 *                       -----
 *                      |  t  |
 *         -  -  -  -  - -----
 *        .               ..(tx,ty)
 *        .            . .
 *        .         .  .       Clearly:  (h-ty )    (tx-sx)
 *        .   ky .   .                   -------  =  ------
 *        .   . ---.----                 (ky-ty)    (tx-kx)
 *  (sx,h) .   | .      |
 *        |    .        |
 *        |  . |        |      implies:
 *        |.   |   k    |               h = (sx-tx)(ky-ty)/(kx-tx)+ty
 * (sx,sy)-    |        |
 *       |s|   |kx      |
 *        -     --------
 */

static int calc_botbendpoint(GNLIST li)
{
	int    bendp, h;
	GEDGE  e;
	
	debugmessage("calc_botbendpoint","");
	assert((li));
	assert((GNNODE(li)));

	/* First, we examine the leftest predecessor */

	bendp = MAXINT;
	e = NPREDL(GNNODE(li));
	if (!e) return(bendp);
	bendp = calc_edgebotbendpoint(e,li);

	/* Now, we examine the rightest successor */
	e = NPREDR(GNNODE(li));
	assert((e));	/* because NPREDL(GNNODE(li)) exists */
	if (e==NPREDL(GNNODE(li))) return(bendp);
	h = calc_edgebotbendpoint(e,li);
	if (h<bendp) bendp = h;
	return(bendp);
}



/*  This function does the work for one edge 
 *  ----------------------------------------
 */

/* already defined, see above:
 *   #define bendformula  ((sx-tx)*(ky-ty))/(kx-tx)+ty
 */

static int calc_edgebotbendpoint(GEDGE e, GNLIST li)
{
	int    bendp, h;
	GEDGE  e2;
	int sx,sy,tx,ty; /* co-ordinates of source and target        */
	int kx,ky;	 /* co-ordinates of node that causes bending */
	GNODE  node;
	int offset, myoff, cross;
	GNLIST li2;
	
	debugmessage("calc_edgebotbendpoint","");
	bendp = MAXINT;

	sx = EENDX(e);
	sy = EENDY(e);
	tx = ETBENDX(e);
	ty = ETBENDY(e);
	offset = 7*EARROWSIZE(e)/10+2;

	if (sx<tx) {
		li2 = GNNEXT(li);
		while (li2) {
			node = GNNODE(li2);
			if (!NANCHORNODE(node)) { 
				kx = NX(node);
				if (kx>=tx) break;
				if (NSHAPE(node)==RHOMB) {
					kx -= 3;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node)/2 - myoff;
					if (kx>=sx) {
						h = bendformula;
						if (h<bendp) bendp = h;
					}
					kx += NWIDTH(node)/2;
					if (kx>=tx) break;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) - myoff;
					if (kx>=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
				}
				else if (NSHAPE(node)==TRIANGLE) {
					kx -= 3;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node) - myoff;
					if (kx>=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
					kx += NWIDTH(node)/2;
					if (kx>=tx) break;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) - myoff;
					if (kx>=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
				}
				else {
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) - myoff;
					if (kx>=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
				}
			}
			e2 = NPREDL(node);
			if ((e2)&&(EBBENDY(e2)!=EENDY(e2))) {
				cross = 0;
				if (  (NPOS(ESTART(e2))<NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))>NPOS(EEND(e))) ) cross=1;
				if (  (NPOS(ESTART(e2))>NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))<NPOS(EEND(e))) ) cross=1;
				kx = EBBENDX(e2) - G_dspace+3;
				if (kx>=tx) break;
				ky = EBBENDY(e2);
				if ((kx>=sx) && (!cross)) {
					h = bendformula;
					if ((h>0)&&(h<bendp)) bendp = h;
				}
                        }
			li2 = GNNEXT(li2);
		}
	}
	else if (tx<sx) { 
		li2 = tpred_connection2[NPOS(GNNODE(li))]; 
		/* li is now the corresponding in the TPRED list */
		li2 = GNNEXT(li2);
		while (li2) {
			node = GNNODE(li2);
			if (!NANCHORNODE(node)) { 
				kx = NX(node) + NWIDTH(node);
				if (kx<=tx) break;
				if (NSHAPE(node)==RHOMB) {
					kx += 3;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node)/2 - myoff;
					if (kx<=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
					kx -= NWIDTH(node)/2;
					if (kx<=tx) break;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) - myoff;
					if (kx<=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
				}
				else if (NSHAPE(node)==TRIANGLE) {
					kx += 3;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) + NHEIGHT(node) - myoff;
					if (kx<=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
					kx -= NWIDTH(node)/2;
					if (kx<=tx) break;
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) - myoff;
					if (kx<=sx) {
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
				}
				else {
					myoff = (kx-sx)*offset/(tx-sx);
					ky = NY(node) - myoff;
					if (kx<=sx) { 
						h = bendformula;
						if ((h>0)&&(h<bendp)) bendp = h;
					}
				}
			}
			e2 = NPREDR(node);
			if ((e2)&&(EBBENDY(e2)!=EENDY(e2))) {
				cross = 0;
				if (  (NPOS(ESTART(e2))<NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))>NPOS(EEND(e))) ) cross=1;
				if (  (NPOS(ESTART(e2))>NPOS(ESTART(e)))
				    &&(NPOS(EEND(e2))<NPOS(EEND(e))) ) cross=1;
				kx = EBBENDX(e2) + G_dspace-3;
				if (kx<=tx) break;
				ky = EBBENDY(e2);
				if ((kx<=sx) && (!cross)) {
					h = bendformula;
					if ((h>0)&&(h<bendp)) bendp = h;
				}
			}
			li2 = GNNEXT(li2);
		}
	}
	return(bendp);
}

/*--------------------------------------------------------------------*/
/*  Fine tuning of bendings on dummy nodes                            */
/*--------------------------------------------------------------------*/


/*   Dummy node bending correction
 *   =============================
 *   The idea is to remove the bendings at dummy nodes by replacing the
 *   ESTARTY(e1) by ETBENDY(e1). The problem is that while doing this,
 *   the corresponding point of the edge to ETBENDY(e1) at the line 
 *   x = ESTARTX(e1) changes, and it may produce a crossing with neighboured
 *   nodes. We check this additionally.
 *
 *   The situation:                          is changed into:
 *
 *       (ax,ay)                             (ax,ay)
 *           \                                   \    dist
 *             \                                  \   |
 *               \                                 \  v
 *                 \ (mx,my) <- dummy node          \....(mx,my)
 *                  |                                \  .
 *                  |                                 \ .
 *                  |  by                              \.
 *                   \                                   \
 *                     \                                   \
 *                       \                                   \
 *
 *   It holds:
 *                   dist      (by-my)
 *                   ------- = -------
 *                   (mx-ax)   (by-ay)
 *
 *
 *   However, now we have to check for a neigboured node k:  h < ky ?
 *                          If not, it is not okay.
 *         (ax,ay)
 *           ..                
 *           .  .                  It holds:
 *           .   _.___ ky                      (by-h)    (mx-kx)
 *           .  |   . |                        ------- =  ------
 *           .  |     .(kx,h)                  (by-ay)   (mx-ax)
 *           .  |     | .
 *           .  |     |   .
 *           .  |     |     .(mx,by)
 * 
 *   We do this for the top-bendings, if the dummy node has no bottom-bending,
 *   and similar for bottom-bendings, if the dummy node has no top-bending.
 */

static void 	tune_dummy_bendings(void)
{
	int i;
	GNLIST li, li2;
	GNODE  node;
	GEDGE e1,e2,e3;
	int ax, ay, mx, my, by, b2y, okay;
	int kx,ky,h;

	debugmessage("tune_dummy_bendings","");
	assert((layer));

	for (i=0; i<=maxdepth+1; i++) {
 		if (i%10==0) gs_wait_message('e');

		li = TPRED(layer[i]);
		while (li) {
			tpred_connection1[NPOS(GNNODE(li))] = li;
			li = GNNEXT(li);
		}

		li = TSUCC(layer[i]);
		while (li) {
			node = GNNODE(li);
			if (  (NWIDTH(node)==0)
			    &&(NSUCC(node)) && (NNEXT(NSUCC(node))==NULL)
			    &&(NPRED(node)) && (NNEXT(NPRED(node))==NULL)) {

				/* It is a dummy node with just one successor
				 * and one predecessor.
				 */

				e1 = AKANTE(NPRED(node));
				e2 = AKANTE(NSUCC(node));
				my  = ESTARTY(e2);
				by  = ETBENDY(e2);
				b2y = EBBENDY(e1);
				ay  = ETBENDY(e1);
				if (  (b2y==EENDY(e1)) 
				    &&(by>my) && (by>ay)) {
					ax = ETBENDX(e1);
					mx = ESTARTX(e2);
					/* assert(bx == mx); */
					okay = 1;

					if (  ((ax<mx)&&(mx<EBBENDX(e2)))
					    ||((ax<mx)&&(G_spline))) {

		li2 = tpred_connection1[NPOS(node)]; 
		/* li is now the corresponding in the TPRED list */
		li2 = GNNEXT(li2);
		while (li2) {
			if (okay==0) break;
			if (!NANCHORNODE(GNNODE(li2))) { 
				kx = NX(GNNODE(li2)) + NWIDTH(GNNODE(li2));
				if (NWIDTH(GNNODE(li2))==0)
					kx += G_dspace;
				if (kx<=ax) break;
				ky = NY(GNNODE(li2)) - 2;
				if (kx<=mx) { 
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky<=h) okay = 0;
				}
			}
			e3 = NPREDR(GNNODE(li2));
			if ((e3)&&(EBBENDY(e3)!=EENDY(e3))) {
				kx = EBBENDX(e3) + G_dspace-3;
				if (kx<=ax) break;
				ky = EBBENDY(e3);
				if (kx<=mx) {
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky<=h) okay = 0;
				}
			}
			li2 = GNNEXT(li2);
		}

						if (okay) {
							ESTARTY(e2)=
							 EBBENDY(e1)=
							 EENDY(e1)=ETBENDY(e2);
						}
					}
					else if (  ((ax>mx)&&(mx>EBBENDX(e2)))
					         ||((ax>mx)&&(G_spline))) {

		li2 = GNNEXT(li);
		while (li2) {
			if (okay==0) break;
			if (!NANCHORNODE(GNNODE(li2))) { 
				kx = NX(GNNODE(li2));
				if (NWIDTH(GNNODE(li2))==0)
					kx -= G_dspace;
				if (kx>=ax) break;
				ky = NY(GNNODE(li2)) - 2;
				if (kx>=mx) { 
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky<=h) okay = 0;
				}
			}
			e3 = NPREDL(GNNODE(li2));
			if ((e3)&&(EBBENDY(e3)!=EENDY(e3))) {
				kx = EBBENDX(e3) - G_dspace+3;
				if (kx>=ax) break;
				ky = EBBENDY(e3);
				if (kx>=mx) {
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky<=h) okay = 0;
				}
			}
			li2 = GNNEXT(li2);
		}

						if (okay) {
							ESTARTY(e2)=
							 EBBENDY(e1) = 
							 EENDY(e1)=ETBENDY(e2);
						}
					}	
				}

				/* now examine the bottom bending of e1 */

				ay  = EBBENDY(e2);
				if (  (by==ESTARTY(e2)) 
				    &&(b2y<my) && (b2y<ay)) {
					ax = EBBENDX(e2);
					mx = ESTARTX(e2);
					by = b2y;    /* to avoid confusion */
					/* assert(bx == mx); */
					okay = 1;

					if (  ((ax<mx)&&(mx<ETBENDX(e1)))
					    ||((ax<mx)&&(G_spline))) {

		li2 = tpred_connection1[NPOS(node)]; 
		/* li is now the corresponding in the TPRED list */
		li2 = GNNEXT(li2);
		while (li2) {
			if (okay==0) break;
			if (!NANCHORNODE(GNNODE(li2))) { 
				kx = NX(GNNODE(li2)) + NWIDTH(GNNODE(li2));
				if (NWIDTH(GNNODE(li2))==0)
					kx += G_dspace;
				if (kx<=ax) break;
				ky = NY(GNNODE(li2))+NHEIGHT(GNNODE(li2))+2;
				if (kx<=mx) { 
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky>=h) okay = 0;
				}
			}
			e3 = NSUCCR(GNNODE(li2));
			if ((e3)&&(ETBENDY(e3)!=ESTARTY(e3))) {
				kx = ETBENDX(e3) + G_dspace-3;
				if (kx<=ax) break;
				ky = ETBENDY(e3);
				if (kx<=mx) {
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky>=h) okay = 0;
				}
			}
			li2 = GNNEXT(li2);
		}

						if (okay) {
							ESTARTY(e2)=
							 ETBENDY(e2)=
							 EENDY(e1)=EBBENDY(e1);
						}
					}
					else if (  ((ax>mx)&&(mx>ETBENDX(e1)))
					         ||((ax>mx)&&(G_spline))) {

		li2 = GNNEXT(li);
		while (li2) {
			if (okay==0) break;
			if (!NANCHORNODE(GNNODE(li2))) { 
				kx = NX(GNNODE(li2));
				if (NWIDTH(GNNODE(li2))==0)
					kx -= G_dspace;
				if (kx>=ax) break;
				ky = NY(GNNODE(li2))+NHEIGHT(GNNODE(li2))+2;
				if (kx>=mx) { 
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky>=h) okay = 0;
				}
			}
			e3 = NSUCCL(GNNODE(li2));
			if ((e3)&&(ETBENDY(e3)!=ESTARTY(e3))) {
				kx = ETBENDX(e3) - G_dspace+3;
				if (kx>=ax) break;
				ky = ETBENDY(e3);
				if (kx>=mx) {
					h = ((mx-kx)*(ay-by))/(mx-ax)+by;
					if (ky>=h) okay = 0;
				}
			}
			li2 = GNNEXT(li2);
		}

						if (okay) {
							ESTARTY(e2)=
							 ETBENDY(e2)=
							 EENDY(e1)=EBBENDY(e1);
						}
					}	
				}
			}
			li = GNNEXT(li);
		}
	}

}


/*--------------------------------------------------------------------*/
/*  Calculation of arrow orientations                                 */
/*--------------------------------------------------------------------*/

/*  Arrow orientations
 *  ==================
 *  we have the following possibilities for orientations:
 *
 *              NORTH                   NORTHWEST  NORTHEAST
 *                ^                       --            --
 *               /|\                      |\            /|
 *         /      |     \                   \          /
 *  WEST  <---         -->  EAST        
 *         \      |     /                   /          \
 *               \|/                      |/_          _\|
 *                v                        
 *              SOUTH                   SOUTHWEST  SOUTHEAST
 *
 *  We use the difference quotient to decide which direction is taken. 
 *  The problem is, that we want to have only one arrow at each port.
 *  Thus we look at the ports from which main direction the edges
 *  come. If no main direction is detectable, we use NORTH or SOUTH
 *  but shorten all edge lines a little bit such that they end exactly
 *  where the arrow starts.
 */

static void	calc_all_edgearrows(void)
{
	GNODE v;

	debugmessage("calc_all_edgearrows","");

	v = nodelist;
        while (v) {
		calc_edgearrow(v);
                v = NNEXT(v);
        }
	v = labellist;
        while (v) {
		calc_edgearrow(v);
                v = NNEXT(v);
        }
	v = dummylist;
        while (v) {
		calc_edgearrow(v);
                v = NNEXT(v);
        }
}


/*  Edge arrows at a node
 *  ---------------------
 *  Here we calculate the edge arrows that point to this node.
 *  The horizontal edges occur only at connections. There we have no
 *  real ports, because each side reaches only one horizontal edge.
 *  The vertical edges reach ports. 
 */

void 	calc_edgearrow(GNODE v)
{
	ADJEDGE a;
	int 	act_port, j;
	CONNECT c;

	debugmessage("calc_edgearrow","");

	/* First the horizontal edges */
	c = NCONNECT(v);
	if (c) {
		if ((CTARGET(c))&&(EEND(CEDGE(c))==v)) 
			check_horizontal(CEDGE(c));
		if ((CTARGET2(c))&&(EEND(CEDGE2(c))==v)) 
			check_horizontal(CEDGE2(c));
	}

	/* Now the remaining edges */
	a = NSUCC(v);
	act_port = -1;
	while (a) {
		if (act_port!=EWEIGHTS(AKANTE(a))) {
			act_port = EWEIGHTS(AKANTE(a));
			check_up_port(a);	
		}
		a = ANEXT(a);
	}

	/* Now the downward edges */
	a = NPRED(v);
	act_port = -1;
	while (a) {
		if ((EKIND(a)=='l')||(EKIND(a)=='r'))
			check_horizontal(AKANTE(a));
		else if (act_port!=EWEIGHTP(AKANTE(a))) {
			act_port = EWEIGHTP(AKANTE(a));
			check_down_port(a);	
		}
		a = ANEXT(a);
	}

	if (NANCHORNODE(v)) {
		c = NCONNECT(v);
		assert(c && (CTARGET(c)));
		assert((EEND(CEDGE(c))==v));
		if (ESTARTX(CEDGE(c))<EENDX(CEDGE(c)))
			j = ORI_WEST;
		else 	j = ORI_EAST;
		a = NSUCC(v);
		while (a) {
			EORI2(AKANTE(a))= j;
			a = ANEXT(a);
		}
		a = NPRED(v);
		while (a) {
			EORI(AKANTE(a))= j;
			a = ANEXT(a);
		}
		if (CTARGET2(c)) {
			assert((ESTART(CEDGE2(c))==v));
			EORI2(CEDGE2(c)) = j;
		}
	}
}



/*  Detect orientation of a horizontal edge e
 *  -----------------------------------------
 *  EORI is set according to the difference quotient fval.
 *  Here, we do not have a bending.
 */

static void check_horizontal(GEDGE e)
{
	float 	fval;

	debugmessage("check_horizontal","");

	assert((ESTARTX(e)!=EENDX(e)));
	fval = (float)(EENDY(e)-ESTARTY(e))/(float)(EENDX(e)-ESTARTX(e));
	if (ESTARTX(e)<EENDX(e)) { 
		if      (0.5 <= fval) 	  EORI(e) = ORI_SOUTHEAST; 
		else if (-0.5 < fval) 	  EORI(e) = ORI_EAST; 
		else /* (fval <= -0.5) */ EORI(e) = ORI_NORTHEAST; 
	}
	else /* (ESTARTX(e)>EENDX(e)) */ { 
		if      (0.5 <= fval) 	  EORI(e) = ORI_NORTHWEST; 
		else if (-0.5 < fval) 	  EORI(e) = ORI_WEST; 
		else /* (fval <= -0.5) */ EORI(e) = ORI_SOUTHWEST; 
	}
	switch (EORI(e)) {
	case ORI_SOUTHEAST: EORI2(e) = ORI_NORTHWEST; break;
	case ORI_EAST:      EORI2(e) = ORI_WEST;      break;
	case ORI_NORTHEAST: EORI2(e) = ORI_SOUTHWEST; break;
	case ORI_SOUTHWEST: EORI2(e) = ORI_NORTHEAST; break;
	case ORI_WEST:      EORI2(e) = ORI_EAST;      break;
	case ORI_NORTHWEST: EORI2(e) = ORI_SOUTHEAST; break;
	}
}




/*  Detect orientation of the vertical down-up edges at port a
 *  ----------------------------------------------------------
 *  EORI2 is set according to the difference quotient fval.
 *  If all edge come from the center upwards, we set orientation NORTH.
 *  If all edge come right from the center, we set orientation NORTHWEST.
 *  If all edge come left  from the center, we set orientation NORTHEAST.
 *  If we have no uniqe direction (i.e. some edges come from the left,
 *  some from the right) we set orientation NORTH, but note that we must
 *  shorten the edge lines later. 
 */

static void check_up_port(ADJEDGE a)
{
	int port;
	int is_north, is_northeast, is_northwest;
	ADJEDGE b;
	GEDGE   e;
	float 	fval;

	debugmessage("check_up_port","");

	port = EWEIGHTS(AKANTE(a));

	/* Check orientation */
	b = a;
	is_north = is_northeast = is_northwest = 1;
	while (b) {
		if (port!=EWEIGHTS(AKANTE(b))) break; 
		e = AKANTE(b);
		/* assert((ESTARTY(e)<EBBENDY(e))); */
		if (ETBENDY(e)!=ESTARTY(e)) 
		        fval = (float)(ETBENDX(e)-ESTARTX(e))/
					(float)(ETBENDY(e)-ESTARTY(e));
		else 	fval = (float)(EBBENDX(e)-ESTARTX(e))/
					(float)(EBBENDY(e)-ESTARTY(e));
		if (!((-0.5<fval)&&(fval<0.5))) is_north = 0;	
		if (!(0.1<fval)) 		is_northwest = 0;	
		if (!(fval<-0.1)) 		is_northeast = 0;	
		b = ANEXT(b);
	}
	if (is_north) {
		b = a;
		while (b) {
			if (port!=EWEIGHTS(AKANTE(b))) break; 
			EORI2(AKANTE(b)) = ORI_NORTH;
			b = ANEXT(b);
		}
		return;
	}	
	if (is_northwest) {
		b = a;
		while (b) {
			if (port!=EWEIGHTS(AKANTE(b))) break; 
			EORI2(AKANTE(b)) = ORI_NORTHWEST;
			b = ANEXT(b);
		}
		return;
	}	
	if (is_northeast) {
		b = a;
		while (b) {
			if (port!=EWEIGHTS(AKANTE(b))) break; 
			EORI2(AKANTE(b)) = ORI_NORTHEAST;
			b = ANEXT(b);
		}
		return;
	}	

	/* else: NORTH, but shorten the edge lines !!! */
	b = a;
	while (b) {
		if (port!=EWEIGHTS(AKANTE(b))) break; 
		EORI2(AKANTE(b)) = ORI_NORTH;
		b = ANEXT(b);
	}
}


/*  Detect orientation of the vertical up-down edges at port a
 *  ----------------------------------------------------------
 *  EORI is set according to the difference quotient fval.
 *  If all edge come from the center upwards, we set orientation SOUTH.
 *  If all edge come right from the center, we set orientation SOUTHEAST.
 *  If all edge come left  from the center, we set orientation SOUTHWEST.
 *  If we have no uniqe direction (i.e. some edges come from the left,
 *  some from the right) we set orientation SOUTH, but note that we must
 *  shorten the edge lines later. 
 */

static void check_down_port(ADJEDGE a)
{
	int port;
	int is_south, is_southwest, is_southeast;
	ADJEDGE b;
	GEDGE   e;
	float 	fval;

	debugmessage("check_down_port","");

	port = EWEIGHTP(AKANTE(a));

	/* Check orientation */
	b = a;
	is_south = is_southeast = is_southwest = 1;
	while (b) {
		if (port!=EWEIGHTP(AKANTE(b))) break; 
		e = AKANTE(b);
		/* assert((ETBENDY(e)<EENDY(e))); */
		if (EBBENDY(e)!=EENDY(e)) 
			fval = (float)(EENDX(e)-EBBENDX(e))/
					(float)(EENDY(e)-EBBENDY(e));
		else	fval = (float)(EENDX(e)-ETBENDX(e))/
					(float)(EENDY(e)-ETBENDY(e));
		if (!((-0.5<fval)&&(fval<0.5))) is_south = 0;	
		if (!(0.1<fval)) 		is_southeast = 0;	
		if (!(fval<-0.1)) 		is_southwest = 0;	
		b = ANEXT(b);
	}
	if (is_south) {
		b = a;
		while (b) {
			if (port!=EWEIGHTP(AKANTE(b))) break; 
			EORI(AKANTE(b)) = ORI_SOUTH;
			b = ANEXT(b);
		}
		return;
	}	
	if (is_southeast) {
		b = a;
		while (b) {
			if (port!=EWEIGHTP(AKANTE(b))) break; 
			EORI(AKANTE(b)) = ORI_SOUTHEAST;
			b = ANEXT(b);
		}
		return;
	}	
	if (is_southwest) {
		b = a;
		while (b) {
			if (port!=EWEIGHTP(AKANTE(b))) break; 
			EORI(AKANTE(b)) = ORI_SOUTHWEST;
			b = ANEXT(b);
		}
		return;
	}	

	/* else: SOUTH, but shorten the edge lines !!! */
	b = a;
	while (b) {
		if (port!=EWEIGHTP(AKANTE(b))) break; 
		EORI(AKANTE(b)) = ORI_SOUTH;
		b = ANEXT(b);
	}
}


/*--------------------------------------------------------------------*/
/*   Flip mirror for left_to_right orientation                        */
/*--------------------------------------------------------------------*/

/*  Flip the whole graph
 *  ====================
 *  Note: if we have left_to_right orientation, width and height are
 *  exchanged during the layout (see step3.c). This is corrected now.
 *  Further, all x and y co-ordinates are exchanged, i.e. the mirrow
 *  axis is the x = y axis.
 */

static void flip_mirror(void)
{
	debugmessage("flip_mirror","");

	flip_all_nodes(nodelist);
	flip_all_nodes(labellist);
	flip_all_nodes(dummylist);
}


/*  Flip the nodes of a list
 *  ------------------------
 *  Flip all nodes and all incoming edges.
 */

#define backward_connection1(c) ((CEDGE(c))&& (EEND(CEDGE(c)) ==v))
#define backward_connection2(c) ((CEDGE2(c))&&(EEND(CEDGE2(c))==v))

static void flip_all_nodes(GNODE v)
{	
	int h;
	CONNECT c;
	ADJEDGE li;

	debugmessage("flip_all_nodes","");
	while (v) {
		h     = NX(v);
		NX(v) = NY(v);
		NY(v) = h;
		h          = NWIDTH(v);
		NWIDTH(v)  = NHEIGHT(v);
		NHEIGHT(v) = h;

		c = NCONNECT(v);
		if (c) {
			if (backward_connection1(c)) flip_edge(CEDGE(c));
			if (backward_connection2(c)) flip_edge(CEDGE2(c));
		}
		li = NPRED(v);
		while (li) {
			flip_edge(AKANTE(li));
			li = ANEXT(li);
		}
		v = NNEXT(v);
	}
}


/*  Flip an edge 
 *  ------------
 */

static void flip_edge(GEDGE e) 
{
	int h;

	debugmessage("flip_edge","");
	h = ESTARTX(e);
	ESTARTX(e) = ESTARTY(e);
	ESTARTY(e) = h;
	h = ETBENDX(e);
	ETBENDX(e) = ETBENDY(e);
	ETBENDY(e) = h;
	h = EBBENDX(e);
	EBBENDX(e) = EBBENDY(e);
	EBBENDY(e) = h;
	h = EENDX(e);
	EENDX(e) = EENDY(e);
	EENDY(e) = h;
	switch (EORI(e)) {
	case NO_ORI: break;
	case ORI_NORTH:     EORI(e) = ORI_WEST;      break;
	case ORI_NORTHEAST: EORI(e) = ORI_SOUTHWEST; break;
	case ORI_NORTHWEST: EORI(e) = ORI_NORTHWEST; break;
	case ORI_SOUTH:     EORI(e) = ORI_EAST;      break; 
	case ORI_SOUTHEAST: EORI(e) = ORI_SOUTHEAST; break;
	case ORI_SOUTHWEST: EORI(e) = ORI_NORTHEAST; break;
	case ORI_EAST:      EORI(e) = ORI_SOUTH;     break; 
	case ORI_WEST:      EORI(e) = ORI_NORTH;     break; 
	}
	switch (EORI2(e)) {
	case NO_ORI: break;
	case ORI_NORTH:     EORI2(e) = ORI_WEST;      break;
	case ORI_NORTHEAST: EORI2(e) = ORI_SOUTHWEST; break;
	case ORI_NORTHWEST: EORI2(e) = ORI_NORTHWEST; break;
	case ORI_SOUTH:     EORI2(e) = ORI_EAST;      break; 
	case ORI_SOUTHEAST: EORI2(e) = ORI_SOUTHEAST; break;
	case ORI_SOUTHWEST: EORI2(e) = ORI_NORTHEAST; break;
	case ORI_EAST:      EORI2(e) = ORI_SOUTH;     break; 
	case ORI_WEST:      EORI2(e) = ORI_NORTH;     break; 
	}
}


/*--------------------------------------------------------------------*/
/*   Flip mirror for bottom_to_top orientation                        */
/*--------------------------------------------------------------------*/

/*  Flip the whole graph
 *  ====================
 *  Calculate the maximal y position maxy and flip the whole graph
 *  at the f(x) = maxy/2 axis.
 */

static int my_maxy;

static void flip_ver_mirror(void)
{
	int i;
	GNLIST k;

	debugmessage("flip_ver_mirror","");

	my_maxy = 0;

	/* First: calculate maxy */
	for (i=maxdepth+1; i>=0; i--) if (TSUCC(layer[i])) break;

	k = TSUCC(layer[i]);
	while (k) {
		if (NY(GNNODE(k))+NHEIGHT(GNNODE(k))>my_maxy)
			my_maxy = NY(GNNODE(k))+NHEIGHT(GNNODE(k));
		k = GNNEXT(k);
	}

	my_maxy = my_maxy + G_ybase;	

	flip_ver_all_nodes(nodelist);
	flip_ver_all_nodes(labellist);
	flip_ver_all_nodes(dummylist);
}


/*  Flip the nodes of a list vertically
 *  -----------------------------------
 *  Flip all nodes and all incoming edges.
 */

#define backward_connection1(c) ((CEDGE(c))&& (EEND(CEDGE(c)) ==v))
#define backward_connection2(c) ((CEDGE2(c))&&(EEND(CEDGE2(c))==v))

static void flip_ver_all_nodes(GNODE v)
{	
	CONNECT c;
	ADJEDGE li;

	debugmessage("flip_ver_all_nodes","");
	while (v) {
		NY(v) = my_maxy - NY(v) - NHEIGHT(v);

		c = NCONNECT(v);
		if (c) {
			if (backward_connection1(c)) flip_ver_edge(CEDGE(c));
			if (backward_connection2(c)) flip_ver_edge(CEDGE2(c));
		}
		li = NPRED(v);
		while (li) {
			flip_ver_edge(AKANTE(li));
			li = ANEXT(li);
		}
		v = NNEXT(v);
	}
}


/*  Flip an edge 
 *  ------------
 */

static void flip_ver_edge(GEDGE e) 
{
	debugmessage("flip_ver_edge","");
	ESTARTY(e) = my_maxy - ESTARTY(e);
	EBBENDY(e) = my_maxy - EBBENDY(e);
	ETBENDY(e) = my_maxy - ETBENDY(e);
	EENDY(e)   = my_maxy - EENDY(e);
	switch (EORI(e)) {
	case NO_ORI: break;
	case ORI_NORTH:     EORI(e) = ORI_SOUTH;     break;
	case ORI_NORTHEAST: EORI(e) = ORI_SOUTHEAST; break;
	case ORI_NORTHWEST: EORI(e) = ORI_SOUTHWEST; break;
	case ORI_SOUTH:     EORI(e) = ORI_NORTH;     break; 
	case ORI_SOUTHEAST: EORI(e) = ORI_NORTHEAST; break;
	case ORI_SOUTHWEST: EORI(e) = ORI_NORTHWEST; break;
	}
	switch (EORI2(e)) {
	case NO_ORI: break;
	case ORI_NORTH:     EORI2(e) = ORI_SOUTH;     break;
	case ORI_NORTHEAST: EORI2(e) = ORI_SOUTHEAST; break;
	case ORI_NORTHWEST: EORI2(e) = ORI_SOUTHWEST; break;
	case ORI_SOUTH:     EORI2(e) = ORI_NORTH;     break; 
	case ORI_SOUTHEAST: EORI2(e) = ORI_NORTHEAST; break;
	case ORI_SOUTHWEST: EORI2(e) = ORI_NORTHWEST; break;
	}
}


/*--------------------------------------------------------------------*/
/*   Calculate xmax and ymax positions                                */
/*--------------------------------------------------------------------*/

int maximal_xpos;
int maximal_ypos;

void calc_max_xy_pos(void)
{
	GNODE v;

	debugmessage("calc_max_xy_pos","");
	maximal_xpos = 0;
	maximal_ypos = 0;

	v = nodelist;
	while (v) {
		if (NX(v)+NWIDTH(v)>maximal_xpos)
			maximal_xpos = NX(v)+NWIDTH(v);
		if (NY(v)+NHEIGHT(v)>maximal_ypos)
			maximal_ypos = NY(v)+NHEIGHT(v);
		v = NNEXT(v);
	}
	v = labellist;
	while (v) {
		if (NX(v)+NWIDTH(v)>maximal_xpos)
			maximal_xpos = NX(v)+NWIDTH(v);
		if (NY(v)+NHEIGHT(v)>maximal_ypos)
			maximal_ypos = NY(v)+NHEIGHT(v);
		v = NNEXT(v);
	}
	v = dummylist;
	while (v) {
		if (NX(v)+NWIDTH(v)>maximal_xpos)
			maximal_xpos = NX(v)+NWIDTH(v);
		if (NY(v)+NHEIGHT(v)>maximal_ypos)
			maximal_ypos = NY(v)+NHEIGHT(v);
		v = NNEXT(v);
	}
	maximal_xpos += G_xbase;
	maximal_ypos += G_ybase;
	debugmessage("end of calc_max_xy_pos","");
}


/*--------------------------------------------------------------------*/
/*   Statistics                                                       */
/*--------------------------------------------------------------------*/

int st_nr_vis_nodes;
int st_nr_vis_edges;
int st_nr_vis_nearedges;
int st_nr_vis_dummies;
int st_nr_vis_labels;
int st_nr_invis_graphs;
int st_nr_invis_nodes;
int st_nr_invis_edges;
int st_max_indeg;
int st_max_outdeg;
int st_max_degree;

#define backward_connection1(c) ((CEDGE(c))&& (EEND(CEDGE(c)) ==v))
#define backward_connection2(c) ((CEDGE2(c))&&(EEND(CEDGE2(c))==v))

void statistics(void)
{
	GNODE v;
	GEDGE e;
	ADJEDGE li;
	CONNECT c;
	int maxdegree;
	int maxindeg;
	int maxoutdeg;

	debugmessage("statistics","");

	st_nr_vis_nodes     = 0;
	st_nr_vis_edges     = 0;
	st_nr_vis_nearedges = 0;
	st_nr_vis_dummies   = 0;
	st_nr_vis_labels    = 0;
	st_nr_invis_graphs  = 0;
	st_nr_invis_nodes   = 0;
	st_nr_invis_edges   = 0;
	st_max_indeg        = 0;
	st_max_outdeg       = 0;
	st_max_degree       = 0;

	v = nodelist;
	while (v) {
		c = NCONNECT(v);
		if (c) {
			if (backward_connection1(c)) st_nr_vis_nearedges++; 
			if (backward_connection2(c)) st_nr_vis_nearedges++; 
		}
		maxdegree = maxindeg = maxoutdeg = 0;
		li = NSUCC(v);
                while (li) {
			maxdegree++;
			maxoutdeg++;
			li = ANEXT(li);
		}
		li = NPRED(v);
                while (li) {
			st_nr_vis_edges++;		
			maxdegree++;
			maxindeg++;
			li = ANEXT(li);
		}
		if (maxdegree>st_max_degree) st_max_degree=maxdegree;
		if (maxindeg >st_max_indeg)  st_max_indeg =maxindeg;
		if (maxoutdeg>st_max_outdeg) st_max_outdeg=maxoutdeg;
		st_nr_vis_nodes++;
		v = NNEXT(v);
	}
	v = labellist;
	while (v) {
		c = NCONNECT(v);
		if (c) {
			if (backward_connection1(c)) st_nr_vis_nearedges++; 
			if (backward_connection2(c)) st_nr_vis_nearedges++; 
		}
		maxdegree = maxindeg = maxoutdeg = 0;
		li = NSUCC(v);
                while (li) {
			maxdegree++;
			maxoutdeg++;
			li = ANEXT(li);
		}
		li = NPRED(v);
                while (li) {
			st_nr_vis_edges++;		
			maxdegree++;
			maxindeg++;
			li = ANEXT(li);
		}
		if (maxdegree>st_max_degree) st_max_degree=maxdegree;
		if (maxindeg >st_max_indeg)  st_max_indeg =maxindeg;
		if (maxoutdeg>st_max_outdeg) st_max_outdeg=maxoutdeg;
		st_nr_vis_labels++;
		v = NNEXT(v);
	}
	v = dummylist;
	while (v) {
		c = NCONNECT(v);
		if (c) {
			if (backward_connection1(c)) st_nr_vis_nearedges++; 
			if (backward_connection2(c)) st_nr_vis_nearedges++; 
		}
		maxdegree = maxindeg = maxoutdeg = 0;
		li = NSUCC(v);
                while (li) {
			maxdegree++;
			maxoutdeg++;
			li = ANEXT(li);
		}
		li = NPRED(v);
                while (li) {
			st_nr_vis_edges++;		
			maxdegree++;
			maxindeg++;
			li = ANEXT(li);
		}
		if (maxdegree>st_max_degree) st_max_degree=maxdegree;
		if (maxindeg >st_max_indeg)  st_max_indeg =maxindeg;
		if (maxoutdeg>st_max_outdeg) st_max_outdeg=maxoutdeg;
		st_nr_vis_dummies++;
		v = NNEXT(v);
	}
	v = invis_nodes;
	while (v) {
		st_nr_invis_nodes++;
		v = NNEXT(v);
	}
	v = graphlist;
	while (v) {
		st_nr_invis_graphs++;
		v = NNEXT(v);
	}
	e = edgelist; 
	while (e) {
		if (EINVISIBLE(e)) st_nr_invis_edges++;
		e = ENEXT(e);
	}

	/* Further we have nr_crossings and maximal_xpos and maximal_ypos */
}

