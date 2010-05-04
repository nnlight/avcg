// GtkGr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <gtk/gtk.h>
#include "ui.h"
#include "vcg/vcg_iface.h"


#ifdef WIN32
#pragma comment(lib,"gthread-2.0.lib")
#pragma comment(lib,"gtk-win32-2.0.lib")
#pragma comment(lib,"glib-2.0.lib")
#pragma comment(lib,"gobject-2.0.lib")
#pragma comment(lib,"gdk-win32-2.0.lib")
#pragma comment(lib,"gdk_pixbuf-2.0.lib")
#pragma comment(lib,"pango-1.0.lib")
#pragma comment(lib,"pangowin32-1.0.lib")
#endif




int main(int argc, char *argv[])
{
	char *filename = NULL;
	if (argc > 1)
	{
		filename = argv[1];
		FILE *f = fopen( filename, "r");
		if (!f)
		{
			printf("Cant open file: %s\n", filename);
			exit(-1);
		}
		vcg_Parse( f);
		fclose(f);
	}
	
	// Иннициализация GTK;
	//gtk_init(&iDummy,NULL) ;
	gtk_init(&argc,/*(char***)*/&argv) ;

	// строим GUI
	std::auto_ptr<UIController> uic( new UIController( filename));

	// запускаем цикл обработки сообщений
	uic->MainLoop();

	return 0;
} /* main */



