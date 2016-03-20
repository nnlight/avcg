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
 * All exisiting nodes and edges come directly from the specification.
 * graphlist contains the list of all invisible subgraph summary nodes.
 * nodelist  contains the list of all visible nodes.
 * edgelist  contains the list of all edges, visible or not.
 * temporary nodes and edges are given free,
 * i.e. tmpnodelist = tmpedgelist = NULL.
 * invis_nodes contains all nodes that are invisible because of a previous
 * edge class hiding.
 * Graphs and regions may be folded, but it is irrelevant which edges are
 * hidden, because the adjacency lists still not exist (e.g. they are removed
 * just before folding).
 *
 * Note again (see also alloc.h):
 *   Folded graphs have a summary node in nodelist and all nodes of this
 *   graphs outside nodelist.
 *   Unfolded graphs have the nodes in nodelist and the summary node in
 *   graphlist.
 *  NSGRAPH(r) is the list of nodes of subgraph r
 *  NROOT(n) = r if n is node of subgraph r
 *   Folded regions have a summary node in nodelist, and all nodes of
 *   this region outside nodelist.
 *  NREGREPL(r) is the start node of the region which has the summary
 *              node r.
 *  NREGION(r)  is the list of nodes of region r (except the start node)
 *  NREGROOT(n) = r if n is invisible and node of region r
 *
 * The task of the driver function `folding' is now to create the correct
 * adjacency lists by respecting new graph/region foldings or unfoldings
 * and hidden edge classes.
 *
 * The `folding keepers' are storages to remark which nodes must be folded
 * or unfolded. The function folding folds with respect to the folding
 * keepers. After that, it reinitializes the folding keepers.
 * The folding keepers DO NOT indicate which subgraphs or regions ARE
 * ACTUALLY FOLDED ! They indicate which actions must be done next.
 * The management of the array hide_class is different. It is initialized
 * once before reading the specification, and changed incrementally by
 * actions. At every time point, it indicates which edge classes ARE hidden.
 *
 * After folding, the following invariants hold:
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
 *
 * This file provides the following functions:
 * ------------------------------------------
 * add_sgfoldstart  add a start point to the subgraph folding keepers
 * add_foldstart    add a start point to the region folding keepers
 * add_foldstop     add a stop point to the region folding keepers
 * folding          do all foldings and create the adjacency lists
 * create_adjedge   insert an edge into the adjacency lists of its source
 *                  and target node.
 * delete_adjedge   delete an adjacency edge from source and target,
 *                  i.e. make the edge invisible.
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "grammar.h"
#include "alloc.h"
#include "main.h"
#include "options.h"
#include "folding.h"
#include "steps.h"
#include "graph.h"


/* Prototypes
 * ----------
 */

static void     revert_subgraph  _PP((GNODE v));
static int  foldstop_reached _PP((GNODE v));

static void delete_node _PP((GNODE v,int k));
static void insert_node _PP((GNODE v,int k));

static void delete_sgnodes  _PP((GNODE u));
static void fold_sg     _PP((GNODE u));
static void unfold_sg   _PP((GNODE u));

static void refresh_all_nodes   _PP((GNODE v));
static void refresh         _PP((void));
static void     sort_all_nodes      _PP((void));
static int      compare_ndfs        _PP((const GNODE *a,const GNODE *b));

static long no_dfs          _PP((GNODE n));
static long no_indeg        _PP((GNODE n));
static long no_outdeg       _PP((GNODE n));
static long no_degree       _PP((GNODE n));

static void create_adjacencies  _PP((void));
static void add_labels_at_folding();
static void adapt_labelpos      _PP((GNODE v,GEDGE e));
static GNODE    search_visible      _PP((GNODE v));
static GEDGE    substed_edge        _PP((GEDGE e));

static void unfold_region   _PP((GNODE n));
static void fold_region _PP((GNODE n, int k));
static void recursive_fold  _PP((GNODE v, GNODE n, int k));

static void hide_edge_classes _PP((void));

static void summarize_edges    _PP((void));
static void     split_double_edges _PP((void));

#ifdef DEBUG
static void db_print_somenode_list(GNODE w,GNODE wend);
#endif


/*--------------------------------------------------------------------*/
/*  Management of folding keepers                                     */
/*--------------------------------------------------------------------*/

/* Global variables
 * ----------------
 */

static GNLIST f_subgraphs; /* List of subgraph nodes to folding          */
static GNLIST uf_subgraphs;/* Node where subgraph unfolding starts           */
static GNLIST foldstops;   /* List of nodes where a fold region operation stops  */
static GNLIST foldstart;   /* List of nodes where a fold region operation starts */
static GNLIST ufoldstart;  /* Node where region unfolding starts         */


static void foldnodelist_add(GNLIST *lp, GNODE v)
{
    *lp = cons_node(v, *lp);
}

static void foldnodelist_remove(GNLIST *lp, GNODE v)
{
    GNLIST l = *lp;

    while (l) {
        if (GNNODE(l)==v) {
            *lp = GNNEXT(l);

            GNNEXT(l) = NULL;
            free_gnlist_list(l);
            break;
        }
        lp = &GNNEXT(l);
        l  = GNNEXT(l);
    }
}

void init_folding_keepers_globals()
{
    f_subgraphs = NULL;
    uf_subgraphs= NULL;
    foldstops   = NULL;
    foldstart   = NULL;
    ufoldstart  = NULL;
}

/*   Fold starters and stoppers
 *   ==========================
 *   Region fold starter are halfreverted, region fold stopper or
 *   subgraph stopper etc. are reverted.
 */

/* Clear all fold starter or stopper lists.
 * ---------------------------------------
 */
void clear_folding_keepers(void)
{
    GNLIST l;

    /* Set folding keeper to NOREVERT */
    for (l = f_subgraphs; l; l = GNNEXT(l))
    {
        NREVERT(GNNODE(l)) = NOREVERT;
        revert_subgraph(GNNODE(l));
    }

    for (l = uf_subgraphs; l; l = GNNEXT(l)) { NREVERT(GNNODE(l)) = NOREVERT; }
    for (l = foldstops; l; l = GNNEXT(l)) { NREVERT(GNNODE(l)) = NOREVERT; }
    for (l = foldstart; l; l = GNNEXT(l)) { NREVERT(GNNODE(l)) = NOREVERT; }
    for (l = ufoldstart; l; l = GNNEXT(l)) { NREVERT(GNNODE(l)) = NOREVERT; }

    free_gnlist_list(f_subgraphs);
    free_gnlist_list(uf_subgraphs);
    free_gnlist_list(foldstops);
    free_gnlist_list(foldstart);
    free_gnlist_list(ufoldstart);

    init_folding_keepers_globals();
}


