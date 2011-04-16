/*
 * File      : fileview.c
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2010, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-01-06     Bernard      first version
 */

#include <rtgui/rtgui.h>
#include <rtgui/rtgui_object.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/rtgui_theme.h>

#include <rtgui/list.h>
#include <rtgui/image.h>
#include <rtgui/widgets/view.h>
#include <rtgui/widgets/filelist_view.h>
#include <rtgui/widgets/listbox.h>
#include <rtgui/widgets/window.h>

#if defined(RTGUI_USING_DFS_FILERW) || defined(RTGUI_USING_STDIO_FILERW)
#ifdef _WIN32
#include <io.h>
#include <dirent.h>
#include <sys/stat.h>
#define PATH_SEPARATOR		'\\'
#else
#include <dfs_posix.h>
#define PATH_SEPARATOR		'/'
#endif

#include <string.h>

const static char * file_xpm[] = {
"16 16 21 1",
" 	c None",
".	c #999999",
"+	c #818181",
"@	c #FFFFFF",
"#	c #ECECEC",
"$	c #EAEAEA",
"%	c #EBEBEB",
"&	c #EDEDED",
"*	c #F0F0F0",
"=	c #C4C4C4",
"-	c #C5C5C5",
";	c #C6C6C6",
">	c #C7C7C7",
",	c #EEEEEE",
"'	c #EDEDE5",
")	c #EDEDE6",
"!	c #EFEFEF",
"~	c #C8C8C8",
"{	c #F1F1F1",
"]	c #F2F2F2",
"^	c #959595",
".++++++++++++   ",
"+@@@@@@@@@@@@+  ",
"+@#$$%%%##&*@+  ",
"+@$=--;;;;>*@+  ",
"+@$%%###&&,*@+  ",
"+@%-;;;;;;>*@+  ",
"+@%%##&&'#,*@+  ",
"+@%;;;;,,),*@+  ",
"+@##&&,,!!!*@+  ",
"+@#;;;>>~~~*@+  ",
"+@#&,,!!*{{{@+  ",
"+@&;>>~~~{{]@+  ",
"+@&&,!!**{]]@+  ",
"+@@@@@@@@@@@@+  ",
"^++++++++++++^  ",
"                "};

