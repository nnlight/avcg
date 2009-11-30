#include "gr.h"
#include <stdlib.h>



//////////////////////////// GrNode ///////////////////////////////////

GrNode::GrNode( GrGraph *graph)
	: graph_(graph)
{
	graph_->IncludeNodeInList( this);
	edges[GR_DIR_UP][GR_LIST_DIR_LEFT] = NULL;
	edges[GR_DIR_UP][GR_LIST_DIR_RIGHT] = NULL;
	edges[GR_DIR_DOWN][GR_LIST_DIR_LEFT] = NULL;
	edges[GR_DIR_DOWN][GR_LIST_DIR_RIGHT] = NULL;
	for (unsigned i = 0; i < GR_MARKERS_COUNT; i++ )
	{
		markers_val[i] = 0;
	}
}

GrNode::~GrNode(void)
{
	/* при удалении узла удал€ютс€ все инцидентные дуги */
	GrEdge *edge;
	GrEdge *next_edge;
	for ( edge = this->GrGetFirstSucc();
		  edge;
		  edge = next_edge )
	{
		next_edge = edge->GrGetNextSucc();
		graph_->DeleteEdge( edge);
	}
	for ( edge = this->GrGetFirstPred();
		  edge;
		  edge = next_edge )
	{
		next_edge = edge->GrGetNextPred();
		graph_->DeleteEdge( edge);
	}
	/* и узел удал€етс€ из списка узлов */
	graph_->ExcludeNodeFromList( this);
}

/**
 * ¬ключить дугу в список инцидентных (в направлении dir) дуг узла 
 */
void GrNode::IncludeEdgeInList( GrEdge *edge, GrDir_t dir)
{
	/* добавл€ем в конец списка дуг (в направлении dir)*/
	GrEdge *last = edges[dir][GR_LIST_DIR_RIGHT];
	if (last)
	{
		last->adjedge[dir][GR_LIST_DIR_RIGHT] = edge;
		edge->adjedge[dir][GR_LIST_DIR_LEFT] = last;
		edge->adjedge[dir][GR_LIST_DIR_RIGHT] = NULL;
		edges[dir][GR_LIST_DIR_RIGHT] = edge;
	} else
	{
		/* тогда и первый элемент списка должен быть нулевым, т.е. список пустой */
		assert( edges[dir][GR_LIST_DIR_LEFT] == 0 );
		edge->adjedge[dir][GR_LIST_DIR_LEFT] = NULL;
		edge->adjedge[dir][GR_LIST_DIR_RIGHT] = NULL;
		edges[dir][GR_LIST_DIR_LEFT] = edge;
		edges[dir][GR_LIST_DIR_RIGHT] = edge;
	}
}

/**
 * »сключить дугу из списока инцидентных (в направлении dir) дуг узла 
 */
void GrNode::ExcludeEdgeFromList( GrEdge *edge, GrDir_t dir)
{
	GrEdge *prev = edge->adjedge[dir][GR_LIST_DIR_LEFT];
	GrEdge *next = edge->adjedge[dir][GR_LIST_DIR_RIGHT];
	if (prev)
	{
		prev->adjedge[dir][GR_LIST_DIR_RIGHT] = next;
	} else
	{
		assert( edges[dir][GR_LIST_DIR_LEFT] == edge );
		edges[dir][GR_LIST_DIR_LEFT] = next;
	}
	if (next)
	{
		next->adjedge[dir][GR_LIST_DIR_LEFT] = prev;
	} else
	{
		assert( edges[dir][GR_LIST_DIR_RIGHT] == edge );
		edges[dir][GR_LIST_DIR_RIGHT] = prev;
	}
}

//////////////////////////// GrEdge ///////////////////////////////////

GrEdge::GrEdge( GrNode *src, GrNode *dst)
{
	node[GR_DIR_UP] = src;
	node[GR_DIR_DOWN] = dst;
	src->IncludeEdgeInList( this, GR_DIR_DOWN);
	dst->IncludeEdgeInList( this, GR_DIR_UP);
	for (unsigned i = 0; i < GR_MARKERS_COUNT; i++ )
	{
		markers_val[i] = 0;
	}
}

