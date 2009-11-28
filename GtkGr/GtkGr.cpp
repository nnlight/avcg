// GtkGr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "draw_buffer.h"
#include "vr_graph.h"

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


static gboolean 
CloseCallback( GtkWindow *window)
{
	GtkWidget *dialog, *label;

	dialog = gtk_dialog_new_with_buttons( "quit dialog", window,
							GtkDialogFlags(GTK_DIALOG_MODAL | GTK_DIALOG_NO_SEPARATOR),
							GTK_STOCK_YES, GTK_RESPONSE_YES,
							GTK_STOCK_NO, GTK_RESPONSE_NO,
							NULL);
	gtk_container_set_border_width( GTK_CONTAINER(dialog), 10);
	label = gtk_label_new ("\nDo you really want to quit?\n");
	gtk_container_add( GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), label);
	gtk_widget_show( label);
	int response = gtk_dialog_run( GTK_DIALOG(dialog));
	gtk_widget_destroy( dialog);
	if ( response == GTK_RESPONSE_YES )
	{
		return FALSE; /* закрываемся*/
	} else
	{
		return TRUE;  /* отбой */
	}
} /* CloseCallback */


static gint 
key_press_cb( GtkWidget* widget, GdkEventKey* event, gpointer data)
{
	/*
  if (event->length > 0)
    printf("The key event's string is `%s'\n", event->string);

  printf("The name of this keysym is `%s'\n", 
         gdk_keyval_name(event->keyval));*/

	DrawBuffer *db = (DrawBuffer *)data;
	switch (event->keyval)
	{
	case GDK_Home:
		GdkCursor* cursor;
		cursor = gdk_cursor_new(GDK_CLOCK);
		gdk_window_set_cursor(widget->window, cursor);
		gdk_cursor_destroy(cursor);
		break;

	case GDK_Up:
		db->MoveVisibleArea( -10, AXIS_Y);
		break;
	case GDK_Down:
		db->MoveVisibleArea( 10, AXIS_Y);
		break;
	case GDK_Left:
		db->MoveVisibleArea( -10, AXIS_X);
		break;
	case GDK_Right:
		db->MoveVisibleArea( 10, AXIS_X);
		break;

	case GDK_p:
		db->PKey();
		break;

	default:
		break;
	}
/*
  if (gdk_keyval_is_lower(event->keyval))
    {
      printf("A non-uppercase key was pressed.\n");
    }
  else if (gdk_keyval_is_upper(event->keyval))
    {
      printf("An uppercase letter was pressed.\n");
    }*/
	//return TRUE;
	return FALSE;
} /* key_press_cb */




static bool g_DaPrintEvents = true;

static gboolean
da_configure_event_cb( GtkWidget         *da,
                       GdkEventConfigure *event,
                       gpointer           data)
{
	DrawBuffer *db = (DrawBuffer *)data;
	if (g_DaPrintEvents ) g_print("configure_event\n");
	
	db->ConfigureDa();

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
} /* da_configure_event_cb */

/* Redraw the screen from the pixmap */
static gboolean
da_expose_event_cb( GtkWidget      *da,
                    GdkEventExpose *event,
                    gpointer        data)
{
	DrawBuffer *db = (DrawBuffer *)data;
  	if (g_DaPrintEvents ) g_print("expose_event\n");

	db->ExposeDa( event->area.x, event->area.y,
				  event->area.width, event->area.height);

	return FALSE;   /* We've handled it, stop processing */
} /* da_expose_event_cb */

static gboolean
da_button_press_event_cb( GtkWidget      *da,
                          GdkEventButton *event,
                          gpointer        data)
{
	DrawBuffer *db = (DrawBuffer *)data;
  	if (g_DaPrintEvents ) g_print("button_press_event\n");

	if (event->button == 1)
	{
		db->ButtonPress( (int)event->x, (int)event->y);
	} else if (event->button == 2)
	{
		db->ButtonPress2( (int)event->x, (int)event->y);
	} else
	{
		printf("unhandled button press %d\n", event->button);
	}
  
	return TRUE;    /* We've handled it, stop processing */
} /* da_button_press_event_cb */

static gboolean
da_motion_notify_event_cb( GtkWidget      *da,
                           GdkEventMotion *event,
                           gpointer        data)
{
	DrawBuffer *db = (DrawBuffer *)data;
  	/*if (g_DaPrintEvents ) g_print("motion_notify_event\n");*/
	int x, y;
	GdkModifierType state;

  /* This call is very important; it requests the next motion event.
   * If you don't call gdk_window_get_pointer() you'll only get
   * a single motion event. The reason is that we specified
   * GDK_POINTER_MOTION_HINT_MASK to gtk_widget_set_events().
   * If we hadn't specified that, we could just use event->x, event->y
   * as the pointer location. But we'd also get deluged in events.
   * By requesting the next event as we handle the current one,
   * we avoid getting a huge number of events faster than we
   * can cope.
   */
	gdk_window_get_pointer (event->window, &x, &y, &state);

	if (state & GDK_BUTTON1_MASK)
		db->ButtonPress( x, y);

	return TRUE;  /* We've handled it, stop processing */
} /* da_motion_notify_event_cb */



/* Obligatory basic callback */
static void print_hello( GtkWidget *w,
                         gpointer   data )
{
  g_message ("Hello, World!\n");
}

