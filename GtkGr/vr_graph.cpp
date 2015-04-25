#include "vr_graph.h"
#include <math.h>
#include <iostream>
#include "preferences.h"

extern "C" GEDGE revert_edge(GEDGE edge);
extern "C" int manhatten_edges;
static bool g_old_fashion_edges_loading = false;



VRNode::VRNode( VRGraph *graph, const char *title, int x, int y)
	: GrNode( graph)
	, title_( title), label_()
	, x_(x), y_(y)
	, width_(0), height_(0)
	, color_(WHITE), textcolor_(BLACK), bcolor_(BLACK)
	, borderw_(2)
	, shape_(NS_BOX)
	, textmode_(TM_CENTER)
	, stretch_ (1), shrink_(1)
{
}

VRNode::~VRNode()
{
}

int VRNode::FindInfoNumBySubstring( const char *substring)
{
	for ( int i = 0; i < VRNODE_INFO_COUNT; i++ )
	{
		if ( infos_[i].find( substring) != string::npos )
			return i + 1;
	}
	return 0;
}

VREdge::VREdge( VRGraph *graph)
    : GrEdge( graph->GrGetDummyNode(), graph->GrGetDummyNode())
    //, label_()
    , dots_(0)
    , linestyle_(LS_SOLID)
    , thickness_(2)
    , color_(BLACK)
{
    x_.reserve(4);
    y_.reserve(4);
    arrowsize_[VRDIR_FORWARD] = 10;
    arrowstyle_[VRDIR_FORWARD] = AS_SOLID;
    arrowcolor_[VRDIR_FORWARD] = BLACK;
    arrowsize_[VRDIR_BACKWARD] = 0;
    arrowstyle_[VRDIR_BACKWARD] = AS_NONE;
    arrowcolor_[VRDIR_BACKWARD] = BLACK;
}

VREdge::VREdge( VRGraph *graph, GEDGE e)
    : GrEdge( graph->GrGetDummyNode(), graph->GrGetDummyNode())
    //, label_()
    , dots_(0)
    , linestyle_((Linestyle_t)ELSTYLE(e))
    , thickness_(ETHICKNESS(e))
    , color_((Color_t)ECOLOR(e))
{
    x_.reserve(4);
    y_.reserve(4);
    if (thickness_ == 0)
        thickness_ = 1;
    arrowsize_[VRDIR_FORWARD] = 0;
    arrowstyle_[VRDIR_FORWARD] = AS_NONE;
    arrowcolor_[VRDIR_FORWARD] = BLACK;
    arrowsize_[VRDIR_BACKWARD] = 0;
    arrowstyle_[VRDIR_BACKWARD] = AS_NONE;
    arrowcolor_[VRDIR_BACKWARD] = BLACK;
}

VREdge::~VREdge()
{
}

void VREdge::SetArrowAttrsFromVcgEdge( VRDir_t dir, GEDGE e)
{
    if ( dir == VRDIR_FORWARD )
    {
	    arrowsize_[dir] = EARROWSIZE(e);
	    arrowstyle_[dir] = (Arrowstyle_t)EARROWSTYLE(e);
	    arrowcolor_[dir] = (Color_t)EARROWCOL(e);
    } else
    {
	    arrowsize_[dir] = EARROWBSIZE(e);
	    arrowstyle_[dir] = (Arrowstyle_t)EARROWBSTYLE(e);
	    arrowcolor_[dir] = (Color_t)EARROWBCOL(e);
    }
}

void VREdge::AddDotsFromVcgEdge( GEDGE e)
{
    assert( dots_ >= 1 );
    assert( thickness_ == (ETHICKNESS(e) > 0 ? ETHICKNESS(e) : 1) );
	assert( color_ == (Color_t)ECOLOR(e) );
	assert( linestyle_ == (Linestyle_t)ELSTYLE(e) );

	int x1 = ESTARTX(e);
	int y1 = ESTARTY(e);
	int x2 = EENDX(e);
	int y2 = EENDY(e);
	int x3 = ETBENDX(e); // top bend, у начала
	int y3 = ETBENDY(e);
	int x4 = EBBENDX(e); // bottom bend, у конца
	int y4 = EBBENDY(e);

#if 0
    x_.push_back( x1);
    y_.push_back( y1);
    dots_++;
#else
    assert( x_[ dots_-1 ] == x1 );
    assert( y_[ dots_-1 ] == y1 );
#endif

	if ( !((y3==y1)&&(x3==x1)) ) 
	{
        x_.push_back( x3);
        y_.push_back( y3);
        dots_++;
	}
	if ( !((y4==y2)&&(x4==x2)) ) 
	{
        x_.push_back( x4);
        y_.push_back( y4);
        dots_++;
	}

    x_.push_back( x2);
    y_.push_back( y2);
    dots_++;
    return;
} /* VREdge::SetDotsFromVcgEdge */