/*   Add fold subgraph starter
 *   -------------------------
 *   Add a new fold subgraph starter v to the list foldstart.
 *   Note: v is the root node of the graph.
 */
void add_sgfoldstart(GNODE v)
{
    if (!v) return;
    if (NREVERT(v)==AREVERT) { /* delete it from sgfoldstart */
        NREVERT(v) = NOREVERT;
        foldnodelist_remove(&f_subgraphs, v);
        revert_subgraph(v);
        return;
    }
    NREVERT(v) = AREVERT;
    foldnodelist_add(&f_subgraphs, v);
    revert_subgraph(v);
}

/* Revert subgraph
 * ---------------
 * If a subgraph is selected for folding, all its nodes
 * are reverted, to easy the menu selection.
 * The nodes get the same revert-flag as the root node.
 */
static void revert_subgraph(GNODE v)
{
    GNODE w;
    GNLIST l;
    int rev;

    debugmessage("revert_subgraph",(NTITLE(v)?NTITLE(v):"(null)"));
    rev = NREVERT(v);

    for (l = NSGRAPH(v); l; l = GNNEXT(l))
    {
        w = GNNODE(l);
        NREVERT(w) = rev;
        if (NSGRAPH(w)) {
            revert_subgraph(w);
        }
    }
}


/*   Add unfold subgraph starter
 *   ---------------------------
 *   Add a new unfold subgraph starter v to the list foldstart.
 */
void add_sgunfoldstart(GNODE v)
{
    if (!v) return;
    if (NREVERT(v)==AREVERT) { /* delete it from uf_subgraphs */
        NREVERT(v) = NOREVERT;
        foldnodelist_remove(&uf_subgraphs, v);
        return;
    }
    NREVERT(v) = AREVERT;
    foldnodelist_add(&uf_subgraphs, v);
}


/*   Add fold region starter
 *   -----------------------
 *   Add a new fold region starter v to the list foldstarters.
 */
void add_foldstart(GNODE v)
{
    if (!v) return;

    if (NREVERT(v)==BREVERT) { /* delete it from foldstart */
        NREVERT(v) = NOREVERT;
        foldnodelist_remove(&foldstart, v);
        return;
    }
    if (NREVERT(v)==AREVERT) return;
    NREVERT(v) = BREVERT;
    foldnodelist_add(&foldstart, v);
}

/*   Add unfold region starter
 *   -------------------------
 *   Add a new unfold region starter v to the list foldstarters.
 */
void add_unfoldstart(GNODE v)
{
    if (!v) return;
    if (NREVERT(v)==AREVERT) { /* delete it from ufoldstart */
        NREVERT(v) = NOREVERT;
        foldnodelist_remove(&ufoldstart, v);
        return;
    }
    NREVERT(v) = AREVERT;
    foldnodelist_add(&ufoldstart, v);
}


/*   Add fold region stopper
 *   -----------------------
 *   Add a new fold region stopper v to the list foldstops.
 */
void add_foldstop(GNODE v)
{
    if (!v) return;
    if (NREVERT(v)==AREVERT) { /* delete it from foldstops */
        NREVERT(v) = NOREVERT;
        foldnodelist_remove(&foldstops, v);
        return;
    }
    NREVERT(v) = AREVERT;
    foldnodelist_add(&foldstops, v);
}


/*   Check fold stopper
 *   ------------------
 *   Returns 1 if v is in the list foldstops, i.e. if v is a fold stopper.
 */
static int foldstop_reached(GNODE v)
{
    GNLIST  l;

    if (!v) return(1);
    for (l = foldstops; l; l = GNNEXT(l))
    {
        if (GNNODE(l) == v) return(1);
    }
    return(0);
}


/*--------------------------------------------------------------------*/
/*  Folding and creation of adjacency lists                           */
/*--------------------------------------------------------------------*/

/*  Defines
 *  -------
 *  To have the possibility to revert foldings, or to find
 *  appropritate substitution node, we must notate why a node
 *  is invisible. Thus we set the INVISIBLE flag:
 *  0   ->   the node is visible
 *  1   ->   the node is a unfolded subgraph
 *  2   ->   the node is part of a folded subgraph
 *  3   ->   the node is part of a folded region
 *  4   ->   the node is invisible because it is only reacheable
 *       by hidden edge classes
 */

#define UNFOLDED_SGRAPH 1
#define FOLDED_SGNODE   2
#define FOLDED_RGNODE   3
#define HIDDEN_CNODE    4


/*   Folding main driver
 *   ===================
 *   We try to do as much as possible BEFORE the adjacency lists are
 *   created, because given all visible nodes, it is relatively simple
 *   to create the adjacency lists of those edges that are visible.
 *   We do subgraph folding, subgraph unfolding and region unfolding
 *   before the adjacency lists are created.
 *   However, to fold subgraphs or to hide edges, we need the adjacency
 *   lists, thus this is done later.
 *   Note that there is no operation `unhide edge class' because
 *   adjacency lists are always created new which automatically respects
 *   only those edges that are unhidden.
 *   At last, we check whether all visible nodes have (x,y) locations,
 *   and set the locFlag according.
 *
 *   The fold stoppers are set by the specification
 *   or by menu interaction.
 */

