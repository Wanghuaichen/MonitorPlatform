/**
 * @file ADXL345.c
 * @brief ADXL345读写操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.26
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
 
#include "ADXL345.h"
#include "DriveI2C.h"

#define TIME_OUT    100

static void ADXL345_ReadAddress(uint8_t* temp, uint8_t id);
static void ADXL345_WriteRegister(uint8_t data, uint8_t registerAddress, uint8_t id);

/**
 * @fn ADXL345_Init()
 * @brief 初始化ADXL345.
 * @return 0,初始化成功;其他,初始化失败.
 */
uint8_t ADXL345_Init(uint8_t id)
{
    uint8_t macID = 0;  //!< 器件地址
    ADXL345_ReadAddress(&macID, id);
    if(macID == MAC_ADXL_ID)
    {
        ADXL345_WriteRegister(0x2B, DATA_FORMAT, id);   //!< 低电平中断输出,13位全分辨率,输出数据右对齐,16g量程 
        ADXL345_WriteRegister(0x0E, BW_RATE, id);       //!< 输出速率800Hz
        ADXL345_WriteRegister(0x28, POWER_CTL, id);     //!< 链接使能,测量模式
        ADXL345_WriteRegister(0x00, INT_ENABLE, id);    //!< 不使用中断  
        ADXL345_WriteRegister(0x00, OFSX, id);  //!< X偏置
        ADXL345_WriteRegister(0x00, OFSY, id);  //!< Y偏置
        ADXL345_WriteRegister(0x00, OFSZ, id);  //!< Z偏置
        return 0;
    }
    return 0x01;
}

/**
 * @fn ADXL345_ReadAddress()
 * @brief 读ADXL345寄存器.
 * @param temp 读取出来的数据
 * @param id ADXL345 ID号
 *
 */
void ADXL345_ReadAddress(uint8_t* temp, uint8_t id)
{
    switch(id)
    {
        case ADXL345_FIRST_ID:
        {
            while(HAL_I2C_Mem_Read(&g_hi2c1, ADXL_FIRST_ADDRESS_READ, DEVICE_ID, 1, temp, 1, TIME_OUT) != HAL_OK)
            {
                /* Error_Handler() function is called when Timeout error occurs.
                When Acknowledge failure occurs (Slave don't acknowledge it's address)
                Master restarts communication */
                if (HAL_I2C_GetError(&g_hi2c1) != HAL_I2C_ERROR_AF)
                {
                    Error_Handler();
                }
            }
            break;
        }
        case ADXL345_SECOND_ID:
        {
            while(HAL_I2C_Mem_Read(&g_hi2c1, ADXL_SECOND_ADDRESS_READ, DEVICE_ID, 1, temp, 1, TIME_OUT) != HAL_OK)
            {
                /* Error_Handler() function is called when Timeout error occurs.
                When Acknowledge failure occurs (Slave don't acknowledge it's address)
                Master restarts communication */
                if (HAL_I2C_GetError(&g_hi2c1) != HAL_I2C_ERROR_AF)
                {
                    Error_Handler();
                }
            }
            break;
        }
    }
}

/**
 * @fn ADXL345_Check()
 * @brief 检查传感器状态
 * @param id ADXL345 ID号
 * @return 0,在线;其他,不在线.
 * 
 */
uint8_t ADXL345_Check(uint8_t id)
{
		uint8_t macID = 0;  //!< 器件地址
    ADXL345_ReadAddress(&macID, id);
		if(macID == MAC_ADXL_ID)
			return 0;
		return 1;
}


/**
 * @fn ADXL345_ReadAddress()
 * @brief 写ADXL345寄存器.
 * @param data 要写入的数据
 * @param registerAddress 寄存器地址
 * @param id ADXL345 ID号
 *
 */
void ADXL345_WriteRegister(uint8_t data, uint8_t registerAddress, uint8_t id)
{ 
//    uint16_t address = ((uint16_t)registerAddress) | 0x00FF;
    
    switch(id)
    {
        case ADXL345_FIRST_ID:
        {
            while(HAL_I2C_Mem_Write(&g_hi2c1, ADXL_FIRST_ADDRESS_WRITE, registerAddress, 1, &data, 1, TIME_OUT) != HAL_OK)
            {
                /* Error_Handler() function is called when Timeout error occurs.
                When Acknowledge failure occurs (Slave don't acknowledge it's address)
                Master restarts communication */
                if (HAL_I2C_GetError(&g_hi2c1) != HAL_I2C_ERROR_AF)
                {
                    Error_Handler();
                }
            }
            break;
        }
        case ADXL345_SECOND_ID:
        {
            while(HAL_I2C_Mem_Write(&g_hi2c1, ADXL_SECOND_ADDRESS_WRITE, registerAddress, 1, &data, 1, TIME_OUT) != HAL_OK)
            {
                /* Error_Handler() function is called when Timeout error occurs.
                When Acknowledge failure occurs (Slave don't acknowledge it's address)
                Master restarts communication */
                if (HAL_I2C_GetError(&g_hi2c1) != HAL_I2C_ERROR_AF)
                {
                    Error_Handler();
                }
            }
            break;
        }
    }
    
}

