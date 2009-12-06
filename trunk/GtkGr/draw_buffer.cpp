#include "draw_buffer.h"
#include "vr_graph.h"

/**
 * Base Original Color maps
 */
static const unsigned char origredmap[BASECMAPSIZE] = {
	  255, 0, 255, 0, 255, 255, 0, 85, 0, 128, 0, 128, 128,
	  0, 255, 170, 128, 255, 128, 255, 255, 128, 238, 64, 127,
	  240, 160, 154, 255, 255, 218, 0 
};
static const unsigned char origgreenmap[BASECMAPSIZE] = {
	  255, 0, 0, 255, 255, 0, 255, 85, 0, 0, 128, 128, 0,
	  128, 215, 170, 128, 128, 255, 255, 128, 255, 130, 224,
	  255, 230, 32, 205, 192, 165, 112, 0 
};
static const unsigned char origbluemap[BASECMAPSIZE] = {
	  255, 255, 0, 0, 0, 255, 255, 85, 128, 0, 0, 0, 128,
	  128, 0, 170, 255, 128, 128, 128, 255, 255, 238, 208, 212,
	  140, 240, 50, 203, 0, 214, 0 
};


void DrawBuffer::InitColormap()
{
	/* иначе надо освобождать уже выделенные цвета */
	assert( m_AllocedColors == 0 );
	for (int i=0; i < BASECMAPSIZE; i++)
	{
		 // вообще-то это поле заполняется и имеет смысл только после выделения
		m_Colormap[i].pixel = 0;
		
		m_Colormap[i].red = origredmap[i] << 8;
		m_Colormap[i].green = origgreenmap[i] << 8;
		m_Colormap[i].blue = origbluemap[i] << 8;
	}
	/* остальные пока пробъем черным */
	for (int i = BASECMAPSIZE; i < CMAPSIZE; i++)
	{
		m_Colormap[i].pixel = 0;
		m_Colormap[i].red = 0;
		m_Colormap[i].green = 0;
		m_Colormap[i].blue = 0;
	}
	m_InitedColors = BASECMAPSIZE;
} /* DrawBuffer::InitColormap */

void DrawBuffer::AllocColormap()
{
	assert( m_GC );
	/* иначе надо освобождать уже выделенные цвета */
	assert( m_AllocedColors == 0 );

	GdkColormap *gc_colormap = gdk_gc_get_colormap( m_GC);
	gboolean success[CMAPSIZE];
	gint not_alloced = gdk_colormap_alloc_colors( gc_colormap,
												  m_Colormap, m_InitedColors,
												  false, // writable
												  true, // best_match
												  success); // *success
	if (0 == not_alloced)
	{
		m_AllocedColors = m_InitedColors;
	} else
	{
		InternalError();
	}

} /* DrawBuffer::AllocColormap */

DrawBuffer::DrawBuffer( GtkWidget *drawing_area)
	: m_da( drawing_area)
	, m_Pixmap( NULL)
	, m_VRGraph( NULL)
	, m_Scaling ( 1.)
	, m_GC( NULL)
	, m_InitedColors( 0)
	, m_AllocedColors( 0)
	, m_BackgroundColor( WHITE)
	, m_CurrentColor( BLACK)
{
	/* захватим drawing area, на всякий случай... */
	g_object_ref( m_da);
	InitColormap();
} /* DrawBuffer::DrawBuffer */

DrawBuffer::~DrawBuffer()
{
	g_object_unref( m_da);
	if (m_Pixmap)
	{
		g_object_unref( m_Pixmap);
	}
	if ( m_GC)
	{
		GdkColormap *gc_colormap = gdk_gc_get_colormap( m_GC);
		gdk_colormap_free_colors( gc_colormap, m_Colormap, m_AllocedColors);
		m_AllocedColors = 0;
		g_object_unref( m_GC);
	}
} /* DrawBuffer::~DrawBuffer */

void DrawBuffer::InitializePixmapToBackgroundColor( GdkPixmap *pixmap, int width, int height)
{

#ifndef NDEBUG
	int actual_width, actual_height;
	gdk_drawable_get_size( pixmap, &actual_width, &actual_height);
	assert( width == actual_width );
	assert( height == actual_height);
#endif /* !NDEBUG */

	gdk_gc_set_foreground( m_GC, &m_Colormap[m_BackgroundColor]);
	gdk_draw_rectangle( pixmap,
	                    m_GC,//m_da->style->white_gc,
	                    TRUE,
	                    0, 0,
	                    width,
	                    height);
	gdk_gc_set_foreground( m_GC, &m_Colormap[m_CurrentColor]);
} /* DrawBuffer::InitializePixmapToBackgroundColor */