void    folding(void)
{
    GNODE   v,vn;
    GNLIST  l;
    int rclass;

    debugmessage("folding","");

    assert((labellist == NULL));
    assert((dummylist == NULL));
    assert((tmpnodelist == NULL));
    assert((tmpedgelist == NULL));

    if (G_displayel==NO) edge_label_phase=0;
    if (G_dirtyel==YES)  edge_label_phase=0;

    /* 1) make all node that are invisible because of edge classes
     *    visible
     */

    gs_wait_message('f');
    for (v = invis_nodes; v; v = vn)
    {
        vn = NNEXT(v);
        NNEXT(v) = DEAD_GNODE;
        insert_node(v, HIDDEN_CNODE);
    }
    invis_nodes = NULL;


    /* 2) subgraph folding */

    for (l = f_subgraphs; l; l = GNNEXT(l))
    {
        fold_sg(GNNODE(l));
    }

    /* 3) subgraph unfolding */

    for (l = uf_subgraphs; l; l = GNNEXT(l))
    {
        unfold_sg(GNNODE(l));
    }

    /* 4) Unfold region (independent of the class) */

    for (l = ufoldstart; l; l = GNNEXT(l))
    {
        unfold_region(GNNODE(l));
    }

    /* 5) Refresh the situation: initialize visibility flag of edges etc.
     *    Clear adjacency lists.
     */

    gs_wait_message('f');
    refresh();

    /* 6) Construct adjacency lists first time */

    create_adjacencies();
    hide_edge_classes();

    /* 7) Fold region: fold first the lower then the higher classes */


    for (rclass=0; rclass<17; rclass++) {
        for (l = foldstart; l; l = GNNEXT(l))
        {
            if (GNNODE(l) && (NFOLDING(GNNODE(l))==rclass))
                fold_region(GNNODE(l), rclass);
        }
    }


    /* 8) Hide edge classes */

    free_tmpedges();
    refresh();
    create_adjacencies();
    hide_edge_classes();

    /* 9) If labels necessary, create labels */

    if ((G_displayel==YES) && (G_dirtyel==NO) && (edge_label_phase==0))
        add_labels_at_folding();


    /* For stable layout: sort nodelist */
    sort_all_nodes();

    /* 10) check whether all nodes have already locations
     *     and transfer the specified locations.
     */

    locFlag = 1;
    for (v = nodelist; v; v = NNEXT(v))
    {
        if ((NSX(v)==0L) && (NSY(v)==0L)) {
            locFlag = 0;
            break;
        }
        NX(v) = NSX(v);
        NY(v) = NSY(v);
    }

    /* 11) If labels, we calculate edge positions without labels.
     */

    if (locFlag && (G_displayel==YES)) {
        free_tmpnodes();
        free_tmpedges();
        refresh();
        create_adjacencies();
        hide_edge_classes();
        for (v = nodelist; v; v = NNEXT(v))
        {
            NX(v) = NSX(v);
            NY(v) = NSY(v);
        }
    }

    /* 12) If necessary, remove double edges from the adjacency
     *     lists.
     */

    gs_wait_message('f');
    if (summarize_double_edges) summarize_edges();


    /* 13) Otherwise split edges which are doubled, such that they
     *     can be recognized better.
     */

    if ((!locFlag) && (!summarize_double_edges))
        split_double_edges();

    /* 14) Reinit the folding keepers to get a clean situation
     *     for the next folding.
     */

    clear_folding_keepers();
} /* folding */


/*--------------------------------------------------------------------*/
/*  Folding primitives                                                */
/*--------------------------------------------------------------------*/

/*   Insertion and deletion at the node list
 *   =======================================
 *   As long as the adjacency lists are not created, folding and is
 *   simply done by insertion and deletion of nodes at the nodelist.
 *   With this method, `fold subgraph' and `unfold subgraph' and
 *   `unfold region' can be implemented.
 *   To have the possibility to revert such foldings, or to find
 *   appropritate substitution node (see substed_edge), we must notate
 *   why a node is invisible. Thus we set the INVISIBLE flag:
 *  0   ->   the node is visible
 *  1   ->   the node is a unfolded subgraph
 *  2   ->   the node is part of a folded subgraph
 *  3   ->   the node is part of a folded region
 *  4   ->   the node is invisible because it is only reacheable
 *       by hidden edge classes
 *   See above the defines of
 *  UNFOLDED_SGRAPH, FOLDED_SGNODE, FOLDED_RGNODE, HIDDEN_CNODE
 */


/*   Delete a node v from the nodelist
 *   ---------------------------------
 *   The node becomes invisible.
 *   The invisibly flag is set to k, which is the reason why the node
 *   is invisible.
 */
static void delete_node(GNODE v, int k)
{
    assert((v));
    assert((k!=0));
    debugmessage("delete_node",(NTITLE(v)?NTITLE(v):"(null)"));
    if (!NINLIST(v)) return;
    NINLIST(v) = 0;
    NINVISIBLE(v) = k;
    del_node_from_dl_list(v,nodelist,nodelistend);
    if (NSGRAPH(v)) { /* a subgraph comes back into the graph list */
        if (k==UNFOLDED_SGRAPH) {
            ins_node_in_dl_list(v,graphlist,graphlistend);
        }
    }

}


/*   Inserts a node v into the nodelist
 *   ----------------------------------
 *   The node becomes visible.
 *   The reason of becoming visible is, that invisibility reason k
 *   is not anymore valid. If k was not the invisibility reason,
 *   then the insertion cannot be done.
 */
static void insert_node(GNODE v, int k)
{
    assert((v));
    assert((k!=0));
    debugmessage("insert_node",(NTITLE(v)?NTITLE(v):"(null)"));
    if (NINLIST(v)) return;
    if (NINVISIBLE(v)!=k) return;
    NINLIST(v) = 1;
    NINVISIBLE(v) = 0;
    if (NSGRAPH(v)) { /* a subgraph must be removed from the graph list */
        if (k==UNFOLDED_SGRAPH) {
            del_node_from_dl_list(v,graphlist,graphlistend);
        }
    }
    ins_node_in_dl_list(v,nodelist,nodelistend);
}


/*--------------------------------------------------------------------*/
/*  Folding of subgraphs                                              */
/*--------------------------------------------------------------------*/

/*  Delete all nodes of a subgraph from the nodelist
 *  ------------------------------------------------
 *  i.e. it hides the subgraph u (and all its subgraphs too).
 */
static void delete_sgnodes(GNODE u)
{
    GNODE   v;
    GNLIST  l;

    debugmessage("delete_sgnodes",(NTITLE(u)?NTITLE(u):"(null)"));
    for (l = NSGRAPH(u); l; l = GNNEXT(l))
    {
        v = GNNODE(l);
        delete_node(v, FOLDED_SGNODE);
        if (NSGRAPH(v))
            delete_sgnodes(v);
    }
}


/*   Fold a subgraph
 *   ---------------
 *   u is the root node of the subgraph
 *   Folding means: delete all nodes of this subgraph from the nodelist
 *   and insert the summary node.
 */
static void fold_sg(GNODE u)
{
    if (!u) return;
    debugmessage("fold_sg",(NTITLE(u)?NTITLE(u):"(null)"));

    delete_sgnodes(u);
    insert_node(u, UNFOLDED_SGRAPH);
}


/*   Unfold a subgraph
 *   -----------------
 *   u is the root node of the subgraph
 *   Unfolding means: delete this subgraph summary node from the nodelist
 *   and insert all its nodes.
 */
static void unfold_sg(GNODE u)
{
    GNODE   v;
    GNLIST  l;

    if (!u) return;
    debugmessage("unfold_sg",(NTITLE(u)?NTITLE(u):"(null)"));

    for (l = NSGRAPH(u); l; l = GNNEXT(l))
    {
        v = GNNODE(l);
        insert_node(v, FOLDED_SGNODE);

        /* If v was an unfolded subgraph before the folding,
         * it was before not visible, thus it is not inserted,
         * since the insert reason FOLDED_SGNODE is not the
         * delete reason UNFOLDED_SGRAPH. Then we have to unfold its
         * subnodes, too.
         */
        if (NSGRAPH(v) && (NINVISIBLE(v)==UNFOLDED_SGRAPH))
            unfold_sg(v);
    }
    delete_node(u, UNFOLDED_SGRAPH);
}


