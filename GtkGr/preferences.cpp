#include "preferences.h"
#include <assert.h>

/**
 * Глобальный экземпляр, в котором хранятся текущие значения опций 
 */
static  Preferences g_preferences;

Preferences & Preferences::GetPreferences()
{ 
	assert( g_preferences.is_init );
	return g_preferences;
} /* Preferences::GetPreferences */

void Preferences::InitDefaultValues()
{
	this->is_init = true;
	m_ScalingCoef = 1.1;
	m_MovePixels = 20;
} /* Preferences::InitDefaultValues */

Preferences::Preferences(void)
	: is_init(false)
{
	InitDefaultValues();
} /* Preferences::Preferences */

Preferences::~Preferences(void)
{
} /* Preferences::~Preferences */
