#include "isr.h"
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "zf_adc.h"
#include "Motor.h"

float ADC_Array_Original[5][3];
float ADC_proc[5];

void ADC_InitAll(void)
{
	adc_init(ADC_P10, ADC_SYSclk_DIV_2);
	adc_init(ADC_P00, ADC_SYSclk_DIV_2);
	
	adc_init(ADC_P16, ADC_SYSclk_DIV_2);

	adc_init(ADC_P05, ADC_SYSclk_DIV_2);
	adc_init(ADC_P06, ADC_SYSclk_DIV_2);
}

//��ȡ����ADC��ͨ��ֵ����ȡƽ��ֵ
void ADC_GetValue(void)
{
	float temp;
	char i,j;
	for(i=0;i<3;i++)
	{
		ADC_Array_Original[0][i] = adc_once(ADC_P10, ADC_8BIT);		//��һ·���
		ADC_Array_Original[1][i] = adc_once(ADC_P00, ADC_8BIT);		//���·���	

		ADC_Array_Original[2][i] = adc_once(ADC_P05, ADC_8BIT);		//����·���
		
		ADC_Array_Original[3][i] = adc_once(ADC_P06, ADC_8BIT);		//�м���� 
		ADC_Array_Original[4][i] = adc_once(ADC_P16, ADC_8BIT);		//����·���
	}
	//ȡ���ε��ֵƽ��ֵ
	for(i=0;i<5;i++)
	{
		temp = 0;
		for(j=0;j<3;j++)
		{
			temp += ADC_Array_Original[i][j];
		}
		ADC_proc[i] = temp/3;
		//�Ե��ֵ�޷�
		if(ADC_proc[i] >= 200)			ADC_proc[i] = 200;	
		else if(ADC_proc[i] <= 2)		ADC_proc[i] = 2;
		//��һ��,�����ֵ������0~100֮��
		ADC_proc[i] = 100*(ADC_proc[i]/200);	

	}
}