const static char * folder_xpm[] = {
"16 16 121 2",
"  	c None",
". 	c #D9B434",
"+ 	c #E1C25E",
"@ 	c #E2C360",
"# 	c #E2C35F",
"$ 	c #DBB63C",
"% 	c #DAB336",
"& 	c #FEFEFD",
"* 	c #FFFFFE",
"= 	c #FFFEFE",
"- 	c #FFFEFD",
"; 	c #FBF7EA",
"> 	c #E4C76B",
", 	c #E3C76B",
"' 	c #E6CD79",
") 	c #E5CA74",
"! 	c #DAAF35",
"~ 	c #FEFCF7",
"{ 	c #F8E48E",
"] 	c #F5DE91",
"^ 	c #F5E09F",
"/ 	c #F6E1AC",
"( 	c #FEFBEF",
"_ 	c #FEFDF4",
": 	c #FEFCF3",
"< 	c #FEFCF1",
"[ 	c #FEFBEE",
"} 	c #FFFDFA",
"| 	c #DAAF36",
"1 	c #DAAA36",
"2 	c #FDFAF1",
"3 	c #F5DE94",
"4 	c #F4DC93",
"5 	c #F2D581",
"6 	c #EDCA6A",
"7 	c #EACB6C",
"8 	c #EFD385",
"9 	c #EFD280",
"0 	c #EFD07A",
"a 	c #EECF76",
"b 	c #EECF72",
"c 	c #FBF7E9",
"d 	c #DAAE34",
"e 	c #DAAB35",
"f 	c #FBF6E8",
"g 	c #EFD494",
"h 	c #EECE88",
"i 	c #E9C173",
"j 	c #F6E9C9",
"k 	c #FEFCF2",
"l 	c #FEFCF0",
"m 	c #DAAB36",
"n 	c #DAA637",
"o 	c #FFFDF8",
"p 	c #FFFDF6",
"q 	c #FFFCF5",
"r 	c #FCF6D8",
"s 	c #F8E694",
"t 	c #F7E385",
"u 	c #F6DF76",
"v 	c #F5DB68",
"w 	c #F4D85C",
"x 	c #FCF4D7",
"y 	c #DAA435",
"z 	c #DAA136",
"A 	c #FEFCF6",
"B 	c #FCF2C8",
"C 	c #FBEFB9",
"D 	c #FAECAC",
"E 	c #F9E89C",
"F 	c #F7E38B",
"G 	c #F6E07C",
"H 	c #F6DC6C",
"I 	c #F5D95D",
"J 	c #F4D64F",
"K 	c #F3D344",
"L 	c #FCF3D0",
"M 	c #DA9F35",
"N 	c #DA9A36",
"O 	c #FDFAF2",
"P 	c #FAEDB3",
"Q 	c #F9E9A4",
"R 	c #F8E695",
"S 	c #F7E285",
"T 	c #F6DE76",
"U 	c #F5DB65",
"V 	c #F4D757",
"W 	c #F3D449",
"X 	c #F2D13B",
"Y 	c #F1CE30",
"Z 	c #FBF2CC",
"` 	c #DA9835",
" .	c #DA9435",
"..	c #FEFAEF",
"+.	c #F9E9A1",
"@.	c #F8E591",
"#.	c #F7E181",
"$.	c #F6DE72",
"%.	c #F5DA63",
"&.	c #F4D754",
"*.	c #F3D347",
"=.	c #F2D039",
"-.	c #F1CD2E",
";.	c #F0CB26",
">.	c #FBF2CA",
",.	c #D98E33",
"'.	c #FAF0DC",
").	c #F4DDA7",
"!.	c #F4DB9E",
"~.	c #F3DA96",
"{.	c #F3D88E",
"].	c #F3D786",
"^.	c #F2D47F",
"/.	c #F2D379",
"(.	c #F1D272",
"_.	c #F1D06C",
":.	c #F1CF69",
"<.	c #F8EAC2",
"[.	c #D8882D",
"}.	c #D8872D",
"|.	c #D8862C",
"                                ",
"                                ",
"                                ",
"  . + @ @ @ # $                 ",
"  % & * = - * ; > , , , ' )     ",
"  ! ~ { ] ^ / ( _ : < ( [ } |   ",
"  1 2 3 4 5 6 7 8 9 0 a b c d   ",
"  e f g h i j k : k l ( [ * m   ",
"  n * o p q : r s t u v w x y   ",
"  z A B C D E F G H I J K L M   ",
"  N O P Q R S T U V W X Y Z `   ",
"   ...+.@.#.$.%.&.*.=.-.;.>. .  ",
"  ,.'.).!.~.{.].^./.(._.:.<.,.  ",
"    [.}.[.[.[.[.[.[.[.[.}.[.|.  ",
"                                ",
"                                "};

/* image for file and folder */
static rtgui_image_t *file_image   = RT_NULL;
static rtgui_image_t *folder_image = RT_NULL;

static rt_bool_t rtgui_fileview_onunfocus(PVOID wdt, rtgui_event_t* event);
static rt_bool_t rtgui_fileview_sbar_handle(PVOID wdt, rtgui_event_t* event);

static void _rtgui_fileview_constructor(rtgui_filelist_view_t *fview)
{	
	/* set default widget rect and set event handler */
	rtgui_widget_set_event_handler(fview, rtgui_filelist_view_event_handler);
	rtgui_widget_set_onunfocus(fview, rtgui_fileview_onunfocus);
	
	RTGUI_WIDGET_FLAG(fview) |= RTGUI_WIDGET_FLAG_FOCUSABLE;

	fview->frist_aloc = 0;
	fview->now_aloc = 0;
	fview->item_count = 0;
	fview->item_per_page = 0;

	fview->current_dir = RT_NULL;
	fview->pattern = RT_NULL;
	RTGUI_WIDGET_BACKGROUND(fview) = white;
	RTGUI_WIDGET_TEXTALIGN(fview) = RTGUI_ALIGN_CENTER_VERTICAL;
	
	fview->on_item = RT_NULL;
	fview->dlgst = RT_NULL;

	if(file_image==RT_NULL)
		file_image = rtgui_image_create_from_mem("xpm",(rt_uint8_t*)file_xpm, sizeof(file_xpm), RT_TRUE);
	if(folder_image==RT_NULL)
		folder_image = rtgui_image_create_from_mem("xpm",(rt_uint8_t*)folder_xpm, sizeof(folder_xpm), RT_TRUE);	
}

static void _rtgui_fileview_destructor(rtgui_filelist_view_t *fview)
{
	rtgui_filelist_view_destroy(fview);	
}

