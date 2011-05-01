/*
 * File      : rtgui_theme.c
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-04     Bernard      first version
 */

#include <rtgui/rtgui.h>
#include <rtgui/color.h>
#include <rtgui/rtgui_server.h>
#include <rtgui/font.h>
#include <rtgui/image.h>
#include <panel.h>
#include <rtgui/rtgui_theme.h>

const rtgui_color_t default_foreground = RTGUI_RGB(0,   0,   0  );
const rtgui_color_t default_background = RTGUI_RGB(212, 208, 200);

extern rtgui_font_t rtgui_font_asc12;
extern rtgui_font_t rtgui_font_asc16;

/* init theme */
void rtgui_system_theme_init(void)
{
#if RTGUI_DEFAULT_FONT_SIZE == 16
	rtgui_font_set_defaut(&rtgui_font_asc16);
#else
	rtgui_font_set_defaut(&rtgui_font_asc12);
#endif
}

static const rt_uint8_t minbox_bmp[]=
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xF0,0x07,0xF0};
static const rt_uint8_t maxbox_bmp[]=
{0x0F,0xF8,0x0F,0xF8,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0F,0xF8};
static const rt_uint8_t normal_bmp[]=
{0x03,0xF8,0x03,0xF8,0x02,0x08,0x0F,0xE8,0x0F,0xE8,0x08,0x38,0x08,0x20,0x08,0x20,0x0F,0xE0};
static const rt_uint8_t close_bmp[] = 
{0x00,0x00,0x06,0x18,0x03,0x30,0x01,0xE0,0x00,0xC0,0x01,0xE0,0x03,0x30,0x06,0x18,0x00,0x00};


void rtgui_theme_draw_win_closebox(rtgui_win_t *win)
{
	rtgui_dc_t* dc;
	
	RT_ASSERT(win != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(win);
	if(dc == RT_NULL)return;

	if(win->style & RTGUI_WIN_CLOSEBOX)
	{/* draw close box */
		rtgui_rect_t rect;
		/* get close button rect */
		rtgui_win_get_closebox_rect(win, &rect);	
		rtgui_dc_fill_rect(dc, &rect);

		/* draw close box */
		if(win->style & RTGUI_WIN_CLOSEBOX_PRESSED)
		{
			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_SUNKEN);
			RTGUI_DC_FC(dc) = blue;
			rtgui_dc_draw_word(dc,rect.x1-0, rect.y1+4, 9, close_bmp);
		}
		else
		{
			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_RAISE);
			RTGUI_DC_FC(dc) = black;
			rtgui_dc_draw_word(dc,rect.x1-1, rect.y1+3, 9, close_bmp);
		}
	}
	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_win_maxbox(rtgui_win_t *win)
{
	rtgui_dc_t* dc;
	
	RT_ASSERT(win != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(win);
	if(dc == RT_NULL)return;

	if(win->style & RTGUI_WIN_MAXBOX)
	{/* maximum button */
		rtgui_rect_t rect;
		rtgui_win_get_maxbox_rect(win, &rect);
		rtgui_dc_fill_rect(dc, &rect);
		
		/* draw close box */
		if(win->style & RTGUI_WIN_MAXBOX_PRESSED)
		{
			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_SUNKEN);
			RTGUI_DC_FC(dc) = blue;
			if(!(win->status & RTGUI_WIN_STATUS_MAX))
				rtgui_dc_draw_word(dc,rect.x1-0, rect.y1+4, 9, maxbox_bmp);
			else
				rtgui_dc_draw_word(dc,rect.x1-1, rect.y1+3, 9, normal_bmp);
		}
		else
		{
			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_RAISE);
			RTGUI_DC_FC(dc) = black;
			if(!(win->status & RTGUI_WIN_STATUS_MAX))
				rtgui_dc_draw_word(dc,rect.x1-1, rect.y1+3, 9, maxbox_bmp);
			else
				rtgui_dc_draw_word(dc,rect.x1-1, rect.y1+3, 9, normal_bmp);
		}	
	}
	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_win_minbox(rtgui_win_t *win)
{
	rtgui_dc_t* dc;
	
	RT_ASSERT(win != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(win);
	if(dc == RT_NULL)return;

	if(win->style & RTGUI_WIN_MINBOX)
	{
		rtgui_rect_t rect;
		rtgui_win_get_minbox_rect(win, &rect);
		rtgui_dc_fill_rect(dc, &rect);
		
		/* draw close box */
		if(win->style & RTGUI_WIN_MINBOX_PRESSED)
		{
			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_SUNKEN);
			RTGUI_DC_FC(dc) = blue;
			rtgui_dc_draw_word(dc, rect.x1-0, rect.y1+4, 9, minbox_bmp);
		}
		else
		{
			rtgui_dc_draw_border(dc, &rect, RTGUI_BORDER_RAISE);
			RTGUI_DC_FC(dc) = black;
			rtgui_dc_draw_word(dc,rect.x1-1, rect.y1+3, 9, minbox_bmp);
		}
	}
	rtgui_dc_end_drawing(dc);
}

/* int TABLE_VARY_COLOR[24][3]={
{22,66,254},{22,65,251},{21,63,248},{20,61,245},{19,60,242},{19,58,239},{18,56,235},{17,55,233},
{17,53,230},{16,51,226},{15,50,224},{14,48,220},{14,47,217},{13,45,215},{12,43,211},{11,42,206},
{11,40,206},{10,38,202},{9, 37,199},{8, 35,197},{7, 34,195},{6, 33,193},{5, 31,191},{4, 29,197}
}; */

