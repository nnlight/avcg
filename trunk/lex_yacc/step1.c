/* SCCS-info %W% %E% */

/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         step1.c                                            */
/*   version:      1.00.00                                            */
/*   creation:     14.4.1993                                          */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */  
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Layout phase 1: building a proper hierarchy        */
/*   status:       in work                                            */
/*                                                                    */
/*--------------------------------------------------------------------*/

#ifndef lint
static char *id_string="$Id: step1.c,v 3.11 1995/02/08 11:11:14 sander Exp $";
#endif


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


/* 
 * $Log: step1.c,v $
 * Revision 3.11  1995/02/08  11:11:14  sander
 * Distribution version 1.3.
 *
 * Revision 3.10  1994/12/23  18:12:45  sander
 * Manhatten layout added.
 * Option interface cleared.
 *
 * Revision 3.9  1994/08/05  14:27:31  sander
 * Negative values of G_width, etc. corrected.
 *
 * Revision 3.8  1994/08/05  12:13:25  sander
 * Treelayout added. Attributes "treefactor" and "spreadlevel" added.
 * Scaling as abbreviation of "stretch/shrink" added.
 *
 * Revision 3.7  1994/08/03  13:58:44  sander
 * Horizontal order mechanism changed.
 * Attribute horizontal_order for edges added.
 *
 * Revision 3.6  1994/08/02  15:36:12  sander
 * On layout algorithm minbackward, fine tuning phase corrected.
 * Allow nodes to be pulled just before/after their successors/predecessors,
 * if no backward edges are produced by this.
 *
 * Revision 3.5  1994/05/16  08:56:03  sander
 * shape attribute (boxes, rhombs, ellipses, triangles) added.
 *
 * Revision 3.4  1994/05/05  08:20:30  sander
 * Algorithm late labels added: If labels are inserted
 * after partitioning, this may yield a better layout.
 *
 * Revision 3.3  1994/04/27  16:05:19  sander
 * Some general changes for the PostScript driver.
 * Horizontal order added. Bug fixes of the folding phases:
 * Folding of nested graphs works now.
 *
 * Revision 3.2  1994/03/04  19:11:24  sander
 * Specification of levels per node added.
 * X11 geometry behaviour (option -geometry) changed such
 * that the window is now opened automatically.
 *
 * Revision 3.1  1994/03/01  10:59:55  sander
 * Copyright and Gnu Licence message added.
 * Problem with "nearedges: no" and "selfloops" solved.
 *
 * Revision 2.5  1994/02/10  15:56:32  sander
 * Layoutalgorithm changed: We not only put the start nodes
 * in front of the node list, but additionally add the end
 * nodes (which have no successor) at the end of the node list.
 *
 * Revision 2.4  1994/01/21  19:33:46  sander
 * VCG Version tested on Silicon Graphics IRIX, IBM R6000 AIX and Sun 3/60.
 * Option handling improved. Option -grabinputfocus installed.
 * X11 Font selection scheme implemented. The user can now select a font
 * during installation.
 * Sun K&R C (a nonansi compiler) tested. Some portabitility problems solved.
 *
 * Revision 2.3  1994/01/03  15:29:06  sander
 * First complete X11 version.
 *
 */


/************************************************************************
 * The situation here is the following:
 * -----------------------------------
 * The folding of the graph is done and the adjacency lists are created,
 * but we have recognized that not all visible nodes have their (x,y) 
 * coordinates.
 * Thus, we have to layout the graph. WE SIMPLY IGNORE ALL EXISTING
 * COORDINATES AND LAYOUT THE WHOLE GRAPH.
 *
 * Before we start, the situation is: 
 *  1) We have proper adjacency lists.
 *  2) All visible nodes are in the nodelist or in the labellist.
 *  3) All potentially visible edges are in the lists edgelist or tmpedgelist.
 *     Visible edges can be detected by the EINVISIBLE flag (==0) in these
 *     lists. Note: invisible edges may also be in edgelist or tmpedgelist.
 *  4) An edge is visible iff it is used in the adjacency lists.
 *  5) NCONNECT of all nodes is empty.
 *  6) There is a list of default connections: the near_edge_list.
 *
 * The task of building a proper hierarchy is to distribute vertically 
 * the nodes into layers. The edges starting at one level go to node
 * of the immediate next level. Thus it is necessary to create dummy
 * nodes in one layer if an edge goes accross several layers.
 * There may be edges inside the layer, but only from nodes to directly 
 * neigboured nodes. This situation is summarized with connection nodes,
 * see alloc.h.
 * If we have A-->B and A and B are on the same level, we call B a
 * forward connection of A, and A a backward connection of B. During
 * the layout, the connection edge between A and B is deleted, but can
 * be restored from the NCONNECT-fields. 
 * The distribution of the nodes into layers is done by a depth first 
 * search that conceptually partitioned the edges into `tree edges', 
 * `forward edges', `backward edges' and `cross edges'. 
 * We have implemented two different depth first searches, which can be
 * selected by the flag -d of vcg.
 * Later, we will layout the tree AS TREE and try to add the nontree
 * edges artificially.
 *
 * After that, we have the following situation:
 *    1)  The array layer contains all visible nodes in the TSUCC lists. 
 *        They are distributed at the layer[i] lists and connected by two 
 *        lists TPRED and TSUCC to allow to traverse the nodes of one 
 *	  layer[i] backwards and forwards. However TSUCC and TPRED are
 *  	  different: TSUCC contains all nodes of the layer including those
 *        nodes reacheable by forward connections. TPRED excludes these
 *	  nodes. TANZ(layer[i]) is not yet initialized, i.e. it is 0.
 *	  If it is TREE_LAYOUT, the TPRED lists are not initialized.
 *	  The hierarchy in layer is proper.
 *    2)  labellist and nodelist have not changed.  
 *	  All visible nodes are in nodelist, labellist and dummylist.
 *    3)  All pot. visible edges are in the lists edgelist or tmpedgelist.
 *	  Same as before.
 *    4)  If it is not a TREE_LAYOUT, then: 
 *	  maxindeg and maxoutdeg are the maximal indegree (number of 
 *	  incoming edges) and maximal outdegree (number of outgoing
 *	  edges) occuring at visible nodes. Forward connections are 
 *	  not counted.
 *    5)  maxdepth+1 is the maximal layer !!! NOT maxdepth !!!
 *    6)  NTIEFE(node) is filled for all nodes. 
 *    7)  If it is not a TREE_LAYOUT, then:
 *	  NINDEG and NOUTDEG are filled for all nodes. Forward connections 
 *	  are not counted. Edges of forward connection are bend to the 
 *	  connection reference node, that is the node that has these forward 
 *	  connections but no backward connection.
 *        NCONNECT(node) is filled for nodes that have direct neighbours
 *	  in the layout. The edges in NCONNECT are not anymore in the
 *	  adjacency lists, but still visible. The forward connection nodes 
 * 	  are still in the layers in the TSUCC lists, but not in the
 *        TPRED lists. 
 *    7)  Reverted edges are marked with EART(e)='R'.
 *  	  Self loops don't anymore exist.
 *
 * This file provides the following functions:
 * ------------------------------------------
 * step1_main		Main routine to build a proper hierarchy
 * revert_edge	 	reverts an edge in the adjacency lists
 *
 * If DEBUG is switched on, we have further:
 *
 * db_output_graph	  prints all visible nodes and edges of the graph
 * db_output_adjacencies  prints the adjacency lists of the visible nodes
 * db_output_adjacency    prints the adjacency lists of one node
 * db_output_layer	  prints the layers
 * db_output_vcglayer	  prints the layers in VCG format to a file
 *
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "alloc.h"
#include "main.h"
#include "options.h"
#include "timelim.h"
#include "folding.h"
#include "steps.h"
#include "timing.h"


/* Prototypes
 * ----------
 */

#ifdef DEBUG
static void	db_output_nodes 	_PP((GNODE node));
static void	db_output_edges  	_PP((GEDGE edge));
static void     db_output_layer		_PP((void));
static void     db_output_vcglayer	_PP((char *fn));
#endif

static void 	insert_startnode	_PP((GNODE node));
static void 	insert_endnode		_PP((GNODE node));
static void	prepare_startnodes 	_PP((void));
static void 	prepare_anchoredge	_PP((GEDGE edge));
static void	insert_near_edges	_PP((void));
static void	insert_bent_near_edges	_PP((void));
static int  	check_connect_cycle	_PP((GNODE v,GNODE w,GNODE z));
static void	partition_edges		_PP((void));
static void	depth_first_search 	_PP((GNODE node));
static void	alt_depth_first_search 	_PP((GNODE node));
static void 	start_dfs_backwards	_PP((ADJEDGE edge,GNODE node,int prio));

static void	topological_sort	  _PP((void));
static void 	add_to_zero_indegree_list _PP((GNODE v));
static GNODE 	get_zero_indegree         _PP((void));
static int 	topsort_maxlevel  	  _PP((GNODE node1,GNODE node2));
static void 	topsort_setlevel 	  _PP((GNODE node1,GNODE node2,int l));
static void 	topsort_add_succs 	  _PP((GNODE node1,GNODE node2));
static int 	topsort_indegree          _PP((GNODE node1,GNODE node2));

static void 	add_to_nlist		_PP((GNODE v, GNLIST *l));
static GNODE 	get_nlist		_PP((GNLIST *l));
static void	sc_component_sort	_PP((void));
static void	calc_sc_component	_PP((GNLIST *nlist));
static int 	complete_scc		_PP((GNLIST nlist));
static int 	scc_outdeg		_PP((GNODE v, GNODE w, int prio));
static int 	scc_succoutdeg		_PP((GNODE v, GNODE w));
static int 	scc_indeg		_PP((GNODE v, GNODE w));
static void	revert_outedges		_PP((GNODE v, GNODE w));
static void 	scc_traversal	_PP((GNODE n, long *d, GNLIST *p));

static void 	add_phase1_labels	_PP((void));

static void 	tune_partitioning	_PP((void));
static int 	tune_node_depth		_PP((GNODE v,int lab));
static void 	create_depth_lists	_PP((void));
static void 	complete_depth_lists	_PP((void));
static void 	calc_connect_adjlists	_PP((GNODE v, GNODE w, GNODE predw));

static void	inspect_edges 		_PP((void));
static void 	check_edge		_PP((GNODE n, ADJEDGE e, int l));
static void	inspect_double_edges	_PP((void));
static void	check_double_edge 	_PP((ADJEDGE e));
static GNODE	create_dummy 		_PP((int t));
static ADJEDGE	create_edge		_PP((GNODE x, GNODE y,GEDGE e,int t));


/* Global variables
 * ----------------
 */

int	maxindeg;	/* maximal indegree  of a node */
int	maxoutdeg;	/* maximal outdegree of a node */

DEPTH *layer = NULL;	       /* The table of layers     */
int    maxdepth = 0;           /* Max. depth of layout    */
static int size_of_layer = 0;  /* Size of table of layers */ 

/* Macros
 * ------
 */

#define forward_connection1(c)  ((CEDGE(c))&& (EEND(CEDGE(c))==CTARGET(c)))
#define forward_connection2(c)  ((CEDGE2(c))&&(EEND(CEDGE2(c))==CTARGET2(c)))
#define backward_connection1(c) ((CEDGE(c))&& (EEND(CEDGE(c))!=CTARGET(c)))
#define backward_connection2(c) ((CEDGE2(c))&&(EEND(CEDGE2(c))!=CTARGET2(c)))

/*--------------------------------------------------------------------*/
/*  Building a proper hierarchy                                       */
/*--------------------------------------------------------------------*/

#ifdef ANSI_C
void	step1_main(void)
#else
void	step1_main()
#endif
{
	int i;

	start_time();
	debugmessage("step1_main","");
	assert((dummylist==NULL));

	/* prepare back edges, i.e. revert all back edges initially
	 */

	prepare_back_edges();

	/* reorder the nodes such that start nodes are found first */

	prepare_startnodes();

	/* insert dummy nodes for edges with anchor connection
	 */

	insert_anchor_edges();

	/* insert near edge connections
	 */

	insert_near_edges();

	/* insert bent near edge connections
	 */

	insert_bent_near_edges();

	/* partition the edges into tree, forward, etc. edges and 
	 * calculate maxdepth.
	 */

	if (layout_flag==3) topological_sort();
	else if (layout_flag==0) sc_component_sort();
	else partition_edges();

	/* Eventually, we have to add the edge labels now.
	 */

	if (edge_label_phase == 1)  add_phase1_labels();

	/* Fine tune this partitioning, such that all edges at a node
 	 * are nearly equidistant.
	 */

	if (fine_tune_layout==1) tune_partitioning();

	/* allocate the array layer */

	if (maxdepth+2 > size_of_layer) {
		if (layer) free(layer);
		layer = (DEPTH *)malloc((maxdepth+2)*sizeof(struct depth_entry));
		if (!layer) Fatal_error("memory exhausted","");
		size_of_layer = maxdepth+2;
#ifdef DEBUG
		PRINTF("Maxdepth of this layout: %d \n",maxdepth);
		PRINTF("Sizeof table `layer': %ld Bytes\n",
			(maxdepth+2)*sizeof(struct depth_entry));
#endif
	}
	for (i=0; i<maxdepth+2; i++) {
		 TANZ(layer[i])      = 0;
		 /* TCROSS is never used for layer */
		 TPRED(layer[i])     = NULL;
		 TSUCC(layer[i])     = NULL;
		 TRESNEEDED(layer[i])= 1;
	}

	/* and fill the array intiefe.
	 * We create only the TSUCC lists of layer[i].
	 */

	create_depth_lists();

	/* check the direction of the edges and create dummy and pseudo nodes,
	 * if edges go beyond several levels, or for self loops. This may 
	 * change the TSUCC lists. After that, the hierarchy is proper.
	 */
 
	inspect_edges();
	inspect_double_edges();

	/* Now, we derive the TPRED lists from the TSUCC lists
	 * and calculate NINDEG, NOUTDEG, maxindeg and maxoutdeg
	 */

	if (layout_flag == TREE_LAYOUT) {
			stop_time("step1_main");
                        layout_flag = tree_main();
			if (layout_flag != TREE_LAYOUT) { 
				FPRINTF(stderr,"\nThis is not a downward tree. ");
				FPRINTF(stderr,"Continuing with normal layout\n");
			}
        }

	/* For the statisctics, calculate the number of reverted edges.
	 */

	calc_number_reversions();


	if (layout_flag != TREE_LAYOUT) {
		complete_depth_lists();
		stop_time("step1_main");
	}
}


/*--------------------------------------------------------------------*/
/*  Looking for start nodes                                           */
/*--------------------------------------------------------------------*/

