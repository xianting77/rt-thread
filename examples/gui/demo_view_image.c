#include "demo_view.h"
#include <rtgui/widgets/button.h>
#include <rtgui/widgets/filelist_view.h>

static rtgui_image_t* image = RT_NULL;
static rtgui_view_t* _view = RT_NULL;

static void open_btn_onbutton(rtgui_widget_t* widget, struct rtgui_event* event)
{
	/* create a file list view */
	rtgui_filelist_view_t *view;
	rtgui_workbench_t *workbench;
	rtgui_rect_t rect;

	workbench = RTGUI_WORKBENCH(rtgui_widget_get_toplevel(widget));
	rtgui_widget_get_rect(RTGUI_WIDGET(workbench), &rect);

#ifdef _WIN32
	view = rtgui_filelist_view_create(workbench, "d:\\", "*.*", &rect);
#else
	view = rtgui_filelist_view_create(workbench, "/", "*.*", &rect);
#endif
	if (rtgui_view_show(RTGUI_VIEW(view), RT_TRUE) == RTGUI_MODAL_OK)
	{
		char path[32], image_type[8];

		/* set label */
		rtgui_filelist_get_fullpath(view, path, sizeof(path));
		if (image != RT_NULL) rtgui_image_destroy(image);

		rt_memset(image_type, 0, sizeof(image_type));

		/* ���ͼ������� */
		if (rt_strstr(path, ".png") != RT_NULL) strcat(image_type, "png");
		if (rt_strstr(path, ".jpg") != RT_NULL) strcat(image_type, "jpeg");
		if (rt_strstr(path, ".hdc") != RT_NULL) strcat(image_type, "hdc");
		if (image_type[0] != '\0')
			image = rtgui_image_create_from_file(image_type, path, RT_TRUE);
	}

	/* ɾ�� �ļ��б� ��ͼ */
	rtgui_view_destroy(RTGUI_VIEW(view));
	rtgui_view_show(_view, RT_FALSE);
}

static rt_bool_t demo_view_event_handler(rtgui_widget_t* widget, rtgui_event_t *event)
{
	rt_bool_t result;

	/* ��Ĭ�ϵ��¼������� */
	result = rtgui_view_event_handler(widget, event);

	if (event->type == RTGUI_EVENT_PAINT)
	{
		struct rtgui_dc* dc;
		rtgui_rect_t rect;

		/* ��ÿؼ�������DC */
		dc = rtgui_dc_begin_drawing(widget);
		if (dc == RT_NULL) /* ��������������DC�����أ�����ؼ��򸸿ؼ�������״̬��DC�ǻ�ȡ���ɹ��ģ� */
			return RT_FALSE;

		/* ���demo view�����ͼ������ */
		demo_view_get_rect(RTGUI_VIEW(widget), &rect);

		/* ���ͼ����ʾ���� */
		rect.x1 += 5; rect.x2 -= 5;
		rect.y1 += 30; 

		if (image != RT_NULL)
			rtgui_image_blit(image, dc, &rect);

		/* ��ͼ��� */
		rtgui_dc_end_drawing(dc);
	}

	return result;
}

rtgui_view_t* demo_view_image(rtgui_workbench_t* workbench)
{
	rtgui_rect_t rect;
	rtgui_button_t* open_btn;

	_view = demo_view(workbench, "ͼ����ʾ");
	if (_view != RT_NULL)
		rtgui_widget_set_event_handler(RTGUI_WIDGET(_view), demo_view_event_handler);

	demo_view_get_rect(_view, &rect);
	rect.x1 += 5; rect.x2 = rect.x1 + 120;
	rect.y2 = rect.y1 + 20;
	open_btn = rtgui_button_create("��ͼ���ļ�");
	rtgui_container_add_child(RTGUI_CONTAINER(_view), RTGUI_WIDGET(open_btn));
	rtgui_widget_set_rect(RTGUI_WIDGET(open_btn), &rect);
	rtgui_button_set_onbutton(open_btn, open_btn_onbutton);

	return _view;
}
