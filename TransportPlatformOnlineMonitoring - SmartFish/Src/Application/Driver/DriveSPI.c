/**
 * @file DriveSPI.c
 * @brief SPI模块初始化，中断操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
 
#include "DriveSPI.h"

SPI_HandleTypeDef hspi1;

/**
 * @fn Init_SPI()
 * @brief SPI初始化
 * 
 */
void Init_SPI(void)
{ 
    /* SPI1 parameter configuration*/
    hspi1.Instance = SPI1;                                      //!< 
    hspi1.Init.Mode = SPI_MODE_MASTER;                          //!< 设置SPI工作模式，设置为主模式
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;                //!< 设置SPI单向或者双向的数据模式:SPI设置为双线模式
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                    //!< 设置SPI的数据大小:SPI发送接收8位帧结构
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;                 //!< 串行同步时钟的空闲状态为高电平
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;                      //!< 串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    hspi1.Init.NSS = SPI_NSS_SOFT;                              //!< NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:
                                                                //!< 内部NSS信号有SSI位控制
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;   //!< 定义波特率预分频的值:波特率预分频值为256
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                     //!< 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;                     //!< 关闭TI模式
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;     //!< 关闭硬件CRC校验
    hspi1.Init.CRCPolynomial = 7;                               //!< CRC值计算的多项式
    if (HAL_SPI_Init(&hspi1) != HAL_OK)                         //!< 初始化
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    //__HAL_SPI_ENABLE(&hspi1);                            //!< 使能SPI1
}

/**
 * @fn HAL_SPI_MspInit
 * @brief SPI底层驱动，开启时钟，设置中断优先级
 * HAL_SPI_Init()函数调用
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(hspi->Instance==SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        /**SPI1 GPIO Configuration    
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* SPI1 interrupt Init */
        HAL_NVIC_SetPriority(SPI1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
        /* USER CODE BEGIN SPI1_MspInit 1 */

        /* USER CODE END SPI1_MspInit 1 */
    }

}
//SPI速度设置函数
//
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
//fAPB1时钟一般为42Mhz：

/**
 * @fn SPI1_SetSpeed
 * @brief SPI速度设置函数
 * SPI速度=fAPB1/分频系数,
 * @param SPI_BaudRate_Prescaler SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
 * fAPB1时钟一般为42Mhz：
 */
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); //!< 判断有效性
    __HAL_SPI_DISABLE(&hspi1);                      //!< 关闭SPI
    hspi1.Instance->CR1&=0XFFC7;                    //!< 位3-5清零，用来设置波特率
    hspi1.Instance->CR1|=SPI_BaudRatePrescaler;     //!< 设置SPI速度
    __HAL_SPI_ENABLE(&hspi1);                       //!< 使能SPI
    
}

/**
 * @fn SPI1_ReadWriteByte()
 * @brief 读写一个字节
 * @param TxData 要写入的字节
 * @return 读取到的字节
 */
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(&hspi1, &TxData, &Rxdata, 1, 1000);       
 	return Rxdata;          		    //!< 返回收到的数据		
}
