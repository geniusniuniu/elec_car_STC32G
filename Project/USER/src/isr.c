#include "headfile.h"
#include "Motor.h"
#include "Buzzer.h"
#include "ADC.h"
#include "math.h"
#include "MPU6050.h"
#include "TOF.h"
#include "Element.h"
#define LED P52

extern char Down_Flag;
extern float Dis_Process;

short gx, gy, gz;
char Speed_Delay = 40;
char Isr_Flag_10 = 0;
char Distance_Num = 0;
char  Flag_Stop = 0;
char Edge_Delay = 0;
char count = 0;
char Read_Nums = 0;

float Flag_Out_L = 0;
float Flag_Out_R = 0;
float Diff,Plus;
float Ratio = 0;
float Diff_Mid,Plus_Mid;
float Ratio_Mid = 0;
float Exp_Speed_L = 0;
float Exp_Speed_R = 0;
float Act_PwmL;	
float Act_PwmR;
float Angle_Edge = 0;
float Sum_Dis = 0;
float sum_L,sum_R;
float Exp_Speed_gain = 1.0;
volatile float Exp_Speed = 0;

void Get_Ratio(void);

void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG;       //清除中断标志
	count++;
    Isr_Flag_10 = 1;
	Get_Speed();            //获取车速
	ADC_GetValue();			//获取电感值
    Get_Ratio();			//计算偏差值
    vl53l0x_get_distance();             //距离测量
   
/*********************** 直道弯道变速 **********************************/ 	
    Turn_PID.Kpa = -0.0006;//理论来讲kpa和kpb同号
    Turn_PID.Kpb = -80;//-170;  
    Turn_PID.Kd = -35;//20;  //1.5
    
	if(Ratio >= -0.15 && Ratio <= 0.15) //直线
    {
        Exp_Speed = 280;   
    }
    else
    {
        Exp_Speed = 240-(Num2Abs(Ratio)/0.1)*15;  
    } 
/************************************************ 圆环判别 ***********************************************/ 
    
	if(ADC_proc[2] > 66 || ADC_proc[0] > 65 || ADC_proc[4] > 65) 
	{
        if(ADC_proc[0] + ADC_proc[4] >= 85 )
            Circle_Flag1 = 1; 
    }    

//    if(Dis_Process <= 50)           //幅值滤波（TOF读值会莫名跳变）
//        Dis_Process = 810;
//	if(Dis_Process < 400)	        //测距值小于50cm，区分坡道，且只执行一次
//		Circle_Delay1 = 100;        //延时0.5秒
    
//	if(Circle_Delay1 > 0)			//检测到坡道
//	{
//		Circle_Flag1 = 0;			//清零环岛标志位						
//		Circle_Delay1--;
//	}
    Elem_Circle((Speed_L+Speed_R)/2,gz);   

/*********************************避开路障***************************************/            

//          if(Special_Elem >= 5)           //经过障碍前的某个元素（环岛，坡道），再开启避障
//              Barrier_Executed = 0;
    if(Barrier_Executed == 0)
    {	
        if (Dis_Process < 880)		        //	检测到路障
            Distance_Num++;
        else
            Distance_Num = 0;
        if(Distance_Num >= 2)               //连续判别2次或以上
        {
            Barrier_Flag1 = 1;
            //x10_ms = 5;
            Distance_Num = 0;
        }
        Elem_Barrier(gz,(Speed_L+Speed_R)/2);
    }


/************************************************ 转向环计算 **********************************************/    
    
	Limit_Out(&Ratio,-0.9,0.9);   //限幅
	PID_Calculate(&Turn_PID,Ratio*150,gz/100); 				
	Limit_Out(&Turn_PID.PID_Out,-8000,8000);
    Exp_Speed *= Exp_Speed_gain;
	if(Ratio >= 0)	
	{
		Exp_Speed_L = Exp_Speed + Turn_PID.PID_Out*0.09;
		Exp_Speed_R = Exp_Speed - Turn_PID.PID_Out*0.08*(1-Ratio);
	}
	else
	{
		Exp_Speed_L = Exp_Speed + Turn_PID.PID_Out*0.08*(1+Ratio);
		Exp_Speed_R = Exp_Speed - Turn_PID.PID_Out*0.09;
	}
	
	PID_Incremental_Calc(&Left_Wheel,Exp_Speed_L,Speed_L);
	PID_Incremental_Calc(&Right_Wheel,Exp_Speed_R,Speed_R);
    