/*--------------------------------------------------------------------*/
/*  Folding of regions                                                */
/*--------------------------------------------------------------------*/

/*   Note that region operations are not inverse implemented, even
 *   if the effect is inverse.
 *   The reason is, that unfolding is done BEFORE the adjacency lists
 *   exist, but folding can only be done AFTER the adjacency lists exist.
 *   Further we use a dirty trick here: instead of creating a summary
 *   node of the region, we use the start node of the region as summary
 *   node. This has the advantage that the summary node is in the correct
 *   subgraph, and that we have no problems with the hashtable to
 *   search node title. An auxiliary node is created as REGREPL, to contain
 *   the information of the originally start node. This must be done
 *   because the originally start node could be a summary node of a
 *   previous region folding.
 */


/*   Fold Region
 *   -----------
 *   Starting from a node n, all nodes and edges reacheable by
 *   edges of class <= k are folded.
 *   Before, the adjacency lists were created.
 */
static void fold_region(GNODE n, int k)
{
    GEDGE   e, nxt_e, ee;
    GNODE   h;
    GNLIST  li;
    GNODE   v;


    assert((n));
    debugmessage("fold_region",(NTITLE(n)?NTITLE(n):"(null)"));

    /* First, we create a stable replacement node, that stores
     * all information of the node. Then we use the start node
     * as summary node, because the start node is part of the
     * right graph.
     */

    h = nodealloc(n);
    NTITLE(h)   = NTITLE(n);
    NROOT(h)    = NROOT(n);
    NREGREPL(h) = NREGREPL(n);
    NREGION(h)  = NREGION(n);
    NREGROOT(h) = NREGROOT(n);

    NSX(n) = NSY(n) = 0L;   /* Locations are not inherited ! */

    NWIDTH(n) = NHEIGHT(n) = -1; /* Sizes are not inherited */

    delete_node(h, FOLDED_RGNODE);        /* h is invisible */

    /* Now: h has the purpose of n, and n is the summary node
     * Now we change the properties of the summary nodes according
     * to the defaults.
     */

    inherit_foldnode_attributes(&foldnode, n);

    assert(NREGION(n) == NULL);
    NREGREPL(n) = h;
    NREGION(n)  = NULL;   /* here we collect the nodes of this region */

    /* Fold the nodes recursively */
    for (e = FirstSucc(n); e; e = nxt_e)
    {
        nxt_e = NextSucc(e);
        if ( ECLASS(e) <= k ) {
            if ( !foldstop_reached(EEND(e)) ) {
                delete_adjedge(e);
                recursive_fold(EEND(e),n,k);
            }
        }
    }

    /* Calculates substitutions for non-folded edges */
    for (li = NREGION(n); li; li = GNNEXT(li))
    {
        v = GNNODE(li);

        /* Substitute the predecessor edges of v */
        for (e = FirstPred(v); e; e = nxt_e)
        {
            nxt_e = NextPred(e);
            ee = substed_edge(e);
            if (ee!=e) {
                /* Edge e should be invisible or substituted:
                 * Remove edge e from adjacency lists.
                 */
                delete_adjedge(e);
                /* and insert new edge, but avoid self loops at
                 * the region node.
                 */
                if (ee) {
                    assert(EEND(ee)==n);
                    if (ESTART(ee)!=n);
                        create_adjedge(ee);
                }
            }
        }

        /* Substitute the successor edges of v */
        for (e = FirstSucc(v); e; e = nxt_e)
        {
            nxt_e = NextSucc(e);
            ee = substed_edge(e);
            if (ee!=e) {
                /* Edge e should be invisible or substituted:
                 * Remove edge e from adjacency lists.
                 */
                delete_adjedge(e);
                /* and insert new edge, but avoid self loops at
                 * the region node.
                 */
                if (ee) {
                    assert(ESTART(ee)==n);
                    if (EEND(ee)!=n);
                        create_adjedge(ee);
                }
            }
        }

        /* nnlight - my watching checks */
        assert(FirstPred(v) == NULL);
        assert(FirstSucc(v) == NULL);

        /*NPRED(v) = NSUCC(v) = NULL;*/  /* because v is invisible */
        unlink_node_edges(v);
    }
} /* fold_region */

/*  Recursive fold region
 *  ---------------------
 *  This is an auxiliary function of fold_region.
 *  It folds the region of class <=k starting at v. The summary node
 *  of the region is n. It makes the nodes of the region invisible,
 *  and fold (unlink) internal edges.
 */
static void recursive_fold(GNODE v, GNODE n, int k)
{
    GEDGE   e, ee, nxt_e;

    assert((v));
    assert((n));
    debugmessage("recursive_fold",(NTITLE(v)?NTITLE(v):"(null)"));

    /* Check of cycle: both checks means the same */
    if ( !NINLIST(v) ) return;
    if ( NREGROOT(v) == n )  return;

    /* Check of cycle: this is the node we started folding */
    if ( v == n )            return;

    /* Add v to the region list of n */
    NREGROOT(v) = n;
    NREGION(n) = cons_node(v, NREGION(n));

    delete_node(v,FOLDED_RGNODE);

    /* Go into the recursion */
    for (e = FirstSucc(v); e; e = nxt_e)
    {
        nxt_e = NextSucc(e);
        if ( ECLASS(e) <= k ) {
            if ( !foldstop_reached(EEND(e)) ) {
                delete_adjedge(e);
                recursive_fold(EEND(e),n,k);
            }
        }
    }
} /* recursive_fold */


/*   Unfold Region
 *   -------------
 *   Different than "fold region", this is done BEFORE the adjacency
 *   lists exist. Unfold the region that has summary node n.
 */
