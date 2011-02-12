#include "ui.h"
#include "vr_graph.h"
#include "vcg/vcg_iface.h"

// коментарий на русском...

typedef struct
{
	const gchar *title;
	const gchar *label;
	gint   x;
	gint   y;
} FindNodeGLSNode;

enum
{
	GLS_COLUMN_TITLE,
	GLS_COLUMN_LABEL,
	GLS_COLUMN_X,
	GLS_COLUMN_Y,
	GLS_COLUMNS_COUNT
};

static GtkTreeModel *
create_model( VRGraph *vrg)
{
	GtkListStore *store;
	GtkTreeIter iter;

	/* create list store */
	store = gtk_list_store_new( GLS_COLUMNS_COUNT,
								G_TYPE_STRING,
								G_TYPE_STRING,
								G_TYPE_INT,
								G_TYPE_INT);

	/* add data to the list store */
	for ( GrNode *n = vrg->GrGetFirstNode();
		  n; 
		  n = n->GrGetNextNode() )
	{
		VRNode *node = static_cast<VRNode*>(n);
		assert( dynamic_cast<VRNode*>(n) );
		gtk_list_store_append( store, &iter);
		gtk_list_store_set(store, &iter,
						GLS_COLUMN_TITLE, node->title_.c_str(),
						GLS_COLUMN_LABEL, node->label_.c_str(),
						GLS_COLUMN_X, node->x_ + node->width_ / 2,
						GLS_COLUMN_Y, node->y_ + node->height_ / 2,
						-1);
	}
	/*for (int i = 0; i < G_N_ELEMENTS (data1); i++)
	{
		gtk_list_store_append( store, &iter);
		gtk_list_store_set(store, &iter,
						GLS_COLUMN_TITLE, data1[i].title,
						GLS_COLUMN_LABEL, data1[i].label,
						GLS_COLUMN_X, data1[i].x,
						GLS_COLUMN_Y, data1[i].y,
						-1);
	}*/

	return GTK_TREE_MODEL( store);
}

#if 0
static void
fixed_toggled_cb( GtkCellRendererToggle *cell,
	       gchar                 *path_str,
	       gpointer               data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreeIter  iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  gboolean fixed;

  /* get toggled iter */
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, COLUMN_FIXED, &fixed, -1);

  /* do something with the value */
  fixed ^= 1;

  /* set new value */
  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_FIXED, fixed, -1);

  /* clean up */
  gtk_tree_path_free (path);
}
#endif

static void
add_columns( GtkTreeView *treeview)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	//GtkTreeModel *model = gtk_tree_view_get_model( treeview);

	/* column for fixed toggles */
	renderer = gtk_cell_renderer_text_new();
	//g_signal_connect( renderer, "toggled",
	//	G_CALLBACK (fixed_toggled_cb), model);

	column = gtk_tree_view_column_new_with_attributes( "Title",
									renderer,
									"text", GLS_COLUMN_TITLE,
									NULL);

	/* set this column to a fixed sizing (of 50 pixels) */
	gtk_tree_view_column_set_sizing( GTK_TREE_VIEW_COLUMN(column), GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_fixed_width( GTK_TREE_VIEW_COLUMN(column), 50);
	gtk_tree_view_append_column( treeview, column);

	/* column for bug numbers */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes( "Label",
									renderer,
									"text", GLS_COLUMN_LABEL,
									NULL);
	gtk_tree_view_column_set_sort_column_id( column, GLS_COLUMN_LABEL);
	gtk_tree_view_append_column( treeview, column);
}


#if 0
static GtkWidget *window = NULL;

typedef struct
{
  const gboolean  fixed;
  const guint     number;
  const gchar    *severity;
  const gchar    *description;
}
Bug;

enum
{
  COLUMN_FIXED,
  COLUMN_NUMBER,
  COLUMN_SEVERITY,
  COLUMN_DESCRIPTION,
  NUM_COLUMNS
};