/////////////////////////////////////////////////////////////

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
void VRGraph::DrawGraph( DrawBuffer *draw_buffer)
{
	for ( VRNode *node = GetFirstNode();
		  node; 
		  node = node->GetNextNode() )
	{
		DrawNode( draw_buffer, node);
		for ( VREdge *edge = node->GetFirstSucc();
			  edge;
			  edge = edge->GetNextSucc() )
		{
			DrawEdge( draw_buffer, edge);
		}
	}
	for ( VREdge *edge = GetDummyNodeFirstSucc();
		  edge;
		  edge = edge->GetNextSucc() )
	{
		DrawEdge( draw_buffer, edge);
	}
	/* лейблы дуг специально рисуются после дуг, ибо для лейблов в vcg строятся узлы маленького размера
	 * и дуги идут к границам этих узлов, образуя пересечениние с текстом лейбла */
	for ( std::list<EdgeLabel>::iterator iter = elabel_list_.begin();
		  iter != elabel_list_.end();
		  ++iter )
	{
		DrawEdgeLabel( draw_buffer, &(*iter));
	}
	for ( std::list<VRInfoBox>::iterator iter = ibox_list_.begin();
		  iter != ibox_list_.end();
		  ++iter )
	{
		DrawInfoBox( draw_buffer, &(*iter));
	}
} /* VRGraph::DrawGraph */

void VRGraph::HandleInfoBoxPress( DrawBuffer *draw_buffer, int x, int y, int info_num)
{
	for ( std::list<VRInfoBox>::reverse_iterator riter = ibox_list_.rbegin();
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
			DrawGraph( draw_buffer);
			draw_buffer->PublicInvalidateDa();
			return;
		}
	}
	for ( VRNode *node = GetFirstNode();
		  node;
		  node = node->GetNextNode() )
	{
		if ( node->x_ <= x  && x < node->x_ + node->width_
			 && node->y_ <= y  && y < node->y_ + node->height_
			 && !node->infos_[info_num-1].empty() )
		{
			VRInfoBox ibox;
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
			DrawGraph( draw_buffer);
			draw_buffer->PublicInvalidateDa();
			return;
		}
	}
	return;
} /* VRGraph::HandleInfoBoxPress */

