#ifndef  __UI_H_
#define  __UI_H_

#include <stdio.h>
#include <string.h>
#include "SEEKFREE_OLED.h"

extern char page;

void ui_init(void);
void ui_show(void);
///////////////////////////////////////////////////////oled��ʾ����

void oled_all_static_state(void);//��̬��ʾ��ʼ��
void oled_all_dynamic_state(void);//��̬��ʾ��ʼ��

void oled_change_value(int pages,int y,float value_new);//�ı����ֵ

void oled_show(void);

///////////////////////////////////////////////////////

void refresh_value();

#endif
