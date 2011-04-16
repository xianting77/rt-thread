/*
 * File      : listctrl.c
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2010, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-01-06     Bernard      first version
 */

#include <rtgui/rtgui_theme.h>
#include <rtgui/widgets/listctrl.h>

static void rtgui_listctrl_update_current(rtgui_listctrl_t* ctrl, rt_uint16_t old_item);

static void _rtgui_listctrl_constructor(rtgui_listctrl_t *ctrl)
{
	/* set default widget rect and set event handler */
	rtgui_widget_set_event_handler(ctrl,rtgui_listctrl_event_handler);

	RTGUI_WIDGET(ctrl)->flag |= RTGUI_WIDGET_FLAG_FOCUSABLE;

	ctrl->current_item = -1;
	ctrl->items_count = 0;
	ctrl->page_items = 0;
	ctrl->on_item = 0;
	ctrl->on_item_draw = RT_NULL;

	RTGUI_WIDGET_BACKGROUND(RTGUI_WIDGET(ctrl)) = white;
	RTGUI_WIDGET_TEXTALIGN(RTGUI_WIDGET(ctrl)) = RTGUI_ALIGN_CENTER_VERTICAL;
}

rtgui_type_t *rtgui_listctrl_type_get(void)
{
	static rtgui_type_t *listctrl_type = RT_NULL;

	if (!listctrl_type)
	{
		listctrl_type = rtgui_type_create("listctrl", RTGUI_WIDGET_TYPE,
			sizeof(rtgui_listctrl_t), RTGUI_CONSTRUCTOR(_rtgui_listctrl_constructor), RT_NULL);
	}

	return listctrl_type;
}

static void _rtgui_listctrl_get_rect(rtgui_listctrl_t* ctrl, rtgui_rect_t* rect)
{
	rtgui_widget_get_rect(RTGUI_WIDGET(ctrl), rect);
	if (ctrl->items_count > rtgui_rect_height(*rect)/SELECTED_HEIGHT)
	{
		rect->x2 = rect->x2 - 8;
	}
}

static void _rtgui_listctrl_get_scrollbar_rect(rtgui_listctrl_t* ctrl, rtgui_rect_t* rect)
{
	rtgui_widget_get_rect(RTGUI_WIDGET(ctrl), rect);
	if (ctrl->items_count > rtgui_rect_height(*rect)/SELECTED_HEIGHT)
	{
		rect->x1 = rect->x2 - 8;
	}
	else
	{
		/* no scrollbar */
		rt_memset(rect, 0, sizeof(rtgui_rect_t));
	}
}

static void _rtgui_listctrl_scrollbar_ondraw(rtgui_listctrl_t* ctrl, struct rtgui_dc* dc)
{
	rtgui_rect_t rect;
	rt_uint32_t height, y1;

	/* get scrollbar rect */
	_rtgui_listctrl_get_scrollbar_rect(ctrl, &rect);
	rtgui_dc_fill_rect(dc, &rect);

	height = rtgui_rect_height(rect);

	height = height / ((ctrl->items_count + (ctrl->page_items - 1))/ctrl->page_items);
	y1 = (ctrl->current_item / ctrl->page_items) * height;

	rect.y1 = rect.y1 + y1; rect.y2 = rect.y1 + height;
	rect.x1 -= 3;
	rtgui_theme_draw_selected(dc, &rect);
}

static void _rtgui_listctrl_scrollbar_onmouse(rtgui_listctrl_t* ctrl, struct rtgui_event_mouse* mouse)
{
	rtgui_rect_t rect;
	rt_uint32_t height, y1;
	rt_uint16_t old_item;

	/* get scrollbar rect */
	_rtgui_listctrl_get_scrollbar_rect(ctrl, &rect);
	height = rtgui_rect_height(rect);
	height = height / ((ctrl->items_count + (ctrl->page_items - 1))/ctrl->page_items);
	y1 = (ctrl->current_item / ctrl->page_items) * height;

	rect.y1 = rect.y1 + y1; rect.y2 = rect.y1 + height;
	rtgui_widget_rect_to_device(RTGUI_WIDGET(ctrl), &rect);

	old_item = ctrl->current_item;
	if (mouse->y < rect.y1)
	{
		if (ctrl->current_item - ctrl->page_items >= 0)
			ctrl->current_item -= ctrl->page_items;
		rtgui_listctrl_update_current(ctrl, old_item);
	}
	else if (mouse->y > rect.y2)
	{
		if (ctrl->current_item + ctrl->page_items < ctrl->items_count - 1)
			ctrl->current_item += ctrl->page_items;
		else
			ctrl->current_item = ((ctrl->current_item / ctrl->page_items) + 1) * ctrl->page_items;
		rtgui_listctrl_update_current(ctrl, old_item);
	}
}

