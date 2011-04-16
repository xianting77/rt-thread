#ifndef __DIALOG_H__
#define __DIALOG_H__

rtgui_win_t* rtgui_win_get_win_by_widget(PVOID wdt);
void OpenFileDialog(PVOID wdt, rtgui_event_t* event);
void NewFileDialog(PVOID wdt, rtgui_event_t *event);
void SaveFileDialog(PVOID wdt, rtgui_event_t *event);

void GoToTopFolder(PVOID wdt, rtgui_event_t *event);
void GoToSubFolder(PVOID wdt, rtgui_event_t *event);




#endif