void DrawBuffer::SetVRGraphRef( VRGraph *vr_graph)
{
	m_VRGraph = vr_graph;
	if (m_Pixmap)
	{
		InitializePixmapToBackgroundColor( m_Pixmap,
										   m_PixmapDims[AXIS_X],
										   m_PixmapDims[AXIS_Y]);
		if ( m_VRGraph )
		{
			m_VRGraph->Expose( this);
		}
		InvalidateDa( NULL);
	}
} /* DrawBuffer::SetVRGraphRef */

void DrawBuffer::ConfigureDa()
{
	assert( m_da );
	if (m_Pixmap)
	{
		g_object_unref( m_Pixmap);
		m_Pixmap = NULL;
	}
	if ( m_GC)
	{
		GdkColormap *gc_colormap = gdk_gc_get_colormap( m_GC);
		gdk_colormap_free_colors( gc_colormap, m_Colormap, m_AllocedColors);
		m_AllocedColors = 0;
		g_object_unref( m_GC);
	}

	gint dims[AXIS_LAST];
	/* Pixmap будем держать примерно в 3 раза больше видимой области */
	dims[AXIS_X] = m_da->allocation.width * 3;
	dims[AXIS_Y] = m_da->allocation.height * 3;

	m_Pixmap = gdk_pixmap_new( m_da->window,		/* только для дефолтной глубины передаем */
                               dims[AXIS_X],
                               dims[AXIS_Y],
                               -1);

	m_GC = gdk_gc_new( /*m_da->window*/ m_Pixmap);
	AllocColormap();
	SetBackgroundColor( /*WHITE*/DARKYELLOW);
	SetCurrentColor( BLACK);

	m_VisibleAreaBase[AXIS_X] = dims[AXIS_X] / 3;
	m_VisibleAreaBase[AXIS_Y] = dims[AXIS_Y] / 3;
	m_VisibleAreaDims[AXIS_X] = m_da->allocation.width;
	m_VisibleAreaDims[AXIS_Y] = m_da->allocation.height;
	m_PixmapDims[AXIS_X] = dims[AXIS_X];
	m_PixmapDims[AXIS_Y] = dims[AXIS_Y];

	m_VRGBase[AXIS_X] = m_VisibleAreaBase[AXIS_X];
	m_VRGBase[AXIS_Y] = m_VisibleAreaBase[AXIS_Y];
  
	InitializePixmapToBackgroundColor( m_Pixmap,
									   dims[AXIS_X],
									   dims[AXIS_Y]);
	if ( m_VRGraph )
	{
		m_VRGraph->Expose( this);
	}
} /* DrawBuffer::ConfigureDa */

void DrawBuffer::ExposeDa( daint x, 
						   daint y, 
						   daint width, 
						   daint height)
{
	gdk_draw_drawable( m_da->window,
                       m_da->style->fg_gc[GTK_WIDGET_STATE (m_da)],
					   m_Pixmap,
                       /* Only copy the area that was exposed. */
                       m_VisibleAreaBase[AXIS_X] + x, m_VisibleAreaBase[AXIS_Y] + y,
                       x, y,
                       width, height);
} /* DrawBuffer::ExposeDa */