/*  The creation of a proper hierarchy is initially driven by a depth
 *  first search. This depth first search should start at nodes that
 *  have no predecessors (i.e. indegree=0, but note that the field 
 *  NINDEG is not yet initialized properly).
 *  Thus we reorder the nodelist that all nodes with NPRED(node)==NULL
 *  are at the beginning of this list, ordered according increasing
 *  NREFNUM. 
 *  We never start the dfs with a label node, thus we can ignore the
 *  label nodes in labellist for this task.
 *
 *  Further, a second criterium is that nodes without successors
 *  come at last. We reorder the node list that all nodes with 
 *  NSUCC(node)==NULL but NPRED(node)!=NULL are at the end of the list,
 *  ordered according increasing NREFNUM.
 */

/* startnodes is a double linked list to contain temporarily 
 * the start nodes.
 */

static GNODE startnodes;
static GNODE startnodesend;

/*  Insert a node into the startnodes
 *  ---------------------------------
 *  The startnodes are sorted according NREFNUMS.
 */ 

#ifdef ANSI_C
static void 	insert_startnode(GNODE node)
#else
static void 	insert_startnode(node)
GNODE node;
#endif
{
	GNODE h,*hp;

	assert((node));
	debugmessage("insert_startnode",(NTITLE(node)?NTITLE(node):"(null)"));

	/* delete the node from the nodelist */
	if (NNEXT(node))   NBEFORE(NNEXT(node)) = NBEFORE(node);
	else 		   nodelistend 	  	= NBEFORE(node);
	if (NBEFORE(node)) NNEXT(NBEFORE(node)) = NNEXT(node);
	else 		   nodelist 	  	= NNEXT(node);

	/* search insertion point */
	h  = startnodes;
	hp = &startnodes;
	while (h) {
		if (NREFNUM(h)>=NREFNUM(node)) break;
		hp = &(NNEXT(h));
		h  = NNEXT(h);
	}

	/* insert the node into the startlist just before h */
	*hp = node;
	if (h) 	NBEFORE(node) = NBEFORE(h);
	else	NBEFORE(node) = startnodesend; 
	NNEXT(node)   = h;
	if (h)  NBEFORE(h)    = node;
	else    startnodesend = node;
}


static GNODE endnodes;
static GNODE endnodesend;

/*  Insert a node into the endnodes
 *  -------------------------------
 *  The endnodes are sorted according NREFNUMS.
 */ 

#ifdef ANSI_C
static void 	insert_endnode(GNODE node)
#else
static void 	insert_endnode(node)
GNODE node;
#endif
{
	GNODE h,*hp;

	assert((node));
	debugmessage("insert_endnode",(NTITLE(node)?NTITLE(node):"(null)"));

	/* delete the node from the nodelist */
	if (NNEXT(node))   NBEFORE(NNEXT(node)) = NBEFORE(node);
	else 		   nodelistend 	  	= NBEFORE(node);
	if (NBEFORE(node)) NNEXT(NBEFORE(node)) = NNEXT(node);
	else 		   nodelist 	  	= NNEXT(node);

	/* search insertion point */
	h  = endnodes;
	hp = &endnodes;
	while (h) {
		if (NREFNUM(h)>=NREFNUM(node)) break;
		hp = &(NNEXT(h));
		h  = NNEXT(h);
	}

	/* insert the node into the endlist just before h */
	*hp = node;
	if (h) 	NBEFORE(node) = NBEFORE(h);
	else	NBEFORE(node) = endnodesend; 
	NNEXT(node)   = h;
	if (h)  NBEFORE(h)    = node;
	else    endnodesend = node;
}



/*  Prepare nodelist to have startnodes in front
 *  ============================================
 */

#ifdef ANSI_C
static void	prepare_startnodes(void)
#else
static void	prepare_startnodes()
#endif
{
	GNODE	node,nxt_node;

	debugmessage("prepare_startnodes","");
	startnodes = NULL;
	startnodesend = NULL;
	endnodes = NULL;
	endnodesend = NULL;
	
	/* create the list of start nodes and end nodes */
	node 	 = nodelist;
	while (node) {
		nxt_node = NNEXT(node);
		if (!NPRED(node)) 	insert_startnode(node);
		else if (!NSUCC(node)) 	insert_endnode(node);
		node = nxt_node;
	}

	/* and insert the start node list just in front of nodelist */
	if (startnodes) {
		if (nodelist) NBEFORE(nodelist) = startnodesend;
		NNEXT(startnodesend) = nodelist;
		nodelist = startnodes;
		if (!nodelistend) nodelistend = startnodesend;
	}

	/* and insert this end node list just at the end of nodelist */
	if (endnodes) {
		if (nodelistend) NNEXT(nodelistend) = endnodes;
		NBEFORE(endnodes) = nodelistend;
		nodelistend = endnodesend;
		if (!nodelist) nodelist = endnodes;
	}
}	

/*--------------------------------------------------------------------*/
/*  Anchored edges                                                    */
/*--------------------------------------------------------------------*/

/*  Treatment of anchored edges
 *  ===========================
 *  For anchored edges, we create an anchor dummy node, that is 
 *  connected with the source. All edges are replaced by edges to
 *  the anchor node. 
 *  The CONNECT fields of source node and anchor node are filled.
 *  Note: The use of anchored edges restricts the use of nearedges.
 */

#ifdef ANSI_C
void insert_anchor_edges(void)
#else
void insert_anchor_edges()
#endif
{
	GEDGE   edge;

	debugmessage("insert_anchor_edges","");
	assert((dummylist==NULL));
	edge = edgelist;
	while (edge) {
		if (  (EANCHOR(edge)<=64) && (EANCHOR(edge)>0)
		    &&(!EINVISIBLE(edge))) 
			prepare_anchoredge(edge);
		edge = ENEXT(edge);
	}
	edge = tmpedgelist;
	while (edge) {
		if (  (EANCHOR(edge)<=64) && (EANCHOR(edge)>0)
		    &&(!EINVISIBLE(edge))) 
			prepare_anchoredge(edge);
		edge = EINTERN(edge);
	}
}

/*  Prepare one anchor edge
 *  -----------------------
 *  If the anchoredge starts on a node that has already an anchornode,
 *  we redirect the edge to point from this anchornode.
 *  Otherwise we create the anchornode and redirect the edge, too. 
 */

#ifdef ANSI_C
static void prepare_anchoredge(GEDGE edge)
#else
static void prepare_anchoredge(edge)
GEDGE edge;
#endif
{
	GEDGE   h;
	GNODE   v;
	CONNECT c1,c2;

	debugmessage("prepare_anchoredge","");
	if ((G_orientation==LEFT_TO_RIGHT)||(G_orientation==RIGHT_TO_LEFT)) {
		G_orientation= TOP_TO_BOTTOM;
		if (!silent) {	
			FPRINTF(stderr,"Orientation ignored, because");
			FPRINTF(stderr," edge attribute `anchor' used !\n");
		}
	}
	c1 = NCONNECT(ESTART(edge));
	if (!c1) { 
		v = create_dummy(-1);
		NINVISIBLE(v) = 0;
		NLEVEL(v) = NLEVEL(ESTART(edge));
		NHORDER(v) = NHORDER(ESTART(edge));
		NANCHORNODE(v) = 1;
		h = tmpedgealloc(
			ELSTYLE(edge),		/* not used later */
			ETHICKNESS(edge),	/* not used later */
			ECLASS(edge),
			200,
			ECOLOR(edge),		/* not used later */
			ELABELCOL(edge),	/* not used later */
			0,
			EARROWBSIZE(edge),	/* not used later */
			ASNONE,	
			EARROWBSTYLE(edge), 	/* not used later */
			EARROWCOL(edge),	/* not used later */
			EARROWBCOL(edge),	/* not used later */
			EHORDER(edge));
		EANCHOR(h) = 66;
		ESTART(h) = ESTART(edge);
		EEND(h)   = v;
		ELABEL(h) = NULL;
		c1 = connectalloc(ESTART(h));
		CTARGET(c1) = v;
		CEDGE(c1)   = h;
		c2 = connectalloc(v);
		CTARGET(c2) = ESTART(h);
		CEDGE(c2)   = h;
	}
	v = CTARGET(c1); 		
	assert((v));
	assert((NANCHORNODE(v)));
	h = tmpedgealloc(
		ELSTYLE(edge),
		ETHICKNESS(edge),
		ECLASS(edge),
		EPRIO(edge),
		ECOLOR(edge),
		ELABELCOL(edge),
		EARROWSIZE(edge),
		EARROWBSIZE(edge),
		EARROWSTYLE(edge),
		EARROWBSTYLE(edge),
		EARROWCOL(edge),
		EARROWBCOL(edge),
		EHORDER(edge));
	EANCHOR(h) = -EANCHOR(edge);
	ESTART(h) = v;
	EEND(h)   = EEND(edge);
	ELABEL(h) = ELABEL(edge);
	delete_adjedge(edge);
	EINVISIBLE(edge) = 0;
	create_adjedge(h);
}


/*--------------------------------------------------------------------*/
/*  Back edge preparation                                             */
/*--------------------------------------------------------------------*/

/*  Treatment of back edge
 *  ======================
 *  Back edges are reverted before the partitioning.  Thus there stay
 *  preferably in the reverted direction during the partitioning.
 *  But "back edge" is no hard constraint, i.e. it may be the case
 *  that the edge is again re-reverted during the partitioning.
 *  This is necessary since for instance a cycle of back edges cannot 
 *  be visualized without at least one edge which is not backwards.  
 */

#ifdef ANSI_C
void prepare_back_edges(void)
#else
void prepare_back_edges()
#endif
{
	ADJEDGE e;
	ADJEDGE a1, a2;

	debugmessage("prepare_back_edges","");
	e = back_edge_list;
	while (e) {
		if (!EINVISIBLE(AKANTE(e))) revert_edge(AKANTE(e));
		else if (ELNODE(AKANTE(e))) {
			a1 = NSUCC(ELNODE(AKANTE(e)));
			a2 = NPRED(ELNODE(AKANTE(e)));
			if (a1) 
				if (!EINVISIBLE(AKANTE(a1))) 
					revert_edge(AKANTE(a1));
			if (a2) 
				if (!EINVISIBLE(AKANTE(a2))) 
					revert_edge(AKANTE(a2));
		}
		e = ANEXT(e);
	}
}


/*--------------------------------------------------------------------*/
/*  Bent near edge connections                                        */
/*--------------------------------------------------------------------*/

/*  Treatment of bent near edge
 *  ===========================
 *  If edges are specified as bentnearedges, we create a dummy node,
 *  which is connected to the source by the CONNECT field, and an
 *  edge starting at this dummy node to the target.
 *  All bent_near_edges are in the bent_near_edge_list. The CONNECT fields
 *  for them are filled here. Later, we will fill additional CONNECT
 *  fields if it happen to be possible to neighbour directly some
 *  nodes that are connected by usual edges.
 */

#ifdef ANSI_C
static void	insert_bent_near_edges(void)
#else
static void	insert_bent_near_edges()
#endif
{
	GNODE   v;
	ADJEDGE edge1;
	ADJEDGE edge;
	CONNECT c1,c2;
	int connection_possible, invisible;

	debugmessage("insert_bent_near_edges","");
	edge1 = bent_near_edge_list;
	while (edge1) {

		invisible = EINVISIBLE(AKANTE(edge1));
		v = ELNODE(AKANTE(edge1));
		if (v) {
			edge = NSUCC(v);
			if (!edge) invisible = 1;
			else invisible = EINVISIBLE(AKANTE(edge));
			edge = NPRED(v);
			if (!edge) invisible |= 1;
			else invisible |= EINVISIBLE(AKANTE(edge));
		} 
		else if (!invisible) {
			if (G_displayel==YES) 
				v = create_labelnode(AKANTE(edge1));
			else {  v = create_dummy(-1);
				NINVISIBLE(v) = 0;
			}
			NLEVEL(v) = NLEVEL(SOURCE(edge1));
			NHORDER(v) = NHORDER(SOURCE(edge1));
			edge = create_edge(v,TARGET(edge1),AKANTE(edge1),1);
			ELABEL(AKANTE(edge)) = NULL;
			edge = create_edge(SOURCE(edge1),v,AKANTE(edge1),0);
			ELABEL(AKANTE(edge)) = NULL;
			EPRIO(AKANTE(edge)) = 0;
			delete_adjedge(AKANTE(edge1));
			EINVISIBLE(AKANTE(edge1)) = 0;
		}

		if (!invisible) {
			c1 = NCONNECT(SOURCE(edge));
			c2 = NCONNECT(TARGET(edge));
			connection_possible = 1;
			if ((c1) && (CTARGET(c1)) && (CTARGET2(c1))) 
				connection_possible = 0;
			if ((c2) && (CTARGET(c2)) && (CTARGET2(c2))) 
				connection_possible = 0;

			if (check_connect_cycle(TARGET(edge),NULL,
						SOURCE(edge))) 
				connection_possible = 0;

			if (connection_possible) {
				if (!c1) {
					c1 = connectalloc(SOURCE(edge));
					CTARGET(c1) = TARGET(edge);
					CEDGE(c1)   = AKANTE(edge);
				}
				else if (!CTARGET2(c1)) {
					CTARGET2(c1) = TARGET(edge);
					CEDGE2(c1)   = AKANTE(edge);
				}
				if (!c2) {
					c2 = connectalloc(TARGET(edge));
					CTARGET(c2) = SOURCE(edge);
					CEDGE(c2)   = AKANTE(edge);
				}
				else if (!CTARGET2(c2)) {
					CTARGET2(c2) = SOURCE(edge);
					CEDGE2(c2)   = AKANTE(edge);
				}
				delete_adjedge(AKANTE(edge));
				EINVISIBLE(AKANTE(edge)) = 0;
			}
			else if (!silent) {
				FPRINTF(stderr,"Nearedge connection (");
				if (NTITLE(SOURCE(edge))[0])
					FPRINTF(stderr,"%s",NTITLE(SOURCE(edge)));
				FPRINTF(stderr," , ");
				if (NTITLE(TARGET(edge))[0])
					FPRINTF(stderr,"%s",NTITLE(TARGET(edge)));
				FPRINTF(stderr,") ignored ! Sorry !\n");
			}
		}
		edge1 = ANEXT(edge1);
	}
}


/*--------------------------------------------------------------------*/
/*  Near edge connections                                             */
/*--------------------------------------------------------------------*/

/*  Treatment of near edge
 *  ======================
 *  If edges are specified as nearedges, they must be layouted at the
 *  same level. We use the connection field of nodes to notify this.
 *  All near_edges are in the near_edge_list. The CONNECT fields
 *  for them are filled here. Later, we will fill additional CONNECT
 *  fields if it happen to be possible to neighbour directly some
 *  nodes that are connected by usual edges.
 */

