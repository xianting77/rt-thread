#ifndef __DIALOG_H__
#define __DIALOG_H__

/* �򿪶Ի����е�һЩ���� */
struct OpenDlgSt
{
	char* path;		 //����·��
	char* filename;	 //�ļ�(��)����
	rt_uint32_t size;//�ļ���С/�ļ����µ��ļ����� 
	rt_uint32_t type;//�ļ�����:�ļ�/�ļ���
	char* pattern;   //�ļ�����;

	rtgui_filelist_view_t* fview;
	rtgui_textbox_t* tbox_path;	//�ؼ�ָ��
	rtgui_textbox_t* tbox_filename;
	rtgui_combo_t*   cbox_pattern;
};


rtgui_win_t* rtgui_win_get_win_by_widget(PVOID wdt);
void OpenFileDialog(PVOID wdt, rtgui_event_t* event);
void NewFileDialog(PVOID wdt, rtgui_event_t *event);
void SaveFileDialog(PVOID wdt, rtgui_event_t *event);

void GoToTopFolder(PVOID wdt, rtgui_event_t *event);
void GoToSubFolder(PVOID wdt, rtgui_event_t *event);




#endif

