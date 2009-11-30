#include "vr_graph.h"



VRNode::VRNode( VRGraph *graph, const char *title, int x, int y)
	: GrNode( graph)
	, title_( title), label_()
	, x_(x), y_(y)
	, width_(0), height_(0)
	, color_(WHITE), bcolor_(BLACK)
	, borderw_(2)
	, stretch_ (1), shrink_(1)
{
}

VRNode::~VRNode()
{
}

VREdge::VREdge()
	: GrEdge( NULL, NULL)
    , label_()
	, dots_(0)
	, linestyle_(0)
	, thickness_(2)
	, color_(BLACK)
	, arrowsize_(10)
	, arrowstyle_(0)
{
}

VREdge::~VREdge()
{
}


/////////////////////////////////////////////////////////////
#define NODE_LABEL_MARGIN 3

VRGraph::VRGraph()
{
}

VRGraph::~VRGraph()
{

}

void VRGraph::AddNode( DrawBuffer *draw_buffer, int x, int y, const char *title, const char *label)
{
	VRNode *p = new VRNode( this, title, x, y);
	if (label)
		p->label_ = label;
	int twidth, theight;
	draw_buffer->GetTextPixelSize( label, &twidth, &theight);
	p->width_ = twidth + 2*(p->borderw_ + NODE_LABEL_MARGIN);
	p->height_ = theight + 2*(p->borderw_ + NODE_LABEL_MARGIN);
	
} /* VRGraph::AddNode */

VRNode *VRGraph::AddSizedNode( int x, int y, int width, int height, const char *title, const char *label)
{
	VRNode *p = new VRNode( this, title, x, y);
	if (label)
		p->label_ = label;
	p->width_ = width;
	p->height_ = height;
	
	return p;
} /* VRGraph::AddSizedNode */

/**
 * Отрисовать заданную область в draw_buffer
 */
void VRGraph::Expose( DrawBuffer *draw_buffer, int x, int y, int width, int height)
{
	GrNode *n;
	for ( n = GrGetFirstNode();
		  n; 
		  n = n->GrGetNextNode() )
	{
		VRNode *node = static_cast<VRNode*>(n);
		assert( dynamic_cast<VRNode*>(n) );
		draw_buffer->SetLineWidth( node->borderw_);
		draw_buffer->DrawRectangle( node->x_, node->y_, node->width_, node->height_, false);
		draw_buffer->DrawText( node->x_ + node->borderw_ + NODE_LABEL_MARGIN, 
							   node->y_ + node->borderw_ + NODE_LABEL_MARGIN,
							   node->label_.c_str());
	}
} /* VRGraph::Expose */




extern "C" {
#include "vcg/alloc.h"
}

void VRGraph::LoadGDL()
{
	GNODE v;
	for ( v = nodelist; v; v = NNEXT(v) )
	{
		if (NWIDTH(v) == 0)
			continue;
		AddSizedNode( NX(v), NY(v), NWIDTH(v), NHEIGHT(v), NTITLE(v), NLABEL(v));
	}
} /* VRGraph::LoadGDL */
