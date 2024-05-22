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

#include "SEEKFREE_WIRELESS_CH573.h"
#include "zf_uart.h"
#include "zf_fifo.h"


static  fifo_struct     wireless_ch573_fifo;
static  uint8           wireless_ch573_buffer[WIRELESS_CH573_BUFFER_SIZE];  // ���ݴ������

static          uint8   wireless_ch573_data;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      573ת����ģ��ص�����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void wireless_ch573_callback(void)
{
    //�ӵ�һ���ֽں�Ƭ��������봮���жϣ�ͨ���ڴ˴���ȡwireless_ch573_data����ȡ������
    wireless_ch573_data = WIRELESS_CH573_DATA_BUF;
    fifo_write_buffer(&wireless_ch573_fifo, &wireless_ch573_data, 1);       // ���� FIFO
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      573ת����ģ�� ���ͺ���
//  @param      buff        ��Ҫ���͵����ݵ�ַ
//  @param      len         ���ͳ���
//  @return     uint32      ʣ��δ���͵��ֽ���   
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_send_buff(uint8 *buff, uint16 len)
{
    while(len>30)
    {
        if(WIRELESS_CH573_RTS_PIN == 1)  
        {
            return len;//ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
        }
        //while(RTS_PIN);  //���RTSΪ�͵�ƽ���������������
        uart_putbuff(WIRELESS_CH573_UART,buff,30);

        buff += 30; //��ַƫ��
        len -= 30;//����
    }
    
    if(WIRELESS_CH573_RTS_PIN == 1)  
    {
        return len;//ģ��æ,�������ǰ����ʹ��while�ȴ� �����ʹ�ú���ע�͵�while�ȴ�����滻��if���
    }
    //while(WIRELESS_CH573_RTS_PIN);  //���RTSΪ�͵�ƽ���������������
    uart_putbuff(WIRELESS_CH573_UART,buff,len);//������������
    
    return 0;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת����ģ�� ��ȡ����
//  @param      buff            �洢�����ݵ�ַ
//  @param      len             ����
//  @return     uint32          ʵ�ʶ�ȡ�ֽ���
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint32 wireless_ch573_read_buff (uint8 *buff, uint32 len)
{
    uint32 data_len = len;
    fifo_read_buffer(&wireless_ch573_fifo, buff, &data_len, FIFO_READ_AND_CLEAN);
    return data_len;
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      573ת����ģ���ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void wireless_ch573_init(void)
{
    WIRELESS_CH573_RTS_PIN = 0;
    wireless_type = WIRELESS_CH573;
    //������ʹ�õĲ�����Ϊ115200��Ϊ573ת����ģ���Ĭ�ϲ����ʣ�������������������������ģ�鲢�޸Ĵ��ڵĲ�����
    fifo_init(&wireless_ch573_fifo, wireless_ch573_buffer, WIRELESS_CH573_BUFFER_SIZE);
    uart_init(WIRELESS_CH573_UART, WIRELESS_CH573_UART_RX_PIN, WIRELESS_CH573_UART_TX_PIN, WIRELESS_CH573_UART_BAUD, WIRELESS_CH573_TIMER_N);	//��ʼ������    
    
}
