/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         alloc.c                                            */
/*   version:      1.00.00                                            */
/*   creation:     14.4.1993                                          */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Memory Management                                  */
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


/****************************************************************************
 * This file is a collection of auxiliary functions that implement the
 * memory management. It provides the following functions:
 *
 * myalloc      allocates memory from the internal memory management.
 * free_memory      gives the complete memory free.
 *
 * nodealloc        allocates a GNODE object (node), adds it to nodelist
 * graphalloc       allocates a GNODE object (graph), adds it to graphlist
 * tmpnodealloc     allocates a temporary GNODE object
 * free_node        deallocate a nontemporary GNODE object
 *
 * nodelist_alloc   allocates a stable node list element (i.e. a cons
 *          cell whose head is a GNODE object)
 * tmpnodelist_alloc    allocates a temporary node list element (i.e. a cons
 *          cell whose head is a GNODE object)
 * free_regionnodelist  deallocate a list of node list elements.
 *          These were used as storage of regions.
 *
 * edgealloc        allocates a GEDGE object (edge), adds it to edgelist
 * tmpedgealloc     allocates a temporary GEDGE object
 *
 * near_edge_insert     insert a near edge into near_edge_list. A near edge
 *          is a special edge that must always be placed near an
 *          other edge. See the nearedge-specification-feature.
 *          We use a special adjacency list to notify all
 *          near edges.
 * bentnear_edge_insert insert a bent near edge into bent_near_edge_list.
 *          A bent near edge is a special edge consisting of a
 *          near edge, a dummy node and a normal edge.
 *          See the nearedge-specification-feature.
 *          We use a special adjacency list to notify all
 *          bent near edges.
 * back_edge_insert     insert a back edge into back_edge_list. A back edge
 *          is an edge that preferably is reverted.
 *          We use a special adjacency list to notify all
 *          back edges.
 *
 * connectalloc     allocates a CONNECT element of a node
 *
 * dllist_alloc     allocates a DLLIST-cons cell. These are double linked
 *          lists of nodes.
 * dllist_free      gives one DLLIST-cons cell free.
 *
 * free_all_lists   gives all temporary memory free.
 ***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "grammar.h"
#include "main.h"
#include "options.h"
#include "alloc.h"
#include "folding.h"
#include "graph.h"


/* Prototypes
 * ----------
 */

static GNODE internal_nodealloc();
static void free_nodelists();
static GEDGE internal_edgealloc();
static void free_tmpedges();
static void free_edgelists();
static void free_connect();
static void reinit_all_lists();

/*--------------------------------------------------------------------*/
/* Memory allocation                                                  */
/*--------------------------------------------------------------------*/

/*  Core Memory Management
 *  ======================
 */

static long node_refnum = 0L;   /* reference counter for REFNUM of nodes */


/**
 *   Allocate x bytes. We use the memory mechanism from the parser.
 */
char *myalloc(int x)
{
    return ParseMalloc(x);
} /* myalloc */

/**
 *   Deallocate the complete memory.
 */
void free_memory(void)
{
    FreeHash();
    ParseFree();
    node_refnum = 0L;
    reinit_all_lists();
    graph_init();
} /* free_memory */

/**
 * Выделить память с помощью malloc, проверив указатель на NULL.
 */
void *libc_malloc(int size)
{
    void *p = malloc(size);
    if (!p) {
        Fatal_error("memory exhausted","");
    }
    return p;
} /* libc_malloc */

/*--------------------------------------------------------------------*/

/*  Memory Management for Nodes
 *  ===========================
 */

/*  We distinguish between
 *     1) GNODE objects as nodes from the specification
 *     2) GNODE objects as representation of subgraphs
 *     3) temporary GNODE objects, used to calculate a layout.
 *        They can be deleted on each change of layout.
 */

/*  Nodes from the specification come into the nodelist.
 *  Subgraphs come into the subgraphlist.
 *  Temporary nodes come into the tmpnodelist.
 *  Free GNODE objects are collected into the node_freelist.
 */

GNODE nodelist     = NULL;     /* List of all real nodes as specified */
GNODE nodelistend  = NULL;     /* End of this list                    */
GNODE graphlist    = NULL;     /* List of all subgraphs as specified  */
GNODE graphlistend = NULL;     /* End of this list                    */

GNODE invis_nodes  = NULL;     /* List of all invisible nodes         */
GNODE labellist    = NULL;     /* List of dummy nodes that contain    */
                               /* the text of an edge label           */
GNODE labellistend = NULL;     /* End of this list                    */
GNODE dummylist    = NULL;     /* List of other dummy nodes           */

GNODE tmpnodelist   = NULL;     /* list of allocated temoprary nodes */
static GNODE node_freelist = NULL;     /* list of free GNODE objects */



/*  Allocate a GNODE object
 *  -----------------------
 *  First, we look in the free list, if we have a free node. Otherwise,
 *  we allocate a node from the core memory.
 *  We also set some default values.
 */