void rtgui_theme_draw_win_title(rtgui_win_t *win)
{
	rtgui_dc_t* dc;
	
	RT_ASSERT(win != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(win);
	if(dc == RT_NULL)return;

	/* draw title */
	if(win->style & RTGUI_WIN_TITLE)
	{
		rt_uint32_t i,j;
		rtgui_rect_t rect; 

		rtgui_win_get_title_rect(win, &rect);

		if(rtgui_win_is_activated(win))
		{
			RTGUI_DC_FC(dc) = RTGUI_RGB(30, 10,250);
			for(i = rect.y1,j=0; i < rect.y2; i ++,j++)
			{
				/* RTGUI_DC_FC(dc) = RTGUI_RGB(TABLE_VARY_COLOR[j][0],TABLE_VARY_COLOR[j][1],TABLE_VARY_COLOR[j][2]); */
				rtgui_dc_draw_hline(dc,rect.x1, rect.x2, i);
			}
		}
		else
		{
			RTGUI_DC_FC(dc) = RTGUI_RGB(128, 128, 128);
			for(i = rect.y1,j=0; i < rect.y2; i ++,j++)
			{
				rtgui_dc_draw_hline(dc,rect.x1, rect.x2, i);
			}

		}
		
		if (win->status & RTGUI_WIN_STATUS_ACTIVATE)
			RTGUI_DC_FC(dc) = white;
		else
			RTGUI_DC_FC(dc) = RTGUI_RGB(212, 208, 200);

		rect.x1 += 4;
		rect.y1 += 2; rect.y2 = rect.y1 + RTGUI_WIN_CLOSEBOX_HEIGHT;

		if(win->image != RT_NULL)
		{
			rtgui_image_paste(win->image, dc, &rect, white);
			rect.x1 += win->image->w+4;
		}
		
		RTGUI_DC_TEXTALIGN(dc) = RTGUI_ALIGN_LEFT;
		rtgui_dc_draw_text(dc,win->title, &rect);

		if(win->style & RTGUI_WIN_BORDER)/* 恢复原来的尺寸(前面缩放处理过) */
			rtgui_rect_inflate(&rect, RTGUI_WIDGET_BORDER(win));

		rtgui_theme_draw_win_closebox(win);
		rtgui_theme_draw_win_maxbox(win);
		rtgui_theme_draw_win_minbox(win);
	}

	rtgui_dc_end_drawing(dc);
}

/* window drawing */
void rtgui_theme_draw_win(rtgui_win_t* win)
{
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(win != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(win);
	if(dc == RT_NULL)return;

	/* get rect */
	rtgui_widget_get_rect(win, &rect);
	/* draw border */
	if(win->style & RTGUI_WIN_BORDER)
	{
		rect.x2 -= 1; rect.y2 -= 1;
		
		RTGUI_DC_FC(dc) = RTGUI_RGB(212, 208, 200);
		rtgui_dc_draw_hline(dc,rect.x1, rect.x2, rect.y1);
		rtgui_dc_draw_vline(dc,rect.x1, rect.y1, rect.y2);

		RTGUI_DC_FC(dc) = white;
		rtgui_dc_draw_hline(dc,rect.x1 + 1, rect.x2 - 1, rect.y1 + 1);
		rtgui_dc_draw_vline(dc,rect.x1 + 1, rect.y1 + 1, rect.y2 - 1);

		RTGUI_DC_FC(dc) = RTGUI_RGB(128, 128, 128);
		rtgui_dc_draw_hline(dc,rect.x1 + 1, rect.x2 - 0, rect.y2 - 1);
		rtgui_dc_draw_vline(dc,rect.x2 - 1, rect.y1 + 1, rect.y2 - 1);

		RTGUI_DC_FC(dc) = RTGUI_RGB(64, 64, 64);
		rtgui_dc_draw_hline(dc,rect.x1, rect.x2+1, rect.y2);
		rtgui_dc_draw_vline(dc,rect.x2, rect.y1, rect.y2);

		/* shrink border */
		rtgui_rect_inflate(&rect, -RTGUI_WIDGET_BORDER(win));
	}

	rtgui_theme_draw_win_title(win);
	/* fill client rect background color */
	rtgui_win_get_client_rect(win,&rect);
	rtgui_dc_fill_rect(dc,&rect);

	rtgui_dc_end_drawing(dc);
}

/* widget drawing */
void rtgui_theme_draw_button(rtgui_button_t* btn)
{
	/* draw button */
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(btn != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(btn);
	if(dc == RT_NULL)return;

	/* get widget rect */
	rtgui_widget_get_rect(btn, &rect);
	rtgui_dc_fill_rect(dc,&rect);

	if(btn->flag & RTGUI_BUTTON_FLAG_PRESS)
	{
		rtgui_dc_draw_border(dc, &rect,RTGUI_BORDER_DOWN);

		if(btn->image != RT_NULL)
		{
			rtgui_rect_t image_rect;
			rtgui_dc_draw_border(dc, &rect,RTGUI_BORDER_DOWN);
			image_rect.x1 = 0; image_rect.y1 = 0;
			image_rect.x2 = btn->image->w;
			image_rect.y2 = btn->image->h;
			rtgui_rect_moveto_align(&rect, &image_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
			rtgui_image_paste(btn->image, dc, &image_rect,white);
		}
	}
	else
	{
		if(btn->image != RT_NULL)
		{
			rtgui_rect_t image_rect;
			rtgui_dc_draw_border(dc, &rect,RTGUI_WIDGET_BORDER_STYLE(btn));
			image_rect.x1 = 0; image_rect.y1 = 0;
			image_rect.x2 = btn->image->w;
			image_rect.y2 = btn->image->h;
			rtgui_rect_moveto_align(&rect, &image_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
			rtgui_image_paste(btn->image, dc, &image_rect,white);
		}
		else
		{
			/* draw normal status border */
			rtgui_dc_draw_border(dc, &rect,RTGUI_WIDGET_BORDER_STYLE(btn));
		}
	}

	if(btn->image == RT_NULL)
	{
		/* re-set foreground and get default rect */
		rtgui_widget_get_rect(btn, &rect);
		/* remove border */
		rtgui_rect_inflate(&rect, -2);

		if(btn->flag & RTGUI_BUTTON_FLAG_PRESS)
		{
			rect.x1 += 1; rect.y1 += 1;
			rect.x2 += 1; rect.y2 += 1;
			rtgui_dc_draw_text(dc, rtgui_label_get_text(RTGUI_LABEL(btn)), &rect);
		}
		else
			rtgui_dc_draw_text(dc, rtgui_label_get_text(RTGUI_LABEL(btn)), &rect);
	}

	if(RTGUI_WIDGET_IS_FOCUSED(btn))
	{
		/* re-set foreground and get default rect */
		rtgui_widget_get_rect(btn, &rect);
		rtgui_rect_inflate(&rect, -2);
		rtgui_dc_draw_focus_rect(dc,&rect);	
	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_label(rtgui_label_t* label)
{
	/* draw label */
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(label != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(label);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(label, &rect);
	rtgui_dc_fill_rect(dc,&rect);
	
	/* default left and center draw */
	rtgui_dc_draw_text(dc, label->text, &rect);

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_textbox(rtgui_textbox_t* box)
{
	/* draw button */
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(box != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(box);
	if(dc == RT_NULL)return;

	/* get widget rect */
	rtgui_widget_get_rect(box, &rect);


	rtgui_rect_inflate(&rect,-RTGUI_WIDGET_BORDER(box));

	/* fill widget rect with white color */
	if(box->isedit == RT_TRUE)
	{
		RTGUI_DC_BC(dc) = white;
		rtgui_dc_fill_rect(dc,&rect);
	}
	else
	{
		if(RTGUI_IS_COMBO(RTGUI_WIDGET(box)->parent))
		{
			RTGUI_DC_BC(dc) = white;
			rtgui_dc_fill_rect(dc,&rect);
		}
		else
		{
			RTGUI_DC_BC(dc) = light_grey;
			rtgui_dc_fill_rect(dc,&rect);
		}
	}

	rtgui_rect_inflate(&rect,RTGUI_WIDGET_BORDER(box));
	/* draw border */
	rtgui_dc_draw_border(dc,&rect,RTGUI_WIDGET_BORDER_STYLE(box));

	/* draw text */
	if(box->text != RT_NULL)
	{
		rect.x1 += RTGUI_WIDGET_DEFAULT_MARGIN;
		if(box->flag & RTGUI_TEXTBOX_MULTI)
		{/* draw multiline text */
			int start,end,alllen,fh,fw,rw;
			fw  = rtgui_font_get_font_width(RTGUI_WIDGET_FONT(box));
			fh  = rtgui_font_get_font_height(RTGUI_WIDGET_FONT(box));
			rw  = rtgui_rect_width(rect); 
			alllen = rt_strlen(box->text);/* all character length */
			start=end=0;
			rect.y1 += 2;
			while(end<alllen)
			{/* is line feed? */
				if(*(box->text+end) == '\n')
				{
					int i, mlen;
					char* string;

					end++;
					mlen = end-start;
					
					/* we use rtgui_dc_draw_text(),need remake string,add '\0' at ending */
					string = rt_malloc(mlen);
					for(i=0;i<mlen;i++)
					{
						if(box->flag & RTGUI_TEXTBOX_MASK) /* ciphertext */
							string[i] = '*';
						else			
							string[i] = *(box->text+start+i);
						if(i==(mlen-1))string[i] = '\0';/* add '\0' by oneself */
					}
					RTGUI_DC_FC(dc) = black;
					
					for(i=0;i<mlen;)
					{
						int linelen = rw/fw;
						rect.y2 = rect.y1+fh;
						
						if(linelen > mlen)
						{
							linelen = mlen;
							RTGUI_DC_FC(dc) = black;
							rtgui_dc_draw_text(dc, string+i, &rect);
						}
						else 
						{/* auto-line feed */
							int k;
							char* tmpstr = rt_malloc(linelen+1);/* temporary memory */

							for(k=0;k<(linelen+1);k++)
							{
								if(box->flag & RTGUI_TEXTBOX_MASK) /* ciphertext */
									tmpstr[k] = '*';
								else						 /* plain code text */
									tmpstr[k] = *(string+i+k);
								if(k==linelen)tmpstr[k]='\0';/* add '\0' by oneself */
							}
							RTGUI_DC_FC(dc) = black;
							rtgui_dc_draw_text(dc, tmpstr, &rect);
							rt_free(tmpstr);
						}
						i += linelen;
						rect.y1 = rect.y2+1;
					}
					rt_free(string);

					start = end;/* next line start position */
				}
				end++;
			}
		}
		else
		{/* draw single text */
			RTGUI_DC_FC(dc) = black;
			if(box->flag & RTGUI_TEXTBOX_MASK)
			{
				/* draw '*' */
				rt_size_t len = rt_strlen(box->text);
				if(len > 0)
				{
					char *text_mask = rt_malloc(len + 1);
					rt_memset(text_mask, '*', len + 1);
					text_mask[len] = 0;
					rtgui_dc_draw_text(dc, text_mask, &rect);
					rt_free(text_mask);
				}
			}
			else
			{
				rtgui_dc_draw_text(dc, box->text, &rect);
			}
		}
	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_iconbox(rtgui_iconbox_t* iconbox)
{
	rtgui_rect_t rect,ico_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(iconbox != RT_NULL);
	
	/* begin drawing */
	dc = rtgui_dc_begin_drawing(iconbox);
	if(dc == RT_NULL)return;

	/* get widget rect */
	rtgui_widget_get_rect(iconbox, &rect);

	ico_rect.x1 = rect.x1+(rtgui_rect_width(rect)-iconbox->image->w)/2;
	ico_rect.y1 = rect.y1+(36-iconbox->image->h)/2+RTGUI_WIDGET_DEFAULT_MARGIN;
	ico_rect.x2 = ico_rect.x1+iconbox->image->w;
	ico_rect.y2 = ico_rect.y1+iconbox->image->h;

	/* draw icon */
	rtgui_image_blit(iconbox->image, dc, &ico_rect);

	/* draw text */
	RTGUI_DC_FC(dc) = white;
	if(iconbox->text_position == RTGUI_ICONBOX_TEXT_BELOW && iconbox->text != RT_NULL)
	{
		rect.y1 += iconbox->image->h + RTGUI_WIDGET_DEFAULT_MARGIN;
		rtgui_dc_draw_text(dc, iconbox->text, &rect);
	}
	else if(iconbox->text_position == RTGUI_ICONBOX_TEXT_RIGHT && iconbox->text != RT_NULL)
	{
		rect.x1 += iconbox->image->w + RTGUI_WIDGET_DEFAULT_MARGIN;
		rtgui_dc_draw_text(dc, iconbox->text, &rect);
	}

	rtgui_dc_end_drawing(dc);
}

static const rt_uint8_t checked_byte[7] = {0x02, 0x06, 0x8E, 0xDC, 0xF8, 0x70, 0x20};

void rtgui_theme_draw_checkbox(rtgui_checkbox_t* checkbox)
{
	rtgui_rect_t rect, box_rect;
	rtgui_dc_t* dc;
	rtgui_color_t color;
	
	RT_ASSERT(checkbox != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(checkbox);
	if(dc == RT_NULL)return;

	/* get rect */
	rtgui_widget_get_rect(checkbox, &rect);
	RTGUI_DC_BC(dc) = default_background;
    /* fill rect */
	rtgui_dc_fill_rect(dc,&rect);

	if(RTGUI_WIDGET_IS_FOCUSED(checkbox))
	{/* only draw focus rect surround string */
		rtgui_rect_t tmp_rect;
		rtgui_font_get_string_rect(RTGUI_WIDGET_FONT(checkbox), rtgui_label_get_text(RTGUI_LABEL(checkbox)), &tmp_rect);
		
		rtgui_rect_moveto(&tmp_rect,rect.x1+CHECK_BOX_W+7, rect.y1);
		tmp_rect.x1 -= 1;
		tmp_rect.x2 += 2;
		tmp_rect.y2 = rect.y2-1;
		/* draw focused border */
		rtgui_dc_draw_focus_rect(dc,&tmp_rect);
	}

	/* draw check box */
	box_rect.x1 = 0;
	box_rect.y1 = 0;
	box_rect.x2 = CHECK_BOX_W;
	box_rect.y2 = CHECK_BOX_H;
	rtgui_rect_moveto_align(&rect, &box_rect, RTGUI_ALIGN_CENTER_VERTICAL);
	box_rect.x1 += 2; box_rect.x2 += 2;

	rtgui_dc_draw_border(dc, &box_rect, RTGUI_BORDER_SUNKEN);
	rtgui_rect_inflate(&box_rect, -2);

	color = RTGUI_DC_BC(dc);
	RTGUI_DC_BC(dc) = white;
	rtgui_dc_fill_rect(dc, &box_rect);
	RTGUI_DC_BC(dc) = color;

	if(checkbox->value)
	{
		color = RTGUI_DC_FC(dc);
		RTGUI_DC_FC(dc) = RTGUI_RGB(33, 161, 33);
		rtgui_dc_draw_byte(dc,box_rect.x1 + 1, box_rect.y1 + 1, 7, checked_byte);
		RTGUI_DC_FC(dc) = color;
	}
	/* draw text */
	rect.x1 += rtgui_rect_height(rect) - 4 + 5;
	rtgui_dc_draw_text(dc, rtgui_label_get_text(RTGUI_LABEL(checkbox)), &rect);

	rtgui_dc_end_drawing(dc);
}

static const rt_uint8_t radio_unchecked_byte[] = 
{
	0x0f, 0x00, 0x30, 0xc0, 0x40, 0x20,
	0x40, 0x20,	0x80, 0x10,	0x80, 0x10,
	0x80, 0x10,	0x80, 0x10,	0x40, 0x20,
	0x40, 0x20,	0x30, 0xc0,	0x0f, 0x00,
};
static const rt_uint8_t radio_checked_byte[] = 
{
	0x0f, 0x00, 0x30, 0xc0, 0x40, 0x20, 
	0x40, 0x20, 0x86, 0x10, 0x8f, 0x10, 
	0x8f, 0x10, 0x86, 0x10, 0x40, 0x20, 
	0x40, 0x20, 0x30, 0xc0, 0x0f, 0x00,
};

void rtgui_theme_draw_radiobox(rtgui_radiobox_t* rbox)
{
	rtgui_rect_t rect,item_rect;
	rtgui_rb_group_t *group;
	rtgui_dc_t* dc;
	rt_base_t len=0,w;
	
	RT_ASSERT(rbox != RT_NULL);

	group = rbox->group;

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(rbox);
	if(dc == RT_NULL)return;

	/* get widget rect */
	rtgui_widget_get_rect(rbox, &rect);
	RTGUI_DC_BC(dc) = default_background;
	rtgui_dc_fill_rect(dc,&rect);

	item_rect = rect;
	/* draw radio */
	item_rect.x2 = item_rect.x1+RADIO_BOX_W;
	item_rect.y2 = item_rect.y1+RADIO_BOX_H;
	rtgui_rect_moveto_align(&rect, &item_rect, RTGUI_WIDGET_TEXTALIGN(rbox));
	rtgui_rect_inflate(&item_rect, -2);
	RTGUI_DC_FC(dc) = white;
	w = rtgui_rect_width(item_rect)/2;
	rtgui_dc_fill_circle(dc, item_rect.x1+w, item_rect.y1+w, w+2);
	rtgui_rect_inflate(&item_rect, 2);
	RTGUI_DC_FC(dc) = black;
	if (*(group->rboxs + group->item_sel) == rbox)
	{
		rtgui_dc_draw_word(dc, item_rect.x1, item_rect.y1, RADIO_BOX_H, radio_checked_byte);
	}
	else
	{
		rtgui_dc_draw_word(dc, item_rect.x1, item_rect.y1, RADIO_BOX_H, radio_unchecked_byte);
	}

	/* draw text */
	rtgui_font_get_string_rect(RTGUI_WIDGET_FONT(rbox), rbox->name, &item_rect);
	len = rtgui_rect_width(item_rect);
	rtgui_rect_moveto_align(&rect, &item_rect, RTGUI_WIDGET_TEXTALIGN(rbox));
	item_rect.x1 += RADIO_BOX_H + RTGUI_WIDGET_DEFAULT_MARGIN;
	item_rect.x2 = item_rect.x1 + len;
	rtgui_dc_draw_text(dc, rbox->name, &item_rect);
	
	if (*(group->rboxs + group->item_sel) == rbox)
	{/* draw focus rect */
		if (RTGUI_WIDGET_IS_FOCUSED(RTGUI_WIDGET(rbox)))
			rtgui_dc_draw_focus_rect(dc, &item_rect);
	}
	rtgui_dc_end_drawing(dc);
}

static const rt_uint8_t menu_popup_byte[7] = {0x40,0x60,0x70,0x78,0x70,0x60,0x40};

void rtgui_theme_draw_menu_item(rtgui_menu_t* menu, rtgui_menu_item_t *item)
{
	rtgui_rect_t rect, item_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(menu != RT_NULL);
	if(item == RT_NULL) return;

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(menu);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(menu, &rect);

	rtgui_rect_inflate(&rect, -RTGUI_WIDGET_BORDER(menu));
	item_rect = rect;

	if(menu->orient == RTGUI_HORIZONTAL)
	{
		int size = 0;
		rtgui_menu_item_t *menu_node = menu->head;
		while(menu_node != item)
		{/* calculate menu item display position */
			size += menu_node->item_width;
			menu_node = menu_node->next;
		}
		item_rect.x1 += size;
		item_rect.x2 = item_rect.x1 + menu_node->item_width;
	}
	else 
	{
		int size = 0;
		rtgui_menu_item_t *menu_node = menu->head;

		while(menu_node != item)
		{/* calculate menu item display position */
			size += menu_node->item_height;
			menu_node = menu_node->next;
		}
		/* set the text rect */
		item_rect.y1 += size;
		item_rect.y2 = item_rect.y1 + menu_node->item_height;
	}

	/* draw menu item */
	if(menu->now_item == item)
	{/* current item */
		RTGUI_DC_BC(dc) = selected_color;
		RTGUI_DC_FC(dc) = white;

		rtgui_dc_fill_rect(dc, &item_rect);
		if(item->image)
		{
			item_rect.x1 += item->image->w + RTGUI_WIDGET_BORDER(menu);
		}
		
		item_rect.x1 += RTGUI_MENU_MARGIN_W;
		rtgui_dc_draw_text(dc, item->caption, &item_rect);
		if((item->flag == RTGUI_MENU_POPUP) && (menu->orient==RTGUI_VERTICAL))
		{
			RTGUI_DC_FC(dc) = white;
			rtgui_dc_draw_byte(dc,rect.x2-8, item_rect.y1+7, 7, menu_popup_byte);
		}
		if(item->image)
		{
			item_rect.x1 -= item->image->w + RTGUI_WIDGET_BORDER(menu);
		}
		item_rect.x1 -= RTGUI_MENU_MARGIN_W;
	}
	else
	{/* other item */
		RTGUI_DC_BC(dc) = default_background;
		RTGUI_DC_FC(dc) = black;
		rtgui_dc_fill_rect(dc,&item_rect);
		if(item->image)
		{
			item_rect.x1 += item->image->w + RTGUI_WIDGET_BORDER(menu);
		}
		
		item_rect.x1 += RTGUI_MENU_MARGIN_W;
		rtgui_dc_draw_text(dc, item->caption, &item_rect);
		if((item->flag == RTGUI_MENU_POPUP) && (menu->orient==RTGUI_VERTICAL))
		{
			rtgui_dc_draw_byte(dc,rect.x2-8, item_rect.y1+7, 7, menu_popup_byte);
		}
		if(item->image)
		{
			item_rect.x1 -= item->image->w + RTGUI_WIDGET_BORDER(menu);
		}
		item_rect.x1 -= RTGUI_MENU_MARGIN_W;
	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_menu(rtgui_menu_t* menu)
{
	rtgui_rect_t rect, item_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(menu != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(menu);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(menu, &rect);

	rtgui_rect_inflate(&rect,-RTGUI_WIDGET_BORDER(menu));
	RTGUI_DC_BC(dc) = default_background;
	rtgui_dc_fill_rect(dc,&rect);
	rtgui_rect_inflate(&rect, RTGUI_WIDGET_BORDER(menu));
	rtgui_dc_draw_border(dc, &rect, RTGUI_WIDGET_BORDER_STYLE(menu));

	rtgui_rect_inflate(&rect, -RTGUI_WIDGET_BORDER(menu));
	item_rect = rect;

	if(menu->orient == RTGUI_HORIZONTAL)
	{/* horizontal style */
		rtgui_menu_item_t* menu_node = menu->head;
		
		/* draw each menu item */
		while(menu_node)
		{
			/* set the text rect */
			item_rect.x2 = item_rect.x1 + menu_node->item_width;
			/* draw text */
			if(menu_node == menu->now_item)
			{/* draw current item */
				if(menu->unfold)
				{
					RTGUI_DC_BC(dc) = selected_color;
					RTGUI_DC_FC(dc) = white;
				}
				else
				{
					RTGUI_DC_BC(dc) = default_background;
					RTGUI_DC_FC(dc) = black;
				}

				rtgui_dc_fill_rect(dc, &item_rect);
				if(menu_node->image)
				{
					item_rect.x1 += menu_node->image->w;
				}
				
				item_rect.x1 += RTGUI_MENU_MARGIN_W;
				rtgui_dc_draw_text(dc, menu_node->caption, &item_rect);
				if((menu_node->flag == RTGUI_MENU_POPUP) && (menu->orient==RTGUI_VERTICAL))
				{
					rtgui_dc_draw_byte(dc,rect.x2-8, item_rect.y1+7, 7, menu_popup_byte);
				}
				if(menu_node->image)
				{
					item_rect.x1 -= menu_node->image->w;
				}
				item_rect.x1 -= RTGUI_MENU_MARGIN_W;
			}
			else
			{
				RTGUI_DC_BC(dc) = default_background;
				RTGUI_DC_FC(dc) = black;
				rtgui_dc_fill_rect(dc,&item_rect);
				if(menu_node->image)
				{
					item_rect.x1 += menu_node->image->w;
				}
				
				item_rect.x1 += RTGUI_MENU_MARGIN_W;
				rtgui_dc_draw_text(dc, menu_node->caption, &item_rect);
				if(menu_node->image)
				{
					item_rect.x1 -= menu_node->image->w;
				}
				item_rect.x1 -= RTGUI_MENU_MARGIN_W;
			}
			menu_node = menu_node->next;

			item_rect.x1 = item_rect.x2;
		}
	}
	else
	{/* vertical style */
		rtgui_menu_item_t* menu_node = menu->head;

		/* draw each menu item */
		while(menu_node)
		{
			/* set the text rect */
			item_rect.y2 = item_rect.y1 + menu_node->item_height;
			/* draw text */
			if(menu_node == menu->now_item)
			{/* draw current item */
				if(menu->unfold)
				{
					RTGUI_DC_BC(dc) = selected_color;
					RTGUI_DC_FC(dc) = white;
				}
				else
				{
					RTGUI_DC_BC(dc) = default_background;
					RTGUI_DC_FC(dc) = black;
				}

				rtgui_dc_fill_rect(dc, &item_rect);
				if(menu_node->image)
				{
					item_rect.x1 += menu_node->image->w;	
				}
				
				item_rect.x1 += RTGUI_MENU_MARGIN_W;
				rtgui_dc_draw_text(dc, menu_node->caption, &item_rect);
				if((menu_node->flag == RTGUI_MENU_POPUP) && (menu->orient==RTGUI_VERTICAL))
				{
					rtgui_dc_draw_byte(dc,rect.x2-8, item_rect.y1+7, 7, menu_popup_byte);
				}
				if(menu_node->image)
				{
					item_rect.x1 -= menu_node->image->w;
				}
				item_rect.x1 -= RTGUI_MENU_MARGIN_W;
			}
			else
			{
				RTGUI_DC_BC(dc) = default_background;
				RTGUI_DC_FC(dc) = black;
				rtgui_dc_fill_rect(dc,&item_rect);
				if(menu_node->image)
				{
					item_rect.x1 += menu_node->image->w;
				}
				
				item_rect.x1 += RTGUI_MENU_MARGIN_W;
				rtgui_dc_draw_text(dc, menu_node->caption, &item_rect);
				if(menu_node->flag == RTGUI_MENU_POPUP)
					rtgui_dc_draw_byte(dc,rect.x2-8, item_rect.y1+7, 7, menu_popup_byte);
				if(menu_node->image)
				{
					item_rect.x1 -= menu_node->image->w;;
				}
				item_rect.x1 -= RTGUI_MENU_MARGIN_W;
			}
			menu_node = menu_node->next;

			item_rect.y1 = item_rect.y2;
		}
	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_slider(rtgui_slider_t* slider)
{
	/* draw button */
	int i, xsize, x0;
	rtgui_rect_t rect, focus_rect, slider_rect, slot_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(slider != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(slider);
	if(dc == RT_NULL)return;

	/* get widget rect */
	rtgui_widget_get_rect(slider, &focus_rect);
	/* fill widget rect with background color */
	rtgui_dc_fill_rect(dc,&focus_rect);
	rect = focus_rect;

	if(slider->orient == RTGUI_VERTICAL)
	{
		rtgui_rect_inflate(&rect, -1);
		xsize = rect.y2 - rect.y1 - slider->thumb_width;
		x0 = rect.y1 + slider->thumb_width / 2;

		/* calculate thumb position */
		slider_rect = rect;
		slider_rect.x1 += 1;
		slider_rect.x2 -= 1;
		slider_rect.y1 = x0 + xsize * (slider->value - slider->min) / (slider->max - slider->min) - slider->thumb_width/2;
		slider_rect.y2  = slider_rect.y1 + slider->thumb_width;

		/* calculate slot position */
		slot_rect.y1 = x0;
		slot_rect.y2 = x0 + xsize;
		slot_rect.x1 = (slider_rect.x1 + slider_rect.x2) /2 -1;
		slot_rect.x2 = slot_rect.x1 +3;
		RTGUI_DC_FC(dc) = black;
		/* draw slot */
		rtgui_dc_draw_border(dc, &slot_rect, RTGUI_WIDGET_BORDER_STYLE(slider));

		/* draw the ticks */
		for(i = 0; i <= slider->ticks; i++)
		{
			int k = x0 + xsize * i / slider->ticks;
			rtgui_dc_draw_hline(dc,rect.x1+1, rect.x1+4, k);
		}

		/* draw the thumb */
		rtgui_dc_fill_rect(dc,&slider_rect);
		rtgui_dc_draw_border(dc, &slider_rect, RTGUI_WIDGET_BORDER_STYLE(slider));
	}
	else
	{
		rtgui_rect_inflate(&rect, -1);
		xsize = rect.x2 - rect.x1 - slider->thumb_width;
		x0 = rect.x1 + slider->thumb_width / 2;

		/* calculate thumb position */
		slider_rect = rect;
		slider_rect.x1 = x0 + xsize*(slider->value - slider->min)/(slider->max - slider->min) - slider->thumb_width/2;
		slider_rect.x2  = slider_rect.x1 + slider->thumb_width;
		slider_rect.y1 += 1;
		slider_rect.y2 -= 1;

		/* calculate slot position */
		slot_rect.x1 = x0;
		slot_rect.x2 = x0 + xsize;
		slot_rect.y1 = (slider_rect.y1 + slider_rect.y2) /2 -1;
		slot_rect.y2 = slot_rect.y1 +3;
		/* draw slot */
		rtgui_dc_draw_border(dc, &slot_rect, RTGUI_WIDGET_BORDER_STYLE(slider));

		/* draw the ticks */
		for(i = 0; i <= slider->ticks; i++)
		{
			int x = x0 + xsize * i / slider->ticks;
			rtgui_dc_draw_vline(dc,x, rect.y1+1, rect.y1+4);
		}

		/* draw the thumb */
		rtgui_dc_fill_rect(dc,&slider_rect);
		rtgui_dc_draw_border(dc, &slider_rect, RTGUI_WIDGET_BORDER_STYLE(slider));
	}

	/* draw focus */
	if(RTGUI_WIDGET_IS_FOCUSED(slider))
	{
		RTGUI_DC_FC(dc) = black;
		rtgui_dc_draw_focus_rect(dc, &focus_rect);
	}

	rtgui_dc_end_drawing(dc);
}

const static rt_uint8_t _up_arrow[]    = {0x10, 0x38, 0x7C, 0xFE};
const static rt_uint8_t _down_arrow[]  = {0xFE,0x7C, 0x38, 0x10};
const static rt_uint8_t _left_arrow[]  = {0x10, 0x30, 0x70, 0xF0, 0x70, 0x30, 0x10};
const static rt_uint8_t _right_arrow[] = {0x80, 0xC0, 0xE0, 0xF0, 0xE0, 0xC0, 0x80};

void rtgui_theme_draw_scrollbar(rtgui_scrollbar_t* bar)
{
	/* draw scroll bar */
	rtgui_rect_t rect, btn_rect, thum_rect, arrow_rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(bar != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(bar);
	if(dc == RT_NULL)return;

	/* begin drawing */
	rtgui_widget_get_rect(bar, &rect);
	RTGUI_DC_BC(dc) = RTGUI_RGB(225, 228, 220);
	rtgui_dc_fill_rect(dc,&rect);

	if(bar->orient == RTGUI_VERTICAL)
	{	
		btn_rect = rect;
		btn_rect.y2 = btn_rect.y1 + (rect.x2 - rect.x1);

		/* draw up button */
		if(bar->status & SBAR_UPARROW) 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_SUNKEN);
		else 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_RAISE);
		/* draw up arrow */
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 7; arrow_rect.y2 = 4;
		rtgui_rect_moveto_align(&btn_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _up_arrow);
		
		/* draw thumb */

		{
			rtgui_scrollbar_get_thumb_rect(bar, &thum_rect);
			rtgui_dc_fill_rect(dc,&thum_rect);
			rtgui_dc_draw_border(dc, &thum_rect, RTGUI_BORDER_RAISE);
		}

		/* draw down button */
		btn_rect.y1 = rect.y2 - (rect.x2 - rect.x1);
		btn_rect.y2 = rect.y2;

		if(bar->status & SBAR_DOWNARROW) 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_SUNKEN);
		else 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_RAISE);
		/* drow down arrow */
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 7; arrow_rect.y2 = 4;
		rtgui_rect_moveto_align(&btn_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _down_arrow);
	}
	else
	{
		btn_rect = rect;
		btn_rect.x2 = btn_rect.x1 + (rect.y2 - rect.y1);

		/* draw left button */
		if(bar->status & SBAR_LEFTARROW) 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_SUNKEN);
		else 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_RAISE);
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 4; arrow_rect.y2 = 7;
		rtgui_rect_moveto_align(&btn_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _left_arrow);

		/* draw thumb */
		if(RTGUI_WIDGET_IS_ENABLE(bar))
		{
			rtgui_scrollbar_get_thumb_rect(bar, &thum_rect);
			rtgui_dc_fill_rect(dc,&thum_rect);
			rtgui_dc_draw_border(dc, &thum_rect, RTGUI_BORDER_RAISE);
		}

		btn_rect.x1 = rect.x2 - (rect.y2-rect.y1);
		btn_rect.x2 = rect.x2;

		/* draw right button */
		if(bar->status & SBAR_RIGHTARROW) 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_SUNKEN);
		else 
			rtgui_dc_draw_border(dc, &btn_rect, RTGUI_BORDER_RAISE);
		arrow_rect.x1 = 0; arrow_rect.y1 = 0;
		arrow_rect.x2 = 4; arrow_rect.y2 = 7;
		rtgui_rect_moveto_align(&btn_rect, &arrow_rect, RTGUI_ALIGN_CENTER_HORIZONTAL|RTGUI_ALIGN_CENTER_VERTICAL);
		rtgui_dc_draw_byte(dc, arrow_rect.x1, arrow_rect.y1, rtgui_rect_height(arrow_rect), _right_arrow);

	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_progressbar(rtgui_progressbar_t* bar)
{
	/* draw progress bar */
	rtgui_dc_t* dc;
	rtgui_rect_t rect;
    int max = bar->range;
    int pos = bar->position;
    int left;
	rtgui_color_t bc;
	
	RT_ASSERT(bar != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(bar);
	if(dc == RT_NULL)return;

	bc = RTGUI_DC_BC(dc);
	rtgui_widget_get_rect(&(bar->parent), &rect);

	/* fill button rect with background color */
	RTGUI_DC_BC(dc) = RTGUI_RGB(212, 208, 200);

    /* draw border */
	rect.x2 --; rect.y2 --;
	rtgui_dc_draw_border(dc, &rect, RTGUI_WIDGET_BORDER_STYLE(bar));

	/* Nothing to draw */
    if(max == 0)
    {
        return;
    }

	rect.x2 ++; rect.y2 ++;
    left = max - pos;
	rtgui_rect_inflate(&rect, -2);
	RTGUI_DC_BC(dc) = RTGUI_RGB(0, 0, 255);

    if(bar->orient == RTGUI_VERTICAL)
    {
        /* Vertical bar grows from bottom to top */
        int dy = (rtgui_rect_height(rect) * left) / max;
        rect.y1 += dy;
		rtgui_dc_fill_rect(dc,&rect);

		rect.y1 -= dy; rect.y2 = dy;
		RTGUI_DC_BC(dc) = white;
		rtgui_dc_fill_rect(dc,&rect);
    }
    else
    {
        /* Horizontal bar grows from left to right */
		int dx = (rtgui_rect_width(rect) * left) / max;
        rect.x2 -= dx;
		rtgui_dc_fill_rect(dc,&rect);

		rect.x1 = rect.x2; rect.x2 += dx;
		RTGUI_DC_BC(dc) = white;
		rtgui_dc_fill_rect(dc,&rect);
	}
	RTGUI_DC_BC(dc) = bc;

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_staticline(rtgui_staticline_t* sline)
{
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(sline != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(sline);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(sline, &rect);
	rtgui_dc_fill_rect(dc,&rect);
	
	if(sline->orient == RTGUI_HORIZONTAL)
	{
		rtgui_dc_draw_horizontal_line(dc, rect.x1, rect.x2, rect.y1);
	}
	else
	{
		rtgui_dc_draw_vertical_line(dc, rect.x1, rect.y1, rect.y2);
	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_selected(rtgui_dc_t *dc, rtgui_rect_t *rect)
{
	rtgui_color_t bc;
	rt_uint16_t index;
	
	bc = RTGUI_DC_FC(dc);
	RTGUI_DC_FC(dc) = selected_color;

	rtgui_dc_draw_hline(dc,rect->x1 + 1, rect->x2 - 0, rect->y1 + 1);
	rtgui_dc_draw_hline(dc,rect->x1 + 1, rect->x2 - 0, rect->y2 - 2);

	rtgui_dc_draw_vline(dc,rect->x1 + 1, rect->y1 + 2, rect->y2 - 2);
	rtgui_dc_draw_vline(dc,rect->x2 - 1, rect->y1 + 2, rect->y2 - 2);

	for(index = rect->y1 + 1; index < rect->y2 - 2; index ++)
		rtgui_dc_draw_hline(dc,rect->x1 + 2, rect->x2 - 1, index);

	RTGUI_DC_FC(dc) = bc;
}

/* get default background color */
rtgui_color_t rtgui_theme_default_bc()
{
	return default_background;
}

/* get default foreground color */
rtgui_color_t rtgui_theme_default_fc()
{
	return default_foreground;
}

void rtgui_theme_draw_panel(rtgui_panel_t *panel)
{
	rtgui_event_paint_t event;
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(panel != RT_NULL);

	rtgui_widget_get_rect(panel, &rect);

	if(panel->image == RT_NULL)
	{	
		/* begin drawing */
		dc = rtgui_dc_begin_drawing(panel);
		if(dc == RT_NULL)return;

		rtgui_dc_fill_rect(dc,&rect);

		rtgui_dc_end_drawing(dc);
	}

	/* paint each widget */
	RTGUI_EVENT_PAINT_INIT(&event);
	event.wid = RT_NULL;
	rtgui_container_dispatch_event(RTGUI_CONTAINER(panel), (rtgui_event_t*)&event);

}

const rt_uint8_t combo_down_bmp[]=
{
0xFE,0x7C,0x38,0x10,
};

void rtgui_theme_draw_combo(rtgui_combo_t *cbo)
{
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(cbo != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(cbo);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(cbo, &rect);

	rtgui_rect_inflate(&rect,-RTGUI_WIDGET_BORDER(cbo));
	RTGUI_DC_BC(dc) = white;
	rtgui_dc_fill_rect(dc, &rect);

	rtgui_rect_inflate(&rect,RTGUI_WIDGET_BORDER(cbo));
	rtgui_dc_draw_border(dc, &rect,RTGUI_WIDGET_BORDER_STYLE(cbo));
	
	if(rtgui_rect_height(rect)<RTGUI_COMBO_HEIGHT)return;

	/* draw downarrow button */
	rect.x1 = rect.x2-RTGUI_COMBO_BUTTON_WIDTH-RTGUI_WIDGET_BORDER(cbo);
	rect.y1 += RTGUI_WIDGET_BORDER(cbo);
	rect.x2 -= RTGUI_WIDGET_BORDER(cbo);
	rect.y2 -= RTGUI_WIDGET_BORDER(cbo);
	RTGUI_DC_BC(dc) = default_background;
	rtgui_dc_fill_rect(dc, &rect);

	if(cbo->style & RTGUI_COMBO_STYLE_DOWNARROW_UP)
	{
		rtgui_dc_draw_border(dc, &rect,RTGUI_BORDER_EXTRA);
		rtgui_dc_draw_byte(dc,rect.x1+4, rect.y1+8, 4, combo_down_bmp);	
	}
	else if(cbo->style & RTGUI_COMBO_STYLE_DOWNARROW_DOWN)
	{
		rtgui_dc_draw_border(dc, &rect,RTGUI_BORDER_SUNKEN);
		rtgui_dc_draw_byte(dc,rect.x1+5, rect.y1+9, 4, combo_down_bmp);	
	}

	if(cbo->tbox != RT_NULL)
	{
		RTGUI_DC_FC(dc) = black;
		rtgui_theme_draw_textbox(cbo->tbox);
	}

	rtgui_dc_end_drawing(dc);
}

void rtgui_theme_draw_combo_downarrow(rtgui_combo_t *cbo)
{
	rtgui_rect_t rect;
	rtgui_dc_t* dc;
	
	RT_ASSERT(cbo != RT_NULL);

	/* begin drawing */
	dc = rtgui_dc_begin_drawing(cbo);
	if(dc == RT_NULL)return;

	rtgui_widget_get_rect(cbo, &rect);

	rect.x1 = rect.x2-RTGUI_COMBO_BUTTON_WIDTH-RTGUI_WIDGET_BORDER(cbo);
	rect.y1 += RTGUI_WIDGET_BORDER(cbo);
	rect.x2 -= RTGUI_WIDGET_BORDER(cbo);
	rect.y2 -= RTGUI_WIDGET_BORDER(cbo);
	RTGUI_DC_BC(dc) = default_background;
	rtgui_dc_fill_rect(dc, &rect);
	
	if(cbo->style & RTGUI_COMBO_STYLE_DOWNARROW_UP)
	{
		rtgui_dc_draw_border(dc, &rect,RTGUI_BORDER_EXTRA);
		rtgui_dc_draw_byte(dc,rect.x1+4, rect.y1+8, 4, combo_down_bmp);	
	}
	else if(cbo->style & RTGUI_COMBO_STYLE_DOWNARROW_DOWN)
	{
		rtgui_dc_draw_border(dc, &rect,RTGUI_BORDER_SUNKEN);
		rtgui_dc_draw_byte(dc,rect.x1+5, rect.y1+9, 4, combo_down_bmp);	
	}

	rtgui_dc_end_drawing(dc);
}


