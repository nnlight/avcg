#include "ui.h"
#include "vr_graph.h"
#include "vcg/vcg_iface.h"

// коментарий на русском...

enum
{
	FND_COLUMN_TITLE,
	FND_COLUMN_LABEL,
	FND_COLUMN_X,
	FND_COLUMN_Y,
	FND_COLUMN_VISIBLE,
	FND_COLUMN_REF,
	FND_COLUMNS_COUNT
};

static GtkTreeModel *
create_model( VRGraph *vrg)
{
	GtkListStore *list_store;
	GtkTreeIter iter;

	/* create list store */
	list_store = gtk_list_store_new( FND_COLUMNS_COUNT,
									G_TYPE_STRING,
									G_TYPE_STRING,
									G_TYPE_INT,
									G_TYPE_INT,
									G_TYPE_BOOLEAN,
									G_TYPE_POINTER);

	/* add data to the list store */
	for ( GrNode *n = vrg->GrGetFirstNode();
		  n; 
		  n = n->GrGetNextNode() )
	{
		VRNode *node = static_cast<VRNode*>(n);
		assert( dynamic_cast<VRNode*>(n) );
		gtk_list_store_append( list_store, &iter);
		gtk_list_store_set( list_store, &iter,
						FND_COLUMN_TITLE, node->title_.c_str(),
						FND_COLUMN_LABEL, node->label_.c_str(),
						FND_COLUMN_X, node->x_ + node->width_ / 2,
						FND_COLUMN_Y, node->y_ + node->height_ / 2,
						FND_COLUMN_VISIBLE, TRUE,
						FND_COLUMN_REF, (gpointer)node,
						-1);
	}

	// сортируем список
	gtk_tree_sortable_set_sort_column_id( GTK_TREE_SORTABLE(list_store),
										  FND_COLUMN_LABEL,
										  GTK_SORT_ASCENDING);
	return GTK_TREE_MODEL( list_store);
}


static void
add_columns_to_treeview( GtkTreeView *treeview)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	/* column for node titles */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes( "Title",
									renderer,
									"text", FND_COLUMN_TITLE,
									NULL);
	/* set this column to a fixed sizing (of 50 pixels) */
	/*gtk_tree_view_column_set_sizing( column, GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_fixed_width( column, 50);*/
	//gtk_tree_view_column_set_resizable( column, TRUE);
	gtk_tree_view_column_set_sort_column_id( column, FND_COLUMN_TITLE);
	gtk_tree_view_append_column( treeview, column);

	/* column for node labels */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes( "Label",
									renderer,
									"text", FND_COLUMN_LABEL,
									NULL);
	gtk_tree_view_column_set_sort_column_id( column, FND_COLUMN_LABEL);
	gtk_tree_view_append_column( treeview, column);
}

static void
recalc_visibility_for_model( GtkTreeModel *model, const char *text)
{
	GtkTreeIter  iter;
	gboolean     valid;

	for ( valid = gtk_tree_model_get_iter_first( model, &iter);
		  valid;
		  valid = gtk_tree_model_iter_next( model , &iter) )
	{
		VRNode *node;
		gtk_tree_model_get( model, &iter, FND_COLUMN_REF, &node, -1);

		for ( int i = 0; i < VRNODE_INFO_COUNT; i++ )
		{
			if ( node->label_.find( text) != string::npos
				 || node->FindInfoNumBySubstring( text) )
			{
				gtk_list_store_set( GTK_LIST_STORE(model), &iter, FND_COLUMN_VISIBLE, TRUE, -1);
			} else
			{
				gtk_list_store_set( GTK_LIST_STORE(model), &iter, FND_COLUMN_VISIBLE, FALSE, -1);
			}
		}
	}
}

static void
entry_changed_cb( GtkEntry *entry,
                  gpointer  user_data)
{
	GtkWidget *treeview = (GtkWidget *)user_data;
	const char *text = gtk_entry_get_text( entry);

	GtkTreeModel *sort_model = gtk_tree_view_get_model( GTK_TREE_VIEW(treeview));
	GtkTreeModel *filter_model = gtk_tree_model_sort_get_model( GTK_TREE_MODEL_SORT(sort_model));
	GtkTreeModel *model = gtk_tree_model_filter_get_model( GTK_TREE_MODEL_FILTER(filter_model));

	recalc_visibility_for_model( model, text);
}

