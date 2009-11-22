#include "draw_buffer.h"
#include "vr_graph.h"


DrawBuffer::DrawBuffer( GtkWidget *drawing_area, VRGraph *vr_graph)
	: m_da( drawing_area)
	, m_Pixmap( NULL)
	, m_VRGraph( vr_graph)
{
	g_object_ref( m_da);
	m_GC = NULL;
} /* DrawBuffer::DrawBuffer */

DrawBuffer::~DrawBuffer()
{
	if (m_Pixmap)
	{
		g_object_unref( m_Pixmap);
	}
	g_object_unref( m_da);
	if ( m_GC)
	{
		g_object_unref( m_GC);
	}
} /* DrawBuffer::~DrawBuffer */

void DrawBuffer::ConfigureDa()
{
	GtkWidget *drawing_area = m_da;
	if (m_Pixmap)
	{
		g_object_unref( m_Pixmap);
		m_Pixmap = NULL;
	}
	if (m_GC)
	{
		g_object_unref( m_GC);
		m_GC = NULL;
	}
	
	gint dims[AXIS_LAST];
	dims[AXIS_X] = drawing_area->allocation.width;
	dims[AXIS_Y] = drawing_area->allocation.height;

	m_Pixmap = gdk_pixmap_new( drawing_area->window,			/* только для дефолтной глубины передаем */
                               drawing_area->allocation.width,
                               drawing_area->allocation.height,
                               -1);

	m_GC = gdk_gc_new( m_Pixmap);
	GdkColor fg_color = {0,0,0,0};
	gdk_gc_set_rgb_fg_color( m_GC, &fg_color);
	GdkColor bg_color = {0,0,0,65000};
	gdk_gc_set_rgb_bg_color( m_GC, &bg_color);

	m_VisibleAreaBase[AXIS_X] = 0;
	m_VisibleAreaBase[AXIS_Y] = 0;
	m_VisibleAreaDims[AXIS_X] = dims[AXIS_X];
	m_VisibleAreaDims[AXIS_Y] = dims[AXIS_Y];
	m_PixmapDims[AXIS_X] = dims[AXIS_X];
	m_PixmapDims[AXIS_Y] = dims[AXIS_Y];

	m_VRGBase[AXIS_X] = 0;
	m_VRGBase[AXIS_Y] = 0;
  
	/* Initialize the pixmap to white */
  gdk_draw_rectangle( m_Pixmap,
                      drawing_area->style->white_gc,
                      TRUE,
                      0, 0,
                      drawing_area->allocation.width,
                      drawing_area->allocation.height);
} /* DrawBuffer::ConfigureDa */

void DrawBuffer::ExposeDa( gint x, 
						   gint y, 
						   gint width, 
						   gint height)
{
	gdk_draw_drawable( m_da->window,
                       m_da->style->fg_gc[GTK_WIDGET_STATE (m_da)],
					   m_Pixmap,
                       /* Only copy the area that was exposed. */
                       m_VisibleAreaBase[AXIS_X] + x, m_VisibleAreaBase[AXIS_Y] + y,
                       x, y,
                       width, height);
} /* DrawBuffer::ExposeDa */

/* Draw a rectangle on the screen */
static void
draw_brush (GtkWidget *widget, /* da */
            gint    x,
            gint    y,
            gint    pixmap_x,
            gint    pixmap_y,
			GdkPixmap *pixmap)
{
  GdkRectangle update_rect;

  update_rect.x = (pixmap_x - 3);
  update_rect.y = (pixmap_y - 3);
  update_rect.width = 6;
  update_rect.height = 6;

  /* Paint to the pixmap, where we store our state */
  gdk_draw_rectangle (pixmap,
                      widget->style->black_gc,
                      TRUE,
                      update_rect.x, update_rect.y,
                      update_rect.width, update_rect.height);

  update_rect.x = (x - 3);
  update_rect.y = (y - 3);
  /* Now invalidate the affected region of the drawing area. */
  gdk_window_invalidate_rect (widget->window,
                              &update_rect,
                              FALSE);
}

void DrawBuffer::ButtonPress( gint x, gint y)
{
	    draw_brush( m_da, x,y, m_VisibleAreaBase[AXIS_X] + x, m_VisibleAreaBase[AXIS_Y] + y, m_Pixmap);
}

void DrawBuffer::ButtonPress2( gint x, gint y)
{
		m_VRGraph->AddNode( this,
							x + m_VisibleAreaBase[AXIS_X] - m_VRGBase[AXIS_X], 
							y + m_VisibleAreaBase[AXIS_Y] - m_VRGBase[AXIS_Y], 
							"title", " label\nnext line");
		m_VRGraph->Expose( this, 0,0,0,0);
		InvalidateDa( NULL);
}

