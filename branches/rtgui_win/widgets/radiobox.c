#include <rtgui/dc.h>
#include <rtgui/rtgui_theme.h>
#include <rtgui/widgets/radiobox.h>

static rt_bool_t rtgui_radiobox_unfocus(PVOID wdt, rtgui_event_t* event);

static void _rtgui_radiobox_constructor(rtgui_radiobox_t *rbox)
{
	/* init widget and set event handler */
	RTGUI_WIDGET_FLAG(rbox) |= RTGUI_WIDGET_FLAG_FOCUSABLE;
	RTGUI_WIDGET_TEXTALIGN(rbox) = RTGUI_ALIGN_LEFT | RTGUI_ALIGN_CENTER_VERTICAL;
	rtgui_widget_set_onunfocus(rbox, rtgui_radiobox_unfocus);
	rtgui_widget_set_event_handler(rbox, rtgui_radiobox_event_handler);
	rbox->group = RT_NULL;
}

static void _rtgui_radiobox_destructor(rtgui_radiobox_t *rbox)
{
	if(rbox->name != RT_NULL)
	{
		rt_free(rbox->name);
		rbox->name = RT_NULL;
	}
	if(rbox->group != RT_NULL)
	{/* in same group, free once only. */
		if(rbox->group->rboxs != RT_NULL)
		{
			rt_free(rbox->group->rboxs);
			rbox->group->rboxs = RT_NULL;
		}
		rt_free(rbox->group);
		rbox->group = RT_NULL;
	}
}
rtgui_type_t *rtgui_radiobox_type_get(void)
{
	static rtgui_type_t *radiobox_type = RT_NULL;

	if(!radiobox_type)
	{
		radiobox_type = rtgui_type_create("radiobox", RTGUI_WIDGET_TYPE,
			sizeof(rtgui_radiobox_t), 
			RTGUI_CONSTRUCTOR(_rtgui_radiobox_constructor), 
			RTGUI_DESTRUCTOR(_rtgui_radiobox_destructor));
	}

	return radiobox_type;
}

/* create a radiobox widget */
rtgui_radiobox_t* rtgui_radiobox_create(PVOID parent, const char* name, 
			int left, int top, int w, int h, rtgui_rb_group_t* group)
{
    rtgui_radiobox_t* rbox;

	RT_ASSERT(parent != RT_NULL);

    rbox = rtgui_widget_create(RTGUI_RADIOBOX_TYPE);
    if(rbox != RT_NULL)
    {
		rtgui_rect_t rect;

		rtgui_widget_get_rect(parent,&rect);
		rtgui_widget_rect_to_device(parent,&rect);
		rect.x1 += left;
		rect.y1 += top;
		rect.x2 = rect.x1+w;
		rect.y2 = rect.y1+h;
		rtgui_widget_set_rect(rbox,&rect);
		rtgui_container_add_child(parent, rbox);

		rbox->name = rt_strdup(name);

		if(group == RT_NULL)
		{/* if radiobox group is null, create it right now. */
			rtgui_rb_group_t *grp = rt_malloc(sizeof(rtgui_rb_group_t));

			if(grp != RT_NULL) 
			{
				grp->item_count = 0;
				grp->item_sel = 0;
				grp->rboxs = rt_malloc(sizeof(rtgui_radiobox_t*));
				grp->bind_var = RT_NULL;
				group = grp;
			}
		}

		if(group != RT_NULL)
		{
			group->rboxs = rt_realloc(group->rboxs, 
					sizeof(rtgui_radiobox_t*)*(group->item_count+1));
			*(group->rboxs+group->item_count) = rbox;
			group->item_count++;

			rbox->group = group;
		}
	}

	return rbox;
}

void rtgui_radiobox_destroy(rtgui_radiobox_t* rbox)
{
	rtgui_widget_destroy(rbox);
}

static void rtgui_radiobox_onmouse(rtgui_radiobox_t* rbox, rtgui_event_mouse_t* event)
{
	RT_ASSERT(rbox != RT_NULL);
	RT_ASSERT(event  != RT_NULL);

	if(event->button & RTGUI_MOUSE_BUTTON_DOWN && event->button & RTGUI_MOUSE_BUTTON_LEFT)
	{
		rtgui_rect_t rect;

		/* focus widgets */
		rtgui_widget_focus(rbox);

		/* get widget physical rect */
		rtgui_widget_get_rect(rbox, &rect);
		rtgui_widget_rect_to_device(rbox, &rect);
		if(rtgui_rect_contains_point(&rect, event->x, event->y) == RT_EOK)
		{
			rtgui_rb_group_t *group = rbox->group;
			rtgui_radiobox_t *radio;
			int old_item = group->item_sel;
			int i;
			for(i=group->item_count-1; i>=0; i--)
			{/* iff one item , it break until 0, it's right */
				radio = *(group->rboxs+i);
				if(radio == rbox) break;
			}
			group->item_sel = i;
			if(group->bind_var != RT_NULL)
				*(group->bind_var) = group->item_sel;
			
			/* update rbox widget */
			rtgui_theme_draw_radiobox(*(group->rboxs + old_item));
			rtgui_theme_draw_radiobox(*(group->rboxs + i));	
		}
	}
}