static GNODE internal_nodealloc(void)
{
    GNODE   h;
    int i;

    if (node_freelist) {
        h = node_freelist;
        node_freelist = NINTERN(node_freelist);
    }
    else {
        h = (GNODE) myalloc(sizeof(struct gnode));
    }

    NREFNUM(h)      = node_refnum++;
    NTITLE(h)       = NULL;
    NLABEL(h)       = NULL;
    NINFO1(h)       = NULL;
    NINFO2(h)       = NULL;
    NINFO3(h)       = NULL;
    NLEVEL(h)       = -1;
    NSHAPE(h)       = 0;
    NHORDER(h)      = -1;
    NSX(h)          = 0L;
    NSY(h)          = 0L;
    NX(h)           = 0L;
    NY(h)           = 0L;
    NSGRAPH(h)      = NULL;
    NROOT(h)        = NULL;
    NREGREPL(h)     = NULL;
    NREGION(h)      = NULL;
    NREGROOT(h)     = NULL;
    NINLIST(h)      = 1;
    NINVISIBLE(h)   = 0;
    NTIEFE(h)       = -1;
    NPOS(h)         = -1;
    NWEIGHTS(h)     = 0L;
    NWEIGHTP(h)     = 0L;
    NMARK(h)        = 0;
    NREVERT(h)      = 0;
    NANCHORNODE(h)  = 0;
    NBARY(h)        = -1;
    NDFS(h)         = -1L;
    NVPTR(h)        = NULL;
    for (i = 0; i < TEMP_ATTR_COUNT; i++) NTEMPATTR(h,i) = DEAD_PTR;
    for (i = 0; i < MARKER_COUNT; i++) NMARKERVAL(h,i) = 0;
    NCONNECT(h)     = NULL;
    NINTERN(h)      = NULL;
    init_node_graph_fields_as_dead(h);

    return(h);
}



/*  Allocate a real node
 *  --------------------
 *  and update the nodelist. Real nodes are such nodes that are specified
 *  as node: { .... }.
 *  We inheret the attributes from the refnode.
 */

GNODE nodealloc(GNODE refnode)
{
    GNODE h;

    h = internal_nodealloc();
    copy_nodeattributes(refnode, h);
    ins_node_in_dl_list(h,nodelist,nodelistend);
    init_node_adj_fields(h);
    return(h);
}

/*  Initialize a node with the node defaults
 *  ----------------------------------------
 */

void nodedefaults(GNODE node)
{
    NTITLE(node)    = G_title;
    NLABEL(node)    = NULL;
    NLEVEL(node)    = -1;
    NSHAPE(node)    = BOX;
    NHORDER(node)   = -1;
    NINFO1(node)    = "";
    NINFO2(node)    = "";
    NINFO3(node)    = "";
    NSX(node)       = 0L;
    NSY(node)       = 0L;
    NTEXTMODE(node) = CENTER;
    NSTATE(node)    = 0;
    NWIDTH(node)    = -1;
    NHEIGHT(node)   = -1;
    NBORDERW(node)  = 2;
    NFOLDING(node)  = -1;
    NCOLOR(node)    = G_color;
    NTCOLOR(node)   = BLACK;
    NBCOLOR(node)   = BLACK;
    NSHRINK(node)   = 1;
    NSTRETCH(node)  = 1;

    init_node_graph_fields_as_dead(node);
}


/*  Initialize a node with the foldnode defaults
 *  --------------------------------------------
 */
void foldnodedefaults(GNODE node)
{
    NTITLE(node)    = NULL;
    NLABEL(node)    = NULL;
    NLEVEL(node)    = -1;
    NSHAPE(node)    = -1;
    NHORDER(node)   = -1;
    NINFO1(node)    = NULL;
    NINFO2(node)    = NULL;
    NINFO3(node)    = NULL;
    NSX(node)       = 0L;
    NSY(node)       = 0L;
    NTEXTMODE(node) = -1;
    NSTATE(node)    = -1;
    NWIDTH(node)    = -1;
    NHEIGHT(node)   = -1;
    NBORDERW(node)  = 4;
    NFOLDING(node)  = -1;
    NCOLOR(node)    = -1;
    NTCOLOR(node)   = -1;
    NBCOLOR(node)   = -1;
    NSHRINK(node)   = -1;
    NSTRETCH(node)  = -1;

    init_node_graph_fields_as_dead(node);
}


/*  Copy the foldnode attributes from fn to y
 *  -----------------------------------------
 */

