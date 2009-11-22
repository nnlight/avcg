#include "vr_graph.h"



VRNode::VRNode( const char *title, int x, int y)
	: title_( title_), label_()
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

static void CalcTextBoundaries( const char *text, int *x_p, int *y_p)
{
	const char *ch_p = text;
	int x = 0, y = 0;
	int max_x = 0, max_y = 0;

	for ( ; *ch_p; ch_p++)
	{
		switch (*ch_p)
		{
		case '\n': /* Next line */
			x=0;
			y++;
			break;
		case '\t': /* Tabbing: 8 steps forward */
			x+=8;
			break;
		case '\r': /* Carriage return: ignore */
		case '\v': /* Vertical tabbing: ignore */
		case '\a': /* Beep in ANSI C: ignore */
			break;
		case '\b': /* Backspace */
			x--;
			break;
		case '\f': /* Form feed is misused for colors and other
			    * controlling styles. Ignore...
			    */
			break;
		default:
			x++;
			break;
		}
		if (x > max_x)
			max_x = x;
		if (y > max_y)
			max_y = y;
	}
	*x_p = max_x + 1;
	*y_p = max_y + 1;
	return;
}
#define NODE_LABEL_MARGIN 3
/////////////////////////////////////////////////////////////

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

void VRGraph::AddNode( int x, int y, const char *title, const char *label)
{
	VRNode *p = new VRNode( title, x, y);
	if (label)
		p->label_ = label;
	int twidth, theight;
	CalcTextBoundaries( label, &twidth, &theight);
	p->width_ = 8 * twidth + 2*(p->borderw_ + NODE_LABEL_MARGIN);
	p->height_ = 16 * theight + 2*(p->borderw_ + NODE_LABEL_MARGIN);
	
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
