#include "isr.h"
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "zf_gpio.h"
#include "zf_delay.h"
#include "HCSR04.h"

volatile int time = 0;

void HC_SR04_Init(void)
{
	gpio_mode(Echo_PIN, GPIO);			//˫��ģʽ
	gpio_pull_set(Echo_PIN, NOPULL);	//����ģʽ
	gpio_mode(Trig_PIN, GPO_PP);		//�������
    
	Trig_PIN = 0;	//GPIO_WriteBit(GPIOA,Trig_PIN,(BitAction)0);				
	delay_us(15);	//��ʱ15΢��
}

float sonar(void) 
{
    static float Distance_mm;
    static float time_end = 0;		//�洢�ز��ź�ʱ��
    static int i = 0,j = 0;
	Trig_PIN = 1;			// GPIO_WriteBit(GPIOA, Trig_PIN, Bit_SET);
    delay_us(15);   		// ����������������10΢��
	Trig_PIN = 0;			//GPIO_WriteBit(GPIOA, Trig_PIN, Bit_RESET);
	
	while(Echo_PIN == 0)    //while (GPIO_ReadInputDataBit(GPIOA, Echo_PIN) == 0);
    {
//        i++;
//        if(i>10000)  
//        {
//            i = 0;
//            return -1;
//        }
     }
    
    time = 0; 				//��¼��ʼʱ��
    
	while(Echo_PIN == 1)		//while (GPIO_ReadInputDataBit(GPIOA, Echo_PIN) == 1);
    {
//       j++;
//       if(j>10000)  
//       {
//           j = 0;
//           return -2;
//       }
    }
    time_end = time; 		// ����ʱ���
    
	if(time_end < 3800)
    {
		Distance_mm = (SOUND_SPEED * time_end) / 20.0;
	} 
    return Distance_mm;
}