#ifdef ANSI_C
static void	insert_near_edges(void)
#else
static void	insert_near_edges()
#endif
{
	ADJEDGE edge;
	CONNECT c1,c2;
	int connection_possible;

	debugmessage("insert_near_edges","");
	edge = near_edge_list;
	while (edge) {
		if (!EINVISIBLE(AKANTE(edge))) { 
			c1 = NCONNECT(SOURCE(edge));
			c2 = NCONNECT(TARGET(edge));
			connection_possible = 1;
			if ((c1) && (CTARGET(c1)) && (CTARGET2(c1))) 
				connection_possible = 0;
			if ((c2) && (CTARGET(c2)) && (CTARGET2(c2))) 
				connection_possible = 0;

			if (check_connect_cycle(TARGET(edge),NULL,
						SOURCE(edge))) 
				connection_possible = 0;

			if (connection_possible) {
				if (!c1) {
					c1 = connectalloc(SOURCE(edge));
					CTARGET(c1) = TARGET(edge);
					CEDGE(c1)   = AKANTE(edge);
				}
				else if (!CTARGET2(c1)) {
					CTARGET2(c1) = TARGET(edge);
					CEDGE2(c1)   = AKANTE(edge);
				}
				if (!c2) {
					c2 = connectalloc(TARGET(edge));
					CTARGET(c2) = SOURCE(edge);
					CEDGE(c2)   = AKANTE(edge);
				}
				else if (!CTARGET2(c2)) {
					CTARGET2(c2) = SOURCE(edge);
					CEDGE2(c2)   = AKANTE(edge);
				}
				if (  (NLEVEL(SOURCE(edge))>=0) 
				    &&(NLEVEL(TARGET(edge))>=0) 
				    &&(NLEVEL(SOURCE(edge))!=
				    		NLEVEL(TARGET(edge))>=0) ) {
					if (!silent) {
		FPRINTF(stderr,"Nearedge connection (");
		if (NTITLE(SOURCE(edge))[0])
			FPRINTF(stderr,"%s",NTITLE(SOURCE(edge)));
		FPRINTF(stderr," , ");
		if (NTITLE(TARGET(edge))[0])
			FPRINTF(stderr,"%s",NTITLE(TARGET(edge)));
		FPRINTF(stderr,"): level of target ignored ! Sorry !\n");
					}
				}
				if (NLEVEL(SOURCE(edge))>=0) {
				    	NLEVEL(TARGET(edge)) = 
				    		NLEVEL(SOURCE(edge)); 
				} 
				if (NLEVEL(TARGET(edge))>=0) {
				    	NLEVEL(SOURCE(edge)) = 
				    		NLEVEL(TARGET(edge)); 
				} 

				delete_adjedge(AKANTE(edge));
				EINVISIBLE(AKANTE(edge)) = 0;
			}
			else if (!silent) {
				FPRINTF(stderr,"Nearedge connection (");
				if (NTITLE(SOURCE(edge))[0])
					FPRINTF(stderr,"%s",NTITLE(SOURCE(edge)));
				FPRINTF(stderr," , ");
				if (NTITLE(TARGET(edge))[0])
					FPRINTF(stderr,"%s",NTITLE(TARGET(edge)));
				FPRINTF(stderr,") ignored ! Sorry !\n");
			}
		}
		edge = ANEXT(edge);
	}
}


/* Check the connection chain for cycles 
 * -------------------------------------
 * We want to insert (z,v) as connection. We check
 * whether this yield a connection cycle, i.e. whether
 * z is reacheable from v. w is the node we were just 
 * previously.
 * We return 1, if z is reacheavle.
 */

#ifdef ANSI_C
static int  check_connect_cycle(GNODE v, GNODE w, GNODE z)
#else
static int  check_connect_cycle(v,w,z)
GNODE v;
GNODE w;
GNODE z;
#endif
{
	CONNECT c;
	int r;

	debugmessage("check_connect_cycle","");

	if (!near_edge_layout) return(1);
	if (!v) return(0);
	if (v==z) return(1);
	r = 0;
	c = NCONNECT(v);
	if (!c) return(0);
	if (CTARGET(c) && (CTARGET(c)!=w)) 
		r |= check_connect_cycle(CTARGET(c),v,z);
	if (CTARGET2(c) && (CTARGET2(c)!=w)) 
		r |= check_connect_cycle(CTARGET2(c),v,z);
	return(r);
}


/*--------------------------------------------------------------------*/
/*  Partition of edges with tree traversal                            */
/*--------------------------------------------------------------------*/

/*  Partition of edges, Driver
 *  ==========================
 *  We calculate by a dfs the spanning tree of the graph to detect:
 *     1) self loops     (v,v)
 *     2) tree edges     (v,w) that are part of the spanning tree
 *     3) forward edges  (v,w) if there is a path on the spanning tree 
 *                             from v to w.
 *     4) backward edges (v,w) if there is a path on the spanning tree
 *			       from w to v
 *     5) cross edges    (v,w) otherwise.
 *
 *  The algorithm is very popular known; e.g., it can be found in 
 *  K.Mehlhorn: Data Structures and Algorithms, Part II, Springer 1984.
 *  The spanning tree is the base of our layout. All nodes of the same
 *  tree depth will have the same vertical coordinates, i.e. they form
 *  a layer. IN FACT, WE ARE ONLY INTERESTED ON THIS DEPTH OF THE
 *  NODES, AND NOT ON THE PARTITIONING ITSELF. 
 *  As side effect, we calculate maxdepth, the maximal depth of the
 *  spanning tree.
 *
 *  If layout_flag 1|2 is specified, we do two partinitioning and take
 *  that one which has a larger|smaller depth. The difference between both is
 *  the order we analyse the edges. 
 */ 

static long	act_dfsnum;  /* actual ref. number of entry of dfs      */
static int	act_level;   /* actual level (depth) in spanning tree   */

#ifdef ANSI_C
static void partition_edges(void)
#else
static void partition_edges()
#endif
{
	GNODE	node;
	int	depth1;
 
	debugmessage("partition_edges","");

	/* First try */
	/* --------- */

    	act_dfsnum = 1L;
    	maxdepth = 0;		/* maximal depth of the spanning tree */
        
	gs_wait_message('p');
	node = nodelist;
	while (node) {
		if ( !NMARK(node) ) {
			act_level 	= 0; 
			depth_first_search(node);
        	}
		node = NNEXT(node);
   	} 

	/* labels are always reachaeble from other nodes, thus all label
 	 * nodes should already be marked now !!!
	 */

#ifdef CHECK_ASSERTIONS
	node = labellist;
	while (node)	{
		if ( !NMARK(node) ) assert((0));
		node = NNEXT(node);
   	} 
#endif

	/* maximize or minimize ? */
	if (layout_flag==12) return;
	if (layout_flag==TREE_LAYOUT) return;
	if (G_timelimit>0)  
		if (test_timelimit(30)) {
			gs_wait_message('t');
			return; 
	}


	/* Second try */
	/* ---------- */

       	depth1 = maxdepth;

	gs_wait_message('p');
	node = nodelist;
	while (node) {
		NMARK(node) = 0; 
		node = NNEXT(node);
   	} 
	node = labellist;
	while (node) {
		NMARK(node) = 0; 
		node = NNEXT(node);
   	} 
	node = dummylist;
	while (node) {
		NMARK(node) = 0; 
		node = NNEXT(node);
   	} 

    	act_dfsnum = 1L;
    	maxdepth = 0;		/* maximal depth of the spanning tree */
        
	node = nodelist;
	while (node) {
		if ( !NMARK(node) ) {
			act_level 	= 0; 
			alt_depth_first_search(node);
        	}
		node = NNEXT(node);
   	} 

	/* labels are always reachable from other nodes, thus all label
 	 * nodes should already be marked now !!!
	 */

#ifdef CHECK_ASSERTIONS
	node = labellist;
	while (node)	{
		if ( !NMARK(node) ) assert((0));
		node = NNEXT(node);
   	} 
#endif

	/* maximize ? */
	if ((layout_flag== 1)&&(depth1 <= maxdepth)) return;
	/* or minimize ? */
	if ((layout_flag== 2)&&(depth1 >= maxdepth)) return;

	/* First try was better: redo it */
	/* ----------------------------- */

	gs_wait_message('p');
	node = nodelist;
	while (node) {
		NMARK(node) = 0; 
		node = NNEXT(node);
   	} 
	node = labellist;
	while (node) {
		NMARK(node) = 0; 
		node = NNEXT(node);
   	} 
	node = dummylist;
	while (node) {
		NMARK(node) = 0; 
		node = NNEXT(node);
   	} 

    	act_dfsnum = 1L;
    	maxdepth = 0;		/* maximal depth of the spanning tree */
        
	node = nodelist;
	while (node) {
		if ( !NMARK(node) ) {
			act_level 	= 0; 
			depth_first_search(node);
        	}
		node = NNEXT(node);
   	} 

	/* labels are always reachaeble from other nodes, thus all label
 	 * nodes should already be marked now !!!
	 */


#ifdef CHECK_ASSERTIONS
	node = labellist;
	while (node)	{
		if ( !NMARK(node) ) assert((0));
		node = NNEXT(node);
   	} 
#endif

}



/*  The depth first search (1. version)
 *  ----------------------------------
 *  traverses the tree starting at node and does the partitioning.  
 */

#ifdef ANSI_C
static void depth_first_search(GNODE node)
#else
static void depth_first_search(node)
GNODE	node;
#endif
{
    	GNODE  	kn;
    	ADJEDGE	edge;
	int     priority;
	
	assert((node));
	debugmessage("depth_first_sea",(NTITLE(node)?NTITLE(node):"(null)"));
       	if (NMARK(node)) return;
 
	NMARK(node)  = 1;
       	NDFS(node)   = act_dfsnum++;
	if (NLEVEL(node)>=0) act_level = NLEVEL(node);
       	NTIEFE(node) = act_level;
       	maxdepth = (act_level>maxdepth) ? act_level : maxdepth;

	if (NCONNECT(node)) {
		/* Connections should be on the same level !!! 
		 * Note: Connections are never self loops.
		 */
		if (CTARGET(NCONNECT(node))) 
			depth_first_search(CTARGET(NCONNECT(node)));
		if (CTARGET2(NCONNECT(node))) 
			depth_first_search(CTARGET2(NCONNECT(node)));
	}

	priority = 1;
	while (priority>-1) {

		/* Fetch next priority value */
		priority = -1;
    		edge = NSUCC(node);
    		while (edge) {
			assert((SOURCE(edge)==node));
			kn = TARGET(edge);
			if ((!NMARK(kn)) && (EPRIO(AKANTE(edge))>priority)) 
				priority = EPRIO(AKANTE(edge));
			edge = ANEXT(edge);
		}
		if (priority== -1) break;

    		edge = NSUCC(node);
    		while (edge) {
			if (EPRIO(AKANTE(edge))!=priority) {
				edge = ANEXT(edge);
				continue;
			}
			assert((SOURCE(edge)==node));
			kn = TARGET(edge);
        		if ( !NMARK(kn) ) {
			       /*
        		 	* EKIND(edge) = 'T';
			 	*/
        			act_level++;
        			depth_first_search(kn);
				act_level = NTIEFE(node);
        		}
        		else { /* partitioning into forward, backward, cross 
				* and self edges,i.e. kinds 'F', 'B', 'C', 'S'.
				* Note NCOMP(kn)==0 implies that dfs(kn) is not 
				* yet completed, i.e. that after all dfs it will
				* be NCOMP(kn)>NCOMP(node).
				*
            			* if ( NDFS(node)<NDFS(kn) ) EKIND(edge)='F';
            			* else if (NDFS(node)>NDFS(kn) && NCOMP(kn)==0) 
                		* 	EKIND(edge) = 'B';
				* else if (kn == node) EKIND(edge) = 'S';
				* else EKIND(edge) = 'C';	
				*
				* The tags 'T', 'B', 'C' and 'F' are never used!
				* Thus it is nonsense to calculate them.
				* The tag 'S' is important.
				*/
				if (kn == node) EKIND(edge) = 'S';
        		}
            		edge = ANEXT(edge);
		}
	} /* while priority */

}
     	

/*  The depth first search (2. version)
 *  ----------------------------------
 *  traverses the tree starting at node and does the partitioning.  
 */

#ifdef ANSI_C
static void alt_depth_first_search(GNODE node)
#else
static void alt_depth_first_search(node)
GNODE	node;
#endif
{
	GNODE kn;
    	ADJEDGE	edge;
	int     priority;

	assert((node));
	debugmessage("alt_depth_first",(NTITLE(node)?NTITLE(node):"(null)"));
       	if (NMARK(node)) return;
 
	NMARK(node)  = 1;
       	NDFS(node)   = act_dfsnum++;
	if (NLEVEL(node)>=0) act_level = NLEVEL(node);
       	NTIEFE(node) = act_level;
       	maxdepth = (act_level>maxdepth) ? act_level : maxdepth;

	if (NCONNECT(node)) {
		/* Connections should be on the same level !!! 
		 * Note: Connections are never self loops.
		 */
		if (CTARGET(NCONNECT(node))) 
			alt_depth_first_search(CTARGET(NCONNECT(node)));
		if (CTARGET2(NCONNECT(node))) 
			alt_depth_first_search(CTARGET2(NCONNECT(node)));
	}
	priority = 1;
	while (priority>-1) {

		/* Fetch next priority value */
		priority = -1;
    		edge = NSUCC(node);
    		while (edge) {
			assert((SOURCE(edge)==node));
			kn = TARGET(edge);
			if ((!NMARK(kn)) && (EPRIO(AKANTE(edge))>priority)) 
				priority = EPRIO(AKANTE(edge));
			edge = ANEXT(edge);
		}
		if (priority== -1) break;

		start_dfs_backwards(NSUCC(node),node,priority);
	}
}


/*  Traverse of adjacency lists in alt_depth_first_search
 *  -----------------------------------------------------
 *  In alt_depth_first_search, we traverse the adjacency lists
 *  backwards.
 */

#ifdef ANSI_C
static void start_dfs_backwards(
	ADJEDGE edge,
	GNODE   node,
	int 	priority)
#else
static void start_dfs_backwards(edge,node,priority)
ADJEDGE edge;
GNODE   node;
int 	priority;
#endif
{
    	GNODE  	kn;

	debugmessage("start_dfs_backwards","");

	if (!edge) return;
	start_dfs_backwards(ANEXT(edge),node,priority);

	if (EPRIO(AKANTE(edge))!=priority) return;

	kn = TARGET(edge);
       	if ( !NMARK(kn) ) {
		/*
       		 * EKIND(edge) = 'T';
		 */
       		act_level++;
       		alt_depth_first_search(kn);
       		act_level = NTIEFE(node);
       	}
       	else { /* partitioning into forward, backward, cross and self
		* edges, i.e. kinds 'F', 'B', 'C', 'S'.
		* Note NCOMP(kn)==0 implies that dfs(kn) is not 
		* yet completed, i.e. that after all dfs it will
		* be NCOMP(kn)>NCOMP(node).
		*
           	* if ( NDFS(node) < NDFS(kn) ) EKIND(edge) = 'F';
           	* else if ( NDFS(node) > NDFS(kn) && NCOMP(kn) == 0) 
               	* 	EKIND(edge) = 'B';
		* else if (kn == node) EKIND(edge) = 'S';
		* else EKIND(edge) = 'C';	
		*
		* The tags 'T', 'B', 'C' and 'F' are never used !
		* Thus it is nonsense to calculate them.
		* The tag 'S' is important.
		*/
		if (kn == node) EKIND(edge) = 'S';
        }
}

