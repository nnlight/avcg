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
	
	/* ссылка на VRGraph */
	VRGraph *m_VRGraph;
	gint m_VRGBase[AXIS_LAST]; /* позиция центра VRGraph'а (ноль vrg-координат) */
	/* gc для vrg-функций */
	GdkGC *m_GC;

	typedef int vrgint; /* "маркер" того, что координаты в терминах VRGraph */
	typedef int daint;  /* "маркер" того, что координаты в терминах m_da */
public:
	DrawBuffer( GtkWidget *drawing_area, VRGraph *vr_graph);
	/* не предназначен для иcпользования в качестве базового класса */
	~DrawBuffer();

	/* da-функции */
	void ConfigureDa();
	void ExposeDa( daint x, daint y, daint width, daint height);
	void ButtonPress( daint x, daint y);
	void ButtonPress2( daint x, daint y);

	void MoveVisibleArea( gint delta, Axis_t axis);

	void PKey();
	
	void GetTextPixelSize( const char *text, int *width_p, int *height_p);
	/* vrg-функции */
	void SetLineWidth( int line_width);
	void DrawLine( vrgint x, vrgint y, vrgint endx, vrgint endy);
	void DrawRectangle( vrgint x, vrgint y, vrgint width, vrgint height, bool filled);
	void DrawTriangle( vrgint x1, vrgint y1, vrgint x2, vrgint y2, vrgint x3, vrgint y3, bool filled);
	void DrawText( vrgint x, vrgint y, const char *text);
private:
	void InvalidateDa( const GdkRectangle *da_update_rect);

	/**
	 * Методы по преобразованию координат
	 * Vrg - VRGraph
	 * Da - drawing_area
	 * Pm - Pixmap(т.е. DrawBuffer-координаты)
	 */
	void Da2Vrg( daint da_x, daint da_y, vrgint &vrg_x, vrgint &vrg_y);
	void Vrg2Da( vrgint vrg_x, vrgint vrg_y, daint &da_x, daint &da_y);
	void Pm2Vrg( int pm_x, int pm_y, vrgint &vrg_x, vrgint &vrg_y);
	void Vrg2Pm( vrgint vrg_x, vrgint vrg_y, int &pm_x, int &pm_y);
	void Pm2Da( int pm_x, int pm_y, daint &da_x, daint &da_y);
	void Da2Pm( daint da_x, daint da_y, int &pm_x, int &pm_y);
};



#endif /* _DRAW_BUFFER_H_ */