static void unfold_region(GNODE n)
{
    GNLIST  l, startl;
    GNODE   h;

    assert((n));
    debugmessage("unfold_region",(NTITLE(n)?NTITLE(n):"(null)"));

    if (NREGREPL(n)==NULL) return; /* it was no region */

    h = NREGREPL(n);
    startl = l = NREGION(n);

    /* Make n again a normal node, i.e. restore the attributes from h. */

    NLABEL(n)    = NLABEL(h);
    NTEXTMODE(n) = NTEXTMODE(h);
    NWIDTH(n)    = NWIDTH(h);
    NHEIGHT(n)   = NHEIGHT(h);
    NBORDERW(n)  = NBORDERW(h);
    NSX(n)       = NSX(h);
    NSY(n)       = NSY(h);
    NFOLDING(n)  = NFOLDING(h);
    NCOLOR(n)    = NCOLOR(h);
    NTCOLOR(n)   = NTCOLOR(h);
    NBCOLOR(n)   = NBCOLOR(h);
    NSHRINK(n)   = NSHRINK(h);
    NSTRETCH(n)  = NSTRETCH(h);
    NINFO1(n)    = NINFO1(h);
    NINFO2(n)    = NINFO2(h);
    NINFO3(n)    = NINFO3(h);
    NLEVEL(n)    = NLEVEL(h);
    NSHAPE(n)    = NSHAPE(h);
    NHORDER(n)   = NHORDER(h);
    NROOT(n)     = NROOT(h);
    NREGREPL(n)  = NREGREPL(h);
    NREGION(n)   = NREGION(h);
    NREGROOT(n)  = NREGROOT(h);

    free_node(h);         /* give h free */

    for ( ; l; l = GNNEXT(l))
    {
        h = GNNODE(l);
        NREGROOT(h) = NULL;
        insert_node(h, FOLDED_RGNODE);
    }

    free_gnlist_list(startl); /* give the region cons cells free */
} /* unfold_region */

/*--------------------------------------------------------------------*/
/*  Hiding of edges                                                   */
/*--------------------------------------------------------------------*/

/*   Management of Edge Classes
 *   ==========================
 *   The hidden edge classes i have hide_class[i]=1.
 *   The nodes from the nodelist that are hidden are pushed into
 *   the list invis_nodes.
 *   At the beginning of the layout process, these nodes are
 *   repushed into the nodelist.
 */


/*  Insert node into the list invis_nodes.
 *  -------------------------------------
 *  If the node is in the node list, remove it from the node list
 *  and push it into the list invis_nodes.
 */
static void hide_node(GNODE v)
{
    assert(NINLIST(v));

    delete_node(v,HIDDEN_CNODE);
    NNEXT(v) = invis_nodes;
    invis_nodes = v;
}


/*  Hide edges and node
 *  -------------------
 */
static void hide_edge_classes(void)
{
    GEDGE h, nxt_h;
    GNODE v, nxt_v;
    int   allhidden;

    /*assert(invis_nodes == NULL);*/

    for (v = nodelist; v; v = nxt_v)
    {
        nxt_v = NNEXT(v);
        if (FirstPred(v) || FirstSucc(v)) {
            allhidden = 1;
            for (h = FirstPred(v);
                 h && allhidden;
                 h = NextPred(h))
            {
                assert(ECLASS(h)>0);
                if (!hide_class[ECLASS(h)-1]) allhidden=0;
            }
            for (h = FirstSucc(v);
                 h && allhidden;
                 h = NextSucc(h))
            {
                assert(ECLASS(h)>0);
                if (!hide_class[ECLASS(h)-1]) allhidden=0;
            }
            if (allhidden) hide_node(v);
        }
        else { if (hide_single_nodes) hide_node(v); }
    }
    assert(labellist == NULL);
    assert(dummylist == NULL);

    /* I assume that the following is not anymore necessary: (work in ccmir.vcg) */
    for (v = invis_nodes; v; v = NNEXT(v))
    {
        /* delete all outgoing edges */
        for (h = FirstSucc(v); h; h = nxt_h)
        {
            nxt_h = NextSucc(h);
            /*assert(EINVISIBLE(h));*/
            if (!EINVISIBLE(h)) delete_adjedge(h);
        }
    }

    for (h = edgelist; h; h = ENEXT(h))
    {
        assert(ECLASS(h)>0);
        if (hide_class[ECLASS(h)-1]) {
            if (!EINVISIBLE(h)) delete_adjedge(h);
        }
    }
    for (h = tmpedgelist; h; h = ENEXT(h))
    {
        assert(ECLASS(h)>0);
        if (hide_class[ECLASS(h)-1]) {
            if (!EINVISIBLE(h)) delete_adjedge(h);
        }
    }
} /* hide_edge_classes */


/*--------------------------------------------------------------------*/
/*  Refresh the situation                                             */
/*--------------------------------------------------------------------*/

/*   Refresh the whole graph
 *   =======================
 *   i.e. refresh all nodes and edges existing before the layouting.
 *   All layout relevent attributes must become undefined and the
 *   adjacency list must be cleared.
 *   Note: this should be done when maximal many nodes are in the node
 *   list, i.e. no unfolding is done anymore, but before the creation
 *   of adjacency lists.
 */

static void refresh(void)
{
    GNODE   v;
    GEDGE   e;
    char    hh;

    debugmessage("refresh","");

    assert((labellist    == NULL));
    assert((labellistend == NULL));
    assert((tmpnodelist == NULL));

    /* Clear layout attributes of all nodes */

    refresh_all_nodes(nodelist);
    refresh_all_nodes(graphlist);
    refresh_all_nodes(invis_nodes);

    /* Revert reverted edges and make them visible */

    for (e = edgelist; e; e = ENEXT(e))
    {
        if (EART(e) == 'R') {
            v     = ESTART(e);
            ESTART(e) = EEND(e);
            EEND(e)   = v;
            hh = EARROWSIZE(e);
            EARROWSIZE(e)  = EARROWBSIZE(e);
            EARROWBSIZE(e) = hh;
            hh = EARROWCOL(e);
            EARROWCOL(e)  = EARROWBCOL(e);
            EARROWBCOL(e) = hh;
            hh = EARROWSTYLE(e);
            EARROWSTYLE(e)  = EARROWBSTYLE(e);
            EARROWBSTYLE(e) = hh;
        }
        EART(e)     = 'U';
        ELNODE(e)   = NULL;
        EINVISIBLE(e)   = 1;
        EORI(e)     = NO_ORI;
        EORI2(e)    = NO_ORI;
    }
    assert(tmpedgelist == NULL);
}


/*  Refresh all nodes of a list
 *  ---------------------------
 *  Given a nodelist v, connected via NNEXT, all nodes become
 *  undefined deepth, position, mark.
 *  Old adjacency list are cleared.
 */
static void refresh_all_nodes(GNODE v)
{
    debugmessage("refresh_all_nodes","");
    for ( ; v; v = NNEXT(v))
    {
        NTIEFE(v)   = -1;
        NPOS(v)     = -1;
        NMARK(v)    = 0;
        NREVERT(v)  = NOREVERT;
        NDFS(v)     = 0L;
        NX(v)       = 0L;
        NY(v)       = 0L;
        NCONNECT(v) = NULL;
        /*
         * FIXME:
         * Дуги из tmpedgelist перед фолдингом удаляются (т.е. переносятся в список
         * свободных), при этом они остаются слинкованными с узлами и между собой,
         * и с дцгами из списка edgelist.
         * Тут производится отвязывание дуг, которые уже логически удалены.
         * Note: есть еще список invis_nodes, который надо тоже учитывать.
         */
        unlink_node_edges(v);
        /*init_node_adj_fields(v);*/
    }
}