static Bug data[] =
{
  { FALSE, 60482, "Normal",     "scrollable notebooks and hidden tabs" },
  { FALSE, 60620, "Critical",   "gdk_window_clear_area (gdkwindow-win32.c) is not thread-safe" },
  { FALSE, 50214, "Major",      "Xft support does not clean up correctly" },
  { TRUE,  52877, "Major",      "GtkFileSelection needs a refresh method. " },
  { FALSE, 56070, "Normal",     "Can't click button after setting in sensitive" },
  { TRUE,  56355, "Normal",     "GtkLabel - Not all changes propagate correctly" },
  { FALSE, 50055, "Normal",     "Rework width/height computations for TreeView" },
  { FALSE, 58278, "Normal",     "gtk_dialog_set_response_sensitive () doesn't work" },
  { FALSE, 55767, "Normal",     "Getters for all setters" },
  { FALSE, 56925, "Normal",     "Gtkcalender size" },
  { FALSE, 56221, "Normal",     "Selectable label needs right-click copy menu" },
  { TRUE,  50939, "Normal",     "Add shift clicking to GtkTextView" },
  { FALSE, 6112,  "Enhancement","netscape-like collapsable toolbars" },
  { FALSE, 1,     "Normal",     "First bug :=)" },
};

static GtkTreeModel *
create_model (void)
{
  gint i = 0;
  GtkListStore *store;
  GtkTreeIter iter;

  /* create list store */
  store = gtk_list_store_new (NUM_COLUMNS,
			      G_TYPE_BOOLEAN,
			      G_TYPE_UINT,
			      G_TYPE_STRING,
			      G_TYPE_STRING);

  /* add data to the list store */
  for (i = 0; i < G_N_ELEMENTS (data); i++)
    {
      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
			  COLUMN_FIXED, data[i].fixed,
			  COLUMN_NUMBER, data[i].number,
			  COLUMN_SEVERITY, data[i].severity,
			  COLUMN_DESCRIPTION, data[i].description,
			  -1);
    }

  return GTK_TREE_MODEL (store);
}

static void
fixed_toggled (GtkCellRendererToggle *cell,
	       gchar                 *path_str,
	       gpointer               data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreeIter  iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  gboolean fixed;

  /* get toggled iter */
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, COLUMN_FIXED, &fixed, -1);

  /* do something with the value */
  fixed ^= 1;

  /* set new value */
  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_FIXED, fixed, -1);

  /* clean up */
  gtk_tree_path_free (path);
}

static void
add_columns (GtkTreeView *treeview)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeModel *model = gtk_tree_view_get_model (treeview);

  /* column for fixed toggles */
  renderer = gtk_cell_renderer_toggle_new ();
  g_signal_connect (renderer, "toggled",
		    G_CALLBACK (fixed_toggled), model);

  column = gtk_tree_view_column_new_with_attributes ("Fixed?",
						     renderer,
						     "active", COLUMN_FIXED,
						     NULL);

  /* set this column to a fixed sizing (of 50 pixels) */
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),
				   GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (GTK_TREE_VIEW_COLUMN (column), 50);
  gtk_tree_view_append_column (treeview, column);

  /* column for bug numbers */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Bug number",
						     renderer,
						     "text",
						     COLUMN_NUMBER,
						     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_NUMBER);
  gtk_tree_view_append_column (treeview, column);

  /* column for severities */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Severity",
						     renderer,
						     "text",
						     COLUMN_SEVERITY,
						     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_SEVERITY);
  gtk_tree_view_append_column (treeview, column);

  /* column for description */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Description",
						     renderer,
						     "text",
						     COLUMN_DESCRIPTION,
						     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_DESCRIPTION);
  gtk_tree_view_append_column (treeview, column);
}

GtkWidget *
do_list_store (GtkWidget *do_widget)
{
  if (!window)
    {
      GtkWidget *vbox;
      GtkWidget *label;
      GtkWidget *sw;
      GtkTreeModel *model;
      GtkWidget *treeview;

      /* create window, etc */
      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_screen (GTK_WINDOW (window),
			     gtk_widget_get_screen (do_widget));
      gtk_window_set_title (GTK_WINDOW (window), "GtkListStore demo");

      g_signal_connect (window, "destroy",
			G_CALLBACK (gtk_widget_destroyed), &window);
      gtk_container_set_border_width (GTK_CONTAINER (window), 8);

      vbox = gtk_vbox_new (FALSE, 8);
      gtk_container_add (GTK_CONTAINER (window), vbox);

      label = gtk_label_new ("This is the bug list (note: not based on real data, it would be nice to have a nice ODBC interface to bugzilla or so, though).");
      gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);

      sw = gtk_scrolled_window_new (NULL, NULL);
      gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
					   GTK_SHADOW_ETCHED_IN);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
				      GTK_POLICY_NEVER,
				      GTK_POLICY_AUTOMATIC);
      gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0);

      /* create tree model */
      model = create_model( NULL);

      /* create tree view */
      treeview = gtk_tree_view_new_with_model (model);
      gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeview), TRUE);
      gtk_tree_view_set_search_column (GTK_TREE_VIEW (treeview),
				       COLUMN_DESCRIPTION);

      g_object_unref (model);

      gtk_container_add (GTK_CONTAINER (sw), treeview);

      /* add columns to the tree view */
      add_columns (GTK_TREE_VIEW (treeview));

      /* finish & show */
      gtk_window_set_default_size (GTK_WINDOW (window), 280, 250);
    }

  if (!GTK_WIDGET_VISIBLE (window))
    gtk_widget_show_all (window);
  else
    {
      gtk_widget_destroy (window);
      window = NULL;
    }

  return window;
}
#endif

