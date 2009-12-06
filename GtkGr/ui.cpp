#include "ui.h"
#include <gdk/gdkkeysyms.h>
#include "vr_graph.h"

#define SCALING_COEF 1.1
#define MOVE_PIXELS 20

/**
 * Обработчик сигнала закрытия главного окна приложения
 */
static gboolean 
ui_close_cb( GtkWindow *window, gpointer data)
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
} /* ui_close_cb */

gboolean 
ui_key_press_cb( GtkWidget* widget, GdkEventKey* event, gpointer data)
{
	/*
  if (event->length > 0)
    printf("The key event's string is `%s'\n", event->string);

  printf("The name of this keysym is `%s'\n", 
         gdk_keyval_name(event->keyval));*/

	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();

	switch (event->keyval)
	{
	case GDK_Home:
		GdkCursor* cursor;
		cursor = gdk_cursor_new(GDK_CLOCK);
		gdk_window_set_cursor(widget->window, cursor);
		gdk_cursor_destroy(cursor);
		break;

	case GDK_Up:
		db->MoveVisibleArea( -MOVE_PIXELS, AXIS_Y);
		break;
	case GDK_Down:
		db->MoveVisibleArea( MOVE_PIXELS, AXIS_Y);
		break;
	case GDK_Left:
		db->MoveVisibleArea( -MOVE_PIXELS, AXIS_X);
		break;
	case GDK_Right:
		db->MoveVisibleArea( MOVE_PIXELS, AXIS_X);
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
} /* ui_key_press_cb */

/////////////////////////////////// drawing_area callbacks //////////////////////////////////////////////
static bool g_DaPrintEvents = true;

gboolean
ui_da_configure_event_cb( GtkWidget         *da,
                          GdkEventConfigure *event,
                          gpointer           data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
	if (g_DaPrintEvents ) g_print("configure_event\n");
	
	db->ConfigureDa();

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
} /* ui_da_configure_event_cb */

/* Redraw the screen from the pixmap */
gboolean
ui_da_expose_event_cb( GtkWidget      *da,
                       GdkEventExpose *event,
                       gpointer        data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
  	if (g_DaPrintEvents ) g_print("expose_event\n");

	db->ExposeDa( event->area.x, event->area.y,
				  event->area.width, event->area.height);

	return FALSE;   /* We've handled it, stop processing */
} /* ui_da_expose_event_cb */

gboolean
ui_da_button_press_event_cb( GtkWidget      *da,
                             GdkEventButton *event,
                             gpointer        data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
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
} /* ui_da_button_press_event_cb */

gboolean 
ui_da_scroll_event_cb( GtkWidget *da, GdkEventScroll *event, gpointer data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
  	if (g_DaPrintEvents ) g_print("scroll_event\n");

	if ( event->state & GDK_CONTROL_MASK )
	{
		/* при нажатом Cеrl'е увеличиваем/уменьшаем граф */
		gint x = (int)event->x;
		gint y = (int)event->y;
		switch (event->direction)
		{
		case GDK_SCROLL_UP:
			db->ChangeScaling( SCALING_COEF, x, y);
			break;
		case GDK_SCROLL_DOWN:
			db->ChangeScaling( 1./SCALING_COEF, x, y);
			break;
		}
	}

	return TRUE;    /* We've handled it, stop processing */
} /* ui_da_scroll_event_cb */

gboolean
ui_da_motion_notify_event_cb( GtkWidget      *da,
                              GdkEventMotion *event,
                              gpointer        data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
  	//if (g_DaPrintEvents ) g_print("motion_notify_event\n");
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
} /* ui_da_motion_notify_event_cb */


/////////////////////////////////// Menu //////////////////////////////////////////////

static void
ui_activate_action( GtkAction *action, gpointer user_data)
{
	UIController *uic = (UIController *)user_data;
	const gchar *action_name = gtk_action_get_name( action);

	g_message( "Action \"%s\" activated", action_name);
	if ( !strcmp( action_name, "Quit") )
	{
		/* выходим без диалогов всяких... :) */
		gtk_main_quit();
	}
} /* ui_activate_action */

static void
activate_radio_action( GtkAction *action, /* Действие на котором издаётся сигнал */
					   GtkRadioAction *current, /* Член группы actions который активирован */
					   gpointer user_data)
{
  g_message ("Radio action \"%s\" selected", 
	     gtk_action_get_name (GTK_ACTION (current)));
}

void
ui_activate_radio_action_mode( GtkAction *action, /* Действие на котором издаётся сигнал */
                               GtkRadioAction *current, /* Член группы actions который активирован */
                               gpointer data)
{
	UIController *uic = (UIController *)data;
	const gchar *name = gtk_action_get_name( GTK_ACTION( current));

	int old_mode = uic->m_CurrentMode;
	uic->m_CurrentMode = (Mode_t)gtk_radio_action_get_current_value( current);
	g_message( "Radio action \"%s\" (%d->%d) selected", name, old_mode, uic->m_CurrentMode);
} /* ui_activate_radio_action_mode */

static GtkActionEntry entries[] = {
  { "FileMenu", NULL, "_File" },               /* name, stock id, label */
  { "EditMenu", NULL, "_Edit" },
  { "PreferencesMenu", NULL, "_Preferences" }, /* name, stock id, label */
  { "ColorMenu", NULL, "_Color"  },            /* name, stock id, label */
  { "ShapeMenu", NULL, "_Shape" },             /* name, stock id, label */
  { "HelpMenu", NULL, "_Help" },               /* name, stock id, label */
  { "New", GTK_STOCK_NEW,                      /* name, stock id */
    "_New", "<control>N",                      /* label, accelerator */
    "Create a new file",                       /* tooltip */ 
    G_CALLBACK (ui_activate_action) },      
  { "Open", GTK_STOCK_OPEN,                    /* name, stock id */
    "_Open","<control>O",                      /* label, accelerator */     
    "Open a file",                             /* tooltip */
    G_CALLBACK (ui_activate_action) }, 
  { "Save", GTK_STOCK_SAVE,                    /* name, stock id */
    "_Save","<control>S",                      /* label, accelerator */     
    "Save current file",                       /* tooltip */
    G_CALLBACK (ui_activate_action) },
  { "SaveAs", GTK_STOCK_SAVE,                  /* name, stock id */
    "Save _As...", NULL,                       /* label, accelerator */     
    "Save to a file",                          /* tooltip */
    G_CALLBACK (ui_activate_action) },
  { "Quit", GTK_STOCK_QUIT,                    /* name, stock id */
    "_Quit", "<control>Q",                     /* label, accelerator */     
    "Quit",                                    /* tooltip */
    G_CALLBACK (ui_activate_action) },
  { "About", NULL,                             /* name, stock id */
    "_About", "<control>A",                    /* label, accelerator */     
    "About",                                   /* tooltip */  
    G_CALLBACK (ui_activate_action) },
  { "Logo", "demo-gtk-logo",                   /* name, stock id */
     NULL, NULL,                               /* label, accelerator */     
    "GTK+",                                    /* tooltip */
    G_CALLBACK (ui_activate_action) },
};

static GtkRadioActionEntry mode_entries[] = {
  { "AddNode", NULL,                               /* name, stock id */
    "Add_Node", NULL,                      /* label, accelerator */     
    "", MODE_ADD_NODE },                      /* tooltip, value */
  { "AddEdge", NULL,                             /* name, stock id */
    "Add_Edge", NULL,                    /* label, accelerator */     
    "", MODE_ADD_EDGE },                    /* tooltip, value */
  { "View", NULL,                              /* name, stock id */
    "_View", NULL,                     /* label, accelerator */     
    "Режим просмотра", MODE_VIEW },                       /* tooltip, value */
};

static GtkToggleActionEntry toggle_entries[] = {
  { "Bold", GTK_STOCK_BOLD,                    /* name, stock id */
     "_Bold", "<control>B",                    /* label, accelerator */     
    "Bold",                                    /* tooltip */
    G_CALLBACK (ui_activate_action), 
    TRUE },                                    /* is_active */
};

enum {
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE
};

static GtkRadioActionEntry color_entries[] = {
  { "Red", NULL,                               /* name, stock id */
    "_Red", "<control>R",                      /* label, accelerator */     
    "Blood", COLOR_RED },                      /* tooltip, value */
  { "Green", NULL,                             /* name, stock id */
    "_Green", "<control>G",                    /* label, accelerator */     
    "Grass", COLOR_GREEN },                    /* tooltip, value */
  { "Blue", NULL,                              /* name, stock id */
    "_Blue", "<control>B",                     /* label, accelerator */     
    "Sky", COLOR_BLUE },                       /* tooltip, value */
};

enum {
  SHAPE_SQUARE,
  SHAPE_RECTANGLE,
  SHAPE_OVAL
};

static GtkRadioActionEntry shape_entries[] = {
  { "Square", NULL,                            /* name, stock id */
    "_Square", "<control>S",                   /* label, accelerator */     
    "Square",  SHAPE_SQUARE },                 /* tooltip, value */
  { "Rectangle", NULL,                         /* name, stock id */
    "_Rectangle", "<control>R",                /* label, accelerator */     
    "Rectangle", SHAPE_RECTANGLE },            /* tooltip, value */
  { "Oval", NULL,                              /* name, stock id */
    "_Oval", "<control>O",                     /* label, accelerator */     
    "Egg", SHAPE_OVAL },                       /* tooltip, value */  
};

static const gchar *ui_info = 
"<ui>"
"  <menubar name='MenuBar'>"
"    <menu action='FileMenu'>"
"      <menuitem action='New'/>"
"      <menuitem action='Open'/>"
"      <menuitem action='Save'/>"
"      <menuitem action='SaveAs'/>"
"      <separator/>"
"      <menuitem action='Quit'/>"
"    </menu>"
"    <menu action='EditMenu'>"
"      <separator/>"
"      <menuitem action='AddNode'/>"
"      <menuitem action='AddEdge'/>"
"      <menuitem action='View'/>"
"      <separator/>"
"    </menu>"
"    <menu action='PreferencesMenu'>"
"      <menu action='ColorMenu'>"
"        <menuitem action='Red'/>"
"        <menuitem action='Green'/>"
"        <menuitem action='Blue'/>"
"      </menu>"
"      <menu action='ShapeMenu'>"
"        <menuitem action='Square'/>"
"        <menuitem action='Rectangle'/>"
"        <menuitem action='Oval'/>"
"      </menu>"
"      <menuitem action='Bold'/>"
"    </menu>"
"    <menu action='HelpMenu'>"
"      <menuitem action='About'/>"
"    </menu>"
"  </menubar>"
"  <toolbar  name='ToolBar'>"
"    <toolitem action='Open'/>"
"    <toolitem action='Quit'/>"
"    <separator action='Sep1'/>"
"    <toolitem action='Logo'/>"
"  </toolbar>"
"</ui>";

///////////////////////////////////////////////////////////////////////////////////


/**
 * Создание меню, возвращается menubar
 */
GtkWidget *UIController::ConstrMenubar( GtkWidget *main_window)
{
	GtkUIManager *ui;
	GtkActionGroup *actions;
	GError *error = NULL;


	/* Actions */
	actions = gtk_action_group_new ("Actions");
	gtk_action_group_add_actions( actions, entries, G_N_ELEMENTS(entries), this);
	gtk_action_group_add_radio_actions( actions, 
										mode_entries, G_N_ELEMENTS(mode_entries),
										m_CurrentMode,
										G_CALLBACK(ui_activate_radio_action_mode), 
										this); // user_data
	gtk_action_group_add_toggle_actions (actions, 
				   toggle_entries, G_N_ELEMENTS(toggle_entries), 
				   this);
	gtk_action_group_add_radio_actions (actions, 
				  color_entries, G_N_ELEMENTS(color_entries), 
				  COLOR_RED,
				  G_CALLBACK (activate_radio_action), 
				  NULL); // user_data
	gtk_action_group_add_radio_actions (actions, 
				  shape_entries, G_N_ELEMENTS(shape_entries), 
				  SHAPE_OVAL,
				  G_CALLBACK (activate_radio_action), 
				  NULL);

	/* ui */
	ui = gtk_ui_manager_new ();
	gtk_ui_manager_insert_action_group (ui, actions, 0);
	g_object_unref (actions);
	gtk_window_add_accel_group (GTK_WINDOW (main_window), 
			  gtk_ui_manager_get_accel_group (ui));



	if (!gtk_ui_manager_add_ui_from_string (ui, ui_info, -1, &error))
	{
		g_message ("building menus failed: %s", error->message);
		g_error_free (error);
	}

	m_UIManager = ui;
	return gtk_ui_manager_get_widget (ui, "/MenuBar");
} /* UIController::ConstrMenubar */

/**
 * Конструктор, здесь мы создаем весь GUI
 */
UIController::UIController( bool is_gdl_present)
	: m_UIManager( NULL)
	,m_CurrentMode( MODE_VIEW)
	, m_Toolbar( NULL)
	, m_Statusbar( NULL)
{
	GtkWidget *main_window;
	GtkWidget *main_vbox;
	GtkWidget *menubar;

	// создаем главное окно
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
	// выставляем величину окантовки внутренней области главного окна
	//gtk_container_set_border_width( GTK_CONTAINER(main_window), 5);
	gtk_window_set_default_size( GTK_WINDOW(main_window), 600, 400);
	// закрытие/уничтожение главного окна
	gtk_signal_connect( GTK_OBJECT(main_window),"delete_event", GTK_SIGNAL_FUNC(ui_close_cb),NULL );
	gtk_signal_connect( GTK_OBJECT(main_window),"destroy", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);

	gtk_window_set_title( GTK_WINDOW(main_window), "avcg tool");

	main_vbox = gtk_vbox_new( FALSE, 1);
	gtk_container_add( GTK_CONTAINER(main_window), main_vbox);

	menubar = ConstrMenubar( main_window);
	gtk_box_pack_start( GTK_BOX(main_vbox), menubar, FALSE, TRUE, 0);
	

	GtkWidget *da = gtk_drawing_area_new();
	/* set a minimum size */
	gtk_widget_set_size_request( da, 100, 100);

	gtk_container_add( GTK_CONTAINER(main_vbox), da);

	m_VRGraph.reset( new VRGraph());
	if (is_gdl_present)
	{
		m_VRGraph->LoadGDL();
	}
	m_DrawBuffer.reset( new DrawBuffer( da, m_VRGraph.get()));

	/* добавляем к da обработчики событий */
	g_signal_connect( da, "expose-event", G_CALLBACK( ui_da_expose_event_cb), this);
	g_signal_connect( da, "configure-event", G_CALLBACK( ui_da_configure_event_cb), this);
	g_signal_connect( da, "motion-notify-event", G_CALLBACK( ui_da_motion_notify_event_cb), this);
	g_signal_connect( da, "button-press-event", G_CALLBACK( ui_da_button_press_event_cb), this);
	g_signal_connect( da, "scroll-event", G_CALLBACK( ui_da_scroll_event_cb), this);

	/* Ask to receive events the drawing area doesn't normally
	 * subscribe to */
	gtk_widget_set_events( da, gtk_widget_get_events (da)
								| GDK_LEAVE_NOTIFY_MASK
								| GDK_BUTTON_PRESS_MASK
								//| GDK_SCROLL_MASK
								| GDK_POINTER_MOTION_MASK
								| GDK_POINTER_MOTION_HINT_MASK);

	// сигнал ("key_press_event")
	gtk_signal_connect( GTK_OBJECT(main_window),"key_press_event", GTK_SIGNAL_FUNC(ui_key_press_cb), this);

printf("before gtk_widget_show_all(main_window)\n");

	// отображение основного окна
	gtk_widget_show_all( main_window);
} /* UIController::UIController */

UIController::~UIController(void)
{
	g_object_unref( m_UIManager);
} /* UIController::~UIController */

