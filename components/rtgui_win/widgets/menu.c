/*
 * File      : menu.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-07-16     amsl         first version
 */

#include <rtgui/dc.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/rtgui_theme.h> 
#include <rtgui/widgets/menu.h>

rt_bool_t rtgui_menu_event_handler(PVOID wdt, rtgui_event_t* event);
//static rt_bool_t rtgui_menu_onfocus(PVOID wdt, rtgui_event_t* event);
static rt_bool_t rtgui_menu_onunfocus(PVOID wdt, rtgui_event_t* event);

static void _rtgui_menu_constructor(rtgui_menu_t *menu)
{
	/* init menu */
	rtgui_widget_set_event_handler(menu, rtgui_menu_event_handler);
	rtgui_widget_set_onunfocus(menu, rtgui_menu_onunfocus);
	menu->current_item	 = RT_NULL;	//��ǰ�˵���
	menu->forego_item = RT_NULL;
	menu->name = RT_NULL;
	RTGUI_WIDGET_FLAG(menu) |= RTGUI_WIDGET_FLAG_FOCUSABLE;

	menu->head = RT_NULL;
	menu->tail = RT_NULL;
	menu->farther = RT_NULL;

	//���û��Ʋ˵�ʱҪ�õ��Ĳ���
	menu->orient = RTGUI_VERTICAL;
	menu->item_count = 0;//�˵�������
	menu->item_size = SELECTED_HEIGHT;
}

static void _rtgui_menu_destructor(rtgui_menu_t *menu)
{
	if(menu->name != RT_NULL)
	{
		rt_free(menu->name);
		menu->name = RT_NULL;
	}
}

rtgui_type_t *rtgui_menu_type_get(void)
{
	static rtgui_type_t *menu_type = RT_NULL;

	if(!menu_type)
	{
		menu_type = rtgui_type_create("menu", RTGUI_WIDGET_TYPE,
			sizeof(rtgui_menu_t), 
			RTGUI_CONSTRUCTOR(_rtgui_menu_constructor), 
			RTGUI_DESTRUCTOR (_rtgui_menu_destructor ));
	}

	return menu_type;
}

void rtgui_menu_set_selected(rtgui_menu_t* menu, int selected)
{
	RT_ASSERT(menu != RT_NULL);

	if(selected >= menu->item_count) return;

	if(selected >= 0)
    {
		rtgui_menu_item_t* item=menu->head;
		while(selected--)
		{
			item = item->next;
		} 
		menu->forego_item = menu->current_item;
		menu->current_item = item; 
	}

    /* update rbox widget */
	rtgui_theme_draw_menu_item(menu, menu->forego_item);
	rtgui_theme_draw_menu_item(menu, menu->current_item);

	if(menu->current_item->func_updown)
	{
		menu->current_item->func_updown(menu);
	}
}

static void rtgui_menu_onmouse(rtgui_menu_t* menu, rtgui_event_mouse_t* event)
{
	RT_ASSERT(menu != RT_NULL);
	RT_ASSERT(event  != RT_NULL);

	if(event->button & RTGUI_MOUSE_BUTTON_DOWN && event->button & RTGUI_MOUSE_BUTTON_LEFT)
	{
		rtgui_rect_t rect;

		rtgui_widget_focus(menu);

		rtgui_widget_get_rect(menu, &rect);
		rtgui_widget_rect_to_device(menu, &rect);

		rtgui_rect_inflate(&rect, -RTGUI_WIDGET_BORDER(menu));
		if(rtgui_rect_contains_point(&rect, event->x, event->y) == RT_EOK)
		{
			if(menu->orient == RTGUI_VERTICAL)
			{
				int delta_y = event->y - rect.y1;
				int sel = delta_y/(menu->item_size+1);

				rtgui_menu_set_selected(menu, sel);

				return;
			}
		}
	}
}