/*  Sort all nodes
 *  --------------
 *  We sort the nodes according to the a criterium. Because the ordering
 *  influences the layout, this increases the stability of the layout,
 *  i.e. after folding of a small subpart, the layout of the large
 *  graph part does not change arbitrary.
 *
 *  The criterium is the following:
 *
 *  layout_flag = 0, 1, 2, 3   -> we use the NREFNUM of the nodes.
 *  layout_flag = 4            -> we use the reverse dfs-depth starting
 *                at that node
 *  layout_flag = 5            -> we use the dfs-depth starting
 *                at that node
 *  layout_flag = 6            -> we use the indegree of that node.
 *  layout_flag = 7            -> we use the reverse indegree of that node.
 *  layout_flag = 8            -> we use the outdegree of that node.
 *  layout_flag = 9            -> we use the reverse outdegree of that node.
 *  layout_flag = 10           -> we use the degree of that node.
 *  layout_flag = 11           -> we use the reverse degree of that node.
 */

static GNODE *node_sort_array = NULL;
static int    noso_size = 0;

static void sort_all_nodes(void)
{
    GNODE v, w;
    int   i,max;


    /* In order to reach stability, we first sort the nodes
     * according to their refnum.
     */

    i = 0;
    for (v = nodelist; v; v = NNEXT(v)) { i++; NDFS(v) = NREFNUM(v); }

    max = i;
    if (max < 2) return;
    if (max > noso_size) {
        if (node_sort_array) free(node_sort_array);
        node_sort_array = (GNODE *)libc_malloc(max*sizeof(GNODE));
        noso_size = max;
    }
    i = 0;
    for (v = nodelist; v; v = NNEXT(v)) { node_sort_array[i++] = v; }

    qsort(node_sort_array,max,sizeof(GNODE),
        (int (*) (const void *, const void *))compare_ndfs);

    /* Now check whether the 10 % time limit is exceeded */

    if (G_timelimit>0) {
        if (test_timelimit(10)) {
            layout_flag = 1;
            gs_wait_message('t');
        }
    }


    /* Now, we sort according to a second criterium, e.g. degree, etc.
     */

    switch (layout_flag) {
    case 4: for (v = nodelist; v; v = NNEXT(v))
        {
            if (G_timelimit>0) {
                if (test_timelimit(15)) {
                    gs_wait_message('t');
                    layout_flag = 1;
                    break;
                }
            }
            for (w = nodelist; w; w = NNEXT(w)) { NMARK(w) = 0; }
            NDFS(v) =  - no_dfs(v);
        }
        break;
    case 5: for (v = nodelist; v; v = NNEXT(v))
        {
            if (G_timelimit>0) {
                if (test_timelimit(15)) {
                    gs_wait_message('t');
                    layout_flag = 1;
                    break;
                }
            }
            for (w = nodelist; w; w = NNEXT(w)) { NMARK(w) = 0; }
            NDFS(v) =  no_dfs(v);
        }
        break;
    case 6: while (v) {
            NDFS(v) =  no_indeg(v);
            v = NNEXT(v);
        }
        break;
    case 7: while (v) {
            NDFS(v) =  - no_indeg(v);
            v = NNEXT(v);
        }
        break;
    case 8: while (v) {
            NDFS(v) =  no_outdeg(v);
            v = NNEXT(v);
        }
        break;
    case 9: while (v) {
            NDFS(v) =  - no_outdeg(v);
            v = NNEXT(v);
        }
        break;
    case 10:
        while (v) {
            NDFS(v) =  no_degree(v);
            v = NNEXT(v);
        }
        break;
    case 11:
        while (v) {
            NDFS(v) =  - no_degree(v);
            v = NNEXT(v);
        }
        break;
    }

    switch (layout_flag) {
    case 4: case 5: case 6: case 7: case 8:
    case 9: case 10: case 11:

        qsort(node_sort_array,max,sizeof(GNODE),
            (int (*) (const void *, const void *))compare_ndfs);

    }

    for (i=1; i<max-1; i++) {
        NNEXT(node_sort_array[i]) = node_sort_array[i+1];
        NBEFORE(node_sort_array[i]) = node_sort_array[i-1];
    }
    NNEXT(node_sort_array[0])       = node_sort_array[1];
    NBEFORE(node_sort_array[0])     = NULL;
    NNEXT(node_sort_array[max-1])   = NULL;
    NBEFORE(node_sort_array[max-1]) = node_sort_array[max-2];
    nodelist    = node_sort_array[0];
    nodelistend = node_sort_array[max-1];

    for (v = nodelist; v; v = NNEXT(v)) { NDFS(v) = 0L; NMARK(v) = 0; }
} /* sort_all_nodes */


/*  Compare function for sorting according dfs values
 *  -------------------------------------------------
 *  returns 1 if NDFS(*a) > NDFS(*b), 0 if equal, -1 otherwise.
 */

static int      compare_ndfs(const GNODE *a,const GNODE *b)
{
        if (NDFS(*a) > NDFS(*b))                return(1);
        if (NDFS(*a) < NDFS(*b))                return(-1);
        return(0);
}


/*  Set the dfs value of a node
 *  ---------------------------
 *  The idea is that we look for the maximal depth of the dfs spanning
 *  tree starting at that node.
 *  If we later sort the nodes according to the negative dfs value,
 *  The first nodes are the nodes that create a maximal depth layout,
 *  because their dfs value is maximal.
 *  Conversly, using positive dfs values, we get a minimal depth
 *  layout.
 */
static long no_dfs(GNODE n)
{
    GEDGE edge;
    long res, z;

    if (NMARK(n)) return 0L;
    NMARK(n) = 1;
    res = 0L;
    for (edge = FirstSucc(n); edge; edge = NextSucc(edge))
    {
        z = no_dfs(ETARGET(edge)) + 1L;
        if (z>res)  res = z;
    }
    return res;
}


/* Calculate the indegree of a node
 * --------------------------------
 */
static long no_indeg(GNODE n)
{
    GEDGE edge;
    long res = 0;

    for (edge = FirstPred(n); edge; edge = NextPred(edge))
    {
        res++;
    }
    return res;
}

/* Calculate the outdegree of a node
 * --------------------------------
 */
static long no_outdeg(GNODE n)
{
    GEDGE edge;
    long res = 0;

    for (edge = FirstSucc(n); edge; edge = NextSucc(edge))
    {
        res++;
    }
    return res;
}

/* Calculate the degree of a node
 * --------------------------------
 */
