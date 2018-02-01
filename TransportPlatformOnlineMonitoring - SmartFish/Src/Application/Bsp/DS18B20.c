/**
 * @file DS18B20.c
 * @brief �¶ȴ�������д����
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.01.26
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "DS18B20.h"
#include "DriveDelay.h"

static void DS18B20_Rest(void);         //��λDS18B20
static uint8_t DS18B20_Read_Bit(void);  //��ȡһλ
static uint8_t DS18B20_Read_Byte(void); //��ȡһ���ֽ�
static void DS18B20_Write_Byte(uint8_t dat); //д��һ���ֽ�
static void DS18B20_Start(void);        //��ʼת��


/**
 * @fn DS18B20_Rest
 * @brief DS18B20��λ���³�ʼ��
 * @return 
 */
static void DS18B20_Rest(void)
{
    DS18B20_IO_OUT();       //����Ϊ���
    DS18B20_DQ_OUT = 0;     //����DQ
    DelayUs(750);           //��ʱ750us
    DS18B20_DQ_OUT = 1;     //DQ=1 
    DelayUs(10);            //��ʱ10us
}

/**
 * @fn DS18B20_Check
 * @brief DS18B20��������Ƿ�����
 * @return 0 ����
 *      ���� δ��⵽DS18B20�Ĵ���
 */
uint8_t DS18B20_Check(void)
{
  uint8_t result=0;
	
	DS18B20_Rest();             //�Ƚ��г�ʼ��
	DS18B20_IO_IN();    //����Ϊ����
  while(DS18B20_DQ_IN && (result < 200))
	{
		result++;
		DelayUs(1);
	}
	if(result >= 200)
  {
      return 1;   
  }
  result=0;
  while(!DS18B20_DQ_IN && (result < 240))
	{
		result++;
		DelayUs(1);
	}
	if(result >= 240)
  {
      return 1;	   
  }        
	return 0;
}

/**
 * @fn DS18B20_Init
 * @brief DS18B20��ʼ��
 * @return 
 */
uint8_t DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pin : PC11 */
    GPIO_InitStruct.Pin = GPIO_PIN_11;              //PC11
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     //�������
    GPIO_InitStruct.Pull = GPIO_PULLUP;             //����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   //����
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);         //��ʼ��

    return DS18B20_Check();     //����״̬
}

/**
 * @fn DS18B20_Read_Bit
 * @brief ��DS18B20��ȡһ��λ
 * @return 0 / 1
 */
static uint8_t DS18B20_Read_Bit(void)
{
    uint8_t data;
    DS18B20_IO_OUT();       //����Ϊ���
    DS18B20_DQ_OUT=0; 
    DelayUs(2);
    DS18B20_DQ_OUT=1; 
    DS18B20_IO_IN();        //����Ϊ����
    DelayUs(12);
    if(DS18B20_DQ_IN)
    {
        data = 1;
    }
    else 
    {
        data = 0;
    }
    DelayUs(50);
    return data;
}

/**
 * @fn DS18B20_Read_Byte
 * @brief ��DS18B20��ȡһ���ֽ�
 * @return ����������
 */
static uint8_t DS18B20_Read_Byte(void)
{        
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t dat = 0;
	for(i=1;i<=8;i++) 
	{
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }
    return dat;
}

/**
 * @fn DS18B20_Write_Byte
 * @brief ��DS18B20д��һ���ֽ�
 * @param Ҫд�������
 */
static void DS18B20_Write_Byte(uint8_t dat)
 {             
    uint8_t j = 0;
    uint8_t testb = 0;
    DS18B20_IO_OUT();     //����Ϊ���
    for (j = 1; j <= 8; j++) 
	{
        testb = dat&0x01;
        dat = dat>>1;
        if(testb)       // д1
        {
            DS18B20_DQ_OUT = 0;
            DelayUs(2);                            
            DS18B20_DQ_OUT = 1;
            DelayUs(60);             
        }
        else    //д0
        {
            DS18B20_DQ_OUT = 0;
            DelayUs(60);             
            DS18B20_DQ_OUT = 1;
            DelayUs(2);                          
        }
    }
}

/**
 * @fn DS18B20_Start
 * @brief ��ʼ�¶�ת��
 */
static void DS18B20_Start(void)
{   						               
    DS18B20_Rest();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xCC);   // skip rom
    DS18B20_Write_Byte(0x44);   // convert
}


/**
 * @fn DS18B20_Get_Temp
 * @brief ��ȡ�¶�ֵ
 * @return �¶�ֵ�������� ��0.1�棬��Χ��-55�� ���� 125�棩
 */
float DS18B20_Get_Temp(void)
{
    uint8_t tempL = 0;
    uint8_t tempH = 0;
    uint16_t tempValue = 0;
    float wendu = 0.0;
    DS18B20_Start();           //��ʼת��
    DS18B20_Rest();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xCC);   // ����ROMָ��
    DS18B20_Write_Byte(0xBE);   // �¶�ת��ָ��	    
    tempL = DS18B20_Read_Byte();     // LSB   
    tempH = DS18B20_Read_Byte();     // MSB   
    
    tempValue = tempH;		   
    tempValue <<= 8;			    //!< temp Ϊunsigned int �ͣ�������16λ��b������8λ������b������tempr�ĸ߰�λ�������˵Ͱ�λ��
    tempValue = tempValue | tempL;  //!< "|",�룬��1���ǡ�1����temp ��ʮ��λ���߰�λ��b��λ���ⲽ�������ǰ�a����temp�ĵͰ�λ����������temp �����¶ȣ�����û����Ĭ��Ϊ�����¶�û�и����¶�
   
    wendu = tempValue * 0.0625;     //!< �����¶�
    
    tempH >>= 4;
    if(tempH == 0x0F)
    {
        wendu -= 4095.5;
    }
    return wendu;
}