/**
 * @fn ADXL345_ReadTheEntireAxis()
 * @brief 读ADXL345所有方向上的加速度
 * @param data 指向读取到数据的指针
 * @param id ADXL345 ID号
 *
 */
void ADXL345_ReadTheEntireAxis(int16_t *data, uint8_t id)
{
    uint8_t temp[6];
    switch(id)
    {
        case ADXL345_FIRST_ID:
        {
            
            while(HAL_I2C_Mem_Read(&g_hi2c1, ADXL_FIRST_ADDRESS_READ, START_ADDRESS, 1, temp, 6, TIME_OUT) != HAL_OK)
            {
                /* Error_Handler() function is called when Timeout error occurs.
                When Acknowledge failure occurs (Slave don't acknowledge it's address)
                Master restarts communication */
                if (HAL_I2C_GetError(&g_hi2c1) != HAL_I2C_ERROR_AF)
                {
                    Error_Handler();
                }
            }
            for(uint8_t i = 0; i < 3;i++)
            {
                data[i] = ((int16_t)(temp[i*2+1])<<8) + temp[i*2];
            }
            break;
        }
        case ADXL345_SECOND_ID:
        {
            
            while(HAL_I2C_Mem_Read(&g_hi2c1, ADXL_SECOND_ADDRESS_READ, START_ADDRESS, 1, temp, 6, TIME_OUT) != HAL_OK)
            {
                /* Error_Handler() function is called when Timeout error occurs.
                When Acknowledge failure occurs (Slave don't acknowledge it's address)
                Master restarts communication */
                if (HAL_I2C_GetError(&g_hi2c1) != HAL_I2C_ERROR_AF)
                {
                    Error_Handler();
                }
            }
            for(uint8_t i = 0; i < 3;i++)
            {
                data[i] = ((int16_t)(temp[i*2+1])<<8) + temp[i*2];
            }
            break;
        }
    }
}

/**
 * @fn ADXL345_ReadAllAxisAverageValue()
 * @brief ADXL345读取10次后取平均值
 * @param xAverage x轴平均值
 * @param yAverage y轴平均值
 * @param zAverage z轴平均值
 * @param id ADXL345 ID号
 *
 */
void ADXL345_ReadAllAxisAverageValue(float *xAverage, float *yAverage, float *zAverage, uint8_t id)
{
    uint16_t xValue = 0;
    uint16_t yValue = 0;
    uint16_t zValue = 0;
	  int16_t accelerationData[3];
    
    for(uint8_t i = 0; i < 10; i++)
    {
        ADXL345_ReadTheEntireAxis(accelerationData, id);
        xValue += accelerationData[0];
        yValue += accelerationData[1];
        zValue += accelerationData[2];
    }
    
    *xAverage = xValue/10.0f;
    *yAverage = yValue/10.0f;
    *zAverage = zValue/10.0f;
}

/**
 * @fn ADXL345_AutomaticCalibration()
 * @brief ADXL345自动校准
 * @param xval x轴校准值
 * @param yval y轴校准值
 * @param zval z轴校准值
 * @param id ADXL345 ID号
 *
 */
void ADXL345_AutomaticCalibration(char *xval,char *yval,char *zval, uint8_t id)
{
	  float xValue = 0,yValue = 0,zValue = 0;
	  float offx = 0,offy = 0,offz = 0;
	  uint8_t i = 0;
    
    ADXL345_WriteRegister(0x00, POWER_CTL, id);     //!< 先进入休眠模式
    
    ADXL345_WriteRegister(0x2B, DATA_FORMAT, id);   //!< 低电平中断输出,13位全分辨率,输出数据右对齐,16g量程 
    ADXL345_WriteRegister(0x0E, BW_RATE, id);       //!< 输出速率800Hz
    ADXL345_WriteRegister(0x28, POWER_CTL, id);     //!< 链接使能,测量模式
    ADXL345_WriteRegister(0x00, INT_ENABLE, id);    //!< 不使用中断  
    ADXL345_WriteRegister(0x00, OFSX, id);  //!< X偏置
    ADXL345_WriteRegister(0x00, OFSY, id);  //!< Y偏置
    ADXL345_WriteRegister(0x00, OFSZ, id);  //!< Z偏置
    
    for(i = 0; i < 10; i++)
    {
        ADXL345_ReadAllAxisAverageValue(&xValue, &yValue, &zValue, id);
        offx += xValue;
        offy += yValue;
        offz += zValue;
    }
    /**
     * @brief 取平均值
     */
		 offx /= 10.0f;
	   offy /= 10.0f;
	   offz /= 10.0f;
    /**
     * @brief 计算校准值
     */
	   *xval = -(offx / 4);
	   *yval = -(offy / 4);
	   *zval = -((offz - 256) / 4);
    /**
     * @brief 将计算后的值写入到校准寄存器中
     */
 	   ADXL345_WriteRegister(*xval, OFSX, id);
 	   ADXL345_WriteRegister(*yval, OFSY, id);
 	   ADXL345_WriteRegister(*zval, OFSZ, id);
}