static void
entry_activate_cb( GtkEntry *entry,
                   gpointer  user_data)
{
	GtkWidget *treeview = (GtkWidget *)user_data;
	const char *text = gtk_entry_get_text( entry);

	GtkTreeModel *sort_model = gtk_tree_view_get_model( GTK_TREE_VIEW(treeview));
	GtkTreeModel *filter_model = gtk_tree_model_sort_get_model( GTK_TREE_MODEL_SORT(sort_model));
	GtkTreeModel *model = gtk_tree_model_filter_get_model( GTK_TREE_MODEL_FILTER(filter_model));

	recalc_visibility_for_model( model, text);

	gtk_widget_grab_focus( treeview);
}

static void
row_activated_cb( GtkTreeView       *tree_view,
                  GtkTreePath       *path,
                  GtkTreeViewColumn *column,
                  gpointer           user_data)
{
	DrawBuffer *db = (DrawBuffer *)user_data;
	GtkTreeModel *model = gtk_tree_view_get_model( tree_view);
	GtkTreeIter iter;
	gboolean res;
	gint x, y;

	res = gtk_tree_model_get_iter( model, &iter, path);
	assert( res );
	gtk_tree_model_get( model, &iter,
						FND_COLUMN_X, &x,
						FND_COLUMN_Y, &y,
						-1);
	db->CenterVisibleAreaByVrgCoords( x, y);
}

static gboolean
completion_match_selected_cb( GtkEntryCompletion *widget,
                              GtkTreeModel       *model,
                              GtkTreeIter        *iter,
                              gpointer            user_data)
{
	DrawBuffer *db = (DrawBuffer *)user_data;
	gint x, y;
	gtk_tree_model_get( model, iter,
						FND_COLUMN_X, &x,
						FND_COLUMN_Y, &y,
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


	//label = gtk_label_new( "Select node from the list bellow. Main window will ceterized by double click.");
	//gtk_box_pack_start( GTK_BOX(vbox), label, FALSE, FALSE, 0);

	/* Scrolled Window */
	sw = gtk_scrolled_window_new( NULL, NULL);
	gtk_scrolled_window_set_shadow_type( GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(sw),
									GTK_POLICY_NEVER,
									GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start( GTK_BOX(vbox), sw, TRUE, TRUE, 0);

	/* create tree model */
	model = create_model( uic->m_VRGraph.get());

	// фильтр
	GtkTreeModel *filter = gtk_tree_model_filter_new( model, NULL);
	gtk_tree_model_filter_set_visible_column( GTK_TREE_MODEL_FILTER(filter),
											  FND_COLUMN_VISIBLE);

	// модель для сортировки
	GtkTreeModel *sort = gtk_tree_model_sort_new_with_model( filter);
	// начальная сортировка
	gtk_tree_sortable_set_sort_column_id( GTK_TREE_SORTABLE(sort),
										  FND_COLUMN_LABEL,
										  GTK_SORT_ASCENDING);
	// это значит, что у этой модели не будет состояния `не отсортирована`
	gtk_tree_sortable_set_default_sort_func( GTK_TREE_SORTABLE(sort), NULL, NULL, NULL);

	/* create tree view */
	treeview = gtk_tree_view_new_with_model( sort);
	gtk_tree_view_set_rules_hint( GTK_TREE_VIEW(treeview), TRUE);
	//gtk_tree_view_set_grid_lines( GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_HORIZONTAL);
	gtk_tree_view_set_search_column( GTK_TREE_VIEW(treeview), -1);
	g_signal_connect( treeview, "row-activated", G_CALLBACK(row_activated_cb), uic->m_DrawBuffer.get());
	/* add columns to the tree view */
	add_columns_to_treeview( GTK_TREE_VIEW(treeview));
	
	gtk_container_add( GTK_CONTAINER(sw), treeview);


	/* добавляем модель к completion, указываем по какому столбцу работать */
	gtk_entry_completion_set_model( completion, model);
	gtk_entry_completion_set_text_column( completion, FND_COLUMN_LABEL);

	g_object_unref( model);
	g_object_unref( filter);
	g_object_unref( sort);


	g_signal_connect( entry, "changed", G_CALLBACK(entry_changed_cb), treeview);
	g_signal_connect( entry, "activate", G_CALLBACK(entry_activate_cb), treeview);

	g_signal_connect( completion, "match-selected", G_CALLBACK(completion_match_selected_cb),
				uic->m_DrawBuffer.get());

	/* finish & show */
	gtk_window_set_default_size( GTK_WINDOW(dialog), 440, 300);
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

