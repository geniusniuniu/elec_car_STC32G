///*---------------------------------------------------------------------*/
///* --- STC MCU Limited ------------------------------------------------*/
///* --- STC 1T Series MCU Demo Programme -------------------------------*/
///* --- Mobile: (86)13922805190 ----------------------------------------*/
///* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
///* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
///* --- Web: www.STCMCU.com --------------------------------------------*/
///* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
///* --- QQ:  800003751 -------------------------------------------------*/
///* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
///*---------------------------------------------------------------------*/
//#include "headfile.h"
//#include "DMA_AD.h"


///*************	功能说明	**************

//本程序演示多路ADC DMA采样.

//初始化时先把要ADC转换的引脚设置为高阻输入.

//设置数据批量传输(DMA)功能，所有通道一次循环采集的数据自动存放到DMA定义的xdata空间.

//******************************************/


////========================================================================
////                               本地常量声明	
////========================================================================

//#define	ADC_CH		4			/* 1~16, ADC转换通道数, 需同步修改转换通道 */
//#define	ADC_DATA	3			/* 6~n, 每个通道ADC转换数据总数, 2*转换次数+4, 需同步修改转换次数 */


////========================================================================
////                               本地变量声明
////========================================================================

//uint8 chn = 0;
//uint8 xdata DmaAdBuffer[ADC_CH][ADC_DATA];
//float ADC_proc[ADC_CH];  // 定义ADC_proc数组

////========================================================================
////                               本地函数声明
////========================================================================


//// 函数: uint8	ADC_Inilize(ADC_InitTypeDef *ADCx)
//// 描述: ADC初始化程序.
//// 参数: ADCx: 结构参数,请参考adc.h里的定义.
//// 返回: none.
//// 版本: V1.0, 2012-10-22
////========================================================================
//uint8 ADC_Init1(ADC_InitTypeDef *ADCx)
//{
//	ADCCFG = (ADCCFG & ~ADC_SPEED_2X16T) | ADCx->ADC_Speed;	//设置ADC工作时钟频率
//	ADC_Justify(ADCx->ADC_AdjResult);		//AD转换结果对齐方式

//	if(ADCx->ADC_SMPduty > 31)	return FAIL;	//错误
//	if(ADCx->ADC_CsSetup > 1)	return FAIL;	//错误
//	if(ADCx->ADC_CsHold > 3)	return FAIL;	//错误

//	ADCTIM = (ADCx->ADC_CsSetup << 7) | (ADCx->ADC_CsHold << 5) | ADCx->ADC_SMPduty ;		//设置 ADC 内部时序，ADC采样时间建议设最大值
//	return SUCCESS;
//}

////========================================================================
//// 函数: void DMA_ADC_Inilize(DMA_ADC_InitTypeDef *DMA)
//// 描述: DMA ADC 初始化程序.
//// 参数: DMA: 结构参数,请参考DMA.h里的定义.
//// 返回: none.
//// 版本: V1.0, 2021-05-17
////========================================================================
//void DMA_ADC_Init(DMA_ADC_InitTypeDef *DMA)
//{
//	DMA_ADC_STA = 0x00;
//	DMA_ADC_CHSW1 = (uint8)(DMA->DMA_Channel>>8);
//	DMA_ADC_CHSW0 = (uint8)(DMA->DMA_Channel);
//	DMA_ADC_RXAH = (uint8)(DMA->DMA_Buffer>>8);
//	DMA_ADC_RXAL = (uint8)(DMA->DMA_Buffer);
//	DMA_ADC_CFG2 = DMA->DMA_Times;

//	if(DMA->DMA_Enable == ENABLE)		DMA_ADC_CR |= 0x80;	//使能ADC DMA
//	else DMA_ADC_CR &= ~0x80;	//禁止ADC DMA
//}

////========================================================================
//// 函数: void	ADC_PowerControl(uint8 pwr)
//// 描述: ADC电源控制程序.
//// 参数: pwr: 电源控制,ENABLE或DISABLE.
//// 返回: none.
//// 版本: V1.0, 2012-10-22
////========================================================================
//void ADC_PowerControl(uint8 pwr)
//{
//	if(pwr == ENABLE)	ADC_POWER = 1;
//	else				ADC_POWER = 0;
//}

////========================================================================
//// 函数: NVIC_ADC_Init
//// 描述: ADC嵌套向量中断控制器初始化.
//// 参数: State:    中断使能状态, ENABLE/DISABLE.
//// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
//// 返回: 执行结果 SUCCESS/FAIL.
//// 版本: V1.0, 2020-09-29
////========================================================================
//uint8 NVIC_ADC_Init(uint8 State, uint8 Priority)
//{
//	if(State <= ENABLE) ADC_Interrupt(State); else  return FAIL;
//	if(Priority <= Priority_3) ADC_Priority(Priority); else  return FAIL;
//	return SUCCESS;
//}