rtgui_type_t *rtgui_filelist_view_type_get(void)
{
	static rtgui_type_t *fileview_type = RT_NULL;
	
	if(!fileview_type)
	{
		fileview_type = rtgui_type_create("fileview", RTGUI_CONTAINER_TYPE,
			sizeof(rtgui_filelist_view_t), 
			RTGUI_CONSTRUCTOR(_rtgui_fileview_constructor), 
			RTGUI_DESTRUCTOR(_rtgui_fileview_destructor));
	}

	return fileview_type;
}


rtgui_filelist_view_t* rtgui_filelist_view_create(PVOID parent, const char* directory, const char* pattern, int left, int top, int w, int h)
{
	rtgui_filelist_view_t* fview = RT_NULL;

	RT_ASSERT(parent != RT_NULL);

	/* create a new view */
	fview = rtgui_widget_create(RTGUI_FILELIST_VIEW_TYPE);

	if(fview != RT_NULL)
	{
		rtgui_rect_t rect;

		rtgui_widget_get_rect(parent, &rect);
		rtgui_widget_rect_to_device(parent,&rect);
		rect.x1 += left;
		rect.y1 += top;
		rect.x2 = rect.x1+w;
		rect.y2 = rect.y1+h;
		
		fview->items = RT_NULL;
		fview->pattern = rt_strdup(pattern);
		fview->item_per_page = rtgui_rect_height(rect) / (1 + SELECTED_HEIGHT);

		rtgui_widget_set_rect(fview,&rect);

		rtgui_container_add_child(parent, fview);
		
		{//创建卷标控件
			rt_uint32_t sLeft,sTop,sw=RTGUI_DEFAULT_SB_WIDTH,sLen;
			sLeft = rtgui_rect_width(rect)-RTGUI_WIDGET_BORDER(fview)-sw;
			sTop = RTGUI_WIDGET_BORDER(fview);

			sLen = rect.y2-rect.y1-RTGUI_WIDGET_BORDER(fview)*2;
			fview->sbar = rtgui_scrollbar_create(fview,sLeft,sTop,sw,sLen,RTGUI_VERTICAL);
			if(fview->sbar != RT_NULL)
			{
				fview->sbar->widgetlnk = fview;
				fview->sbar->on_scroll = rtgui_fileview_sbar_handle;
				RTGUI_WIDGET_HIDE(fview->sbar);//默认隐藏滚动条
			}
		}

		rtgui_filelist_view_set_directory(fview, directory);
	}

	return fview;
}

static void rtgui_filelist_view_clear(rtgui_filelist_view_t* view);

void rtgui_filelist_view_destroy(rtgui_filelist_view_t* fview)
{
    /* delete all file items */
    rtgui_filelist_view_clear(fview);

	/* delete current directory and pattern */
	if(fview->current_dir != RT_NULL)
		rt_free(fview->current_dir);
	fview->current_dir = RT_NULL;

	if(fview->pattern != RT_NULL)
		rt_free(fview->pattern);
	fview->pattern = RT_NULL;

	/* delete image */
	rtgui_image_destroy(file_image);
	file_image = RT_NULL;
	rtgui_image_destroy(folder_image);
	folder_image = RT_NULL;
}

/* set fview directory on top folder */
void rtgui_filelist_view_goto_topfolder(rtgui_filelist_view_t* fview)
{
	char* dirstr = fview->current_dir;

	if(strlen(dirstr) > 1)
	{
		char new_path[256];
		char* ptr = strrchr(dirstr,PATH_SEPARATOR);/* last char '/' */

		if(ptr == dirstr)
		{	/* It's root dir */
			new_path[0] = PATH_SEPARATOR;
			new_path[1] = '\0';
		}
		else
		{
			strncpy(new_path, dirstr, ptr - dirstr + 1);
			new_path[ptr - dirstr] = '\0';
		}
		dirstr = new_path;

		rtgui_filelist_view_set_directory(fview, dirstr);
	}
}

static void rtgui_filelist_view_on_folder_item(rtgui_filelist_view_t *fview)
{
	char* dir_ptr;

	dir_ptr = (char*)rt_malloc(256);
	if(dir_ptr==RT_NULL)
	{
		return;
	}
	/* no file, exit */
	if(fview->items==RT_NULL)
	{
		rt_free(dir_ptr);
		return;
	}

	rtgui_filelist_view_get_fullpath(fview, dir_ptr, 256);

	rtgui_filelist_view_set_directory(fview, dir_ptr);
	rt_free(dir_ptr);
}

