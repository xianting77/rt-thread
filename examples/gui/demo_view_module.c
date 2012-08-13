/*
 * 3��D��??�̣���oDC��???��?��????Y��?
 *
 * ?a??��y����?��?�����?��3?��?view��???��?��???
 */

#include "demo_view.h"
#include <rtgui/widgets/button.h>
#include <rtgui/widgets/filelist_view.h>
#include <string.h>

#ifdef RT_USING_MODULE
#if defined(RTGUI_USING_DFS_FILERW) || defined(RTGUI_USING_STDIO_FILERW)
static rtgui_container_t *_view = RT_NULL;

/* �䨰?a���?����???�̡�o����y */
static void open_btn_onbutton(rtgui_widget_t *widget, struct rtgui_event *event)
{
    rtgui_filelist_view_t *view;
    rtgui_workbench_t *workbench;
    rtgui_rect_t rect;

    /* ??��??��2?��?workbench */
    workbench = RTGUI_WORKBENCH(rtgui_widget_get_toplevel(widget));
    rtgui_widget_get_rect(RTGUI_WIDGET(workbench), &rect);

    /* WIN32??������?o��??���̨�����?��?��?3?��??��??��|���� */
#ifdef _WIN32
    view = rtgui_filelist_view_create(workbench, "d:\\", "*.*", &rect);
#else
    view = rtgui_filelist_view_create(workbench, "/", "*.*", &rect);
#endif
    /* ?�ꨬ???��?��??????t��D����������?��?��?����1???��??��????��??????t */
    if (rtgui_container_show(RTGUI_CONTAINER(view), RT_TRUE) == RTGUI_MODAL_OK)
    {
        char path[32], name[8];

        /* ����?????t?��??��?����?? */
        rtgui_filelist_view_get_fullpath(view, path, sizeof(path));

        rt_memset(name, 0, sizeof(name));

        /* ??��?��|��??��?����?����D�� */
        if (rt_strstr(path, ".mo") != RT_NULL || rt_strstr(path, ".so") != RT_NULL)
        {
            rt_module_open(path);
        }
    }

    /* ��?3y ???t��D���� ������? */
    rtgui_container_destroy(RTGUI_CONTAINER(view));
    rtgui_container_show(_view, RT_FALSE);
}

/* ���?����?����??��?��|��??��?����??Y��?������? */
rtgui_container_t *demo_view_module(rtgui_workbench_t *workbench)
{
    rtgui_rect_t rect;
    rtgui_button_t *open_btn;

    /* ?�����?����????Y��?������? */
    _view = demo_view(workbench, "��|��??��?��?Y��?");

    /* ����?����???���?�� */
    demo_view_get_rect(_view, &rect);
    rect.x1 += 5;
    rect.x2 = rect.x1 + 120;
    rect.y2 = rect.y1 + 20;
    open_btn = rtgui_button_create("�䨰?a��|��??��?��");
    rtgui_container_add_child(RTGUI_CONTAINER(_view), RTGUI_WIDGET(open_btn));
    rtgui_widget_set_rect(RTGUI_WIDGET(open_btn), &rect);
    rtgui_button_set_onbutton(open_btn, open_btn_onbutton);

    return _view;
}
#endif
#endif