////========================================================================
//// 函数: NVIC_DMA_ADC_Init
//// 描述: DMA ADC嵌套向量中断控制器初始化.
//// 参数: State:    中断使能状态, ENABLE/DISABLE.
//// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
//// 参数: Bus_Priority: 数据总线访问优先级, Priority_0,Priority_1,Priority_2,Priority_3.
//// 返回: 执行结果 SUCCESS/FAIL.
//// 版本: V1.0, 2021-05-21
////========================================================================
//uint8 NVIC_DMA_ADC_Init(uint8 State, uint8 Priority, uint8 Bus_Priority)
//{
//	DMA_ADC_CFG &= ~0x0f;
//	if(Priority <= Priority_3) DMA_ADC_CFG |= Priority << 2;
//	if(Bus_Priority <= Priority_3) DMA_ADC_CFG |= Bus_Priority;	//数据总线访问优先级
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
//	DMA_M2M_AMT = (uint8)DMA->DMA_Length;				//设置传输总字节数(低8位)：n+1
//	DMA_M2M_AMTH = (uint8)(DMA->DMA_Length>>8);	//设置传输总字节数(高8位)：n+1
//	
//	if(DMA->DMA_SRC_Dir == M2M_ADDR_DEC)		DMA_M2M_CFG |= 0x20;	//数据读完后地址自减
//	else DMA_M2M_CFG &= ~0x20;	//数据读完后地址自减
//	if(DMA->DMA_DEST_Dir == M2M_ADDR_DEC)		DMA_M2M_CFG |= 0x10;	//数据写入后地址自减
//	else DMA_M2M_CFG &= ~0x10;	//数据写入后地址自减
//	
//	if(DMA->DMA_Enable == ENABLE)		DMA_M2M_CR |= 0x80;	//使能M2M DMA
//	else DMA_M2M_CR &= ~0x80;	//禁止M2M DMA
//}


////使用P0_0，P0_5，P0_6，P1_6四个引脚读取AD值，通过DMA传输到DmaAdBuffer数组中
//void DMA_AD_init(void)
//{
//	ADC_InitTypeDef ADC_InitStructure;
//	DMA_ADC_InitTypeDef DMA_ADC_InitStructure;
//	DMA_M2M_InitTypeDef DMA_M2M_InitStructure;
//	
//	ADC_InitStructure.ADC_Speed = ADC_SPEED_2X16T;	//设置ADC工作时钟频率
//	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//AD转换结果对齐方式
//	ADC_InitStructure.ADC_SMPduty = 31;				//设置ADC采样时间
//	ADC_InitStructure.ADC_CsSetup = 0;				//设置ADC内部时序
//	ADC_InitStructure.ADC_CsHold = 0;				//设置ADC内部时序
//	ADC_Init1(&ADC_InitStructure);					//初始化ADC
//	
//	DMA_ADC_InitStructure.DMA_Channel = ADC_CH;		//设置DMA通道
//	DMA_ADC_InitStructure.DMA_Buffer = (uint16)DmaAdBuffer;	//设置DMA缓冲区
//	DMA_ADC_InitStructure.DMA_Times = ADC_DATA;		//设置DMA传输次数
//	DMA_ADC_InitStructure.DMA_Enable = ENABLE;		//设置DMA使能
//	DMA_ADC_Init(&DMA_ADC_InitStructure);			//初始化DMA
//	
//	DMA_M2M_InitStructure.DMA_Rx_Buffer = (uint16)DmaAdBuffer;	//设置DMA接收缓冲区
//	DMA_M2M_InitStructure.DMA_Tx_Buffer = (uint16)DmaAdBuffer;	//设置DMA发送缓冲区
//	DMA_M2M_InitStructure.DMA_Length = ADC_CH*ADC_DATA;	//设置DMA传输长度
//	DMA_M2M_InitStructure.DMA_SRC_Dir = M2M_ADDR_INC;	//设置DMA源地址自增
//	DMA_M2M_InitStructure.DMA_DEST_Dir = M2M_ADDR_INC;	//设置DMA目的地址自增
//	DMA_M2M_InitStructure.DMA_Enable = ENABLE;	//设置DMA使能
//	DMA_M2M_Inilize(&DMA_M2M_InitStructure);		//初始化DMA
//	
//	NVIC_ADC_Init(ENABLE, Priority_3);	//初始化ADC中断
//	NVIC_DMA_ADC_Init(ENABLE, Priority_3, Priority_3);	//初始化DMA ADC中断
//	ADC_PowerControl(ENABLE);	//使能ADC
//	DMA_ADC_TRIG();		//启动转换

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
//         if (temp >= 200)            temp = 200;    // 对电感值进行限幅处理
//         else if (temp <= 2)         temp = 2;    // 对电感值进行限幅处理

//         ADC_proc[k] = 100 * (temp / 200);        // 归一化,将电感值限制在0~100之间
//     }
//	 while(DMA_ADC_STA & 0x01);	//等待DMA传输完成
//	 DMA_ADC_STA = 0x01;	//清除DMA传输完成标志
//	 DMA_ADC_TRIG();		//启动转换
// }


