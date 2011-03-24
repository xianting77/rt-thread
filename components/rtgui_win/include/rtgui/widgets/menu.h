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
 * 2009-10-16     Bernard      first version
 */
#ifndef __RTGUI_MENU_H__
#define __RTGUI_MENU_H__

#include <rtgui/rtgui.h>
#include <rtgui/image.h>
#include <rtgui/widgets/widget.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int  HMENU;

#define RTGUI_MENU_HEIGHT 43	/*�˵���߶�*/
#define RTGUI_MENU_BORDER	2

#define RTGUI_MENU_IMAGE_W	24
#define RTGUI_MENU_IMAGE_H	24

#define ITEM (System,Name,ID) \
rtgui_menu_append(hPopupMenu_##System, MF_ENABLED, ID, Name); 

/*�˵����־*/
#define RTGUI_MENU_POPUP       		0x00000080L

typedef struct rtgui_menu_item 	rtgui_menu_item_t; //��ͨ�˵�
typedef struct rtgui_menu		rtgui_menu_t;//�˵�


/** Gets the type of a view */
#define RTGUI_MENU_TYPE       (rtgui_menu_type_get())
/** Casts the object to an rtgui_view_t */
#define RTGUI_MENU(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_MENU_TYPE, rtgui_menu_t))
/** Checks if the object is an rtgui_view_t */
#define RTGUI_IS_MENU(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_MENU_TYPE))

struct rtgui_menu
{        
	rtgui_widget_t			parent;
	char*					name;
	/* ���ڸ�����ͼ�Ĳ���,��������ͼ,ȥ����Щ����û��Ӱ�� */
	rt_uint32_t     		orient;		   /* ���з��� */
	rt_uint32_t     		item_size;	   /* �˵���ߴ� */
	rt_uint16_t 			item_count;	   /* �˵����� */

	rtgui_menu_item_t*  	current_item;  /* ��ǰ�Ĳ˵��� */
	rtgui_menu_item_t*		forego_item;   /* ��һ���˵��� */ 

	/*�¼��˵��� */
    rtgui_menu_item_t*  	head;    /*ͷ*/
	rtgui_menu_item_t*  	tail;    /*β*/
	/*�ϼ��˵��� */
	rtgui_menu_item_t*  	farther; 
};

struct rtgui_menu_item
{
    char*     				caption;  //�˵���
	rt_uint32_t				item_id; 
	rt_uint32_t 			type;	  //�˵��Ĵ�������   
    rt_uint32_t				shortcut;
	rt_bool_t				bexit;	  //���øò˵����Ƿ��˳��˵�
	
	rtgui_menu_item_t* 		next;	 
	rtgui_menu_item_t* 		prev;

	rtgui_image_t*			image;	  //�󶨵�ͼ��
	rtgui_menu_t* 			sub_menu; //�Ӳ˵�
	 
	void(*func_updown)(rtgui_menu_t *menu);	//��UP/DOWN����֮�����еĺ���
	void(*func_enter)(void);	  //��ȷ�Ϻ�ִ�еĺ���
};

rtgui_type_t *rtgui_menu_type_get(void);

rtgui_menu_t* rtgui_menu_create(PVOID parent, const char* name);
void rtgui_menu_destroy(rtgui_menu_t* menu);


HMENU rtgui_menu_popup_create (void);
rt_bool_t rtgui_menu_append(rtgui_menu_t *menu,rt_uint32_t flags,rt_uint32_t ID,char * caption);
void rtgui_menu_popup_delete(rtgui_menu_t* menu);
rtgui_menu_t* rtgui_menu_item_delete(rtgui_menu_t* menu,rtgui_menu_item_t* pItem);
void rtgui_menu_on_down(rtgui_menu_t* menu);
void rtgui_menu_on_up(rtgui_menu_t* menu);
void rtgui_menu_on_exit(rtgui_menu_t* menu);
void rtgui_menu_set_selected(rtgui_menu_t* menu, int selected);

#ifdef __cplusplus
}
#endif

#endif
