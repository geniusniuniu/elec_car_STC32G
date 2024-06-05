#include "isr.h"
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "Element.h"
#include "Motor.h"
#include "PID.h"
#include "Buzzer.h"
#include "MPU6050.h"

extern float Exp_Speed;
extern float Ratio;
extern float ADC_proc[5];

//���±�־λ
char Down_Flag = 0;

//������־λ
char Circle_Flag1 = 0;  // ���һ�����־λ
char Circle_Flag2 = 0;


//������ʱ
int circle_Delay1 = 0;
int Circle_Delay2 = 0;

//��¼Ԫ�ظ���
char Special_Elem = 0;


void Elem_Up_Down(float Angle)  
{
    static char Elem_Over;
	if(Angle > -2)          //����
		Exp_Speed = 320;
	else if(Angle < -16)  //����
    {
        Down_Flag = 5;
        if(Elem_Over == 0)
        {
            Elem_Over=1;
            Special_Elem++;
        }
    }
    
}


//�ϰ���ʶ��   
char Barrier_Executed = 0;
char Barrier_Flag1 = 0;
void Elem_Barrier_Timer(void)  
{
	static float Barrier_Timer;	
	#if BARRIER_DIR == 0						//���ұ���
		if(Barrier_Flag1 == 1)					//ʶ���ϰ���
		{
			Barrier_Timer--;
			if(Barrier_Timer > 90)
				Ratio = -0.235;
			else if(Barrier_Timer > 30 && Barrier_Timer <= 90)
				Ratio = 0.5;
			else 
				Ratio -= 0.06;
			if(Barrier_Timer == 0)
			{
				Barrier_Flag1 = 0;
				Barrier_Executed = 1;
                Special_Elem = 0;
			}
		}
		else
			Barrier_Timer = 100;
	#elif BARRIER_DIR == 1  
		//�������
		if(Barrier_Flag1 == 1)					//ʶ���ϰ���
		{
			Barrier_Timer--;
			if(Barrier_Timer > 64)
				Ratio = 0.49;
			else if(Barrier_Timer > 18 && Barrier_Timer <= 64)
				Ratio = -0.43;
			else 
				Ratio += 0.04;
			if(Barrier_Timer == 0)
			{
				Barrier_Flag1 = 0;
				Barrier_Executed = 1;
                Special_Elem = 0;
			}
		}
		else
			Barrier_Timer = 100;
		
	#endif
}

char Circle_Flag3 = 0;

void Elem_Circle(float Speed,float Gyro_Z)
{
    static char Elem_Over;
	static float Sum_Dis1 = 0;
	static float Sum_Angle_C1 = 0;
	if(Circle_Delay2 > 0)					//�����������־λ
	{
		Circle_Flag1 = 0;
		Circle_Flag2 = 0;
		if(Circle_Flag3 == LEFT_CIRCLE)		//������¼����ʱ�ķ���
			Ratio -= 0.05;
		else if(Circle_Flag3 == RIGHT_CIRCLE)
			Ratio += 0.05;
		Circle_Delay2--;
		return ;        					//�˳�����
	}

	if(Circle_Flag1 == 1)					    //ʶ�𵽻���
	{
       Gyro_Z = (Gyro_Z*2000)/32768;       
		if(Sum_Dis1 > DIS_ROUND_IN)			    //·�̻��֣���������
		{
			Sum_Angle_C1 += Gyro_Z * 0.005;     //������ʼ�ǶȻ���
            //��־λδ����ʱֻ��λһ�Σ���ֹ�ظ�ʶ��
			if(Circle_Flag2 == 0 && (ADC_proc[1]+ ADC_proc[0] >= ADC_proc[3]+ADC_proc[4]))
			{
				Circle_Flag2 = LEFT_CIRCLE;     //��Ϊ�󻷵�
				x10_ms = 13;
			}
			else if(Circle_Flag2 == 0 && (ADC_proc[1]+ ADC_proc[0] < ADC_proc[3]+ADC_proc[4]))
			{
				Circle_Flag2 = RIGHT_CIRCLE;
				x10_ms = 13;
			}
			
			Circle_Flag3 = Circle_Flag2;		//��ʱ������¼����
			
			if(Sum_Angle_C1 < 24 && Circle_Flag2 == LEFT_CIRCLE )	//�Ƕ�δ�������̶�ƫ��ֵ
			{
                Ratio = 0.5;  
            }
			else if(Sum_Angle_C1 > -24 && Circle_Flag2 == RIGHT_CIRCLE)
			{
                Ratio = -0.5;
            }
		}
		else
			Sum_Dis1 += Speed;
		
		if(Sum_Angle_C1 > ROUND_L || Sum_Angle_C1 < ROUND_R )	           //��������֮һ���ǶȻ��ֹ���
		{
			if(ADC_proc[0] > 59 || ADC_proc[4] > 59 || ADC_proc[2] > 62)   //Ԥ���� ��ֹ�����ٴ��뻷
			{
                Circle_Delay2 = 100;   //��ʱ1000ms
				Sum_Dis1 = 0;
				Sum_Angle_C1 = 0;
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
