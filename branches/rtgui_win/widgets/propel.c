/*
 * File      : propel.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * propel(abbr.ppl): a change/UpDown subsidiary widget.
 * Change Logs:
 * Date           Author       Notes
 * 2011-05-01     amsl         Frist Version
 */
#include <rtgui/dc.h>
#include <rtgui/event.h>
#include <rtgui/widgets/container.h>
#include <rtgui/widgets/propel.h>

static void _rtgui_propel_constructor(rtgui_propel_t *ppl)
{
	/* init widget and set event handler */
	rtgui_widget_set_event_handler(ppl, rtgui_propel_event_handler);

	RTGUI_WIDGET_FOREGROUND(ppl) = black;
	RTGUI_WIDGET_BACKGROUND(ppl) = default_background;
	RTGUI_WIDGET_TEXTALIGN(ppl) = RTGUI_ALIGN_CENTER_VERTICAL;
	/* set field */
	ppl->orient = RTGUI_HORIZONTAL;
	ppl->flag = PROPEL_FLAG_NONE;
	ppl->range_min = 0;
	ppl->range_max = 0;
	ppl->wdtlnk = RT_NULL;
	ppl->bind = RT_NULL;
}

static void _rtgui_propel_destructor(rtgui_propel_t *ppl)
{
}

rtgui_type_t *rtgui_propel_type_get(void)
{
	static rtgui_type_t *ppl_type = RT_NULL;

	if(!ppl_type)
	{
		ppl_type = rtgui_type_create("propel", RTGUI_WIDGET_TYPE,
			sizeof(rtgui_propel_t),
			RTGUI_CONSTRUCTOR(_rtgui_propel_constructor),
			RTGUI_DESTRUCTOR(_rtgui_propel_destructor));
	}

	return ppl_type;
}

const static rt_uint8_t _up_arrow[]    = {0x10, 0x38, 0x7C, 0xFE};
const static rt_uint8_t _down_arrow[]  = {0xFE,0x7C, 0x38, 0x10};
const static rt_uint8_t _left_arrow[]  = {0x10, 0x30, 0x70, 0xF0, 0x70, 0x30, 0x10};
const static rt_uint8_t _right_arrow[] = {0x80, 0xC0, 0xE0, 0xF0, 0xE0, 0xC0, 0x80};

void rtgui_propel_ondraw(rtgui_propel_t* ppl)
{
	rtgui_rect_t rect, tmp_rect, arrow_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(ppl != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(ppl);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(ppl, &rect);
	
	tmp_rect = rect;
	if(ppl->orient == RTGUI_HORIZONTAL)
	{
		tmp_rect.x2 = tmp_rect.x2/2;
		rtgui_rect_inflate(&tmp_rect, -2);
		rtgui_dc_fill_rect(dc, &tmp_rect);
		rtgui_rect_inflate(&tmp_rect, 2);
		if(ppl->flag & PROPEL_FLAG_LEFT)
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_SUNKEN);
		else
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_RAISE);
		
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 4; arrow_rect.y2 = 7;
		rtgui_rect_moveto_align(&tmp_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		if(ppl->flag & PROPEL_UNVISIBLE_LEFT)
			RTGUI_DC_FC(dc) = dark_grey;
		else
			RTGUI_DC_FC(dc) = black;
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _left_arrow);

		tmp_rect.x1 = tmp_rect.x2;
		tmp_rect.x2 = rect.x2;
		rtgui_rect_inflate(&tmp_rect, -2);
		rtgui_dc_fill_rect(dc, &tmp_rect);
		rtgui_rect_inflate(&tmp_rect, 2);
		if(ppl->flag & PROPEL_FLAG_RIGHT)
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_SUNKEN);
		else
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_RAISE);
		
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 4; arrow_rect.y2 = 7;
		rtgui_rect_moveto_align(&tmp_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		if(ppl->flag & PROPEL_UNVISIBLE_RIGHT)
			RTGUI_DC_FC(dc) = dark_grey;
		else
			RTGUI_DC_FC(dc) = black;
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _right_arrow);	
	}
	else if(ppl->orient == RTGUI_VERTICAL)
	{
		tmp_rect.y2 = tmp_rect.y2/2;
		rtgui_rect_inflate(&tmp_rect, -2);
		rtgui_dc_fill_rect(dc, &tmp_rect);
		rtgui_rect_inflate(&tmp_rect, 2);
		if(ppl->flag & PROPEL_FLAG_UP)
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_SUNKEN);
		else
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_RAISE);
		
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 7; arrow_rect.y2 = 4;
		rtgui_rect_moveto_align(&tmp_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		if(ppl->flag & PROPEL_UNVISIBLE_UP)
			RTGUI_DC_FC(dc) = dark_grey;
		else
			RTGUI_DC_FC(dc) = black;
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _up_arrow);

		tmp_rect.y1 = tmp_rect.y2;
		tmp_rect.y2 = rect.y2;
		rtgui_rect_inflate(&tmp_rect, -2);
		rtgui_dc_fill_rect(dc, &tmp_rect);
		rtgui_rect_inflate(&tmp_rect, 2);
		if(ppl->flag & PROPEL_FLAG_DOWN)
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_SUNKEN);
		else
			rtgui_dc_draw_border(dc, &tmp_rect, RTGUI_BORDER_RAISE);
		
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 7; arrow_rect.y2 = 4;
		rtgui_rect_moveto_align(&tmp_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		if(ppl->flag & PROPEL_UNVISIBLE_DOWN)
			RTGUI_DC_FC(dc) = dark_grey;
		else
			RTGUI_DC_FC(dc) = black;
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _down_arrow);
	}

	rtgui_dc_end_drawing(dc);
}

