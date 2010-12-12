/*
 * �����嵥��label�ؼ���ʾ
 *
 * ������ӻ��ڴ�������view����Ӽ�����ͬ���͵�label�ؼ�
 */
#include "demo_view.h"
#include <rtgui/widgets/label.h>
#include <rtgui/widgets/listctrl.h>

const static struct list_item
{
	const char* name;
	const char* gender;
	int age;
	rtgui_image_t* image;
} items[] = 
{
	{"bernard", "��", 30, RT_NULL},
	{"˾���", "��", 1024, RT_NULL},
	{"����", "��", 28, RT_NULL},
};

void _rtgui_listctrl_item_draw(struct rtgui_listctrl *list, struct rtgui_dc* dc, rtgui_rect_t* rect, rt_uint16_t index)
{
	char age_str[8];
	rtgui_rect_t item_rect;
	struct list_item *items, *item;

	item_rect = *rect;
	item_rect.x1 += 5;
	items = (struct list_item*)list->items;
	item = &items[index];

	if (item->image != RT_NULL)
	{
		rtgui_image_blit(item->image, dc, &item_rect);
		item_rect.x1 += item->image->w + 3;
	}

	/* draw text */
	rtgui_dc_draw_text(dc, item->name, &item_rect); item_rect.x1 += 80;
	rtgui_dc_draw_vline(dc, item_rect.x1, item_rect.y1, item_rect.y2);
	item_rect.x1 += 5;
	rtgui_dc_draw_text(dc, item->gender, &item_rect); item_rect.x1 += 80;
	rtgui_dc_draw_vline(dc, item_rect.x1, item_rect.y1, item_rect.y2);
	item_rect.x1 += 5;
	rt_snprintf(age_str, sizeof(age_str), "%d", item->age);
	rtgui_dc_draw_text(dc, age_str, &item_rect);
}

static void on_items(rtgui_widget_t* widget, struct rtgui_event* event)
{
	rtgui_listctrl_t* ctrl;
	/* get listctrl */
	ctrl = RTGUI_LISTCTRL(widget);

	/* ��ӡ��ǰ���� */
	rt_kprintf("current item: %d\n", ctrl->current_item);
}

/* ����������ʾlabel�ؼ�����ͼ */
rtgui_view_t* demo_view_listctrl(rtgui_workbench_t* workbench)
{
	rtgui_rect_t rect;
	rtgui_view_t* view;
	rtgui_label_t* label;
	rtgui_listctrl_t* box;

	/* �ȴ���һ����ʾ�õ���ͼ */
	view = demo_view(workbench, "List Control Demo");

	/* �����ͼ��λ����Ϣ */
	demo_view_get_rect(view, &rect);
	rect.x1 += 5;
	rect.x2 -= 5;
	rect.y1 += 5;
	rect.y2 = rect.y1 + 20;
	/* ����һ��label�ؼ� */
	label = rtgui_label_create("List Control: ");
	/* ����label��λ�� */
	rtgui_widget_set_rect(RTGUI_WIDGET(label), &rect);
	/* view��һ��container�ؼ�������add_child����������label�ؼ� */
	rtgui_container_add_child(RTGUI_CONTAINER(view), RTGUI_WIDGET(label));

	rect.y1 = rect.y2 + 3;
	rect.y2 = 250;
	box = rtgui_listctrl_create((rt_uint32_t)items, sizeof(items)/sizeof(items[0]), &rect,
		_rtgui_listctrl_item_draw);
	rtgui_listctrl_set_onitem(box, on_items);
	/* view��һ��container�ؼ�������add_child����������listctrl�ؼ� */
	rtgui_container_add_child(RTGUI_CONTAINER(view), RTGUI_WIDGET(box));

	return view;
}