static long no_degree(GNODE n)
{
    return no_indeg(n) + no_outdeg(n);
}


/*--------------------------------------------------------------------*/
/*  Creation and management of adjacency lists                        */
/*--------------------------------------------------------------------*/

/*  Primitives
 *  ==========
 */


/*   Create an adjacency
 *   -------------------
 *   i.e. insert an edge into the adjacency lists of its source and
 *   target node.
 */
void    create_adjedge(GEDGE edge)
{
    assert(EINVISIBLE(edge) == 1);
    link_edge(edge);
    EINVISIBLE(edge) = 0;
}

/*   Delete an adjacency
 *   -------------------
 *   i.e. delete an edge e from the adjacency lists of its source
 *   and target node.
 */
void    delete_adjedge(GEDGE edge)
{
    assert(EINVISIBLE(edge) == 0);
    unlink_edge(edge);
    EINVISIBLE(edge) = 1;
}


/*   Create an label node
 *   --------------------
 *   For adjacencies with labels, we create label nodes and auxiliary
 *   edges between the label:   s -> label -> t.
 *   This simplifies the layout, because edge labels can be dealed
 *   as nodes.
 *   The new temporary node is inserted into the label list.
 */
GNODE   create_labelnode(GEDGE e)
{
    GNODE   v;

    debugmessage("create_labelnode","");
    v = tmpnodealloc(CENTER,-1,-1,0,
            G_color,ELABELCOL(e),ELABELCOL(e));
    NSX(v) = (NSX(ESTART(e))+NSX(EEND(e)))/2L;
    NSY(v) = (NSY(ESTART(e))+NSY(EEND(e)))/2L;
    NTITLE(v)   = "";
    NLABEL(v)   = ELABEL(e);
    adapt_labelpos(v,e);
    ins_node_in_dl_list(v,labellist,labellistend);
    ELNODE(e) = v;
    return(v);
}

/*   Calculate the string size of an label
 *   -------------------------------------
 *   and adapt the coordinates of v. The coordinates of labels derived
 *   from nodes that have no coordinates may be wrong; but in this
 *   case, all positions are recalculated anyway.
 */
static void adapt_labelpos(GNODE v, GEDGE e)
{
    char *ss;
int gs_stringw =8;
int gs_stringh =16;
    assert((v));
    ss = NLABEL(v);
    if (!ss) return;
    if (NSHRINK(v)==0) NSHRINK(v) = 1;
/*TODO  gs_setshrink(NSTRETCH(v),NSHRINK(v));*/
/*TODO  gs_calcstringsize(ss);*/
/*TODO  gs_stringw += (ETHICKNESS(e)/2+1);
    gs_stringh += (ETHICKNESS(e)/2+1);*/
    NWIDTH(v)  = gs_stringw;
    NHEIGHT(v) = gs_stringh;
/*TODO  NSX(v) = NSX(v) - (long)(gs_stringw/2);
    NSY(v) = NSY(v) - (long)(gs_stringh/2);*/
}


/*   Search visible node
 *   -------------------
 *   given a node v of a subgraph or region nest, look for the
 *   innerst subgraph summary node or region node that is visible.
 *   Return NULL if no such node exists.
 *   The Search is driven by the reason why v is unvisible.
 */

static GNODE    search_visible(GNODE v)
{
    GNODE w;

    debugmessage("search_visible","");

    /* Dangerous: I'm not sure that this terminates always */

    if (!v) return(NULL);
    if (NINLIST(v)) return(v);
    switch (NINVISIBLE(v)) {
        case UNFOLDED_SGRAPH:
            w = v;
            while (NINVISIBLE(w)==UNFOLDED_SGRAPH) {
                w = GNNODE(NSGRAPH(v));             /* WTF */
            }
            if (NINVISIBLE(w)==FOLDED_SGNODE)
                return(search_visible(NROOT(v)));   /* WTF */
            else
                return(search_visible(GNNODE(NSGRAPH(v))));
        case FOLDED_SGNODE:
            return(search_visible(NROOT(v)));
        case FOLDED_RGNODE:
            return(search_visible(NREGROOT(v)));
    }
    return(NULL);
}


/*   Check substitution of edges
 *   ---------------------------
 *   If a subgraph is folded, all edges to nodes of this subgraph must
 *   be replaced by edges to the summary node.
 *   This function returns e,                   if e must be drawn,
 *                         a substitution edge, if e is from a visible node
 *                                              to an invisible subgraph node,
 *                         NULL,                if neither e nor substitution edge
                                                must be drawn.
 *
 *   Note the situation we expect: edgelist contains all stable edge,
 *   independent of visible or not.
 *   tmpedgelist contains the temporary edges that may be additionally
 *   visible.
 *   Subgraph folding/unfolding was done before, and region folding
 *   and edge hiding is done after that !
 */

static GEDGE    substed_edge(GEDGE e)
{
    GNODE   s,t,ss,tt;
    GEDGE   h;

    assert((e));
    debugmessage("substed_edge","");

    /* We assume: e is in edgelist !!! */

    s = ESTART(e);
    t = EEND(e);
    if ( NINLIST(s) && NINLIST(t) ) return(e);  /* edge must be drawn */

    ss = search_visible(s);
    tt = search_visible(t);

    /* We never want to have an edge from a folded region/subgraph to
     * the region/subgraph summary node. This would create unnecessary
     * self loops.
     */
    if (((ss!=s)||(tt!=t))&&(ss==tt)) return(NULL);

    /*  It may really happen that s or t are ZERO, e.g. if there are
     *  invisible subgraphs that are not folded but don't contain nodes.
     */
    if ( !ss || !tt ) return(NULL);

    /*  Look whether we have already a substitution between s and t.
     *  Before the first call, we assume that tmpedgelist is NULL.
     */

    for (h = tmpedgelist; h; h = ENEXT(h))
    {
        assert(!EINVISIBLE(h));
        /*if ((ESTART(h)==ss) && (EEND(h)==tt)) return(h);*/
        if ((ESTART(h)==ss) && (EEND(h)==tt)) return NULL;
    }

    /*  HERE IS THE POSSIBLE ENTRY POINT TO GIVE THE EDGES TO FOLDED
     *  SUBGRAPHS A SPECIAL ATTRIBUTE
     */

    h = tmpedgealloc(
        ELSTYLE(e),
        ETHICKNESS(e),
        ECLASS(e),
        EPRIO(e),
        ECOLOR(e),
        EARROWSIZE(e),
        EARROWBSIZE(e),
        EARROWSTYLE(e),
        EARROWBSTYLE(e),
        EARROWCOL(e),
        EARROWBCOL(e),
        EHORDER(e));
    if (s==ss) EANCHOR(h) = EANCHOR(e);

    inherit_foldedge_attributes(&foldedge, h);

    ESTART(h)   = ss;
    EEND(h)     = tt;
    return(h);
} /* substed_edge */