static void rtgui_filelist_view_on_file_item(rtgui_filelist_view_t *fview)
{
	if(fview == RT_NULL) return;

	if(fview->on_item != RT_NULL)
		fview->on_item(fview, RT_NULL);		
}

void rtgui_filelist_view_ondraw(rtgui_filelist_view_t* fview)
{
	rt_uint16_t frist, i,rx2;
	rtgui_filelist_view_item_t* item;
	rtgui_rect_t rect, item_rect, image_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(fview != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(fview);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(fview, &rect); 
	rtgui_dc_draw_border(dc, &rect,RTGUI_WIDGET_BORDER_STYLE(fview));
	rtgui_rect_inflate(&rect,-RTGUI_WIDGET_BORDER(fview));
	RTGUI_DC_BC(dc) = white;
	rtgui_dc_fill_rect(dc,&rect);
	rtgui_rect_inflate(&rect,RTGUI_WIDGET_BORDER(fview));
	if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview->sbar))
		rect.x2 -= rtgui_rect_width(fview->sbar->parent.extent);
	
	rect.x2 -=1; rect.y2 -= 1;

	/* get item base rect */
	item_rect = rect; 
	item_rect.x1 += RTGUI_WIDGET_BORDER(fview);
	item_rect.x2 -= RTGUI_WIDGET_BORDER(fview); rx2 = item_rect.x2;
	item_rect.y1 += RTGUI_WIDGET_BORDER(fview);	
	item_rect.y2 = item_rect.y1 + (1 + SELECTED_HEIGHT);

	/* get image base rect */
	image_rect.x1 = RTGUI_WIDGET_DEFAULT_MARGIN; 
	image_rect.y1 = 0;
	image_rect.x2 = RTGUI_WIDGET_DEFAULT_MARGIN + file_image->w; 
	image_rect.y2 = file_image->h;
	rtgui_rect_moveto_align(&item_rect, &image_rect, RTGUI_ALIGN_CENTER_VERTICAL);

	/* get current page */
	frist = fview->frist_aloc;
	for(i = 0; i < fview->item_per_page; i ++)
	{	
		char str_size[16];

		if(frist + i >= fview->item_count) break;

		item = &(fview->items[frist + i]);

		/* draw item image */
		if(item->type == RTGUI_FITEM_FILE)
			rtgui_image_blit(file_image, dc, &image_rect);
		else
			rtgui_image_blit(folder_image, dc, &image_rect);

        /* draw text */
		item_rect.x1 += RTGUI_WIDGET_DEFAULT_MARGIN + file_image->w + 2;
		item_rect.x2 = item_rect.x1 + rtgui_font_get_string_width(RTGUI_DC_FONT(dc), item->name);
		if(frist + i == fview->now_aloc)
		{
			if(RTGUI_WIDGET_IS_FOCUSED(fview)) 
			{
				RTGUI_DC_BC(dc) = selected_color;
				RTGUI_DC_FC(dc) = white;
			}
			else
			{
				RTGUI_DC_BC(dc) = dark_grey;
				RTGUI_DC_FC(dc) = black;
			}
			rtgui_dc_fill_rect(dc, &item_rect);
			rtgui_dc_draw_text(dc, item->name, &item_rect);
		}
		else
		{
			/* draw background */
			RTGUI_DC_BC(dc) = white;
			RTGUI_DC_FC(dc) = black;
			rtgui_dc_fill_rect(dc,&item_rect);
			rtgui_dc_draw_text(dc, item->name, &item_rect);
		}

#if (1) /* please turn off it when need. */		
		if(item->type == RTGUI_FITEM_FILE)
		{	
			rtgui_rect_t rect=item_rect;
			/* print file information */
			rt_snprintf(str_size, 16, "(%dB)",item->size);
			rect.x1 = rect.x2 + RTGUI_WIDGET_DEFAULT_MARGIN;
			rect.x2 = rect.x1 + rt_strlen(str_size) * 
								rtgui_font_get_font_width(RTGUI_WIDGET_FONT(fview));
			RTGUI_DC_FC(dc) = black;
			rtgui_dc_draw_text(dc, str_size, &rect);
		}
#endif			
		item_rect.x1 -= RTGUI_WIDGET_DEFAULT_MARGIN + file_image->w + 2;
		item_rect.x2 = rx2;
        /* move to next item position */
		item_rect.y1 += (SELECTED_HEIGHT + 1);
		item_rect.y2 += (SELECTED_HEIGHT + 1);

		image_rect.y1 += (SELECTED_HEIGHT + 1);
		image_rect.y2 += (SELECTED_HEIGHT + 1);
	}

	if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview->sbar))
	{
		rtgui_theme_draw_scrollbar(fview->sbar);
	}

	rtgui_dc_end_drawing(dc);
}