rt_bool_t rtgui_radiobox_event_handler(PVOID wdt, rtgui_event_t* event)
{
	rtgui_widget_t *widget = (rtgui_widget_t*)wdt;
	rtgui_radiobox_t* rbox = (rtgui_radiobox_t*)widget;
	rtgui_rb_group_t *group;

	RT_ASSERT(rbox != RT_NULL);

	group = rbox->group;

	switch (event->type)
	{
		case RTGUI_EVENT_PAINT:
			if(widget->on_draw != RT_NULL) 
				widget->on_draw(widget, event);
			else
			{	
				rtgui_theme_draw_radiobox(rbox);
			}
			break;
	
		case RTGUI_EVENT_KBD:
			if(widget->on_key != RT_NULL) 
				widget->on_key(widget, event);
			else
			{
	            rtgui_event_kbd_t *e = (rtgui_event_kbd_t*)event;
	
	            /* set focused */
	            rtgui_widget_focus(rbox);
	            if(!(RTGUI_KBD_IS_UP(e))) return RT_FALSE;
	
	            if(e->key == RTGUIK_UP || e->key == RTGUIK_LEFT)
	            {
	                if(group->item_sel > 0)
					{
	                    if(group->bind_var != RT_NULL)
							*(group->bind_var) = group->item_sel - 1;
						rtgui_rb_group_set_sel(group, group->item_sel - 1);
					}
	            }
	            else if(e->key == RTGUIK_DOWN || e->key == RTGUIK_RIGHT)
	            {
	                if(group->item_sel < group->item_count - 1)
					{
	                    if(group->bind_var != RT_NULL)
							*(group->bind_var) = group->item_sel +1;
						rtgui_rb_group_set_sel(group, group->item_sel + 1);
					}
	            }
	        }
			break;
	
		case RTGUI_EVENT_MOUSE_BUTTON:
			if(widget->on_mouseclick != RT_NULL) 
				widget->on_mouseclick(widget, event);
			else
			{
				rtgui_radiobox_onmouse(rbox, (rtgui_event_mouse_t*)event);
			}
			break;
	}

	return RT_FALSE;
}

rtgui_rb_group_t* rtgui_radiobox_create_group(void)
{
	rtgui_rb_group_t *group=RT_NULL;
	
	group = rt_malloc(sizeof(rtgui_rb_group_t));
	if(group != RT_NULL)
	{
		group->item_count = 0;
		group->item_sel = 0;
		group->rboxs = rt_malloc(sizeof(rtgui_radiobox_t*));
		group->bind_var = RT_NULL;
	}

	return group;
}

rtgui_rb_group_t* rtgui_radiobox_get_group(rtgui_radiobox_t* rbox)
{
	RT_ASSERT(rbox != RT_NULL);

	return rbox->group;
}

/* bind a external variable */
void rtgui_rb_group_bind(rtgui_rb_group_t *group, rt_uint32_t *var)
{
	if(group != RT_NULL)
	{
		group->bind_var = var;
	}
}

/* terminate binding relation */
void rtgui_rb_group_unbind(rtgui_rb_group_t *group)
{
	if(group != RT_NULL)
	{
		group->bind_var = RT_NULL;
	}		
}

/* set selection in group */
void rtgui_rb_group_set_sel(rtgui_rb_group_t* group, int sel)
{
	rt_uint16_t old;

	if(sel == group->item_sel) return;

	old = group->item_sel;
    if(sel >= 0 && sel < group->item_count)
    {
    	group->item_sel = sel;
		if(group->bind_var != RT_NULL)
			*(group->bind_var) = group->item_sel;
    }

    /* update rbox widget */
	rtgui_theme_draw_radiobox(*(group->rboxs + old));
	rtgui_theme_draw_radiobox(*(group->rboxs + sel));
}

int rtgui_rb_group_get_sel(rtgui_rb_group_t* group)
{
    if(group != RT_NULL)
	{
		return group->item_sel;
	}

	return 0;
}

static rt_bool_t rtgui_radiobox_unfocus(PVOID wdt, rtgui_event_t* event)
{
	rtgui_rect_t rect, item_rect;
	rtgui_radiobox_t *rbox = (rtgui_radiobox_t*)wdt;
	rtgui_dc_t *dc;
	rt_base_t len=0;

	RT_ASSERT(wdt != RT_NULL);

	dc = rtgui_dc_begin_drawing(wdt);
	if(dc == RT_NULL)return RT_FALSE;

	rtgui_widget_get_rect(rbox, &rect);

	item_rect = rect;
	rtgui_rect_moveto_align(&rect, &item_rect, RTGUI_WIDGET_TEXTALIGN(rbox));

	/* draw text */
	rtgui_font_get_string_rect(RTGUI_WIDGET_FONT(rbox), rbox->name, &item_rect);
	len = rtgui_rect_width(item_rect);
	rtgui_rect_moveto_align(&rect, &item_rect, RTGUI_WIDGET_TEXTALIGN(rbox));
	item_rect.x1 += RADIO_BOX_H + RTGUI_WIDGET_DEFAULT_MARGIN;
	item_rect.x2 = item_rect.x1 + len;
	
	if(!RTGUI_WIDGET_IS_FOCUSED(rbox))
	{//Çå³ý½¹µã¿ò
		rtgui_color_t color;
		color = RTGUI_DC_FC(dc);
		RTGUI_DC_FC(dc) = default_background;//RTGUI_DC_BC(dc);
		rtgui_dc_draw_focus_rect(dc,&item_rect);
		RTGUI_DC_FC(dc) = color;
	}

	rtgui_dc_end_drawing(dc);

	return RT_EOK;	
}
