#include "vr_graph.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif



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

VREdge::VREdge( VRGraph *graph)
	: GrEdge( graph->GrGetDummyNode(), graph->GrGetDummyNode())
    , label_()
	, dots_(0)
	, linestyle_(LS_SOLID)
	, thickness_(2)
	, color_(BLACK)
	, arrowsize_(10)
	, arrowstyle_(AS_SOLID)
	, arrowcolor_(BLACK)
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
	for ( GrNode *n = GrGetFirstNode();
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
		for ( GrEdge *e = n->GrGetFirstSucc();
			  e;
			  e = e->GrGetNextSucc() )
		{
			VREdge *edge = static_cast<VREdge*>(e);
			assert( dynamic_cast<VREdge*>(e) );
			DrawEdge( draw_buffer, edge);
		}
	}
	for ( GrEdge *e = GrGetDummyNode()->GrGetFirstSucc();
		  e;
		  e = e->GrGetNextSucc() )
	{
		VREdge *edge = static_cast<VREdge*>(e);
		assert( dynamic_cast<VREdge*>(e) );
		DrawEdge( draw_buffer, edge);
	}
} /* VRGraph::Expose */


void VRGraph::DrawEdge( DrawBuffer *draw_buffer, VREdge *edge)
{
	draw_buffer->SetLineWidth( edge->thickness_);
	for ( int i = 1; i < edge->dots_; i++ )
	{
		draw_buffer->DrawLine( edge->x_[i-1], edge->y_[i-1],
							   edge->x_[i], edge->y_[i]);
		if ( i == edge->dots_ - 1 )
		{
			/* рисуем стрелку, является равносторонним треугольником со стороной edge->arrowsize_ */
			double d_x = edge->x_[i-1] - edge->x_[i];
			double d_y = edge->y_[i-1] - edge->y_[i];
			double len = sqrt(d_x*d_x + d_y*d_y);
			/* поворачиваем на + 30 градусов и приводи длину */
			double x2 = cos(M_PI/6) * d_x - sin(M_PI/6) * d_y;
			double y2 = sin(M_PI/6) * d_x + cos(M_PI/6) * d_y;
			int ix2 = x2 * edge->arrowsize_ / len + edge->x_[i];
			int iy2 = y2 * edge->arrowsize_ / len + edge->y_[i];
			/* поворачиваем на - 30 градусов и приводим длину */
			double x3 = cos(-M_PI/6) * d_x - sin(-M_PI/6) * d_y;
			double y3 = sin(-M_PI/6) * d_x + cos(-M_PI/6) * d_y;
			int ix3 = x3 * edge->arrowsize_ / len + edge->x_[i];
			int iy3 = y3 * edge->arrowsize_ / len + edge->y_[i];
			/* рисуем */
			draw_buffer->DrawTriangle( edge->x_[i], edge->y_[i],
									   ix2, iy2,
									   ix3, iy3,
									   true);
		}
	}
} /* VRGraph::DrawEdge */



void VRGraph::LoadVcgEdge( GEDGE e)
{
	VREdge *edge = new VREdge( this);
	edge->thickness_ = ETHICKNESS(e);
	if (edge->thickness_ == 0)
		edge->thickness_ = 1;
	edge->color_ = (Color_t)ECOLOR(e);
	edge->linestyle_ = (Linestyle_t)ELSTYLE(e);
	edge->arrowstyle_ = (Arrowstyle_t)EARROWSTYLE(e);
	edge->arrowcolor_ = (Color_t)EARROWCOL(e);

	int x1 = ESTARTX(e);
	int y1 = ESTARTY(e);
	int x2 = EENDX(e);
	int y2 = EENDY(e);
	int x3 = ETBENDX(e); // top bend, у начала
	int y3 = ETBENDY(e);
	int x4 = EBBENDX(e); // bottom bend, у конца
	int y4 = EBBENDY(e);

	int i = edge->dots_;
	edge->x_[i] = x1;
	edge->y_[i] = y1;
	edge->dots_++;

	if ( !((y3==y1)&&(x3==x1)) ) 
	{
		i = edge->dots_;
		edge->x_[i] = x3;
		edge->y_[i] = y3;
		edge->dots_++;
	}
	if ( !((y4==y2)&&(x4==x2)) ) 
	{
		i = edge->dots_;
		edge->x_[i] = x4;
		edge->y_[i] = y4;
		edge->dots_++;
	}

	i = edge->dots_;
	edge->x_[i] = x2;
	edge->y_[i] = y2;
	edge->dots_++;
	assert( edge->dots_ < VREDGE_DOT_COUNT );
	return;
} /* VRGraph::LoadVcgEdge */



void VRGraph::LoadGDL()
{
	/* узлы */
	GNODE v;
	for ( v = nodelist; v; v = NNEXT(v) )
	{
		if (NWIDTH(v) == 0)
			continue;
		AddSizedNode( NX(v), NY(v), NWIDTH(v), NHEIGHT(v), NTITLE(v), NLABEL(v));
	}

	/* дуги */
/*  Draw all edges
 *  ==============
 *  We assume that edges have already a filled ESTARTX, ESTARTY, ETBENDX
 *  ETBENDY, EBBENDX, EBBENDYm EENDX, EENDY, EORI, EORI2 (if EART='D'), 
 *  ELSTYLE, ETHICKNESS, 
 *  ECOLOR, EARROWSIZE and EART.
 *  Note that these edges should not have ELABEL entries, because 
 *  edge labels should be converted before into label nodes.
 *
 *  We traverse all nodes and draw all edges pointing to these nodes.
 *  This seems to be faster than to inspect edgelist and tmpedgelist.
 *  This allows to correct EARROWSIZE at dummy nodes and label nodes.
 */

#define backward_connection1(c) ((CEDGE(c))&& (EEND(CEDGE(c)) ==v))
#define backward_connection2(c) ((CEDGE2(c))&&(EEND(CEDGE2(c))==v))

	GEDGE	e;
	ADJEDGE li;
	CONNECT c;
	
	for ( v = nodelist; v; v = NNEXT(v) )
	{
		c = NCONNECT(v);
		if (c) {
			if (backward_connection1(c)) {
				e = CEDGE(c);
				LoadVcgEdge( e);
			}
			if (backward_connection2(c)) {
				e = CEDGE2(c);
				LoadVcgEdge( e);
			}
		}
		li = NPRED(v);
		for ( li = NPRED(v); li; li = ANEXT(li) )
		{
			e  = AKANTE(li);
			LoadVcgEdge( e);
		}
	}
} /* VRGraph::LoadGDL */
