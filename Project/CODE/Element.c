#include "isr.h"
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "Element.h"
#include "Motor.h"
#include "PID.h"
#include "Buzzer.h"
#include "MPU6050.h"
#include "isr.h"

extern float Ratio;
extern float ADC_proc[5];

//下坡标志位
char Down_Flag = 0;

//环岛标志位
float Circle_Flag1 = 0;  // 左右环岛标志位
float Circle_Flag2 = 0;


//出环延时
int circle_Delay1 = 0;
int Circle_Delay2 = 0;

//记录元素个数
char Special_Elem = 0;


void Elem_Up_Down(float Angle)  
{
    static char Elem_Over;
	if(Angle > 5.5)          //上坡
		Exp_Speed = 320;
	else if(Angle < -10)  //下坡
    {
        Down_Flag = 5;
        if(Elem_Over == 0)
        {
            Elem_Over=1;
            Special_Elem++;
        }
    }
    
}


//障碍物识别   
char Barrier_Executed = 1;
char Barrier_Flag1 = 0;
//void Elem_Barrier_Timer(void)  
//{
//	static float Barrier_Timer;	
//	#if BARRIER_DIR == 0						//向右避障
//		if(Barrier_Flag1 == 1)					//识别到障碍物
//		{
//            Exp_Speed = 220;
//			Barrier_Timer--;
//			if(Barrier_Timer > 90)
//				Ratio = -0.235;
//			else if(Barrier_Timer > 30 && Barrier_Timer <= 90)
//				Ratio = 0.5;
//			else 
//				Ratio -= 0.06;
//			if(Barrier_Timer == 0)
//			{
//				Barrier_Flag1 = 0;
//				Barrier_Executed = 1;
//                Special_Elem = 0;
//			}
//		}
//		else
//			Barrier_Timer = 130;
//	#elif BARRIER_DIR == 1  
//		//向左避障
//		if(Barrier_Flag1 == 1)					//识别到障碍物
//		{
//            Exp_Speed = 220;
//			Barrier_Timer--;
//			if(Barrier_Timer > 70)
//				Ratio = 0.49;
//			else if(Barrier_Timer > 18 && Barrier_Timer <= 70)
//				Ratio = -0.43;
//			else 
//				Ratio += 0.04;
//			if(Barrier_Timer == 0)
//			{
//				Barrier_Flag1 = 0;
//				Barrier_Executed = 1;
//                Special_Elem = 0;
//			}
//		}
//		else
//			Barrier_Timer = 120;
//		
//	#endif
//}


char Barrier_Flag2 = 0;
char Barrier_Flag3 = 0;
float Dis_Bar = 0;
float Sum_Angle_Bar = 0;
void Elem_Barrier(float Gyro_Z,float Speed)
{
	Gyro_Z = (Gyro_Z*2000)/32768;	
    
	#if BARRIER_DIR == 0		    //向右避障
		if(Barrier_Flag1==1)        
		{
            Sum_Angle_Bar += Gyro_Z*0.005;
            Dis_Bar += Speed;
			Ratio = -0.385;			

		}
		if(Sum_Angle_Bar < -23 && Dis_Bar > 4000) 
		{
			Barrier_Flag1 = 0;   
			Barrier_Flag2 = 1;
		}
		if(Barrier_Flag2 == 1)      //左拐
		{   
            Sum_Angle_Bar += Gyro_Z*0.005;
            Dis_Bar += Speed;
			if(Sum_Angle_Bar > 21 && Dis_Bar > 8000)  
			{
                Barrier_Flag3 = 1;  //回正标志位			
			}
			else  	            //回正
				Ratio = 0.395; 
		}
    #elif BARRIER_DIR == 1               //向左避障
		if(Barrier_Flag1==1)        
		{
            Sum_Angle_Bar += Gyro_Z*0.005;
            Dis_Bar += Speed;
			Ratio = 0.425;			

		}
		if(Sum_Angle_Bar > 23 && Dis_Bar > 4000) 
		{
			Barrier_Flag1 = 0;   
			Barrier_Flag2 = 1;
		}
		if(Barrier_Flag2 == 1)      //右拐
		{   
            Sum_Angle_Bar += Gyro_Z*0.005;
            Dis_Bar += Speed;
			if(Sum_Angle_Bar < -22 && Dis_Bar > 8000)  
			{
                Barrier_Flag3 = 1;  //回正标志位			
			}
			else  	            //回正
				Ratio = -0.395; 
		}
   #endif
        
    if(Barrier_Flag3 == 1)		//回正后标志位清零
    {
        Sum_Angle_Bar = 0;
        Dis_Bar = 0;
        Barrier_Flag1 = 0;            
        Barrier_Flag2 = 0;
        Barrier_Executed = 1;
        
        Barrier_Flag3 = 0;
    }        
}