static void _rtgui_listctrl_ondraw(rtgui_listctrl_t* ctrl)
{
	rtgui_rect_t rect, item_rect;
	rtgui_dc_t* dc;
	rt_uint16_t page_index, index;

	dc = rtgui_dc_begin_drawing(ctrl);
	if (dc == RT_NULL) return;

	_rtgui_listctrl_get_rect(ctrl, &rect);
	rtgui_dc_fill_rect(dc, &rect);

	rect.x2 -= 1; rect.y2 -= 1;

	/* get item base rect */
	item_rect = rect;
	item_rect.x1 += 1; item_rect.x2 -= 1;
	item_rect.y1 += 2;
	item_rect.y2 = item_rect.y1 + (2 + SELECTED_HEIGHT);

	/* get current page */
	page_index = (ctrl->current_item / ctrl->page_items) * ctrl->page_items;
	for (index = 0; index < ctrl->page_items; index ++)
	{
		if (page_index + index >= ctrl->items_count) break;

		if (page_index + index == ctrl->current_item)
		{
			rtgui_theme_draw_selected(dc, &item_rect);
		}

		if (ctrl->on_item_draw != RT_NULL)
		{
			ctrl->on_item_draw(ctrl, dc, &item_rect, page_index + index);
		}

        /* move to next item position */
		item_rect.y1 += (SELECTED_HEIGHT + 2);
		item_rect.y2 += (SELECTED_HEIGHT + 2);
	}

	/* draw scrollbar */
	_rtgui_listctrl_scrollbar_ondraw(ctrl, dc);
	rtgui_dc_end_drawing(dc);
}

void rtgui_listctrl_update_current(rtgui_listctrl_t* ctrl, rt_uint16_t old_item)
{
	struct rtgui_dc* dc;
	rtgui_rect_t rect, item_rect;

	if (old_item/ctrl->page_items != ctrl->current_item/ctrl->page_items)
	{
		/* it's not a same page, update all */
		rtgui_widget_update(RTGUI_WIDGET(ctrl));
		return;
	}

	dc = rtgui_dc_begin_drawing(RTGUI_WIDGET(ctrl));
	if (dc == RT_NULL) return;

	_rtgui_listctrl_get_rect(ctrl, &rect);
	rect.x2 -= 1; rect.y2 -= 1;

	item_rect = rect;
	/* get old item's rect */
	item_rect.x1 += 1; item_rect.x2 -= 1;
	item_rect.y1 += 2;
	item_rect.y1 += (old_item % ctrl->page_items) * (2 + SELECTED_HEIGHT);
	item_rect.y2 = item_rect.y1 + (2 + SELECTED_HEIGHT);

	/* draw old item */
	rtgui_dc_fill_rect(dc, &item_rect);
	if (ctrl->on_item_draw != RT_NULL)
		ctrl->on_item_draw(ctrl, dc, &item_rect, old_item);

	/* draw current item */
	item_rect = rect;
	/* get current item's rect */
	item_rect.x1 += 1; item_rect.x2 -= 1;
	item_rect.y1 += 2;
	item_rect.y1 += (ctrl->current_item % ctrl->page_items) * (2 + SELECTED_HEIGHT);
	item_rect.y2 = item_rect.y1 + (2 + SELECTED_HEIGHT);

	/* draw current item */
	rtgui_theme_draw_selected(dc, &item_rect);
	if (ctrl->on_item_draw != RT_NULL)
		ctrl->on_item_draw(ctrl, dc, &item_rect, ctrl->current_item);

	rtgui_dc_end_drawing(dc);
}

