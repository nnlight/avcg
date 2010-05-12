#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_


/**
 * ��������� ��� �����(����������)
 */
class Preferences
{
private:
	bool is_init; /*! ��� ��������, ��� ����� ���������������� */
	double m_ScalingCoef; /*! ����������� ��������������, > 1. */
	int m_MovePixels; /*! �� �������� �������� ���������� ������� ������ �� �������� */

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