#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_


/**
 * Хранилище для опций(параметров)
 */
class Preferences
{
private:
	double m_ScalingCoef; /*! коэффициент рескалирования, > 1. */
	int m_MovePixels; /*! на скольлко пикселей сдвигается видимая облась по стрелкам */

public:
	Preferences(void);
	~Preferences(void);

	double GetScalingCoef() { return m_ScalingCoef; };
	int GetMovePixels() { return m_MovePixels; };

	void LoadFromFile();
	void SaveToFile();
};

extern Preferences *g_Preferences;


#endif /* _PREFERENCES_H_ */