void
row_activated_cb( GtkTreeView       *tree_view,
                  GtkTreePath       *path,
                  GtkTreeViewColumn *column,
                  gpointer           user_data)
{
	GtkTreeModel *model = gtk_tree_view_get_model( tree_view);
	GtkTreeIter iter;
	gboolean res;
	gint x, y;
	DrawBuffer *db = (DrawBuffer *)user_data;

	res = gtk_tree_model_get_iter( model, &iter, path);
	assert( res );
	gtk_tree_model_get( model, &iter,
						GLS_COLUMN_X, &x,
						GLS_COLUMN_Y, &y,
						-1);
	db->CenterVisibleAreaByVrgCoords( x, y);
}

void UiShowFindNodeDialog( UIController *uic)
{
	GtkWidget *dialog, *label;
	GtkWidget *entry;
	GtkEntryCompletion *completion;
	GtkWidget *vbox;
	GtkWidget *sw;
	GtkTreeModel *model;
	GtkWidget *treeview;

	dialog = gtk_dialog_new_with_buttons( "find node dialog", GTK_WINDOW(uic->m_MainWindow),
						GtkDialogFlags(/*GTK_DIALOG_MODAL|*/GTK_DIALOG_DESTROY_WITH_PARENT),
						GTK_STOCK_CLOSE, GTK_RESPONSE_NONE,
						NULL);
	g_signal_connect( dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
	g_signal_connect( dialog, "destroy", G_CALLBACK(gtk_widget_destroyed), &dialog);
	vbox = GTK_DIALOG(dialog)->vbox;

	/* Create our entry */
	entry = gtk_entry_new();
	gtk_box_pack_start( GTK_BOX(vbox), entry, FALSE, FALSE, 0);
	/* Create the completion object */
	completion = gtk_entry_completion_new ();
	/* Assign the completion to the entry */
	gtk_entry_set_completion (GTK_ENTRY (entry), completion);
	g_object_unref (completion);

	label = gtk_label_new( "Select node from the list bellow. Main window will ceterized by double click.");
	gtk_box_pack_start( GTK_BOX(vbox), label, FALSE, FALSE, 0);

	sw = gtk_scrolled_window_new( NULL, NULL);
	gtk_scrolled_window_set_shadow_type( GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(sw),
									GTK_POLICY_NEVER,
									GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start( GTK_BOX(vbox), sw, TRUE, TRUE, 0);

	/* create tree model */
	model = create_model( uic->m_VRGraph.get());

	/* create tree view */
	treeview = gtk_tree_view_new_with_model( model);
	gtk_tree_view_set_rules_hint( GTK_TREE_VIEW(treeview), TRUE);
	gtk_tree_view_set_search_column( GTK_TREE_VIEW(treeview), GLS_COLUMN_LABEL);
	g_signal_connect( treeview, "row-activated", G_CALLBACK(row_activated_cb), uic->m_DrawBuffer.get());

	gtk_container_add( GTK_CONTAINER(sw), treeview);

	/* add columns to the tree view */
	add_columns( GTK_TREE_VIEW(treeview));

	gtk_entry_completion_set_model( completion, model);
	/* Use model column 0 as the text column */
	gtk_entry_completion_set_text_column( completion, GLS_COLUMN_LABEL);

	g_object_unref( model);

	/* finish & show */
	//gtk_window_set_default_size( GTK_WINDOW(window), 280, 250);
	gtk_window_set_default_size( GTK_WINDOW(dialog), 280, 250);
    
	//gtk_widget_show_all( window);
	gtk_widget_show_all( dialog);

	/*int response = gtk_dialog_run( GTK_DIALOG(dialog));
	gtk_widget_destroy( dialog);
	if ( response == GTK_RESPONSE_YES )
	{
		printf("YES\n");
	} else
	{
		printf("~NO\n");
	}*/
	//gtk_widget_show( dialog);
}