void VRGraph::DrawNodeText( DrawBuffer *draw_buffer, VRNode *node)
{
	draw_buffer->SetCurrentColor( node->textcolor_);
	/*draw_buffer->DrawText( node->x_ + node->borderw_ + NODE_LABEL_MARGIN, DTP_MIN,
						   node->y_ + node->borderw_ + NODE_LABEL_MARGIN, DTP_MIN,
						   node->label_.c_str());*/
	int text_x, text_y;
	DrawTextPos_t pos_x, pos_y;
	int x = node->x_, y =  node->y_, w =  node->width_, h =  node->height_;
	int b = node->borderw_;
	
	switch (node->shape_)
	{
	case NS_BOX:
		switch (node->textmode_)
		{
		case TM_LEFT:   text_x = x + b; pos_x = DTP_MIN;      text_y = y + h/2; pos_y = DTP_CENTER; break;
		case TM_CENTER: text_x = x + w/2; pos_x = DTP_CENTER; text_y = y + h/2; pos_y = DTP_CENTER; break;
		case TM_RIGHT:  text_x = x + w - b; pos_x = DTP_MAX;  text_y = y + h/2; pos_y = DTP_CENTER; break;
		default: assert( !"unknown textmode" );
		}
		break;
	case NS_RHOMB:
		switch (node->textmode_)
		{
		case TM_LEFT:   text_x = x + w/4 + b; pos_x = DTP_MIN;   text_y = y + h/2; pos_y = DTP_CENTER; break;
		case TM_CENTER: text_x = x + w/2; pos_x = DTP_CENTER;    text_y = y + h/2; pos_y = DTP_CENTER; break;
		case TM_RIGHT:  text_x = x + 3*w/4 - b; pos_x = DTP_MAX; text_y = y + h/2; pos_y = DTP_CENTER; break;
		default: assert( !"unknown textmode" );
		}
		break;
	case NS_ELLIPSE:
		switch (node->textmode_)
		{
		case TM_LEFT:   text_x = x + 146*w/1000 + b; pos_x = DTP_MIN;     text_y = y + h/2; pos_y = DTP_CENTER; break;
		case TM_CENTER: text_x = x + w/2; pos_x = DTP_CENTER;             text_y = y + h/2; pos_y = DTP_CENTER; break;
		case TM_RIGHT:  text_x = x + w - 146*w/1000 - b; pos_x = DTP_MAX; text_y = y + h/2; pos_y = DTP_CENTER; break;
		default: assert( !"unknown textmode" );
		}
		break;
	case NS_TRIANGLE:
		switch (node->textmode_)
		{
		case TM_LEFT:   text_x = x + w/4 + b; pos_x = DTP_MIN;   text_y = y + 3*h/4; pos_y = DTP_CENTER; break;
		case TM_CENTER: text_x = x + w/2; pos_x = DTP_CENTER;    text_y = y + 3*h/4; pos_y = DTP_CENTER; break;
		case TM_RIGHT:  text_x = x + 3*w/4 - b; pos_x = DTP_MAX; text_y = y + 3*h/4; pos_y = DTP_CENTER; break;
		default: assert( !"unknown textmode" );
		}
		break;
	default: assert( !"unknown nodeshape" );
	}
	draw_buffer->DrawText( text_x, pos_x, text_y, pos_y, node->label_.c_str());
	return;
} /* VRGraph::DrawNodeText */

