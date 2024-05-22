/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file             zf_device_wireless_ch573
* @company          �ɶ���ɿƼ����޹�˾
* @author           ��ɿƼ�(QQ790875685)
* @version          �鿴doc��version�ļ� �汾˵��
* @Software         MounRiver Studio V1.51
* @Target core      CH32V307VCT6
* @Taobao           https://seekfree.taobao.com/
* @date             2022-03-02
* @note
********************************************************************************************************************/
#ifndef _SEEKFREE_WIRELESS_CH573_H_
#define _SEEKFREE_WIRELESS_CH573_H_


#include "common.h"

#define WIRELESS_CH573_UART        		UART_4         //CH573ת����ģ�� ��ʹ�õ��Ĵ���     
#define WIRELESS_CH573_UART_TX_PIN    	UART4_TX_P03
#define WIRELESS_CH573_UART_RX_PIN    	UART4_RX_P02
#define WIRELESS_CH573_TIMER_N    		TIM_2
#define WIRELESS_CH573_UART_BAUD   		115200
#define WIRELESS_CH573_DATA_BUF         S4BUF



#define WIRELESS_CH573_RTS_PIN P07 			//��������λ����  ָʾ��ǰģ���Ƿ���Խ�������  0���Լ�������  1�����Լ�������
//#define WIRELESS_CH573_CMD_PIN P05 			//������������


#define WIRELESS_CH573_BUFFER_SIZE       64
#define WIRELESS_CH573_TIMEOUT_COUNT     500


void wireless_ch573_init(void);
void wireless_ch573_callback(void);

uint32 wireless_ch573_send_buff(uint8 *buff, uint16 len);
uint32 wireless_ch573_read_buff (uint8 *buff, uint32 len);


#endif