void inherit_foldnode_attributes(GNODE fn, GNODE y)
{
    /* NTITLE not needed */
    /* NFOLDING not needed */
    if (NLABEL(fn))         NLABEL(y)   = NLABEL(fn);
    if (NLEVEL(fn)!= -1)    NLEVEL(y)   = NLEVEL(fn);
    if (NSHAPE(fn)!= -1)    NSHAPE(y)   = NSHAPE(fn);
    if (NHORDER(fn)!= -1)   NHORDER(y)  = NHORDER(fn);
    if (NINFO1(fn))         NINFO1(y)   = NINFO1(fn);
    if (NINFO2(fn))         NINFO2(y)   = NINFO2(fn);
    if (NINFO3(fn))         NINFO3(y)   = NINFO3(fn);
    if (NSX(fn)!= -1L)      NSX(y)      = NSX(fn);
    if (NSY(fn)!= -1L)      NSY(y)      = NSY(fn);
    if (NTEXTMODE(fn)!= -1) NTEXTMODE(y)= NTEXTMODE(fn);
    if (NSTATE(fn)!= -1)    NSTATE(y)   = NSTATE(fn);
    if (NWIDTH(fn)!= -1)    NWIDTH(y)   = NWIDTH(fn);
    if (NHEIGHT(fn)!= -1)   NHEIGHT(y)  = NHEIGHT(fn);
    if (NBORDERW(fn)!= -1)  NBORDERW(y) = NBORDERW(fn);
    if (NCOLOR(fn)!= -1)    NCOLOR(y)   = NCOLOR(fn);
    if (NTCOLOR(fn)!= -1)   NTCOLOR(y)  = NTCOLOR(fn);
    if (NBCOLOR(fn)!= -1)   NBCOLOR(y)  = NBCOLOR(fn);
    if (NSHRINK(fn)!= -1)   NSHRINK(y)  = NSHRINK(fn);
    if (NSTRETCH(fn)!= -1)  NSTRETCH(y) = NSTRETCH(fn);
}


/*  Copy a GNODE object x into a second GNODE object y
 *  --------------------------------------------------
 */

void copy_nodeattributes(GNODE x, GNODE y)
{
    NTITLE(y)   = NTITLE(x);
    NLABEL(y)   = NLABEL(x);
    NLEVEL(y)   = NLEVEL(x);
    NSHAPE(y)   = NSHAPE(x);
    NHORDER(y)  = NHORDER(x);
    NINFO1(y)   = NINFO1(x);
    NINFO2(y)   = NINFO2(x);
    NINFO3(y)   = NINFO3(x);
    NSX(y)      = NSX(x);
    NSY(y)      = NSY(x);
    NTEXTMODE(y)= NTEXTMODE(x);
    NSTATE(y)   = NSTATE(x);
    NWIDTH(y)   = NWIDTH(x);
    NHEIGHT(y)  = NHEIGHT(x);
    NBORDERW(y) = NBORDERW(x);
    NFOLDING(y) = NFOLDING(x);
    NCOLOR(y)   = NCOLOR(x);
    NTCOLOR(y)  = NTCOLOR(x);
    NBCOLOR(y)  = NBCOLOR(x);
    NSHRINK(y)  = NSHRINK(x);
    NSTRETCH(y) = NSTRETCH(x);
}



/*  Allocate a graph object
 *  -----------------------
 *  and update the graphlist. Such objects are summary nodes whose graph
 *  is specified as graph: { .... }.
 *  We inheret the attributes from the refnode.
 */

GNODE graphalloc(GNODE refnode)
{
    GNODE   h;

    h = internal_nodealloc();
    copy_nodeattributes(refnode, h);
    NFOLDING(h)   = -1;
    NINLIST(h)    = 0;
    NINVISIBLE(h) = 1;
    NDFS(h)       = 0L;

    ins_node_in_dl_list(h,graphlist,graphlistend);
    init_node_adj_fields(h);
    return(h);
}


/*  Allocate a temporary GNODE object
 *  ---------------------------------
 *  and update the tmpnodelist. These are node for dummy's, label's etc.
 *  These nodes are only needed for the layouting.
 */

GNODE   tmpnodealloc(
    int textm,
    int width,
    int height,
    int borderw,
    int fold,
    int color,
    int textc,
    int borderc,
    int shrink,
    int stretch,
    int horder)
{
    GNODE   h;

    h = internal_nodealloc();

    NHORDER(h)      = horder;
    NTEXTMODE(h)    = textm;
    NSTATE(h)       = 0;
    NWIDTH(h)       = width;
    NHEIGHT(h)      = height;
    NBORDERW(h)     = borderw;
    NFOLDING(h)     = fold;
    NCOLOR(h)       = color;
    NTCOLOR(h)      = textc;
    NBCOLOR(h)      = borderc;
    NSHRINK(h)      = shrink;
    NSTRETCH(h)     = stretch;
    NINLIST(h)      = 0;
    NINVISIBLE(h)   = 1;
    NDFS(h)         = 0L;

    NINTERN(h)   = tmpnodelist;
    tmpnodelist  = h;
    init_node_adj_fields(h);
    return(h);
}


/*  Deallocate all temporary GNODE objects
 *  --------------------------------------
 */

void free_tmpnodes(void)
{
    GNODE   h;

    h = tmpnodelist;
    if (h) {
        while (NINTERN(h)) h = NINTERN(h);
        NINTERN(h) = node_freelist;
        node_freelist = tmpnodelist;
        tmpnodelist = NULL;
    }
    labellist    = NULL;
    labellistend = NULL;
    dummylist = NULL;
    /* Labels and dummys are temporary nodes thus they
     * are given free, too
     */
}


