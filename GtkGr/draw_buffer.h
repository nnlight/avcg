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
 * ������������ �������� ������������ ����������� ����� (m_Pixmap)
 * � ���������� ��������������� � drawing_area (m_da).
 * � m_Pixmap �������� ������ �����������, �� ������ ��, ��� ����� � m_da.
 * ����� m_Pixmap'�, ������������ � m_da, ����������� ( m_VisibleArea)
 */
class DrawBuffer
{
private:
	/* ������ �� drawing_area */
	GtkWidget *m_da;

	/* ���� �������, ��� pixmap'� �������� �� Xserver'� � ������ GdkPixmap 
	 * � ������� �������� "����������" */
	GdkPixmap *m_Pixmap;
	
	/* ����� ����� ������ ��� ��������� pixmap */
	gint m_VisibleAreaBase[AXIS_LAST];
	gint m_VisibleAreaDims[AXIS_LAST];
	gint m_PixmapDims[AXIS_LAST];
	
public:
	DrawBuffer( GtkWidget *drawing_area);
	/* �� ������������ ��� �c����������� � �������� �������� ������ */
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