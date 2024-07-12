/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __ISR_H_
#define __ISR_H_


#define SUM_THRESHOLD  720

extern volatile float Exp_Speed;
extern float Exp_Speed_L;
extern float Exp_Speed_R;
extern float Diff;
extern float Plus;
extern float Ratio;
extern float Act_PwmL,Act_PwmR;

extern float Flag_Out_L;
extern float Flag_Out_R;
extern float Sum_Dis;
extern float Exp_Speed_gain;

extern short gx,gy,gz;
extern char Isr_Flag_10;
extern char Flag_Stop;
extern char Edge_Delay;





#endif