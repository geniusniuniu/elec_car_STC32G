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
*                   VCC4                 5V ��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/


#include "zf_delay.h"
#include "TOF.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

#pragma warning disable = 183


uint8 vl53l0x_finsh_flag;
uint16 vl53l0x_distance_mm = 850;



#define GET_VL53L0X_SDA   		 	VL53L0X_SDA_PIN
#define VL53L0X_SDA_LOW()         	VL53L0X_SDA_PIN = 0		//IO������͵�ƽ
#define VL53L0X_SDA_HIGH()        	VL53L0X_SDA_PIN = 1		//IO������ߵ�ƽ

#define VL53L0X_SCL_LOW()          	VL53L0X_SCL_PIN = 0		//IO������͵�ƽ
#define VL53L0X_SCL_HIGH()         	VL53L0X_SCL_PIN = 1		//IO������ߵ�ƽ

#define ack 1      //��Ӧ��
#define no_ack 0   //��Ӧ��	

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ʱ
//  @return     void						
//  @since      v1.0
//  Sample usage:				���IICͨѶʧ�ܿ��Գ�������j��ֵ
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_simiic_delay(void)
{
    uint16 j=VL53L0X_SOFT_IIC_DELAY;   
	while(j--);
}

//�ڲ�ʹ�ã��û��������
static void vl53l0x_simiic_start(void)
{
	VL53L0X_SDA_HIGH();
	VL53L0X_SCL_HIGH();
	vl53l0x_simiic_delay();
	VL53L0X_SDA_LOW();
	vl53l0x_simiic_delay();
	VL53L0X_SCL_LOW();
}

//�ڲ�ʹ�ã��û��������
static void vl53l0x_simiic_stop(void)
{
	VL53L0X_SDA_LOW();
	VL53L0X_SCL_LOW();
	vl53l0x_simiic_delay();
	VL53L0X_SCL_HIGH();
	vl53l0x_simiic_delay();
	VL53L0X_SDA_HIGH();
	vl53l0x_simiic_delay();
}

//��Ӧ��(����ack:SDA=0��no_ack:SDA=0)
//�ڲ�ʹ�ã��û��������
static void vl53l0x_simiic_sendack(unsigned char ack_dat)
{
    VL53L0X_SCL_LOW();
	vl53l0x_simiic_delay();
	if(ack_dat) VL53L0X_SDA_LOW();
    else    	VL53L0X_SDA_HIGH();

    VL53L0X_SCL_HIGH();
    vl53l0x_simiic_delay();
    VL53L0X_SCL_LOW();
    vl53l0x_simiic_delay();
}


static int vl53l0x_sccb_waitack(void)
{
    VL53L0X_SCL_LOW();

	vl53l0x_simiic_delay();
	
	VL53L0X_SCL_HIGH();
    vl53l0x_simiic_delay();
	
    if(GET_VL53L0X_SDA)           //Ӧ��Ϊ�ߵ�ƽ���쳣��ͨ��ʧ��
    {

        VL53L0X_SCL_LOW();
        return 0;
    }

    VL53L0X_SCL_LOW();
	vl53l0x_simiic_delay();
    return 1;
}

//�ֽڷ��ͳ���
//����c(����������Ҳ���ǵ�ַ)���������մ�Ӧ��
//�����Ǵ�Ӧ��λ
//�ڲ�ʹ�ã��û��������
static void vl53l0x_send_ch(uint8 c)
{
	uint8 i = 8;
    while(i--)
    {
        if(c & 0x80)	VL53L0X_SDA_HIGH();//SDA �������
        else			VL53L0X_SDA_LOW();
        c <<= 1;
        vl53l0x_simiic_delay();
        VL53L0X_SCL_HIGH();                //SCL ���ߣ��ɼ��ź�
        vl53l0x_simiic_delay();
        VL53L0X_SCL_LOW();                //SCL ʱ��������
    }
	vl53l0x_sccb_waitack();
}