void DrawBuffer::Da2Vrg( daint da_x, daint da_y, vrgint &vrg_x, vrgint &vrg_y)
{
	int pm_x, pm_y;
	Da2Pm( da_x, da_y, pm_x, pm_y);
	Pm2Vrg( pm_x, pm_y, vrg_x, vrg_y);
} /* DrawBuffer::Da2Vrg */
void DrawBuffer::Vrg2Da( vrgint vrg_x, vrgint vrg_y, daint &da_x, daint &da_y)
{
	int pm_x, pm_y;
	Vrg2Pm( vrg_x, vrg_y, pm_x, pm_y);
	Pm2Da( pm_x, pm_y, da_x, da_y);
} /* DrawBuffer::Vrg2Da */
void DrawBuffer::Pm2Vrg( int pm_x, int pm_y, vrgint &vrg_x, vrgint &vrg_y)
{
	vrg_x = (pm_x - m_VRGBase[AXIS_X]) / m_Scaling;
	vrg_y = (pm_y - m_VRGBase[AXIS_Y]) / m_Scaling;
} /* DrawBuffer::Pm2Vrg */
void DrawBuffer::Vrg2Pm( vrgint vrg_x, vrgint vrg_y, int &pm_x, int &pm_y)
{
	pm_x = m_VRGBase[AXIS_X] + vrg_x * m_Scaling;
	pm_y = m_VRGBase[AXIS_Y] + vrg_y * m_Scaling;
} /* DrawBuffer::Vrg2Pm */
void DrawBuffer::Pm2Da( int pm_x, int pm_y, daint &da_x, daint &da_y)
{
	da_x = pm_x - m_VisibleAreaBase[AXIS_X];
	da_y = pm_y - m_VisibleAreaBase[AXIS_Y];
} /* DrawBuffer::Pm2Da */
void DrawBuffer::Da2Pm( daint da_x, daint da_y, int &pm_x, int &pm_y)
{
	pm_x = m_VisibleAreaBase[AXIS_X] + da_x;
	pm_y = m_VisibleAreaBase[AXIS_Y] + da_y;
} /* DrawBuffer::Da2Pm */

/* Draw a rectangle on the screen */
static void
draw_brush (GtkWidget *widget, /* da */
            gint    da_x,
            gint    da_y,
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

  update_rect.x = (da_x - 3);
  update_rect.y = (da_y - 3);
  /* Now invalidate the affected region of the drawing area. */
  gdk_window_invalidate_rect (widget->window,
                              &update_rect,
                              FALSE);
} /* draw_brush */

void DrawBuffer::ButtonPress( daint x, daint y)
{
	int pm_x, pm_y;
	Da2Pm( x,y, pm_x,pm_y);
	draw_brush( m_da, x,y, pm_x,pm_y, m_Pixmap);
}

void DrawBuffer::ButtonPress2( daint x, daint y)
{
	vrgint vrg_x, vrg_y;
	Da2Vrg( x,y, vrg_x,vrg_y);
	m_VRGraph->AddNode( this, 
						vrg_x, vrg_y, 
						"title", " label\nnext line");
	m_VRGraph->Expose( this);
	InvalidateDa( NULL);
}

void DrawBuffer::MoveVisibleArea( gint delta,
								  Axis_t axis)
{
	gint delt[AXIS_LAST];
	delt[AXIS_X] = 0;
	delt[AXIS_Y] = 0;
	delt[axis] = delta;

	MoveVisibleArea2d( delt);
	return;
} /* DrawBuffer::MoveVisibleArea */

void DrawBuffer::MoveVisibleArea2d( gint delta[AXIS_LAST])
{
	bool is_need_redraw_pixmap = false;

	/* если видимая область доходит до границы Pixmap'а, то мы перерисовываем Pixmap
	   так, чтобы видимая область стала посередине Pixmap'а */
	for ( int axis = 0; axis < AXIS_LAST; axis++ )
	{
		if ( m_VisibleAreaBase[axis] + delta[axis] < 0 )
		{
			is_need_redraw_pixmap = true;
		}
		if ( m_VisibleAreaBase[axis] + m_VisibleAreaDims[axis] + delta[axis] > m_PixmapDims[axis] )
		{
			is_need_redraw_pixmap = true;
		}
	}
	if (is_need_redraw_pixmap)
	{
		/* смещение нового положения Pixmap относительно старых координат Pixmap */
		int pixmap_shift[AXIS_LAST];
		/* смещать будем так, чтобы VisibleArea оказалась посередине Pixmap'а */
		pixmap_shift[AXIS_X] = m_VisibleAreaBase[AXIS_X] - m_PixmapDims[AXIS_X] / 3;
		pixmap_shift[AXIS_Y] = m_VisibleAreaBase[AXIS_Y] - m_PixmapDims[AXIS_Y] / 3;

		/* пересчитываем положение видимой области (размеры видимой области не меняются) */
		m_VisibleAreaBase[AXIS_X] -= pixmap_shift[AXIS_X];
		m_VisibleAreaBase[AXIS_Y] -= pixmap_shift[AXIS_Y];
		assert( m_VisibleAreaBase[AXIS_X] == m_PixmapDims[AXIS_X] / 3 );
		assert( m_VisibleAreaBase[AXIS_Y] == m_PixmapDims[AXIS_Y] / 3 );
		/* пересчитываем положение центра VRGraph'а */
		m_VRGBase[AXIS_X] -= pixmap_shift[AXIS_X];
		m_VRGBase[AXIS_Y] -= pixmap_shift[AXIS_Y];

		/* обновляем содержимое Pixmap'а (т.к. теперь там должна лежать изображения другой части графа) */  
		InitializePixmapToBackgroundColor( m_Pixmap,
										   m_PixmapDims[AXIS_X],
										   m_PixmapDims[AXIS_Y]);
		if ( m_VRGraph )
		{
			m_VRGraph->Expose( this);
		}
	}

	/* сдвигаем visible area :) */
	m_VisibleAreaBase[AXIS_X] += delta[AXIS_X];
	m_VisibleAreaBase[AXIS_Y] += delta[AXIS_Y];
	/* Now invalidate the affected region of the drawing area. */
	/* инвалидируем всю drawing_area (потом должно будет прийти expose_event) */
	InvalidateDa( NULL);
	return;
} /* DrawBuffer::MoveVisibleArea2d */