/*--------------------------------------------------------------------*/
/*  Partition of edges with topological sorting                       */
/*--------------------------------------------------------------------*/

/* Topological sorting creates a layout with minimum backward edges.
 * If a topological order exists, we have no backward edge. Otherwise,
 * (on cyclic graphs) it results in a graph with minimal backward edges.
 *
 * We keep a list `zero_indegree_list' containing the nodes without
 * unmarked predecessor.
 * The zero_free_list contains these temporary cons cells for the
 * zero_indegree_list. These need not to be freed explicitely for
 * the relayout, see alloc.c.
 */

static GNLIST zero_indegree_list; 
static GNLIST zero_free_list; 

/* Add a node to the zero_indegree_list
 * ------------------------------------
 */

#ifdef ANSI_C
static void add_to_zero_indegree_list(GNODE v)
#else
static void add_to_zero_indegree_list(v)
GNODE v;
#endif
{
	GNLIST h;

	debugmessage("add_to_zero_indegree_list","");

	if (zero_free_list) {
		h = zero_free_list;
		zero_free_list = GNNEXT(zero_free_list);
	}
	else h = tmpnodelist_alloc();
	GNNODE(h) = v;
	GNNEXT(h) = zero_indegree_list;
	zero_indegree_list = h;
}

/* Delete the first entry of the zero_indegree_list
 * ------------------------------------------------
 * and return the node. Returns NULL on failure.
 */

#ifdef ANSI_C
static GNODE get_zero_indegree(void)
#else
static GNODE get_zero_indegree()
#endif
{
	GNLIST h;

	debugmessage("get_zero_indegree","");
	h = zero_indegree_list;
	if (h) {
		zero_indegree_list = GNNEXT(h);
		GNNEXT(h) = zero_free_list;
		zero_free_list = h;
		return(GNNODE(h));
	}
	return(NULL);
}


/* Topological sort try
 * ====================
 */

#ifdef ANSI_C
static void	topological_sort(void)
#else
static void	topological_sort()
#endif
{
	GNODE  v;
	int    not_ready, actlevel;

	debugmessage("topological_sort","");

	/* look for the nodes without predecessor and put them into
	 * the zero_indegree_list.
	 */ 

	zero_indegree_list = NULL;
  	zero_free_list     = NULL;  
	v = nodelist;
	while (v) {
		if (topsort_indegree(v,v)==0) add_to_zero_indegree_list(v);
		v = NNEXT(v);
	}
	/* Labels don't have zero indegree here */

    	maxdepth = 0;		/* maximal depth of the layout */
        
	gs_wait_message('p');
	not_ready = 1;
	while (not_ready) {
		/* First: the normal topological sorting algorithm
		 */
		while ((v = get_zero_indegree())!=NULL) {
			actlevel = topsort_maxlevel(v,v);
			if (NLEVEL(v)>=0) actlevel = NLEVEL(v);
			if (maxdepth<actlevel) maxdepth = actlevel;
			topsort_setlevel(v,v,actlevel);
			topsort_add_succs(v,v);
		}

		/* Now we have no nodes without predecessor, i.e. all
		 * remaining nodes are part of cycles.
		 * Check whether there are remainig parts.
		 */
		gs_wait_message('p');
		not_ready = 0;	
		v = nodelist;
		while (v && !not_ready) {
			if (!NMARK(v)) {
				add_to_zero_indegree_list(v); not_ready = 1;
			}
			v = NNEXT(v);
		}
		v = labellist;
		while (v && !not_ready) {
			if (!NMARK(v)) {
				add_to_zero_indegree_list(v); not_ready = 1;
			}
			v = NNEXT(v);
		}
		v = dummylist;
		while (v && !not_ready) {
			if (!NMARK(v)) {
				add_to_zero_indegree_list(v); not_ready = 1;
			}
			v = NNEXT(v);
		}
	}
}


/* Calculate the maximal predecessor level
 * ---------------------------------------
 * node1 is the node we want to analyze. node2 is the node we just come from.
 */

#ifdef ANSI_C
static int topsort_maxlevel(GNODE node1, GNODE node2)
#else
static int topsort_maxlevel(node1,node2)
GNODE node1;
GNODE node2;
#endif
{
	int result, h;
	ADJEDGE a;
	CONNECT c;

	debugmessage("topsort_maxlevel","");
	result = 0;
	a = NPRED(node1);
	while (a) {
		if (SOURCE(a)==node1) EKIND(a) = 'S';
		else if (NMARK(SOURCE(a))) {
			if (NTIEFE(SOURCE(a))>=result)
				result = NTIEFE(SOURCE(a))+1;
		}
		a = ANEXT(a);
	}
	c = NCONNECT(node1);
	if (c && CTARGET(c) && (CTARGET(c)!=node2)) {  
		h = topsort_maxlevel(CTARGET(c),node1);
		if (h>result) result = h;
	}
	if (c && CTARGET2(c) && (CTARGET2(c)!=node2)) {  
		h = topsort_maxlevel(CTARGET2(c),node1);
		if (h>result) result = h;
	}
	return(result);
}

/* Set NTIEFE of all nearedge node  
 * --------------------------------
 * node1 is the node we want to analyze. node2 is the node we just come from.
 */

#ifdef ANSI_C
static void topsort_setlevel(GNODE node1,GNODE node2,int level)
#else
static void topsort_setlevel(node1,node2,level)
GNODE node1;
GNODE node2;
int   level;
#endif
{
	CONNECT c;

	debugmessage("topsort_setlevel","");
	if ((NLEVEL(node1)>=0)&&(level!=NLEVEL(node1))) {
		if (!silent) {	
			FPRINTF(stderr,"Level specification ignored, ");
			FPRINTF(stderr,"because nearedge was specified !\n");
		}
	}
	NTIEFE(node1) = level;
	NMARK(node1) = 1;
	c = NCONNECT(node1);
	if (c && CTARGET(c) && (CTARGET(c)!=node2))  
		topsort_setlevel(CTARGET(c),node1,level);
	if (c && CTARGET2(c) && (CTARGET2(c)!=node2)) 
		topsort_setlevel(CTARGET2(c),node1,level);
}

/* Add successors to zero_indegree_list
 * ------------------------------------
 * If the successors of this node and all nodes connected to it 
 * by nearedges have nor unmarked predecessors, then we add
 * the successors to the zero_indegree_list.
 * node1 is the node we want to analyze. node2 is the node we just come from.
 */

#ifdef ANSI_C
static void topsort_add_succs(GNODE node1, GNODE node2)
#else
static void topsort_add_succs(node1,node2)
GNODE node1;
GNODE node2;
#endif
{
	ADJEDGE a;
	CONNECT c;

	debugmessage("topsort_add_succs","");

	a = NSUCC(node1);
	while (a) {
		if (  (topsort_indegree(TARGET(a),TARGET(a))==0)
		    &&(!NMARK(TARGET(a))) ) 
			add_to_zero_indegree_list(TARGET(a));
		a = ANEXT(a);
	}
	c = NCONNECT(node1);
	if (c && CTARGET(c) && (CTARGET(c)!=node2))  
		topsort_add_succs(CTARGET(c),node1);
	if (c && CTARGET2(c) && (CTARGET2(c)!=node2)) 
		topsort_add_succs(CTARGET2(c),node1);
}


/* Check how many predecessors of a node are marked
 * ------------------------------------------------
 * returns the number of unmarked indegrees
 * node1 is the node we want to analyze. node2 is the node we just come from.
 */

#ifdef ANSI_C
static int topsort_indegree(GNODE node1,GNODE node2)
#else
static int topsort_indegree(node1,node2)
GNODE node1;
GNODE node2;
#endif
{
	int result;
	ADJEDGE a;
	CONNECT c;

	debugmessage("topsort_indegree","");
	result = 0;
	a = NPRED(node1);
	while (a) {
		if ( !NMARK(SOURCE(a)) ) result++;
		a = ANEXT(a);
	}
	c = NCONNECT(node1);
	if (c && CTARGET(c) && (CTARGET(c)!=node2))  
		result += topsort_indegree(CTARGET(c),node1);
	if (c && CTARGET2(c) && (CTARGET2(c)!=node2)) 
		result += topsort_indegree(CTARGET2(c),node1);
	return(result);
}


/*--------------------------------------------------------------------*/
/*  Partition of edges with strongly connected components             */
/*--------------------------------------------------------------------*/

static GNLIST global_node_list; 


/* Add a node to the list l 
 * ------------------------
 */

#ifdef ANSI_C
static void add_to_nlist(GNODE v, GNLIST *l)
#else
static void add_to_nlist(v, l)
GNODE v;
GNLIST *l;
#endif
{
	GNLIST h;

	debugmessage("add_to_nlist","");

	if (zero_free_list) {
		h = zero_free_list;
		zero_free_list = GNNEXT(zero_free_list);
	}
	else h = tmpnodelist_alloc();
	GNNODE(h) = v;
	GNNEXT(h) = *l;
	*l = h;
}


/* Delete the first entry of the list l
 * ------------------------------------
 * and return the node. Returns NULL on failure.
 */

#ifdef ANSI_C
static GNODE get_nlist(GNLIST *l)
#else
static GNODE get_nlist(l)
GNLIST *l;
#endif
{
	GNLIST h;

	debugmessage("get_nlist","");
	h = *l;
	if (h) {
		*l = GNNEXT(h);
		GNNEXT(h) = zero_free_list;
		zero_free_list = h;
		return(GNNODE(h));
	}
	return(NULL);
}


/* Strongly connected component try
 * ================================
 */

#ifdef ANSI_C
static void	sc_component_sort(void)
#else
static void	sc_component_sort()
#endif
{
	GNODE  v;

	debugmessage("sc_component_sort","");

  	zero_free_list   = NULL;  
	global_node_list = NULL; 

    	maxdepth = 1;		/* maximal depth of the layout */

	v = nodelist;
	while (v) { if (!NINVISIBLE(v)) add_to_nlist(v, &global_node_list);
		    NTIEFE(v) = 1;
		    v = NNEXT(v);
	}
	v = labellist;
	while (v) { add_to_nlist(v, &global_node_list);
		    NTIEFE(v) = 1;
		    v = NNEXT(v);
	}
	v = dummylist;
	while (v) { add_to_nlist(v, &global_node_list);
		    NTIEFE(v) = 1;
		    v = NNEXT(v);
	}

	/* Calc connected components */

	calc_sc_component(&global_node_list);

}


/* Calculation of the strongly connected components of nlist 
 * ---------------------------------------------------------
 */


#ifdef ANSI_C
static void	calc_sc_component(GNLIST *nlist)
#else
static void	calc_sc_component(nlist)
GNLIST *nlist;
#endif
{
	GNODE  v;
	GNLIST h;
	GNLIST open_scc_list;
	long   mydfsnum;

	debugmessage("sc_component_sort","");

#ifdef SCCDEBUG
	PRINTF("Calc SCC:\n");
#endif

	h = *nlist;
	while (h) {
		v = GNNODE(h);
		NMARK(v) = NTIEFE(v) = 0;
		NLOWPT(v) = NOPENSCC(v) = 0L;
#ifdef SCCDEBUG
		PRINTF("[%ld|%s] ", v,(NTITLE(v)?NTITLE(v):"null"));
#endif
		h = GNNEXT(h);
	}
#ifdef SCCDEBUG
	PRINTF("\n");
#endif
	open_scc_list = NULL;
	mydfsnum = 0;

	gs_wait_message('p');

	/* It holds:
	 * all nodes in nlist have      NTIEFE = 0.
	 * all nodes outside nlist have NTIEFE != 0.
	 */

	v = get_nlist(nlist);
	while (v) {
		scc_traversal(v,&mydfsnum,&open_scc_list);
		v = get_nlist(nlist);
	}
}



/*  The strongly connected component traversal
 *  ------------------------------------------
 *  is also a backward depth first search with some additions.
 */

