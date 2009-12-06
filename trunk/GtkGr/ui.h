#ifndef _UI_H_
#define _UI_H_

#include "stdafx.h"
#include <gtk/gtk.h>
#include "draw_buffer.h"


/**
 * Режим, в одном из которых может находится в данный момент ui
 */
enum Mode_t
{
  MODE_ADD_NODE,	// режим добавления узлов
  MODE_ADD_EDGE,	// режим добавления дуг
  MODE_VIEW			// режим просмотра
};

/**
 * Класс, заведеющий меню и хранящий ткущее состояние парметров программы
 */
class UIController
{
private:
	GtkUIManager *m_UIManager;
	std::auto_ptr<DrawBuffer> m_DrawBuffer;
	std::auto_ptr<VRGraph> m_VRGraph;
	Mode_t m_CurrentMode;
	GtkWidget *m_Toolbar;
	GtkWidget *m_Statusbar;
public:
	UIController( bool is_gdl_present);
	~UIController(void);
	
	/* цикл обработки сообщений */
	void MainLoop() { gtk_main(); };

private:
	GtkWidget *ConstrMenubar( GtkWidget *main_window);
private:
	friend gboolean ui_key_press_cb( GtkWidget* widget, GdkEventKey* event, gpointer data);
	friend gboolean ui_da_configure_event_cb( GtkWidget *da, GdkEventConfigure *event, gpointer data);
	friend gboolean ui_da_expose_event_cb( GtkWidget *da, GdkEventExpose *event, gpointer data);
	friend gboolean ui_da_button_press_event_cb( GtkWidget *da, GdkEventButton *event, gpointer data);
	friend gboolean ui_da_scroll_event_cb( GtkWidget *da, GdkEventScroll *event, gpointer data);
	friend gboolean ui_da_motion_notify_event_cb( GtkWidget *da, GdkEventMotion *event, gpointer data);
	friend void ui_activate_radio_action_mode( GtkAction *action, GtkRadioAction *current, gpointer data);
};

#endif /* _UI_H_ */
