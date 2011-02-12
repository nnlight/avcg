#include "ui.h"
#include <gdk/gdkkeysyms.h>
#include "vr_graph.h"
#include "vcg/vcg_iface.h"
#include "preferences.h"


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

	int MOVE_PIXELS = g_Preferences->GetMovePixels();

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

	case GDK_x:
		gdk_set_show_events( TRUE);
		break;
	case GDK_z:
		gdk_set_show_events( FALSE);
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

gboolean
ui_da_configure_event_cb( GtkWidget         *da,
                          GdkEventConfigure *event,
                          gpointer           data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
	if (g_Preferences->DebugGetPrintEvents()) g_print("configure_event\n");

#ifdef WIN32
	static GdkWindow *da_window = NULL;
	if  (da_window == NULL)
	{
		da_window = da->window;
		gdk_window_add_filter( uic->m_MainWindow->window, &ui_win32_main_window_filter, da->window);
	}
	assert( da_window == da->window );
	//gdk_window_add_filter( da->window, da_filter_func, NULL/*data*/);
#endif /* WIN32 */

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
	if (g_Preferences->DebugGetPrintEvents()) g_print("expose_event (count=%d)\n", event->count);

	db->ExposeDa( event->area.x, event->area.y,
				  event->area.width, event->area.height);

	return FALSE;   /* We've handled it, stop processing */
} /* ui_da_expose_event_cb */

gboolean
ui_da_mouse_button_press_cb( GtkWidget      *da,
                             GdkEventButton *event,
                             gpointer        data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
  	if (g_Preferences->DebugGetPrintEvents()) g_print("button_press_event\n");

	if ( event->type != GDK_BUTTON_PRESS )
	{
		/* двойные и тройные щелчки мы не обрабатываем */
		return FALSE;
	}
	int x = (int)event->x;
	int y = (int)event->y;
	if (event->button == 1)
	{
		if ( uic->m_CurrentMode == MODE_VIEW_NODE_INFO1
			|| uic->m_CurrentMode == MODE_VIEW_NODE_INFO2
			|| uic->m_CurrentMode == MODE_VIEW_NODE_INFO3 )
		{
			int vrg_x, vrg_y;
			db->ConvertDa2Vrg( x, y, vrg_x, vrg_y);
			uic->m_VRGraph->HandleInfoBoxPress( db, vrg_x, vrg_y,
				                                uic->m_CurrentMode - MODE_VIEW_NODE_INFO1 + 1);
		} else
		{
			db->ButtonPress( x, y);
		}
	} else if (event->button == 2)
	{
		db->ButtonPress2( x, y);
	} else if (event->button == 3)
	{
		uic->m_PickUped = true;
		uic->m_PickUpBase[AXIS_X] = x;
		uic->m_PickUpBase[AXIS_Y] = y;
	} else
	{
		printf("unhandled button press %d\n", event->button);
	}
  
	return TRUE;    /* We've handled it, stop processing */
} /* ui_da_mouse_button_press_cb */

gboolean
ui_da_mouse_button_release_cb( GtkWidget      *da,
                             GdkEventButton *event,
                             gpointer        data)
{
	UIController *uic = (UIController *)data;
	//DrawBuffer *db = uic->m_DrawBuffer.get();
  	if (g_Preferences->DebugGetPrintEvents()) g_print("button_release_event\n");

	int x = (int)event->x;
	int y = (int)event->y;
	if (event->button == 3)
	{
		assert( uic->m_PickUped == true );
		uic->m_PickUped = false;
		if (g_Preferences->DebugGetPrintEvents()) printf("x=%d  y=%d\n", x, y);
	}
  
	return TRUE;    /* We've handled it, stop processing */
} /* ui_da_mouse_button_release_cb */

