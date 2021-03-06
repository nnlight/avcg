#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#include "stdafx.h"
#include <glib.h>


/**
 * Хранилище для опций(параметров)
 */
class Preferences
{
private:
    /* [General] */
    double m_ScalingCoef; /*! коэффициент рескалирования, > 1. */
    int m_MovePixels; /*! на скольлко пикселей сдвигается видимая облась по стрелкам */
    int m_SizeX; /*! ширина окна в пикселях */
    int m_SizeY; /*! высота окна в пикселях */
    string m_FontFamily; /*! используемый класс шрифта: monospace, sans, serif, etc */
    /* [Debug] */
    bool m_PrintEvents;         /*! печатать ли приходящие события */
    bool m_PrintActions;        /*! печать действий */
    bool m_PrintTimes;          /*! печать некоторых времен */

private:
    int GetKeyFileInteger( GKeyFile *key_file, const gchar *group_name, const gchar *key,
                           int default_val);
    double GetKeyFileDouble( GKeyFile *key_file, const gchar *group_name, const gchar *key,
                             double default_val);
    bool GetKeyFileBool( GKeyFile *key_file, const gchar *group_name, const gchar *key,
                         bool default_val);
    string GetKeyFileString( GKeyFile *key_file, const gchar *group_name, const gchar *key,
                             string default_val);
public:
    Preferences(void);
    ~Preferences(void);

    double GetScalingCoef() { return m_ScalingCoef; };
    int GetMovePixels() { return m_MovePixels; };
    int GetSizeX() { return m_SizeX; };
    int GetSizeY() { return m_SizeY; };
    const string &GetFontFamily() { return m_FontFamily; };
    bool DebugGetPrintEvents() { return m_PrintEvents; };
    bool DebugGetPrintActions() { return m_PrintActions; };
    bool DebugGetPrintTimes() { return m_PrintTimes; };

    void LoadFromFile();
    void SaveToFile();
};

extern Preferences *g_Preferences;


#endif /* _PREFERENCES_H_ */

