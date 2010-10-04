#include "preferences.h"
#include "stdafx.h"
#include <glib.h>

/**
 * Глобальный экземпляр, в котором хранятся текущие значения опций 
 */
Preferences *g_Preferences = NULL;


Preferences::Preferences(void)
	: m_ScalingCoef(1.1)
	, m_MovePixels(50)
{
} /* Preferences::Preferences */

Preferences::~Preferences(void)
{
} /* Preferences::~Preferences */

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
		g_message( "Preferences::LoadFromFile() failed: %s\n", error->message);
		g_error_free( error);
		g_free( full_path);
		g_key_file_free( key_file);
		return;
	}
	printf( "avcg.ini loaded from %s\n", full_path);
	g_free( full_path);

	error = NULL;
	double scaling = g_key_file_get_double( key_file, "General", "ScalingCoef", &error);
	if ( error != NULL )
	{
		g_error_free( error);
	} else
	{
		m_ScalingCoef = scaling;
	}
	
	error = NULL;
	int move_pixels = g_key_file_get_integer( key_file, "General", "MovePixels", &error);
	if ( error != NULL )
	{
		g_error_free( error);
	} else
	{
		m_MovePixels = move_pixels;
	}

	g_key_file_free( key_file);
} /* Preferences::LoadFromFile */

void Preferences::SaveToFile()
{
	const gchar *full_path_dir = g_get_user_data_dir();
	g_message("user_data_dir = %s\n", full_path_dir);
	GKeyFile *key_file = g_key_file_new();

	g_key_file_set_double( key_file, "General", "ScalingCoef", m_ScalingCoef);
	g_key_file_set_integer( key_file, "General", "MovePixels", m_MovePixels);

	gsize key_file_data_length = 0;
	const gchar *key_file_data = g_key_file_to_data( key_file, &key_file_data_length, NULL);
	gchar *full_path_filename = g_strdup_printf( "%s/avcg.ini", full_path_dir);
	g_file_set_contents( full_path_filename, key_file_data, key_file_data_length, NULL);
	g_free( (gpointer)key_file_data);
	g_free( full_path_filename);

	g_key_file_free( key_file);
} /* Preferences::SaveToFile */

