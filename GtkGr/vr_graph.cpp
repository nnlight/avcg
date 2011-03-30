#include "vr_graph.h"
#include <math.h>



VRNode::VRNode( VRGraph *graph, const char *title, int x, int y)
	: GrNode( graph)
	, title_( title), label_()
	, x_(x), y_(y)
	, width_(0), height_(0)
	, color_(WHITE), textcolor_(BLACK), bcolor_(BLACK)
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
	/*, arrowsize_({10, 0})
	, arrowstyle_({AS_SOLID, AS_NONE})
	, arrowcolor_({BLACK, BLACK})*/
{
	arrowsize_[VRDIR_FORWARD] = 10;
	arrowstyle_[VRDIR_FORWARD] = AS_SOLID;
	arrowcolor_[VRDIR_FORWARD] = BLACK;
	arrowsize_[VRDIR_BACKWARD] = 0;
	arrowstyle_[VRDIR_BACKWARD] = AS_NONE;
	arrowcolor_[VRDIR_BACKWARD] = BLACK;
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
void VRGraph::Expose( DrawBuffer *draw_buffer)
{
	for ( GrNode *n = GrGetFirstNode();
		  n; 
		  n = n->GrGetNextNode() )
	{
		VRNode *node = static_cast<VRNode*>(n);
		assert( dynamic_cast<VRNode*>(n) );
		DrawNode( draw_buffer, node);
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
	for ( std::list<VRIBox>::iterator iter = ibox_list_.begin();
		  iter != ibox_list_.end();
		  ++iter )
	{
		DrawIBox( draw_buffer, &(*iter));
	}
} /* VRGraph::Expose */

void VRGraph::HandleInfoBoxPress( DrawBuffer *draw_buffer, int x, int y, int info_num)
{
	for ( std::list<VRIBox>::reverse_iterator riter = ibox_list_.rbegin();
		  riter != ibox_list_.rend();
		  ++riter )
	{
		if ( riter->x_ <= x  && x < riter->x_ + riter->width_
			 && riter->y_ <= y  && y < riter->y_ + riter->height_
			 /*&& riter->info_num_ == info_num*/)
		{
			void *r_addr = &(*riter);
			void *addr = &(*--riter.base());
			assert( r_addr == addr );
			ibox_list_.erase( --riter.base());
			draw_buffer->PublicFillByBgColor();
			Expose( draw_buffer);
			draw_buffer->PublicInvalidateDa();
			return;
		}
	}
	for ( GrNode *n = GrGetFirstNode();
		  n; 
		  n = n->GrGetNextNode() )
	{
		VRNode *node = static_cast<VRNode*>(n);
		assert( dynamic_cast<VRNode*>(n) );
		if ( node->x_ <= x  && x < node->x_ + node->width_
			 && node->y_ <= y  && y < node->y_ + node->height_
			 && !node->infos_[info_num-1].empty() )
		{
			VRIBox ibox;
			int twidth, theight;
			ibox.x_ = node->x_;
			ibox.y_ = node->y_;
			/* TODO: для инфобоксов наверно правила не такие как для узлов */
			draw_buffer->GetTextPixelSize( node->infos_[info_num-1].c_str(), &twidth, &theight);
			ibox.width_ = twidth + 2*(node->borderw_ + NODE_LABEL_MARGIN);
			ibox.height_ = theight + 2*(node->borderw_ + NODE_LABEL_MARGIN);
			ibox.node_ = node;
			ibox.info_num_ = info_num;
			ibox_list_.push_back( ibox);
			Expose( draw_buffer);
			draw_buffer->PublicInvalidateDa();
			return;
		}
	}
	return;
} /* VRGraph::HandleInfoBoxPress */

void VRGraph::DrawNode( DrawBuffer *draw_buffer, VRNode *node)
{
	draw_buffer->SetCurrentColor( node->color_);
	draw_buffer->DrawRectangle( node->x_, node->y_, node->width_, node->height_, true);
	draw_buffer->SetLineWidth( node->borderw_);
	draw_buffer->SetCurrentColor( node->bcolor_);
	draw_buffer->DrawRectangle( node->x_, node->y_, node->width_, node->height_, false);
	draw_buffer->SetCurrentColor( node->textcolor_);
	draw_buffer->DrawText( node->x_ + node->borderw_ + NODE_LABEL_MARGIN, 
						   node->y_ + node->borderw_ + NODE_LABEL_MARGIN,
						   node->label_.c_str());
} /* VRGraph::DrawNode */

void VRGraph::DrawEdgeArrow( DrawBuffer *draw_buffer, VREdge *edge, VRDir_t dir)
{
	int nib_i, foot_i;
	if ( dir == VRDIR_FORWARD )
	{
		nib_i = edge->dots_ - 1;
		foot_i = nib_i - 1;
		assert( foot_i >= 0);
	} else
	{
		nib_i = 0;
		foot_i = 1;
		assert( 1 < edge->dots_);
	}
	if ( edge->arrowstyle_[dir] != AS_NONE
		 && edge->arrowstyle_[dir] != AS_NONESPEC
		 && edge->arrowsize_[dir] > edge->thickness_ )
	{

		draw_buffer->SetLineWidth( 1);
		draw_buffer->SetCurrentColor( edge->arrowcolor_[dir]);
		/* рисуем стрелку, является равносторонним треугольником со стороной edge->arrowsize_ */
		double d_x = edge->x_[foot_i] - edge->x_[nib_i];
		double d_y = edge->y_[foot_i] - edge->y_[nib_i];
		double len = sqrt(d_x*d_x + d_y*d_y);
		/* поворачиваем на + 30 градусов и приводи длину */
		double x2 = cos(M_PI/6) * d_x - sin(M_PI/6) * d_y;
		double y2 = sin(M_PI/6) * d_x + cos(M_PI/6) * d_y;
		int ix2 = x2 * edge->arrowsize_[dir] / len + edge->x_[nib_i];
		int iy2 = y2 * edge->arrowsize_[dir] / len + edge->y_[nib_i];
		/* поворачиваем на - 30 градусов и приводим длину */
		double x3 = cos(-M_PI/6) * d_x - sin(-M_PI/6) * d_y;
		double y3 = sin(-M_PI/6) * d_x + cos(-M_PI/6) * d_y;
		int ix3 = x3 * edge->arrowsize_[dir] / len + edge->x_[nib_i];
		int iy3 = y3 * edge->arrowsize_[dir] / len + edge->y_[nib_i];
		/* рисуем */
		if ( edge->arrowstyle_[dir] == AS_SOLID )
		{
			draw_buffer->DrawTriangle( edge->x_[nib_i], edge->y_[nib_i],
					ix2, iy2,
					ix3, iy3,
					true);
		} else
		{
			assert( edge->arrowstyle_[dir] == AS_LINE );
			draw_buffer->DrawLine( edge->x_[nib_i], edge->y_[nib_i], ix2, iy2);
			draw_buffer->DrawLine( edge->x_[nib_i], edge->y_[nib_i], ix3, iy3);
		}
	}
} /* VRGraph::DrawEdgeArrow */

void VRGraph::DrawEdge( DrawBuffer *draw_buffer, VREdge *edge)
{
	draw_buffer->SetLineWidth( edge->thickness_, edge->linestyle_);
	draw_buffer->SetCurrentColor( edge->color_);
	for ( int i = 1; i < edge->dots_; i++ )
	{
		draw_buffer->DrawLine( edge->x_[i-1], edge->y_[i-1],
							   edge->x_[i], edge->y_[i]);
	}
	if ( edge->dots_ >= 2 )
	{
		DrawEdgeArrow( draw_buffer, edge, VRDIR_FORWARD);
		DrawEdgeArrow( draw_buffer, edge, VRDIR_BACKWARD);
	}
} /* VRGraph::DrawEdge */

void VRGraph::DrawIBox( DrawBuffer *draw_buffer, VRIBox *ibox)
{
	VRNode *node = ibox->node_;
	draw_buffer->SetCurrentColor( /*node->color_*/ WHITE);
	draw_buffer->DrawRectangle( ibox->x_, ibox->y_, ibox->width_, ibox->height_, true);
	draw_buffer->SetLineWidth( node->borderw_);
	draw_buffer->SetCurrentColor( /*node->bcolor_*/ BLACK);
	draw_buffer->DrawRectangle( ibox->x_, ibox->y_, ibox->width_, ibox->height_, false);
	draw_buffer->SetCurrentColor( /*node->textcolor_*/ BLACK);
	draw_buffer->DrawText( ibox->x_ + node->borderw_ + NODE_LABEL_MARGIN, 
						   ibox->y_ + node->borderw_ + NODE_LABEL_MARGIN,
						   node->infos_[ibox->info_num_ - 1].c_str());
} /* VRGraph::DrawIBox */



void VRGraph::LoadVcgEdge( GEDGE e)
{
	VREdge *edge = new VREdge( this);
	edge->thickness_ = ETHICKNESS(e);
	if (edge->thickness_ == 0)
		edge->thickness_ = 1;
	edge->color_ = (Color_t)ECOLOR(e);
	edge->linestyle_ = (Linestyle_t)ELSTYLE(e);
	edge->arrowstyle_[VRDIR_FORWARD] = (Arrowstyle_t)EARROWSTYLE(e);
	edge->arrowcolor_[VRDIR_FORWARD] = (Color_t)EARROWCOL(e);
	edge->arrowsize_[VRDIR_FORWARD] = EARROWSIZE(e);
	edge->arrowstyle_[VRDIR_BACKWARD] = (Arrowstyle_t)EARROWBSTYLE(e);
	edge->arrowcolor_[VRDIR_BACKWARD] = (Color_t)EARROWBCOL(e);
	edge->arrowsize_[VRDIR_BACKWARD] = EARROWBSIZE(e);

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
	assert( edge->dots_ <= VREDGE_DOT_COUNT );
	return;
} /* VRGraph::LoadVcgEdge */

void VRGraph::LoadVcgPredEdgesForVcgNodeList( GNODE list)
{
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

	GNODE	v;
	GEDGE	e;
	ADJEDGE li;
	CONNECT c;
	
	for ( v = list; v; v = NNEXT(v) )
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
}


void VRGraph::LoadGDL()
{
	/* узлы */
	GNODE v;
	for ( v = nodelist; v; v = NNEXT(v) )
	{
		if (NWIDTH(v) == 0)
			continue;
		VRNode *node = AddSizedNode( NX(v), NY(v), NWIDTH(v), NHEIGHT(v), NTITLE(v), NLABEL(v));
		node->color_ = (Color_t)NCOLOR(v);
		node->bcolor_ = (Color_t)NBCOLOR(v);
		node->borderw_ = NBORDERW(v);
		node->textcolor_ = (Color_t)NTCOLOR(v);
		node->infos_[0] = NINFO1(v);
		node->infos_[1] = NINFO2(v);
		node->infos_[2] = NINFO3(v);
	}

	/* дуги */
	LoadVcgPredEdgesForVcgNodeList( nodelist);
	LoadVcgPredEdgesForVcgNodeList( labellist);
	LoadVcgPredEdgesForVcgNodeList( dummylist);
} /* VRGraph::LoadGDL */
