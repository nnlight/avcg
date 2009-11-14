#ifndef _DRAW_BUFFER_H_
#define _DRAW_BUFFER_H_

#include "stdafx.h"

enum Axis_t
{
	AXIS_X = 0,
	AXIS_Y,
	AXIS_LAST
};

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
	
public:
	DrawBuffer( GtkWidget *drawing_area);
	/* не предназначен для иcпользования в качестве базового класса */
	~DrawBuffer();

	void ConfigureDa();
	void ExposeDa( gint x, gint y, gint width, gint height);
	void ButtonPress( gint x, gint y);

	void MoveVisibleArea( gint delta, Axis_t axis);

	void PKey();

private:
	void InvalidateDa( const GdkRectangle *update_rect);
};



#endif /* _DRAW_BUFFER_H_ */