#ifndef __Element_H
#define __Element_H
#include "PID.h"

#define RIGHT_CIRCLE		2
#define LEFT_CIRCLE			1

#define ROUND_R 			-115  	//车转一圈陀螺仪角度积分
#define ROUND_L 		 	115  

#define DIS_ROUND_IN        7000

#define BARRIER_FIELD_STATUS	0	// 可以根据赛道状态改变，1表示赛道周围空旷，可以常开避障
									// 0 表示赛道周围多干扰，只有经过障碍前
									// 最后一个特殊元素在开启避障
									
#define BARRIER_DIR 		0  		//0表示向右避障，1表示向左避障
									
									
extern char Barrier_Executed;
extern char Barrier_Flag1;
extern char Barrier_Flag2;
extern char Barrier_Flag3;


extern char Special_Elem;

extern float Circle_Flag1;
extern float Circle_Flag2;
extern int Circle_Delay1;	
extern int Circle_Delay2;

//void Elem_Barrier_Timer(void);  //2秒钟
void Elem_Up_Down(float Angle);  
void Elem_Circle(float Speed,float Gyro_Z);
void Elem_Barrier(float Gyro_Z,float Speed);

#endif