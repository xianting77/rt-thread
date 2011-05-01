/*
 * File      : window.c
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-04     Bernard      first version
 */
#include <rtgui/dc.h>
#include <rtgui/color.h>
#include <rtgui/image.h>
#include <rtgui/rtgui_system.h>
#include <panel.h>
#include <rtgui/widgets/window.h>
#include <rtgui/widgets/button.h>
#include <rtgui/rtgui_theme.h>

static void _rtgui_win_constructor(rtgui_win_t *win)
{
	/* init window attribute */
	win->title			= RT_NULL;
	win->image			= RT_NULL;
	win->level			= RTGUI_WIN_LEVEL_NORMAL;
	win->on_activate	= RT_NULL;
	win->on_deactivate  = RT_NULL;
	win->on_close		= RT_NULL;

	win->modal_widget	= RT_NULL;
	win->tid			= RT_NULL;

	/* set window hide */
	RTGUI_WIDGET_HIDE(win);

	RTGUI_WIDGET_FLAG(win) |= RTGUI_WIDGET_FLAG_FOCUSABLE;
	RTGUI_WIDGET_BACKGROUND(win) = default_background;

	/* set window status and style */
	win->status  = 0;
	win->style = RTGUI_WIN_DEFAULT;
	rtgui_widget_set_event_handler(win, rtgui_win_event_handler);
}

static void _rtgui_win_destructor(rtgui_win_t* win)
{
	rtgui_event_win_t ewin;

	/* remove win from box */
	if(RTGUI_WIDGET_PARENT(win) != RT_NULL)
	{
		rtgui_container_t *box;

		box = RTGUI_CONTAINER(RTGUI_WIDGET_PARENT(win));
		rtgui_container_remove_child(box, win);
	}

	/* destroy in server */
	RTGUI_EVENT_WIN_DESTROY_INIT(&ewin);
	ewin.wid = win;
	if(rtgui_thread_send_sync(rtgui_thread_get_server(), RTGUI_EVENT(&ewin),sizeof(rtgui_event_win_t)) != RT_EOK)
	{
		return;	 /* destroy in server failed */
	}
		
	/* release field */
	rt_free(win->title);
	rt_free(win->image);
}

rtgui_type_t *rtgui_win_type_get(void)
{
	static rtgui_type_t *win_type = RT_NULL;

	if(!win_type)
	{
		win_type = rtgui_type_create("win", RTGUI_CONTAINER_TYPE,
			sizeof(rtgui_win_t),
			RTGUI_CONSTRUCTOR(_rtgui_win_constructor),
			RTGUI_DESTRUCTOR(_rtgui_win_destructor));
	}

	return win_type;
}

rtgui_win_t* rtgui_win_create(PVOID parent, const char* title, rtgui_rect_t *rect, rt_uint32_t style)
{
	rtgui_win_t* win;
	/* get GUI server thread */
	struct rt_thread* server = rtgui_thread_get_server();
	rtgui_thread_t* thread;

	if(parent == RT_NULL) parent = RTGUI_CONTAINER(rtgui_panel_get());

	RT_ASSERT(parent != RT_NULL);
	if(server == RT_NULL) return RT_NULL;

	win = rtgui_widget_create(RTGUI_WIN_TYPE);
	
	if(win != RT_NULL)
	{
		/* set window title */
		if(title != RT_NULL) 
			win->title = rt_strdup(title);
		else 
			win->title = RT_NULL;

		win->style = style;

		/* set default size */
		win->title_height = RTGUI_WIN_TITLE_HEIGHT;
		win->status_height= RTGUI_WIN_STATUS_HEIGHT;
		win->menu_height  = RTGUI_WIN_MENU_HEIGHT;
		
		RTGUI_WIDGET(win)->toplevel = RTGUI_WIDGET(win); /* window is a toplevel widget */
		/* set extent of win */
		rtgui_widget_set_rect(win, rect); 
		rtgui_container_add_child(parent, win);

		thread = rtgui_thread_self();
		win->tid = thread->tid;/* use current thread */
		
		rtgui_thread_set_widget(RTGUI_WIDGET(win));

		{/* send a create window event. */
			rtgui_event_win_t ewin;
			RTGUI_EVENT_WIN_CREATE_INIT(&ewin);
			ewin.wid = win;

			if(rtgui_thread_send_sync(server, RTGUI_EVENT(&ewin),sizeof(rtgui_event_win_t)) != RT_EOK)
			{	
				rt_kprintf("create win: %s failed\n", win->title);
				return RT_NULL;
			}
		}
	}

	return win;
}