rt_bool_t rtgui_menu_event_handler(PVOID wdt, rtgui_event_t* event)
{
	rtgui_widget_t *widget = (rtgui_widget_t*)wdt;
	rtgui_menu_t* menu = (rtgui_menu_t*)widget;

	RT_ASSERT(widget != RT_NULL);

	switch (event->type)
	{
		case RTGUI_EVENT_PAINT:		
			if(widget->on_draw != RT_NULL) 
				widget->on_draw(widget, event);
			else
			{
				rtgui_theme_draw_menu(menu);
			}
			break;

		case RTGUI_EVENT_MOUSE_BUTTON:
			if(widget->on_mouseclick != RT_NULL) 
				widget->on_mouseclick(widget, event);
			else
			{
				rtgui_menu_onmouse(menu, (rtgui_event_mouse_t*)event);
			}
			break;
		case RTGUI_EVENT_KBD: 
	    { 
	        rtgui_event_kbd_t* ekbd = (rtgui_event_kbd_t*)event;
	
	        if(RTGUI_KBD_IS_DOWN(ekbd)) 
	        { 
				rt_uint16_t key = ekbd->key;
	
				switch(key)
				{
					case RTGUIK_UP:
						//������һ���˵�
						rtgui_menu_on_up(menu);
						break;
	
					case RTGUIK_DOWN:
						//������һ���˵�
						rtgui_menu_on_down(menu);
						break;
	
					case RTGUIK_LEFT:
						//������һ���˵�
						break;
	
					case RTGUIK_RIGHT:
						//�����Ӳ˵�
						break;
	
					default: 
						break;
				}
	        } 
	    }
		default:
			break;
	}

	return RT_FALSE;
}

static rt_bool_t rtgui_menu_onunfocus(PVOID wdt, rtgui_event_t* event)
{
	rtgui_widget_t *widget = (rtgui_widget_t*)wdt;
	rtgui_menu_t* menu = (rtgui_menu_t*)widget;

	rtgui_menu_on_exit(menu);
	return RT_TRUE;
}

rtgui_menu_t* rtgui_menu_create(PVOID parent, const char* name)
{
	rtgui_menu_t* menu;

	RT_ASSERT(parent != RT_NULL);

	/* allocate view */
	menu = rtgui_widget_create (RTGUI_MENU_TYPE);
	if(menu != RT_NULL)
	{
		rtgui_rect_t rect;
		
		rtgui_widget_get_rect(parent, &rect);
		rtgui_widget_rect_to_device(parent,&rect);
		rect.x1 += 1;//������ø�ֵ
		rect.y1 += 1;
		rect.x2 = rect.x1+10;
		rect.y2 = rect.y1+10;
		rtgui_widget_set_rect(menu, &rect);

		menu->name = rt_strdup(name);
		RTGUI_WIDGET_BACKGROUND(menu) = white;
		RTGUI_WIDGET_TEXTALIGN(menu) = RTGUI_ALIGN_CENTER_VERTICAL;

		rtgui_container_add_child(parent, menu);
	}

	return menu;
}

void rtgui_menu_destroy(rtgui_menu_t* menu)
{
	rtgui_widget_destroy(menu);
}

/*
��һ��POPUP�˵�����������׷��һ���˵���,ͬWindow����
Input: menu   -- Ҫ׷�ӵĲ˵�
	   flags  -- RTGUI_MENU_POPUP:��ʾ׷�ӵ���ΪPOPUP�˵�,����:��ʾ׷��Ϊ��ͨ�˵���
       ID     -- ���flagsΪMENU_FLAG_POPUP ,������ʾΪPOPUP�˵���ָ��,���flagsΪ���� ,������ʾΪ�˵����ID
       name   -- �˵�����
return : TRUE --�ɹ�
*/
rt_bool_t rtgui_menu_append(rtgui_menu_t *menu,rt_uint32_t flag,rt_uint32_t ID,char* caption)
{
	rtgui_menu_item_t *item,*node,*old;
	
	if(menu == RT_NULL) return RT_FALSE;

    item = rt_malloc(sizeof(rtgui_menu_item_t));
	if(item == RT_NULL) return RT_FALSE;	
	rt_memset(item, 0, sizeof(rtgui_menu_item_t));

    item->type = flag;

	item->caption = rt_strdup(caption);  
    
	item->shortcut = 0; /*�޿�ݼ�*/
	item->bexit = RT_FALSE;//TRUE; 
	item->func_updown  = RT_NULL; 
	item->func_enter = RT_NULL; 

    if(flag & RTGUI_MENU_POPUP)
    {//popup�˵�
		item->item_id = 0;
    	item->sub_menu = (rtgui_menu_t*)ID;

		/*�����ϼ��˵�*/
		item->sub_menu->farther = item;
	}
	else
	{//��ͨ�˵�
		item->item_id = ID;
		item->sub_menu = 0;
	}
	item->next = RT_NULL;
	menu->item_count++;
	
	node = menu->head;
	if(!node)
	{ 
		/*û�в˵����ǵ�һ��*/
		menu->head = item;
		menu->tail = item;
		item->prev = RT_NULL;
		return RT_TRUE;
	}
	
	while(node)
	{
		old = node;
		node = node->next;/*��һ���˵��� */
	}

	/* ��������� */
	old->next = item;	
	item->prev = old;

	/*���¶���βָ�� */
	menu->tail = item;

	return RT_TRUE;
}

