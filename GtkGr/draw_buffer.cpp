#include "draw_buffer.h"
#include "vr_graph.h"
#include "preferences.h"


void DrawBuffer::InitColormapBaseColors()
{
	InitColormap( BASECMAPSIZE, gdl_OrigColormapRed, gdl_OrigColormapGreen, gdl_OrigColormapBlue);
} /* DrawBuffer::InitColormapBaseColors */

DrawBuffer::DrawBuffer( GtkWidget *drawing_area)
	: m_da( drawing_area)
	, m_Pixmap( NULL)
	, m_Scaling ( 1.)
	, m_VRGraph( NULL)
	, m_IsNeedRedrawPixmap( false)
	, m_GC( NULL)
	, m_InitedColors( 0)
	, m_AllocedColors( 0)
	, m_BackgroundColor( WHITE)
	, m_CurrentColor( BLACK)
{
	/* захватим drawing area, на всякий случай... */
	g_object_ref( m_da);
	InitColormapBaseColors();
} /* DrawBuffer::DrawBuffer */

DrawBuffer::~DrawBuffer()
{
	g_object_unref( m_da);
	DeletePixmap();
	return;
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
		m_IsNeedRedrawPixmap = true;
		InvalidateDa( NULL);
	}

	if (m_GC)
	{
		FreeColormap();
	}
	vr_graph->SetupDrawBufferSetting( this);
	if (m_GC)
	{
		AllocColormap();
	}

} /* DrawBuffer::SetVRGraphRef */

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

void DrawBuffer::FreeColormap()
{
	assert( m_GC );
	assert( m_AllocedColors > 0 );

	GdkColormap *gc_colormap = gdk_gc_get_colormap( m_GC);
	gdk_colormap_free_colors( gc_colormap, m_Colormap, m_AllocedColors);
	m_AllocedColors = 0;

} /* DrawBuffer::FreeColormap */

void DrawBuffer::CreatePixmap( int width, int height)
{
	assert( m_da );
	assert( m_Pixmap == NULL );

	m_Pixmap = gdk_pixmap_new( m_da->window,	/* передаем только для дефолтной глубины */
                               width,
                               height,
                               -1);

	m_GC = gdk_gc_new( /*m_da->window*/ m_Pixmap);
	AllocColormap();
	SetBackgroundColor( m_BackgroundColor);
	SetCurrentColor( m_CurrentColor);
} /* DrawBuffer::CreatePixmap */

bool DrawBuffer::DeletePixmap()
{
	bool res = false;
	if (m_Pixmap)
	{
		g_object_unref( m_Pixmap);
		m_Pixmap = NULL;
		res = true;
	}
	if (m_GC)
	{
		FreeColormap();
		g_object_unref( m_GC);
		m_GC = NULL;
	}
	return (res);
} /* DrawBuffer::DeletePixmap */

void DrawBuffer::ConfigureDa()
{
	assert( m_da );
	bool was_pixmap = DeletePixmap();

	gint new_va_dims[AXIS_LAST];
	gint dims[AXIS_LAST];

	new_va_dims[AXIS_X] = m_da->allocation.width;
	new_va_dims[AXIS_Y] = m_da->allocation.height;

	/* Pixmap будем держать примерно в 3 раза больше видимой области */
	dims[AXIS_X] = new_va_dims[AXIS_X] * 3;
	dims[AXIS_Y] = new_va_dims[AXIS_Y] * 3;

	CreatePixmap( dims[AXIS_X], dims[AXIS_Y]);

	m_PixmapDims[AXIS_X] = dims[AXIS_X];
	m_PixmapDims[AXIS_Y] = dims[AXIS_Y];
	if (was_pixmap)
	{
		/* если до этого Pixmap был, т.е. это resize окна, то расширяем (сужаем)
		   видимую область, оставляя центр на месте */
		m_VisibleAreaBase[AXIS_X] -= (new_va_dims[AXIS_X] - m_VisibleAreaDims[AXIS_X]) / 2;
		m_VisibleAreaBase[AXIS_Y] -= (new_va_dims[AXIS_Y] - m_VisibleAreaDims[AXIS_Y]) / 2;
		m_VisibleAreaDims[AXIS_X] = new_va_dims[AXIS_X];
		m_VisibleAreaDims[AXIS_Y] = new_va_dims[AXIS_Y];
	}
	else
	{
		m_VisibleAreaBase[AXIS_X] = 0;
		m_VisibleAreaBase[AXIS_Y] = 0;
		m_VisibleAreaDims[AXIS_X] = new_va_dims[AXIS_X];
		m_VisibleAreaDims[AXIS_Y] = new_va_dims[AXIS_Y];
		/* начальное положение центра координат VRGraph'а - левый верхний угол da */
		m_VRGBase[AXIS_X] = m_VisibleAreaBase[AXIS_X];
		m_VRGBase[AXIS_Y] = m_VisibleAreaBase[AXIS_Y];
	}
	/* требуем обязательного перепозиционирования(и перерисовки) Pixmap'а */
	InspectVisibleArea( true);
	return;
} /* DrawBuffer::ConfigureDa */