//更新FileView的界面
void rtgui_filelist_view_update_current(rtgui_filelist_view_t* fview)
{
	rtgui_filelist_view_item_t* item;
	rtgui_rect_t rect, item_rect, image_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(fview != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(fview);
	if(dc == RT_NULL)return;

	//当文件夹为空时，不处理
	if(fview->items==RT_NULL)return;

	rtgui_widget_get_rect(fview, &rect);
	if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview->sbar))
		rect.x2 -= rtgui_rect_width(fview->sbar->parent.extent);

	if((fview->old_aloc >= fview->frist_aloc) && 
	   (fview->old_aloc < fview->frist_aloc+fview->item_per_page) &&
	   (fview->old_aloc != fview->now_aloc))
	{//这些条件,可以最大限度的减少绘图闪烁现象
		//取得旧的项目
		item_rect = rect;
		item_rect.x1 += RTGUI_WIDGET_BORDER(fview);
		item_rect.x2 -= RTGUI_WIDGET_BORDER(fview);
		item_rect.y1 += RTGUI_WIDGET_BORDER(fview);	
		item_rect.y1 += ((fview->old_aloc-fview->frist_aloc) % fview->item_per_page) * (1 + SELECTED_HEIGHT);
		item_rect.y2 = item_rect.y1 + (1 + SELECTED_HEIGHT);
	
		//取得图像矩形
		image_rect.x1 = RTGUI_WIDGET_DEFAULT_MARGIN; image_rect.y1 = 0;
		image_rect.x2 = RTGUI_WIDGET_DEFAULT_MARGIN + file_image->w; image_rect.y2 = file_image->h;
		rtgui_rect_moveto_align(&item_rect, &image_rect, RTGUI_ALIGN_CENTER_VERTICAL);
		
		//绘制旧的项目
		item = &(fview->items[fview->old_aloc]);
		if(item->type == RTGUI_FITEM_FILE) /* draw item image */
			rtgui_image_blit(file_image, dc, &image_rect);
		else
			rtgui_image_blit(folder_image, dc, &image_rect);
	
		item_rect.x1 += RTGUI_WIDGET_DEFAULT_MARGIN + file_image->w + 2;
		item_rect.x2 = item_rect.x1 + rtgui_font_get_string_width(RTGUI_DC_FONT(dc), item->name);
		RTGUI_DC_BC(dc) = white;
		RTGUI_DC_FC(dc) = black;
		rtgui_dc_fill_rect(dc,&item_rect);
		rtgui_dc_draw_text(dc, item->name, &item_rect);
	}
	//绘制当前项目
	item_rect = rect;
	item_rect.x1 += RTGUI_WIDGET_BORDER(fview);
	item_rect.x2 -= RTGUI_WIDGET_BORDER(fview);
	item_rect.y1 += RTGUI_WIDGET_BORDER(fview);
	item_rect.y1 += ((fview->now_aloc-fview->frist_aloc) % fview->item_per_page) * (1 + SELECTED_HEIGHT);
	item_rect.y2 = item_rect.y1 + (1 + SELECTED_HEIGHT);

	/* get image base rect */
	image_rect.x1 = RTGUI_WIDGET_DEFAULT_MARGIN; image_rect.y1 = 0;
	image_rect.x2 = RTGUI_WIDGET_DEFAULT_MARGIN + file_image->w; image_rect.y2 = file_image->h;
	rtgui_rect_moveto_align(&item_rect, &image_rect, RTGUI_ALIGN_CENTER_VERTICAL);

	item = &(fview->items[fview->now_aloc]);
	if(item->type == RTGUI_FITEM_FILE) /* draw item image */
		rtgui_image_blit(file_image, dc, &image_rect);
	else
		rtgui_image_blit(folder_image, dc, &image_rect);

	if(fview->dlgst != RT_NULL)
	{
		if(fview->dlgst->filename) rt_free(fview->dlgst->filename);
		fview->dlgst->filename = rt_strdup(item->name);
	}

	item_rect.x1 += RTGUI_WIDGET_DEFAULT_MARGIN + file_image->w + 2;
	item_rect.x2 = item_rect.x1 + rtgui_font_get_string_width(RTGUI_DC_FONT(dc), item->name);

	{
		if(RTGUI_WIDGET_IS_FOCUSED(fview)) 
		{
			RTGUI_DC_BC(dc) = selected_color;
			RTGUI_DC_FC(dc) = white;
		}
		else
		{
			RTGUI_DC_BC(dc) = dark_grey;
			RTGUI_DC_FC(dc) = black;	
		}
		rtgui_dc_fill_rect(dc, &item_rect);
		rtgui_dc_draw_text(dc, item->name, &item_rect);
	}
	
	if(fview->dlgst != RT_NULL)
	{
		if(item->type == RTGUI_FITEM_FILE)
			rtgui_textbox_set_value(fview->dlgst->tbox_filename,fview->dlgst->filename);
		RTGUI_DC_FC(dc) = black;
		rtgui_theme_draw_textbox(fview->dlgst->tbox_filename);
	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_filelist_view_on_enter(rtgui_filelist_view_t* fview)
{	
	if(fview->items==RT_NULL)return;
	if(fview->item_count==0)return;

	if(fview->items[fview->now_aloc].type == RTGUI_FITEM_DIR)
	{/* directory */
		rtgui_filelist_view_on_folder_item(fview);
	}
	else
	{/* file */
		rtgui_filelist_view_on_file_item(fview);
	}
}

rt_bool_t rtgui_filelist_view_event_handler(PVOID wdt, rtgui_event_t* event)
{
	rtgui_widget_t *widget = (rtgui_widget_t*)wdt;
	rtgui_filelist_view_t* fview = RT_NULL;

	fview = RTGUI_FILELIST_VIEW(widget);
	switch (event->type)
	{
		case RTGUI_EVENT_PAINT:
			rtgui_filelist_view_ondraw(fview);
			return RT_FALSE;
	
	    case RTGUI_EVENT_RESIZE:
	        {
				rtgui_event_resize_t* resize;
	
				resize = (rtgui_event_resize_t*)event;
	
	            /* recalculate page items */
				if(file_image != RT_NULL)
					fview->item_per_page = resize->h  / (1 + SELECTED_HEIGHT);
				else
					fview->item_per_page = resize->h / (2 + 14);
	        }
	        break;
	
		case RTGUI_EVENT_MOUSE_BUTTON:
			{
				rtgui_rect_t rect;
				rtgui_event_mouse_t* emouse;
	
				emouse = (rtgui_event_mouse_t*)event;
	
				//计算选择的项目位置
	
				rtgui_widget_focus(fview);
				/* get physical extent information */
				rtgui_widget_get_rect(fview, &rect);
				rtgui_widget_rect_to_device(fview, &rect);
				
				if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview->sbar))
					rect.x2 -= rtgui_rect_width(fview->sbar->parent.extent);
				
				if((rtgui_rect_contains_point(&rect, emouse->x, emouse->y) == RT_EOK) && fview->item_count>0)
				{
					rt_uint16_t i;
	
					i = (emouse->y - rect.y1) / (2 + SELECTED_HEIGHT);
					
					/* set focus */
					rtgui_widget_focus(fview);

					if((i < fview->item_count) && (i < fview->item_per_page))
					{
						//设置选择项
						if(emouse->button & RTGUI_MOUSE_BUTTON_DOWN)
						{
							fview->old_aloc = fview->now_aloc;
							fview->now_aloc = fview->frist_aloc + i;
							rtgui_filelist_view_update_current(fview);
						}
						else if(emouse->button & RTGUI_MOUSE_BUTTON_UP)
						{
							if(fview->now_aloc==fview->old_aloc) return RT_FALSE;

							rtgui_filelist_view_update_current(fview);
						}
						if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview))
						{
							if(!RTGUI_WIDGET_IS_HIDE(fview->sbar))
								rtgui_scrollbar_set_value(fview->sbar,fview->frist_aloc);
						}
					}
					return RT_TRUE;
				}
			}
			break;
	
	    case RTGUI_EVENT_KBD:
	        {
	            rtgui_event_kbd_t* ekbd = (rtgui_event_kbd_t*)event;
	            if(RTGUI_KBD_IS_DOWN(ekbd))
	            {	
					switch (ekbd->key)
	                {
		                case RTGUIK_UP: //一次上翻一条
							if(fview->now_aloc > 0)
							{
								fview->old_aloc = fview->now_aloc;
								fview->now_aloc --;

								if(fview->now_aloc < fview->frist_aloc)
								{//向上翻页了
									fview->frist_aloc = fview->now_aloc;
									rtgui_filelist_view_ondraw(fview);
								}
								else
								{//当前页中
									rtgui_filelist_view_update_current(fview);
								}

								if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview))
								{
									if(!RTGUI_WIDGET_IS_HIDE(fview->sbar))
										rtgui_scrollbar_set_value(fview->sbar,fview->frist_aloc);
								}
							}
							return RT_TRUE;
		
		                case RTGUIK_DOWN: //一次下翻一条
							if(fview->now_aloc < fview->item_count-1)
							{
								fview->old_aloc = fview->now_aloc;
								fview->now_aloc ++;
								
								if(fview->now_aloc >= fview->frist_aloc+fview->item_per_page)
								{//翻页了
									fview->frist_aloc++;
									rtgui_filelist_view_ondraw(fview);
								}
								else
								{//在当前页中
									rtgui_filelist_view_update_current(fview);
								}
								if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview))
								{
									if(!RTGUI_WIDGET_IS_HIDE(fview->sbar))
										rtgui_scrollbar_set_value(fview->sbar,fview->frist_aloc);
								}
							}
							return RT_TRUE;
		
						case RTGUIK_LEFT:
							if(fview->item_count==0)return RT_FALSE;
							fview->old_aloc = fview->now_aloc;
							fview->now_aloc -= fview->item_per_page;

							if(fview->now_aloc < 0)
								fview->now_aloc = 0;
							
							if(fview->now_aloc < fview->frist_aloc)
							{
								fview->frist_aloc = fview->now_aloc;
								rtgui_filelist_view_ondraw(fview);
							}
							else
							{
								rtgui_filelist_view_update_current(fview);
							}
							if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview))
							{
								if(!RTGUI_WIDGET_IS_HIDE(fview->sbar))
									rtgui_scrollbar_set_value(fview->sbar,fview->frist_aloc);
							}
							return RT_TRUE;
		
						case RTGUIK_RIGHT:
							if(fview->item_count==0)return RT_FALSE;
							fview->old_aloc = fview->now_aloc;
							fview->now_aloc += fview->item_per_page;

							if(fview->now_aloc > fview->item_count-1)
								fview->now_aloc = fview->item_count-1;
							
							if(fview->now_aloc >= fview->frist_aloc+fview->item_per_page)
							{
								fview->frist_aloc += fview->item_per_page;
								if(fview->frist_aloc >fview->item_count-fview->item_per_page)
								{
									fview->frist_aloc = fview->item_count-fview->item_per_page;
									fview->now_aloc = fview->frist_aloc;
								}
								rtgui_filelist_view_ondraw(fview);
							}
							else
							{
								rtgui_filelist_view_update_current(fview);
							}
							if(fview->sbar && !RTGUI_WIDGET_IS_HIDE(fview))
							{
								if(!RTGUI_WIDGET_IS_HIDE(fview->sbar))
									rtgui_scrollbar_set_value(fview->sbar,fview->frist_aloc);
							}
							return RT_TRUE;
		
						case RTGUIK_RETURN:
							rtgui_filelist_view_on_enter(fview);
							return RT_TRUE;
						case RTGUIK_BACKSPACE:
							rtgui_filelist_view_goto_topfolder(fview);
							return RT_TRUE;
		                default:
		                    break;
	                }
	            }
	        }
			return RT_FALSE;
		default:
			return rtgui_container_event_handler(widget, event);
	}

    /* use view event handler */
    return rtgui_container_event_handler(widget, event);
}