/*  Deallocate one GNODE objects
 *  ----------------------------
 *  This object should not be temporary !!!
 *  In fact, it is a region summary substitution node. See folding.c
 *  If the object would be allocated by tmpnodealloc,
 *  it could be given free twice, which is wrong.
 */

void free_node(GNODE h)
{
    check_node_no_adj_edges(h);
    NINTERN(h) = node_freelist;
    node_freelist = h;
}


/*  Give a position, search a node in the node list
 *  -----------------------------------------------
 *  This is used in the menues after selecting a node.
 *  At this time point, all visible nodes are in the node list.
 */

GNODE   search_xy_node(long x,long y)
{
    GNODE   v;
    int     width, height;
    long    xpos, ypos;

    for (v = nodelist; v; v = NNEXT(v))
    {
        xpos = (NX(v)*G_stretch)/G_shrink - V_xmin;
        ypos = (NY(v)*G_stretch)/G_shrink - V_ymin;
        width = (NWIDTH(v)*G_stretch)/G_shrink;
        height = (NHEIGHT(v)*G_stretch)/G_shrink;
        if ( (xpos <= x) && (x <= xpos+width) &&
             (ypos <= y) && (y <= ypos+height) )
                return(v);      /* node found */
    }
    return NULL;        /* no node found */
}


/*  Check the graph consistency
 *  ---------------------------
 *  A serious problem is that subgraphs may not have any nodes.
 *  This leads to confusion if such a subgraph is folded.
 *  Thus, for such subgraphs, we add auxiliary nodes.
 */
void check_graph_consistency(void)
{
    GNODE v,w;

    for (v = graphlist; v; v = NNEXT(v))
    {
        if (NSGRAPH(v)==NULL) {
            if (!silent) {
                FPRINTF(stderr,"\nWarning: Graph %s",
                        (NTITLE(v)?NTITLE(v):""));
                FPRINTF(stderr," has no nodes.");
                FPRINTF(stderr," I add a node !\n");
            }

            w = nodealloc( v );
            NTITLE(w) = "artificial node";
            NROOT(w) = v;
            NSGRAPH(v) = nodelist_alloc(w);
        }
    }
} /* check_graph_consistency */

/*--------------------------------------------------------------------*/


/*  Memory Management lists of GNODE objects
 *  ========================================
 */

/*  Lists of GNODE objects, if they are not connected via internal
 *  GNODE pointers, use special cons-cells, i.e. GNLIST objects, whose
 *  heads are GNODE objects. Because some cons-cells are temporary,
 *  we use a similar memory management as for temporary GNODE objects.
 */


static GNLIST tmpnconslist   = NULL;  /* list of allocated cons cells */
static GNLIST foldnconslist   = NULL; /* list of all. fold cons cells */
static GNLIST ncons_freelist = NULL;  /* list of free cons cells      */


/*  Allocate a GNLIST object
 *  ------------------------
 *  First, we look in the free list, if we have a free node. Otherwise,
 *  we allocate a node from the core memory.
 *  We also set some default values.
 *  These node lists are part of the stable graph representation, i.e.
 *  need not to be freed unless a reload of the graph. Thus we don't
 *  store them in the tmpnconslist.
 */
GNLIST nodelist_alloc(GNODE v)
{
    GNLIST  h;

    h = (GNLIST)myalloc(sizeof(struct gnlist));
    GNINTERN(h) = NULL;
    GNNODE(h)   = v;
    GNNEXT(h)   = NULL;
    return(h);
}

/*  Allocate a temporary GNLIST object
 *  ----------------------------------
 *  First, we look in the free list, if we have a free node. Otherwise,
 *  we allocate a node from the core memory.
 *  We also set some default values.
 *  These node lists are temporary, thus we store them in the
 *  tmpnconslist, to give them free later.
 */
GNLIST  tmpnodelist_alloc(void)
{
    GNLIST  h;

    if (ncons_freelist) {
        h = ncons_freelist;
        ncons_freelist = GNINTERN(ncons_freelist);
    }
    else
        h = (GNLIST)myalloc(sizeof(struct gnlist));
    GNINTERN(h) = tmpnconslist;
    GNNODE(h)   = NULL;
    GNNEXT(h)   = NULL;
    tmpnconslist = h;
    return(h);
}


/*  Allocate a foldlist GNLIST object
 *  ---------------------------------
 *  First, we look in the free list, if we have a free node. Otherwise,
 *  we allocate a node from the core memory.
 *  We also set some default values.
 *  These node lists are used for the folding action keepers in
 *  folding.c. They live longer than temporary nodes, but are
 *  also temporary, because they are deallocated after folding.
 */