void DrawBuffer::ExposeDa( daint x, 
						   daint y, 
						   daint width, 
						   daint height)
{
	GTimer *timer = g_timer_new();
	if (m_IsNeedRedrawPixmap)
	{
		InitializePixmapToBackgroundColor( m_Pixmap,
										   m_PixmapDims[AXIS_X],
										   m_PixmapDims[AXIS_Y]);
		if ( m_VRGraph )
		{
			m_VRGraph->DrawGraph( this);
		}
		m_IsNeedRedrawPixmap = false;
	}
	double draw_time = g_timer_elapsed( timer, NULL);
	g_timer_start( timer);
	//if ( !gtk_events_pending() )
	{
	gdk_draw_drawable( m_da->window,
                       //m_da->style->fg_gc[GTK_WIDGET_STATE (m_da)],
                       m_da->style->black_gc,
					   m_Pixmap,
                       /* Only copy the area that was exposed. */
                       m_VisibleAreaBase[AXIS_X] + x, m_VisibleAreaBase[AXIS_Y] + y,
                       x, y,
                       width, height);
	}
	double copy_time = g_timer_elapsed( timer, NULL);
	if (g_Preferences->DebugGetPrintEvents())
	{
		g_print(" times:draw=%f  copy=%f\n", draw_time, copy_time);
	}
    g_timer_destroy( timer);
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
	//draw_brush( m_da, x,y, pm_x,pm_y, m_Pixmap);
}

void DrawBuffer::ButtonPress2( daint x, daint y)
{
	vrgint vrg_x, vrg_y;
	Da2Vrg( x,y, vrg_x,vrg_y);
	/*m_VRGraph->AddNode( this, 
						vrg_x, vrg_y, 
						"title", " label\nnext line");*/
	m_VRGraph->DrawGraph( this);
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
	/* просто сдвигаем visible area */
	m_VisibleAreaBase[AXIS_X] += delta[AXIS_X];
	m_VisibleAreaBase[AXIS_Y] += delta[AXIS_Y];

	/* проверим, не вышла ли видимая область за границы области */
	InspectVisibleArea();

	/* Now invalidate the affected region of the drawing area. */
	/* инвалидируем всю drawing_area (потом должно будет прийти expose_event) */
	InvalidateDa( NULL);
	return;
} /* DrawBuffer::MoveVisibleArea2d */

void DrawBuffer::CenterVisibleAreaByVrgCoords( vrgint vrg_x, vrgint vrg_y)
{
	daint da_x, da_y;
	Vrg2Da( vrg_x, vrg_y, da_x, da_y);
	daint da_center_x = m_VisibleAreaDims[AXIS_X] / 2;
	daint da_center_y = m_VisibleAreaDims[AXIS_Y] / 2;
	gint delta[AXIS_LAST];
	delta[AXIS_X] = da_x - da_center_x;
	delta[AXIS_Y] = da_y - da_center_y;
	MoveVisibleArea2d( delta);
	return;
} /* DrawBuffer::CenterVisibleAreaByVrgCoords */

double DrawBuffer::ChangeScaling( double scaling_factor, daint x, daint y)
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

	/* заказываем обновление содержимого Pixmap'а 
	   (т.к. теперь там должно лежать изображение другой части графа) */
	m_IsNeedRedrawPixmap = true;

	/* инвалидируем всю drawing_area (потом должно будет прийти expose_event) */
	InvalidateDa( NULL);
	return (m_Scaling);
} /* DrawBuffer::ChangeScaling */

