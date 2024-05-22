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

//#ifndef __DMA_AD_H_
//#define __DMA_AD_H_

//#include	"common.h"

//#define 	ADC_Interrupt(n)		(n==0?(EADC = 0):(EADC = 1))
//#define 	ADC_PowerOn(n)			(n==0?(ADC_POWER = 0):(ADC_POWER = 1))	/* ADC??????? */
//#define 	ADC_Justify(n)			(n==0?(RESFMT = 0):(RESFMT = 1))				/* ADC?????????????, ?????/????? */
//#define 	DMA_ADC_TRIG()					DMA_ADC_CR |= 0x40

//#define ADC_LEFT_JUSTIFIED		0		//ADC Result left-justified
//#define ADC_RIGHT_JUSTIFIED		1		//ADC Result right-justified

//#define SUCCESS		0
//#define FAIL		-1

//#define M2M_ADDR_INC			0
//#define M2M_ADDR_DEC			1

//#define	Priority_0			0	
//#define	Priority_1			1	
//#define	Priority_2			2	
//#define	Priority_3			3


//#define ADC_1_Times				0
//#define ADC_2_Times				8
//#define ADC_4_Times				9
//#define ADC_8_Times				10
//#define ADC_16_Times			11
//#define ADC_32_Times			12
//#define ADC_64_Times			13
//#define ADC_128_Times			14
//#define ADC_256_Times			15


//#define ADC_SPEED_2X1T		0			//SYSclk/2/1
//#define ADC_SPEED_2X2T		1			//SYSclk/2/2
//#define ADC_SPEED_2X3T		2			//SYSclk/2/3
//#define ADC_SPEED_2X4T		3			//SYSclk/2/4
//#define ADC_SPEED_2X5T		4			//SYSclk/2/5
//#define ADC_SPEED_2X6T		5			//SYSclk/2/6
//#define ADC_SPEED_2X7T		6			//SYSclk/2/7
//#define ADC_SPEED_2X8T		7			//SYSclk/2/8
//#define ADC_SPEED_2X9T		8			//SYSclk/2/9
//#define ADC_SPEED_2X10T		9			//SYSclk/2/10
//#define ADC_SPEED_2X11T		10		//SYSclk/2/11
//#define ADC_SPEED_2X12T		11		//SYSclk/2/12
//#define ADC_SPEED_2X13T		12		//SYSclk/2/13
//#define ADC_SPEED_2X14T		13		//SYSclk/2/14
//#define ADC_SPEED_2X15T		14		//SYSclk/2/15
//#define ADC_SPEED_2X16T		15		//SYSclk/2/16

//#define 	ADC_Priority(n)				do{if(n == 0) PADCH = 0, PADC = 0; \
//																if(n == 1) PADCH = 0, PADC = 1; \
//																if(n == 2) PADCH = 1, PADC = 0; \
//																if(n == 3) PADCH = 1, PADC = 1; \
//															}while(0)


//															
//typedef struct
//{
//	uint8	DMA_Enable;					//DMA???  	ENABLE,DISABLE
//	uint16	DMA_Channel;				//ADC??????????, 1:???, bit15~bit0 ??? ADC15~ADC0
//	uint16	DMA_Buffer;					//ADC???????��???
//	uint8	DMA_Times;					//?????????????, ADC_1_Times,ADC_2_Times,ADC_4_Times,ADC_8_Times,ADC_16_Times,ADC_32_Times,ADC_64_Times,ADC_128_Times,ADC_256_Times
//} DMA_ADC_InitTypeDef;
//															
//typedef struct
//{
//	uint8	ADC_SMPduty;		//ADC ???????????????, 0~31????? SMPDUTY ???????????��?? 10??
//	uint8	ADC_Speed;			//???? ADC ??????????	ADC_SPEED_2X1T~ADC_SPEED_2X16T
//	uint8	ADC_AdjResult;	//ADC???????,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
//	uint8	ADC_CsSetup;		//ADC ???????????? 0(???),1
//	uint8	ADC_CsHold;			//ADC ?????????????? 0,1(???),2,3
//} ADC_InitTypeDef;														
//						
//typedef struct
//{
//	uint8	DMA_Enable;					//DMAʹ��  	ENABLE,DISABLE
//	uint16	DMA_Rx_Buffer;			//�������ݴ洢��ַ
//	uint16	DMA_Tx_Buffer;			//�������ݴ洢��ַ
//	uint16	DMA_Length;					//DMA�������ֽ���  	(0~65535) + 1, ��Ҫ����оƬ xdata �ռ�����
//	uint8	DMA_SRC_Dir;				//����Դ��ַ�ı䷽��  	M2M_ADDR_INC,M2M_ADDR_DEC
//	uint8	DMA_DEST_Dir;				//����Ŀ���ַ�ı䷽�� 	M2M_ADDR_INC,M2M_ADDR_DEC
//} DMA_M2M_InitTypeDef;

//					
//extern float ADC_proc[];

//void DMA_AD_init(void);
//void ADC_GetValue(void);

//#endif

