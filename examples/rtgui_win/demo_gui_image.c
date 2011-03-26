/*
 * �����嵥��DC����ʾͼ����ʾ
 *
 * ������ӻ��ڴ�������view����ʾͼ��
 */

#include "demo_view.h"
#include <rtgui/widgets/button.h>
#include <rtgui/widgets/filelist_view.h>
#include <string.h>

#if defined(RTGUI_USING_DFS_FILERW) || defined(RTGUI_USING_STDIO_FILERW)
/* �򿪰�ť�Ļص����� */
static void open_btn_onbutton(PVOID wdt, rtgui_event_t* event)
{
	
}

/* ��ʾ��ͼ���¼������� */
static rt_bool_t demo_view_event_handler(PVOID wdt, rtgui_event_t *event)
{
	rtgui_widget_t *widget = RTGUI_WIDGET(wdt);
	rt_bool_t result;

	/* �ȵ���Ĭ�ϵ��¼�������(����ֻ����PAINT�¼�������ʾ��ͼ���б����һЩ�ؼ�) */
	result = rtgui_view_event_handler(widget, event);

	if (event->type == RTGUI_EVENT_PAINT)
	{
		rtgui_dc_t* dc;

		/* ��ÿؼ�������DC */
		dc = rtgui_dc_begin_drawing(widget);
		if (dc == RT_NULL) return RT_FALSE;

		/* ��ͼ��� */
		rtgui_dc_end_drawing(dc);
	}

	return result;
}

/* ����������ʾͼ�����ʾ��ͼ */
rtgui_view_t* demo_gui_image(rtgui_view_t* parent_view)
{
	rtgui_button_t* open_btn;
	rtgui_view_t *view;
	rtgui_filelist_view_t *fview;
	

	/* �ȴ���һ����ʾ��ͼ */
	view = demo_view_create(parent_view, "ͼ����ʾ");

#ifdef _WIN32
	fview = rtgui_filelist_view_create(view, "d:\\", "*.*", 5, 65, 200, 180);
#else
	fview = rtgui_filelist_view_create(view, "/", "*.*", 5, 65, 200, 180);
#endif

	if (view != RT_NULL)
		/* ����Ĭ�ϵ��¼���������demo_view_event_handler���� */
		rtgui_widget_set_event_handler(view, demo_view_event_handler);

	/* ���һ����ť */
	open_btn = rtgui_button_create(view, "��ͼ���ļ�", 10, 40, 120, 22);
	rtgui_button_set_onbutton(open_btn, open_btn_onbutton);

	return view;
}
#endif
