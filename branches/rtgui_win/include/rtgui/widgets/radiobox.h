#ifndef __RTGUI_RADIOBOX_H__
#define __RTGUI_RADIOBOX_H__

#include <rtgui/rtgui.h>
#include <rtgui/kbddef.h>
#include <rtgui/widgets/container.h>

/** Gets the type of a rbox */
#define RTGUI_RADIOBOX_TYPE       (rtgui_radiobox_type_get())
/** Casts the object to an rtgui_radiobox_t */
#define RTGUI_RADIOBOX(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_RADIOBOX_TYPE, rtgui_radiobox_t))
/** Checks if the object is an rtgui_radiobox_t */
#define RTGUI_IS_RADIOBOX(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_RADIOBOX_TYPE))

typedef struct rtgui_rb_group rtgui_rb_group_t;

/* rb group, rb => radiobox */
struct rtgui_rb_group
{
	rt_uint32_t	item_count;
	rt_uint16_t	item_sel;
	rtgui_radiobox_t **rboxs;
	rt_uint32_t* bind_var; /* binding user variable */
};

struct rtgui_radiobox
{
	rtgui_widget_t parent;
	char* name;
	rtgui_rb_group_t *group;
};

typedef struct rtgui_radiobox rtgui_radiobox_t;

rtgui_type_t *rtgui_radiobox_type_get(void);

rtgui_radiobox_t* rtgui_radiobox_create(PVOID parent, const char* name, 
			int left, int top, int w, int h, rtgui_rb_group_t* group);
void rtgui_radiobox_destroy(rtgui_radiobox_t* rbox);
rtgui_rb_group_t* rtgui_radiobox_create_group(void);
rtgui_rb_group_t* rtgui_radiobox_get_group(rtgui_radiobox_t* rbox);
/* bind a external variable */
void rtgui_rb_group_bind(rtgui_rb_group_t *group, rt_uint32_t *var);
/* terminate binding relation */
void rtgui_rb_group_unbind(rtgui_rb_group_t *group);
/* set selection in group */
void rtgui_rb_group_set_sel(rtgui_rb_group_t* group, int selection);
int rtgui_rb_group_get_sel(rtgui_rb_group_t* group);

rt_bool_t rtgui_radiobox_event_handler(PVOID wdt, rtgui_event_t* event);

#endif