/* This is the GtkItemFactoryEntry structure used to generate new menus.
   Item 1: The menu path. The letter after the underscore indicates an
           accelerator key once the menu is open.
   Item 2: The accelerator key for the entry
   Item 3: The callback function.
   Item 4: The callback action.  This changes the parameters with
           which the function is called.  The default is 0.
   Item 5: The item type, used to define what kind of an item it is.
           Here are the possible values:

           NULL               -> "<Item>"
           ""                 -> "<Item>"
           "<Title>"          -> create a title item
           "<Item>"           -> create a simple item
           "<CheckItem>"      -> create a check item
           "<ToggleItem>"     -> create a toggle item
           "<RadioItem>"      -> create a radio item
           <path>             -> path of a radio item to link against
           "<Separator>"      -> create a separator
           "<Branch>"         -> create an item to hold sub items (optional)
           "<LastBranch>"     -> create a right justified branch 
*/

static GtkItemFactoryEntry menu_items[] = {
  { "/_File",         NULL,         NULL, 0, "<Branch>" },
  { "/File/_New",     "<control>N", (GtkItemFactoryCallback)print_hello, 0, NULL },
  { "/File/_Open",    "<control>O", (GtkItemFactoryCallback)print_hello, 0, NULL },
  { "/File/_Save",    "<control>S", (GtkItemFactoryCallback)print_hello, 0, NULL },
  { "/File/Save _As", NULL,         NULL, 0, NULL },
  { "/File/sep1",     NULL,         NULL, 0, "<Separator>" },
  { "/File/Quit",     "<control>Q", gtk_main_quit, 0, NULL },
  { "/_Options",      NULL,         NULL, 0, "<Branch>" },
  { "/Options/Test",  NULL,         NULL, 0, NULL },
  { "/Options/ToggleTest",  NULL,   NULL, 0, "<ToggleItem>" },
  { "/Options/CheckTest",  NULL,         NULL, 0, "<CheckItem>" },
  { "/_Help",         NULL,         NULL, 0, "<LastBranch>" },
  { "/_Help/About",   NULL,         NULL, 0, NULL },
};

void get_main_menu( GtkWidget  *window,
                    GtkWidget **menubar )
{
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

  accel_group = gtk_accel_group_new ();

  /* This function initializes the item factory.
     Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
              or GTK_TYPE_OPTION_MENU.
     Param 2: The path of the menu.
     Param 3: A pointer to a gtk_accel_group.  The item factory sets up
              the accelerator table while generating menus.
  */

  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
                                       accel_group);

  /* This function generates the menu items. Pass the item factory,
     the number of items in the array, the array itself, and any
     callback data for the the menu items. */
  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);

  /* Attach the new accelerator group to the window. */
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

  if (menubar)
    /* Finally, return the actual menu bar created by the item factory. */ 
    *menubar = gtk_item_factory_get_widget (item_factory, "<main>");
}


#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
#endif
{
#ifdef WIN32
	printf("Hello, its GtkGr! (sizeof(_TCHAR)=%d) (sizeof(gchar)=%d)\n",  sizeof(_TCHAR), sizeof(gchar));
#endif
	printf("sizeof(long)=%d\n",  sizeof(long));
	
	
	
	int iDummy=0;
	GtkWidget *main_window;
	GtkWidget *main_vbox;
	GtkWidget *menubar;


	// Иннициализация GTK;
	gtk_init(&iDummy,NULL) ;
	// assert( sizeof(_TCHAR) == sizeof(char) );
	// gtk_init(&argc,(char***)&argv) ;
	// создаем главное окно
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
	// выставляем величину окантовки внутренней области главного окна
	//gtk_container_set_border_width( GTK_CONTAINER(main_window), 5);
	// сигнал ("delete_event") - принудительное закрытие главного окна
	gtk_signal_connect( GTK_OBJECT(main_window),"delete_event", GTK_SIGNAL_FUNC(CloseCallback),NULL );
	// сигнал ("destroy") - уничтожение главного окна
	gtk_signal_connect( GTK_OBJECT(main_window),"destroy", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);

	gtk_window_set_title( GTK_WINDOW(main_window), "avcg tool");

	main_vbox = gtk_vbox_new( FALSE, 1);
	//gtk_container_border_width( GTK_CONTAINER (main_vbox), 1);
	gtk_container_add( GTK_CONTAINER(main_window), main_vbox);

	get_main_menu (main_window, &menubar);
	gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, TRUE, 0);
	

	GtkWidget *da = gtk_drawing_area_new();
	/* set a minimum size */
	gtk_widget_set_size_request( da, 600, 400);

	gtk_container_add( GTK_CONTAINER(main_vbox), da);

	std::auto_ptr<VRGraph> vr_graph( new VRGraph());
	std::auto_ptr<DrawBuffer> draw_buffer( new DrawBuffer( da, vr_graph.get()));
	DrawBuffer *db = draw_buffer.get();

	/* добавляем к da обработчики событий */
	g_signal_connect( da, "expose-event", G_CALLBACK( da_expose_event_cb), db);
	g_signal_connect( da, "configure-event", G_CALLBACK( da_configure_event_cb), db);
	g_signal_connect( da, "motion-notify-event", G_CALLBACK( da_motion_notify_event_cb), db);
	g_signal_connect( da, "button-press-event",	G_CALLBACK( da_button_press_event_cb), db);

	/* Ask to receive events the drawing area doesn't normally
	 * subscribe to */
	gtk_widget_set_events( da, gtk_widget_get_events (da)
								| GDK_LEAVE_NOTIFY_MASK
								| GDK_BUTTON_PRESS_MASK
								| GDK_POINTER_MOTION_MASK
								| GDK_POINTER_MOTION_HINT_MASK);

	// сигнал ("key_press_event")
	gtk_signal_connect( GTK_OBJECT(main_window),"key_press_event", GTK_SIGNAL_FUNC(key_press_cb), db);


	// отображение основного окна
	gtk_widget_show_all( main_window);
	gtk_main();
	return 0;
}

