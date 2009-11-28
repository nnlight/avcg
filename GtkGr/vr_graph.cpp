#include "vr_graph.h"



VRNode::VRNode( const char *title, int x, int y)
	: title_( title), label_()
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
	: label_()
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
	nodes_t::iterator it;
	for ( it = nodes_.begin(); it != nodes_.end(); ++it)
	{
		delete *it;
	}
}

void VRGraph::AddNode( DrawBuffer *draw_buffer, int x, int y, const char *title, const char *label)
{
	VRNode *p = new VRNode( title, x, y);
	if (label)
		p->label_ = label;
	int twidth, theight;
	draw_buffer->GetTextPixelSize( label, &twidth, &theight);
	p->width_ = twidth + 2*(p->borderw_ + NODE_LABEL_MARGIN);
	p->height_ = theight + 2*(p->borderw_ + NODE_LABEL_MARGIN);
	
	nodes_.push_back(p);
} /* VRGraph::AddNode */

/**
 * Отрисовать заданную область в draw_buffer
 */
void VRGraph::Expose( DrawBuffer *draw_buffer, int x, int y, int width, int height)
{
	nodes_t::iterator it;
	for ( it = nodes_.begin(); it != nodes_.end(); ++it)
	{
		VRNode *node = *it;
		draw_buffer->SetLineWidth( node->borderw_);
		draw_buffer->DrawRectangle( node->x_, node->y_, node->width_, node->height_, false);
		draw_buffer->DrawText( node->x_ + node->borderw_ + NODE_LABEL_MARGIN, 
							   node->y_ + node->borderw_ + NODE_LABEL_MARGIN,
							   node->label_.c_str());
	}
} /* VRGraph::Expose */
