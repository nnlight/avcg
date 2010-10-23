#include "duck.h"



/**
 * Обработчик сигнала закрытия главного окна приложения
 */
static gboolean 
ui_close_cb( GtkWindow *window, gpointer data)
{
	GtkWidget *dialog, *label;

#if 0
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
#endif
} /* ui_close_cb */