void rtgui_win_destroy(rtgui_win_t* win)
{
	if(RTGUI_WIN_IS_MODAL_MODE(win))
	{	
		/* end modal */
		rtgui_win_end_modal(win);
	}
	
	rtgui_widget_destroy(win);
}

/* get widget's container window */
rtgui_win_t* rtgui_win_get_win_by_widget(PVOID wdt)
{
	rtgui_widget_t *parent=(rtgui_widget_t*)wdt;

	if(RTGUI_IS_WIN(parent))
		return (rtgui_win_t*)parent;

	while(parent->parent != RT_NULL)
	{
		if(RTGUI_IS_WIN(parent))break;
		parent = parent->parent;
	}
	if(RTGUI_IS_WIN(parent))
		return (rtgui_win_t*)parent;

	return RT_NULL;
}

/* window close event handler */
void rtgui_win_close(PVOID wdt, rtgui_event_t *event)
{
	rtgui_win_t* win = RT_NULL;

	if(RTGUI_IS_WIN(wdt))
		win = RTGUI_WIN(wdt);
	else
		win = rtgui_win_get_win_by_widget(wdt);

	if(win != RT_NULL)
	{
		rtgui_panel_t *panel=rtgui_panel_get();
		/* set window status is closed. */
		win->status |= RTGUI_WIN_STATUS_CLOSED;
		RTGUI_CONTAINER(panel)->focused	= RT_NULL;

		rtgui_win_destroy(win);
	}
}

void rtgui_win_show(rtgui_win_t* win, rt_bool_t modal)
{
	
	RT_ASSERT(win != RT_NULL);

	if(RTGUI_WIDGET_IS_HIDE(win))
	{	
		/* send show message to server */
		rtgui_event_win_t ewin;
		RTGUI_EVENT_WIN_SHOW_INIT(&ewin);
		ewin.wid = win;

		if(rtgui_thread_send_sync(rtgui_thread_get_server(), RTGUI_EVENT(&ewin),sizeof(rtgui_event_win_t)) != RT_EOK)
		{
			return;
		}
	}
	else rtgui_widget_update(win);

	if(modal == RT_TRUE)
	{/* display by modal. */
		rtgui_widget_t *parent_widget = RTGUI_WIDGET_PARENT(win);
		
		if(parent_widget != RT_NULL)
		{
			/* set style */
			win->status |= RTGUI_WIN_STATUS_MODAL;

			if(RTGUI_IS_PANEL(parent_widget))
			{
				rtgui_panel_t* panel;
				panel = RTGUI_PANEL(parent_widget);
				panel->status |= RTGUI_PANEL_STATUS_MODAL;
				panel->modal_widget = win;
			}
			else if(RTGUI_IS_WIN(parent_widget))
			{
				rtgui_win_t* pwin;
				pwin = RTGUI_WIN(parent_widget);
				pwin->status |= RTGUI_WIN_STATUS_MODAL;
				pwin->modal_widget = win;
			}
		}
		else
		{
			/* which is a top window */
			win->status |= RTGUI_WIN_STATUS_MODAL;
			rtgui_win_event_loop(win);

			win->status &= ~RTGUI_WIN_STATUS_MODAL;
		}
	}
}

void rtgui_win_end_modal(rtgui_win_t* win)
{
	rtgui_widget_t* parent_widget;

	RT_ASSERT(win != RT_NULL);	

	parent_widget = RTGUI_WIDGET_PARENT(win);

	if(parent_widget != RT_NULL)
	{
		if(RTGUI_IS_PANEL(parent_widget))
		{
			rtgui_panel_t* panel;
			
			/* which is shown under workbench */
			panel = RTGUI_PANEL(parent_widget);
			panel->status &= ~RTGUI_PANEL_STATUS_MODAL;
			panel->modal_widget = RT_NULL;
		}
		else if(RTGUI_IS_WIN(parent_widget))
		{
			rtgui_win_t* pwin;
			
			/* which is shown under win */
			pwin = RTGUI_WIN(parent_widget);
			pwin->status &= ~RTGUI_WIN_STATUS_MODAL;
			pwin->modal_widget = RT_NULL;	
		}
	}
	else
	{
		/* which is a stand alone window */
	}

	/* remove modal mode */
	win->status &= ~RTGUI_WIN_STATUS_MODAL;
}