GNLIST  foldnodelist_alloc(void)
{
    GNLIST  h;

    if (ncons_freelist) {
        h = ncons_freelist;
        ncons_freelist = GNINTERN(ncons_freelist);
    }
    else
        h = (GNLIST)myalloc(sizeof(struct gnlist));
    GNINTERN(h) = foldnconslist;
    GNNODE(h)   = NULL;
    GNNEXT(h)   = NULL;
    foldnconslist = h;
    return(h);
}



/*  Deallocate all temporary GNLIST objects
 *  --------------------------------------
 */
static void free_nodelists(void)
{
    GNLIST  h;

    /* все из tmpnconslist переносим в ncons_freelist */
    h = tmpnconslist;
    if (h) {
        while(GNINTERN(h)) h = GNINTERN(h);
        GNINTERN(h) = ncons_freelist;
        ncons_freelist = tmpnconslist;
        tmpnconslist = NULL;
    }
}


/*  Deallocate all fold GNLIST objects
 *  ----------------------------------
 */
void free_foldnodelists(void)
{
    GNLIST  h;

    /* все из foldnconslist переносим в ncons_freelist */
    h = foldnconslist;
    if (h) {
        while (GNINTERN(h)) h = GNINTERN(h);
        GNINTERN(h) = ncons_freelist;
        ncons_freelist = foldnconslist;
        foldnconslist = NULL;
    }
}


/*  Deallocate GNLIST objects of regions
 *  ------------------------------------
 *  These GNLIST objects should be allocated by nodelist_alloc,
 *  i.e. should not be temporary.
 */
void free_regionnodelist(GNLIST r)
{
    GNLIST  h;

    /* все из r переносим в ncons_freelist, r не нулим */
    h = r;
    if (h) {
        while(GNINTERN(h)) h = GNINTERN(h);
        GNINTERN(h) = ncons_freelist;
        ncons_freelist = r;
    }
}


/*--------------------------------------------------------------------*/


/*  Memory Management for Edges
 *  ===========================
 */

/*  We distinguish between
 *     1) GEDGE objects as edges from the specification
 *     2) GEDGE objects from the specification that are not visualized
 *        directly. Neverthelesss, we need the attributes of these
 *        edges, thus we create a auxiliary GEDGE object for them.
 *     3) temporary GEDGE objects, used to calculate a layout.
 *        They can be deleted on each change of layout.
 */

/*  Edge from the specification come into the edgelist.
 *  Temporary edges come into the tmpedgelist.
 *  Free GEDGE objects are collected into the node_freelist.
 */

GEDGE edgelist     = NULL;     /* List of all real edges as specified */
GEDGE edgelistend  = NULL;     /* End of this list                    */

GEDGE tmpedgelist   = NULL;     /* list of allocated temporary edges */
static GEDGE edge_freelist = NULL;     /* list of free GEDGE objects        */


/*  Allocate a GEDGE object
 *  -----------------------
 *  First, we look in the free list, if we have a free edge. Otherwise,
 *  we allocate an edge from the core memory.
 *  We also set some default values.
 */
static GEDGE internal_edgealloc(void)
{
    GEDGE   h;

    if (edge_freelist) {
        h = edge_freelist;
        edge_freelist = ENEXT(edge_freelist);
    }
    else {
        h = (GEDGE) myalloc(sizeof(struct gedge));
    }

    ESTART(h)       = NULL;
    EEND(h)         = NULL;
    ESTARTX(h)      = 0;
    ESTARTY(h)      = 0;
    ETBENDX(h)      = 0;
    ETBENDY(h)      = 0;
    EBBENDX(h)      = 0;
    EBBENDY(h)      = 0;
    EENDX(h)        = 0;
    EENDY(h)        = 0;
    EORI(h)         = NO_ORI;
    EORI2(h)        = NO_ORI;
    ELABEL(h)       = NULL;
    ELABELCOL(h)    = BLACK;
    EART(h)         = 'U';
    ELNODE(h)       = NULL;
    EANCHOR(h)      = 0;
    EINVISIBLE(h)   = 0;
    EWEIGHTS(h)     = 0;
    EWEIGHTP(h)     = 0;
    init_edge_graph_fields_as_dead(h);
    return(h);
}


/*  Allocate a real edge
 *  --------------------
 *  and update the edgelist. These edges are specified, e.g.
 *  as edge: { ... }
 *  We inheret the attributes from the refedge.
 */

GEDGE edgealloc(GEDGE refedge)
{
    GEDGE   h;

    h = internal_edgealloc();
    copy_edgeattributes(refedge, h);
    ins_edge_in_dl_list(h, edgelist, edgelistend);
    return(h);
}


/*  Initialize an edge with the edge defaults
 *  -----------------------------------------
 */

void edgedefaults(GEDGE edge)
{
    ELABEL(edge)        = NULL;
    ELSTYLE(edge)       = SOLID;
    ETHICKNESS(edge)    = 2;
    ECLASS(edge)        = 1;
    EPRIO(edge)         = 1;
    EHORDER(edge)       = -1;
    ECOLOR(edge)        = BLACK;
    ELABELCOL(edge)     = BLACK;
    EARROWSIZE(edge)    = 10;
    EARROWSTYLE(edge)   = AS_SOLID;
    EARROWCOL(edge)     = BLACK;
    EARROWBSIZE(edge)   = 0;
    EARROWBSTYLE(edge)  = AS_NONE;
    EARROWBCOL(edge)    = BLACK;

    init_edge_graph_fields_as_dead(edge);
}


