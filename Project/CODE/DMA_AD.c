///*---------------------------------------------------------------------*/
///* --- STC MCU Limited ------------------------------------------------*/
///* --- STC 1T Series MCU Demo Programme -------------------------------*/
///* --- Mobile: (86)13922805190 ----------------------------------------*/
///* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
///* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
///* --- Web: www.STCMCU.com --------------------------------------------*/
///* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
///* --- QQ:  800003751 -------------------------------------------------*/
///* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
///*---------------------------------------------------------------------*/
//#include "headfile.h"
//#include "DMA_AD.h"


///*************	����˵��	**************

//��������ʾ��·ADC DMA����.

//��ʼ��ʱ�Ȱ�ҪADCת������������Ϊ��������.

//����������������(DMA)���ܣ�����ͨ��һ��ѭ���ɼ��������Զ���ŵ�DMA�����xdata�ռ�.

//******************************************/


////========================================================================
////                               ���س�������	
////========================================================================

//#define	ADC_CH		4			/* 1~16, ADCת��ͨ����, ��ͬ���޸�ת��ͨ�� */
//#define	ADC_DATA	3			/* 6~n, ÿ��ͨ��ADCת����������, 2*ת������+4, ��ͬ���޸�ת������ */


////========================================================================
////                               ���ر�������
////========================================================================

//uint8 chn = 0;
//uint8 xdata DmaAdBuffer[ADC_CH][ADC_DATA];
//float ADC_proc[ADC_CH];  // ����ADC_proc����

////========================================================================
////                               ���غ�������
////========================================================================


//// ����: uint8	ADC_Inilize(ADC_InitTypeDef *ADCx)
//// ����: ADC��ʼ������.
//// ����: ADCx: �ṹ����,��ο�adc.h��Ķ���.
//// ����: none.
//// �汾: V1.0, 2012-10-22
////========================================================================
//uint8 ADC_Init1(ADC_InitTypeDef *ADCx)
//{
//	ADCCFG = (ADCCFG & ~ADC_SPEED_2X16T) | ADCx->ADC_Speed;	//����ADC����ʱ��Ƶ��
//	ADC_Justify(ADCx->ADC_AdjResult);		//ADת��������뷽ʽ

//	if(ADCx->ADC_SMPduty > 31)	return FAIL;	//����
//	if(ADCx->ADC_CsSetup > 1)	return FAIL;	//����
//	if(ADCx->ADC_CsHold > 3)	return FAIL;	//����

//	ADCTIM = (ADCx->ADC_CsSetup << 7) | (ADCx->ADC_CsHold << 5) | ADCx->ADC_SMPduty ;		//���� ADC �ڲ�ʱ��ADC����ʱ�佨�������ֵ
//	return SUCCESS;
//}

////========================================================================
//// ����: void DMA_ADC_Inilize(DMA_ADC_InitTypeDef *DMA)
//// ����: DMA ADC ��ʼ������.
//// ����: DMA: �ṹ����,��ο�DMA.h��Ķ���.
//// ����: none.
//// �汾: V1.0, 2021-05-17
////========================================================================
//void DMA_ADC_Init(DMA_ADC_InitTypeDef *DMA)
//{
//	DMA_ADC_STA = 0x00;
//	DMA_ADC_CHSW1 = (uint8)(DMA->DMA_Channel>>8);
//	DMA_ADC_CHSW0 = (uint8)(DMA->DMA_Channel);
//	DMA_ADC_RXAH = (uint8)(DMA->DMA_Buffer>>8);
//	DMA_ADC_RXAL = (uint8)(DMA->DMA_Buffer);
//	DMA_ADC_CFG2 = DMA->DMA_Times;

//	if(DMA->DMA_Enable == ENABLE)		DMA_ADC_CR |= 0x80;	//ʹ��ADC DMA
//	else DMA_ADC_CR &= ~0x80;	//��ֹADC DMA
//}

////========================================================================
//// ����: void	ADC_PowerControl(uint8 pwr)
//// ����: ADC��Դ���Ƴ���.
//// ����: pwr: ��Դ����,ENABLE��DISABLE.
//// ����: none.
//// �汾: V1.0, 2012-10-22
////========================================================================
//void ADC_PowerControl(uint8 pwr)
//{
//	if(pwr == ENABLE)	ADC_POWER = 1;
//	else				ADC_POWER = 0;
//}

////========================================================================
//// ����: NVIC_ADC_Init
//// ����: ADCǶ�������жϿ�������ʼ��.
//// ����: State:    �ж�ʹ��״̬, ENABLE/DISABLE.
//// ����: Priority: �ж����ȼ�, Priority_0,Priority_1,Priority_2,Priority_3.
//// ����: ִ�н�� SUCCESS/FAIL.
//// �汾: V1.0, 2020-09-29
////========================================================================
//uint8 NVIC_ADC_Init(uint8 State, uint8 Priority)
//{
//	if(State <= ENABLE) ADC_Interrupt(State); else  return FAIL;
//	if(Priority <= Priority_3) ADC_Priority(Priority); else  return FAIL;
//	return SUCCESS;
//}