#ifdef ANSI_C
static void scc_traversal(GNODE node, long *dfsnum, GNLIST *open_sccp)
#else
static void scc_traversal(node, dfsnum, open_sccp)
GNODE	node;
long *dfsnum;
GNLIST *open_sccp;
#endif
{
    	GNODE  	kn;
	GNLIST  h;
	GNLIST  closed_scc_list;
    	ADJEDGE	edge;
	int mylevel;
	GNODE   actrev;
	int	degree;
	int 	minindeg;
	int	maxoutdeg;
	int	maxpreindeg;
	int 	minlevel;

	assert((node));
	debugmessage("scc_traversal",(NTITLE(node)?NTITLE(node):"(null)"));
       	if (NMARK(node)) return;
 
	NMARK(node)     = 1;
	NOPENSCC(node)  = 1L;
       	NDFS(node)      = *dfsnum;
       	NLOWPT(node)    = *dfsnum;
	(*dfsnum)++;

	add_to_nlist(node, open_sccp);

	if (NCONNECT(node)) {
		/* Connections should be on the same level !!! 
		 * Note: Connections are never self loops.
		 */
		if (CTARGET(NCONNECT(node))) {
			kn = CTARGET(NCONNECT(node));
			scc_traversal(kn,dfsnum,open_sccp);
			if ((NOPENSCC(kn)) && (NLOWPT(kn)<NLOWPT(node))) 
				NLOWPT(node) = NLOWPT(kn);
		}
		if (CTARGET2(NCONNECT(node))) {
			kn = CTARGET2(NCONNECT(node));
			scc_traversal(kn,dfsnum,open_sccp);
			if ((NOPENSCC(kn)) && (NLOWPT(kn)<NLOWPT(node))) 
				NLOWPT(node) = NLOWPT(kn);
		}
	}

    	edge = NPRED(node);
    	while (edge) {
		assert((TARGET(edge)==node));
		kn = SOURCE(edge);
		if (NTIEFE(kn)==0) {
			/* The nodes with NTIEFE != 0 are not in the
			 * actual list of nodes to be inspected.
			 */
			scc_traversal(kn,dfsnum,open_sccp);
			if ((NOPENSCC(kn)) && (NLOWPT(kn)<NLOWPT(node))) 
				NLOWPT(node) = NLOWPT(kn);
		}
		edge = ANEXT(edge);
	}

	if (NLOWPT(node) == NDFS(node)) {

		/* split the scc from the open_scc list */

		h = closed_scc_list = *open_sccp;
		assert((h));
		kn = GNNODE(h);
		while (kn!=node) {
			NOPENSCC(kn) = 0L;
			h = GNNEXT(h);
			assert((h));
			kn = GNNODE(h);
		}
		assert((kn==node));
		NOPENSCC(node) = 0L;

		*open_sccp = GNNEXT(h);
		GNNEXT(h) = 0;

#ifdef SCCDEBUG
		PRINTF("Test SCC:\n");
		h = closed_scc_list;
		while (h) {
			kn = GNNODE(h);
			PRINTF("[%ld|%s] ", kn,(NTITLE(kn)?NTITLE(kn):"null"));
			h = GNNEXT(h);
		}
#endif

		/* Calculate the maximal predecessor NTIEFE for all
		 * nodes of the SCC.
		 */

		minlevel = -1;
		h = closed_scc_list;
		while (h) {
			node = GNNODE(h);
    			edge = NPRED(node);
			while (edge) {
				kn = SOURCE(edge);
				mylevel = NTIEFE(kn);	
				if (mylevel > minlevel) minlevel = mylevel;
				edge = ANEXT(edge);
			}
			h = GNNEXT(h);
		}

#ifdef SCCDEBUG
		PRINTF(" minlevel: %d\n",minlevel);
#endif


		/* If the SCC has only one element, 
		 * or each element of SCC is connected by nearedges,
		 * we can assign the level.
		 */

		assert((closed_scc_list));
		degree = complete_scc(closed_scc_list);

		if (degree) {
#ifdef SCCDEBUG
			PRINTF("Next complete SCC:\n");
#endif
			minlevel++;
			kn = get_nlist(&closed_scc_list);
			while (kn) {
				if (NLEVEL(kn)>=0) NTIEFE(kn) = NLEVEL(kn);
				else NTIEFE(kn) = minlevel;
       				maxdepth = (NTIEFE(kn)>maxdepth) ? 
						NTIEFE(kn) : maxdepth;
#ifdef SCCDEBUG
				PRINTF("[%ld|%s] (%d) (max %d)\n", kn,
					(NTITLE(kn)?NTITLE(kn):"null"),
					NTIEFE(kn), maxdepth);
#endif
				kn = get_nlist(&closed_scc_list);
			}
			return;
		}


		/* It holds for all nodes v in nlist:
		 *    NTIEFE(v) != 0   <=>  v is in another closed component
		 *		            except closed_scc_list.
		 *    NTIEFE(v) == 0   <=>  v is in closed_scc_list   or
		 *			    v is not reachable from node.
		 * 
		 * Note: if v is not reachable from node, then v is either
		 * in a open component or v is not yet inspected. 
		 */


		/* Mark all nodes of this SCC.
		 * We can use the NTIEFE field because this is reinitialized
		 * just at the beginning of calc_sc_component.
		 */

		h = closed_scc_list;
		while (h) {
			NTIEFE(GNNODE(h)) = MAXINT;
			h = GNNEXT(h);
		}

		/* Calculate node actrev which predecessor edges must be
		 * reverted.
		 */

		actrev = GNNODE(closed_scc_list);
		minindeg    = MAXINT;
		maxoutdeg   = 0;
		maxpreindeg = 0;
		h = closed_scc_list;
		while (h) {
			node = GNNODE(h);
			degree = scc_outdeg(node, NULL, 1); 
			if (degree < minindeg) {
				minindeg = degree;
				actrev = node;
				h = GNNEXT(h);
				continue;
			}
			else if (degree > minindeg) {
				h = GNNEXT(h);
				continue;
			}

			/* now, scc_outdeg == minindeg */

			degree = scc_indeg(node, NULL); 
			if (degree > maxoutdeg) {
				maxoutdeg = degree;
				actrev = node;
				h = GNNEXT(h);
				continue;
			}
			else if (degree < maxoutdeg) {
				h = GNNEXT(h);
				continue;
			}

			/* scc_outdeg == minindeg and scc_indeg == maxoutdeg */

			degree = scc_succoutdeg(node, NULL); 
			if (degree > maxpreindeg) {
				maxpreindeg = degree;
				actrev = node;
			}
			
			h = GNNEXT(h);
		}

		/* Revert all predecessor edges of the SCC of actrev. */

#ifdef SCCDEBUG
		PRINTF("Revert Preds on [%d|%s] %d %d %d\n", actrev,
				(NTITLE(actrev)?NTITLE(actrev):"null"),
				minindeg, maxoutdeg, maxpreindeg);
#endif

		revert_outedges(actrev, NULL);

		/* and now try to calculate the SCC again. 
		 */

		calc_sc_component(&closed_scc_list);
	}
}


/* Check whether a strongly connected component can be closed.
 * ----------------------------------------------------------
 * This is the case if either all nodes have specified levels
 * or it is only one node, or it is one connected chain of nearedges.
 * Return 1 in this case.
 */

#ifdef ANSI_C
static int complete_scc(GNLIST nlist)
#else
static int complete_scc(nlist)
GNLIST nlist;
#endif
{
	GNODE v;
	GNLIST h;
	int res, count;
	CONNECT c;

	debugmessage("complete_scc","");

	assert((nlist));
	count = 0;
	res = 1;
	h = nlist;
	while (h) {
		count++;
		if (NLEVEL(GNNODE(h))<0) res = 0;
		h = GNNEXT(h);
	}

	if (res) return(1);
	if (count==1) return(1);

	h = nlist;
	while (h) {
		assert((NMARK(GNNODE(h))==1));
		NMARK(GNNODE(h)) = 0;
		h = GNNEXT(h);
	}

	count = 1;
	NMARK(GNNODE(nlist)) = 1;
	while (count) {
		count = 0;
		h = nlist;
		while (h) {
			v = GNNODE(h);
			if (NMARK(v)) {
				c = NCONNECT(v);
				if ((c)&&CEDGE(c)) {
					if (!NMARK(ESTART(CEDGE(c)))) count++;
					if (!NMARK(EEND(CEDGE(c))))   count++;
					NMARK(ESTART(CEDGE(c))) = 1;
					NMARK(EEND(CEDGE(c))) = 1;
				}
				if ((c)&&CEDGE2(c)) {
					if (!NMARK(ESTART(CEDGE2(c)))) count++;
					if (!NMARK(EEND(CEDGE2(c))))   count++;
					NMARK(ESTART(CEDGE2(c))) = 1;
					NMARK(EEND(CEDGE2(c))) = 1;
				}
			}	
			h = GNNEXT(h);
		}	
	}

	res = 1;
	h = nlist;
	while (h) {
		if (!NMARK(GNNODE(h))) { res = 0; break; }
		h = GNNEXT(h);
	}

	h = nlist;
	while (h) {
		NMARK(GNNODE(h)) = 1;
		h = GNNEXT(h);
	}
	return(res);	
}


/* Calculate the outdegree of node and its neighbours
 * --------------------------------------------------
 * This is done relatively to the actual SCC, which is marked by NTIEFE=MAXINT.
 * w is the node we just were previously, in order to avoid cycling
 * along the CTARGET's.
 */

#ifdef ANSI_C
static int scc_outdeg(GNODE v, GNODE w, int prio) 
#else
static int scc_outdeg(v, w, prio)
GNODE v;
GNODE w; 
int prio;
#endif
{

	int degree;
	ADJEDGE e;
	CONNECT c;

	degree = 0;
    	e = NSUCC(v);
	while (e) {
		if (NTIEFE(TARGET(e))==MAXINT) {
			/* if NTIEFE<MAXINT, it is not part 
		   	 * of the closed_scc_list.
		  	 */
			if (prio)
				degree+=EPRIO(AKANTE(e));
			else 	degree++;
		}
		e = ANEXT(e);
	}
	c = NCONNECT(v);
	if (!c) return(degree);

	if (CTARGET(c) && (w!=CTARGET(c))) 
		degree += scc_outdeg(CTARGET(c),v, prio);
	if (CTARGET2(c) && (w!=CTARGET2(c))) 
		degree += scc_outdeg(CTARGET2(c),v, prio);

	return(degree);
}


/* Calculate the outdegree of successors of a node and its neighbours
 * -------------------------------------------------------------------
 * This is done relatively to the actual SCC, which is marked by NTIEFE=MAXINT.
 * w is the node we just were previously, in order to avoid cycling
 * along the CTARGET's.
 */

#ifdef ANSI_C
static int scc_succoutdeg(GNODE v, GNODE w) 
#else
static int scc_succoutdeg(v, w)
GNODE v;
GNODE w; 
#endif
{

	int degree;
	ADJEDGE e;
	CONNECT c;

	degree = 0;
    	e = NSUCC(v);
	while (e) {
		if (NTIEFE(TARGET(e))==MAXINT) {
			/* if NTIEFE<MAXINT, it is not part 
		   	 * of the closed_scc_list.
		  	 */
			degree += scc_outdeg(SOURCE(e),NULL,0);
		}
		e = ANEXT(e);
	}
	c = NCONNECT(v);
	if (!c) return(degree);

	if (CTARGET(c) && (w!=CTARGET(c))) 
		degree += scc_succoutdeg(CTARGET(c),v);
	if (CTARGET2(c) && (w!=CTARGET2(c))) 
		degree += scc_succoutdeg(CTARGET2(c),v);

	return(degree);
}




/* Calculate the outdegree of node and its neighbours
 * --------------------------------------------------
 * This is done relatively to the actual SCC, which is marked by NTIEFE=MAXINT.
 * w is the node we just were previously, in order to avoid cycling
 * along the CTARGET's.
 */

#ifdef ANSI_C
static int scc_indeg(GNODE v, GNODE w) 
#else
static int scc_indeg(v, w)
GNODE v;
GNODE w; 
#endif
{

	int degree;
	ADJEDGE e;
	CONNECT c;

	degree = 0;
    	e = NPRED(v);
	while (e) {
		if (NTIEFE(SOURCE(e))==MAXINT) {
			/* if NTIEFE<MAXINT, it is not part 
		   	 * of the closed_scc_list.
		  	 */
			degree++;
		}
		e = ANEXT(e);
	}
	c = NCONNECT(v);
	if (!c) return(degree);

	if (CTARGET(c) && (w!=CTARGET(c))) 
		degree += scc_indeg(CTARGET(c),v);
	if (CTARGET2(c) && (w!=CTARGET2(c))) 
		degree += scc_indeg(CTARGET2(c),v);

	return(degree);
}


/* Revert all outgoinging edges at node and its neighbours
 * -------------------------------------------------------
 * This is done relatively to the actual SCC, which is marked by NTIEFE=MAXINT.
 * w is the node we just were previously, in order to avoid cycling
 * along the CTARGET's.
 */

#ifdef ANSI_C
static void	revert_outedges(GNODE v, GNODE w) 
#else
static void	revert_outedges(v, w)
GNODE v;
GNODE w; 
#endif
{
	ADJEDGE e, en;
	CONNECT c;

    	e = NSUCC(v);
	while (e) {
		en = ANEXT(e);
		if (NTIEFE(TARGET(e))==MAXINT) {
			/* if NTIEFE<MAXINT, it is not part 
		   	 * of the closed_scc_list.
		  	 */
			revert_edge(AKANTE(e));
		}
		e = en;
	}
	c = NCONNECT(v);
	if (!c) return;

	if (CTARGET(c) && (w!=CTARGET(c))) 
		revert_outedges(CTARGET(c),v);
	if (CTARGET2(c) && (w!=CTARGET2(c))) 
		revert_outedges(CTARGET2(c),v);
}



/*--------------------------------------------------------------------*/
/*  Add labels after partitioning                                     */
/*--------------------------------------------------------------------*/

/* If we still don't have the labels, we double the NTIEFE of the nodes
 * and add the labels in between.
 */

#ifdef ANSI_C
static void 	add_phase1_labels(void)
#else
static void 	add_phase1_labels()
#endif
{
	GNODE v, vl, vt;
	ADJEDGE edge, edgenext;

	debugmessage("add_phase1_labels","");

	/* In order to avoid that the level of a node is double duplicated,
	 * we mark the nodes.
	 */
	v = nodelist;
	while (v) { NMARK(v) = 0; v = NNEXT(v); }
	v = dummylist;
	while (v) { NMARK(v) = 0; v = NNEXT(v); }
	v = labellist;
	while (v) { NMARK(v) = 0; v = NNEXT(v); }

	maxdepth = 2*maxdepth;
	v = nodelist;
	while (v) {
		if (!NMARK(v)) { NTIEFE(v) = 2* NTIEFE(v); NMARK(v) = 1; }
		else assert((0));;
		v = NNEXT(v);
	}	
	v = dummylist;
	while (v) {
		if (!NMARK(v)) { NTIEFE(v) = 2* NTIEFE(v); NMARK(v) = 1; }
		else assert((0));;
		v = NNEXT(v);
	}	
	v = labellist;
	while (v) {
		if (!NMARK(v)) { NTIEFE(v) = 2* NTIEFE(v); NMARK(v) = 1; }
		else assert((0));;
		v = NNEXT(v);
	}	

	v = nodelist;
	while (v) {
		edge = NSUCC(v); 
		while (edge) {
			edgenext = ANEXT(edge);
			if (ELABEL(AKANTE(edge))) {
				vl = create_labelnode(AKANTE(edge));
				vt = TARGET(edge);
				NTIEFE(vl) = ( NTIEFE(SOURCE(edge))
					      +NTIEFE(TARGET(edge)))/2;
				(void)create_edge(v,vl,AKANTE(edge),0);
				(void)create_edge(vl,vt,AKANTE(edge),1);
				delete_adjedge(AKANTE(edge));
			}
			edge = edgenext;
		}
		v = NNEXT(v);
	}	
	v = tmpnodelist;
	while (v) {
		edge = NSUCC(v); 
		while (edge) {
			edgenext = ANEXT(edge);
			if (ELABEL(AKANTE(edge))) {
				vl = create_labelnode(AKANTE(edge));
				vt = TARGET(edge);
				NTIEFE(vl) = ( NTIEFE(SOURCE(edge))
					      +NTIEFE(TARGET(edge)))/2;
				(void)create_edge(v,vl,AKANTE(edge),0);
				(void)create_edge(vl,vt,AKANTE(edge),1);
				delete_adjedge(AKANTE(edge));
			}
			edge = edgenext;
		}
		v = NNEXT(v);
	}	
}


/*--------------------------------------------------------------------*/
/*  Relook the partitioning                                           */
/*--------------------------------------------------------------------*/

