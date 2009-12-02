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




#ifdef WIN32____
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
#endif
{
#ifdef WIN32
	printf("Hello, its GtkGr! (sizeof(_TCHAR)=%d) (sizeof(gchar)=%d)\n",  sizeof(_TCHAR), sizeof(gchar));
#endif
	printf("sizeof(long)=%d\n",  sizeof(long));
	
	bool is_gdl_present = false;
	if (argc > 1)
	{
		FILE *f = fopen( argv[1], "r");
		if (!f)
		{
			printf("Cant open file: %s\n", argv[1]);
			exit(-1);
		}
		vcg_Parse( f);
		fclose(f);
		is_gdl_present = true;
	}
	
	// Иннициализация GTK;
	//gtk_init(&iDummy,NULL) ;
	gtk_init(&argc,/*(char***)*/&argv) ;

	// строим GUI
	std::auto_ptr<UIController> uic( new UIController( is_gdl_present));

	// запускаем цикл обработки сообщений
	uic->MainLoop();

	return 0;
} /* main */



