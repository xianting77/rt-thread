/*
 * �����嵥��DC������ʾ
 *
 * ������ӻ��ڴ�������view�Ͻ���DC��������ʾ
 */

#include "demo_view.h"
#include <rtgui/dc.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/label.h>
#include <rtgui/widgets/slider.h>

/*
 * view���¼�������
 */
rt_bool_t dc_event_handler(rtgui_widget_t* widget, rtgui_event_t *event)
{
	/* ����PAINT�¼����д��� */
	if (event->type == RTGUI_EVENT_PAINT)
	{
		struct rtgui_dc* dc;
		rtgui_rect_t rect;
		rt_uint32_t vx[] = {20, 50, 60, 45, 60, 20};
		rt_uint32_t vy[] = {150, 50, 90, 60, 45, 50};

		/*
		 * ��Ϊ�õ���demo view�����汾����һ���ֿؼ��������ڻ�ͼʱ��Ҫ��demo view
		 * �Ȼ�ͼ
		 */
		rtgui_view_event_handler(widget, event);

		/************************************************************************/
		/* �������DC�Ĳ���                                                     */
		/************************************************************************/

		/* ��ÿؼ�������DC */
		dc = rtgui_dc_begin_drawing(widget);
		/* ��������������DC�����أ�����ؼ��򸸿ؼ�������״̬��DC�ǻ�ȡ���ɹ��ģ� */
		if (dc == RT_NULL)
			return RT_FALSE;

		/* ���demo view�����ͼ������ */
		demo_view_get_rect(RTGUI_VIEW(widget), &rect);

		RTGUI_DC_TEXTALIGN(dc) = RTGUI_ALIGN_BOTTOM | RTGUI_ALIGN_CENTER_HORIZONTAL;
		/* ��ʾGUI�İ汾��Ϣ */
#ifdef RTGUI_USING_SMALL_SIZE
		rtgui_dc_draw_text(dc, "RT-Thread/GUIС�Ͱ汾", &rect);
#else
		rtgui_dc_draw_text(dc, "RT-Thread/GUI��׼�汾", &rect);
#endif

		/* ����һ��Բ�� */
		RTGUI_DC_FC(dc) = red;
		rtgui_dc_draw_circle(dc, rect.x1 + 10, rect.y1 + 10, 10);

		/* ���һ��Բ�� */
		RTGUI_DC_FC(dc) = green;
		rtgui_dc_fill_circle(dc, rect.x1 + 30, rect.y1 + 10, 10);

		/* ��һ��Բ�� */
		RTGUI_DC_FC(dc) = RTGUI_RGB(250, 120, 120);
		rtgui_dc_draw_arc(dc, rect.x1 + 120, rect.y1 + 60, 30, 0, 120);

		/* ����� */
		RTGUI_DC_FC(dc) = blue;
		rtgui_dc_draw_polygon(dc, vx, vy, 6);

		/* ���Ʋ�ͬ�ı߿� */
		{
			rtgui_rect_t rect = {0, 0, 16, 16};
			rtgui_rect_moveto(&rect, 30, 120);

			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_RAISE);
			rect.x1 += 20;
			rect.x2 += 20 + 50;
			rtgui_dc_draw_text(dc, "raise", &rect);
			rect.x1 -= 20;
			rect.x2 -= 20 + 50;
			rect.y1 += 20;
			rect.y2 += 20;

			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_SIMPLE);
			rect.x1 += 20;
			rect.x2 += 20 + 50;
			rtgui_dc_draw_text(dc, "simple", &rect);
			rect.x1 -= 20;
			rect.x2 -= 20 + 50;
			rect.y1 += 20;
			rect.y2 += 20;

			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_SUNKEN);
			rect.x1 += 20;
			rect.x2 += 20 + 50;
			rtgui_dc_draw_text(dc, "sunken", &rect);
			rect.x1 -= 20;
			rect.x2 -= 20 + 50;
			rect.y1 += 20;
			rect.y2 += 20;

			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_BOX);
			rect.x1 += 20;
			rect.x2 += 20 + 50;
			rtgui_dc_draw_text(dc, "box", &rect);
			rect.x1 -= 20;
			rect.x2 -= 20 + 50;
			rect.y1 += 20;
			rect.y2 += 20;

			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_STATIC);
			rect.x1 += 20;
			rect.x2 += 20 + 50;
			rtgui_dc_draw_text(dc, "static", &rect);
			rect.x1 -= 20;
			rect.x2 -= 20 + 50;
			rect.y1 += 20;
			rect.y2 += 20;

			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_EXTRA);
			rect.x1 += 20;
			rect.x2 += 20 + 50;
			rtgui_dc_draw_text(dc, "extra", &rect);
			rect.x1 -= 20;
			rect.x2 -= 20 + 50;
			rect.y1 += 20;
			rect.y2 += 20;
		}

		/* ��ͼ��� */
		rtgui_dc_end_drawing(dc);
	}
	else
	{
		/* �����¼�������Ĭ�ϵ��¼������� */
		return rtgui_view_event_handler(widget, event);
	}

	return RT_FALSE;
}

/* ��������DC������ʾ�õ���ͼ */
rtgui_view_t *demo_view_dc(rtgui_workbench_t* workbench)
{
	rtgui_view_t *view;

	view = demo_view(workbench, "DC Demo");
	if (view != RT_NULL)
		/* ���ó��Լ����¼������� */
		rtgui_widget_set_event_handler(RTGUI_WIDGET(view), dc_event_handler);

	return view;
}
