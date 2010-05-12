#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_


/**
 * Хранилище для опций(параметров)
 */
class Preferences
{
private:
	bool is_init; /*! для проверок, что опции инициализированы */
	double m_ScalingCoef; /*! коэффициент рескалирования, > 1. */
	int m_MovePixels; /*! на скольлко пикселей сдвигается видимая облась по стрелкам */

private:
	static Preferences & GetPreferences();
	void InitDefaultValues();
public:
	Preferences(void);
	~Preferences(void);

	static double GetScalingCoef() { return GetPreferences().m_ScalingCoef; };
	static int GetMovePixels() { return GetPreferences().m_MovePixels; };

};


#endif /* _PREFERENCES_H_ */