/* clear all file items */
static void rtgui_filelist_view_clear(rtgui_filelist_view_t* fview)
{
	rt_uint32_t index;
    rtgui_filelist_view_item_t* item;
	
	if(fview->items == RT_NULL) return;
		
	for(index = 0; index < fview->item_count; index ++)
	{
		item = &(fview->items[index]);

		/* release item name */
		if(item->name != RT_NULL)
		{
			rt_free(item->name);
			item->name = RT_NULL;
		}
	}

	/* release items */
	rt_free(fview->items);
	fview->items = RT_NULL;

	fview->item_count = 0;
	fview->now_aloc = 0;
}

void rtgui_filelist_view_set_directory(rtgui_filelist_view_t* fview, const char* directory)
{
	char fullpath[256];
	rtgui_filelist_view_item_t *item;

	fview->frist_aloc = 0; 

    //首先清除文件项目
    rtgui_filelist_view_clear(fview);
    if(directory != RT_NULL)
    {
		DIR* dir;
		struct stat s;
		rt_uint32_t i;
		struct dirent* dirent;

		fview->item_count = 0;
		//打开文件夹			   
        dir = opendir(directory); 
		if (dir == RT_NULL) return;

		//设置当前文件夹
		if(fview->current_dir != RT_NULL) 
			rt_free(fview->current_dir);
		fview->current_dir = rt_strdup(directory);

		if(fview->dlgst != RT_NULL)
		{
			if(fview->dlgst->path != RT_NULL) rt_free(fview->dlgst->path);
			fview->dlgst->path = rt_strdup(fview->current_dir);
	
			rtgui_textbox_set_value(fview->dlgst->tbox_path,fview->dlgst->path);
			rtgui_theme_draw_textbox(fview->dlgst->tbox_path);
		}

		do{
			dirent = readdir(dir);
			if(dirent == RT_NULL) break;
			fview->item_count ++; 
		}while (dirent != RT_NULL);
		closedir(dir);

		if((fview->item_count > fview->item_per_page) && fview->sbar!=RT_NULL)
		{
			RTGUI_WIDGET_UNHIDE(fview->sbar);
			rtgui_scrollbar_set_line_step(fview->sbar,1);
			rtgui_scrollbar_set_page_step(fview->sbar, fview->item_per_page);
			rtgui_scrollbar_set_range(fview->sbar, fview->item_count);
		}
		else
		{
			RTGUI_WIDGET_HIDE(fview->sbar);
		}
		rtgui_widget_update_clip(fview);

		//开辟一个可以放下所有项目的空间
		fview->items = (rtgui_filelist_view_item_t*) rt_malloc(sizeof(rtgui_filelist_view_item_t) * fview->item_count);

		if(fview->items == RT_NULL) goto __return; /*under the folder has not sub files. */

		//重新打开文件夹
		dir = opendir(directory);  //rt_kprintf("2.set_directory, dir=%x, directory=%s\n",dir,directory);
		if(dir == RT_NULL)  goto __return;

		for(i=0; i < fview->item_count; i ++)
		{
			dirent = readdir(dir);
			if(dirent == RT_NULL) break;

			item = &(fview->items[i]);
			item->name = rt_strdup(dirent->d_name);

			rt_memset(&s, 0, sizeof(struct stat));

			//构建每个文件的全路径
			if(directory[strlen(directory) - 1] != PATH_SEPARATOR)
				rt_sprintf(fullpath, "%s%c%s", directory, PATH_SEPARATOR, dirent->d_name);
			else
				rt_sprintf(fullpath, "%s%s", directory, dirent->d_name);

			stat(fullpath, &s);	 //rt_kprintf("fullpath=%-16s, st_mode=%x ",fullpath,s.st_mode);
			if( s.st_mode & S_IFDIR )
			{	//rt_kprintf("<DIR>\n");
				item->type = RTGUI_FITEM_DIR;
				item->size = 0;
			}
			else
			{	//rt_kprintf("<FILE>\n");
				item->type = RTGUI_FITEM_FILE;
				item->size = s.st_size;
			}
		}

		closedir(dir);
    }

	fview->now_aloc = 0;

__return:
    /* update view */
    rtgui_widget_update(fview);
}

