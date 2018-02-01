/**
 * @file DriveI2C.c
 * @brief I2C模块初始化，中断操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
 
#include "DriveI2C.h"
#include "ADXL345.h"

I2C_HandleTypeDef g_hi2c1;


/**
 * @fn Init_I2C1()
 * @brief I2C1初始化
 * 
 */
void Init_I2C1(void)
{ 
    g_hi2c1.Instance = I2C1;                                //!< 初始化I2C1模块
    g_hi2c1.Init.ClockSpeed = 400000;                       //!< I2C2的速度为400k
    g_hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;               //!< 快速模式下占空比
    g_hi2c1.Init.OwnAddress1 = ADXL_FIRST_ADDRESS;          //!< 指定第一个设备自己的地址，参数7或者10位均可
    g_hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;  //!< 指定为7位地址模式
    g_hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_ENABLE;  //!< 开启双寻址模式
    g_hi2c1.Init.OwnAddress2 = ADXL_SECEND_ADDRESS;         //!< 若使能了双寻址模式，指定第二个设备地址，可以是7位地址
    g_hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_ENABLE;  //!< 禁止通用调用寻址模式
    g_hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     //!< 禁止nostretch模式
    if (HAL_I2C_Init(&g_hi2c1) != HAL_OK)                   //!< 初始化I2C2模块，检查其是否能初始化成功
    {
    _Error_Handler(__FILE__, __LINE__);
    }
}


/**
 * @fn HAL_I2C_MspInit
 * @brief I2C底层驱动，开启时钟，设置中断优先级
 * HAL_I2C_Init()函数调用
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(hi2c->Instance == I2C1)
    {
        /* USER CODE BEGIN I2C1_MspInit 0 */

        /* USER CODE END I2C1_MspInit 0 */

        /**I2C1 GPIO Configuration    
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
        /* USER CODE BEGIN I2C1_MspInit 1 */

        /* USER CODE END I2C1_MspInit 1 */
    }
    else if(hi2c->Instance == I2C2)
    {
        /* USER CODE BEGIN I2C2_MspInit 0 */

        /* USER CODE END I2C2_MspInit 0 */

        /**I2C2 GPIO Configuration    
        PB10     ------> I2C2_SCL
        PB3     ------> I2C2_SDA 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_I2C2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();
        /* USER CODE BEGIN I2C2_MspInit 1 */

        /* USER CODE END I2C2_MspInit 1 */
    }

}


