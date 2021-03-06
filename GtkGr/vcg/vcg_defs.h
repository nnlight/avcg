#ifndef _VCG_DEFS_H_
#define _VCG_DEFS_H_

#ifdef __cplusplus
extern "C" {
#endif



/*--------------------------------------------------------------------*/

/* Attribute values
 *-----------------
 */

/* Color definitions
 * - - - - - - - - -
 */

/* Graph orientation */

#define TOP_TO_BOTTOM   0
#define LEFT_TO_RIGHT   1
#define RIGHT_TO_LEFT   2
#define BOTTOM_TO_TOP   3

/* Node y-alignment */

#define AL_TOP    0
#define AL_CENTER 1
#define AL_BOTTOM 2

/* Display edge labels */

#define NO  0
#define YES 1

/* Arrow modi */

#define AMFIXED 0
#define AMFREE  1

/* Edge orientation (no orientation, north, north east, ...) */

#define NO_ORI      0
#define ORI_NORTH   1
#define ORI_NORTHEAST   2
#define ORI_NORTHWEST   3
#define ORI_SOUTH   4
#define ORI_SOUTHEAST   5
#define ORI_SOUTHWEST   6
#define ORI_EAST    7
#define ORI_WEST    8

/*enum Listdir_t
{
    LD_PREV,
    LD_NEXT,
    LD_COUNT
};*/

/* adjacency list id: direction in gaph */
typedef enum Graphdir_t
{
    GD_PRED,  /* predecessor(s) (list) */
    GD_SUCC,  /* successor(s)  (list) */
    GD_COUNT
} Graphdir_t;

typedef enum Tempattr_t {
    /* values of temp attr should not be used directly */
    TEMP_ATTR_COUNT = 1
} Tempattr_t;

#define MARKER_COUNT 1
typedef unsigned Marker_t;
#define MARKER_IDX_BITS  2
#define MARKER_VAL_BITS  (sizeof(unsigned)*8 - MARKER_IDX_BITS)
#define MARKER_IDX_MASK  ((1 << MARKER_IDX_BITS) - 1)

/*--------------------------------------------------------------------*/

/*  Auxiliary Structs
 *  =================
 */

/*  Connections
 *  ------------
 */

/*  Connections are necessary for the layout of directly neighboured
 *  connected nodes at the same level. This situation may occur accidently,
 *  but automatical occurs if there is a self loop.
 *
 *  On self loops:
 *  - - - - - - -      N            A self loop of N is an edge (N,N).
 *                    / ^           To layout this, we create a dummy node
 *                   /   \          D and a pseudo node P. D and P are
 *                  D-----P         always neighboured at the same level.
 *
 *  Only N and D is layouted. The connection of the dummy node D contains the
 *  information needed to draw P (that is not layouted), i.e.:
 *     CTARGET(D)  -> P
 *     CEDGE(D)    -> edge (D,P)
 *     CTARGET(P)  -> D
 *     CEDGE(P)    -> edge (D,P)
 *
 *  On connected neigboured nodes:
 *  - - - - - - - - - - - - - - -
 *              /    |    \       or    |     \      or     /     |
 *             A<----B---->C            B----->C           C<-----B
 *
 *  Only B is layouted. The connection of B contains the information
 *  to draw A and/or C, i.e.
 *     CTARGET(B)  -> C
 *     CEDGE(B)    -> edge (B,C)
 *     CTARGET2(B) -> A           or NULL, if not necessary
 *     CEDGE2(B)   -> edge (B,A)  or NULL, if not necessary
 *
 *     CTARGET(C)  -> B
 *     CEDGE(C)    -> edge (B,C)
 *     etc.
 *
 *  We call the connection field of B a `forward connection' and the
 *  connection field of B or A a `backward connection', because its
 *  in converse of the edge direction.
 *  Note that A or C can also contain connections, e.g. on
 *               |    \     \
 *               B---->C---->D           CTARGET(B) = C, CTARGET(C) = D, ...
 *
 *
 */
typedef struct connect
{
    struct gnode    *target;        /* First found target         */
    struct gedge    *edge;          /* and its edge               */
    struct gnode    *target2;       /* Second found target        */
    struct gedge    *edge2;         /* and its edge               */

    /* At connections we temporary change the adjacency lists.
     * Thus we store the original list in save fields.
     */
    struct  adjedge *savepred;      /* adjacency list: predecessors */
    struct  adjedge *savesucc;      /* adjacency list: successors   */

    struct connect  *internal_next; /* for memory allocation only */
} *CONNECT;

#define CTARGET(x)      ((x)->target)
#define CEDGE(x)        ((x)->edge)
#define CTARGET2(x)     ((x)->target2)
#define CEDGE2(x)       ((x)->edge2)
#define CSVPRED(x)      ((x)->savepred)
#define CSVSUCC(x)      ((x)->savesucc)
#define CINTERN(x)      ((x)->internal_next)

#if VCG_USE_MACROS
#define forward_connection1(c)  ((CEDGE(c))&& (EEND(CEDGE(c)) ==CTARGET(c)))
#define forward_connection2(c)  ((CEDGE2(c))&&(EEND(CEDGE2(c))==CTARGET2(c)))
#define backward_connection1(c) ((CEDGE(c))&& (EEND(CEDGE(c)) !=CTARGET(c)))
#define backward_connection2(c) ((CEDGE2(c))&&(EEND(CEDGE2(c))!=CTARGET2(c)))
#else
extern int forward_connection1(CONNECT c);
extern int forward_connection2(CONNECT c);
extern int backward_connection1(CONNECT c);
extern int backward_connection2(CONNECT c);
#endif


/*--------------------------------------------------------------------*/

/*  Graphs
 *  ======
 *  The complete data structure of a graph is an adjacency list
 *  representation.
 *  Furthermore, all nodes contain in NROOT a pointer to the subgraph
 *  summary node they belong to. NROOT of top level nodes is NULL.
 *  Summary nodes of subgraphs contain in NSGRAPH a list of all nodes
 *  that belong to this subgraph.
 *  Furthermore all real nodes are in the node list, while all graph
 *  summary nodes are in the graphlist.
 *  E.g. the top graph contains a node A and a graph S1
 *      which contains nodes B and C and a graph S2
 *              which contains nodes D and E
 *  NROOT connection:
 *
 *     nodelist --> A --> B ---> C --> D ---> E
 *                  |      \    /       \    /
 *                 NULL     \  /         \  /NROOT
 *                           \/           \/
 *     graphlist ----------> S1 --------> S2
 *                            |            |
 *                            V            V
 *                          NULL          NULL
 *
 *                        ,-----------------------------,
 *  NSGRAPH connection:   |            ,-------------,  |
 *                        V            V             |  |
 *                        *----->*->*  *----->*      |  |
 *                        |      |  |  |      |      |  |
 *                        V      V  |  V      V      |  |
 *     nodelist --> A --> B ---> C -+> D ---> E      |  |
 *                                  |                |  |
 *                                  `-----,          |  |
 *                                        |   NSGRAPH|  |NSGRAPH
 *                                        V          |  |
 *     graphlist ----------> S1 --------> S2         |  |
 *                           |            |          |  |
 *                           |            `----------'  |
 *                           `--------------------------'
 */


/*  Nodes of a graph
 *  ----------------
 *  Graphs are also represented as nodes, i.e. as summary node.
 *  Graphs s are initially not in the nodelist, but in the graphlist.
 *  In consequence, NINLIST(s) is 0 for them.
 */

typedef struct gnode
{

    /* Each node has an unique reference number.
     * These are used to debug, and to have a stable layout,
     * because we use the numbers as sorting criteria.
     */

    long    refnum;

    /* These attributes come directly from the specification */

    char    *title;
    char    *label;
    char    *info1;
    char    *info2;
    char    *info3;
    int width;
    int height;
    long    sxloc;
    long    syloc;
    char    textmode;
    char    borderwidth;
    char    folding;
    char    color;
    char    textcolor;
    char    bordercolor;
    char    shape;
    int shrink;
    int stretch;
    int level;
    int nhorder;

    struct  gnlist  *subgraph;   /* List of subgraph nodes  */
    struct  gnode   *root;       /* Root of graph           */
    struct  gnode   *regionrepl; /* Replacement node for    */
                                 /* roots of regions        */
    struct  gnlist  *region;     /* List of nodes in region */
    struct  gnode   *regroot;    /* Root of region          */

    /* These are the locations used for the layout */

    long    xloc;
    long    yloc;

    /* nodelist, graphlist links: these are double linked lists */

    struct  gnode   *next;      /* successor in the node/graphlist   */
    struct  gnode   *before;    /* predecessor in the node/graphlist */

    char    in_nodelist;        /* flag, 1 if node is in nodelist */
    char    invisible;          /* flag, 1 if node is invisible   */

    /* layout attributes: nodes are distributed into layers according
     * to their deeths relatively to the root of the graph, and have
     * positions in these layers.
     */

    char    markiert;           /* marker for depth first search    */
    char    revert;             /* marker for reverted drawing      */
    char    anchordummy;        /* marker for anchor dummy nodes    */
    int tiefe;          /* the number (deepth) of the layer */
    int position;           /* the position in the layer        */
    float   bary;               /* the weight of the bary centering */

    /* The following two fields have sev. purposes: they are the layout
     * weights nws and nwp of the layout algorithm, and on drawing
     * they are used as number of anchor points.
     * The field weights is also used during the partitioning of
     * edges as LOWPT of the strongly connected component.
     * The field weightp is aslo used during the partitioning of
     * edges as OPENSCC flag of the strongly connected component.
     */

    long    weights;        /* node weight succ. (nws)   */
    long    weightp;        /* node weight pred. (nwp)   */

    /* For partitioning the edges, we need to know cross edges, tree
     * edges, forward edges and backward edges. Thus, we calculate a
     * depth first search (dfs) and protocol by numbers when we entry
     * the dfs for a node and when we leave the dfs of this node.
     */

    long    dfsnum;         /* the dfs entry number      */

    /* To calculate crossings, we need a pointer to the last instance
     * of the upper or lower completion lists. See step2.c
     */

    struct  dllist  *Vpointer;  /* this crossing pointer        */

    void *temp_attr[TEMP_ATTR_COUNT];
    unsigned marker_val[MARKER_COUNT];

    /* The graph is represented with adjacency lists. We have some
     * fast accesses to the leftest or rightedst prede/successor.
     * Further, at connections we temporary change the adjacency
     * lists. Thus we store the original list in save fields.
     * For crossing calculation, we need to reorder the adjacency
     * lists. Thus we use a pointer tmpadj to the actual position
     * in the adjacency list.
     */
    /*struct    adjedge *tmpadj;*/          /* temporary adjacency list     */
    /*struct    adjedge *pred;*/            /* adjacency list: predecessors */
    /*struct    adjedge *succ;*/            /* adjacency list: successors   */
    /*struct    adjedge *savepred;*/        /* adjacency list: predecessors */
    /*struct    adjedge *savesucc;*/        /* adjacency list: successors   */
    /*struct    gedge   *predleft;*/        /* leftest predecessor          */
    /*struct    gedge   *predright;*/       /* rightest predecessor         */
    /*struct    gedge   *succleft;*/    /* leftest successor        */
    /*struct    gedge   *succright;*/   /* rightest successor           */
    struct gedge *adjfirst[GD_COUNT]; /* adjacency list, first edge */
    struct gedge *adjlast[GD_COUNT];  /* adjacency list, last edge  */

    struct  connect *connection;    /* horizontal connection to a   */
                    /* neighboured node, see above  */

    /* The next field has two purposes: During parsing, we use it to
     * create a hash table of all nontemporary node, to have fast
     * access to the title.
     * Otherwise, it is used for the memory management of temporary
     * nodes.
     */

    struct  gnode   *internal_next;
} *GNODE;

#define NREFNUM(x)      ((x)->refnum)
#define NTITLE(x)       ((x)->title)
#define NLABEL(x)       ((x)->label)
#define NINFO1(x)       ((x)->info1)
#define NINFO2(x)       ((x)->info2)
#define NINFO3(x)       ((x)->info3)
#define NTEXTMODE(x)    ((x)->textmode)
#define NWIDTH(x)       ((x)->width)
#define NHEIGHT(x)      ((x)->height)
#define NSHAPE(x)       ((x)->shape)
#define NBORDERW(x)     ((x)->borderwidth)
#define NSX(x)          ((x)->sxloc)
#define NSY(x)          ((x)->syloc)
#define NX(x)           ((x)->xloc)
#define NY(x)           ((x)->yloc)
#define NFOLDING(x)     ((x)->folding)
#define NCOLOR(x)       ((x)->color)
#define NTCOLOR(x)      ((x)->textcolor)
#define NBCOLOR(x)      ((x)->bordercolor)
#define NSHRINK(x)      ((x)->shrink)
#define NSTRETCH(x)     ((x)->stretch)
#define NLEVEL(x)       ((x)->level)
#define NHORDER(x)      ((x)->nhorder)
#define NSGRAPH(x)      ((x)->subgraph)
#define NROOT(x)        ((x)->root)
#define NREGREPL(x)     ((x)->regionrepl)
#define NREGION(x)      ((x)->region)
#define NREGROOT(x)     ((x)->regroot)
#define NNEXT(x)        ((x)->next)
#define NBEFORE(x)      ((x)->before)
#define NPREV(x)        ((x)->before)
#define NINLIST(x)      ((x)->in_nodelist)
#define NINVISIBLE(x)   ((x)->invisible)
#define NTIEFE(x)       ((x)->tiefe)
#define NPOS(x)         ((x)->position)
#define NMARK(x)        ((x)->markiert)
#define NREVERT(x)      ((x)->revert)
#define NANCHORNODE(x)  ((x)->anchordummy)
#define NBARY(x)        ((x)->bary)
#define NLOWPT(x)       ((x)->weights)
#define NOPENSCC(x)     ((x)->weightp)
#define NWEIGHTS(x)     ((x)->weights)
#define NWEIGHTP(x)     ((x)->weightp)
#define NDFS(x)         ((x)->dfsnum)
#define NHIGHPRIO(x)    ((x)->dfsnum)
#define NVPTR(x)        ((x)->Vpointer)
#define NTEMPATTR(x,i)  ((x)->temp_attr[i])  /* for internal use */
#define NMARKERVAL(x,i) ((x)->marker_val[i]) /* for internal use */
#define NTMPADJ(x)      ((x)->tmpadj)
#define NPRED(x)        ((x)->pred)
#define NSUCC(x)        ((x)->succ)
#define NSVPRED(x)      ((x)->savepred)
#define NSVSUCC(x)      ((x)->savesucc)
#define NPREDL(x)       FirstPred(x)
#define NPREDR(x)       LastPred(x)
#define NSUCCL(x)       FirstSucc(x)
#define NSUCCR(x)       LastSucc(x)
#define NADJFIRST(x,di) ((x)->adjfirst[di])  /* for internal use */
#define NADJLAST(x,dir) ((x)->adjlast[dir])  /* for internal use */
#if VCG_USE_MACROS
#define FirstPred(x)    NADJFIRST(x, GD_PRED)
#define FirstSucc(x)    NADJFIRST(x, GD_SUCC)
#endif
#define LastPred(x)     NADJLAST(x, GD_PRED)
#define LastSucc(x)     NADJLAST(x, GD_SUCC)
#define NCONNECT(x)     ((x)->connection)
#define NINTERN(x)      ((x)->internal_next)


/* For NREVERT, we allow the values:
 * ---------------------------------
 */

#define NOREVERT  0
#define AREVERT   1
#define BREVERT   2


/*--------------------------------------------------------------------*/

/*  List of GNODE-objects
 *  ---------------------
 *  These are used for various reasons: as list of nodes of a (sub)graph,
 *  as lists of nodes of a region, etc.
 */

typedef struct gnlist
{
    struct gnode    *node;           /* The node              */
    struct gnlist   *next;           /* The remaining list    */
    struct gnlist   *internal_next;  /* For memory management */
} *GNLIST;

#define GNNODE(x)   ((x)->node)
#define GNNEXT(x)   ((x)->next)
#define GNINTERN(x) ((x)->internal_next)

/*--------------------------------------------------------------------*/


/*  Edges of a graph
 *  ----------------
 *  During the layout, edges are partitioned into reverted edges,
 *  etc. Thus, we have for `kantenart', or EART, or EKIND, respectively,
 *  the following possibilities:
 *  'U' -> the kind of this edge is undefined, (it is a normal edge)
 *  'S' -> this is a self loop
 *  'R' -> this is a reverted edge.
 *  If we do not self-layout, we have further:
 *  'l' -> this edge goes to the left
 *  'r' -> this edge goes to the right
 *
 *  The following tags indicate the initial partitionig, but they are not
 *  anymore used.
 *  'T' -> this is a tree edge
 *  'F' -> this is a forward edge
 *  'B' -> this is a backward edge (not yet reverted)
 *  'C' -> this is a cross edge
 *
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
 *      i.e. each edge have start point and end point, and further a
 *      bend point that has the same x-co-ordinate as the start point
 *      (hold for top_down_layout).
 */

typedef struct gedge
{
    struct  gnode   *start;    /* Source node             */
    long    sxloc;             /* and its co-ordinates    */
    long    syloc;
    long    btxloc;        /* Bend location on top of */
    long    btyloc;        /* a node                  */
    long    bbxloc;        /* Bend location on bottom */
    long    bbyloc;
    struct  gnode   *end;      /* Target node             */
    long    exloc;             /* and its co-ordinates    */
    long    eyloc;
    char    orientation;       /* updown, northwest, etc. */
    char    orientation2;      /* dito, for double edges  */

    /* These attributes come directly from the specification */

    char    linestyle;
    char    thickness;
    char    *label;
    int priority;
    int horder;
    char    eclass;
    char    color;
    char    arrowstyle1;
    char    arrowstyle2;
    char    arrowsize1;
    char    arrowcolor1;
    char    arrowsize2;
    char    arrowcolor2;
    char    labelcolor;

    signed char    anchor;

    /* The layout decides wether an edge is visible or reverted, etc. */

    char    kantenart;      /* flag 'U', 'S' etc. see above  */
    char    invisible;      /* 1, if the edge is not visible */

    /* The following two fields are the layout weights ews and ewp of
     * the layout algorithm. See the documentation of the layout alg.
     * On drawing, they are further used as number of anchor point.
     */

    long    weights;          /* edge weight succ. (ews)   */
    long    weightp;      /* edge weight pred. (ewp)   */

    struct  gnode   *labelnode; /* Label node if the edge is replaced */

    struct  gedge   *next;       /* [tmp]edgelist next element */

    /* adjacency lists: these are double linked lists */
    struct gedge *adjnext[GD_COUNT]; /* adjacency list, next edge */
    struct gedge *adjprev[GD_COUNT]; /* adjacency list, prev edge */
} *GEDGE;

#define ESTART(x)       ((x)->start)
#define EEND(x)         ((x)->end)
#define ESOURCE(x)      ((x)->start)
#define ETARGET(x)      ((x)->end)
#define ESTARTX(x)      ((x)->sxloc)
#define ESTARTY(x)      ((x)->syloc)
#define ETBENDX(x)      ((x)->btxloc)
#define ETBENDY(x)      ((x)->btyloc)
#define EBBENDX(x)      ((x)->bbxloc)
#define EBBENDY(x)      ((x)->bbyloc)
#define EENDX(x)        ((x)->exloc)
#define EENDY(x)        ((x)->eyloc)
#define EORI(x)         ((x)->orientation)
#define EORI2(x)        ((x)->orientation2)
#define ELABEL(x)       ((x)->label)
#define ELABELCOL(x)    ((x)->labelcolor)
#define ELSTYLE(x)      ((x)->linestyle)
#define ETHICKNESS(x)   ((x)->thickness)
#define ECLASS(x)       ((x)->eclass)
#define EPRIO(x)        ((x)->priority)
#define EHORDER(x)      ((x)->horder)
#define ECOLOR(x)       ((x)->color)
#define EARROWSTYLE(x)  ((x)->arrowstyle1)
#define EARROWBSTYLE(x) ((x)->arrowstyle2)
#define EARROWSIZE(x)   ((x)->arrowsize1)
#define EARROWBSIZE(x)  ((x)->arrowsize2)
#define EARROWCOL(x)    ((x)->arrowcolor1)
#define EARROWBCOL(x)   ((x)->arrowcolor2)
#define EANCHOR(x)      ((x)->anchor)
#define ELNODE(x)       ((x)->labelnode)
#define ENEXT(x)        ((x)->next)
#define EADJNEXT(x,dir) ((x)->adjnext[dir])
#define EADJPREV(x,dir) ((x)->adjprev[dir])
#if VCG_USE_MACROS
#define NextPred(x)     EADJNEXT(x, GD_PRED)
#define NextSucc(x)     EADJNEXT(x, GD_SUCC)
#endif
#define PrevPred(x)     EADJPREV(x, GD_PRED)
#define PrevSucc(x)     EADJPREV(x, GD_SUCC)
#define EART(x)         ((x)->kantenart)
#define EKIND(x)        ((x)->kantenart)
#define EINVISIBLE(x)   ((x)->invisible)
#define EWEIGHTS(x)     ((x)->weights)
#define EWEIGHTP(x)     ((x)->weightp)



/*--------------------------------------------------------------------*/

/*  Adjacency lists
 *  ---------------
 *  Graphs are represented as adjacency list, see above.
 *  The cons cell of the list of edges used in adjacency list
 *  is the struct adjedge.
 */

typedef struct adjedge
{
    struct gedge    *kante;           /* Edge */
    struct adjedge  *next;
    struct adjedge  *internal_next;
} *ADJEDGE;

/* direct accesses */

#define AKANTE(x)   ((x)->kante)
#define ANEXT(x)    ((x)->next)
#define AINTERN(x)  ((x)->internal_next)

/* accesses to attributes of source and target of an edge */

#define SOURCE(x)   (ESTART(AKANTE(x)))
#define TARGET(x)   (EEND(AKANTE(x)))

/*--------------------------------------------------------------------*/

/*  Layout structs
 *  ==============
 *  To calculate the layout, the nodes are scheduled into layers.
 *  All nodes of one layer have the same vertical position, i.e.
 *  have the same deepth (level number) in the spanning tree of the graph.
 *  Inside the layers, the nodes are scheduled to minimize the crossings
 *  of the edges.
 */


/*  Depth entries
 *  -------------
 *  This is an entry in the table of existing layers. Each layer
 *  node contains the list of nodes of this layer, and the layout
 *  values. For the list of nodes, we use two lists to traverse
 *  the nodes forward and backward (similar to a double linked list).
 */

typedef struct depth_entry
{
    int anz;             /* Number of nodes of this layer */
    int actx;            /* actual  x coord.in this layer */
    int     cross;           /* number of crossings of layer  */
    GNODE   refnode;         /* Reference node for part 3     */
    struct  gnlist  *predlist;   /* nodes of this layer (forward) */
    struct  gnlist  *succlist;   /* nodes of this layer (backward)*/
    char    resort_necessary;    /* indicates if resorting by     */
                     /* barycentering etc. is needed  */
} DEPTH;

#define TANZ(x)     ((x).anz)
#define TPRED(x)    ((x).predlist)
#define TSUCC(x)    ((x).succlist)
#define TCROSS(x)   ((x).cross)
#define TACTX(x)    ((x).actx)
#define TREFN(x)    ((x).refnode)
#define TRESNEEDED(x)   ((x).resort_necessary)


/*--------------------------------------------------------------------*/

/*  Dllists
 *  -------
 *  To calculate the number of crossings, we use a plain sweep algorithm
 *  that uses two lists. These sets are implemented as double
 *  linked lists containing nodes.
 */

typedef struct dllist
{
    struct  gnode   *node;      /* the node              */
    int     dlinfo;     /* the node info     */
    int     dlx;        /* the info coordinate   */
    struct  dllist  *pred;          /* predecessor cons cell */
    struct  dllist  *succ;          /* successor   cons cell */
} *DLLIST;

#define DPRED(x)    ((x)->pred)
#define DSUCC(x)    ((x)->succ)
#define DNODE(x)    ((x)->node)
#define DNX(x)      ((x)->dlx)
#define DINFO(x)    ((x)->dlinfo)

/*--------------------------------------------------------------------*/


/* see alloc.c for more information */

/* Global Variables
 * ----------------
 */

extern GNODE nodelist;
extern GNODE nodelistend;
extern GNODE tmpnodelist;
extern GNODE graphlist;
extern GNODE graphlistend;
extern GNODE invis_nodes;
extern GNODE labellist;
extern GNODE labellistend;
extern GNODE dummylist;
extern GEDGE edgelist;
extern GEDGE edgelistend;
extern GEDGE tmpedgelist;
extern ADJEDGE near_edge_list;
extern ADJEDGE bent_near_edge_list;
extern ADJEDGE back_edge_list;

/*--------------------------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif /* _VCG_DEFS_H_ */