void VRGraph::DrawNode( DrawBuffer *draw_buffer, VRNode *node)
{
	draw_buffer->SetCurrentColor( node->color_);
	switch (node->shape_)
	{
	case NS_BOX:      draw_buffer->DrawRectangle( node->x_, node->y_, node->width_, node->height_, true); break;
	case NS_RHOMB:    draw_buffer->DrawRhomb( node->x_, node->y_, node->width_, node->height_, true); break;
	case NS_ELLIPSE:  draw_buffer->DrawEllipse( node->x_, node->y_, node->width_, node->height_, true); break;
	case NS_TRIANGLE: draw_buffer->DrawGdlTriang( node->x_, node->y_, node->width_, node->height_, true); break;
	default: assert( !"unknown nodeshape" );
	}
	draw_buffer->SetLineWidth( node->borderw_);
	draw_buffer->SetCurrentColor( node->bcolor_);
	switch (node->shape_)
	{
	case NS_BOX:      draw_buffer->DrawRectangle( node->x_, node->y_, node->width_, node->height_, false); break;
	case NS_RHOMB:    draw_buffer->DrawRhomb( node->x_, node->y_, node->width_, node->height_, false); break;
	case NS_ELLIPSE:  draw_buffer->DrawEllipse( node->x_, node->y_, node->width_, node->height_, false); break;
	case NS_TRIANGLE: draw_buffer->DrawGdlTriang( node->x_, node->y_, node->width_, node->height_, false); break;
	default: assert( !"unknown nodeshape" );
	}
	DrawNodeText( draw_buffer, node);
	return;
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
		int ix2 = around(x2 * edge->arrowsize_[dir] / len + edge->x_[nib_i]);
		int iy2 = around(y2 * edge->arrowsize_[dir] / len + edge->y_[nib_i]);
		/* поворачиваем на - 30 градусов и приводим длину */
		double x3 = cos(-M_PI/6) * d_x - sin(-M_PI/6) * d_y;
		double y3 = sin(-M_PI/6) * d_x + cos(-M_PI/6) * d_y;
		int ix3 = around(x3 * edge->arrowsize_[dir] / len + edge->x_[nib_i]);
		int iy3 = around(y3 * edge->arrowsize_[dir] / len + edge->y_[nib_i]);
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
	if (edge->linestyle_ == LS_UNVISIBLE)
	{
		return;
	}
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

void VRGraph::DrawInfoBox( DrawBuffer *draw_buffer, VRInfoBox *ibox)
{
	VRNode *node = ibox->node_;
	draw_buffer->SetCurrentColor( /*node->color_*/ WHITE);
	draw_buffer->DrawRectangle( ibox->x_, ibox->y_, ibox->width_, ibox->height_, true);
	draw_buffer->SetLineWidth( node->borderw_);
	draw_buffer->SetCurrentColor( /*node->bcolor_*/ BLACK);
	draw_buffer->DrawRectangle( ibox->x_, ibox->y_, ibox->width_, ibox->height_, false);
	draw_buffer->SetCurrentColor( /*node->textcolor_*/ BLACK);
	draw_buffer->DrawText( ibox->x_ + node->borderw_ + NODE_LABEL_MARGIN, DTP_MIN,
						   ibox->y_ + node->borderw_ + NODE_LABEL_MARGIN, DTP_MIN,
						   node->infos_[ibox->info_num_ - 1].c_str());
} /* VRGraph::DrawInfoBox */

void VRGraph::DrawEdgeLabel( DrawBuffer *draw_buffer, EdgeLabel *elabel)
{
	int twidth, theight;
	draw_buffer->GetTextPixelSize( elabel->label_.c_str(), &twidth, &theight);

	int x = elabel->x_ + (elabel->width_ - twidth) / 2 - EDGE_LABEL_MARGIN;
	int y = elabel->y_ + (elabel->height_ - theight) / 2 - EDGE_LABEL_MARGIN;
	int width = twidth + 2*EDGE_LABEL_MARGIN;
	int height = theight + 2*EDGE_LABEL_MARGIN;

	draw_buffer->SetCurrentColor( elabel->color_);
	draw_buffer->DrawRectangle( x, y, width, height, true);
	draw_buffer->SetCurrentColor( elabel->textcolor_);
	draw_buffer->DrawText( x + EDGE_LABEL_MARGIN, DTP_MIN,
						   y + EDGE_LABEL_MARGIN, DTP_MIN,
						   elabel->label_.c_str());
} /* VRGraph::DrawEdgeLabel */



void VRGraph::LoadVcgEdge( GEDGE e, bool ignore_back_arrow)
{
	VREdge *edge = new VREdge( this, e);
    edge->SetArrowAttrsFromVcgEdge( VRDIR_FORWARD, e);
	if ( !ignore_back_arrow ) {
        edge->SetArrowAttrsFromVcgEdge( VRDIR_BACKWARD, e);
	}

	int x1 = ESTARTX(e);
	int y1 = ESTARTY(e);
	int x2 = EENDX(e);
	int y2 = EENDY(e);
	int x3 = ETBENDX(e); // top bend, у начала
	int y3 = ETBENDY(e);
	int x4 = EBBENDX(e); // bottom bend, у конца
	int y4 = EBBENDY(e);

    edge->x_.push_back( x1);
    edge->y_.push_back( y1);
    edge->dots_++;

	if ( !((y3==y1)&&(x3==x1)) ) 
	{
        edge->x_.push_back( x3);
        edge->y_.push_back( y3);
		edge->dots_++;
	}
	if ( !((y4==y2)&&(x4==x2)) ) 
	{
        edge->x_.push_back( x4);
        edge->y_.push_back( y4);
		edge->dots_++;
	}

    edge->x_.push_back( x2);
    edge->y_.push_back( y2);
    edge->dots_++;
    return;
} /* VRGraph::LoadVcgEdge */

int VRGraph::GetVcgNodeAnchorsFirstY( GNODE v)
{
	int lines_num = calc_str_lines_num( NLABEL(v));
	int h = NHEIGHT(v);
	int y;

	y = NY(v) + (h - lines_num*16)/2 - 10;
	if (NSHAPE(v) == NS_TRIANGLE) {
		y = y + h/4;
	}
	return y;
} /* VRGraph::GetVcgNodeAnchorsFirstY */

int VRGraph::GetVcgNodeAnchorX( GNODE n, int y1, GEDGE conn_e)
{
    // см. draw_solidanchors
	int x1 = ESTARTX(conn_e);
	int x2 = EENDX(conn_e);
	int myypos = NY(n);
	int h = NHEIGHT(n);
	int w = NWIDTH(n);
	int d;
	int xx;

	switch (NSHAPE(n)) {
	case RHOMB:
		if (y1-myypos<h/2) d = ((h/2-y1+myypos)*w)/h;
		else		   d = ((y1-myypos-h/2)*w)/h; 
		if (x1<x2) xx = x1-d;
		else	   xx = x1+d;
		break;
	case TRIANGLE:
		d = ((h-y1+myypos)*w/2)/h;
		if (x1<x2) xx = x1-d+w/4;
		else	   xx = x1+d-w/4;
		break;
	case ELLIPSE:
		d = (w+1)/2 - gstoint( sqrt( 
					(double)(w*w)/4.0-(double)(w*w)/(double)(h*h)*
					((double)h/2.0-y1+myypos) 
					*((double)h/2.0-y1+myypos)));
		if (x1<x2) xx = x1-d;
		else	   xx = x1+d;
		break;
	default: xx = x1;
	}
	return xx;
} /* VRGraph::GetVcgNodeAnchorX */

void VRGraph::LoadVcgEdgesForVcgAnchorNode( GNODE v, Marker_t marker, Tempattr_t node_ta)
{
    CONNECT c = NCONNECT(v);
    GNODE   n = CTARGET(c);
    GEDGE   conn_e = CEDGE(c);
    int x2 = EENDX(conn_e);
    int ybase = GetVcgNodeAnchorsFirstY(n);
    GEDGE e;

	for ( e = FirstSucc(v); e; e = NextSucc(e) )
	{
		int y = ybase + -EANCHOR(e)*16;
		int x1 = GetVcgNodeAnchorX( n, y, conn_e);
		int xlast = ESTARTX(e);
		int ylast = ESTARTY(e);

		VREdge *edge = new VREdge( this, e);
        edge->SetArrowAttrsFromVcgEdge( VRDIR_BACKWARD, e);

        edge->x_.push_back( x1);
		edge->y_.push_back( y);
		if (manhatten_edges) {
			edge->x_.push_back( xlast);
			edge->y_.push_back( y);
		} else {
			edge->x_.push_back( x2);
			edge->y_.push_back( y);
		}
		edge->x_.push_back( xlast);
		edge->y_.push_back( ylast);
		edge->dots_ = 3;

        if (g_old_fashion_edges_loading) {
            LoadVcgEdge( e, true);
        } else {
            AddVcgEdgesChainToEdge( e, edge, marker, node_ta);
        }
	}
	// в оигинальной VCG (в gs_anchornode) еще входные дуги смотрелись...
} /* VRGraph::LoadVcgEdgesForVcgAnchorNode */

void VRGraph::LoadVcgEdgesForVcgNodeList( GNODE list)
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
    GNODE   v;
    GEDGE   e;
    CONNECT c;

    for ( v = list; v; v = NNEXT(v) )
	{
		if ( NANCHORNODE(v) )
		{
			LoadVcgEdgesForVcgAnchorNode( v, Marker_t(-1), Tempattr_t(-1));
			continue;
		}
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
		for ( e = FirstPred(v); e; e = NextPred(e) )
		{
			if ( NANCHORNODE(ESOURCE(e)) ) {
				continue;
			}
			LoadVcgEdge( e);
		}
	}
} /* VRGraph::LoadVcgEdgesForVcgNodeList */