void rtgui_win_hide(rtgui_win_t* win)
{
	RT_ASSERT(win != RT_NULL);

	if(!RTGUI_WIDGET_IS_HIDE(win) && rtgui_thread_get_server() != RT_NULL)
	{
		/* send hidden message to server */
		rtgui_event_win_t ewin;
		RTGUI_EVENT_WIN_HIDE_INIT(&ewin);
		ewin.wid = win;

		if(rtgui_thread_send_sync(rtgui_thread_get_server(), RTGUI_EVENT(&ewin),sizeof(rtgui_event_win_t)) != RT_EOK)
		{
			rt_kprintf("hide win: %s failed\n", win->title);
			return;
		}

		/* set window hide and deactivated */
		RTGUI_WIDGET_HIDE(win);
		win->status &= ~RTGUI_WIN_STATUS_ACTIVATE;
	}
}

rt_bool_t rtgui_win_is_activated(rtgui_win_t* win)
{
	RT_ASSERT(win != RT_NULL);

	return win->status & RTGUI_WIN_STATUS_ACTIVATE;
}

void rtgui_win_move(rtgui_win_t* win, int x, int y)
{
	rtgui_event_win_move_t emove;
	RTGUI_EVENT_WIN_MOVE_INIT(&emove);

	if (win == RT_NULL) return;

	emove.wid 	= win;
	emove.x		= x;
	emove.y		= y;
	if (rtgui_thread_send_sync(rtgui_thread_get_server(), RTGUI_EVENT(&emove),
		sizeof(rtgui_event_win_move_t)) != RT_EOK)
	{
		return;
	}
}

rt_bool_t rtgui_win_ondraw(rtgui_win_t* win)
{
	rtgui_event_paint_t event;

	rtgui_theme_draw_win(win);

	/* paint each widget */
	RTGUI_EVENT_PAINT_INIT(&event);
	event.wid = RT_NULL;
	rtgui_container_dispatch_event(RTGUI_CONTAINER(win), (rtgui_event_t*)&event);

	return RT_FALSE;
}

rt_bool_t rtgui_win_event_handler(PVOID wdt, rtgui_event_t* event)
{
	rtgui_widget_t *widget = (rtgui_widget_t*)wdt;
	rtgui_win_t* win = (rtgui_win_t*)widget;

	RT_ASSERT((win != RT_NULL) && (event != RT_NULL));

	switch (event->type)
	{
		case RTGUI_EVENT_WIN_CLOSE:
			if(win->on_close != RT_NULL)
			{
				if(win->on_close(widget, event) == RT_FALSE) return RT_TRUE;
			}
		
			if(RTGUI_WIN_IS_MODAL_MODE(win))
			{
				rtgui_win_destroy(win);
			}
			else
			{	
				rtgui_win_close(win,RT_NULL);
			}
	
			/* exit event loop */
			return RT_TRUE;

		case RTGUI_EVENT_WIN_MAX:
			/* TODO: Place code here. */
			break;

		case RTGUI_EVENT_WIN_MIN:
			/* TODO: Place code here. */
			break;

		case RTGUI_EVENT_WIN_MOVE: 
			{
				rtgui_event_win_move_t* emove = (rtgui_event_win_move_t*)event;
				/* move window */
				rtgui_win_move(win, emove->x, emove->y);
			}
			break;

		case RTGUI_EVENT_WIN_ACTIVATE:
			break;
	
		case RTGUI_EVENT_WIN_DEACTIVATE:
			break;	
		case RTGUI_EVENT_PAINT:
			if(widget->on_draw != RT_NULL) widget->on_draw(widget, event);
			else
				rtgui_win_ondraw(win);
			break;
	
		case RTGUI_EVENT_MOUSE_BUTTON:
			if(RTGUI_WIN_IS_MODAL_MODE(win))
			{
				if(win->modal_widget != RT_NULL)
					return RTGUI_WIDGET_EVENT_CALL(win->modal_widget, event);
			}
	 		
			if(rtgui_container_dispatch_mouse_event(RTGUI_CONTAINER(win),(rtgui_event_mouse_t*)event) == RT_FALSE)
			{/* dispatch event */
				if(widget->on_mouseclick != RT_NULL)
				{
					return widget->on_mouseclick(widget, event);
				}
			}
			break;
	
		case RTGUI_EVENT_MOUSE_MOTION:
			if (RTGUI_WIN_IS_MODAL_MODE(win))
			{
				if (win->modal_widget != RT_NULL)
					return RTGUI_WIDGET_EVENT_CALL(win->modal_widget, event);
			}
	 		
			rtgui_container_dispatch_mouse_event(RTGUI_CONTAINER(win),(rtgui_event_mouse_t*)event);
			break;

	    case RTGUI_EVENT_KBD:
			if(RTGUI_WIN_IS_MODAL_MODE(win))
			{
				if(win->modal_widget != RT_NULL)
					return RTGUI_WIDGET_EVENT_CALL(win->modal_widget, event);
			}

			if(RTGUI_CONTAINER(win)->focused != widget)
			{
				RTGUI_WIDGET_EVENT_CALL(RTGUI_CONTAINER(win)->focused, event);
			}
	        break;

		default:
			//call parent widget event
			return rtgui_container_event_handler(wdt,event);
	}

	return RT_FALSE;
}