/*  Initialize an edge with the foldedge defaults
 *  ---------------------------------------------
 */
void foldedgedefaults(GEDGE edge)
{
    ELABEL(edge)        = "...";
    ELSTYLE(edge)       = -1;
    ETHICKNESS(edge)    = 4;
    ECLASS(edge)        = -1;
    EPRIO(edge)         = -1;
    EHORDER(edge)       = -1;
    ECOLOR(edge)        = -1;
    ELABELCOL(edge)     = -1;
    EARROWSIZE(edge)    = -1;
    EARROWSTYLE(edge)   = -1;
    EARROWCOL(edge)     = -1;
    EARROWBSIZE(edge)   = -1;
    EARROWBSTYLE(edge)  = -1;
    EARROWBCOL(edge)    = -1;

    init_edge_graph_fields_as_dead(edge);
}


/*  Copy the foldedge attributes from fn to y
 *  -----------------------------------------
 */

void inherit_foldedge_attributes(GEDGE fn, GEDGE y)
{
    if (ELABEL(fn))            ELABEL(y)       = ELABEL(fn);
    if (ELSTYLE(fn)     != -1) ELSTYLE(y)      = ELSTYLE(fn);
    if (ETHICKNESS(fn)  != -1) ETHICKNESS(y)   = ETHICKNESS(fn);
    if (ECLASS(fn)      != -1) ECLASS(y)       = ECLASS(fn);
    if (EPRIO(fn)       != -1) EPRIO(y)        = EPRIO(fn);
    if (EHORDER(fn)     != -1) EHORDER(y)      = EHORDER(fn);
    if (ECOLOR(fn)      != -1) ECOLOR(y)       = ECOLOR(fn);
    if (ELABELCOL(fn)   != -1) ELABELCOL(y)    = ELABELCOL(fn);
    if (EARROWSIZE(fn)  != -1) EARROWSIZE(y)   = EARROWSIZE(fn);
    if (EARROWSTYLE(fn) != -1) EARROWSTYLE(y)  = EARROWSTYLE(fn);
    if (EARROWCOL(fn)   != -1) EARROWCOL(y)    = EARROWCOL(fn);
    if (EARROWBSIZE(fn) != -1) EARROWBSIZE(y)  = EARROWBSIZE(fn);
    if (EARROWBSTYLE(fn)!= -1) EARROWBSTYLE(y) = EARROWBSTYLE(fn);
    if (EARROWBCOL(fn)  != -1) EARROWBCOL(y)   = EARROWBCOL(fn);
}


/*  Copy a GEDGE object x into a second GEDGE object y
 *  --------------------------------------------------
 */

void copy_edgeattributes(GEDGE x, GEDGE y)
{
    ELABEL(y)       = ELABEL(x);
    ELSTYLE(y)      = ELSTYLE(x);
    ETHICKNESS(y)   = ETHICKNESS(x);
    ECLASS(y)       = ECLASS(x);
    EPRIO(y)        = EPRIO(x);
    EHORDER(y)      = EHORDER(x);
    ECOLOR(y)       = ECOLOR(x);
    ELABELCOL(y)    = ELABELCOL(x);
    EARROWSIZE(y)   = EARROWSIZE(x);
    EARROWSTYLE(y)  = EARROWSTYLE(x);
    EARROWCOL(y)    = EARROWCOL(x);
    EARROWBSIZE(y)  = EARROWBSIZE(x);
    EARROWBSTYLE(y) = EARROWBSTYLE(x);
    EARROWBCOL(y)   = EARROWBCOL(x);
}



/*  Allocate a temporary GEDGE object
 *  ---------------------------------
 *  and update the tmpedgelist. These are edges to dummy nodes or
 *  to labels.
 */

GEDGE   tmpedgealloc(
    int lstyle,
    int thick,
    int xclass,
    int prio,
    int ecolor,
    int elcol,
    int arrows,
    int barrows,
    int arrowsty,
    int barrowsty,
    int arrowc,
    int barrowc,
    int horder)
{
    GEDGE   h;

    h = internal_edgealloc();

    ELSTYLE(h)      = lstyle;
    ETHICKNESS(h)   = thick;
    ECLASS(h)       = xclass;
    EPRIO(h)        = prio;
    EHORDER(h)      = horder;
    ECOLOR(h)       = ecolor;
    ELABELCOL(h)    = elcol;
    EARROWSTYLE(h)  = AS_SOLID;
    EARROWCOL(h)    = ecolor;
    EARROWSIZE(h)   = arrows;
    EARROWSTYLE(h)  = arrowsty;
    EARROWCOL(h)    = arrowc;
    EARROWBSIZE(h)  = barrows;
    EARROWBSTYLE(h) = barrowsty;
    EARROWBCOL(h)   = barrowc;

    assert(ENEXT(h) == DEAD_GEDGE);
    ENEXT(h) = tmpedgelist;
    tmpedgelist = h;
    return(h);
}