rt_bool_t rtgui_listctrl_event_handler(PVOID wdt, rtgui_event_t* event)
{
	rtgui_widget_t *widget = (rtgui_widget_t*)wdt;
	rtgui_listctrl_t* ctrl = RT_NULL;

	ctrl = RTGUI_LISTCTRL(widget);
	switch (event->type)
	{
	case RTGUI_EVENT_PAINT:
		if(widget->on_draw != RT_NULL ) 
			widget->on_draw(widget, event);
		else
			_rtgui_listctrl_ondraw(ctrl);
		return RT_FALSE;

    case RTGUI_EVENT_RESIZE:
        {
			struct rtgui_event_resize* resize;

			resize = (struct rtgui_event_resize*)event;

            /* recalculate page items */
			ctrl->page_items = resize->h  / (2 + SELECTED_HEIGHT);
        }
        break;

	case RTGUI_EVENT_MOUSE_BUTTON:
		{
			rtgui_rect_t rect;
			struct rtgui_event_mouse* emouse;

			emouse = (struct rtgui_event_mouse*)event;

			/* get scrollbar rect */
			_rtgui_listctrl_get_scrollbar_rect(ctrl, &rect);
			rtgui_widget_rect_to_device(RTGUI_WIDGET(ctrl), &rect);
			if (rtgui_rect_contains_point(&rect, emouse->x, emouse->y) == RT_EOK)
			{
				_rtgui_listctrl_scrollbar_onmouse(ctrl, emouse);
				return RT_TRUE;
			}

			/* calculate selected item */

			/* get physical extent information */
			_rtgui_listctrl_get_rect(ctrl, &rect);
			rtgui_widget_rect_to_device(widget, &rect);

			if ((rtgui_rect_contains_point(&rect, emouse->x, emouse->y) == RT_EOK) &&
					(ctrl->items_count > 0))
			{
				rt_uint16_t index;
				index = (emouse->y - rect.y1) / (2 + SELECTED_HEIGHT);

				/* set focus */
				rtgui_widget_focus(widget);
				{
					struct rtgui_rect rect;
					struct rtgui_dc* dc;

					dc = rtgui_dc_begin_drawing(RTGUI_WIDGET(ctrl));
					if (dc != RT_NULL)
					{
						/* get widget rect */
						rtgui_widget_get_rect(RTGUI_WIDGET(ctrl), &rect);
						/* update focus border */
						rect.x2 -= 1; rect.y2 -= 1;
						rtgui_dc_end_drawing(dc);
					}
				}

				if ((index < ctrl->page_items) &&
					(ctrl->current_item/ctrl->page_items)* ctrl->page_items + index < ctrl->items_count)
				{
					rt_uint16_t old_item;

					old_item = ctrl->current_item;

					/* set selected item */
					ctrl->current_item = (ctrl->current_item/ctrl->page_items) * ctrl->page_items + index;
					if (emouse->button & RTGUI_MOUSE_BUTTON_DOWN)
					{
						/* down event */
						rtgui_listctrl_update_current(ctrl, old_item);
					}
					else
					{
						/* up event */
						if (ctrl->on_item != RT_NULL)
						{
							ctrl->on_item(RTGUI_WIDGET(ctrl), RT_NULL);
						}
					}
				}
			}

			return RT_TRUE;
		}

    case RTGUI_EVENT_KBD:
        {
            struct rtgui_event_kbd* ekbd = (struct rtgui_event_kbd*)event;
            if ((ekbd->type == RTGUI_KEYDOWN) && (ctrl->items_count > 0))
            {
				rt_uint16_t old_item;

				old_item = ctrl->current_item;
                switch (ekbd->key)
                {
				case RTGUIK_LEFT:
					if (ctrl->current_item - ctrl->page_items >= 0)
						ctrl->current_item -= ctrl->page_items;
					rtgui_listctrl_update_current(ctrl, old_item);
					return RT_FALSE;

                case RTGUIK_UP:
					if (ctrl->current_item > 0)
						ctrl->current_item --;
					rtgui_listctrl_update_current(ctrl, old_item);
					return RT_FALSE;

				case RTGUIK_RIGHT:
					if (ctrl->current_item + ctrl->page_items < ctrl->items_count - 1)
						ctrl->current_item += ctrl->page_items;
					else
					{
						if ((((ctrl->current_item/ctrl->page_items) + 1) * ctrl->page_items) < ctrl->items_count - 1)
							ctrl->current_item = ((ctrl->current_item / ctrl->page_items) + 1) * ctrl->page_items;
					}
					rtgui_listctrl_update_current(ctrl, old_item);
					return RT_FALSE;

                case RTGUIK_DOWN:
					if (ctrl->current_item < ctrl->items_count - 1)
						ctrl->current_item ++;
					rtgui_listctrl_update_current(ctrl, old_item);
					return RT_FALSE;

				case RTGUIK_RETURN:
                    if (ctrl->on_item != RT_NULL)
					{
						ctrl->on_item(RTGUI_WIDGET(ctrl), RT_NULL);
					}
					return RT_FALSE;

                default:
                    break;
                }
            }
        }
		return RT_FALSE;
	}

    /* use ctrl event handler */
    return rtgui_widget_event_handler(widget, event);
}

