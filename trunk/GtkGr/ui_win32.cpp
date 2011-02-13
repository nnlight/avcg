
/**
 * Этот файл полностью только для win32
 */
#ifdef WIN32

#define _WIN32_WINNT 0x0502
#include <windows.h>
#include <winuser.h>

#include "ui.h"
#include <gdk/gdk.h>
#include <gdk/gdkwin32.h>


/**
 * Эта функция точный copy-paste функции build_pointer_event_state
 * из gtk+-2.16.6/gdk/win32/gdkevents-win32.c 
 */
static gint
ui_win32_build_pointer_event_state( MSG *msg)
{
	gint state;

	state = 0;

	if (msg->wParam & MK_CONTROL)
		state |= GDK_CONTROL_MASK;

	if ((msg->message != WM_LBUTTONDOWN &&
		(msg->wParam & MK_LBUTTON)) ||
		msg->message == WM_LBUTTONUP)
		state |= GDK_BUTTON1_MASK;

	if ((msg->message != WM_MBUTTONDOWN &&
		(msg->wParam & MK_MBUTTON)) ||
		msg->message == WM_MBUTTONUP)
		state |= GDK_BUTTON2_MASK;

	if ((msg->message != WM_RBUTTONDOWN &&
		(msg->wParam & MK_RBUTTON)) ||
		msg->message == WM_RBUTTONUP)
		state |= GDK_BUTTON3_MASK;

	if (((msg->message != WM_XBUTTONDOWN || HIWORD (msg->wParam) != XBUTTON1) &&
		(msg->wParam & MK_XBUTTON1)) ||
		(msg->message == WM_XBUTTONUP && HIWORD (msg->wParam) == XBUTTON1))
		state |= GDK_BUTTON4_MASK;

	if (((msg->message != WM_XBUTTONDOWN || HIWORD (msg->wParam) != XBUTTON2) &&
		(msg->wParam & MK_XBUTTON2)) ||
		(msg->message == WM_XBUTTONUP && HIWORD (msg->wParam) == XBUTTON2))
		state |= GDK_BUTTON5_MASK;

	if (msg->wParam & MK_SHIFT)
		state |= GDK_SHIFT_MASK;

	if (GetKeyState (VK_MENU) < 0)
		state |= GDK_MOD1_MASK;

	if (GetKeyState (VK_CAPITAL) & 0x1)
		state |= GDK_LOCK_MASK;

	return state;
} /* ui_win32_build_pointer_event_state */


/**
 * Фильтр приходящих из win32 сообщений.
 * Используется из-за того, что win32 кслевивает сообщения WM_MOUSEWHEEL.
 * И в обработчике сигналов по пришедшему GdkEventScroll невозможно
 * понять, сколько сообщений было склеено.
 * В данной функции по приходящему WM_MOUSEWHEEL генерится необходимое кол-во
 * GdkEventScroll'ов.
 */
GdkFilterReturn
ui_win32_main_window_filter( GdkXEvent *xevent,
                             GdkEvent *unused_event,
                             gpointer data)
{
	MSG *msg = (MSG *)xevent;
	GdkWindow *da_window = (GdkWindow *)data;

	/* интересуют только scroll-сообщения */
	if ( msg->message != WM_MOUSEWHEEL )
	{
		return GDK_FILTER_CONTINUE;
	}

	unsigned fwKeys = GET_KEYSTATE_WPARAM(msg->wParam);
	int zDelta = GET_WHEEL_DELTA_WPARAM(msg->wParam);
	int n_scrolls = abs(zDelta) / WHEEL_DELTA;
	GdkScrollDirection direct = zDelta > 0 ? GDK_SCROLL_UP : GDK_SCROLL_DOWN;
	int xPos = /*GET_X_LPARAM*/LOWORD(msg->lParam); 
	int yPos = /*GET_Y_LPARAM*/HIWORD(msg->lParam); 
	int x2 = msg->pt.x;
	int y2 = msg->pt.y;

	POINT point;
	point.x = xPos;
	point.y = yPos;
	HWND hwnd = WindowFromPoint( point);
	if ( hwnd == NULL )
	{
		return GDK_FILTER_CONTINUE;
	}
	GdkWindow *new_window = (GdkWindow *)gdk_win32_handle_table_lookup( (GdkNativeWindow)hwnd);
	if ( new_window == NULL || new_window != da_window )
	{
		return GDK_FILTER_CONTINUE;
	}
	if( GDK_WINDOW_DESTROYED( da_window) )
	{
		return GDK_FILTER_CONTINUE;
	}

	//printf( "--WM_MOUSEWHEEL(%d %d)  (%d %d) fwKeys=%d, time=%d {\n", xPos, yPos, x2, y2, fwKeys, msg->time);

	int da_x_root, da_y_root;
	gdk_window_get_root_origin( da_window, &da_x_root, &da_y_root);

	GdkEvent gdkevent_s;
	memset( &gdkevent_s, 0, sizeof(GdkEvent));
	GdkEvent *ev = //gdk_event_new(GDK_SCROLL);
					&gdkevent_s;
	GdkEventScroll *sev = &ev->scroll;

	sev->type = GDK_SCROLL;
	//sev->window = (GdkWindow *)g_object_ref( da_window);
	sev->window = da_window;
	sev->state = ui_win32_build_pointer_event_state( msg);
	sev->time = msg->time;
	sev->direction = direct;
	sev->x_root = xPos;
	sev->y_root = yPos;
	//sev->x = 100;
	//sev->y = 100;
	sev->x = xPos - da_x_root;
	sev->y = yPos - da_y_root;

	for ( int i=0; i < n_scrolls ; i++ )
	{
		gtk_main_do_event( ev);
		//gdk_event_put( ev);
	}
	//gdk_event_free( ev);
	//printf("}\n");

	return GDK_FILTER_REMOVE;
} /* ui_win32_main_window_filter */


#endif /* WIN32 */