void DrawBuffer::ChangeScaling( double scaling_factor, daint x, daint y)
{
	assert( m_da );
	
	int pm_x, pm_y;
	Da2Pm( x, y, pm_x, pm_y);

	assert( m_VisibleAreaBase[AXIS_X] <= pm_x 
			&& pm_x < m_VisibleAreaBase[AXIS_X] + m_VisibleAreaDims[AXIS_X] );
	assert( m_VisibleAreaBase[AXIS_Y] <= pm_y
			&& pm_y < m_VisibleAreaBase[AXIS_Y] + m_VisibleAreaDims[AXIS_Y] );

	m_Scaling *= scaling_factor;
	/*
     * Еще подкручиваем VRGBase таким образом, чтобы с новым scaling 
	 * под pm_x pm_y оказалось та же часть графа, что была со старым
	 * (относительно (pm_x,pm_y) растягиваем VRGBase на scaling_factor)
	 */
	/* смещение VRGBase относительно pm_x, pm_y */
	int vrgbase_shift[AXIS_LAST];
	vrgbase_shift[AXIS_X] = m_VRGBase[AXIS_X] - pm_x;
	vrgbase_shift[AXIS_Y] = m_VRGBase[AXIS_Y] - pm_y;

	/* растягиваем */
	vrgbase_shift[AXIS_X] *= scaling_factor;
	vrgbase_shift[AXIS_Y] *= scaling_factor;

	m_VRGBase[AXIS_X] = pm_x + vrgbase_shift[AXIS_X];
	m_VRGBase[AXIS_Y] = pm_y + vrgbase_shift[AXIS_Y];

	/* обновляем содержимое Pixmap'а (т.к. теперь там должна лежать изображения другой части графа) */  
	InitializePixmapToBackgroundColor( m_Pixmap,
									   m_PixmapDims[AXIS_X],
									   m_PixmapDims[AXIS_Y]);
	if ( m_VRGraph )
	{
		m_VRGraph->Expose( this);
	}
	/* инвалидируем всю drawing_area (потом должно будет прийти expose_event) */
	InvalidateDa( NULL);
	return;
} /* DrawBuffer::ChangeScaling */


void DrawBuffer::PKey()
{
	printf( "dims %d %d\n", m_PixmapDims[AXIS_X], m_PixmapDims[AXIS_Y]);
	printf( "va_base %d %d\n", m_VisibleAreaBase[AXIS_X], m_VisibleAreaBase[AXIS_Y]);
	printf( "vrg_base %d %d\n", m_VRGBase[AXIS_X], m_VRGBase[AXIS_Y]);
	printf( "scaling %f\n", m_Scaling);
} /* DrawBuffer::PKey */



void DrawBuffer::InvalidateDa( const GdkRectangle *da_update_rect)
{
	GdkRectangle rect_var;
	if ( !da_update_rect )
	{
		/* если подали NULL, то инвалидируем всю drawing_area */
		assert( m_da->allocation.width == m_VisibleAreaDims[AXIS_X] );
		assert( m_da->allocation.height == m_VisibleAreaDims[AXIS_Y] );
		rect_var.x = 0;
		rect_var.y = 0;
		rect_var.width = m_VisibleAreaDims[AXIS_X];
		rect_var.height = m_VisibleAreaDims[AXIS_Y];

		da_update_rect = &rect_var;
	}
	/* данный метод работает в терминах координат drawing_area */
	/* (потом должно будет прийти expose_event)*/
	gdk_window_invalidate_rect( m_da->window,
								da_update_rect,
								FALSE);
} /* DrawBuffer::InvalidateDa */

