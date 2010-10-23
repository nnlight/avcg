#ifndef _DUCK_H_
#define _DUCK_H_

#include <gtk/gtk.h>


/**
 * Режим, в одном из которых может находится в данный момент ui
 */
enum _Mode_t
{
  _MODE_ADD_NODE,	// режим добавления узлов
  _MODE_ADD_EDGE,	// режим добавления дуг
  _MODE_VIEW,		// режим просмотра
  _MODE_VIEW_NODE_INFO1,
  _MODE_VIEW_NODE_INFO2,
  _MODE_VIEW_NODE_INFO3
};

#endif /* _DUCK_H_ */