void VRGraph::LinkConnectEdgesAndRevertRevertedForVcgNodeList( GNODE list, std::list<GEDGE> &rev_e)
{
    GNODE   v;
    GEDGE   e, nxt_e;
    CONNECT c;

    for ( v = list; v; v = NNEXT(v) )
    {
        c = NCONNECT(v);
        if (c) {
            if (forward_connection1(c)) {
                e = CEDGE(c);
                link_edge(e);
                assert(EKIND(e) != 'R');
            }
            if (forward_connection2(c)) {
                e = CEDGE2(c);
                link_edge(e);
                assert(EKIND(e) != 'R');
            }
        }
        for (e = FirstSucc(v); e; e = nxt_e)
        {
            nxt_e = NextSucc(e);
            if (EKIND(e) == 'R') {
                rev_e.push_back( e);
                revert_edge(e);
                std::swap( ESTARTX(e), EENDX(e));
                std::swap( ESTARTY(e), EENDY(e));
                std::swap( ETBENDX(e), EBBENDX(e));
                std::swap( ETBENDY(e), EBBENDY(e));
            }
        }
    }
} /* VRGraph::LinkConnectEdgesAndRevertRevertedForVcgNodeList */

void VRGraph::UnlinkConnectEdgesForVcgNodeList( GNODE list)
{
    GNODE   v;
    GEDGE   e;
    CONNECT c;

    for ( v = list; v; v = NNEXT(v) )
    {
        c = NCONNECT(v);
        if (c) {
            if (forward_connection1(c)) {
                e = CEDGE(c);
                unlink_edge(e);
            }
            if (forward_connection2(c)) {
                e = CEDGE2(c);
                unlink_edge(e);
            }
        }
    }
} /* VRGraph::UnlinkConnectEdgesForVcgNodeList */

