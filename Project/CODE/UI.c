#include "ui.h"
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "headfile.h"
#include "ADC.h"
#include "Motor.h"
#include "Element.h"
#include "MPU6050.h"

extern short gx,gy,gz;
extern float Ratio;
extern float Flag_Out_L;
extern float Flag_Out_R;
extern volatile float Exp_Speed;
extern float Dis_Process;
extern float Adjust_Val;
char page=0;
float value[4][7];
char value_name[4][7][7];


void value_name_init(void)//将name改为想要的参数名，但“ ”中的总长度不要变
{
	//page=0
	strcpy(value_name[0][0],"Key_Vl"); //NAME_1表示显示的是第一页
	strcpy(value_name[0][1],"ADC0  ");
	strcpy(value_name[0][2],"ADC1  ");
	strcpy(value_name[0][3],"ADC2  ");


	//page=1	
	strcpy(value_name[1][0],"ADC3  ");
	strcpy(value_name[1][1],"ADC4  ");
	strcpy(value_name[1][2],"Ratio ");
	strcpy(value_name[1][3],"Dis_Pr");

	
	//page=2
	strcpy(value_name[2][0],"Trn_Kp");
	strcpy(value_name[2][1],"Trn_Ki");
	strcpy(value_name[2][2],"Trn_Kd");
	strcpy(value_name[2][3],"Pitch");
	
	//page=3
	strcpy(value_name[3][0],"Exp_Sp");
	strcpy(value_name[3][1],"Gyro_z");
	strcpy(value_name[3][2],"Dis2  ");
	strcpy(value_name[3][3],"An_Sum");

}

void refresh_value()//value=填入变量名
{
	//page=0
    if(page == 0)
    {
        value[0][0]= Adjust_Val;
        value[0][1]= ADC_proc[0];
        value[0][2]= ADC_proc[1];
        value[0][3]= ADC_proc[2];
    }
	else if(page == 1)
    {
        //page=1
        value[1][0]= ADC_proc[3];
        value[1][1]= ADC_proc[4];
        value[1][2]= Ratio;
        value[1][3]= Dis_Process;
    }
	else if(page == 2)
    {
        //page=2
        value[2][0]= Left_Wheel.Kp;
        value[2][1]= Left_Wheel.Ki;
        value[2][2]= Left_Wheel.Kd;
        value[2][3]= Pitch;
	}
    else if(page == 3)
    {
        //page=3
        value[3][0]= Exp_Speed;
        value[3][1]= gz;
        value[3][2]= Flag_Out_R;
        value[3][3]= 0.0;
    }	
}
//                         oled显示函数
///////////////////////////////////////////////////////////////////////

void oled_change_value(int pages,int y,float value_new)
{
    value[pages][y]=value_new;
    oled_printf_float(55,y,value[pages][y],4,4);
}

void oled_all_static_state()  //静态显示部分
{
    uint8 i,j;
//    oled_p8x16str(0, 0,"page");
//    oled_p8x16str(40, 0,"=");
//    oled_int16(60,0,Mode_Num);

//    oled_p8x16str(70, 0,"mode");
//    oled_int16(120, 0,page);
	for(j=0;j<=3;j++)
	{
		i = j*2;
		oled_p8x16str(0 ,i,value_name[page][j]);
		oled_p8x16str(50,i,"=");
	}
}


void oled_show(void)
{
    uint8 i,j;
	refresh_value();
	for(j=0;j<=3;j++)
	{
		i = j*2;
		oled_printf_float(55,i,value[page][j],5,2);
	}
}

//////////////////////////////////////////////////////////////
void ui_init(void)
{
	oled_init();
	value_name_init();
	oled_all_static_state();
}

void ui_show(void)
{
	refresh_value();
	oled_show();
}