void DrawBuffer::InspectVisibleArea( bool force_pixmap_repos)
{
	bool is_need_repos_pixmap = force_pixmap_repos;

	/* если видимая область выходит за границы Pixmap'а, то следует переместить Pixmap
	   так, чтобы видимая область стала посередине Pixmap'а */
	for ( int axis = 0; axis < AXIS_LAST; axis++ )
	{
		if ( m_VisibleAreaBase[axis] < 0 )
		{
			is_need_repos_pixmap = true;
		}
		if ( m_VisibleAreaBase[axis] + m_VisibleAreaDims[axis] > m_PixmapDims[axis] )
		{
			is_need_repos_pixmap = true;
		}
	}
	if (is_need_repos_pixmap)
	{
		/* смещение нового положения Pixmap относительно старых координат Pixmap */
		int pixmap_shift[AXIS_LAST];
		/* смещать будем так, чтобы VisibleArea оказалась посередине Pixmap'а */
		pixmap_shift[AXIS_X] = m_VisibleAreaBase[AXIS_X] -
								(m_PixmapDims[AXIS_X] - m_VisibleAreaDims[AXIS_X]) / 2;
		pixmap_shift[AXIS_Y] = m_VisibleAreaBase[AXIS_Y] -
								(m_PixmapDims[AXIS_Y] - m_VisibleAreaDims[AXIS_Y]) / 2;

		/* пересчитываем положение видимой области (размеры видимой области не меняются) */
		m_VisibleAreaBase[AXIS_X] -= pixmap_shift[AXIS_X];
		m_VisibleAreaBase[AXIS_Y] -= pixmap_shift[AXIS_Y];
		/* пересчитываем положение центра VRGraph'а */
		m_VRGBase[AXIS_X] -= pixmap_shift[AXIS_X];
		m_VRGBase[AXIS_Y] -= pixmap_shift[AXIS_Y];

		/* заказываем обновление содержимого Pixmap'а
		   (т.к. теперь там должна лежать изображения другой части графа) */
		m_IsNeedRedrawPixmap = true;
	}
	return;
} /* DrawBuffer::InspectVisibleArea */


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
	if ( da_update_rect )
	{
		rect_var = *da_update_rect;
	} else
	{
		/* если подали NULL, то инвалидируем всю drawing_area */
		assert( m_da->allocation.width == m_VisibleAreaDims[AXIS_X] );
		assert( m_da->allocation.height == m_VisibleAreaDims[AXIS_Y] );
		rect_var.x = 0;
		rect_var.y = 0;
		rect_var.width = m_VisibleAreaDims[AXIS_X];
		rect_var.height = m_VisibleAreaDims[AXIS_Y];
	}
	/* данный метод работает в терминах координат drawing_area */
	/* (потом должно будет прийти expose_event)*/
	gdk_window_invalidate_rect( m_da->window,
								&rect_var,
								FALSE);
} /* DrawBuffer::InvalidateDa */

void DrawBuffer::InitColormap( int cmap_size, const byte *cmap_red, const byte *cmap_green, const byte *cmap_blue)
{
	/* иначе надо освобождать уже выделенные цвета */
	assert( m_AllocedColors == 0 );
	
	assert( cmap_size <= CMAPSIZE );
	for (int i=0; i < cmap_size; i++)
	{
		 // вообще-то это поле заполняется и имеет смысл только после выделения
		m_Colormap[i].pixel = 0;
		
		m_Colormap[i].red = cmap_red[i] << 8;
		m_Colormap[i].green = cmap_green[i] << 8;
		m_Colormap[i].blue = cmap_blue[i] << 8;
	}
	/* остальные пробъем черным на всякий случай */
	for (int i = cmap_size; i < CMAPSIZE; i++)
	{
		m_Colormap[i].pixel = 0;
		m_Colormap[i].red = 0;
		m_Colormap[i].green = 0;
		m_Colormap[i].blue = 0;
	}
	m_InitedColors = cmap_size;
} /* DrawBuffer::InitColormap */

