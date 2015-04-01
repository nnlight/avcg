#include "ui.h"
#include "vr_graph.h"
#include "vcg/vcg_iface.h"

// коментарий на русском...

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

	// сортируем список
	gtk_tree_sortable_set_sort_column_id( GTK_TREE_SORTABLE(store),
										  GLS_COLUMN_LABEL,
										  GTK_SORT_ASCENDING/*GTK_SORT_DESCENDING*/);
	return GTK_TREE_MODEL( store);
}


static void
add_columns( GtkTreeView *treeview)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	//GtkTreeModel *model = gtk_tree_view_get_model( treeview);

	/* column for node titles */
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
	//gtk_tree_view_column_set_sort_column_id( column, GLS_COLUMN_TITLE);
	gtk_tree_view_append_column( treeview, column);

	/* column for node labels */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes( "Label",
									renderer,
									"text", GLS_COLUMN_LABEL,
									NULL);
	gtk_tree_view_column_set_sort_column_id( column, GLS_COLUMN_LABEL);
	gtk_tree_view_append_column( treeview, column);
}

/*
static GtkWidget
create_tree_view_widget(GtkTreeModel model)
{
	GtkWidget *sw;
	GtkTreeModel *model;
	GtkWidget *treeview;

	sw = gtk_scrolled_window_new( NULL, NULL);
	gtk_scrolled_window_set_shadow_type( GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(sw),
									GTK_POLICY_NEVER,
									GTK_POLICY_AUTOMATIC);


	treeview = gtk_tree_view_new_with_model( model);
	gtk_tree_view_set_rules_hint( GTK_TREE_VIEW(treeview), TRUE);
	gtk_tree_view_set_search_column( GTK_TREE_VIEW(treeview), GLS_COLUMN_LABEL);
	g_signal_connect( treeview, "row-activated", G_CALLBACK(row_activated_cb), uic->m_DrawBuffer.get());

	gtk_container_add( GTK_CONTAINER(sw), treeview);

	// add columns to the tree view
	add_columns( GTK_TREE_VIEW(treeview));

}
*/

static void
entry_changed_cb( GtkEntry *entry,
                  gpointer  user_data)
{
	const char *text = gtk_entry_get_text( entry);
	printf( "(change)entry text: %s\n", text);
}

static void
entry_activate_cb( GtkEntry *entry,
                   gpointer  user_data)
{
	const char *text = gtk_entry_get_text( entry);
	printf( "(activate)entry text: %s\n", text);
	GtkWidget *treeview = (GtkWidget *)user_data;

	gtk_widget_grab_focus( treeview);
}

static void
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

static gboolean
cursor_on_match_cb(GtkEntryCompletion *widget,
               GtkTreeModel       *model,
               GtkTreeIter        *iter,
               gpointer            user_data)
{
	printf("cursor_on_match_cb\n");
	return FALSE;
}

static gboolean
match_selected_cb(GtkEntryCompletion *widget,
               GtkTreeModel       *model,
               GtkTreeIter        *iter,
               gpointer            user_data)
{
	//printf("match_selected_cb\n");
	DrawBuffer *db = (DrawBuffer *)user_data;
	gint x, y;
	gtk_tree_model_get( model, iter,
						GLS_COLUMN_X, &x,
						GLS_COLUMN_Y, &y,
						-1);
	db->CenterVisibleAreaByVrgCoords( x, y);
	// возвращаем FALSE(сигнал не обработан), что бы еще действие по умолчанию отработало
	return FALSE;
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

	/* Create entry with completion */
	entry = gtk_entry_new();
	gtk_box_pack_start( GTK_BOX(vbox), entry, FALSE, FALSE, 0);
	completion = gtk_entry_completion_new();
	gtk_entry_set_completion( GTK_ENTRY(entry), completion);
	g_object_unref( completion);


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

	/* добавляем модель к completion, указываем по какому столбцу работать */
	gtk_entry_completion_set_model( completion, model);
	gtk_entry_completion_set_text_column( completion, GLS_COLUMN_LABEL);

	g_object_unref( model);


	/*g_signal_connect( entry, "changed", G_CALLBACK(entry_changed_cb), NULL);
	g_signal_connect( entry, "activate", G_CALLBACK(entry_activate_cb), treeview);

	g_signal_connect( completion, "cursor-on-match", G_CALLBACK(cursor_on_match_cb), NULL);*/
	g_signal_connect( completion, "match-selected", G_CALLBACK(match_selected_cb), uic->m_DrawBuffer.get());

	/* finish & show */
	gtk_window_set_default_size( GTK_WINDOW(dialog), 280, 250);
	//gtk_window_set_default_size( GTK_WINDOW(dialog), 450, 500);
    
	gtk_widget_show_all( dialog);

	/*int response = gtk_dialog_run( GTK_DIALOG(dialog));
	//gtk_widget_destroy( dialog);
	if ( response == GTK_RESPONSE_YES )
	{
		printf("YES\n");
	} else
	{
		printf("~NO\n");
	}*/
}