/*  Deallocate all temporary GEDGE objects
 *  --------------------------------------
 */

static void free_tmpedges(void)
{
    GEDGE   h;

    h = tmpedgelist;
    if (h) {
        while (ENEXT(h)) h = ENEXT(h);
        ENEXT(h) = edge_freelist;
        edge_freelist = tmpedgelist;
        tmpedgelist = NULL;
    }
}


/*--------------------------------------------------------------------*/

/*  Memory Management lists of GEDGE objects
 *  ========================================
 */

/*  Lists of GEDGE objects are used in adjacency lists.
 *  We use special cons-cells, i.e. ADJEDGE objects, whose
 *  heads are GEDGE objects. Because these cons-cells are temporary,
 *  we use a similar memory management as for temporary GNODE objects.
 *
 *  Further, we have one nontemporary list of edges that contains the
 *  default connections as specified by `near_edge'. This is the
 *  near_edge_list.
 *  Dito, we have one nontemporary list of edges that contains the
 *  edges specified by `back_edge'. This is the back_edge_list.
 */


/* for stable default connections: */

ADJEDGE near_edge_list = NULL;      /* list of default connections */
ADJEDGE bent_near_edge_list = NULL; /* list of bent near edges     */
ADJEDGE back_edge_list = NULL;      /* list of back edges          */

/* for temporaries: */

static ADJEDGE tmpeconslist   = NULL;  /* list of allocated cons cells */
static ADJEDGE econs_freelist = NULL;  /* list of free cons cells      */


/*  Insert a near edge into near_edge_list
 *  --------------------------------------
 *  First, we look in the free list, if we have a free cell. Otherwise,
 *  we allocate a cell from the core memory.
 */

void near_edge_insert(GEDGE e)
{
    ADJEDGE h;

    if (econs_freelist) {
        h = econs_freelist;
        econs_freelist = AINTERN(econs_freelist);
    }
    else
        h = (ADJEDGE)myalloc(sizeof(struct adjedge));
    AKANTE(h) = e;
    ANEXT(h) = AINTERN(h) = near_edge_list;
    near_edge_list = h;
}


/*  Insert a bent near edge into bent_near_edge_list
 *  ------------------------------------------------
 *  First, we look in the free list, if we have a free cell. Otherwise,
 *  we allocate a cell from the core memory.
 */

void bentnear_edge_insert(GEDGE e)
{
    ADJEDGE h;

    if (econs_freelist) {
        h = econs_freelist;
        econs_freelist = AINTERN(econs_freelist);
    }
    else
        h = (ADJEDGE)myalloc(sizeof(struct adjedge));
    AKANTE(h) = e;
    ANEXT(h) = AINTERN(h) = bent_near_edge_list;
    bent_near_edge_list = h;
}



/*  Insert a back edge into back_edge_list
 *  --------------------------------------
 *  First, we look in the free list, if we have a free cell. Otherwise,
 *  we allocate a cell from the core memory.
 */

void back_edge_insert(GEDGE e)
{
    ADJEDGE h;

    if (econs_freelist) {
        h = econs_freelist;
        econs_freelist = AINTERN(econs_freelist);
    }
    else
        h = (ADJEDGE)myalloc(sizeof(struct adjedge));
    AKANTE(h) = e;
    ANEXT(h) = AINTERN(h) = back_edge_list;
    back_edge_list = h;
}



/*  Allocate a ADJEDGE object
 *  -------------------------
 *  First, we look in the free list, if we have a free cell. Otherwise,
 *  we allocate a cell from the core memory.
 */

ADJEDGE edgelist_alloc(void)
{
    ADJEDGE h;

    if (econs_freelist) {
        h = econs_freelist;
        econs_freelist = AINTERN(econs_freelist);
    }
    else
        h = (ADJEDGE)myalloc(sizeof(struct adjedge));
    AINTERN(h) = tmpeconslist;
    tmpeconslist = h;
    return(h);
}


/*  Deallocate all temporary ADJEDGE objects
 *  ----------------------------------------
 */

static void free_edgelists(void)
{
    ADJEDGE h;

    h = tmpeconslist;
    if (h) {
        while(AINTERN(h)) h = AINTERN(h);
        AINTERN(h) = econs_freelist;
        econs_freelist = tmpeconslist;
        tmpeconslist = NULL;
    }
}



/*--------------------------------------------------------------------*/

/*  Memory Management for CONNECT objects
 *  =====================================
 */

/*  CONNECT objects are annotations of GNODE objects.
 *  They indicate that two nodes must be directly neigboured during
 *  the layout. This occurs if nodes are at the same level connected.
 *  E.g.
 *          /    |    \      this situation is layouted as if we had only
 *         A<----B---->C     one node B. The connections of B are A and C.
 */

