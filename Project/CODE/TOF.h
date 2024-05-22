/*********************************************************************************************************************
* MM32F527X-E9P Opensourec Library ����MM32F527X-E9P ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
* 
* ���ļ��� MM32F527X-E9P ��Դ���һ����
* 
* MM32F527X-E9P ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
* 
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
* 
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
* 
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
* 
* �ļ�����          zf_device_vl53l0x
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MDK 5.37
* ����ƽ̨          MM32F527X_E9P
* ��������          https://seekfree.taobao.com/
* 
* �޸ļ�¼
* ����              ����                ��ע
* 2022-08-10        Teternal            first version
********************************************************************************************************************/
/*********************************************************************************************************************
* ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   SCL                 �鿴 zf_device_vl53l0x.h �� VL53L0X_SCL_PIN  �궨��
*                   SDA                 �鿴 zf_device_vl53l0x.h �� VL53L0X_SDA_PIN  �궨��
*                   VCC                 5V ��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ZF_DEVICE_VL53L0X_H_
#define _ZF_DEVICE_VL53L0X_H_

#include "common.h"
#include "board.h"

// ��Ҫע����� VL53L0X �ֲ����֧�� 400KHz �� IIC ͨ������
// ��Ҫע����� VL53L0X �ֲ����֧�� 400KHz �� IIC ͨ������
// ��Ҫע����� VL53L0X �ֲ����֧�� 400KHz �� IIC ͨ������


//====================================================��� IIC ����====================================================
#define VL53L0X_SOFT_IIC_DELAY      (10 )                                        // ��� IIC ��ʱ����ʱ���� ��ֵԽС IIC ͨ������Խ��
#define VL53L0X_SCL_PIN             (P32) //1.5                                      // ��� IIC SCL ���� ���� VL53L0X �� SCL ����
#define VL53L0X_SDA_PIN             (P33) //1.4                                      // ��� IIC SDA ���� ���� VL53L0X �� SDA ����
//====================================================��� IIC ����====================================================


//#define VL53L0X_XSHUT_PIN           (P47)
#define VL53L0X_TIMEOUT_COUNT       (0x00FF)                                    // VL53L0X ��ʱ����

//================================================���� VL53L0X �ڲ���ַ================================================
#define VL53L0X_DEV_ADDR                                    (0x52 >> 1)         // 0b0101001

#define VL53L0X_SYSRANGE_START                              (0x00)

#define VL53L0X_SYSTEM_SEQUENCE_CONFIG                      (0x01)
#define VL53L0X_SYSTEM_INTERMEASUREMENT_PERIOD              (0x04)
#define VL53L0X_SYSTEM_RANGE_CONFIG                         (0x09)
#define VL53L0X_SYSTEM_INTERRUPT_GPIO_CONFIG                (0x0A)
#define VL53L0X_SYSTEM_INTERRUPT_CLEAR                      (0x0B)
#define VL53L0X_SYSTEM_THRESH_HIGH                          (0x0C)
#define VL53L0X_SYSTEM_THRESH_LOW                           (0x0E)
#define VL53L0X_SYSTEM_HISTOGRAM_BIN                        (0x81)

#define VL53L0X_RESULT_INTERRUPT_STATUS                     (0x13)
#define VL53L0X_RESULT_RANGE_STATUS                         (0x14)
#define VL53L0X_RESULT_PEAK_SIGNAL_RATE_REF                 (0xB6)
#define VL53L0X_RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN       (0xBC)
#define VL53L0X_RESULT_CORE_RANGING_TOTAL_EVENTS_RTN        (0xC0)
#define VL53L0X_RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF       (0xD0)
#define VL53L0X_RESULT_CORE_RANGING_TOTAL_EVENTS_REF        (0xD4)

#define VL53L0X_PRE_RANGE_CONFIG_MIN_SNR                    (0x27)
#define VL53L0X_PRE_RANGE_CONFIG_VCSEL_PERIOD               (0x50)
#define VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI          (0x51)
#define VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO          (0x52)
#define VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_LOW            (0x56)
#define VL53L0X_PRE_RANGE_CONFIG_VALID_PHASE_HIGH           (0x57)
#define VL53L0X_PRE_RANGE_CONFIG_SIGMA_THRESH_HI            (0x61)
#define VL53L0X_PRE_RANGE_CONFIG_SIGMA_THRESH_LO            (0x62)
#define VL53L0X_PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT          (0x64)

#define VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_LOW          (0x47)
#define VL53L0X_FINAL_RANGE_CONFIG_VALID_PHASE_HIGH         (0x48)
#define VL53L0X_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT (0x44)
#define VL53L0X_FINAL_RANGE_CONFIG_MIN_SNR                  (0x67)
#define VL53L0X_FINAL_RANGE_CONFIG_VCSEL_PERIOD             (0x70)
#define VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI        (0x71)
#define VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO        (0x72)

#define VL53L0X_GLOBAL_CONFIG_VCSEL_WIDTH                   (0x32)
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_0            (0xB0)
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_1            (0xB1)
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_2            (0xB2)
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_3            (0xB3)
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_4            (0xB4)
#define VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_5            (0xB5)
#define VL53L0X_GLOBAL_CONFIG_REF_EN_START_SELECT           (0xB6)

#define VL53L0X_ALGO_PART_TO_PART_RANGE_OFFSET_MM           (0x28)
#define VL53L0X_ALGO_PHASECAL_LIM                           (0x30)
#define VL53L0X_ALGO_PHASECAL_CONFIG_TIMEOUT                (0x30)

#define VL53L0X_HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT       (0x33)
#define VL53L0X_HISTOGRAM_CONFIG_READOUT_CTRL               (0x55)

#define VL53L0X_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD         (0x4E)
#define VL53L0X_DYNAMIC_SPAD_REF_EN_START_OFFSET            (0x4F)

#define VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP                  (0x46)
#define VL53L0X_MSRC_CONFIG                                 (0x60)

#define VL53L0X_IDENTIFICATION_MODEL_ID                     (0xC0)
#define VL53L0X_IDENTIFICATION_REVISION_ID                  (0xC2)

#define VL53L0X_CROSSTALK_COMPENSATION_PEAK_RATE_MCPS       (0x20)

#define VL53L0X_POWER_MANAGEMENT_GO1_POWER_FORCE            (0x80)

#define VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH                     (0x84)

#define VL53L0X_I2C_SLAVE_DEVICE_ADDRESS                    (0x8A)

#define VL53L0X_SOFT_RESET_GO2_SOFT_RESET_N                 (0xBF)

#define VL53L0X_OSC_CALIBRATE_VAL                           (0xF8)

#define VL53L0X_IO_VOLTAGE_CONFIG                           (0x89)              // IO ��ѹ���üĴ�����ַ Ĭ�� 1V8 ʹ���޸�Ϊ 2V8

//================================================���� VL53L0X �ڲ���ַ================================================

#define VL53L0X_MIN_TIMING_BUDGET                           (20000)

#define VL53L0X_GET_START_OVERHEAD                          (1910)
#define VL53L0X_SET_START_OVERHEAD                          (1320)
#define VL53L0X_END_OVERHEAD                                (960 )
#define VL53L0X_TCC_OVERHEAD                                (590 )
#define VL53L0X_DSS_OVERHEAD                                (690 )
#define VL53L0X_MSRC_OVERHEAD                               (660 )
#define VL53L0X_PRERANGE_OVERHEAD                           (660 )
#define VL53L0X_FINALlRANGE_OVERHEAD                        (550 )

typedef enum
{
    VL53L0X_VCSEL_PERIOD_PER_RANGE,
    VL53L0X_VCSEL_PERIOD_FINAL_RANGE,
}vl53l0x_vcsel_period_type_enum;

typedef struct
{
    uint8 tcc;
    uint8 msrc;
    uint8 dss;
    uint8 pre_range;
    uint8 final_range;
}vl53l0x_sequence_enables_step_struct;

typedef struct
{
    uint16 pre_range_vcsel_period_pclks;
    uint16 final_range_vcsel_period_pclks;

    uint16 msrc_dss_tcc_mclks;
    uint16 pre_range_mclks;
    uint16 final_range_mclks;
    uint32 msrc_dss_tcc_us;
    uint32 pre_range_us;
    uint32 final_range_us;
}vl53l0x_sequence_timeout_step_struct;

extern uint8 vl53l0x_finsh_flag;
extern uint16 vl53l0x_distance_mm;
void 	vl53l0x_get_distance (void);

uint8  vl53l0x_read_range   (void);

uint8  vl53l0x_init         (void);

#endif