/*   Create adjacency lists
 *   ======================
 *   Adjacency lists of all nodes in the nodelist, i.e. of all visible nodes.
 */
static void create_adjacencies(void)
{
    GEDGE edge, e;

    assert(tmpedgelist == NULL);
    for (edge = edgelist; edge; edge = ENEXT(edge))
    {
        e = substed_edge(edge);
        if (e) create_adjedge(e);
    }
}



/*   Add label nodes
 *   ======================
 */
static void add_labels_at_folding(void)
{
    GNODE v, vlab;
    GEDGE e, nxt_e, e1, e2;

    assert(labellist == NULL);
    assert(dummylist == NULL);
    for (v = nodelist; v; v = NNEXT(v))
    {
        for (e = FirstSucc(v); e; e = nxt_e)
        {
            nxt_e = NextSucc(e);
            assert(!EINVISIBLE(e));
            if (ELABEL(e)) {
                vlab = create_labelnode(e);
                e1 = create_edge(ESOURCE(e), vlab, e, 0);
                e2 = create_edge(vlab, ETARGET(e), e, 1);
                EANCHOR(e2) = 0;
                delete_adjedge(e);
            }
        }
    }
}

/*--------------------------------------------------------------------*/

/* Summarize doublicated edges
 * ---------------------------
 * It is sometimes very ugly to have many identical edges from one node
 * to the same other node. To avoid to layout such situations,
 * we delete all duplicates. This happens only for edges without
 * labels.
 * But note that this function is very slow and may be cubic in
 * complexity. Thus it is optional.
 */
static void summarize_edges(void)
{
    GNODE v;
    GEDGE e1, nxt_e1, e2;
    int found, ide;

    for (v = nodelist; v; v = NNEXT(v))
    {
        for (e1 = FirstSucc(v); e1; e1 = nxt_e1)
        {
            nxt_e1 = NextSucc(e1);
            found = 0;
            for (e2 = FirstSucc(v); e2; e2 = NextSucc(e2))
            {
                if (e2 == e1)
                    continue;
                ide = 1;
                if (ESTART(e1)      != ESTART(e2))       ide=0;
                if (EEND(e1)        != EEND(e2))         ide=0;
                if (ELSTYLE(e1)     != ELSTYLE(e2))      ide=0;
                if (ETHICKNESS(e1)  >  ETHICKNESS(e2))   ide=0;
                if (EPRIO(e1)       >  EPRIO(e2))        ide=0;
                if (EHORDER(e1)     != EHORDER(e2))      ide=0;
                if (ECLASS(e1)      != ECLASS(e2))       ide=0;
                if (ECOLOR(e1)      != ECOLOR(e2))       ide=0;
                if (EARROWSIZE(e1)  != EARROWSIZE(e2))   ide=0;
                if (EARROWBSIZE(e1) != EARROWBSIZE(e2))  ide=0;
                if (EARROWSTYLE(e1) != EARROWSTYLE(e2))  ide=0;
                if (EARROWBSTYLE(e1)!= EARROWBSTYLE(e2)) ide=0;
                if (EARROWCOL(e1)   != EARROWCOL(e2))    ide=0;
                if (EARROWBCOL(e1)  != EARROWBCOL(e2))   ide=0;
                if (EANCHOR(e1)     != EANCHOR(e2))      ide=0;

                if (ide) {
                    found++; break;
                }
            }
            if (found) delete_adjedge(e1);
        }
    }
#ifdef CHECK_ASSERTIONS
    for (v = labellist; v; v = NNEXT(v))
    {
        /* only one edge !!! */
        assert(NextSucc(FirstSucc(v)) == NULL);
    }
#endif
    assert(dummylist == NULL);
} /* summarize_edges */



/*--------------------------------------------------------------------*/

/* Split doublicated edges
 * -----------------------
 * This is something like the converse of "summarize edges":
 * If we have several edges between two nodes a and b, these edges
 * may be drawn at the same place such that we cannot distinguish
 * them. Thus, we add dummy label nodes to them, which are layouted
 * at different places such that we we can see them.
 *
 * Example:   A ===> B   is drawn as   A--->B
 *                                      \   ^
 *                                       \_/
 *
 * Note: self loops are not split since we have a special treatment
 * of self loops.
 */
static void split_double_edges(void)
{
    GNODE v, w;
    GEDGE e, nxt_e, c, e1, e2;
    int found;

    for (v = nodelist; v; v = NNEXT(v))
    {
        for (e = FirstSucc(v); e; e = nxt_e)
        {
            nxt_e = NextSucc(e);
            if (ETARGET(e) == ESOURCE(e)) {
                continue;       /* selfloop */
            }
            found = 0;
            for (c = FirstSucc(v); c; c = NextSucc(c))
            {
                if ((c!=e)&&(ETARGET(c)==ETARGET(e))) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                w = create_labelnode(e);
                NLABEL(w) = "";
                NWIDTH(w) = NHEIGHT(w) = 0;
                /*ELNODE(e) = NULL; ???*/
                e1 = create_edge(ESOURCE(e), w, e, 0);
                ELABEL(e1)    = ELABEL(e);
                ELABELCOL(e1) = ELABELCOL(e);
                e2 = create_edge(w, ETARGET(e), e, 1);
                EANCHOR(e2) = 0;
                delete_adjedge(e);
            }
        }
    }
#ifdef CHECK_ASSERTIONS
    for (v = labellist; v; v = NNEXT(v))
    {
        /* only one edge !!! */
        assert(NextSucc(FirstSucc(v)) == NULL);
    }
#endif
    assert(dummylist == NULL);
} /* split_double_edges */



/*--------------------------------------------------------------------*/

#ifdef DEBUG

/* Debugging function: print a double linked nodelist
 * --------------------------------------------------
 * w is startnode, wend is endnode. We print maximal
 * DB_MAXNODES nodes to avoid to come into an infinite
 * loop, if the data structure is incorrect.
 */

#define DB_MAXNODES 25

static void db_print_somenode_list(GNODE w,GNODE wend)
{
    GNODE v; int i;

    i = 0;
    PRINTF("Addresses Startnode %p Endnode %p\n",w,wend);
    for (v = w; v; v = NNEXT(v))
    {
        i++; if (i>DB_MAXNODES) break;
        PRINTF("Address %p:%s [%d]    (Address next: %p)\n",
            v, NTITLE(v)?NTITLE(v):"(null)", NINVISIBLE(v),
            NNEXT(v));
    }
}
#endif


