#ifndef  __UI_H_
#define  __UI_H_

#include <stdio.h>
#include <string.h>
#include "SEEKFREE_OLED.h"

extern char page;

void ui_init(void);
void ui_show(void);
///////////////////////////////////////////////////////oled显示部分

void oled_all_static_state(void);//静态显示初始化
void oled_all_dynamic_state(void);//动态显示初始化

void oled_change_value(int pages,int y,float value_new);//改变变量值

void oled_show(void);

///////////////////////////////////////////////////////

void refresh_value();

#endif