rtgui_listctrl_t* rtgui_listctrl_create(PVOID parent, rt_uint32_t items, rt_uint16_t count, 
			int left, int top, int w, int h, rtgui_onitem_draw_t ondraw)
{
	rtgui_listctrl_t* ctrl = RT_NULL;

	ctrl = (rtgui_listctrl_t*) rtgui_widget_create(RTGUI_LISTCTRL_TYPE);
	if (ctrl != RT_NULL)
	{
	    rtgui_rect_t rect;

		rtgui_widget_get_rect(parent, &rect);
		rtgui_widget_rect_to_device(parent,&rect);
		rect.x1 += left;
		rect.y1 += top;
		rect.x2 = rect.x1+w;
		rect.y2 = rect.y1+h;
		rtgui_widget_set_rect(ctrl, &rect);
		rtgui_container_add_child(parent, ctrl);

		ctrl->items = items;
	    ctrl->items_count = count;
		ctrl->on_item_draw = ondraw;

		ctrl->page_items = h / (2 + SELECTED_HEIGHT);
		
	}

	return ctrl;
}

void rtgui_listctrl_destroy(rtgui_listctrl_t* ctrl)
{
    /* destroy ctrl */
	rtgui_widget_destroy(RTGUI_WIDGET(ctrl));
}

void rtgui_listctrl_set_onitem(rtgui_listctrl_t* ctrl, rtgui_onitem_func_t func)
{
	RT_ASSERT(ctrl != RT_NULL);

	ctrl->on_item = func;
}

void rtgui_listctrl_set_items(rtgui_listctrl_t* ctrl, rt_uint32_t items, rt_uint16_t count)
{
	rtgui_rect_t rect;
	
	ctrl->items = items;
	ctrl->items_count = count;
	ctrl->current_item = 0;

	rtgui_widget_get_rect(RTGUI_WIDGET(ctrl), &rect);
	ctrl->page_items = rtgui_rect_height(rect) / (2 + SELECTED_HEIGHT);

	rtgui_widget_update(RTGUI_WIDGET(ctrl));
}

rt_bool_t rtgui_listctrl_get_item_rect(rtgui_listctrl_t* ctrl, rt_uint16_t item, rtgui_rect_t* item_rect)
{
	if (item < ctrl->items_count)
	{
		rt_uint16_t index;

		/* check whether this item in current page */
		index = (ctrl->current_item / ctrl->page_items) * ctrl->page_items;
		if (index > item || index + ctrl->page_items <= item) return RT_FALSE;

		*item_rect = RTGUI_WIDGET_EXTENT(ctrl);
		item_rect->y1 -= 2;
		item_rect->y1 += (item % ctrl->page_items) * (2 + SELECTED_HEIGHT);
		item_rect->y2 = item_rect->y1 + (2 + SELECTED_HEIGHT);
		return RT_TRUE;
	}

	return RT_FALSE;
}
