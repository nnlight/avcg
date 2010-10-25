#include "mw_duck.h"

/**
 * Реализация собственного виджета MwDuck
 */



enum {
  TICTACTOE_SIGNAL,
  LAST_SIGNAL
};

static void mw_duck_class_init          (MwDuckClass *klass);
static void mw_duck_init                (MwDuck      *duck);
static void mw_duck_toggle              (GtkWidget *widget, MwDuck *ttt);

static guint tictactoe_signals[LAST_SIGNAL] = { 0 };

GType
mw_duck_get_type (void)
{
	static GType duck_type = 0;

	if (!duck_type)
	{
		const GTypeInfo duck_info =
			{
				sizeof (MwDuckClass),
				NULL, /* base_init */
				NULL, /* base_finalize */
				(GClassInitFunc) mw_duck_class_init,
				NULL, /* class_finalize */
				NULL, /* class_data */
				sizeof (MwDuck),
				0,
				(GInstanceInitFunc) mw_duck_init,
			};

		duck_type = g_type_register_static (GTK_TYPE_CONTAINER, "MwDuck", &duck_info, 0);
	}

	return duck_type;
}

static void
mw_duck_class_init (MwDuckClass *klass)
{
  
  tictactoe_signals[TICTACTOE_SIGNAL] = g_signal_new ("tictactoe",
					 G_TYPE_FROM_CLASS (klass),
	                                 G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
	                                 G_STRUCT_OFFSET (MwDuckClass, tictactoe),
                                         NULL, 
                                         NULL,                
					 g_cclosure_marshal_VOID__VOID,
                                         G_TYPE_NONE, 0);


}

static void
mw_duck_init (MwDuck *duck)
{
 /* gint i,j;
  
  gtk_table_resize (GTK_TABLE (ttt), 3, 3);
  gtk_table_set_homogeneous (GTK_TABLE (ttt), TRUE);

  for (i=0;i<3; i++)
    for (j=0;j<3; j++)      {
	ttt->buttons[i][j] = gtk_toggle_button_new ();
	gtk_table_attach_defaults (GTK_TABLE (ttt), ttt->buttons[i][j], 
				   i, i+1, j, j+1);
	g_signal_connect (ttt->buttons[i][j], "toggled",
			  G_CALLBACK (tictactoe_toggle), (gpointer) ttt);
	gtk_widget_set_size_request (ttt->buttons[i][j], 20, 20);
	gtk_widget_show (ttt->buttons[i][j]);
      }*/
	duck->bin_window = NULL;
	duck->private_int= 0;
}

GtkWidget*
mw_duck_new ()
{
	return GTK_WIDGET (g_object_new (MW_TYPE_DUCK, NULL));
}

void	       
mw_duck_clear (MwDuck *ttt)
{
  int i,j;

  for (i = 0; i<3; i++)
    for (j = 0; j<3; j++)
      {
	g_signal_handlers_block_matched (G_OBJECT (ttt->buttons[i][j]), 
                                         G_SIGNAL_MATCH_DATA,
                                         0, 0, NULL, NULL, ttt);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ttt->buttons[i][j]),
				      FALSE);
	g_signal_handlers_unblock_matched (G_OBJECT (ttt->buttons[i][j]),
                                           G_SIGNAL_MATCH_DATA,
                                           0, 0, NULL, NULL, ttt);
      }
}


static void
mw_duck_toggle (GtkWidget *widget, MwDuck *ttt)
{
  int i,k;

  static int rwins[8][3] = { { 0, 0, 0 }, { 1, 1, 1 }, { 2, 2, 2 },
			     { 0, 1, 2 }, { 0, 1, 2 }, { 0, 1, 2 },
			     { 0, 1, 2 }, { 0, 1, 2 } };
  static int cwins[8][3] = { { 0, 1, 2 }, { 0, 1, 2 }, { 0, 1, 2 },
			     { 0, 0, 0 }, { 1, 1, 1 }, { 2, 2, 2 },
			     { 0, 1, 2 }, { 2, 1, 0 } };

  int success, found;

  for (k = 0; k<8; k++)
    {
      success = TRUE;
      found = FALSE;

      for (i = 0; i<3; i++)
	{
	  success = success && 
	    GTK_TOGGLE_BUTTON (ttt->buttons[rwins[k][i]][cwins[k][i]])->active;
	  found = found ||
	    ttt->buttons[rwins[k][i]][cwins[k][i]] == widget;
	}
      
      if (success && found)
	{
	  g_signal_emit (ttt, 
	                 tictactoe_signals[TICTACTOE_SIGNAL], 0);
	  break;
	}
    }
}

