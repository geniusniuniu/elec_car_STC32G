/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
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