static CONNECT  connectlist      = NULL;   /* list of alloc. connect cells */
static CONNECT  connect_freelist = NULL;   /* list of free   connect cells */


/*  Allocate a CONNECT object
 *  -------------------------
 *  First, we look in the free list, if we have a free cell. Otherwise,
 *  we allocate a cell from the core memory.
 *  The new connect node is inserted into the connection field of the
 *  GNODE node.
 */

CONNECT connectalloc(GNODE node)
{
    CONNECT h;

    if (connect_freelist) {
        h = connect_freelist;
        connect_freelist = CINTERN(connect_freelist);
    }
    else
        h = (CONNECT)myalloc(sizeof(struct connect));
    CTARGET(h)  = NULL;
    CEDGE(h)    = NULL;
    CTARGET2(h) = NULL;
    CEDGE2(h)   = NULL;
    CSVPRED(h)  = DEAD_GELIST;
    CSVSUCC(h)  = DEAD_GELIST;
    CINTERN(h)  = connectlist;
    connectlist = h;
    NCONNECT(node)  = h;
    return(h);
}


/*  Deallocate all temporary CONNECT objects
 *  ----------------------------------------
 */

static void free_connect(void)
{
    CONNECT h;

    h = connectlist;
    if (h) {
        while(CINTERN(h)) h = CINTERN(h);
        CINTERN(h) = connect_freelist;
        connect_freelist = connectlist;
        connectlist = NULL;
    }
}


/*--------------------------------------------------------------------*/

/*  Memory Management for DLLIST objects
 *  ====================================
 */

/*  To have a good layout, we calculate the number of crossings of edges
 *  and try to minimize them. For the calculation of crossings, we need
 *  double linked lists of nodes (see step2.c) representing the upper
 *  list of touched nodes and the lower list of touched nodes. Because
 *  nodes may have multible occurences in these lists, we use the special
 *  data structure DLLIST.
 *  We reuse the DSUCC field to keep the list of free dllist nodes.
 *  But THIS dllist_freelist is NOT double linked.
 */

static DLLIST   dllist_freelist  = NULL;     /* list of free dllist nodes */


/*  Allocate a DLLIST object
 *  ------------------------
 *  First, we look in the free list, if we have a free cell. Otherwise,
 *  we allocate a cell from the core memory.
 *  The successor is always NULL, because we append at the end of the
 *  list. pred is the predecessor.
 */

DLLIST  dllist_alloc(GNODE node, DLLIST pred)
{
    DLLIST  h;

    if (dllist_freelist) {
        h = dllist_freelist;
        dllist_freelist = DSUCC(dllist_freelist);
    }
    else
        h = (DLLIST)myalloc(sizeof(struct dllist));
    DNODE(h) = node;
    DPRED(h) = pred;
    DSUCC(h) = NULL;
    return(h);
}


/*  Deallocate one DLLIST objects
 *  -----------------------------
 *  The crossing algorithm is stable enough such that after calculation
 *  of crossings all DLLIST elements are given free by this function.
 *  Thus we don't need any additional memory management.
 */

void    dllist_free(DLLIST x)
{
    DSUCC(x) = dllist_freelist;
    dllist_freelist = x;
}


/*  Deallocate a list of DLLIST objects
 *  -----------------------------------
 */

void    dllist_free_all(DLLIST x)
{
    DLLIST h;

    if (x) {
        h = x;
        while (DSUCC(h)) h = DSUCC(h);
        DSUCC(h) = dllist_freelist;
        dllist_freelist = x;
    }
}



/*--------------------------------------------------------------------*/


/*  Deallocation of all temporary lists
 *  ===================================
 */
void free_all_lists(void)
{
    free_tmpnodes();
    free_tmpedges();
    free_nodelists();
    free_edgelists();
    free_connect();
} /* free_all_lists */


/*  Reinitialization of all struct keeping lists
 *  --------------------------------------------
 */
static void reinit_all_lists(void)
{
    ufoldstart   = NULL;
    foldstart    = NULL;
    foldstops    = NULL;
    f_subgraphs  = NULL;
    uf_subgraphs = NULL;
    invis_nodes  = NULL;
    labellist    = NULL;
    labellistend = NULL;
    dummylist    = NULL;

    nodelist         = NULL;
    nodelistend      = NULL;
    graphlist        = NULL;
    graphlistend     = NULL;
    tmpnodelist      = NULL;
    node_freelist    = NULL;

    tmpnconslist     = NULL;
    ncons_freelist   = NULL;

    edgelist         = NULL;
    edgelistend      = NULL;
    tmpedgelist      = NULL;
    edge_freelist    = NULL;

    near_edge_list      = NULL;
    back_edge_list      = NULL;
    bent_near_edge_list = NULL;
    tmpeconslist        = NULL;
    econs_freelist      = NULL;

    connectlist      = NULL;
    connect_freelist = NULL;

    dllist_freelist  = NULL;
} /* reinit_all_lists */