void VRGraph::RevertBackVcgEdges(std::list<GEDGE> &rev_e)
{
    std::list<GEDGE>::iterator it;

    for ( it = rev_e.begin(); it != rev_e.end(); ++it )
    {
        GEDGE e = *it;
        revert_edge(e);
        std::swap( ESTARTX(e), EENDX(e));
        std::swap( ESTARTY(e), EENDY(e));
        std::swap( ETBENDX(e), EBBENDX(e));
        std::swap( ETBENDY(e), EBBENDY(e));
    }
} /* VRGraph::RevertBackVcgEdges */

void VRGraph::AddVcgEdgesChainToEdge( GEDGE e, VREdge *edge, Marker_t marker, Tempattr_t node_ta)
{
    GEDGE next_e;
    GNODE e_succ;

    e_succ = ETARGET(e);
    while ( !is_node_marked( e_succ, marker) )
    {
        edge->AddDotsFromVcgEdge( e);
        assert( NextPred(FirstPred(e_succ)) == NULL );
        assert( NextSucc(FirstSucc(e_succ)) == NULL );
        next_e = FirstSucc(e_succ);
        if ( EENDX(e) != ESTARTX(next_e)
             || EENDY(e) != ESTARTY(next_e) )
        {
            // Продолжающая vcg-дуга начинается не с того места, где закончилась предыдущая.
            // Значит соединительным узлом был label node. Делаем тут 2 VR-дуги.
            bool is_found = false;
            for (GNODE v = labellist; v; v = NNEXT(v))
            {
                if (e_succ == v) {
                    is_found = true;
                    break;
                }
            }
            assert(is_found);
            break;
        }
        e = next_e;
        e_succ = ETARGET(e);
    }
    if ( is_node_marked( e_succ, marker) )
    {
        edge->AddDotsFromVcgEdge( e);
        edge->SetArrowAttrsFromVcgEdge( VRDIR_FORWARD, e);
    } else
    {
        // продолжаем в новой VR-дуге
        VREdge *next_edge = new VREdge( this, next_e);
        next_edge->x_.push_back( ESTARTX(next_e));
        next_edge->y_.push_back( ESTARTY(next_e));
        next_edge->dots_++;

        AddVcgEdgesChainToEdge( next_e, next_edge, marker, node_ta);
    }
} /* VRGraph::AddVcgEdgesChainToEdge */

void VRGraph::LoadVcgEdges( Marker_t marker, Tempattr_t node_ta)
{
    GNODE   v;
    GEDGE   e;

    for ( v = nodelist; v; v = NNEXT(v) )
    {
        assert( is_node_marked( v, marker) );

        for ( e = FirstSucc(v); e; e = NextSucc(e) )
        {
            if ( NANCHORNODE(ETARGET(e)) )
            {
                LoadVcgEdgesForVcgAnchorNode( ETARGET(e), marker, node_ta);
                continue;
            }
            VREdge *edge = new VREdge( this, e);
            edge->SetArrowAttrsFromVcgEdge( VRDIR_BACKWARD, e);
            edge->x_.push_back( ESTARTX(e));
            edge->y_.push_back( ESTARTY(e));
            edge->dots_++;

            AddVcgEdgesChainToEdge( e, edge, marker, node_ta);
        }
    }
} /* VRGraph::LoadVcgEdges */

