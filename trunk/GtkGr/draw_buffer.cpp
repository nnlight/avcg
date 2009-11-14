#include "draw_buffer.h"


DrawBuffer::DrawBuffer( GtkWidget *drawing_area)
	: m_da( drawing_area)
	, m_Pixmap( NULL)
{
	g_object_ref( m_da);
} /* DrawBuffer::DrawBuffer */

DrawBuffer::~DrawBuffer()
{
	if (m_Pixmap)
	{
		g_object_unref( m_Pixmap);
	}
	g_object_unref( m_da);
} /* DrawBuffer::~DrawBuffer */

void DrawBuffer::ConfigureDa()
{
	GtkWidget *drawing_area = m_da;
	if (m_Pixmap)
	{
		g_object_unref( m_Pixmap);
		m_Pixmap = NULL;
	}
	
	gint dims[AXIS_LAST];
	dims[AXIS_X] = drawing_area->allocation.width;
	dims[AXIS_Y] = drawing_area->allocation.height;

	m_Pixmap = gdk_pixmap_new( drawing_area->window,			/* ������ ��� ��������� ������� �������� */
                               drawing_area->allocation.width,
                               drawing_area->allocation.height,
                               -1);
	m_VisibleAreaBase[AXIS_X] = 0;
	m_VisibleAreaBase[AXIS_Y] = 0;
	m_VisibleAreaDims[AXIS_X] = dims[AXIS_X];
	m_VisibleAreaDims[AXIS_Y] = dims[AXIS_Y];
	m_PixmapDims[AXIS_X] = dims[AXIS_X];
	m_PixmapDims[AXIS_Y] = dims[AXIS_Y];
  
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

void DrawBuffer::MoveVisibleArea( gint delta,
								  Axis_t axis)
{
	bool is_need_new_pixmap = false;
	gint new_pixmap_dims[AXIS_LAST];
	gint old_pixmap_pose[AXIS_LAST] = {0,0}; /* ��������� ������ ������������ ����� */
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
		/* �������� ���������� ������ */
		gdk_draw_drawable( new_pixmap,
						   m_da->style->fg_gc[GTK_WIDGET_STATE (m_da)],
					       m_Pixmap,
						   /* src x,y */
						   0, 0,
						   /* dst x,y */
                           old_pixmap_pose[AXIS_X], old_pixmap_pose[AXIS_Y],
						   m_PixmapDims[AXIS_X], m_PixmapDims[AXIS_Y]);
		/* ��������� */
		{
			g_object_unref( m_Pixmap);
			m_Pixmap = new_pixmap;
			m_PixmapDims[AXIS_X] = new_pixmap_dims[AXIS_X];
			m_PixmapDims[AXIS_Y] = new_pixmap_dims[AXIS_Y];
			m_VisibleAreaBase[AXIS_X] += old_pixmap_pose[AXIS_X];
			m_VisibleAreaBase[AXIS_Y] += old_pixmap_pose[AXIS_Y];
		}
	}

	/* � ���� ����� �� �����������, ��� � m_Pixmap ������ ����� ����� �������� VisibleArea */
	m_VisibleAreaBase[axis] += delta; /* �������� visible area :) */
	/* Now invalidate the affected region of the drawing area. */
	/* ������������ ��� drawing_area (������ ����� ����� expose_event ������) */
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
		/* ���� ������ NULL, �� ������������ ��� drawing_area */
		assert( m_da->allocation.width == m_VisibleAreaDims[AXIS_X] );
		assert( m_da->allocation.height == m_VisibleAreaDims[AXIS_Y] );
		rect_var.x = 0;
		rect_var.y = 0;
		rect_var.width = m_VisibleAreaDims[AXIS_X];
		rect_var.height = m_VisibleAreaDims[AXIS_Y];

		update_rect = &rect_var;
	}
	/* ������ ����� �������� � �������� ��������� drawing_area */
	/* (����� ������ ����� ������ expose_event)*/
	gdk_window_invalidate_rect( m_da->window,
								update_rect,
								FALSE);
} /* DrawBuffer::InvalidateDa */

