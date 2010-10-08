#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#include <glib.h>


/**
 * Хранилище для опций(параметров)
 */
class Preferences
{
private:
	double m_ScalingCoef; /*! коэффициент рескалирования, > 1. */
	int m_MovePixels; /*! на скольлко пикселей сдвигается видимая облась по стрелкам */
	int m_SizeX; /*! ширина окна в пикселях */
	int m_SizeY; /*! высота окна в пикселях */

private:
	int GetKeyFileInteger( GKeyFile *key_file, const gchar *group_name, const gchar *key,
		                   int default_val);
	double GetKeyFileDouble( GKeyFile *key_file, const gchar *group_name, const gchar *key,
		                     double default_val);

public:
	Preferences(void);
	~Preferences(void);

	double GetScalingCoef() { return m_ScalingCoef; };
	int GetMovePixels() { return m_MovePixels; };
	int GetSizeX() { return m_SizeX; };
	int GetSizeY() { return m_SizeY; };

	void LoadFromFile();
	void SaveToFile();
};

extern Preferences *g_Preferences;


#endif /* _PREFERENCES_H_ */