void DrawBuffer::GetTextPixelSize( const char *text, int *width_p, int *height_p)
{
	PangoLayout *layout;
	layout = gtk_widget_create_pango_layout( m_da, text);
	pango_layout_get_pixel_size( layout, width_p, height_p);
	g_object_unref( layout);
}
void DrawBuffer::SetBackgroundColor( Color_t c)
{
	assert( c < m_AllocedColors );
	m_BackgroundColor = c;
	gdk_gc_set_background( m_GC, &m_Colormap[c]);
}
void DrawBuffer::SetCurrentColor( Color_t c)
{
	assert( c < m_AllocedColors );
	m_CurrentColor = c;
	gdk_gc_set_foreground( m_GC, &m_Colormap[c]);
}
void DrawBuffer::SetLineWidth( vrgint line_width, Linestyle_t lstyle)
{
	int pm_line_width = line_width * m_Scaling;
	GdkLineStyle pm_lstyle;
	switch (lstyle)
	{
	case LS_SOLID:  pm_lstyle = GDK_LINE_SOLID; break;
	case LS_DOTTED: pm_lstyle = GDK_LINE_ON_OFF_DASH; break;
	case LS_DASHED: pm_lstyle = GDK_LINE_DOUBLE_DASH; break;
	default: assert( !"unknown linestyle" );
	}
	gdk_gc_set_line_attributes( m_GC, pm_line_width, pm_lstyle, GDK_CAP_BUTT, GDK_JOIN_MITER);
}
void DrawBuffer::DrawLine( vrgint x, vrgint y, vrgint endx, vrgint endy)
{
	int pm_x, pm_y;
	int pm_endx, pm_endy;
	Vrg2Pm( x,y, pm_x, pm_y);
	Vrg2Pm( endx, endy, pm_endx, pm_endy);

	gdk_draw_line( m_Pixmap, m_GC, pm_x, pm_y, pm_endx, pm_endy);
}
void DrawBuffer::DrawRectangle( vrgint x, vrgint y, vrgint width, vrgint height, bool filled)
{
	gboolean pm_filled = filled ? TRUE : FALSE;
	int pm_x, pm_y;
	Vrg2Pm( x,y, pm_x, pm_y);
	int pm_width = width * m_Scaling;
	int pm_height = height * m_Scaling;

	gdk_draw_rectangle( m_Pixmap, m_GC, pm_filled, 
						pm_x, pm_y, 
						pm_width, pm_height);
}
void DrawBuffer::DrawTriangle( vrgint x1, vrgint y1, vrgint x2, vrgint y2, vrgint x3, vrgint y3, bool filled)
{
	gboolean pm_filled = filled ? TRUE : FALSE;
	GdkPoint pm_p[3];
	Vrg2Pm( x1, y1, pm_p[0].x, pm_p[0].y);
	Vrg2Pm( x2, y2, pm_p[1].x, pm_p[1].y);
	Vrg2Pm( x3, y3, pm_p[2].x, pm_p[2].y);

	gdk_draw_polygon( m_Pixmap, m_GC, pm_filled, 
					  pm_p, 3);
}
void DrawBuffer::DrawText( vrgint x, vrgint y, const char *text)
{
	int pm_x, pm_y;
	Vrg2Pm( x,y, pm_x, pm_y);
	/* 10 - это что-то типа размера текущего шрифта... */
	gchar *str = g_strdup_printf( "<span size=\"%d\">%s</span>",
								  (int)(PANGO_SCALE * 10 * m_Scaling),
								  text);
	PangoLayout *layout;
	layout = gtk_widget_create_pango_layout( m_da, 0);
	pango_layout_set_markup( layout, str, -1);
	g_free( str);
	/*pango_layout_get_pixel_size (layout, &w, &h);*/
	gdk_draw_layout( m_Pixmap, m_GC, pm_x, pm_y, layout);
	g_object_unref( layout);
}
