#ifndef _DUCK_H_
#define _DUCK_H_

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

/**
 * Реализация собственного виджета MwDuck
 */

#define MW_TYPE_DUCK            (mw_duck_get_type ())
#define MW_DUCK(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), MW_TYPE_DUCK, MwDuck))
#define MW_DUCK_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), MW_TYPE_DUCK, MwDuckClass))
#define MW_IS_DUCK(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MW_TYPE_DUCK))
#define MW_IS_DUCK_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MW_TYPE_DUCK))
#define MW_DUCK_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_DUCK, MwDuckClass))

typedef struct _MwDuck        MwDuck;
typedef struct _MwDuckClass   MwDuckClass;

struct _MwDuck
{
  GtkContainer container;
  /*GtkContainer GSEAL (container);*/
  /*GtkTable table;*/
  
  /*< public >*/
  GdkWindow *bin_window;
  /*GdkWindow *GSEAL (bin_window);*/

  GtkWidget *buttons[3][3];
  /*< private >*/
  int private_int;
};

struct _MwDuckClass
{
  GtkContainerClass parent_class;

  void (* tictactoe) (MwDuck *ttt);
};

GType          mw_duck_get_type        (void);
GtkWidget*     mw_duck_new             (void);
void	       mw_duck_clear           (MwDuck *duck);


G_END_DECLS

#endif /* _DUCK_H_ */