/*  A second analysis of the partitioning
 *  =====================================
 *  It may happen, that after partitioning of edges, a node is at 
 *  depth i, but all its predecessors are at depth i-1 and all
 *  successors are at depth  i+n. Then it is better to put such
 *  nodes into depth i+n/2, to support that all edges have nearly
 *  the same length.
 *  E.g.:    A --> B ---------------> C  is changed into
 *           A --------> B ---------> C.
 *
 *  A second task here is to push label nodes into an upper depth
 *  if the target of the edge the label belongs to is in an upper
 *  depth.
 *  Note: We do NEVER change the depth of a node that has a connection
 *  field, because this would destroy the nearedge relation. 
 *  It gives no sense to do this too often. Thus we stop after 50 times,
 *  even if the result is still not optimal.
 */

#ifdef ANSI_C
static void tune_partitioning(void)
#else
static void tune_partitioning()
#endif
{
	GNODE v;
	int   changed,count;

	debugmessage("tune_partitioning","");

	count   = 0;
	changed = 1;
	while (changed) {
		changed = 0;
		gs_wait_message('p');

		if (G_timelimit>0)  
			if (test_timelimit(30)) {
				gs_wait_message('t');
				return; 
		}

		/* First, check normal nodes */
		v = nodelist;
		while (v) {
			if (!NCONNECT(v)) changed += tune_node_depth(v,0); 
			v = NNEXT(v);
		}	

		/* Then, check labels */
		v = labellist;
		while (v) {
			if (!NCONNECT(v)) changed += tune_node_depth(v,1); 
			v = NNEXT(v);
		}	
		count++;
		if (count>=50) return;
	}
} 


/*  Check tuning of the depth of a node v
 *  -------------------------------------
 *  This node has no nearedge, thus we can change its depth NTIEFE.
 */

#ifdef ANSI_C
static int tune_node_depth(GNODE v,int lab)
#else
static int tune_node_depth(v, lab)
GNODE v;
int lab;
#endif
{ 
	int   nodelevel,leveldiff,nr_edges, nr_redges, changed, delta, hdelta;
	ADJEDGE edge, hedge;
	GNODE hh;

	debugmessage("tune_node_depth","");

	/*  If the node has a fixed specified level, we should not
	 *  change it.
	 */

	if (NLEVEL(v)>=0) return(0);

	/*  Calculate the average level difference between the node
 	 *  and all nodes adjacent to it. We have to ensure that
	 *  nodes that have no successor edges are not drawn too much
	 *  to the predecessors. Thus we set the delta. However, if
	 *  the predecessors or successors are already at the same level,
	 *  they can also stay at the same level. In this case, adding delta
	 *  can be avoided. 
	 */

	nr_redges  = nr_edges = leveldiff = 0;
	nodelevel = NTIEFE(v);
	if ((!NSUCC(v)) || (!NPRED(v))) delta = 1;
	else delta = 0;

    	edge = NPRED(v);
	while (edge) {
		nr_edges += EPRIO(AKANTE(edge));
		nr_redges++;
		if (NTIEFE(SOURCE(edge))!=nodelevel)
			leveldiff += (EPRIO(AKANTE(edge))*
			  	(NTIEFE(SOURCE(edge))-nodelevel+delta));
		edge = ANEXT(edge);
	}
    	edge = NSUCC(v);
    	while (edge) {
		nr_edges += EPRIO(AKANTE(edge));
		nr_redges++;
		if (NTIEFE(TARGET(edge))!=nodelevel)
			leveldiff += (EPRIO(AKANTE(edge))*
				(NTIEFE(TARGET(edge))-nodelevel-delta));
		edge = ANEXT(edge);
	}

	/*  Nodes without edges should be at position 0.
	 */

	if (nr_redges==0) {
		NTIEFE(v) = 0;
		return(0);
	}

	/*  If the edges of a node have no priority, the node need not to
	 *  be changed.
	 */

	if (nr_edges==0) return(0);

	/*  Calculate the new level.
	 */

	changed = 0;
	if (leveldiff/nr_edges <= -2) {
		changed = 1;
		nodelevel += (leveldiff/nr_edges);
	}
	else if (leveldiff/nr_edges >  0) { 
		changed = 1;
		nodelevel += (leveldiff/nr_edges);
	}

#ifdef TUNEDEBUG
	PRINTF("Wish %sto move \"%s\" from level %d to level %d\n",
		(changed? "" : "not "),   (NTITLE(v)?NTITLE(v):"(null)"),
		NTIEFE(v), nodelevel);
#endif

	if (!changed) return(0);

	if ((layout_flag == 3) || (layout_flag == 0)) {

		/* On normal and minbackward, we must ensure that this does 
		 * not create additional backward edges.
	 	 */

		if (near_edge_layout) delta = 0; else delta = 1;

		if (nodelevel > NTIEFE(v)) {
                	edge = NSUCC(v);
                	while (edge) {
                        	if (   (NTIEFE(TARGET(edge))>NTIEFE(v)) 
                           	    && (nodelevel>=NTIEFE(TARGET(edge))))
					nodelevel = NTIEFE(TARGET(edge))-delta; 
                        	edge = ANEXT(edge);
                	}
                	edge = NPRED(v);
                	while (edge) {
                       		if (   (NTIEFE(SOURCE(edge))>NTIEFE(v)) 
                        	    && (nodelevel>=NTIEFE(SOURCE(edge))))
					nodelevel = NTIEFE(SOURCE(edge))-delta; 
                        	edge = ANEXT(edge);
                	}
			if (nodelevel <= NTIEFE(v)) return(0);
		}
	    	if (nodelevel < NTIEFE(v)) {
                	edge = NSUCC(v);
                	while (edge) {
                        	if (   (NTIEFE(TARGET(edge))<NTIEFE(v)) 
                        	    && (nodelevel<=NTIEFE(TARGET(edge))))
					nodelevel = NTIEFE(TARGET(edge))+delta; 
                        	edge = ANEXT(edge);
                	}
                	edge = NPRED(v);
                	while (edge) {
                        	if (   (NTIEFE(SOURCE(edge))<NTIEFE(v)) 
                        	    && (nodelevel<=NTIEFE(SOURCE(edge))))
					nodelevel = NTIEFE(SOURCE(edge))+delta; 
                        	edge = ANEXT(edge);
                	}
			if (nodelevel >= NTIEFE(v)) return(0);
		}

		/* For the edges that are originally not reverted, check 
		 * whether they are still not reverted. But we allow to 
		 * re-revert edges such that they are now normal again.
		 */
                edge = NPRED(v);
                while (edge) {
			if (EKIND(edge)!='R') {
                        	if (  (nodelevel< NTIEFE(SOURCE(edge))) 
                    	            &&(NTIEFE(v)>=NTIEFE(SOURCE(edge))) )
					return(0);
			}
                        edge = ANEXT(edge);
                }
                edge = NSUCC(v);
                while (edge) {
			if (EKIND(edge)!='R') {
                        	if (  (nodelevel> NTIEFE(TARGET(edge))) 
                            	    &&(NTIEFE(v)<=NTIEFE(TARGET(edge))) )
					return(0);
			}
                        edge = ANEXT(edge);
                }
	}
	else {  /*  For tree layout, it is not allowed to create nearedges
		 *  where previously no nearedges existed.
		 */

                edge = NPRED(v);
                while (edge) {
                        if (nodelevel==NTIEFE(SOURCE(edge))) return(0);
                        edge = ANEXT(edge);
                }
                edge = NSUCC(v);
                while (edge) {
                        if (nodelevel==NTIEFE(TARGET(edge))) return(0);
                        edge = ANEXT(edge);
                }
        }

	/*  Each node can only have two neighbors. Here we check that
	 *  the node at the new position is maximal two neighbours,
	 *  and that these nodes currently have maximal 1 neighbour.
	 */
 
	delta = 0;
        edge = NPRED(v);
        while (edge) {
		hh = SOURCE(edge);
                if (nodelevel==NTIEFE(hh)) {
			delta++;
			if (NCONNECT(hh)) return(0);
			hdelta = 0;
        		hedge = NPRED(hh);
        		while (hedge) {
				if (NTIEFE(hh)==NTIEFE(SOURCE(hedge))) hdelta++;
                		hedge = ANEXT(hedge);
			}
        		hedge = NSUCC(hh);
        		while (hedge) {
				if (NTIEFE(hh)==NTIEFE(TARGET(hedge))) hdelta++;
                		hedge = ANEXT(hedge);
			}
			if (hdelta>1) return(0);
		}
                edge = ANEXT(edge);
        }
        edge = NSUCC(v);
        while (edge) {
		hh = TARGET(edge);
                if (nodelevel==NTIEFE(hh)) {
			delta++;
			if (NCONNECT(hh)) return(0);
			hdelta = 0;
        		hedge = NPRED(hh);
        		while (hedge) {
				if (NTIEFE(hh)==NTIEFE(SOURCE(hedge))) hdelta++;
                		hedge = ANEXT(hedge);
			}
        		hedge = NSUCC(hh);
        		while (hedge) {
				if (NTIEFE(hh)==NTIEFE(TARGET(hedge))) hdelta++;
                		hedge = ANEXT(hedge);
			}
			if (hdelta>1) return(0);
		}
                edge = ANEXT(edge);
        }
	if (delta>2) return(0);

	/* Now, nearly everything is okay. Final checks.
	 */

	if (nodelevel<0) return(0);
	if (nodelevel>maxdepth) return(0);

#ifdef TUNEDEBUG
	PRINTF("Move \"%s\" from level %d to level %d\n",
		(NTITLE(v)?NTITLE(v):"(null)"), NTIEFE(v), nodelevel);
#endif
	if (NTIEFE(v)!=nodelevel) changed = 1;
	NTIEFE(v) = nodelevel;
	return(changed);
}


/*--------------------------------------------------------------------*/
/*  Fill the array layer                                              */
/*--------------------------------------------------------------------*/

/*  Layer list creation 
 *  ===================
 *  After depth first search and allocation of the array layer,
 *  we traverse all visible nodes and insert them into the array layer.
 *  Note that until now, we create and use only the TSUCC list and
 *  not the TPRED list.
 */
 
#ifdef ANSI_C
static void create_depth_lists(void)
#else
static void create_depth_lists()
#endif
{
	GNODE h;
	GNLIST hl;
	int t;

	debugmessage("create_depth_lists","");

	h = nodelist;
	while (h) {
		t = NTIEFE(h);
		assert((t<=maxdepth));
		hl = tmpnodelist_alloc();
		GNNEXT(hl) = TSUCC(layer[t]);
		TSUCC(layer[t]) = hl;
        	GNNODE(hl) = h;
		h = NNEXT(h);
	}
	h = labellist;
	while (h) {
		t = NTIEFE(h);
		assert((t<=maxdepth+1));
		hl = tmpnodelist_alloc();
		GNNEXT(hl) = TSUCC(layer[t]);
		TSUCC(layer[t]) = hl;
        	GNNODE(hl) = h;
		h = NNEXT(h);
	}
	h = dummylist;
	while (h) {
		t = NTIEFE(h);
		assert((t<=maxdepth+1));
		hl = tmpnodelist_alloc();
		GNNEXT(hl) = TSUCC(layer[t]);
		TSUCC(layer[t]) = hl;
        	GNNODE(hl) = h;
		h = NNEXT(h);
	}
}


/*  Layer list completion
 *  ---------------------
 *  Because during `inspect_edges' the layers are changed by adding 
 *  dummy nodes, we have only used TSUCC of the layers. Now, at the
 *  end of this module, we derive the TPRED lists from the TSUCC
 *  lists. TPRED(layer[i]) is the reverted list of TSUCC(layer[i]), 
 *  without the connection nodes. E.g. in A->B->C, the TSUCC-list
 *  contains A, B and C but the TPRED-list only contains A.
 *  NINDEG and NOUTDEG are calculated here. 
 *
 *  If a node comes not into the TPRED-list because it has connections,
 *  we mark the node by NMARK.
 */

/* Abreviations to check whether a connection is a forward connection 
 * or backward connection.
 */

#ifdef ANSI_C
static void	complete_depth_lists(void)
#else
static void	complete_depth_lists()
#endif
{
	int 	i;
	GNLIST	n,hl;
	GNODE	node;
	ADJEDGE	edge;
	int	backward_connection;
	int	forward_connection;
	CONNECT c;

	debugmessage("complete_depth_lists","");

	maxindeg = maxoutdeg = 0;

	for (i=0; i<=maxdepth+1; i++) {
		n = TSUCC(layer[i]);
		while (n) {
			NMARK(GNNODE(n))=0;
			n = GNNEXT(n);
		}
		n = TSUCC(layer[i]);
		assert((TPRED(layer[i])==NULL));
		while (n) {
			node = GNNODE(n);

			edge = NSUCC(node);

			/* If there is a forward connection, save the
			 * adjacency lists. Check for backward connection.
			 */
			backward_connection = 0;
			forward_connection = 0;
			c = NCONNECT(node);
			if (c) {
				if (backward_connection1(c))
					backward_connection = 1;
				if (backward_connection2(c))
					backward_connection = 1;
				if (forward_connection1(c))
					forward_connection = 1;
				if (forward_connection2(c))
					forward_connection = 1;
			}
			if (  (forward_connection)&&(!backward_connection)
			    &&(NMARK(GNNODE(n))==0)) 
				calc_connect_adjlists(node,node,NULL);

			/* fill TPRED-list */
			if (  (!backward_connection)&&(NMARK(GNNODE(n))==0)) {
				hl = tmpnodelist_alloc();
				GNNEXT(hl) = TPRED(layer[i]);
				TPRED(layer[i]) = hl;
				GNNODE(hl) = node;
			}

			/* count the edges. Note: forward connections 
			 * have its successor and all successors of
			 * their direct neigbours. 
			 */
			edge = NSUCC(node);
			while (edge) {
				NOUTDEG(node)++;
				assert((NTIEFE(TARGET(edge))>=i));
				edge = ANEXT(edge);
			}
			edge = NPRED(node);
			while (edge) {
				NINDEG(node)++;
				edge = ANEXT(edge);
			}
			if (NOUTDEG(node) > maxoutdeg)
				maxoutdeg = NOUTDEG(node);
			if (NINDEG(node) > maxindeg)
				maxindeg = NINDEG(node);
			n = GNNEXT(n);
		}
	}
}


/*  Calculate adjacencies at connections
 *  ------------------------------------
 *  Add the adjacencies of a connection node w to the node v.
 *  We start these method by nodes that have only forward connections.
 *  Because connections are not cyclic, we will always find such nodes.
 *  predw indicated where we come, to prevent running in a cycle. 
 *  Further, we mark the node w by NMARK, to prevent to calculate again
 *  the connection of w.
 *  This changes destructively the SOURCE and TARGET-field of nodes.
 *  Note that at connection nodes SOURCE(NSUCC(w))==w and
 *  TARGET(NPRED(w))==w does NOT HOLD anymore after this.
 *
 *  The situation in an example:
 *  Before:   A      B     C             After   A  B  C
 *            |      |     |                      \ | /
 *            V      V     V                       \|/
 *            X----->Y---->Z                        X----->Y---->Z
 *             connections                           connections
 */