gboolean
ui_da_delayed_zoom_cb( gpointer data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
	if (g_Preferences->DebugGetPrintEvents())
		g_print( "delayed_zoom\n");

	if ( uic->m_DelayedZoomDelta == 0 )
	{
		return FALSE;
	}

	gint x = uic->m_DelayedZoomBase[AXIS_X];
	gint y = uic->m_DelayedZoomBase[AXIS_Y];
	double SCALING_COEF = g_Preferences->GetScalingCoef();
	double scaling_coef = 1.;
	int delta = uic->m_DelayedZoomDelta;
	for ( ; delta > 0; delta-- )
	{
		scaling_coef *= SCALING_COEF;
	}
	for ( ; delta < 0; delta++ )
	{
		scaling_coef /= SCALING_COEF;
	}
	uic->m_DelayedZoomDelta = 0;

	db->ChangeScaling( scaling_coef, x, y);
	uic->UpdateStatusbar();

	return FALSE; /* source func should be removed */
} /* ui_da_delayed_zoom_cb */

gboolean 
ui_da_mouse_scroll_cb( GtkWidget *da, GdkEventScroll *event, gpointer data)
{
	UIController *uic = (UIController *)data;
	//DrawBuffer *db = uic->m_DrawBuffer.get();
	if (g_Preferences->DebugGetPrintEvents())
	{
		g_print("scroll_event %d,state=%d,%d, %d\n", event->send_event, event->state, event->type,event->time);
		//printf( "(%f %f) (%f %f) \n", event->x, event->y, event->x_root, event->y_root);
	}

	if ( 1/*event->state & GDK_CONTROL_MASK*/ )
	{
		/* при нажатом Ctrl'е увеличиваем/уменьшаем граф */
		uic->m_DelayedZoomBase[AXIS_X] = (int)event->x;
		uic->m_DelayedZoomBase[AXIS_Y] = (int)event->y;
		/* регистрировать событие надо только если оно уже не было заругистрировано */
		bool is_need_delayed_event = (uic->m_DelayedZoomDelta == 0);
		switch (event->direction)
		{
			case GDK_SCROLL_UP:   uic->m_DelayedZoomDelta++; break;
			case GDK_SCROLL_DOWN: uic->m_DelayedZoomDelta--; break;
			default: break;
		}
		if ( g_Preferences->DebugGetDelayedZooming() )
		{
			if ( is_need_delayed_event )
				g_idle_add_full( G_PRIORITY_HIGH_IDLE, &ui_da_delayed_zoom_cb, (gpointer)uic, NULL);
		} else
		{
			ui_da_delayed_zoom_cb( uic);
		}
	}

	return TRUE;    /* We've handled it, stop processing */
} /* ui_da_mouse_scroll_cb */

gboolean
ui_da_mouse_motion_notify_cb( GtkWidget      *da,
                              GdkEventMotion *event,
                              gpointer        data)
{
	UIController *uic = (UIController *)data;
	DrawBuffer *db = uic->m_DrawBuffer.get();
  	//if (g_Preferences->DebugGetPrintEvents()) g_print("motion_notify_event\n");
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
	
	if (uic->m_PickUped)
	{
		int delta[AXIS_LAST];
		delta[AXIS_X] = uic->m_PickUpBase[AXIS_X] - x;
		delta[AXIS_Y] = uic->m_PickUpBase[AXIS_Y] - y;
		uic->m_PickUpBase[AXIS_X] = x;
		uic->m_PickUpBase[AXIS_Y] = y;
		db->MoveVisibleArea2d( delta);
	}

	return TRUE;  /* We've handled it, stop processing */
} /* ui_da_mouse_motion_notify_cb */



/////////////////////////////////// Menu //////////////////////////////////////////////

