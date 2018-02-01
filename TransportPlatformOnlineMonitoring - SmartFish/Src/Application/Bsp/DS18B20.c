/**
 * @file DS18B20.c
 * @brief 温度传感器读写操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.01.26
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "DS18B20.h"
#include "DriveDelay.h"

static void DS18B20_Rest(void);         //复位DS18B20
static uint8_t DS18B20_Read_Bit(void);  //读取一位
static uint8_t DS18B20_Read_Byte(void); //读取一个字节
static void DS18B20_Write_Byte(uint8_t dat); //写入一个字节
static void DS18B20_Start(void);        //开始转换


/**
 * @fn DS18B20_Rest
 * @brief DS18B20复位重新初始化
 * @return 
 */
static void DS18B20_Rest(void)
{
    DS18B20_IO_OUT();       //设置为输出
    DS18B20_DQ_OUT = 0;     //拉低DQ
    DelayUs(750);           //延时750us
    DS18B20_DQ_OUT = 1;     //DQ=1 
    DelayUs(10);            //延时10us
}

/**
 * @fn DS18B20_Check
 * @brief DS18B20检测器件是否正常
 * @return 0 存在
 *      其他 未检测到DS18B20的存在
 */
uint8_t DS18B20_Check(void)
{
  uint8_t result=0;
	
	DS18B20_Rest();             //先进行初始化
	DS18B20_IO_IN();    //设置为输入
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
 * @brief DS18B20初始化
 * @return 
 */
uint8_t DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /*Configure GPIO pin : PC11 */
    GPIO_InitStruct.Pin = GPIO_PIN_11;              //PC11
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     //推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;             //上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   //高速
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);         //初始化

    return DS18B20_Check();     //返回状态
}

/**
 * @fn DS18B20_Read_Bit
 * @brief 从DS18B20读取一个位
 * @return 0 / 1
 */
static uint8_t DS18B20_Read_Bit(void)
{
    uint8_t data;
    DS18B20_IO_OUT();       //设置为输出
    DS18B20_DQ_OUT=0; 
    DelayUs(2);
    DS18B20_DQ_OUT=1; 
    DS18B20_IO_IN();        //设置为输入
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
 * @brief 从DS18B20读取一个字节
 * @return 读到的数据
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
 * @brief 从DS18B20写入一个字节
 * @param 要写入的数据
 */
static void DS18B20_Write_Byte(uint8_t dat)
 {             
    uint8_t j = 0;
    uint8_t testb = 0;
    DS18B20_IO_OUT();     //设置为输出
    for (j = 1; j <= 8; j++) 
	{
        testb = dat&0x01;
        dat = dat>>1;
        if(testb)       // 写1
        {
            DS18B20_DQ_OUT = 0;
            DelayUs(2);                            
            DS18B20_DQ_OUT = 1;
            DelayUs(60);             
        }
        else    //写0
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
 * @brief 开始温度转换
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
 * @brief 获取温度值
 * @return 温度值，精度在 ±0.1℃，范围（-55℃ —— 125℃）
 */
float DS18B20_Get_Temp(void)
{
    uint8_t tempL = 0;
    uint8_t tempH = 0;
    uint16_t tempValue = 0;
    float wendu = 0.0;
    DS18B20_Start();           //开始转换
    DS18B20_Rest();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xCC);   // 跳过ROM指令
    DS18B20_Write_Byte(0xBE);   // 温度转化指令	    
    tempL = DS18B20_Read_Byte();     // LSB   
    tempH = DS18B20_Read_Byte();     // MSB   
    
    tempValue = tempH;		   
    tempValue <<= 8;			    //!< temp 为unsigned int 型，它共有16位，b向右左8位，即把b放在了tempr的高八位，放在了低八位。
    tempValue = tempValue | tempL;  //!< "|",与，有1便是“1”，temp 有十六位，高八位是b的位，这步的作用是把a赋给temp的低八位，所以现在temp 就是温度，但是没是这默认为正的温度没有负的温度
   
    wendu = tempValue * 0.0625;     //!< 计算温度
    
    tempH >>= 4;
    if(tempH == 0x0F)
    {
        wendu -= 4095.5;
    }
    return wendu;
}

