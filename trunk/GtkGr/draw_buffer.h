#ifndef _DRAW_BUFFER_H_
#define _DRAW_BUFFER_H_

#include "stdafx.h"
#include <gtk/gtk.h>
//#include <glib.h>
//#include <glib-object.h>


enum Axis_t
{
	AXIS_X = 0,
	AXIS_Y,
	AXIS_LAST
};

/* forward declaration */
class VRGraph;
/**
 * Осуществляет хранение графического изображения графа (m_Pixmap)
 * и занимается взаимодействием с drawing_area (m_da).
 * В m_Pixmap хранится полное изображение, не только то, что видно в m_da.
 * Часть m_Pixmap'а, отображаемая в m_da, описывается ( m_VisibleArea)
 */
class DrawBuffer
{
private:
	/* ссылка на drawing_area */
	GtkWidget *m_da;

	/* надо помнить, что pixmap'ы хранятся на Xserver'е и объект GdkPixmap 
	 * в клиенте является "описателем" */
	GdkPixmap *m_Pixmap;
	
	/* имеют смысл только при ненулевом pixmap */
	gint m_VisibleAreaBase[AXIS_LAST];
	gint m_VisibleAreaDims[AXIS_LAST];
	gint m_PixmapDims[AXIS_LAST];
	
	VRGraph *m_VRGraph;
	gint m_VRGBase[AXIS_LAST]; /* позиция центра VRGraph'а (ноль vrg-координат) */
	/* gc для vrg-функций */
	GdkGC *m_GC;
	typedef int vrgint; /* "маркер" того, что координаты в терминах VRGraph */
public:
	DrawBuffer( GtkWidget *drawing_area, VRGraph *vr_graph);
	/* не предназначен для иcпользования в качестве базового класса */
	~DrawBuffer();

	void ConfigureDa();
	void ExposeDa( gint x, gint y, gint width, gint height);
	void ButtonPress( gint x, gint y);
	void ButtonPress2( gint x, gint y);

	void MoveVisibleArea( gint delta, Axis_t axis);

	void PKey();
	
	void GetTextPixelSize( const char *text, int *width_p, int *height_p);
	/* vrg-функции */
	void SetLineWidth( int line_width);
	void DrawLine( vrgint x, vrgint y, vrgint endx, vrgint endy);
	void DrawRectangle( vrgint x, vrgint y, vrgint width, vrgint height, bool filled);
	void DrawText( vrgint x, vrgint y, const char *text);
private:
	void InvalidateDa( const GdkRectangle *update_rect);
};



#endif /* _DRAW_BUFFER_H_ */