char Circle_Flag3 = 0;
void Elem_Circle(float Speed,float Gyro_Z)
{
    static char Elem_Over;
	static float Sum_Dis1 = 0;
    static float Sum_Angle_C1 = 0;
	if(Circle_Delay2 > 0)					//出环，清除标志位
	{
		Circle_Flag1 = 0;
		Circle_Flag2 = 0;
        if(Circle_Delay2 < 70)
		{
            if(Circle_Flag3 == LEFT_CIRCLE)		//用来记录出环时的方向
                Ratio -= 0.085;
            else if(Circle_Flag3 == RIGHT_CIRCLE)
                Ratio += 0.075;
		
        }
        Circle_Delay2--;
		return ;        					//退出函数
	}

	if(Circle_Flag1 == 1)					    //识别到环岛
	{
        Exp_Speed = 240;
       Gyro_Z = (Gyro_Z*2000)/32768;       
		if(Sum_Dis1 > DIS_ROUND_IN)			    //路程积分，积满进环
		{
			Sum_Angle_C1 += Gyro_Z * 0.005;     //进环开始角度积分
            //标志位未清零时只置位一次，防止重复识别
			if(Circle_Flag2 == 0 && (ADC_proc[1]+ ADC_proc[0] >= ADC_proc[3]+ADC_proc[4]))
			{
				Circle_Flag2 = LEFT_CIRCLE;     //判为左环岛
				x10_ms = 13;
			}
			else if(Circle_Flag2 == 0 && (ADC_proc[1]+ ADC_proc[0] < ADC_proc[3]+ADC_proc[4]))
			{
				Circle_Flag2 = RIGHT_CIRCLE;
				x10_ms = 13;
			}
			
			Circle_Flag3 = Circle_Flag2;		//临时变量记录方向
			
			if(Sum_Angle_C1 < 24 && Circle_Flag2 == LEFT_CIRCLE )	//角度未积满，固定偏差值
			{
                Ratio = 0.34;  
            }
			else if(Sum_Angle_C1 > -24 && Circle_Flag2 == RIGHT_CIRCLE)
			{
                Ratio = -0.34;
            }
		}
		else
			Sum_Dis1 += Speed;
		
		if(Sum_Angle_C1 > ROUND_L || Sum_Angle_C1 < ROUND_R )	           //出环条件之一，角度积分够大
		{
			if(ADC_proc[0] > 61 || ADC_proc[4] > 61 || ADC_proc[2] > 64)   //预出环 防止误判再次入环
			{
                Circle_Delay2 = 85;   //延时850ms
				Sum_Dis1 = 0;
				Sum_Angle_C1 = 0;
                Circle_Flag1 = 0;
				Circle_Flag2 = 0;
                if(Elem_Over == 0)
                {
                    Elem_Over = 1;
                    Special_Elem++;
                }
			}
		}
	}
	else
	{
		Sum_Dis1 = 0;
		Sum_Angle_C1 = 0;
		Circle_Flag2 = 0;
	}
}