void rtgui_filelist_view_get_fullpath(rtgui_filelist_view_t* view, char* path, rt_size_t len)
{
	RT_ASSERT(view != RT_NULL);

	if(view->current_dir[strlen(view->current_dir) - 1] != PATH_SEPARATOR)
		rt_snprintf(path, len, "%s%c%s",view->current_dir, PATH_SEPARATOR,
			view->items[view->now_aloc].name);
	else
		rt_snprintf(path, len, "%s%s",view->current_dir, 
			view->items[view->now_aloc].name);
}

static rt_bool_t rtgui_fileview_onunfocus(PVOID wdt, rtgui_event_t* event)
{
	rtgui_filelist_view_t *fview = (rtgui_filelist_view_t*)wdt;

	if(fview == RT_NULL) return RT_FALSE;

	if(!RTGUI_WIDGET_IS_FOCUSED(fview))
	{//清除焦点框
		rtgui_filelist_view_update_current(fview);	
	}

	return RT_TRUE;
}

static rt_bool_t rtgui_fileview_sbar_handle(PVOID wdt, rtgui_event_t* event)
{
	rtgui_filelist_view_t *fview = (rtgui_filelist_view_t*)wdt;

	fview->frist_aloc = fview->sbar->value;

	rtgui_filelist_view_ondraw(fview);

	return RT_TRUE;
}

#endif