#ifdef ANSI_C
static void calc_connect_adjlists(GNODE v,GNODE w, GNODE predw)
#else
static void calc_connect_adjlists(v,w,predw)
GNODE v;
GNODE w;
GNODE predw;
#endif
{
	ADJEDGE	edge;
	CONNECT c;

	debugmessage("calc_connect_adjlists","");

	if (v!=w) NMARK(w)=1;
	/* save a copy of the actual adjacency lists of w to v */
	if (v==w) {
		NSVSUCC(w) = NSUCC(w);
		NSVPRED(v) = NPRED(v);
	}
	edge = NSUCC(w);
	if (v==w) NSUCC(v) = NULL;
	while (edge) {
		succedgealloc(v,AKANTE(edge));
		SOURCE(edge) = v;
		edge = ANEXT(edge);
	}
	edge = NPRED(w);
	if (v==w) NPRED(v) = NULL;
	while (edge) {
		prededgealloc(v,AKANTE(edge));
		TARGET(edge) = v;
		edge = ANEXT(edge);
	}

	c = NCONNECT(w);
	if (!c) return;
	if (CTARGET(c) && (CTARGET(c)!=predw)) 
		calc_connect_adjlists(v,CTARGET(c),w);
	if (CTARGET2(c) && (CTARGET2(c)!=predw)) 
		calc_connect_adjlists(v,CTARGET2(c),w);
}



/*--------------------------------------------------------------------*/
/*  Inspect edges                                                     */
/*--------------------------------------------------------------------*/

/*  Analyse whether the hierarchy is proper
 *  =======================================
 *  Note that dummy nodes are inserted at beginning of the layer[i]-lists
 *  i.e. are not automatically checked, if they are inserted at the
 *  actual or an upper layer. Thus we need recursion in check_edge.
 */ 

#ifdef ANSI_C
static void    inspect_edges(void)
#else
static void    inspect_edges()
#endif
{
	int     i;
	GNLIST	li;
	GNODE   node;
	ADJEDGE edge,nextedge;
	GNODE   d1, d2; 	/* for dummy nodes */
	ADJEDGE e1,e2,e3,e4;	/* for dummy edges */
	GEDGE   a1, a2;

	debugmessage("inspect_edges","");
 
	for (i=0; i<=maxdepth; i++) {
		li = TSUCC(layer[i]);
		while (li) {
			node = GNNODE(li);
			edge = NSUCC(node);
			while (edge) {
				/* Because check_edge may delete edge */
				assert((SOURCE(edge)==node));
				nextedge = ANEXT(edge);
				check_edge(node,edge,i);
				edge = nextedge;
			}
			li = GNNEXT(li);
		}
	}
	node = labellist;
	while (node) {
		edge = NSUCC(node);
		if (edge && (ANEXT(edge))) {
			a1 = AKANTE(edge);
			a2 = AKANTE(ANEXT(edge));
			d1 = create_dummy(NTIEFE(node));
			d2 = create_dummy(NTIEFE(node));
			e1 = create_edge(node,d1,a1,0);
			e2 = create_edge(d1,EEND(a1),a1,1);
			e3 = create_edge(node,d2,a2,0);
			e4 = create_edge(d2,EEND(a2),a2,1);
			check_edge(SOURCE(e1),e1,NTIEFE(node));
			check_edge(SOURCE(e2),e2,NTIEFE(node));
			check_edge(SOURCE(e3),e3,NTIEFE(node));
			check_edge(SOURCE(e4),e4,NTIEFE(node));
			delete_adjedge(a1);
			delete_adjedge(a2);
		}
		edge = NPRED(node);
		if (edge && (ANEXT(edge))) {
			a1 = AKANTE(edge);
			a2 = AKANTE(ANEXT(edge));
			d1 = create_dummy(NTIEFE(node));
			d2 = create_dummy(NTIEFE(node));
			e1 = create_edge(d1,node,a1,1);
			e2 = create_edge(ESTART(a1),d1,a1,0);
			e3 = create_edge(d2,node,a2,1);
			e4 = create_edge(ESTART(a2),d2,a2,0);
			check_edge(SOURCE(e1),e1,NTIEFE(node));
			check_edge(SOURCE(e2),e2,NTIEFE(node)-1);
			check_edge(SOURCE(e3),e3,NTIEFE(node));
			check_edge(SOURCE(e4),e4,NTIEFE(node)-1);
			delete_adjedge(a1);
			delete_adjedge(a2);
		}
		node = NNEXT(node);
	}
}

/*
 *  Check whether one edge is proper
 *  --------------------------------
 *  Source of the edge is node, which is at this level.
 *  Here is the reason why we need maximal maxdepth+1 layers.
 *  The layout of the nodes at level maxdepth inserts into
 *  the layer maxdepth+1 only dummy nodes, if necessary.
 *  Dummy nodes never cause the creation of an other dummy node. 
 *
 *  Upward edges are reverted.
 *  Edges over more than one level are splitted by dummy nodes.
 *  Edges at the same level are tried to neigbour their nodes directly. 
 *  If there are more than 3 such neighbours, direct neighbouring is 
 *  impossible, thus we use dummy nodes (see (1)). Note that direct 
 *  neigbours stay in the layer-list, but are not enymore in any 
 *  adjacency list. Self loops are layoutes with two dummy nodes (see (2)).
 *  Double edges in adjacent layers are layouted as one edge with
 *  two arrows (see (3)).
 *                                                    N
 *       A<--N-->B   C                  N             ^ 
 *            \     ^                  / ^            |
 *    (1)      \   /          (2)     /   \      (3)  |
 *               D                  D1----D2          v
 *                                                    M
 */

#ifdef ANSI_C
static void check_edge(GNODE node,ADJEDGE edge,int level)
#else
static void check_edge(node,edge,level)
GNODE node;
ADJEDGE edge;
int level;
#endif
{
	int edgelen;	/* length of the edge, i.e. difference of levels */
	int i,j;
	GNODE   d1, d2; 	/* for dummy nodes */
	ADJEDGE e1,e2,e3;	/* for dummy edges */
	CONNECT	c1,c2;
	int connection_possible, lab_set;

	debugmessage("check_edge","");
	assert((node));
	assert((edge));
	assert((AKANTE(edge)));
	assert((NTIEFE(node)==level));
	assert((SOURCE(edge)==node));

	edgelen = NTARTIEFE(edge) - level;
	if (edgelen < 0) { /* Revert edge and continue checking */
		e1 = revert_edge(AKANTE(edge));
		/* it may be an forward edge now */
		check_edge(SOURCE(e1),e1,NTIEFE(SOURCE(e1)));
	}
	else if (edgelen == 0) { /* edge at the same level */
		if (EKIND(edge)=='R') {
			e1 = revert_edge(AKANTE(edge));
			check_edge(SOURCE(e1),e1,NTIEFE(SOURCE(e1)));
		}
		else if (SOURCE(edge) == TARGET(edge)) { /* self loop */
			assert((TARGET(edge)==node));
			assert((level<=maxdepth));
			d1 = create_dummy(level+1);
			d2 = create_dummy(level+1);
			NHORDER(d1) = NHORDER(d2) = EHORDER(AKANTE(edge));
			e1 = create_edge(node,d1,AKANTE(edge),0);
			e2 = create_edge(d1,d2,  AKANTE(edge),2);
			e3 = create_edge(d2,node,AKANTE(edge),1);
			ELABEL(AKANTE(e2)) = ELABEL(AKANTE(edge));
			/* e1 is already okay: no recursion necessary  */
			check_edge(SOURCE(e2),e2,NTIEFE(SOURCE(e2)));
			check_edge(SOURCE(e3),e3,NTIEFE(SOURCE(e3)));
			delete_adjedge(AKANTE(edge));
		}
		else { /* no self loop: try to neighbour the nodes */
			c1 = NCONNECT(node);
			c2 = NCONNECT(TARGET(edge));
			connection_possible = 1;
			if ((c1) && (CTARGET(c1)) && (CTARGET2(c1))) 
				connection_possible = 0;
			if ((c2) && (CTARGET(c2)) && (CTARGET2(c2))) 
				connection_possible = 0;
			if (check_connect_cycle(TARGET(edge),NULL,
						SOURCE(edge))) 
				connection_possible = 0;
			if (connection_possible) {
				if (!c1) {
					c1 = connectalloc(node);
					CTARGET(c1) = TARGET(edge);
					CEDGE(c1)   = AKANTE(edge);
				}
				else if (!CTARGET2(c1)) {
					CTARGET2(c1) = TARGET(edge);
					CEDGE2(c1)   = AKANTE(edge);
				}
				if (!c2) {
					c2 = connectalloc(TARGET(edge));
					CTARGET(c2) = node;
					CEDGE(c2)   = AKANTE(edge);
				}
				else if (!CTARGET2(c2)) {
					CTARGET2(c2) = node;
					CEDGE2(c2)   = AKANTE(edge);
				}
				delete_adjedge(AKANTE(edge));
				EINVISIBLE(AKANTE(edge)) = 0;
			}
			else {
				if (level<=maxdepth) 
					d1 = create_dummy(level+1);
				else if (level>0) 
					d1 = create_dummy(level-1);
				
				else { assert((0)); }
				NHORDER(d1) = EHORDER(AKANTE(edge));
				e1 = create_edge(node,d1,AKANTE(edge),0);
				e2 = create_edge(d1,TARGET(edge),AKANTE(edge),1);
				ELABEL(AKANTE(e2)) = ELABEL(AKANTE(edge));
				check_edge(SOURCE(e1),e1,NTIEFE(SOURCE(e1)));
				check_edge(SOURCE(e2),e2,NTIEFE(SOURCE(e2)));
				delete_adjedge(AKANTE(edge));
			}
		}
	}
	else if (edgelen == 1) { /* edge at adjacent level */
		/* SUCH NODES ARE OKAY */
	}
	else if (edgelen > 1) { /* Forward edges etc. */
		d1 = node;
		j  = lab_set = 0;
		for (i=1; i<edgelen; i++) {
			d2 = create_dummy(level+i);
			NHORDER(d2) = EHORDER(AKANTE(edge));
			e1 = create_edge(d1,d2,AKANTE(edge),j);
			if (i==(edgelen+1)/2) {
				ELABEL(AKANTE(e1)) = ELABEL(AKANTE(edge));
				lab_set = 1;
			}
			/* e1 is already okay: no recursion necessary  */
			d1 = d2;
			j = 2;
		}
		e1 = create_edge(d1,TARGET(edge),AKANTE(edge),1);
		if (!lab_set) ELABEL(AKANTE(e1)) = ELABEL(AKANTE(edge));
		/* e1 is already okay: no recursion necessary  */
		delete_adjedge(AKANTE(edge));
	}
}


/*  Revert an edge
 *  --------------
 *  The edge is reused again and stays visible. But source and target 
 *  are exchanged, and the edge is marked by 'R'. The adjacency entry
 *  of the new source node is returned.
 */
 
#ifdef ANSI_C
ADJEDGE revert_edge(GEDGE edge)
#else
ADJEDGE revert_edge(edge)
GEDGE edge;
#endif
{
	GNODE h;
	char  hh;

	debugmessage("revert_edge","");
	delete_adjedge(edge);
	h = ESTART(edge); 
	ESTART(edge) = EEND(edge);
	EEND(edge) = h;
	if (EART(edge) == 'R')  EART(edge) = 'U';
	else if (EART(edge) == 'S')  EART(edge) = 'S';
	else EART(edge) = 'R';

	hh = EARROWSIZE(edge);
	EARROWSIZE(edge)  = EARROWBSIZE(edge);
	EARROWBSIZE(edge) = hh;
	hh = EARROWCOL(edge);
	EARROWCOL(edge)  = EARROWBCOL(edge);
	EARROWBCOL(edge) = hh;
	hh = EARROWSTYLE(edge);
	EARROWSTYLE(edge)  = EARROWBSTYLE(edge);
	EARROWBSTYLE(edge) = hh;

	create_adjedge(edge);

	/* Warning: We know from folding.c that this is the edge we just
 	 * created. But it is not quite obvious.
	 */

	return(NSUCC(ESTART(edge)));
}


/*  Create an additional edge
 *  -------------------------
 *  between the nodes `start' and `end'. The attributes of this
 *  new edge are derive from 'edge'.
 *  arrow = 0 means the target is a dummy node, i.e. no arrow is needed there. 
 *  arrow = 1 means the source is a dummy node, i.e. no arrow is needed there. 
 *  arrow = 2 means both source and target are dummy nodes.
 *  arrow = 3 means both source and target are no dummy nodes.
 */

#ifdef ANSI_C
static ADJEDGE	create_edge(GNODE start,GNODE end,GEDGE edge,int arrow)
#else
static ADJEDGE	create_edge(start, end, edge,arrow)
GNODE	start;
GNODE  	end;
GEDGE   edge;
int	arrow;
#endif
{
	GEDGE	h;
	
	h = tmpedgealloc(
		ELSTYLE(edge),
		ETHICKNESS(edge),
		ECLASS(edge),
		EPRIO(edge),
		ECOLOR(edge),
		ELABELCOL(edge),
		EARROWSIZE(edge),
		EARROWBSIZE(edge),
		EARROWSTYLE(edge),
		EARROWBSTYLE(edge),
		EARROWCOL(edge),
		EARROWBCOL(edge),
		EHORDER(edge));
	ELABEL(h) = ELABEL(edge);
	EANCHOR(h) = EANCHOR(edge);
	ESTART(h) = start;
	EEND(h)	  = end;

	switch (arrow) {
	case 0: EARROWSTYLE(h) = ASNONE;
		EARROWSIZE(h)  = 0;
		ELABEL(h)      = NULL;
		break;
	case 1: EARROWBSTYLE(h) = ASNONE;
		EARROWBSIZE(h)  = 0;
		ELABEL(h)       = NULL;
		break;
	case 2: EARROWSTYLE(h)  = ASNONE;
		EARROWSIZE(h)   = 0;
		ELABEL(h)       = NULL;
	        EARROWBSTYLE(h) = ASNONE;
		EARROWBSIZE(h)  = 0;
		ELABEL(h)       = NULL;
		break;
	}

	if (EART(edge)=='S') EART(h) = 'U';
	else		     EART(h) = EART(edge);
	if (start==end) EART(h) = 'S';
	create_adjedge(h);

	/* Warning: We know from alloc.c that this is the edge we just
 	 * created. But it is not quite obvious.
	 */

	return(NSUCC(ESTART(h)));
}



/*  Create a dummy node
 *  -------------------
 *  returns a new dummy node that is inserted at layer[t].
 *  A dummy node is always a temporary node.
 */

