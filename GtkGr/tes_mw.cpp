#include "mw/mw_duck.h"
#include <stdlib.h>
#include <gtk/gtk.h>

/**
 * Реализация собственного виджета MwDuck
 */

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


void win( GtkWidget *widget,
          gpointer   data )
{
  g_print ("Yay!\n");
  mw_duck_clear (MW_DUCK (widget));
}

int main( int   argc,
          char *argv[] )
{
  GtkWidget *window;
  GtkWidget *ttt;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  gtk_window_set_title (GTK_WINDOW (window), "Aspect Frame");

  g_signal_connect (window, "destroy",
            G_CALLBACK (exit), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  ttt = mw_duck_new ();

  gtk_container_add (GTK_CONTAINER (window), ttt);
  gtk_widget_show (ttt);

  /* And attach to its "tictactoe" signal */
  g_signal_connect (ttt, "tictactoe",
            G_CALLBACK (win), NULL);

  gtk_widget_show (window);

  gtk_main ();

  return 0;
}