void DrawBuffer::MoveVisibleArea( gint delta,
								  Axis_t axis)
{
	bool is_need_new_pixmap = false;
	gint new_pixmap_dims[AXIS_LAST];
	gint old_pixmap_pose[AXIS_LAST] = {0,0}; /* положение старой относительно новой */
	new_pixmap_dims[AXIS_X] = m_PixmapDims[AXIS_X];
	new_pixmap_dims[AXIS_Y] = m_PixmapDims[AXIS_Y];

	if ( m_VisibleAreaBase[axis] + delta < 0 )
	{
		is_need_new_pixmap = true;
		new_pixmap_dims[axis] += -(m_VisibleAreaBase[axis] + delta);
		old_pixmap_pose[axis] = -(m_VisibleAreaBase[axis] + delta);
	}
	if ( m_VisibleAreaBase[axis] + m_VisibleAreaDims[axis] + delta > m_PixmapDims[axis] )
	{
		is_need_new_pixmap = true;
		new_pixmap_dims[axis] 
			+= m_VisibleAreaBase[axis] + m_VisibleAreaDims[axis] + delta - m_PixmapDims[axis];
	}

	if (is_need_new_pixmap)
	{
		GdkPixmap *new_pixmap = gdk_pixmap_new( m_da->window,
												new_pixmap_dims[AXIS_X], new_pixmap_dims[AXIS_X],
												-1);
		/* Initialize the new pixmap to white */
		gdk_draw_rectangle( new_pixmap,
							m_da->style->white_gc,
							TRUE,
							0, 0,
							new_pixmap_dims[AXIS_X], new_pixmap_dims[AXIS_X]);
		/* копируем содержимое старой */
		gdk_draw_drawable( new_pixmap,
						   m_da->style->fg_gc[GTK_WIDGET_STATE (m_da)],
					       m_Pixmap,
						   /* src x,y */
						   0, 0,
						   /* dst x,y */
                           old_pixmap_pose[AXIS_X], old_pixmap_pose[AXIS_Y],
						   m_PixmapDims[AXIS_X], m_PixmapDims[AXIS_Y]);
		/* подменяем */
		{
			g_object_unref( m_Pixmap);
			m_Pixmap = new_pixmap;
			m_PixmapDims[AXIS_X] = new_pixmap_dims[AXIS_X];
			m_PixmapDims[AXIS_Y] = new_pixmap_dims[AXIS_Y];
			m_VisibleAreaBase[AXIS_X] += old_pixmap_pose[AXIS_X];
			m_VisibleAreaBase[AXIS_Y] += old_pixmap_pose[AXIS_Y];
		}
		m_VRGBase[AXIS_X] += old_pixmap_pose[AXIS_X];
		m_VRGBase[AXIS_Y] += old_pixmap_pose[AXIS_Y];
	}

	/* в этой точке мы гарантируем, что в m_Pixmap хватит место чтобы сдвинуть VisibleArea */
	m_VisibleAreaBase[axis] += delta; /* сдвигаем visible area :) */
	/* Now invalidate the affected region of the drawing area. */
	/* инвалидируем всю drawing_area (должно будет потом expose_event прийти) */
	InvalidateDa( NULL);
	return;
} /* DrawBuffer::MoveVisibleArea */


void DrawBuffer::PKey()
{
	printf( "%d %d\n", m_VisibleAreaBase[AXIS_X], m_VisibleAreaBase[AXIS_Y]);
	printf( "%d %d\n", m_PixmapDims[AXIS_X], m_PixmapDims[AXIS_Y]);
} /* DrawBuffer::PKey */



void DrawBuffer::InvalidateDa( const GdkRectangle *update_rect)
{
	GdkRectangle rect_var;
	if ( !update_rect )
	{
		/* если подали NULL, то инвалидируем всю drawing_area */
		assert( m_da->allocation.width == m_VisibleAreaDims[AXIS_X] );
		assert( m_da->allocation.height == m_VisibleAreaDims[AXIS_Y] );
		rect_var.x = 0;
		rect_var.y = 0;
		rect_var.width = m_VisibleAreaDims[AXIS_X];
		rect_var.height = m_VisibleAreaDims[AXIS_Y];

		update_rect = &rect_var;
	}
	/* данный метод работает в терминах координат drawing_area */
	/* (потом должно будет прийти expose_event)*/
	gdk_window_invalidate_rect( m_da->window,
								update_rect,
								FALSE);
} /* DrawBuffer::InvalidateDa */

void DrawBuffer::GetTextPixelSize( const char *text, int *width_p, int *height_p)
{
	PangoLayout *layout;
	layout = gtk_widget_create_pango_layout( m_da, text);
	pango_layout_get_pixel_size( layout, width_p, height_p);
	g_object_unref( layout);
}
void DrawBuffer::SetLineWidth( int line_width)
{
	gdk_gc_set_line_attributes( m_GC, line_width, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
}
void DrawBuffer::DrawLine()
{
}
void DrawBuffer::DrawRectangle( vrgint x, vrgint y, int width, int height, bool filled)
{
	gboolean pm_filled = filled ? TRUE : FALSE;
	gint pm_x = m_VRGBase[AXIS_X] + x;
	gint pm_y = m_VRGBase[AXIS_Y] + y;

	gdk_draw_rectangle( m_Pixmap, m_GC, pm_filled, 
						pm_x, pm_y, 
						width, height);
}
void DrawBuffer::DrawText( vrgint x, vrgint y, const char *text)
{
	gint pm_x = m_VRGBase[AXIS_X] + x;
	gint pm_y = m_VRGBase[AXIS_Y] + y;
	PangoLayout *layout;
	layout = gtk_widget_create_pango_layout( m_da, text);
	/*pango_layout_get_pixel_size (layout, &w, &h);*/
	gdk_draw_layout( m_Pixmap, m_GC, pm_x, pm_y, layout);
	g_object_unref( layout);
}
