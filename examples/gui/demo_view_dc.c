#include "demo_view.h"
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/label.h>
#include <rtgui/widgets/slider.h>

rt_bool_t dc_event_handler(rtgui_widget_t* widget, rtgui_event_t *event)
{
	if (event->type == RTGUI_EVENT_PAINT)
	{
		struct rtgui_dc* dc;
		rtgui_rect_t rect;
		rt_uint32_t vx[] = {20, 50, 60, 45, 60, 20};
		rt_uint32_t vy[] = {150, 50, 90, 60, 45, 50};

		/* ��Ϊ�õ���demo view�����汾����һ���ֿؼ��������ڻ�ͼʱ��Ҫ�����Ȼ�ͼ */
		rtgui_view_event_handler(widget, event);

		/************************************************************************/
		/* �������DC�Ĵ���                                                     */
		/************************************************************************/

		/* ��ÿؼ�������DC */
		dc = rtgui_dc_begin_drawing(widget);
		if (dc == RT_NULL) /* ��������������DC�����أ�����ؼ��򸸿ؼ�������״̬��DC�ǻ�ȡ���ɹ��ģ� */
			return RT_FALSE;

		/* ���demo view�����ͼ������ */
		demo_view_get_rect(RTGUI_VIEW(widget), &rect);

		/* ����һ��Բ�� */
		rtgui_dc_set_color(dc, red);
		rtgui_dc_draw_circle(dc, rect.x1 + 10, rect.y1 + 10, 10);

		/* ���һ��Բ�� */
		rtgui_dc_set_color(dc, green);
		rtgui_dc_fill_circle(dc, rect.x1 + 30, rect.y1 + 10, 10);

		/* ����� */
		rtgui_dc_set_color(dc, blue);
		rtgui_dc_draw_polygon(dc, vx, vy, 6);

		/* ��ͼ��� */
		rtgui_dc_end_drawing(dc);
	}
	else
	{
		/* ����Ĭ�ϵ��¼������� */
		return rtgui_view_event_handler(widget, event);
	}

	return RT_FALSE;
}

rtgui_view_t *demo_view_dc(rtgui_workbench_t* workbench)
{
	rtgui_view_t *view;

	view = demo_view(workbench, "DC Demo");
	rtgui_widget_set_event_handler(RTGUI_WIDGET(view), dc_event_handler);

	return view;
}