static void rtgui_prople_onclick(rtgui_propel_t* ppl)
{
	if(ppl == RT_NULL) return;

	if(ppl->wdtlnk != RT_NULL && ppl->on_click != RT_NULL)
	{
		ppl->on_click(ppl->wdtlnk, RT_NULL);
	}
}

static void rtgui_propel_onmouse(rtgui_propel_t* ppl, rtgui_event_mouse_t* emouse)
{
	rtgui_rect_t rect, tmp_rect;

	/* get physical extent information */
	rtgui_widget_get_rect(ppl, &rect);
	rtgui_widget_rect_to_device(ppl, &rect);

	if(rtgui_rect_contains_point(&rect, emouse->x, emouse->y) == RT_EOK) 
	{
		tmp_rect = rect;
		if(emouse->button & RTGUI_MOUSE_BUTTON_DOWN)
		{	
			if(ppl->orient == RTGUI_HORIZONTAL)
			{
				tmp_rect.x2 = tmp_rect.x1 + rtgui_rect_width(tmp_rect)/2;
				if(rtgui_rect_contains_point(&tmp_rect, emouse->x, emouse->y) == RT_EOK)
				{	
					if(ppl->flag & PROPEL_UNVISIBLE_LEFT) return;
					ppl->flag |= PROPEL_FLAG_LEFT;
					if(ppl->bind != RT_NULL)
					{	
						if(*(ppl->bind) > ppl->range_min) (*(ppl->bind))--;
						if(*(ppl->bind) <= ppl->range_min) 
						{	
							ppl->flag |= PROPEL_UNVISIBLE_LEFT;
						}
						if(ppl->flag & PROPEL_UNVISIBLE_RIGHT)
						{
							if(*(ppl->bind) < ppl->range_max)
							{
								ppl->flag &= ~PROPEL_UNVISIBLE_RIGHT;
							}
						}
					} 
					rtgui_propel_ondraw(ppl);
					rtgui_prople_onclick(ppl);
					return;
				}
				tmp_rect.x1 = tmp_rect.x2;
				tmp_rect.x2 = rect.x2;
				if(rtgui_rect_contains_point(&tmp_rect, emouse->x, emouse->y) == RT_EOK)
				{
					if(ppl->flag & PROPEL_UNVISIBLE_RIGHT) return;
					ppl->flag |= PROPEL_FLAG_RIGHT;
					if(ppl->bind != RT_NULL)
					{	
						if(*(ppl->bind) < ppl->range_max) (*(ppl->bind))++;
						if(*(ppl->bind) >= ppl->range_max) 
						{
							ppl->flag |= PROPEL_UNVISIBLE_RIGHT;
						}
						if(ppl->flag & PROPEL_UNVISIBLE_LEFT)
						{
							if(*(ppl->bind) > ppl->range_min)
							{
								ppl->flag &= ~PROPEL_UNVISIBLE_LEFT;
							}
						}
					} 
					rtgui_propel_ondraw(ppl);
					rtgui_prople_onclick(ppl);
					return;
				}	
			}
			else if(ppl->orient == RTGUI_VERTICAL)
			{
				tmp_rect.y2 = tmp_rect.y1 + rtgui_rect_height(tmp_rect)/2;
				if(rtgui_rect_contains_point(&tmp_rect, emouse->x, emouse->y) == RT_EOK)
				{
					if(ppl->flag & PROPEL_UNVISIBLE_UP) return;
					ppl->flag |= PROPEL_FLAG_UP;
					if(ppl->bind != RT_NULL)
					{
						if(*(ppl->bind) > ppl->range_min) (*(ppl->bind))--;
						if(*(ppl->bind) <= ppl->range_min) 
						{
							ppl->flag |= PROPEL_UNVISIBLE_UP;
						}
						if(ppl->flag & PROPEL_UNVISIBLE_DOWN)
						{
							if(*(ppl->bind) < ppl->range_max)
							{
								ppl->flag &= ~PROPEL_UNVISIBLE_DOWN;
							}
						}
					}
					rtgui_propel_ondraw(ppl);
					rtgui_prople_onclick(ppl);
					return;
				}
				tmp_rect.y1 = tmp_rect.y2;
				tmp_rect.y2 = rect.y2;
				if(rtgui_rect_contains_point(&tmp_rect, emouse->x, emouse->y) == RT_EOK)
				{
					if(ppl->flag & PROPEL_UNVISIBLE_DOWN) return;
					ppl->flag |= PROPEL_FLAG_DOWN;
					if(ppl->bind != RT_NULL)
					{
						if(*(ppl->bind) < ppl->range_max) (*(ppl->bind))++;
						if(*(ppl->bind) >= ppl->range_max) 
						{
							ppl->flag |= PROPEL_UNVISIBLE_DOWN;
						}
						if(ppl->flag & PROPEL_UNVISIBLE_UP)
						{
							if(*(ppl->bind) > ppl->range_min)
							{
								ppl->flag &= ~PROPEL_UNVISIBLE_UP;
							}
						}
					}
					rtgui_propel_ondraw(ppl);
					rtgui_prople_onclick(ppl);
					return;
				}
			}
		}
		if(emouse->button & RTGUI_MOUSE_BUTTON_UP)
		{
			ppl->flag &= ~PROPEL_FLAG_MASK;
			rtgui_propel_ondraw(ppl);	
		}
	}
}