/*
 * �ڲ˵���ɾ��һ���˵���,�����������һ���Ӳ˵�,�����Ӳ˵�ָ��
 */
rtgui_menu_t* rtgui_menu_item_delete(rtgui_menu_t* menu,rtgui_menu_item_t* item)
{
	rtgui_menu_t* ret;

	if(item->prev == RT_NULL)
	{
		if(item->next == RT_NULL)
		{
			/*ֻ��һ�� */
			menu->head = RT_NULL;
			menu->tail = RT_NULL;
		}
		else
		{
			/*ɾ����һ�� */
			item->next->prev = RT_NULL;
			menu->head = item->next;
		}						
	}
	else
	{
		if( item->next == RT_NULL)
		{
			/*���һ�� */
			item->prev->next = RT_NULL;
			menu->tail = item->prev;
		}
		else
		{
			/* �м�� */
			item->prev->next = item->next;
			item->next->prev = item->prev;
		}
	}
	
	if(item->type & RTGUI_MENU_POPUP)
		ret = item->sub_menu;	/* �����Ӳ˵� */
	else
		ret = RT_NULL;

	rt_free(item->caption);
	rt_free(item);

	return ret;
}

/*
�ж��Ƿ�һ���˵�Ϊ�ղ˵���û�в˵���)
*/
rt_bool_t rtgui_menu_is_null(rtgui_menu_t* hMenu)
{
	if(hMenu->head == RT_NULL)
		return RT_TRUE;
	return RT_FALSE;
}

/* ɾ��һ��Popup�˵� */
void rtgui_menu_popup_delete(rtgui_menu_t* menu)
{
	rtgui_menu_item_t* pItem;
	rtgui_menu_t* sub_menu;

	if(rtgui_menu_is_null(menu))
	{
		/*�˵�Ϊ��,�ͷ�Popup�ռ�*/
		rt_free(menu );
		return;
	}
	
	pItem = menu->head;
	while(pItem)
	{
		sub_menu = rtgui_menu_item_delete(menu,pItem);
		if(sub_menu)
		{
			/*ʹ�õݹ��ͷ���һ���Ӳ˵� */
			rtgui_menu_popup_delete(sub_menu);
		}
			/*ȡ��һ���� */
		pItem = menu->head;
	}
	/*ȫ���˵����ͷ����,�ͷ�POPUPָ�� */

	rt_free(menu);		
}

/* �����ƶ�һ���˵�,����:��ǰ�˵���� */
void rtgui_menu_on_down(rtgui_menu_t* menu)
{	
	RT_ASSERT(menu != RT_NULL);

	/* Popup�˵�û�в˵��� */
	if(menu->current_item==RT_NULL)
	{	
		return ; /* �˵��ݴ� */
	}

	menu->forego_item = menu->current_item;
	/* ���һ���˵����� */
	if(menu->current_item->next==RT_NULL)
	{	
		/* ��,�ƶ�����ͷ��һ�� */
		menu->current_item = menu->head;
	}
	else
	{
		/*�Ƶ���һ�� */
		menu->current_item = menu->current_item->next;
	}
	
	rtgui_theme_draw_menu_item(menu, menu->forego_item);
	rtgui_theme_draw_menu_item(menu, menu->current_item);

	if(menu->current_item->func_updown)
	{
		menu->current_item->func_updown(menu);
	}
}

/*
�����ƶ�һ���˵�
*/
void rtgui_menu_on_up(rtgui_menu_t* menu)
{	
	RT_ASSERT(menu != RT_NULL);

	/*Popup�˵�û�в˵���,�˵���ƴ��� */
	if(menu->current_item==RT_NULL)
	{
		return ;  /*�˵��ݴ� */
	}

	menu->forego_item = menu->current_item;
	/* ��һ���˵����� */
	if(menu->current_item->prev==RT_NULL)
	{
		/* ��,�����ƶ�*/
		menu->current_item = menu->tail;
	}
	else
	{
		/* �Ƶ���һ�� */
		menu->current_item = menu->current_item->prev;
	}
	
	rtgui_theme_draw_menu_item(menu, menu->forego_item);
	rtgui_theme_draw_menu_item(menu, menu->current_item);

	if(menu->current_item->func_updown)
	{
		menu->current_item->func_updown(menu);
	}
}

/*�����˵�
*/
void rtgui_menu_on_exit(rtgui_menu_t* menu)
{
	rtgui_widget_unfocus(menu);
	rtgui_theme_draw_menu(menu);

	if(menu->current_item->bexit)
	{
		menu->current_item = RT_NULL;
		menu->forego_item = RT_NULL;
	}
}