void VRGraph::LoadGDL()
{
    clock_t t = clock();
    GTimer *g = g_timer_new();
	/* узлы */
    Tempattr_t node_ta = new_temp_attr();
    Marker_t marker = new_marker();
	GNODE v;
	for ( v = nodelist; v; v = NNEXT(v) )
	{
        if (NWIDTH(v) == 0) {
            set_node_marker( v, marker);
            set_node_temp_attr( v, node_ta, NULL);
            continue;
        }
		VRNode *node = AddSizedNode( NX(v), NY(v), NWIDTH(v), NHEIGHT(v), NTITLE(v), NLABEL(v));
		node->color_ = (Color_t)NCOLOR(v);
		node->bcolor_ = (Color_t)NBCOLOR(v);
		node->borderw_ = NBORDERW(v);
		node->textcolor_ = (Color_t)NTCOLOR(v);
		node->shape_ = (Nodeshape_t)NSHAPE(v);
		node->textmode_ = (Textmode_t)NTEXTMODE(v);
		node->infos_[0] = NINFO1(v);
		node->infos_[1] = NINFO2(v);
		node->infos_[2] = NINFO3(v);

        set_node_marker( v, marker);
        set_node_temp_attr( v, node_ta, node);
	}
	/* лейблы дуг (узлы без границ) */
	for ( v = labellist; v; v = NNEXT(v) )
	{
		if (NWIDTH(v) == 0)
			continue;
		EdgeLabel elabel;
		elabel.x_ = NX(v);
		elabel.y_ = NY(v);
		elabel.width_ = NWIDTH(v);
		elabel.height_ = NHEIGHT(v);
		elabel.label_ = NLABEL(v);
		elabel.color_ = (Color_t)NCOLOR(v);
		elabel.textcolor_ = (Color_t)NTCOLOR(v);
		elabel_list_.push_back( elabel);
	}

	/* дуги */
    if ( g_old_fashion_edges_loading )
    {
	    LoadVcgEdgesForVcgNodeList( nodelist);
	    LoadVcgEdgesForVcgNodeList( labellist);
	    LoadVcgEdgesForVcgNodeList( dummylist);
    } else
    {
        std::list<GEDGE> rev_edges;
        LinkConnectEdgesAndRevertRevertedForVcgNodeList( nodelist, rev_edges);
        LinkConnectEdgesAndRevertRevertedForVcgNodeList( labellist, rev_edges);
        LinkConnectEdgesAndRevertRevertedForVcgNodeList( dummylist, rev_edges);
        LoadVcgEdges( marker, node_ta);
        UnlinkConnectEdgesForVcgNodeList( nodelist);
        UnlinkConnectEdgesForVcgNodeList( labellist);
        UnlinkConnectEdgesForVcgNodeList( dummylist);
        RevertBackVcgEdges( rev_edges);
    }
    free_marker( marker);
    free_temp_attr( node_ta);
    if (g_Preferences->DebugGetPrintTimes())
    {
        std::cout << "LoadGDL time " << double(clock() - t) / CLOCKS_PER_SEC << "sec"
                  <<" (" << g_timer_elapsed(g,NULL) << ")" << std::endl;
    }
    g_timer_destroy(g);
} /* VRGraph::LoadGDL */

void VRGraph::SetupDrawBufferSetting( DrawBuffer *draw_buffer)
{
	if (vcg_GetCmapSize())
	{
		/* если LoadGDL() еще не вызывали, то в vcg colormap еще не инициализирована,
		 * в этом случае будем пользоваться только базовыми цветами */
		draw_buffer->InitColormap( vcg_GetCmapSize(), vcg_GetCmapRed(), vcg_GetCmapGreen(), vcg_GetCmapBlue());
	}
	draw_buffer->SetBackgroundColor( vcg_GetBgColor());

} /* VRGraph::SetupDrawBufferSetting */