rt_bool_t rtgui_propel_event_handler(PVOID wdt, rtgui_event_t* event)
{
	rtgui_widget_t* widget = wdt;
	rtgui_propel_t* ppl = wdt;

	RT_ASSERT(wdt != RT_NULL);

	switch (event->type)
	{
		case RTGUI_EVENT_PAINT:
			rtgui_propel_ondraw(ppl);
			break;

		case RTGUI_EVENT_MOUSE_BUTTON:
			if(widget->on_mouseclick != RT_NULL) 
			{
				widget->on_mouseclick(widget, event);
			}
			else
			{	
				rtgui_propel_onmouse(ppl, (rtgui_event_mouse_t*)event);
			}
			break;

		default:
			break;
	}

	return RT_FALSE;
}

//parent必须是一个容器类控件
rtgui_propel_t* rtgui_propel_create(PVOID parent, int left, int top, int w, int h, int orient)
{
    rtgui_propel_t* ppl;
	
	RT_ASSERT(parent != RT_NULL);

    ppl = rtgui_widget_create(RTGUI_PROPEL_TYPE);
    if(ppl != RT_NULL)
    {
		rtgui_rect_t rect;
		rtgui_widget_get_rect(parent, &rect);
		rtgui_widget_rect_to_device(parent,&rect);
		rect.x1 += left;
		rect.y1 += top;
		rect.x2 = rect.x1+w;
		rect.y2 = rect.y1+h;
		rtgui_widget_set_rect(ppl, &rect);
		rtgui_container_add_child(parent, ppl);
		ppl->orient = orient;
    }
    return ppl;
}

void rtgui_ppl_destroy(rtgui_propel_t* ppl)
{
	rtgui_widget_destroy(ppl);
}

/* bind a external variable */
void rtgui_propel_bind(rtgui_propel_t *ppl, rt_uint32_t *var)
{
	if(ppl != RT_NULL)
	{
		ppl->bind = var;
	}
}

/* terminate binding relation */
void rtgui_propel_unbind(rtgui_propel_t *ppl)
{
	if(ppl != RT_NULL)
	{
		ppl->bind = RT_NULL;
	}		
}
