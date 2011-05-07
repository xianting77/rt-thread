#ifndef __RTGUI_PROPEL_H__
#define __RTGUI_PROPEL_H__

#include <rtgui/rtgui.h>
#include <rtgui/widgets/widget.h>

/** Gets the type of a button */
#define RTGUI_PROPEL_TYPE       (rtgui_propel_type_get())
/** Casts the object to an rtgui_label_t */
#define RTGUI_PROPEL(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_PROPEL_TYPE, rtgui_propel_t))
/** Checks if the object is an rtgui_label_t */
#define RTGUI_IS_PROPEL(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_PROPEL_TYPE))

#define PROPEL_FLAG_MASK		0x000F
#define PROPEL_FLAG_NONE    	0x0000
#define PROPEL_FLAG_LEFT  		0x0001
#define PROPEL_FLAG_RIGHT		0x0002
#define PROPEL_FLAG_UP			0x0004
#define PROPEL_FLAG_DOWN		0x0008

#define PROPEL_UNVISIBLE_MASK	0x00F0
#define PROPEL_UNVISIBLE_LEFT 	0x0010
#define PROPEL_UNVISIBLE_RIGHT	0x0020
#define PROPEL_UNVISIBLE_UP		0x0040
#define PROPEL_UNVISIBLE_DOWN	0x0080

/*
 * the propel widget
 */
struct rtgui_propel
{
	rtgui_widget_t 	parent;
	rt_uint32_t 	orient;
	rt_uint32_t     flag;
	rt_int16_t		range_min;
	rt_int16_t		range_max;
	rt_uint32_t*	bind;
	PVOID 			wdtlnk;
	rt_bool_t (*on_click) (PVOID wdt, rtgui_event_t* event);
};
typedef struct rtgui_propel rtgui_propel_t;

rtgui_type_t *rtgui_propel_type_get(void);

rtgui_propel_t* rtgui_propel_create(PVOID parent, int left, int top, int w, int h, int orient);
void rtgui_propel_destroy(rtgui_label_t* label);
rt_bool_t rtgui_propel_event_handler(PVOID wdt, rtgui_event_t* event);
void rtgui_propel_bind(rtgui_propel_t *ppl, rt_uint32_t *var);
void rtgui_propel_unbind(rtgui_propel_t *ppl);

#endif

