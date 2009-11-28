#ifndef _UI_H_
#define _UI_H_

#include "stdafx.h"
#include <gtk/gtk.h>


/**
 * Класс, заведеющий меню и хранящий ткущее состояние парметров программы
 */
class UIController
{
private:
	GtkWidget *m_Menubar;
public:
	UIController( GtkWidget *main_window);
	~UIController(void);

	GtkWidget *GetMenubar() { return m_Menubar; };
};

#endif /* _UI_H_ */
