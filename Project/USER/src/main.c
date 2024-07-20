#include "headfile.h"
#include "Motor.h"
#include "ADC.h"
#include "MPU6050.h"
#include "Element.h"
#include "ADC.h"
#include "math.h"
#include "TOF.h"
#include "Buzzer.h"
#include "Key.h"
#include "ui.h"
#include "isr.h"

#define FILTER_N 8

float Dis_Process = 0;
float Adjust_Val = 0;
char KeyValue = 0;	

void Init_all(void);
void Get_Ratio(void);
void Speed_Gain(void);

uint16 Filter_Window(uint16 Dis) 
{
    static uint16 filter_buf[FILTER_N + 1];
    int i;    
    uint16 filter_sum = 0;
    filter_buf[FILTER_N] = Dis;
    for(i = 0; i < FILTER_N; i++) 
    {
        filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，低位仍掉
        filter_sum += filter_buf[i];
    }
    return (uint16)(filter_sum / FILTER_N);
}

void main(void)	  
{
	Init_all();
	EnableGlobalIRQ();	
	Adjust_Val = 0;
	while(1)
	{	
		//printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",Exp_Speed_gain ,Circle_Flag1,Circle_Flag2,ADC_proc[2],Ratio,Dis_Process);
        printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",Act_PwmL ,Act_PwmR,Exp_Speed_L,Exp_Speed_R,Speed_L,Speed_R); 
/******************************************** 按键读值**********************************************************************/ 	
		
        ui_show();
		KeyValue = GetKey_Value(0);
		if 		(KeyValue == KEY2_PRES) 	{page++; if(page >= 3)  page = 3;oled_all_static_state();}		
		else if (KeyValue == KEY3_PRES) 	{page--; if(page <= 0)  page = 0;oled_all_static_state();}			
		else if (KeyValue == KEY0_PRES) 	Adjust_Val += 1;
		else if (KeyValue == KEY1_PRES) 	Adjust_Val -= 1;
        
/******************************************** 类似中断服务处理 **************************************************************/ 
		if(Isr_Flag_10 == 1)
		{
			MPU6050_Refresh_DMP();				//读取角度值
			MPU_Get_Gyroscope(&gx, &gy, &gz);	//读取角速度
            if(vl53l0x_finsh_flag == 1)         //一次测距有效
            {
                Dis_Process = Filter_Window(vl53l0x_distance_mm);
            }
            Speed_Gain();
/************************************************ 上下坡道 ********************************************/
            Elem_Up_Down(Pitch);
//############如果下坡连着急弯###########只能注释不可删除
//            if(Down_Flag>0)
//            {
//                Down_Flag--;
//                Exp_Speed = 100;
//            }
//            else
//                Down_Flag=0;

/************************************************ 大弯丢线 ********************************************/ 		
            if(Flag_Out_L == 1 && (--Edge_Delay) > 0 )   //-- Edge_Delay 保证清零时不会再进判断
            {
                //x10_ms = 10;
                Exp_Speed = 130;
                Ratio = 0.2+(Speed_R/120)*0.15;	
            }
            else if(Flag_Out_R == 1 && (--Edge_Delay) > 0)
            {
                //x10_ms = 10;
                Exp_Speed = 130;
                Ratio = -0.2-(Speed_L/120)*0.15;	
            }
            else
                Edge_Delay = 0; 
            
            Isr_Flag_10 = 0;
        }
	}	        
}

void Speed_Gain(void)
{
    int i = 0;
    static float Speed_Buf[6] = {0,0};
    Speed_Buf[5] = (Speed_L+Speed_R)/2;     //取车的平均速度
    for(i ; i < 5; i++) 
    {
        Speed_Buf[i] = Speed_Buf[i + 1]; // 所有数据左移，低位仍掉
        Sum_Dis += Speed_Buf[i];
    }
    if(Sum_Dis <= SUM_THRESHOLD)    // 5次之内积分值高于SUM_THRESHOLD则是直线
    {
        Sum_Dis = 0;
        Exp_Speed_gain = 0.8;
    }
    else
    {
        Sum_Dis = 0;
        Exp_Speed_gain = 1.0;
    }
}



void Init_all(void)
{
	WTST = 0;						 
	DisableGlobalIRQ();				
	sys_clk = 35000000;				
	board_init();					

////编码器方向引脚初始化
	gpio_mode(P5_3, GPIO);          
	gpio_mode(P3_5, GPIO);	
	
////按键引脚初始化
	Key_Init();
	
////测距模块初始化
    vl53l0x_init();
    
////OLED初始化
	ui_init();					
	
////MPU6050初始化
	MPU6050_DMP_Init();	
//	
////定时器初始化
	pit_timer_ms(TIM_4, 10);		//10ms定时器
//	
////编码器初始化
	ctimer_count_init(CTIM0_P34);	//编码器1计数
	ctimer_count_init(CTIM3_P04);	//编码器2计数
	
////电机初始化
	Motor_Init();
	
////蜂鸣器初始化
	 Buzzer_Init();
	
////初始化所有AD引脚
	ADC_InitAll(); 
	
    ////pid初始化  PID_Init(结构体, KPa,Kpb, KI, KD, 输出限幅，积分限幅)
	PID_Init(&Turn_PID , 0,-2, 0, 0 ,10000, 0);
	
    // PID_Incremental_Init(PID_Incremental *pid, float Kp, float Ki, float Kd,float Out_Limit, float Integral_Limit,uint8 use_lowpass_filter)
    PID_Incremental_Init(&Left_Wheel ,58,3.9,0.1,9000,3000,1);//48
    PID_Incremental_Init(&Right_Wheel,58,3.9,0.1,9000,3000,1);//48
    
//    PID_Incremental_Init(&Left_Wheel ,58,3.9,0.1,9000,3000,1);//48
//    PID_Incremental_Init(&Right_Wheel,58,3.9,0.1,9000,3000,1);//48
	
} 
