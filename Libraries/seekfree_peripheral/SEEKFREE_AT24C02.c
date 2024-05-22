/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		����ʾ����Э��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note		    
 ********************************************************************************************************************/
#include "zf_delay.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_AT24C02.h"


#pragma warning disable = 173

//-------------------------------------------------------------------------------------------------------------------
//  @brief      at24c02дһ���ֽ�
//  @param      byte_reg	at24c02��ַ(0-255)
//  @param      dat			��Ҫд�������
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void at24c02_write_byte(uint8 byte_reg, uint8 dat)
{
	delay_ms(6);
	simiic_write_reg(AT24C02_DEV_ADDR, byte_reg, dat);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      at24c02��һ���ֽ�
//  @param      byte_reg	at24c02��ַ(0-255)
//  @return     uint8		���ض�ȡ�����ֽ���					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
uint8 at24c02_read_byte(uint8 byte_reg)
{
	delay_ms(6);
	return simiic_read_reg(AT24C02_DEV_ADDR, byte_reg, SIMIIC);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      at24c02д����ֽ�
//  @param      byte_reg	at24c02��ʼ��ַ(0-255)
//  @param      dat_add		��Ҫд�������ָ��
//  @param      num			��Ҫд����ٸ�����
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void at24c02_write_bytes(uint8 byte_reg, uint8 *dat_add, uint8 num)
{
	delay_ms(6);
	simiic_start();
    send_ch( (AT24C02_DEV_ADDR<<1) | 0x00);
	send_ch( byte_reg );   				

    while(--num)
    {
		send_ch( *dat_add++ );   				
		byte_reg++;
		if((byte_reg % 8) == 0)
		{
			simiic_stop();
			delay_ms(6);
			simiic_start();
			send_ch( (AT24C02_DEV_ADDR<<1) | 0x00);
			send_ch( byte_reg );   				
		}
    }
	send_ch( *dat_add++ );   
	simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      at24c02������ֽ�
//  @param      byte_reg	at24c02��ʼ��ַ(0-255)
//  @param      dat_add		��Ҫ����������ָ��
//  @param      num			��Ҫ�������ٸ�����
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void at24c02_read_bytes(uint8 byte_reg, uint8 *dat_add, uint8 num)
{
	delay_ms(5);
	simiic_start();
    send_ch( (AT24C02_DEV_ADDR<<1) | 0x00);  //����������ַ��дλ
	send_ch( byte_reg );   					 //���ʹӻ��Ĵ�����ַ
	delay_ms(5);
	simiic_start();
	send_ch( (AT24C02_DEV_ADDR<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add++ = read_ch(1); //��ȡ����
		byte_reg++;
    }
	*dat_add++ = read_ch(0); //��ȡ����
	simiic_stop();
}
