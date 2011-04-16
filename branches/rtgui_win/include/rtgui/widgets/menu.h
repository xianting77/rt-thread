/*
 * File      : view.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __RTGUI_MENU_H__
#define __RTGUI_MENU_H__

#include <rtgui/rtgui.h>
#include <rtgui/image.h>
#include <rtgui/widgets/widget.h>
#include <rtgui/widgets/window.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RTGUI_MENU_MARGIN_W	8
#define RTGUI_MENU_MARGIN_H	4

#define MENU_ADD_ITEM (System,Name,ID) \
rtgui_menu_append(hPopupMenu_##System, MF_ENABLED, ID, Name); 

/* menu item flag */
#define RTGUI_MENU_NORMAL	0x00000000L
#define RTGUI_MENU_POPUP	0x00000040L

typedef struct rtgui_menu_item   rtgui_menu_item_t;
typedef struct rtgui_menu        rtgui_menu_t;

/** Gets the type of a menu */
#define RTGUI_MENU_TYPE       (rtgui_menu_type_get())
/** Casts the object to an rtgui_menu_t */
#define RTGUI_MENU(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_MENU_TYPE, rtgui_menu_t))
/** Checks if the object is an rtgui_menu_t */
#define RTGUI_IS_MENU(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_MENU_TYPE))

struct rtgui_menu
{        
	rtgui_widget_t			parent;
	char*					name;
	rt_uint16_t				type;       /* menu type */
	rt_bool_t				unfold:1;	/* is it unfold? */
	rt_uint32_t     		orient;	    /* orient */
	rt_uint16_t 			item_count;	/* item count */

	rtgui_menu_item_t*  	now_item;   /* current item */
	rtgui_menu_item_t*	    old_item;   /* previous item */ 

    rtgui_menu_item_t*  	head;       /* frist item */
	rtgui_menu_item_t*  	tail;       /* last item */

	rtgui_menu_t*  			farther;
	
	rtgui_win_t*			entrust_win;/* popup menu will use. */
	void (*func_updown)(rtgui_menu_t *menu);	/* running at once when execute up/down action */ 
};

struct rtgui_menu_item
{
    char*     				caption;    /* menu name */
	rt_uint32_t				item_id; 
	rt_uint32_t 			flag;       /* menu flag */ 
	rtgui_image_t*			image;      /* icon image */
    rt_uint32_t				shortcut;	/* shortcut key */
	rt_uint16_t				item_width;
	rt_uint16_t				item_height;
	rt_bool_t				bexit:1;    /* is or isn't exit */
	
	rtgui_menu_item_t* 		next;	 
	rtgui_menu_item_t* 		prev;
	rtgui_menu_t* 			submenu;    /* children menu */

	void (*func_enter)(rtgui_menu_t *menu);	  /* running when execute enter/on_item action */
};

typedef void (*menu_callback_t)(rtgui_menu_t *menu);

rtgui_type_t *rtgui_menu_type_get(void);

rtgui_menu_t* rtgui_menu_create(PVOID parent, const char* name, int left, int top, int flag);
void rtgui_menu_destroy(rtgui_menu_t* menu);

rt_bool_t rtgui_menu_append(rtgui_menu_t *menu,rt_uint32_t flags,rt_uint32_t ID,char * caption, 
		menu_callback_t func_enter);
void rtgui_menu_popup_delete(rtgui_menu_t* menu);
rtgui_menu_t* rtgui_menu_item_delete(rtgui_menu_t* menu,rtgui_menu_item_t* pItem);
void rtgui_menu_set_selected(rtgui_menu_t* menu, int selected);

#ifdef __cplusplus
}
#endif

#endif
