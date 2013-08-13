#include "preferences.h"
#include "vcg/gdl_defs.h"

/**
 * Глобальный экземпляр, в котором хранятся текущие значения опций 
 */
Preferences *g_Preferences = NULL;


Preferences::Preferences(void)
	: m_ScalingCoef(1.15)
	, m_MovePixels(60)
	, m_SizeX(1100)
	, m_SizeY(850)
	, m_PrintEvents( false)
	, m_PrintActions( false)
{
} /* Preferences::Preferences */

Preferences::~Preferences(void)
{
} /* Preferences::~Preferences */
	
int Preferences::GetKeyFileInteger( GKeyFile *key_file, const gchar *group_name, const gchar *key,
									int default_val)
{
	GError *error = NULL;
	int res = g_key_file_get_integer( key_file, group_name, key, &error);
	if ( error != NULL )
	{
		g_error_free( error);
		res = default_val;
	}
	return res;
} /* Preferences::GetKeyFileInteger */

double Preferences::GetKeyFileDouble( GKeyFile *key_file, const gchar *group_name, const gchar *key,
									  double default_val)
{
	GError *error = NULL;
	double res = g_key_file_get_double( key_file, group_name, key, &error);
	if ( error != NULL )
	{
		g_error_free( error);
		res = default_val;
	}
	return res;
} /* Preferences::GetKeyFileDouble */

bool Preferences::GetKeyFileBool( GKeyFile *key_file, const gchar *group_name, const gchar *key,
								  bool default_val)
{
	GError *error = NULL;
	bool res = (bool)g_key_file_get_boolean( key_file, group_name, key, &error);
	if ( error != NULL )
	{
		g_error_free( error);
		res = default_val;
	}
	return res;
} /* Preferences::GetKeyFileDouble */

void Preferences::LoadFromFile()
{
	GKeyFile *key_file = g_key_file_new();
	gchar *full_path = NULL;
	GError *error = NULL;
	
	if( !g_key_file_load_from_data_dirs( key_file, "avcg.ini", &full_path,
										 (GKeyFileFlags)(G_KEY_FILE_KEEP_COMMENTS
										                 | G_KEY_FILE_KEEP_TRANSLATIONS),
										 &error) )
	{
		//g_message( "Preferences::LoadFromFile() failed: %s\n", error->message);
		g_error_free( error);
		g_free( full_path);
		g_key_file_free( key_file);
		return;
	}
	printf( "avcg.ini loaded from %s\n", full_path);
	g_free( full_path);

	m_ScalingCoef = GetKeyFileDouble( key_file, "General", "ScalingCoef", m_ScalingCoef);
	m_MovePixels = GetKeyFileInteger( key_file, "General", "MovePixels", m_MovePixels);
	m_SizeX = GetKeyFileInteger( key_file, "General", "SizeX", m_SizeX);
	m_SizeY = GetKeyFileInteger( key_file, "General", "SizeY", m_SizeY);
	//m_DefaultBgColorNum = GetKeyFileInteger( key_file, "General", "DefaultBgColorNum", m_DefaultBgColorNum);
	m_PrintEvents = GetKeyFileBool( key_file, "Debug", "PrintEvents", m_PrintEvents);
	m_PrintActions = GetKeyFileBool( key_file, "Debug", "PrintActions", m_PrintActions);


	g_key_file_free( key_file);
	return;
} /* Preferences::LoadFromFile */

void Preferences::SaveToFile()
{
	const gchar *full_path_dir = g_get_user_data_dir();
	//g_message("user_data_dir = %s\n", full_path_dir);
	GKeyFile *key_file = g_key_file_new();

	g_key_file_set_double( key_file, "General", "ScalingCoef", m_ScalingCoef);
	g_key_file_set_integer( key_file, "General", "MovePixels", m_MovePixels);
	g_key_file_set_integer( key_file, "General", "SizeX", m_SizeX);
	g_key_file_set_integer( key_file, "General", "SizeY", m_SizeY);
	//g_key_file_set_integer( key_file, "General", "DefaultBgColorNum", m_DefaultBgColorNum);
	g_key_file_set_boolean( key_file, "Debug", "PrintEvents", (gboolean)m_PrintEvents);
	g_key_file_set_boolean( key_file, "Debug", "PrintActions", (gboolean)m_PrintActions);

	gsize key_file_data_length = 0;
	const gchar *key_file_data = g_key_file_to_data( key_file, &key_file_data_length, NULL);
	gchar *full_path_filename = g_strdup_printf( "%s/avcg.ini", full_path_dir);
	g_message( "Saving preferences to %s ...\n", full_path_filename);
	g_file_set_contents( full_path_filename, key_file_data, key_file_data_length, NULL);
	g_free( (gpointer)key_file_data);
	g_free( full_path_filename);

	g_key_file_free( key_file);
	return;
} /* Preferences::SaveToFile */