GrEdge::~GrEdge(void)
{
	node[GR_DIR_UP]->ExcludeEdgeFromList( this, GR_DIR_DOWN);
	node[GR_DIR_DOWN]->ExcludeEdgeFromList( this, GR_DIR_UP);
}

void GrEdge::GrChangeNode( GrDir_t dir, GrNode *new_node)
{
	GrNode *old_node = GrGetNode( dir);
	old_node->ExcludeEdgeFromList( this, GrReverseDir( dir));
	this->node[dir] = new_node;
	new_node->IncludeEdgeInList( this, GrReverseDir( dir));
}

//////////////////////////// GrEdge ///////////////////////////////////

GrGraph::GrGraph(void)
{
	nodes[GR_LIST_DIR_LEFT] = NULL;
	nodes[GR_LIST_DIR_RIGHT] = NULL;
	for (unsigned i = 0; i < GR_MARKERS_COUNT; i++ )
	{
		busy_markers[i] = false;
		markers_init_val[i] = 0;
	}
}

GrGraph::~GrGraph(void)
{
	/* при удалении графа удал€ютс€ все узлы графа */
	GrNode *node;
	GrNode *next_node;
	for ( node = this->GrGetFirstNode();
		  node;
		  node = next_node )
	{
		next_node = node->GrGetNextNode();
		this->DeleteNode( node);
	}
}

GrMarker_t GrGraph::AllocMarker()
{
	for (unsigned i = 0; i < GR_MARKERS_COUNT; i++ )
	{
		if ( busy_markers[i] )
			continue;
		/* нашли не зан€тый маркер */
		busy_markers[i] = true;
		assert( markers_init_val[i] < GR_MARKER_VALUE_MAX );
		GrMarkerValueType_t new_init_val = ++markers_init_val[i];
		return GR_MARKER( i, new_init_val);
	}
	assert( !"no more markers available" );
	return 0;
}

void GrGraph::FreeMarker( GrMarker_t marker)
{
	assert( busy_markers[ GR_MARKER_IDX( marker)] );
	busy_markers[ GR_MARKER_IDX( marker)] = false;
}

/**
 * ¬ключить узел в список узлов графа
 */
void GrGraph::IncludeNodeInList( GrNode *node)
{
	/* добавл€ем в конец списка узлов */
	GrNode *last = nodes[GR_LIST_DIR_RIGHT];
	if (last)
	{
		last->adjnode[GR_LIST_DIR_RIGHT] = node;
		node->adjnode[GR_LIST_DIR_LEFT] = last;
		node->adjnode[GR_LIST_DIR_RIGHT] = NULL;
		nodes[GR_LIST_DIR_RIGHT] = node;
	} else
	{
		/* тогда и первый элемент списка должен быть нулевым, т.е. список пустой */
		assert( nodes[GR_LIST_DIR_LEFT] == 0 );
		node->adjnode[GR_LIST_DIR_LEFT] = NULL;
		node->adjnode[GR_LIST_DIR_RIGHT] = NULL;
		nodes[GR_LIST_DIR_LEFT] = node;
		nodes[GR_LIST_DIR_RIGHT] = node;
	}
}

/**
 * »сключить узел из списка узлов графа
 */
void GrGraph::ExcludeNodeFromList( GrNode *node)
{
	GrNode *prev = node->adjnode[GR_LIST_DIR_LEFT];
	GrNode *next = node->adjnode[GR_LIST_DIR_RIGHT];
	if (prev)
	{
		prev->adjnode[GR_LIST_DIR_RIGHT] = next;
	} else
	{
		assert( nodes[GR_LIST_DIR_LEFT] == node );
		nodes[GR_LIST_DIR_LEFT] = next;
	}
	if (next)
	{
		next->adjnode[GR_LIST_DIR_LEFT] = prev;
	} else
	{
		assert( nodes[GR_LIST_DIR_RIGHT] == node );
		nodes[GR_LIST_DIR_RIGHT] = prev;
	}
}
