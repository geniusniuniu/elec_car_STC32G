#include <string.h>
#include <stdio.h>
#include "common.h"
#include "zf_gpio.h"
#include <STC32Gxx.H>
#include "Key.h"
#include "zf_delay.h"

char Mode = 1;
char clear_Num = 0;

void Key_Init(void)
{
	//按键初始化
	gpio_mode(P7_0, GPIO); //KEY1
	gpio_mode(P7_1, GPIO); //KEY2
	gpio_mode(P7_2, GPIO); //KEY3
	gpio_mode(P7_3, GPIO); //KEY4
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1!!
uint8 GetKey_Value(uint8 mode)
{ 
	static uint8 key_up=1;//按键松开标志
	if(mode)
		key_up=1;  //支持连按		  
	if(key_up&&(P73==0 || P72==0 ||P71==0||P70==0))  //注意此函数有响应优先级,KEY0>KEY1!!
	{
//		delay_ms(10);//去抖动 /////////////////////////////////
		key_up=0;
		if(P70==0)			return KEY0_PRES; //1
		else if(P71==0)		return KEY1_PRES; //2
		else if(P72==0)		return KEY2_PRES;
		else if(P73==0)		return KEY3_PRES; 
	}
	else if(P70==1 && P71==1 && P72==1 && P73==1)
		key_up=1; 	     
	return 0;// 无按键按下
}