void DrawBuffer::SetBackgroundColor( Color_t c)
{
	assert( c < m_InitedColors );
	m_BackgroundColor = c;
	if ( m_GC && m_AllocedColors )
	{
		assert( c < m_AllocedColors );
		gdk_gc_set_background( m_GC, &m_Colormap[c]);
	}
}
void DrawBuffer::SetCurrentColor( Color_t c)
{
	assert( c < m_InitedColors );
	m_CurrentColor = c;
	if ( m_GC && m_AllocedColors )
	{
		assert( c < m_AllocedColors );
		gdk_gc_set_foreground( m_GC, &m_Colormap[c]);
	}
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
	gdk_gc_set_line_attributes( m_GC, pm_line_width, /*GDK_LINE_SOLID*/pm_lstyle, GDK_CAP_BUTT, GDK_JOIN_MITER);
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
void DrawBuffer::DrawRhomb( vrgint x, vrgint y, vrgint width, vrgint height, bool filled)
{
	gboolean pm_filled = filled ? TRUE : FALSE;
	GdkPoint pm_p[4];
	Vrg2Pm( x + width / 2, y,          pm_p[0].x, pm_p[0].y);
	Vrg2Pm( x + width, y + height /2,  pm_p[1].x, pm_p[1].y);
	Vrg2Pm( x + width / 2, y + height, pm_p[2].x, pm_p[2].y);
	Vrg2Pm( x, y + height / 2,         pm_p[3].x, pm_p[3].y);

	gdk_draw_polygon( m_Pixmap, m_GC, pm_filled, 
					  pm_p, 4);
}
void DrawBuffer::DrawEllipse( vrgint x, vrgint y, vrgint width, vrgint height, bool filled)
{
	gboolean pm_filled = filled ? TRUE : FALSE;
	int pm_x, pm_y;
	Vrg2Pm( x,y, pm_x, pm_y);
	int pm_width = width * m_Scaling;
	int pm_height = height * m_Scaling;

	gdk_draw_arc( m_Pixmap, m_GC, pm_filled, 
				  pm_x, pm_y, 
				  pm_width, pm_height,
				  0, 64 * 360);
}
void DrawBuffer::DrawGdlTriang( vrgint x, vrgint y, vrgint width, vrgint height, bool filled)
{
	DrawTriangle( x + width / 2, y,
				  x + width, y + height,
				  x, y + height,
				  filled);
}
void DrawBuffer::DrawPie( vrgint x, vrgint y, vrgint radius, bool filled, std::list<int> &colors)
{
	int pmx, pmy;
	Vrg2Pm( x, y, pmx, pmy);
	int pmradius = radius * m_Scaling;
	int pieces = colors.size();

	cairo_t *cr;
	/* get a cairo_t */
	cr = gdk_cairo_create( m_Pixmap/*m_da->window*/);

	if ( filled )
	{
		cairo_arc( cr, pmx, pmy, pmradius, 0, 2 * M_PI);
		cairo_set_source_rgb( cr, 0, 0, 0);
		cairo_fill( cr);
	} else
	{
		if ( pieces != 0 )
		{
			std::list<int>::iterator it;
			int i;
			double curr_angle;
			double delta_angle = 2 * M_PI / pieces;
			for ( it = colors.begin(), i = 0, curr_angle = 0.;
				  i < pieces;
				  ++it, ++i, curr_angle += delta_angle )
			{
				assert( it != colors.end() );
				assert( sizeof(m_Colormap[0].red) == 2 );
				double red = (double)m_Colormap[*it].red / 65535.;
				double green = (double)m_Colormap[*it].green / 65535.;
				double blue = (double)m_Colormap[*it].blue / 65535.;

				cairo_move_to( cr, pmx, pmy);
				cairo_arc( cr, pmx, pmy, pmradius, curr_angle, curr_angle + delta_angle);
				cairo_set_source_rgb( cr, red, green, blue);
				cairo_fill( cr);
			}
		} else
		{
			cairo_arc( cr, pmx, pmy, pmradius, 0, 2 * M_PI);
			cairo_set_source_rgb( cr, 1, 1, 1);
			cairo_fill( cr);
		}

		cairo_arc( cr, pmx, pmy, pmradius, 0, 2 * M_PI);
		cairo_set_source_rgb( cr, 0, 0, 0);
		cairo_stroke( cr);
	}

	/*cairo_arc( cr, pmx, pmy, pmradius, 0, 2 * M_PI);
	cairo_set_source_rgb( cr, 1, 1, 1);
    cairo_fill_preserve( cr);
	cairo_set_source_rgb( cr, 0, 0, 0);
	cairo_stroke( cr);*/

	cairo_destroy( cr);
}
void DrawBuffer::GetTextPixelSize( const char *text, int *width_p, int *height_p)
{
#if 0
	PangoLayout *layout;
	layout = gtk_widget_create_pango_layout( m_da, text);
	pango_layout_get_pixel_size( layout, width_p, height_p);
	g_object_unref( layout);
#else
	PangoLayout *layout;
	PangoFontDescription *font_description;
	font_description = pango_font_description_new();
	if ( !g_Preferences->GetFontFamily().empty() )
	{
		pango_font_description_set_family_static( font_description, g_Preferences->GetFontFamily().c_str());
	}

	layout = gtk_widget_create_pango_layout( m_da, text);
	pango_layout_set_font_description( layout, font_description);
	//pango_layout_set_text (layout, text, -1);
	pango_layout_get_pixel_size( layout, width_p, height_p);

	g_object_unref( layout);
	pango_font_description_free( font_description);

#endif
}
#if 0
void DrawBuffer::DrawText( vrgint x, vrgint y, const char *text)
{
	int pm_x, pm_y;
	Vrg2Pm( x,y, pm_x, pm_y);
	std::string text2( text);
	replace( text2, "&", "&amp;");
	replace( text2, "<", "&lt;");
	replace( text2, ">", "&gt;");
	/* 10 - это что-то типа размера текущего шрифта... */
	gchar *str = g_strdup_printf( "<span size=\"%d\">%s</span>",
								  (int)(PANGO_SCALE * 10 * m_Scaling),
								  text2.c_str());
	PangoLayout *layout;
	layout = gtk_widget_create_pango_layout( m_da, 0);
	pango_layout_set_markup( layout, str, -1);
	g_free( str);
	/*pango_layout_get_pixel_size (layout, &w, &h);*/
	gdk_draw_layout( m_Pixmap, m_GC, pm_x, pm_y, layout);
	g_object_unref( layout);
}
#else
void DrawBuffer::DrawText( vrgint x, DrawTextPos_t x_pos, vrgint y, DrawTextPos_t y_pos, const char *text)
{
    if ( 16 * m_Scaling < 1. )
    {
        // 16 - высота шрифта в vrg-пикселях
        return;
    }

    int pm_x, pm_y;
	Vrg2Pm( x,y, pm_x, pm_y);

	PangoLayout *layout;
	PangoFontDescription *font_description;

	font_description = pango_font_description_new();
	if ( !g_Preferences->GetFontFamily().empty() )
	{
		//pango_font_description_set_family( font_description, g_Preferences->GetFontFamily().c_str());
		pango_font_description_set_family_static( font_description, g_Preferences->GetFontFamily().c_str());
	}
	//pango_font_description_set_weight( font_description, PANGO_WEIGHT_NORMAL);
	pango_font_description_set_absolute_size( font_description, 13 * PANGO_SCALE  * m_Scaling);

	layout = gtk_widget_create_pango_layout( m_da, text);
	pango_layout_set_font_description( layout, font_description);
	
	//pango_layout_set_text( layout, text, -1);

	//PangoAttrList *attr_list = pango_attr_list_new();
	//pango_attr_list_insert( attr_list, pango_attr_scale_new( m_Scaling));
	//pango_attr_list_insert( attr_list, pango_attr_family_new( "monospace"));
	//pango_layout_set_attributes( layout, attr_list);

	int pm_width, pm_height;
	pango_layout_get_pixel_size( layout, &pm_width, &pm_height);
	switch (x_pos)
	{
	case DTP_CENTER: pm_x = pm_x - pm_width / 2; break;
	case DTP_MAX: pm_x = pm_x - pm_width; break;
	default: break;
	}
	switch (y_pos)
	{
	case DTP_CENTER: pm_y = pm_y - pm_height / 2; break;
	case DTP_MAX: pm_y = pm_y - pm_height; break;
	default: break;
	}
	gdk_draw_layout( m_Pixmap, m_GC, pm_x, pm_y, layout);

	//pango_attr_list_unref( attr_list);
	g_object_unref( layout);
	pango_font_description_free( font_description);
} /* DrawBuffer::DrawText */
#endif