////========================================================================
//// ����: NVIC_DMA_ADC_Init
//// ����: DMA ADCǶ�������жϿ�������ʼ��.
//// ����: State:    �ж�ʹ��״̬, ENABLE/DISABLE.
//// ����: Priority: �ж����ȼ�, Priority_0,Priority_1,Priority_2,Priority_3.
//// ����: Bus_Priority: �������߷������ȼ�, Priority_0,Priority_1,Priority_2,Priority_3.
//// ����: ִ�н�� SUCCESS/FAIL.
//// �汾: V1.0, 2021-05-21
////========================================================================
//uint8 NVIC_DMA_ADC_Init(uint8 State, uint8 Priority, uint8 Bus_Priority)
//{
//	DMA_ADC_CFG &= ~0x0f;
//	if(Priority <= Priority_3) DMA_ADC_CFG |= Priority << 2;
//	if(Bus_Priority <= Priority_3) DMA_ADC_CFG |= Bus_Priority;	//�������߷������ȼ�
//	if(State == ENABLE)
//		DMA_ADC_CFG |= 0x80;		//bit7 1:Enable Interrupt
//	else
//		DMA_ADC_CFG &= ~0x80;		//bit7 0:Disable Interrupt
//	return SUCCESS;
//}

//void DMA_M2M_Inilize(DMA_M2M_InitTypeDef *DMA)
//{
//	DMA_M2M_STA = 0x00;
//	DMA_M2M_RXAH = (uint8)(DMA->DMA_Rx_Buffer>>8);
//	DMA_M2M_RXAL = (uint8)(DMA->DMA_Rx_Buffer);
//	DMA_M2M_TXAH = (uint8)(DMA->DMA_Tx_Buffer>>8);
//	DMA_M2M_TXAL = (uint8)(DMA->DMA_Tx_Buffer);
//	DMA_M2M_AMT = (uint8)DMA->DMA_Length;				//���ô������ֽ���(��8λ)��n+1
//	DMA_M2M_AMTH = (uint8)(DMA->DMA_Length>>8);	//���ô������ֽ���(��8λ)��n+1
//	
//	if(DMA->DMA_SRC_Dir == M2M_ADDR_DEC)		DMA_M2M_CFG |= 0x20;	//���ݶ�����ַ�Լ�
//	else DMA_M2M_CFG &= ~0x20;	//���ݶ�����ַ�Լ�
//	if(DMA->DMA_DEST_Dir == M2M_ADDR_DEC)		DMA_M2M_CFG |= 0x10;	//����д����ַ�Լ�
//	else DMA_M2M_CFG &= ~0x10;	//����д����ַ�Լ�
//	
//	if(DMA->DMA_Enable == ENABLE)		DMA_M2M_CR |= 0x80;	//ʹ��M2M DMA
//	else DMA_M2M_CR &= ~0x80;	//��ֹM2M DMA
//}


////ʹ��P0_0��P0_5��P0_6��P1_6�ĸ����Ŷ�ȡADֵ��ͨ��DMA���䵽DmaAdBuffer������
//void DMA_AD_init(void)
//{
//	ADC_InitTypeDef ADC_InitStructure;
//	DMA_ADC_InitTypeDef DMA_ADC_InitStructure;
//	DMA_M2M_InitTypeDef DMA_M2M_InitStructure;
//	
//	ADC_InitStructure.ADC_Speed = ADC_SPEED_2X16T;	//����ADC����ʱ��Ƶ��
//	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADת��������뷽ʽ
//	ADC_InitStructure.ADC_SMPduty = 31;				//����ADC����ʱ��
//	ADC_InitStructure.ADC_CsSetup = 0;				//����ADC�ڲ�ʱ��
//	ADC_InitStructure.ADC_CsHold = 0;				//����ADC�ڲ�ʱ��
//	ADC_Init1(&ADC_InitStructure);					//��ʼ��ADC
//	
//	DMA_ADC_InitStructure.DMA_Channel = ADC_CH;		//����DMAͨ��
//	DMA_ADC_InitStructure.DMA_Buffer = (uint16)DmaAdBuffer;	//����DMA������
//	DMA_ADC_InitStructure.DMA_Times = ADC_DATA;		//����DMA�������
//	DMA_ADC_InitStructure.DMA_Enable = ENABLE;		//����DMAʹ��
//	DMA_ADC_Init(&DMA_ADC_InitStructure);			//��ʼ��DMA
//	
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (uint16)DmaAdBuffer;	//����DMA���ջ�����
//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (uint16)DmaAdBuffer;	//����DMA���ͻ�����
//	DMA_M2M_InitStructure.DMA_Length = ADC_CH*ADC_DATA;	//����DMA���䳤��
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_INC;	//����DMAԴ��ַ����
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_INC;	//����DMAĿ�ĵ�ַ����
//	DMA_M2M_InitStructure.DMA_Enable = ENABLE;	//����DMAʹ��
//	DMA_M2M_Inilize(&DMA_M2M_InitStructure);		//��ʼ��DMA
//	
//	NVIC_ADC_Init(ENABLE, Priority_3);	//��ʼ��ADC�ж�
//	NVIC_DMA_ADC_Init(ENABLE, Priority_3, Priority_3);	//��ʼ��DMA ADC�ж�
//	ADC_PowerControl(ENABLE);	//ʹ��ADC
//	DMA_ADC_TRIG();		//����ת��

//}

// 
//void ADC_GetValue(void)
// {
//     float temp;
//     char j, k;

//     for (k = 0; k < ADC_CH; k++) 
// 	{
//         temp = 0;
//         for (j = 0; j < ADC_DATA; j++) 
// 		{
//             temp += DmaAdBuffer[k][j];
//         }
//         temp /= ADC_DATA * 1.0;
//         if (temp >= 200)            temp = 200;    // �Ե��ֵ�����޷�����
//         else if (temp <= 2)         temp = 2;    // �Ե��ֵ�����޷�����

//         ADC_proc[k] = 100 * (temp / 200);        // ��һ��,�����ֵ������0~100֮��
//     }
//	 while(DMA_ADC_STA & 0x01);	//�ȴ�DMA�������
//	 DMA_ADC_STA = 0x01;	//���DMA������ɱ�־
//	 DMA_ADC_TRIG();		//����ת��
// }


