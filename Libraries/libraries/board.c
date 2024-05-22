/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		board
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/
 
 
#include "board.h"
#include "zf_uart.h"
#include "zf_tim.h"
#include "zf_delay.h"

//22.11MHz��IRC�����Ĵ��� 0xFB
//24MHz��IRC�����Ĵ��� 0xFB
#define IRC_22M (*((uint8  idata*)0xFA))
#define IRC_24M (*((uint8  idata*)0xFB))


//�ں�Ƶ��
int32 sys_clk = 0;


//void set_clk(uint32 clk)
//{
//	if(33177600 == clk)
//	{
//		IRCBAND = 1;
//		LIRTRIM = 3;
//		IRTRIM = 38;
//		CLKDIV = 0;
//	}
//	else if(30000000 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 3;
//		IRTRIM = 227;
//		CLKDIV = 0;
//	}
//	else if(27000000 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 3;
//		IRTRIM = 171;
//		CLKDIV = 0;
//	}
//	else if(24000000 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 1;
//		IRTRIM = 115;
//		CLKDIV = 0;
//	}
//	else if(22118400 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 3;
//		IRTRIM = 80;
//		CLKDIV = 0;
//	}
//	else if(20000000 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 1;
//		IRTRIM = 40;
//		CLKDIV = 0;
//	}
//	else if(18432000 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 1;
//		IRTRIM = 12;
//		CLKDIV = 0;
//	}
//	else if(12000000 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 1;
//		IRTRIM = 115;
//		CLKDIV = 1;
//	}
//	else if(11059200 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 3;
//		IRTRIM = 80;
//		CLKDIV = 1;
//	}	
//	else if(6000000 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 1;
//		IRTRIM = 115;
//		CLKDIV = 3;
//	}
//	else if(5529600 == clk)
//	{
//		IRCBAND = 0;
//		LIRTRIM = 3;
//		IRTRIM = 80;
//		CLKDIV = 3;
//	}
//	else	//������� ������	30M
//	{
//		IRCBAND = 0;
//		LIRTRIM = 3;
//		IRTRIM = 227;
//		CLKDIV = 0;
//	}
//}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡϵͳƵ��
//  @param      NULL          	��ֵ
//  @return     void        	ϵͳƵ��
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
//uint32 get_clk(void)
//{
//	uint32 temp_count;
//	P_SW2 |= 0x80;
//	
//	if(IRCBAND)
//		temp_count = 36000000UL + ((int32)((int32)IRTRIM - (int32)IRC_22M) * 0x128E0UL); //Ƶ�ʵ�ƫ��,������������
//	else
//		temp_count = 24000000UL + ((int32)((int32)IRTRIM - (int32)IRC_24M) * 0xBB80UL);  //Ƶ�ʵ�ƫ��,������������
//	
//		temp_count /= CLKDIV;                              	  		//Ƶ��̫����Ҫ��Ƶ
//	
//	if 	(temp_count < 5764800UL)
//		return 5529600UL;
//	else if(temp_count < 8529600UL)
//		return 6000000UL;
//	else if(temp_count < 11529600UL)
//		return 11059200UL;
//	else if(temp_count < 15216000UL)
//		return  12000000UL;
//	else if(temp_count < 19216000UL)
//		return  18432000UL;
//	else if(temp_count < 21059200UL)
//		return  20000000UL;
//	else if(temp_count < 23059200UL)
//		return  22118400UL;
//	else if(temp_count < 25500000UL)
//		return  24000000UL;
//	else if(temp_count < 28500000UL)
//		return  27000000UL;
//	else if(temp_count < 31500000UL)
//		return  30000000UL;
//	else if(temp_count < 33500000UL)
//		return  33177600UL;
//	else if(temp_count < 35932000UL)
//		return  35000000UL;
//	else if(temp_count < 38432000UL)
//		return  36864000UL;
//	else if(temp_count < 42000000UL)
//		return  40000000UL;
//	else if(temp_count < 46000000UL)
//		return  44236800UL;
//	else 
//		return 48000000UL;
//}

void board_init(void)
{
	SET_P54_RESRT;
	
//	//��ȡϵͳƵ��
//#if (0 != FOSC)
//	sys_clk = FOSC;
//	set_clk(sys_clk);
//#endif


#if (1 == EXTERNAL_CRYSTA_ENABLE)
	P_SW2 = 0x80;
	XOSCCR = 0xc0; 			//�����ⲿ����
	while (!(XOSCCR & 1)); 	//�ȴ�ʱ���ȶ�
	CLKDIV = 0x00; 			//ʱ�Ӳ���Ƶ
	CLKSEL = 0x01; 			//ѡ���ⲿ����
	P_SW2 = 0x00;
#endif

	delay_init();			//��ʱ������ʼ��
	
	WTST = 0;
    P_SW2 |= 0x80;
    CLKDIV = 0;				//24MHz��Ƶ����Ƶ����
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	P6M0 = 0x00;
	P6M1 = 0x00;
	P7M0 = 0x00;
	P7M1 = 0x00;
	
	ADCCFG = 0;
	AUXR = 0;
	SCON = 0;
	S2CON = 0;
	S3CON = 0;
	S4CON = 0;
	P_SW1 = 0;
	P_SW2 = 0;
	IE2 = 0;
	TMOD = 0;

	uart_init(DEBUG_UART, DEBUG_UART_RX_PIN, DEBUG_UART_TX_PIN, DEBUG_UART_BAUD, DEBUG_UART_TIM);
}


#if (1 == PRINTF_ENABLE)      //��ʼ�����Դ���
//�ض���printf ���� ֻ�����uint16
char putchar(char c)
{
	uart_putchar(UART_1, c);//���Լ�ʵ�ֵĴ��ڴ�ӡһ�ֽ����ݵĺ����滻������

	return c;
}
#endif

void DisableGlobalIRQ(void)
{
	EA = 0;
}


void EnableGlobalIRQ(void)
{
	EA = 1;
}