/* windows event loop */
void rtgui_win_event_loop(rtgui_win_t* win)
{
	rt_err_t result;
	rtgui_thread_t* tid;
	rtgui_event_t* event;

	tid = rtgui_thread_self();
	RT_ASSERT(tid != RT_NULL);

	/* point to event buffer */
	event = (rtgui_event_t*)tid->event_buffer;

	if(RTGUI_WIN_IS_MODAL_MODE(win))
	{
		while(RTGUI_WIN_IS_MODAL_MODE(win))
		{
			if (tid->on_idle != RT_NULL)
			{
				result = rtgui_thread_recv_nosuspend(event, RTGUI_EVENT_BUFFER_SIZE);
				if (result == RT_EOK)
				{
					/* perform event handler */
					RTGUI_WIDGET_EVENT_CALL(win, event);
				}
				else if (result == -RT_ETIMEOUT)
				{
					tid->on_idle(win, RT_NULL);
				}
			}
			else
			{
				result = rtgui_thread_recv(event, RTGUI_EVENT_BUFFER_SIZE);
				if (result == RT_EOK)
				{
					/* perform event handler */
					RTGUI_WIDGET_EVENT_CALL(win, event);
				}
			}
		}
	}
	else
	{
		while(!(win->status & RTGUI_WIN_STATUS_CLOSED))
		{
			if (tid->on_idle != RT_NULL)
			{
				result = rtgui_thread_recv_nosuspend(event, RTGUI_EVENT_BUFFER_SIZE);
				if (result == RT_EOK)
				{
					/* perform event handler */
					RTGUI_WIDGET_EVENT_CALL(win, event);
				}
				else if (result == -RT_ETIMEOUT)
				{
					tid->on_idle(win, RT_NULL);
				}
			}
			else
			{
				result = rtgui_thread_recv(event, RTGUI_EVENT_BUFFER_SIZE);
				if (result == RT_EOK)
				{
					/* perform event handler */
					RTGUI_WIDGET_EVENT_CALL(win, event);
				}
			}
		}
	}
}

void rtgui_win_set_rect(rtgui_win_t* win, rtgui_rect_t* rect)
{
	rtgui_event_win_resize_t event;

	if(win == RT_NULL || rect == RT_NULL) return;

	RTGUI_WIDGET(win)->extent = *rect;

	if(rtgui_thread_get_server() != RT_NULL)
	{
		/* set window resize event to server */
		RTGUI_EVENT_WIN_RESIZE_INIT(&event);
		event.wid = win;
		event.rect = *rect;

		rtgui_thread_send(rtgui_thread_get_server(), &(event.parent), sizeof(rtgui_event_win_resize_t));
	}
}