//�ֽڽ��ճ���
//�����������������ݣ��˳���Ӧ���|��Ӧ����|ʹ��
//�ڲ�ʹ�ã��û��������
static uint8 vl53l0x_read_ch(uint8 ack_x)
{
    uint8 i;
    uint8 c;
    c=0;
    VL53L0X_SCL_LOW();
    vl53l0x_simiic_delay();
    VL53L0X_SDA_HIGH();             

    for(i=0;i<8;i++)
    {
        vl53l0x_simiic_delay();
        VL53L0X_SCL_LOW();         //��ʱ����Ϊ�ͣ�׼����������λ
        vl53l0x_simiic_delay();
        VL53L0X_SCL_HIGH();         //��ʱ����Ϊ�ߣ�ʹ��������������Ч
        vl53l0x_simiic_delay();
        c<<=1;
        if(GET_VL53L0X_SDA) 
        {
            c+=1;   //������λ�������յ����ݴ�c
        }
    }

	VL53L0X_SCL_LOW();
	vl53l0x_simiic_delay();
	vl53l0x_simiic_sendack(ack_x);
	
    return c;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IICд���ݵ��豸�Ĵ�������
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat				д�������
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_simiic_write_dats(uint8 dev_add, uint8 *dat, uint32 len)
{
	vl53l0x_simiic_start();
    vl53l0x_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	while(len--)
	{
		vl53l0x_send_ch( *dat++ );   				 //������Ҫд�������
	}

	
	vl53l0x_simiic_stop();
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IICд���ݵ��豸�Ĵ�������
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat				д�������
//  @return     void						
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat)
{
	vl53l0x_simiic_start();
    vl53l0x_send_ch( (dev_add<<1) | 0x00);   //����������ַ��дλ
	vl53l0x_send_ch( reg );   				 //���ʹӻ��Ĵ�����ַ
	vl53l0x_send_ch( dat );   				 //������Ҫд�������
	vl53l0x_simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC���豸�Ĵ�����ȡ����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static uint8 vl53l0x_simiic_read_reg(uint8 dev_add, uint8 reg)
{
	uint8 dat;
	vl53l0x_simiic_start();
    vl53l0x_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	vl53l0x_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ

	
	vl53l0x_simiic_start();
	vl53l0x_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
	dat = vl53l0x_read_ch(no_ack);   				//��ȡ����
	vl53l0x_simiic_stop();
	
	return dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ȡ���ֽ�����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat_add			���ݱ���ĵ�ַָ��
//  @param      num				��ȡ�ֽ�����
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_simiic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint32 num)
{
	vl53l0x_simiic_start();
    vl53l0x_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	vl53l0x_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ

	
	vl53l0x_simiic_start();
	vl53l0x_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = vl53l0x_read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = vl53l0x_read_ch(no_ack); //��ȡ����
	vl53l0x_simiic_stop();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ��IIC��ȡ���ֽ�����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat_add			���ݱ���ĵ�ַָ��
//  @param      num				��ȡ�ֽ�����
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������			
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_simiic_read_regs_1(uint8 dev_add, uint8 reg, uint8 *dat_add, uint32 num)
{
	vl53l0x_simiic_start();
    vl53l0x_send_ch( (dev_add<<1) | 0x00);  //����������ַ��дλ
	vl53l0x_send_ch( reg );   				//���ʹӻ��Ĵ�����ַ

	
	vl53l0x_simiic_start();
	vl53l0x_send_ch( (dev_add<<1) | 0x01);  //����������ַ�Ӷ�λ
    while(--num)
    {
        *dat_add = vl53l0x_read_ch(ack); //��ȡ����
        dat_add++;
    }
    *dat_add = vl53l0x_read_ch(no_ack); //��ȡ����
	vl53l0x_simiic_stop();
}


#define vl53l0x_write_array(dat, len)          (vl53l0x_simiic_write_dats(VL53L0X_DEV_ADDR, (dat), (len)))
#define vl53l0x_write_register(reg, dat)       (vl53l0x_simiic_write_reg(VL53L0X_DEV_ADDR, (reg), (dat)))
#define vl53l0x_read_register(reg)             (vl53l0x_simiic_read_reg (VL53L0X_DEV_ADDR, (reg)))
#define vl53l0x_read_registers(reg, dat, len)  (vl53l0x_simiic_read_regs(VL53L0X_DEV_ADDR, (reg), (dat), (len)))
#define vl53l0x_read_registers_1(reg, dat, len)  (vl53l0x_simiic_read_regs_1(VL53L0X_DEV_ADDR, (reg), (dat), (len)))

// ������ʱ�ʾ��Ŀ�귴�䲢���豸��⵽���źŵ����
// ���ô����ƿ���ȷ��������������Ч�����������С����ֵ
// ����һ���ϵ͵����ƿ������Ӵ������Ĳ�����Χ
// ���ƺ�Ҳ������ <��������Ŀ�����������Ĳ���Ҫ�ķ��䵼��> �õ���׼ȷ�����Ŀ�����
// Ĭ��Ϊ 0.25 MCPS ��Ԥ�跶ΧΪ 0 - 511.99
#define VL53L0X_DEFAULT_RATE_LIMIT  (0.25)

// �ӼĴ������ݽ��� PCLKs �� VCSEL (vertical cavity surface emitting laser) ����������
#define decode_vcsel_period(reg_val)            (((reg_val) + 1) << 1)

// �� PCLK �е� VCSEL ���ڼ�������� (�� *����Ϊ��λ)
// PLL_period_ps = 1655
// macro_period_vclks = 2304
#define calc_macro_period(vcsel_period_pclks)   ((((uint32)2304 * (vcsel_period_pclks) * 1655) + 500) / 1000)

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ�豸 SPAD ��Ϣ
// ����˵��     index           ����
// ����˵��     type            ����ֵ
// ���ز���     uint8           �Ƿ�ɹ� 0-�ɹ� 1-ʧ��
// ʹ��ʾ��     vl53l0x_get_spad_info(index, type_is_aperture);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
static uint8 vl53l0x_get_spad_info (uint8 *index, uint8 *type_is_aperture)
{
    uint8 tmp = 0;
    uint8 return_state = 0;
    volatile uint16 loop_count = 0;

    do
    {
        vl53l0x_write_register(0x80, 0x01);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x00, 0x00);

        vl53l0x_write_register(0xFF, 0x06);
        vl53l0x_read_registers(0x83, &tmp, 1);
        vl53l0x_write_register(0x83, tmp | 0x04);
        vl53l0x_write_register(0xFF, 0x07);
        vl53l0x_write_register(0x81, 0x01);

        vl53l0x_write_register(0x80, 0x01);

        vl53l0x_write_register(0x94, 0x6b);
        vl53l0x_write_register(0x83, 0x00);

        tmp = 0x00;
        while(tmp != 0x10)
        {
            vl53l0x_read_registers(0x83, &tmp, 1);
            loop_count++;
            if(loop_count == VL53L0X_TIMEOUT_COUNT)
            {
                return_state = 1;
                break;
            }
        }
		
        if(return_state)
        {
            break;
        }
        vl53l0x_write_register(0x83, 0x01);
        vl53l0x_read_registers(0x92, &tmp, 1);

        *index = tmp & 0x7f;
        *type_is_aperture = (tmp >> 7) & 0x01;

        vl53l0x_write_register(0x81, 0x00);
        vl53l0x_write_register(0xFF, 0x06);
        vl53l0x_read_registers(0x83, &tmp, 1);
        vl53l0x_write_register(0x83, tmp);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x00, 0x01);

        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x80, 0x00);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ʱ��ֵ�� MCLKs ת������Ӧ�� ms
// ����˵��     timeout_period_mclks    ��ʱ���� MCLKs
// ����˵��     vcsel_period_pclks      PCLK ֵ
// ���ز���     uint32                  ���س�ʱ��ֵ
// ʹ��ʾ��     vl53l0x_timeout_mclks_to_microseconds(timeout_period_mclks, vcsel_period_pclks);
// ��ע��Ϣ     �����в��賬ʱ�Ӿ��и��� VCSEL ���ڵ� MCLK (�� PCLK Ϊ��λ)ת��Ϊ΢��
//-------------------------------------------------------------------------------------------------------------------
static uint32 vl53l0x_timeout_mclks_to_microseconds (uint16 timeout_period_mclks, uint8 vcsel_period_pclks)
{
    uint32 macro_period_ns = calc_macro_period(vcsel_period_pclks);

    return ((timeout_period_mclks * macro_period_ns) + (macro_period_ns / 2)) / 1000;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ʱ��ֵ�� ms ת������Ӧ�� MCLKs
// ����˵��     timeout_period_us   ��ʱ���� ΢�뵥λ
// ����˵��     vcsel_period_pclks  PCLK ֵ
// ���ز���     uint32              ���س�ʱ��ֵ
// ʹ��ʾ��     vl53l0x_timeout_microseconds_to_mclks(timeout_period_us, vcsel_period_pclks);
// ��ע��Ϣ     �����в��賬ʱ��΢��ת��Ϊ���и��� VCSEL ���ڵ� MCLK (�� PCLK Ϊ��λ)
//-------------------------------------------------------------------------------------------------------------------
static uint32 vl53l0x_timeout_microseconds_to_mclks (uint32 timeout_period_us, uint8 vcsel_period_pclks)
{
    uint32 macro_period_ns = calc_macro_period(vcsel_period_pclks);

    return (((timeout_period_us * 1000) + (macro_period_ns / 2)) / macro_period_ns);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �Գ�ʱ��ֵ���н���
// ����˵��     reg_val         ��ʱʱ�� �Ĵ���ֵ
// ���ز���     uint16          ���س�ʱ��ֵ
// ʹ��ʾ��     vl53l0x_decode_timeout(reg_val);
// ��ע��Ϣ     �ӼĴ���ֵ���� MCLK �е����в��賬ʱ   
//-------------------------------------------------------------------------------------------------------------------
static uint16 vl53l0x_decode_timeout (uint16 reg_val)
{
  // ��ʽ: (LSByte * 2 ^ MSByte) + 1
    return  (uint16)((reg_val & 0x00FF) <<
            (uint16)((reg_val & 0xFF00) >> 8)) + 1;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �Գ�ʱ��ֵ���б���
// ����˵��     timeout_mclks   ��ʱʱ�� -MCLKs ֵ
// ���ز���     uint16          ���ر���ֵ
// ʹ��ʾ��     vl53l0x_encode_timeout(timeout_mclks);
// ��ע��Ϣ     �� MCLK �жԳ�ʱ�����в��賬ʱ�Ĵ���ֵ���б���
//-------------------------------------------------------------------------------------------------------------------
static uint16 vl53l0x_encode_timeout (uint16 timeout_mclks)
{
    uint32 ls_byte = 0;
    uint16 ms_byte = 0;
    uint16 return_data = 0;

    if (timeout_mclks > 0)
    {
        // ��ʽ: (LSByte * 2 ^ MSByte) + 1
        ls_byte = timeout_mclks - 1;
        while ((ls_byte & 0xFFFFFF00) > 0)
        {
            ls_byte >>= 1;
            ms_byte++;
        }
        return_data = (ms_byte << 8) | (ls_byte & 0xFF);
    }
    return return_data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ���в���ʹ������
// ����˵��     enables         ����ʹ�ܲ���ṹ��
// ���ز���     void
// ʹ��ʾ��     vl53l0x_get_sequence_step_enables(enables);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_get_sequence_step_enables(vl53l0x_sequence_enables_step_struct *enables)
{
    uint8 sequence_config = 0;
    vl53l0x_read_registers(VL53L0X_SYSTEM_SEQUENCE_CONFIG, &sequence_config, 1);

    enables->tcc          = (sequence_config >> 4) & 0x1;
    enables->dss          = (sequence_config >> 3) & 0x1;
    enables->msrc         = (sequence_config >> 2) & 0x1;
    enables->pre_range    = (sequence_config >> 6) & 0x1;
    enables->final_range  = (sequence_config >> 7) & 0x1;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ��������
// ����˵��     type            Ԥ��������
// ���ز���     uint8           ���ص�����ֵ
// ʹ��ʾ��     vl53l0x_get_vcsel_pulse_period(VL53L0X_VCSEL_PERIOD_PER_RANGE);
// ��ע��Ϣ     �� PCLKs �л�ȡ�����������͵� VCSEL ��������
//-------------------------------------------------------------------------------------------------------------------
static uint8 vl53l0x_get_vcsel_pulse_period (vl53l0x_vcsel_period_type_enum type)
{
    uint8 data_buffer = 0;
    if (type == VL53L0X_VCSEL_PERIOD_PER_RANGE)
    {
        vl53l0x_read_registers(VL53L0X_PRE_RANGE_CONFIG_VCSEL_PERIOD, &data_buffer, 1);
        data_buffer = decode_vcsel_period(data_buffer);
    }
    else if (type == VL53L0X_VCSEL_PERIOD_FINAL_RANGE)
    {
        vl53l0x_read_registers(VL53L0X_FINAL_RANGE_CONFIG_VCSEL_PERIOD, &data_buffer, 1);
        data_buffer = decode_vcsel_period(data_buffer);
    }
    else
    {
        data_buffer = 255;
    }
    return data_buffer;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ���в��賬ʱ����
// ����˵��     enables         ����ʹ�ܲ���ṹ��
// ����˵��     timeouts        ���г�ʱ����ṹ��
// ���ز���     void
// ʹ��ʾ��     vl53l0x_get_sequence_step_timeouts(enables, timeouts);
// ��ע��Ϣ     ��ȡ���г�ʱ��������������ĳ�ʱ ���һ��洢�м�ֵ
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_get_sequence_step_timeouts (vl53l0x_sequence_enables_step_struct const *enables, vl53l0x_sequence_timeout_step_struct *timeouts)
{
    uint8 reg_buffer[2];
    uint16 reg16_buffer = 0;

    timeouts->pre_range_vcsel_period_pclks = vl53l0x_get_vcsel_pulse_period(VL53L0X_VCSEL_PERIOD_PER_RANGE);

    vl53l0x_read_registers(VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP, reg_buffer, 1);
    timeouts->msrc_dss_tcc_mclks = reg_buffer[0] + 1;
    timeouts->msrc_dss_tcc_us = vl53l0x_timeout_mclks_to_microseconds(timeouts->msrc_dss_tcc_mclks, (uint8)timeouts->pre_range_vcsel_period_pclks);

    vl53l0x_read_registers(VL53L0X_PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI, reg_buffer, 2);
    reg16_buffer = ((uint16) reg_buffer[0] << 8) | reg_buffer[1];
    timeouts->pre_range_mclks = vl53l0x_decode_timeout(reg16_buffer);
    timeouts->pre_range_us = vl53l0x_timeout_mclks_to_microseconds(timeouts->pre_range_mclks, (uint8)timeouts->pre_range_vcsel_period_pclks);

    timeouts->final_range_vcsel_period_pclks = vl53l0x_get_vcsel_pulse_period(VL53L0X_VCSEL_PERIOD_FINAL_RANGE);

    vl53l0x_read_registers(VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI, reg_buffer, 2);
    reg16_buffer = ((uint16) reg_buffer[0] << 8) | reg_buffer[1];
    timeouts->final_range_mclks = vl53l0x_decode_timeout(reg16_buffer);

    if (enables->pre_range)
    {
        timeouts->final_range_mclks -= timeouts->pre_range_mclks;
    }

    timeouts->final_range_us = vl53l0x_timeout_mclks_to_microseconds(timeouts->final_range_mclks, (uint8)timeouts->final_range_vcsel_period_pclks);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ִ�е��βο�У׼
// ����˵��     vhv_init_byte   Ԥ��У׼ֵ
// ���ز���     uint8           �����Ƿ�ɹ� 0-�ɹ� 1-ʧ��
// ʹ��ʾ��     vl53l0x_get_vcsel_pulse_period(VL53L0X_VCSEL_PERIOD_PER_RANGE);
// ��ע��Ϣ     �� PCLKs �л�ȡ�����������͵� VCSEL ��������
//-------------------------------------------------------------------------------------------------------------------
static uint8 vl53l0x_perform_single_ref_calibration (uint8 vhv_init_byte)
{
    uint8 return_state = 0;
    uint8 data_buffer = 0;
    volatile uint16 loop_count = 0;
    do
    {
        vl53l0x_write_register(VL53L0X_SYSRANGE_START, 0x01 | vhv_init_byte);
        vl53l0x_read_registers(VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP, &data_buffer, 1);
        while ((data_buffer & 0x07) == 0)
        {
            if (loop_count > 0x8fe0)
            {
                return_state = 1;
                break;
            }
            if (loop_count++ % 0x10 == 0)
            {
                vl53l0x_read_registers(VL53L0X_MSRC_CONFIG_TIMEOUT_MACROP, &data_buffer, 1);
            }
        }
        if(return_state)
        {
            break;
        }
        vl53l0x_write_register(VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);
        vl53l0x_write_register(VL53L0X_SYSRANGE_START, 0x00);
    }while(0);

    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���ò�����ʱԤ�� (��΢��Ϊ��λ)
// ����˵��     budget_us       �趨�Ĳ��������ʱ��
// ���ز���     uint8           ������� 0-�ɹ� 1-ʧ��
// ʹ��ʾ��     vl53l0x_set_measurement_timing_budget(measurement_timing_budget_us);
// ��ע��Ϣ     ����һ�β��������ʱ��
//              ���ڲ�����е��Ӳ���֮�����ʱ��Ԥ��
//              ������ʱ��Ԥ���������ȷ�Ĳ���
//              ����һ��N����Ԥ����Լ���һ��sqrt(N)���ķ�Χ������׼ƫ��
//              Ĭ��Ϊ33���� ��СֵΪ20 ms
//-------------------------------------------------------------------------------------------------------------------
static uint8 vl53l0x_set_measurement_timing_budget (uint32 budget_us)
{
    uint8 return_state = 0;
    uint8 data_buffer[3];
    uint16 dat = 0;
	uint32 used_budget_us;
	uint32 final_range_timeout_us;
	uint16 final_range_timeout_mclks;
	
    vl53l0x_sequence_enables_step_struct enables;
    vl53l0x_sequence_timeout_step_struct timeouts;

    do
    {
        if (budget_us < VL53L0X_MIN_TIMING_BUDGET)
        {
            return_state = 1;
            break;
        }

        used_budget_us = VL53L0X_SET_START_OVERHEAD + VL53L0X_END_OVERHEAD;
        vl53l0x_get_sequence_step_enables(&enables);
        vl53l0x_get_sequence_step_timeouts(&enables, &timeouts);

        if (enables.tcc)
        {
            used_budget_us += (timeouts.msrc_dss_tcc_us + VL53L0X_TCC_OVERHEAD);
        }

        if (enables.dss)
        {
            used_budget_us += 2 * (timeouts.msrc_dss_tcc_us + VL53L0X_DSS_OVERHEAD);
        }
        else if (enables.msrc)
        {
            used_budget_us += (timeouts.msrc_dss_tcc_us + VL53L0X_MSRC_OVERHEAD);
        }

        if (enables.pre_range)
        {
            used_budget_us += (timeouts.pre_range_us + VL53L0X_PRERANGE_OVERHEAD);
        }

        if (enables.final_range)
        {
            // ��ע�� ���շ�Χ��ʱ�ɼ�ʱԤ�������������������ʱ���ܺ;���
            // ���û�пռ��������շ�Χ��ʱ �����ô���
            // ���� ʣ��ʱ�佫Ӧ�������շ�Χ
            used_budget_us += VL53L0X_FINALlRANGE_OVERHEAD;
            if (used_budget_us > budget_us)
            {
                // ����ĳ�ʱ̫��
                return_state = 1;
                break;
            }

            // �������ճ�ʱ��Χ �������Ԥ���̷�Χ��ʱ
            // Ϊ�� ���ճ�ʱ��Ԥ���̳�ʱ�����Ժ����� MClks ��ʾ
            // ��Ϊ���Ǿ��в�ͬ�� VCSEL ����
            final_range_timeout_us = budget_us - used_budget_us;
            final_range_timeout_mclks =
            vl53l0x_timeout_microseconds_to_mclks(final_range_timeout_us,
                     (uint8)timeouts.final_range_vcsel_period_pclks);

            if (enables.pre_range)
            {
                final_range_timeout_mclks += timeouts.pre_range_mclks;
            }

            dat = vl53l0x_encode_timeout(final_range_timeout_mclks);
            data_buffer[0] = VL53L0X_FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI;
            data_buffer[1] = ((dat >> 8) & 0xFF);
            data_buffer[2] = (dat & 0xFF);
            vl53l0x_write_array(data_buffer, 3);
        }
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ������ʱԤ�� (��΢��Ϊ��λ)
// ����˵��     void
// ���ز���     uint32          ���趨�Ĳ��������ʱ��
// ʹ��ʾ��     vl53l0x_get_measurement_timing_budget();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
static uint32 vl53l0x_get_measurement_timing_budget (void)
{
    vl53l0x_sequence_enables_step_struct enables;
    vl53l0x_sequence_timeout_step_struct timeouts;

    // ��ʼ�ͽ�������ʱ��ʼ�մ���
    uint32 budget_us = VL53L0X_GET_START_OVERHEAD + VL53L0X_END_OVERHEAD;

    vl53l0x_get_sequence_step_enables(&enables);
    vl53l0x_get_sequence_step_timeouts(&enables, &timeouts);

    if (enables.tcc)
    {
        budget_us += (timeouts.msrc_dss_tcc_us + VL53L0X_TCC_OVERHEAD);
    }

    if (enables.dss)
    {
        budget_us += 2 * (timeouts.msrc_dss_tcc_us + VL53L0X_DSS_OVERHEAD);
    }
    else if (enables.msrc)
    {
        budget_us += (timeouts.msrc_dss_tcc_us + VL53L0X_MSRC_OVERHEAD);
    }

    if (enables.pre_range)
    {
        budget_us += (timeouts.pre_range_us + VL53L0X_PRERANGE_OVERHEAD);
    }

    if (enables.final_range)
    {
        budget_us += (timeouts.final_range_us + VL53L0X_FINALlRANGE_OVERHEAD);
    }

    return budget_us;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ���÷����ź��������� ��ֵ��λΪ MCPS (�����ÿ��)
// ����˵��     limit_mcps      ���õ���С����
// ���ز���     void
// ʹ��ʾ��     vl53l0x_set_signal_rate_limit(0.25);
// ��ע��Ϣ     ������ʱ�ʾ��Ŀ�귴�䲢���豸��⵽���źŵ����
//              ���ô����ƿ���ȷ��������������Ч�����������С����ֵ
//              ����һ���ϵ͵����ƿ������Ӵ������Ĳ�����Χ
//              ���ƺ�Ҳ������ <��������Ŀ�����������Ĳ���Ҫ�ķ��䵼��> �õ���׼ȷ�����Ŀ�����
//              Ĭ��Ϊ 0.25 MCPS ��Ԥ�跶ΧΪ 0 - 511.99
//-------------------------------------------------------------------------------------------------------------------
static void vl53l0x_set_signal_rate_limit (float limit_mcps)
{
	uint8 data_buffer[3];
    uint16 limit_mcps_16bit = (limit_mcps * (1 << 7));
    //zf_assert(limit_mcps >= 0 || limit_mcps <= 511.99);


    data_buffer[0] = VL53L0X_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT;
    data_buffer[1] = ((limit_mcps_16bit >> 8) & 0xFF);
    data_buffer[2] = (limit_mcps_16bit & 0xFF);

    vl53l0x_write_array(data_buffer, 3);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����Ժ���Ϊ��λ�ķ�Χ����
// ����˵��     void
// ���ز���     uint8           0-������Ч 1-������Ч
// ʹ��ʾ��     vl53l0x_get_distance();
// ��ע��Ϣ     �ڿ�ʼ������̲�����Ҳ���ô˺���
//-------------------------------------------------------------------------------------------------------------------
#define FILTER_THRESHOLD 800 // �����޷���ֵ��������Ҫ����

void vl53l0x_get_distance(void)
{
    uint8 reg_databuffer[3];
    uint16 distance;
    vl53l0x_read_registers_1(VL53L0X_RESULT_INTERRUPT_STATUS, reg_databuffer, 1);
    if ((reg_databuffer[0] & 0x07) == 0)
    {
        vl53l0x_finsh_flag = 0;
    }
    else
    {
        // �������Զ�У������ΪĬ��ֵ 1000 ��δ���÷�����Χ
        vl53l0x_read_registers_1(VL53L0X_RESULT_RANGE_STATUS + 10, reg_databuffer, 2);
        distance = ((uint16)reg_databuffer[0] << 8) | reg_databuffer[1];
        vl53l0x_finsh_flag = 1;
        // ���ƾ���ֵ������1100
        if (distance > 1100)
            distance = 1100;
        // ����˲�
       if(abs(distance - vl53l0x_distance_mm) < FILTER_THRESHOLD)   //������Ч
           vl53l0x_distance_mm = distance;
       
    }
}



//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� VL53L0X
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     vl53l0x_init();
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
uint8 vl53l0x_init (void)
{
    uint32 measurement_timing_budget_us = 0;
    uint8 stop_variable = 0;
    uint8 return_state = 0;
    uint8 reg_data_buffer = 0;
    uint8 ref_spad_map[6];
    uint8 data_buffer[7];
	uint8 i = 0;
	
    memset(ref_spad_map, 0, 6);
    memset(data_buffer, 0, 7);



    do
    {
//        delay_ms(100);
//        VL53L0X_XSHUT_PIN = 0;
//        delay_ms(50);
//        VL53L0X_XSHUT_PIN = 1;
//        delay_ms(100);

        // -------------------------------- VL53L0X ������ʼ�� --------------------------------
        reg_data_buffer = vl53l0x_read_register(VL53L0X_IO_VOLTAGE_CONFIG);         // ������Ĭ�� IO Ϊ 1.8V ģʽ
        vl53l0x_write_register(VL53L0X_IO_VOLTAGE_CONFIG, reg_data_buffer | 0x01);  // ���� IO Ϊ 2.8V ģʽ

        vl53l0x_write_register(0x88, 0x00);                                         // ����Ϊ��׼ IIC ģʽ

        vl53l0x_write_register(0x80, 0x01);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x00, 0x00);

        vl53l0x_read_registers(0x91, &stop_variable , 1);

        vl53l0x_write_register(0x00, 0x01);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x80, 0x00);

        // ���� SIGNAL_RATE_MSRC(bit1) �� SIGNAL_RATE_PRE_RANGE(bit4) ���Ƽ��
        reg_data_buffer = vl53l0x_read_register(VL53L0X_MSRC_CONFIG);
        vl53l0x_write_register(VL53L0X_MSRC_CONFIG, reg_data_buffer | 0x12);

        vl53l0x_set_signal_rate_limit(VL53L0X_DEFAULT_RATE_LIMIT);                  // �����ź���������
        vl53l0x_write_register(VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0xFF);
        // -------------------------------- VL53L0X ������ʼ�� --------------------------------

        // -------------------------------- VL53L0X ���ó�ʼ�� --------------------------------
        if (vl53l0x_get_spad_info(&data_buffer[0], &data_buffer[1]))
        {
			return_state = 1;
            // �������������˶�����Ϣ ������ʾ����λ��������
            // ��ô���� vl53l0x �Լ������ʱ�˳���
            // ���һ�½�����û������ ���û������ܾ��ǻ���

			printf("vl53l0x init error.\r\n");
			break;
        }

        // �� GLOBAL_CONFIG_SPAD_ENABLES_REF_[0-6] ��ȡ SPAD map (RefGoodSpadMap) ����
        vl53l0x_read_registers(VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_0, ref_spad_map, 6);

        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(VL53L0X_DYNAMIC_SPAD_REF_EN_START_OFFSET, 0x00);
        vl53l0x_write_register(VL53L0X_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD, 0x2C);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(VL53L0X_GLOBAL_CONFIG_REF_EN_START_SELECT, 0xB4);

        data_buffer[2] = data_buffer[1] ? 12 : 0; // 12 is the first aperture spad
        for (i = 0; i < 48; i++)
        {
            if (i < data_buffer[2] || data_buffer[3] == data_buffer[0])
            {
                // ��λ����Ӧ���õĵ�һ��λ
                // ���� (eference_spad_count) λ������
                // ��˴�λΪ��
                ref_spad_map[i / 8] &= ~(1 << (i % 8));
            }
            else if ((ref_spad_map[i / 8] >> (i % 8)) & 0x1)
            {
                data_buffer[3]++;
            }
        }

        data_buffer[0] = VL53L0X_GLOBAL_CONFIG_SPAD_ENABLES_REF_0;
        for(i = 1; i < 7; i++)
        {
            data_buffer[1] = ref_spad_map[i-1];
        }
        vl53l0x_write_array(data_buffer, 7);

        // Ĭ��ת������ version 02/11/2015_v36
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x00, 0x00);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x09, 0x00);
        vl53l0x_write_register(0x10, 0x00);
        vl53l0x_write_register(0x11, 0x00);
        vl53l0x_write_register(0x24, 0x01);
        vl53l0x_write_register(0x25, 0xFF);
        vl53l0x_write_register(0x75, 0x00);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x4E, 0x2C);
        vl53l0x_write_register(0x48, 0x00);
        vl53l0x_write_register(0x30, 0x20);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x30, 0x09);
        vl53l0x_write_register(0x54, 0x00);
        vl53l0x_write_register(0x31, 0x04);
        vl53l0x_write_register(0x32, 0x03);
        vl53l0x_write_register(0x40, 0x83);
        vl53l0x_write_register(0x46, 0x25);
        vl53l0x_write_register(0x60, 0x00);
        vl53l0x_write_register(0x27, 0x00);
        vl53l0x_write_register(0x50, 0x06);
        vl53l0x_write_register(0x51, 0x00);
        vl53l0x_write_register(0x52, 0x96);
        vl53l0x_write_register(0x56, 0x08);
        vl53l0x_write_register(0x57, 0x30);
        vl53l0x_write_register(0x61, 0x00);
        vl53l0x_write_register(0x62, 0x00);
        vl53l0x_write_register(0x64, 0x00);
        vl53l0x_write_register(0x65, 0x00);
        vl53l0x_write_register(0x66, 0xA0);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x22, 0x32);
        vl53l0x_write_register(0x47, 0x14);
        vl53l0x_write_register(0x49, 0xFF);
        vl53l0x_write_register(0x4A, 0x00);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x7A, 0x0A);
        vl53l0x_write_register(0x7B, 0x00);
        vl53l0x_write_register(0x78, 0x21);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x23, 0x34);
        vl53l0x_write_register(0x42, 0x00);
        vl53l0x_write_register(0x44, 0xFF);
        vl53l0x_write_register(0x45, 0x26);
        vl53l0x_write_register(0x46, 0x05);
        vl53l0x_write_register(0x40, 0x40);
        vl53l0x_write_register(0x0E, 0x06);
        vl53l0x_write_register(0x20, 0x1A);
        vl53l0x_write_register(0x43, 0x40);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x34, 0x03);
        vl53l0x_write_register(0x35, 0x44);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x31, 0x04);
        vl53l0x_write_register(0x4B, 0x09);
        vl53l0x_write_register(0x4C, 0x05);
        vl53l0x_write_register(0x4D, 0x04);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x44, 0x00);
        vl53l0x_write_register(0x45, 0x20);
        vl53l0x_write_register(0x47, 0x08);
        vl53l0x_write_register(0x48, 0x28);
        vl53l0x_write_register(0x67, 0x00);
        vl53l0x_write_register(0x70, 0x04);
        vl53l0x_write_register(0x71, 0x01);
        vl53l0x_write_register(0x72, 0xFE);
        vl53l0x_write_register(0x76, 0x00);
        vl53l0x_write_register(0x77, 0x00);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x0D, 0x01);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x80, 0x01);
        vl53l0x_write_register(0x01, 0xF8);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x8E, 0x01);
        vl53l0x_write_register(0x00, 0x01);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x80, 0x00);

        // ���ж���������Ϊ����Ʒ����
        vl53l0x_write_register(VL53L0X_SYSTEM_INTERRUPT_GPIO_CONFIG, 0x04);
        reg_data_buffer = vl53l0x_read_register(VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH);
        vl53l0x_write_register(VL53L0X_GPIO_HV_MUX_ACTIVE_HIGH, reg_data_buffer & ~0x10);
        vl53l0x_write_register(VL53L0X_SYSTEM_INTERRUPT_CLEAR, 0x01);

        measurement_timing_budget_us  = vl53l0x_get_measurement_timing_budget();

        // Ĭ������½��� MSRC �� TCC
        // MSRC = Minimum Signal Rate Check
        // TCC = Target CentreCheck
        vl53l0x_write_register(VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0xE8);
        vl53l0x_set_measurement_timing_budget(measurement_timing_budget_us);    // ���¼���ʱ��Ԥ��
        // -------------------------------- VL53L0X ���ó�ʼ�� --------------------------------

        vl53l0x_write_register(VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0x01);
        if (vl53l0x_perform_single_ref_calibration(0x40))
        {
            return_state = 1;
            break;
        }
        vl53l0x_write_register(VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0x02);
        if (vl53l0x_perform_single_ref_calibration(0x00))
        {
            return_state = 1;
            break;
        }
        vl53l0x_write_register(VL53L0X_SYSTEM_SEQUENCE_CONFIG, 0xE8);           // �ָ���ǰ����������

        delay_ms(100);

        vl53l0x_write_register(0x80, 0x01);
        vl53l0x_write_register(0xFF, 0x01);
        vl53l0x_write_register(0x00, 0x00);
        vl53l0x_write_register(0x91, stop_variable);
        vl53l0x_write_register(0x00, 0x01);
        vl53l0x_write_register(0xFF, 0x00);
        vl53l0x_write_register(0x80, 0x00);

        vl53l0x_write_register(VL53L0X_SYSRANGE_START, 0x02);
    }while(0);

    return return_state;
}

