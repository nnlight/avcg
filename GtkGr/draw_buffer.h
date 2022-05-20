#ifndef _DRAW_BUFFER_H_
#define _DRAW_BUFFER_H_

#include "stdafx.h"
#include <gtk/gtk.h>
#include "vcg/gdl_defs.h"



enum Axis_t
{
    AXIS_X = 0,
    AXIS_Y,
    AXIS_LAST
};

enum DrawTextPos_t
{
    DTP_MIN,    /*!< указывается минимальная координата */
    DTP_CENTER, /*!< указывается центр */
    DTP_MAX     /*!< указывается максимальная координата */
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

    /* текущий коэффициент увеличния */
    double m_Scaling;

    /* ссылка на VRGraph */
    VRGraph *m_VRGraph;
    gint m_VRGBase[AXIS_LAST]; /* позиция центра VRGraph'а (ноль vrg-координат) */

    /* признак того, что надо VRGraph перерисовывать (в expose_event'е) */
    bool m_IsNeedRedrawPixmap;

    typedef int vrgint; /* "маркер" того, что координаты в терминах VRGraph */
    typedef int daint;  /* "маркер" того, что координаты в терминах m_da */

    /* gc для рисования */
    GdkGC *m_GC;

    int m_InitedColors;              /*!< кол-во заполненных в m_Colormap цветов (rgb имеет смысл)*/
    GdkColor m_Colormap[CMAPSIZE];   /*!< таблица используемых цветов */
    int m_AllocedColors;             /*!< кол-во выделенных цветов (pixel field имеет смысл)*/
    Color_t m_BackgroundColor;       /*!< цвет фона */
    Color_t m_CurrentColor;          /*!< текущий (foreground) цвет */
    vector<GdkPoint> m_TmpPoints;    /*!< вспомогательный буффер для DrawLines */

    string highlight;
public:
    DrawBuffer( GtkWidget *drawing_area);
    /* не предназначен для иcпользования в качестве базового класса */
    ~DrawBuffer();

    /* установка отображаемого графа */
    void SetVRGraphRef( VRGraph *vr_graph);

    /* da-функции */
    void ConfigureDa();
    void ExposeDa( daint x, daint y, daint width, daint height);
    void ButtonPress( daint x, daint y);
    void ButtonPress2( daint x, daint y);
    void ConvertDa2Vrg( daint da_x, daint da_y, vrgint &vrg_x, vrgint &vrg_y)
    { Da2Vrg( da_x, da_y, vrg_x, vrg_y); };

    /* сдвинуть видимую область */
    void MoveVisibleArea( gint delta, Axis_t axis);
    void MoveVisibleArea2d( gint delta[AXIS_LAST]);
    void CenterVisibleAreaByVrgCoords( vrgint vrg_x, vrgint vrg_y);
    /** изменить Scaling, координаты передаются для того, чтобы та точка графа,
       которая находится под ними, осталась бы на том же месте */
    double ChangeScaling( double scaling_factor, daint x, daint y);
    double GetScaling() { return m_Scaling; };

    void PKey();

    /* vrg-функции */
    void InitColormap( int cmap_size, const uint8_t *cmap_red, const uint8_t *cmap_green, const uint8_t *cmap_blue);
    void SetBackgroundColor( Color_t c);
    void SetCurrentColor( Color_t c);
    void SetLineWidth( vrgint line_width, Linestyle_t lstyle = LS_SOLID);
    void DrawLine( vrgint x, vrgint y, vrgint endx, vrgint endy);
    void DrawLines( const vector<vrgint> &x, const vector<vrgint> &y, int n_points);
    void DrawRectangle( vrgint x, vrgint y, vrgint width, vrgint height, bool filled);
    void DrawTriangle( vrgint x1, vrgint y1, vrgint x2, vrgint y2, vrgint x3, vrgint y3, bool filled);
    void DrawRhomb( vrgint x, vrgint y, vrgint width, vrgint height, bool filled);
    void DrawEllipse( vrgint x, vrgint y, vrgint width, vrgint height, bool filled);
    void DrawGdlTriang( vrgint x, vrgint y, vrgint width, vrgint height, bool filled);
    void DrawPie( vrgint x, vrgint y, vrgint radius, bool filled, std::list<int> &colors);
    void GetTextPixelSize( const char *text, int *width_p, int *height_p);
    void DrawText( vrgint x, DrawTextPos_t x_pos, vrgint y, DrawTextPos_t y_pos, const char *text);

    void PublicInvalidateDa() { InvalidateDa( NULL); };
    void PublicFillByBgColor()
    { InitializePixmapToBackgroundColor( m_Pixmap, m_PixmapDims[AXIS_X], m_PixmapDims[AXIS_Y]); };

    void SetHighlight(string s) { highlight = s; m_IsNeedRedrawPixmap = true; InvalidateDa( NULL); }
    void ClearHighlight() { SetHighlight(""); }

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

    /** инициализация m_Colormap */
    void InitColormapBaseColors();
    /** выделение цветов из m_Colormap (заполняются pixel-поля) */
    void AllocColormap();
    /** освобождение цветов из m_Colormap */
    void FreeColormap();
    /** заполнение Pixmap'а цветом фона */
    void InitializePixmapToBackgroundColor( GdkPixmap *pixmap, int width, int height);
    void CreatePixmap( int width, int height);
    bool DeletePixmap();
    void InspectVisibleArea( bool force_pixmap_repos = false);

    static const int kPmMargin = 1000;  /*!< запас при подсчете пересечения с pixmap'ом */
    bool IsPointInPm( int pm_x, int pm_y)
    {
        return pm_x >= -kPmMargin && pm_x < m_PixmapDims[AXIS_X] + kPmMargin &&
               pm_y >= -kPmMargin && pm_y < m_PixmapDims[AXIS_Y] + kPmMargin;
    }
    bool IsBBoxIntersectsPm( int pm_min_x, int pm_min_y, int pm_max_x, int pm_max_y)
    {
        bool res = pm_max_x >= -kPmMargin && pm_min_x < m_PixmapDims[AXIS_X] + kPmMargin &&
                   pm_max_y >= -kPmMargin && pm_min_y < m_PixmapDims[AXIS_Y] + kPmMargin;
        if ( res == true )
        {
            /* в X-протоколе координаты передаются в 16-битных целых */
            static const int x11_max_val = (1 << 15) - 1;
            static const int x11_min_val = -(1 << 15);
            if ( pm_max_x > x11_max_val || pm_max_y > x11_max_val ||
                 pm_min_x < x11_min_val || pm_min_y < x11_min_val )
            {
                printf("bad situation: BBox intersects pm but too large for x11\n");
            }
        }
        return res;
    }
};



#endif /* _DRAW_BUFFER_H_ */