void
ui_activate_action( GtkAction *action, gpointer data)
{
	UIController *uic = (UIController *)data;
	const gchar *action_name = gtk_action_get_name( action);

	if (g_Preferences->DebugGetPrintActions())
		g_message( "Action \"%s\" activated", action_name);
	if ( !strcmp( action_name, "Quit") )
	{
		/* выходим без диалогов всяких... :) */
		gtk_main_quit();
	} else if ( !strcmp( action_name, "Open") )
	{
		GtkWidget *dialog;
		dialog = gtk_file_chooser_dialog_new( "Open File", 
											GTK_WINDOW( uic->m_MainWindow),
											GTK_FILE_CHOOSER_ACTION_OPEN,
											GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
											GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
											NULL);
		if (gtk_dialog_run( GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		{
			char *filename;
			filename = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER(dialog));
			uic->LoadGDL( filename);
			g_free( filename);
		}
		gtk_widget_destroy (dialog);
	} else if ( !strcmp( action_name, "Reload") )
	{
		if ( !uic->m_CurrentFilename.empty() )
		{
			uic->LoadGDL( uic->m_CurrentFilename.c_str());
		}
	} else if ( !strcmp( action_name, "FindNode") )
	{
		UiShowFindNodeDialog( uic);
	} else if ( !strcmp( action_name, "Preferences_SetDefaults") )
	{
		delete g_Preferences;
		g_Preferences = new Preferences();
	} else if ( !strcmp( action_name, "Preferences_Load") )
	{
		g_Preferences->LoadFromFile();
	} else if ( !strcmp( action_name, "Preferences_Save") )
	{
		g_Preferences->SaveToFile();
	} else if ( !strcmp( action_name, "About") )
	{
		uic->ShowAboutDialog();
	}
	return;
} /* ui_activate_action */

static void
activate_radio_action( GtkAction *action, /* Действие на котором издаётся сигнал */
					   GtkRadioAction *current, /* Член группы actions который активирован */
					   gpointer user_data)
{
  if (g_Preferences->DebugGetPrintActions())
	  g_message( "Radio action \"%s\" selected", 
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
	uic->UpdateStatusbar();
	if (g_Preferences->DebugGetPrintActions())
		g_message( "Radio action \"%s\" (%d->%d) selected", name, old_mode, uic->m_CurrentMode);
} /* ui_activate_radio_action_mode */

static GtkActionEntry entries[] = {
  { "FileMenu", NULL, "_File" },               /* name, stock id, label */
  { "EditMenu", NULL, "_Edit" },
  { "ViewMenu", NULL, "_View" },
  { "PreferencesMenu", NULL, "_Preferences" }, /* name, stock id, label */
  { "ColorMenu", NULL, "_Color"  },            /* name, stock id, label */
  { "ShapeMenu", NULL, "_Shape" },             /* name, stock id, label */
  { "HelpMenu", NULL, "_Help" },               /* name, stock id, label */
  { "New", GTK_STOCK_NEW,                      /* name, stock id */
    "_New", "<control>N",                      /* label, accelerator */
    "Create a new file",                       /* tooltip */ 
    G_CALLBACK (ui_activate_action) },      
  { "Open", GTK_STOCK_OPEN,                    /* name, stock id */
    "_Open...","<control>O",                   /* label, accelerator */
    "Open a file",                             /* tooltip */
    G_CALLBACK (ui_activate_action) }, 
  { "Reload", GTK_STOCK_REFRESH,               /* name, stock id */
    "_Reload","<control>R",                    /* label, accelerator */
    "Reload file",                             /* tooltip */
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
  { "FindNode", GTK_STOCK_FIND,                /* name, stock id */
    "Find Node...", "<control>F",              /* label, accelerator */
    NULL,                                      /* tooltip */
    G_CALLBACK (ui_activate_action) },
  { "Preferences_SetDefaults", NULL,           /* name, stock id */
    "Set Defaults", NULL,                      /* label, accelerator */
    NULL,                                      /* tooltip */
    G_CALLBACK (ui_activate_action) },
  { "Preferences_Load", NULL,                  /* name, stock id */
    "Load", NULL,                              /* label, accelerator */
    NULL,                                      /* tooltip */
    G_CALLBACK (ui_activate_action) },
  { "Preferences_Save", NULL,                  /* name, stock id */
    "Save", NULL,                              /* label, accelerator */
    NULL,                                      /* tooltip */
    G_CALLBACK (ui_activate_action) },
  { "About", GTK_STOCK_ABOUT,                  /* name, stock id */
    "_About...", "<control>A",                 /* label, accelerator */
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
    "_View", "v",                     /* label, accelerator */     
    "Режим просмотра", MODE_VIEW },                       /* tooltip, value */
  { "Mode_ViewNodeInfo1", NULL,                              /* name, stock id */
    "ViewNodeInfo1", "i",                     /* label, accelerator */     
    "", MODE_VIEW_NODE_INFO1 },                       /* tooltip, value */
  { "Mode_ViewNodeInfo2", NULL,                              /* name, stock id */
    "ViewNodeInfo2", "j",                     /* label, accelerator */     
    "", MODE_VIEW_NODE_INFO2 },                       /* tooltip, value */
  { "Mode_ViewNodeInfo3", NULL,                              /* name, stock id */
    "ViewNodeInfo3", "<shift>i",                     /* label, accelerator */     
    "", MODE_VIEW_NODE_INFO3 },                       /* tooltip, value */
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
"      <menuitem action='Reload'/>"
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
"      <menuitem action='Mode_ViewNodeInfo1'/>"
"      <menuitem action='Mode_ViewNodeInfo2'/>"
"      <menuitem action='Mode_ViewNodeInfo3'/>"
"      <separator/>"
"    </menu>"
"    <menu action='ViewMenu'>"
"      <menuitem action='FindNode'/>"
"    </menu>"
"    <menu action='PreferencesMenu'>"
"      <menuitem action='Preferences_SetDefaults'/>"
"      <menuitem action='Preferences_Load'/>"
"      <menuitem action='Preferences_Save'/>"
"      <separator/>"
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
UIController::UIController( const char *filename)
	: m_MainWindow( NULL)
	, m_UIManager( NULL)
	, m_CurrentMode( MODE_VIEW)
	, m_Toolbar( NULL)
	, m_Statusbar( NULL)
	, m_PickUped( false)
	, m_DelayedZoomDelta( 0)
{
	GtkWidget *main_window;
	GtkWidget *main_vbox;
	GtkWidget *menubar;
	GtkWidget *statusbar;

	// создаем главное окно
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
	// выставляем величину окантовки внутренней области главного окна
	//gtk_container_set_border_width( GTK_CONTAINER(main_window), 5);
	gtk_window_set_default_size( GTK_WINDOW(main_window),
							     g_Preferences->GetSizeX(),
								 g_Preferences->GetSizeY());
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
	if (filename)
	{
		m_VRGraph->LoadGDL();
		m_CurrentFilename = filename;
		gtk_window_set_title( GTK_WINDOW(main_window), filename);
	}
	m_DrawBuffer.reset( new DrawBuffer( da));
	m_DrawBuffer->SetVRGraphRef( m_VRGraph.get());

	/* добавляем к da обработчики событий */
	g_signal_connect( da, "expose-event", G_CALLBACK( ui_da_expose_event_cb), this);
	g_signal_connect( da, "configure-event", G_CALLBACK( ui_da_configure_event_cb), this);
	g_signal_connect( da, "motion-notify-event", G_CALLBACK( ui_da_mouse_motion_notify_cb), this);
	g_signal_connect( da, "button-press-event", G_CALLBACK( ui_da_mouse_button_press_cb), this);
	g_signal_connect( da, "button-release-event", G_CALLBACK( ui_da_mouse_button_release_cb), this);
	g_signal_connect( da, "scroll-event", G_CALLBACK( ui_da_mouse_scroll_cb), this);

	/* Ask to receive events the drawing area doesn't normally
	 * subscribe to */
	gtk_widget_set_events( da, gtk_widget_get_events( da)
								| GDK_LEAVE_NOTIFY_MASK
								| GDK_BUTTON_PRESS_MASK
								| GDK_BUTTON_RELEASE_MASK
								| GDK_SCROLL_MASK
								| GDK_POINTER_MOTION_MASK
								| GDK_POINTER_MOTION_HINT_MASK
							);

	// сигнал ("key_press_event")
	gtk_signal_connect( GTK_OBJECT(main_window),"key_press_event", GTK_SIGNAL_FUNC(ui_key_press_cb), this);

	// Statusbar
	statusbar = gtk_statusbar_new ();
	gtk_box_pack_end( GTK_BOX(main_vbox), statusbar, FALSE, TRUE, 0);



	m_MainWindow = main_window;
	m_Statusbar = statusbar;

//printf("before gtk_widget_show_all(main_window)\n");
	// отображение основного окна
	gtk_widget_show_all( main_window);

	UpdateStatusbar();
	return;
} /* UIController::UIController */

UIController::~UIController(void)
{
	g_object_unref( m_UIManager);
} /* UIController::~UIController */

void UIController::LoadGDL( const char *filename)
{
	assert( filename );
	if ( !m_CurrentFilename.empty() )
	{
	}

	FILE *f = fopen( filename, "r");
	if (!f)
	{
		printf("Cant open file: %s\n", filename);
		exit(-1);
	}
	vcg_Parse( f);
	fclose(f);

	/* удаляем старый граф (если есть)*/
	m_VRGraph.reset( NULL);
	m_VRGraph.reset( new VRGraph());
	m_VRGraph->LoadGDL();
	m_DrawBuffer->SetVRGraphRef( m_VRGraph.get());
	m_CurrentFilename = filename;
	gtk_window_set_title( GTK_WINDOW(m_MainWindow), filename);
} /* UIController::LoadGDL */

void UIController::UpdateStatusbar()
{
	gchar *msg;

	/* clear any previous message, 
	 * underflow is allowed 
	 */
	gtk_statusbar_pop( GTK_STATUSBAR( m_Statusbar), 0);

	msg = g_strdup_printf( "Current Scaling: %f  Mode: %d",
							m_DrawBuffer->GetScaling(),
							m_CurrentMode);
	gtk_statusbar_push( GTK_STATUSBAR( m_Statusbar), 0, msg);

	g_free (msg);
	return;
} /* UIController::UpdateStatusbar */

void UIController::ShowAboutDialog()
{
#if 0
		gtk_show_about_dialog( GTK_WINDOW( uic->m_MainWindow),
			 "program-name", "GTK+ Code Demos",
			 "version", /*PACKAGE_VERSION*/__FILE__,
			 "copyright", "(C) 1997-2009 The GTK+ Team",
			 "license", "GNU General Public License v2" /*license*/,
			 "website", "http://www.gtk.org",
			 "comments", "Program to demonstrate GTK+ functions.",
			 "authors", "authors_value",
			 "documenters", "documenters_v",
			 "logo", "logo_v"/*transparent*/,
			 "title", "About GTK+ Code Demos",
			 NULL);*/
#endif
	GdkPixbuf *pixbuf, *transparent;
	gchar *filename;

	const gchar *authors[] = {
		"John Smith* <nnlight@gmail.com>",
		"and many more...",
		NULL
	};

	const gchar *documentors[] = {
		"John Smith* <nnlight@gmail.com>",
		"and many more...",
		NULL
	};

	const gchar *license =
		"GNU General Public License v2.\n"
		"\n"
		"\n"
		"This program is free software: you can redistribute it and/or\n"
		"modify it under the terms of the GNU General Public License\n"
		"as published by the Free Software Foundation, either version 2\n"
		"of the License, or (at your option) any later version.\n"
		"\n"
		"This program is distributed in the hope that it will be useful,\n"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"GNU General Public License for more details.\n"
		"\n"
		"You should have received a copy of the GNU General Public License\n"
		"along with this program. If not, see <http://www.gnu.org/licenses/>.\n";

	pixbuf = NULL;
	transparent = NULL;
	filename = NULL/*demo_find_file ("gtk-logo-rgb.gif", NULL)*/;
  if (filename)
    {
      pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
      g_free (filename);
      transparent = gdk_pixbuf_add_alpha (pixbuf, TRUE, 0xff, 0xff, 0xff);
      g_object_unref (pixbuf);
    }

	gtk_about_dialog_set_email_hook (0/*activate_email*/, NULL, NULL);
	gtk_about_dialog_set_url_hook (0/*activate_url*/, NULL, NULL);
	gtk_show_about_dialog( GTK_WINDOW( m_MainWindow),
			"program-name", "avcg tool",
			"version", "v0.1 (Build: " __DATE__ ")",
			"comments", "Just another realization of xvcg tool based on GTK+ library.",
			"copyright", "(C) 2009-2011 ...",
			"website", "http://code.google.com/p/avcg/",
			"license", license,
			"authors", authors,
			"documenters", documentors,
			/*"logo", transparent,*/
			"title", "About avcg tool",
			 NULL);

	if (transparent)
		g_object_unref( transparent);
	return;
} /* UIController::ShowAboutDialog */