#ifdef ANSI_C
static GNODE create_dummy(int t)
#else
static GNODE create_dummy(t)
int t;
#endif
{
	GNLIST hl;
	GNODE  v;

	debugmessage("create_dummy","");
	assert((t<=maxdepth+1));

	v = tmpnodealloc(CENTER,-1,-1,0,-1,G_color,G_color,G_color,1,1,-1);
	NTITLE(v)   = "";
	NLABEL(v)   = "";
	NTIEFE(v)   = t;
	NHORDER(v)  = -1;
	NBEFORE(v)      = NULL;
	NNEXT(v)        = dummylist;
	if (dummylist) NBEFORE(dummylist) = v;
	dummylist       = v;
	dummyanz++;

	/* if t appropriate, insert dummy node into the layer[t] */
	if (t<0) return(v);
	hl = tmpnodelist_alloc();
	GNNEXT(hl) = TSUCC(layer[t]);
	TSUCC(layer[t]) = hl;
       	GNNODE(hl) = v;
	return(v);
}


/*--------------------------------------------------------------------*/
/*  Check double edges                                                */
/*--------------------------------------------------------------------*/

/*  Check doublicated edges
 *  -----------------------
 *  check whether an edge is double in the adjacency list.
 *  Important are only the doublicated edges from one level
 *  to the next level, to recognize short biconnected edges.
 *  This is basically the same as split_double_edges and
 *  summarize_edges (see folding.c) in one function.
 */


#ifdef ANSI_C
static void	inspect_double_edges(void)
#else
static void	inspect_double_edges()
#endif
{
	int     i;
	GNLIST	li;
	GNODE   node;
	ADJEDGE edge,nextedge;

	debugmessage("inspect_double_edge","");
	for (i=0; i<=maxdepth; i++) {
		li = TSUCC(layer[i]);
		while (li) {
			node = GNNODE(li);
			edge = NSUCC(node);
			while (edge) {
				/* Because check_double_edge may delete edge */
				assert((SOURCE(edge)==node));
				nextedge = ANEXT(edge);
				check_double_edge(edge);
				edge = nextedge;
			}
			li = GNNEXT(li);
		}
	}
}


/*  Check whether a doublicate of edge exist
 *  ----------------------------------------
 *  If yes, then check whether both can be summarized.
 *  If no,  then split the edge. Note: this splitting may introduce 
 *  new edges, but none of these new edges can be doublicated. 
 *  Thus, no recursion is necessary.
 */


#ifdef ANSI_C
static void	check_double_edge(ADJEDGE edge)
#else
static void	check_double_edge(edge)
ADJEDGE	edge;
#endif
{
	ADJEDGE	l, lnext;
	GNODE   d1; 	/* for dummy nodes */
	ADJEDGE e1;	/* for dummy edges */
	GEDGE   f1, f2;
	int 	ide, aside1, aside2, tide;

	debugmessage("check_double_edge","");

	f1 = AKANTE(edge);
	l = NSUCC(SOURCE(edge));
	while (l) {
		lnext = ANEXT(l);

		f2 = AKANTE(l);
		if (f1!=f2) { tide = ide = aside1 = aside2 = 1; }
		else { tide = ide = aside1 = aside2 = 0; }

                if (ESTART(f1)      != ESTART(f2))       tide=0;
                if (EEND(f1)        != EEND(f2))         tide=0;
                if (ELSTYLE(f1)     != ELSTYLE(f2))      ide=0;
                if (ETHICKNESS(f1)  >  ETHICKNESS(f2))   ide=0;
                if (EPRIO(f1)       >  EPRIO(f2))        ide=0;
                if (EHORDER(f1)     != EHORDER(f2))      ide=0;
                if (ECLASS(f1)      != ECLASS(f2))       ide=0;
                if (ECOLOR(f1)      != ECOLOR(f2))       ide=0;
                if (EARROWSIZE(f1)  != EARROWSIZE(f2))   aside1=0;
                if (EARROWBSIZE(f1) != EARROWBSIZE(f2))  aside2=0;
                if (EARROWSTYLE(f1) != EARROWSTYLE(f2))  aside1=0;
                if (EARROWBSTYLE(f1)!= EARROWBSTYLE(f2)) aside2=0;
                if (EARROWCOL(f1)   != EARROWCOL(f2))    aside1=0;
                if (EARROWBCOL(f1)  != EARROWBCOL(f2))   aside2=0;
                if (EANCHOR(f1)     != EANCHOR(f2))      ide=0;

                if (tide && ide && aside1&&aside2 && summarize_double_edges) { 
			delete_adjedge(f1); 
			return; 
		}

		if (tide) {
			d1 = create_dummy(NTIEFE(TARGET(l)));
			NHORDER(d1) = EHORDER(f1);
			e1 = create_edge(SOURCE(edge),d1,f1,0);
			check_edge(SOURCE(e1),e1,NTIEFE(SOURCE(e1)));
			e1 = create_edge(d1,TARGET(edge),AKANTE(edge),1);
			ELABEL(AKANTE(e1)) = ELABEL(f1);
			check_edge(SOURCE(e1),e1,NTIEFE(SOURCE(e1)));

			/* The double edge is not anymore a 
			 * double edge. Thus the original edge
			 * can be removed.
			 */
			delete_adjedge(f1); 
			return; 
		}

		l = lnext;
	}
}


/*--------------------------------------------------------------------*/
/*  Calculation of the number of reverted edges                       */
/*--------------------------------------------------------------------*/

int number_reversions;

#ifdef ANSI_C
void calc_number_reversions(void)
#else
void calc_number_reversions()
#endif
{
	GNODE v;
	ADJEDGE e;

	debugmessage("calc_number_reversions","");

	number_reversions = 0;

	v = nodelist;
	while (v) {
		e = NPRED(v);
		while (e) {
			if (EKIND(e)=='R') number_reversions++;
			e = ANEXT(e);
		}
		v = NNEXT(v);
	}
	v = labellist;
	while (v) {
		e = NPRED(v);
		while (e) {
			if (EKIND(e)=='R') number_reversions++;
			e = ANEXT(e);
		}
		v = NNEXT(v);
	}
	v = dummylist;
	while (v) {
		e = NPRED(v);
		while (e) {
			if (EKIND(e)=='R') number_reversions++;
			e = ANEXT(e);
		}
		v = NNEXT(v);
	}
}


/*--------------------------------------------------------------------*/
/*  Debugging routines                                                */
/*--------------------------------------------------------------------*/

/*  The following routines are normally not used, but are provided
 *  to simplify debugging later on.
 */

/*  Output the graph textually
 *  ==========================
 */

#ifdef DEBUG

#ifdef ANSI_C
void db_output_graph(void)
#else
void db_output_graph()
#endif
{
	PRINTF("Nodes:\n");
	db_output_nodes(nodelist);
	PRINTF("Labels:\n");
	db_output_nodes(labellist);
	PRINTF("Dummynodes:\n");
	db_output_nodes(dummylist);
	PRINTF("\n\n");
	PRINTF("Edges:\n");
	db_output_edges(edgelist); 
	PRINTF("Temporary edges:\n");
	db_output_edges(tmpedgelist); 
}
 
#endif


/*  Output the nodes 
 *  ----------------
 */

#ifdef DEBUG

#ifdef ANSI_C
static void db_output_nodes(GNODE n)
#else
static void db_output_nodes(n)
GNODE   n;
#endif
{
	if (!n) return; 
	db_output_nodes(NNEXT(n));
	PRINTF("%s, ", NTITLE(n));
}

#endif


/*  Output the edges 
 *  ----------------
 */

#ifdef DEBUG

#ifdef ANSI_C
static void db_output_edges(GEDGE e)
#else
static void db_output_edges(e)
GEDGE   e;
#endif
{
 	if (!e) return; 
	db_output_edges(ENEXT(e));
	if (EINVISIBLE(e)==0)
		PRINTF("(%s -> %s)\n", ESTART(e)->title, EEND(e)->title);
}

#endif


/*  Output the graph in adjacency representation
 *  ============================================
 */
         
#ifdef DEBUG

#ifdef ANSI_C
void db_output_adjacencies(void)
#else
void db_output_adjacencies()
#endif
{
	GNODE	node;
	ADJEDGE	edge;

	PRINTF("\n\nAdjacency lists: ");
	node = nodelist;
	while (node) {
		PRINTF("\n%s(%d)%ld\n", NTITLE(node), NTIEFE(node),node);
		PRINTF("(in:%d,out:%d)\n", NINDEG(node), NOUTDEG(node));
		PRINTF("Succs:");
		edge = NSUCC(node);
		while (edge) {
			PRINTF("|%s(%ld) ", NTITLE(TARGET(edge)), TARGET(edge));
			edge = ANEXT(edge);
		}
		PRINTF("\nPreds:");
		edge = NPRED(node);
		while (edge) {
			PRINTF("|%s(%ld) ", NTITLE(SOURCE(edge)), SOURCE(edge));
            		edge = ANEXT(edge); 
        	}
		node = NNEXT(node);
	}
	node = labellist;
	while (node) {
		PRINTF("\n%s(%d)%ld\n", NTITLE(node), NTIEFE(node),node);
		PRINTF("(in:%d,out:%d)\n", NINDEG(node), NOUTDEG(node));
		PRINTF("Succs:");
		edge = NSUCC(node);
		while (edge) {
			PRINTF("|%s(%ld) ", NTITLE(TARGET(edge)), TARGET(edge));
			edge = ANEXT(edge);
		}
		PRINTF("\nPreds:");
		edge = NPRED(node);
		while (edge) {
			PRINTF("|%s(%ld) ", NTITLE(SOURCE(edge)), SOURCE(edge));
            		edge = ANEXT(edge); 
        	}
		node = NNEXT(node);
	}
	node = dummylist;
	while (node) {
		PRINTF("\n%s(%d)%ld\n", NTITLE(node), NTIEFE(node),node);
		PRINTF("(in:%d,out:%d)\n", NINDEG(node), NOUTDEG(node));
		PRINTF("Succs:");
		edge = NSUCC(node);
		while (edge) {
			PRINTF("|%s(%ld) ", NTITLE(TARGET(edge)), TARGET(edge));
			edge = ANEXT(edge);
		}
		PRINTF("\nPreds:");
		edge = NPRED(node);
		while (edge) {
			PRINTF("|%s(%ld) ", NTITLE(SOURCE(edge)), SOURCE(edge));
            		edge = ANEXT(edge); 
        	}
		node = NNEXT(node);
	}
	PRINTF("\n");
}

#endif

/*  Output one node in adjacency representation
 *  ===========================================
 *  f = 0 => only succs, f = 1 => only preds
 */
         
#ifdef DEBUG

#define gtitle(v)  (NTITLE(v)?NTITLE(v):"??")

#ifdef ANSI_C
void db_output_adjacency(GNODE node,int f)
#else
void db_output_adjacency(node,f)
GNODE node;
int f;
#endif
{
	ADJEDGE	edge;

	PRINTF("\n\nAdjacency lists: ");
	PRINTF("\n%ld %s(%d)\n", node, gtitle(node), NTIEFE(node));
	if (f!=1) {
	PRINTF("Succs:");
	edge = NSUCC(node);
	while (edge) {
		PRINTF("%ld %c:",edge,EKIND(edge));
		PRINTF("(%ld)%s-", SOURCE(edge),gtitle(SOURCE(edge)));
		PRINTF("(%ld)%s ", TARGET(edge),gtitle(TARGET(edge)));
		edge = ANEXT(edge);
	}
	}
	if (f!=0) {
	PRINTF("\nPreds:");
	edge = NPRED(node);
	while (edge) {
		PRINTF("%ld %c:",edge,EKIND(edge));
		PRINTF("(%ld)%s-", SOURCE(edge),gtitle(SOURCE(edge))); 
		PRINTF("(%ld)%s ", TARGET(edge),gtitle(TARGET(edge)));
            	edge = ANEXT(edge); 
        }
	node = NNEXT(node);
	PRINTF("End\n");
	}
}
#endif



/* Output the layer *
 *==================*/

#ifdef DEBUG

#define mtitle(v)  (NTITLE(v)?NTITLE(v):"??")

#ifdef ANSI_C
static void    db_output_layer(void)
#else
static void    db_output_layer()
#endif
{
	int     i;
	GNLIST	li;
	GNODE   node;

	for (i=0; i<=maxdepth+1; i++) {
		PRINTF("Layer %d:\n",i);
		li = TSUCC(layer[i]);
		while (li) {
			node = GNNODE(li);
			PRINTF("\n%ld %s(%d)\n",node, mtitle(node),NTIEFE(node));
			li = GNNEXT(li);
		}
		PRINTF("-----------------\n");
	}
}
#endif


/* Output the layer in VCG format *
 *================================*/

#ifdef DEBUG

#ifdef ANSI_C
static void    db_output_vcglayer(char *fn)
#else
static void    db_output_vcglayer(fn)
char *fn;
#endif
{
	int     i, j;
	GNLIST	li;
	GNODE   node;
	ADJEDGE	edge;
	CONNECT c1;
	FILE *f;

	f = fopen(fn,"w");

	FPRINTF(f, "graph: { title: \"db_%s\" \n",fn);

	for (i=0; i<=maxdepth+1; i++) {
		li = TSUCC(layer[i]);
		j = 1;
		while (li) {
			node = GNNODE(li);
			FPRINTF(f, "\nnode: { title: \"%ld\" ",node );
			if ((NTITLE(node)) && (NTITLE(node)[0]))
				FPRINTF(f, "label: \"%s\" ", NTITLE(node) );
			FPRINTF(f, "level: %d ", i);
			FPRINTF(f, "horizontal_order: %d ", j);
			FPRINTF(f, "loc: { x: %d y: %d } ", 10+j*80, 10+i*60);
			FPRINTF(f, "}\n");
			edge = NSUCC(node);
			while (edge) {
				FPRINTF(f,"edge: { ");
				FPRINTF(f,"sourcename: \"%ld\" ",SOURCE(edge));
				FPRINTF(f,"targetname: \"%ld\" ",TARGET(edge));
				FPRINTF(f,"}\n");
				edge = ANEXT(edge);
			}
			c1 = NCONNECT(node);
			if (c1) {
				if ((CTARGET(c1)) && (CTARGET(c1)!=node)) { 
					FPRINTF(f,"edge: { ");
					FPRINTF(f,"sourcename: \"%ld\" ",node);
					FPRINTF(f,"targetname: \"%ld\" ",
							CTARGET(c1));
					FPRINTF(f, "linestyle: dashed }\n");
				}
				if ((CTARGET2(c1)) && (CTARGET2(c1)!=node)) { 
					FPRINTF(f,"edge: { ");
					FPRINTF(f,"sourcename: \"%ld\" ",node);
					FPRINTF(f,"targetname: \"%ld\" ",
							CTARGET2(c1));
					FPRINTF(f, "linestyle: dashed }\n");
				}
			}
			j++;
			li = GNNEXT(li);
		}
	}
	FPRINTF(f, "}\n");
	fclose(f);
}
#endif

/*--------------------------------------------------------------------*/


