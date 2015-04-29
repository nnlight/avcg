// GtkGr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <gtk/gtk.h>
#include "ui.h"
#include "vcg/vcg_iface.h"
#include "preferences.h"


#ifdef WIN32
#pragma comment(lib,"gthread-2.0.lib")
#pragma comment(lib,"gtk-win32-2.0.lib")
#pragma comment(lib,"glib-2.0.lib")
#pragma comment(lib,"gobject-2.0.lib")
#pragma comment(lib,"gdk-win32-2.0.lib")
#pragma comment(lib,"gdk_pixbuf-2.0.lib")
#pragma comment(lib,"pango-1.0.lib")
#pragma comment(lib,"pangowin32-1.0.lib")
#pragma comment(lib,"cairo.lib")
#endif




int main(int argc, char *argv[])
{
    char *filename = NULL;
    if (argc > 1)
    {
        filename = argv[1];
        vcg_ParseFile( filename);
    }

    // Иннициализация GTK;
    //gtk_init(&iDummy,NULL) ;
    gtk_init(&argc,/*(char***)*/&argv) ;

    // Загружаем настройки
    g_Preferences = new Preferences();
    g_Preferences->LoadFromFile();
    // строим GUI
    std::auto_ptr<UIController> uic( new UIController( filename));

    // запускаем цикл обработки сообщений
    uic->MainLoop();

    return 0;
} /* main */



