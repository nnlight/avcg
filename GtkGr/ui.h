#ifndef _UI_H_
#define _UI_H_

#include "stdafx.h"
#include <gtk/gtk.h>
#include "draw_buffer.h"



/**
 * Класс, заведеющий меню и хранящий ткущее состояние парметров программы
 */
class UIController
{
private:
	GtkUIManager *m_UIManager;
	std::auto_ptr<DrawBuffer> m_DrawBuffer;
	std::auto_ptr<VRGraph> m_VRGraph;
public:
	UIController( bool is_gdl_present);
	~UIController(void);
	
	/* цикл обработки сообщений */
	void MainLoop() { gtk_main(); };

private:
	GtkWidget *ConstrMenubar( GtkWidget *main_window);
private:
	friend gint ui_key_press_cb( GtkWidget* widget, GdkEventKey* event, gpointer data);
	friend gboolean ui_da_configure_event_cb( GtkWidget *da, GdkEventConfigure *event, gpointer data);
	friend gboolean ui_da_expose_event_cb( GtkWidget *da, GdkEventExpose *event, gpointer data);
	friend gboolean ui_da_button_press_event_cb( GtkWidget *da, GdkEventButton *event, gpointer data);
	friend gboolean ui_da_motion_notify_event_cb( GtkWidget *da, GdkEventMotion *event, gpointer data);
};

#endif /* _UI_H_ */