/********************************************* 驶离赛道，撞到障碍，停车 *********************************************/
	if(Speed_Delay > 0)         //刚启动时候给定一小段延时
        Speed_Delay --;
    
    if(Speed_Delay == 0 && abs(Speed_L) < 15 && abs(Speed_R) < 15)
		Flag_Stop = 1;
    else if(abs(Speed_L) >= 15 || abs(Speed_R) >= 15)
       Flag_Stop = 0; 
    
/********************************************* 设置左右PWM ************************************************/ 	  

    if(Dis_Process < 120 || Flag_Stop == 1) 
	{
		Act_PwmL = Left_SetSpeed(0);		
		Act_PwmR = Right_SetSpeed(0);
	}
	else
	{
        Act_PwmL = Left_SetSpeed(Left_Wheel.out);
        Act_PwmR = Right_SetSpeed(Right_Wheel.out);
	}

	//蜂鸣器
	if(x10_ms > 0)
	{
		pwm_duty(PWMB_CH4_P77,2000);
		x10_ms--;
	}
	else
		pwm_duty(PWMB_CH4_P77,0);
	
	//闪灯
    if(count > 50)
    {
		LED = !LED;
		count = 0;
    }

}

//对ADC值进行处理得到差比和
void Get_Ratio(void)
{    
    static float sum_01;
    static float sum_34;
    static float sum;
    
	sum_L = sqrt(ADC_proc[0]*ADC_proc[0]+ADC_proc[1]*ADC_proc[1]);
	sum_R = sqrt(ADC_proc[4]*ADC_proc[4]+ADC_proc[3]*ADC_proc[3]);
	Diff  = sum_L - sum_R;
	Plus  = sum_L + sum_R;
	   
    sum_01= ADC_proc[0] + ADC_proc[1];
    sum_34= ADC_proc[3] + ADC_proc[4];
    sum   = sum_01 + sum_34;
    
	if((sum > EDGE_PROTECT) && Barrier_Flag1 == 0)  
	{
		Ratio = Diff/Plus;	//如果小于EDGE_PROTECT//视作丢线，下次偏差值
		Flag_Out_L = 0;		//在上次基础上再次加（减）
		Flag_Out_R = 0;
		Edge_Delay= 0;
	}																			
	else 																		
	{
        if(ADC_proc[0] + ADC_proc[4] < 5) 
            Flag_Stop = 1;
        //在避障阶段和环岛阶段以及上一次丢线未寻回前不做判断
		else if(Barrier_Flag1 == 1 && Circle_Flag1 == 0 && Circle_Delay2 == 0 && Edge_Delay == 0)  
		{
           Edge_Delay = 100;	//50ms	
           if(sum_01 >= sum_34 && Flag_Out_R == 0) 
           {  
                 Flag_Out_L = 1;
           } 
           else if(sum_01 < sum_34 && Flag_Out_L == 0) 
           {
                 Flag_Out_R = 1;
           } 
        }
	}
}




















//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF

	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;

		//接收数据寄存器为：S4BUF;
		if(wireless_type == WIRELESS_SI24R1)
        {
            wireless_uart_callback();           //无线转串口回调函数
        }
        else if(wireless_type == WIRELESS_CH9141)
        {
            bluetooth_ch9141_uart_callback();   //蓝牙转串口回调函数
        }
        else if(wireless_type == WIRELESS_CH573)
        {
            wireless_ch573_callback();          //CH573无线模块回调函数
        }
	}
}

#define LED P52
void INT0_Isr() interrupt 0
{
	LED = 0;	//点亮LED
}
void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}

void TM0_Isr() interrupt 1
{

}
void TM1_Isr() interrupt 3
{

}
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //清除中断标志
	
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
	
}


//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;