void rtgui_win_set_onactivate(rtgui_win_t* win, rtgui_event_handler_ptr handler)
{
	if(win != RT_NULL)
	{
		win->on_activate = handler;
	}
}

void rtgui_win_set_ondeactivate(rtgui_win_t* win, rtgui_event_handler_ptr handler)
{
	if(win != RT_NULL)
	{
		win->on_deactivate = handler;
	}
}

void rtgui_win_set_onclose(rtgui_win_t* win, rtgui_event_handler_ptr handler)
{
	if(win != RT_NULL)
	{
		win->on_close = handler;
	}
}

void rtgui_win_set_title(rtgui_win_t* win, const char *title)
{
	/* modify in local side */
	rt_free(win->title);

	if(title != RT_NULL)
		win->title = rt_strdup(title);
	else
		win->title = RT_NULL;
}

char* rtgui_win_get_title(rtgui_win_t* win)
{
	RT_ASSERT(win != RT_NULL);

	return win->title;
}

rtgui_point_t rtgui_win_get_client_zero(rtgui_win_t *win)
{
	rtgui_point_t p={0};
	rtgui_rect_t r1,r2;

	rtgui_win_get_client_rect(win, &r1);
	rtgui_widget_get_rect(win, &r2);
	p.x = r1.x1-r2.x1;
	p.y = r1.y1-r2.y1;

	return p;
}

/* return window's client area */
void rtgui_win_get_client_rect(rtgui_win_t *win, rtgui_rect_t *rect)
{
	rtgui_widget_get_rect(win, rect);

	if(win->style & RTGUI_WIN_BORDER)
		rtgui_rect_inflate(rect, -RTGUI_WIDGET_BORDER(win));
	
	if(win->style & RTGUI_WIN_TITLE)
		rect->y1 += win->title_height;

	if(win->style & RTGUI_WIN_MENU)
		rect->y1 += win->menu_height;
	
	if(win->style & RTGUI_WIN_STATUS)
		rect->y2 -= win->status_height;

}

/* return winodw's title bar area */
void rtgui_win_get_title_rect(rtgui_win_t *win, rtgui_rect_t *rect)
{
	rtgui_widget_get_rect(win, rect);

	if(win->style & RTGUI_WIN_TITLE)
	{
		if(win->style & RTGUI_WIN_BORDER)
		{
			rtgui_rect_inflate(rect, -RTGUI_WIDGET_BORDER(win));
		}
		rect->y2 = rect->y1 + win->title_height;
	}
	else 
	{
		rect->x1=rect->x2=rect->y1=rect->y2 = 0;
	}
}

/* return window's closebox area */
void rtgui_win_get_closebox_rect(rtgui_win_t *win, rtgui_rect_t *rect)
{
	rtgui_win_get_title_rect(win,rect);
	if(win->style & RTGUI_WIN_CLOSEBOX)
	{
		/* get close button rect (device value) */
		rect->x2 = rect->x2 - 2;
		rect->x1 = rect->x2 - RTGUI_WIN_CLOSEBOX_WIDTH;
		rect->y1 = rect->y1 + (rtgui_rect_height(*rect)-RTGUI_WIN_CLOSEBOX_HEIGHT)/2;
		rect->y2 = rect->y1 + RTGUI_WIN_CLOSEBOX_HEIGHT;
	}
}

/* return window's maximum box area */
void rtgui_win_get_maxbox_rect(rtgui_win_t *win, rtgui_rect_t *rect)
{
	rtgui_win_get_closebox_rect(win,rect);
	if(win->style & RTGUI_WIN_CLOSEBOX)	
	{
		rtgui_rect_moveto(rect,-2-RTGUI_WIN_CLOSEBOX_WIDTH,0);	
	}
}

/* return window's minimum box area */
void rtgui_win_get_minbox_rect(rtgui_win_t *win, rtgui_rect_t *rect)
{
	rtgui_win_get_closebox_rect(win,rect);
	if(win->style & RTGUI_WIN_CLOSEBOX)	
	{
		rtgui_rect_moveto(rect,-2-RTGUI_WIN_CLOSEBOX_WIDTH,0);	
	}
	if(win->style &RTGUI_WIN_MAXBOX)	
	{
		rtgui_rect_moveto(rect,-2-RTGUI_WIN_CLOSEBOX_WIDTH,0);
	}
}


