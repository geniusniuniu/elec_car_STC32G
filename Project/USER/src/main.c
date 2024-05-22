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

extern float Diff;
extern float Plus;
extern float Ratio;
extern short gx,gy,gz;
extern volatile float Exp_Speed;
extern float Exp_Speed_L;
extern float Exp_Speed_R;
extern float Act_PwmL,Act_PwmR;
extern char Isr_Flag_10;
extern float Flag_Out_L;
extern float Flag_Out_R;

volatile uint16 Dis_Process = 0;

float Adjust_Val = 0;
char KeyValue = 0;	

void Init_all(void);

#define FILTER_N 12

uint16 filter_buf[FILTER_N + 1];

uint16 Filter_Window(uint16 Dis) 
{
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
		printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",ADC_proc[0] ,ADC_proc[1],ADC_proc[2],ADC_proc[3],Ratio,ADC_proc[4],Right_Wheel.out);
/******************************************** 按键读值**********************************************************************/ 	
		ui_show();
		KeyValue = GetKey_Value(0);
		if 		(KeyValue == KEY2_PRES) 	{page++; if(page >= 3)  page = 3;oled_all_static_state();}		
		else if (KeyValue == KEY3_PRES) 	{page--; if(page <= 0)  page = 0;oled_all_static_state();}			
		else if (KeyValue == KEY0_PRES) 	Adjust_Val += 0.05;
		else if (KeyValue == KEY1_PRES) 	Adjust_Val -= 0.05;
	
/******************************************** 类似中断服务处理 **************************************************************/ 
		if(Isr_Flag_10 == 1)
		{
			MPU6050_Refresh_DMP();				//读取角度值
			MPU_Get_Gyroscope(&gx, &gy, &gz);	//读取角速度
            vl53l0x_get_distance();             //距离测量
            if (vl53l0x_finsh_flag == 1)        //一次测距有效
                Dis_Process = Filter_Window(vl53l0x_distance_mm);
            Elem_Up_Down(Pitch);	
			Isr_Flag_10 = 0;
		}
          
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
	//gpio_mode(P3_2, GPIO);
    vl53l0x_init();
//    dl1a_init();
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
	
////pid初始化  PID_Init(结构体, KP, KI, KD, 输出限幅，积分限幅)
//	PID_Init(&Left_Wheel_PID , 20, 0.5, 0, 9000, 2000);
//	PID_Init(&Right_Wheel_PID, 20, 0.5, 0, 9000, 2000);
	PID_Init(&Turn_PID , -2, 0, 0 ,10000, 0);
	
    PID_Incremental_Init(&Left_Wheel ,58,3.9,0.1,9000,3000,1);//48
    PID_Incremental_Init(&Right_Wheel,58,3.9,0.1,9000,3000,1);//48
	
} 
