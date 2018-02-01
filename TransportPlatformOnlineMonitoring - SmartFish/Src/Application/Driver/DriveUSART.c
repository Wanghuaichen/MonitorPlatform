/**
 * @file usart.h
 * @brief 串口1配置
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company  SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "DriveUSART.h"	
#include "TimeOutDetection.h"

/**
 * @brief FreeRTOS Includes
 */
#include "FreeRTOS.h"
#include "task.h"

static DMA_HandleTypeDef UART2TxDMA_Handler;
static DMA_HandleTypeDef UART2RxDMA_Handler;
static UART_HandleTypeDef Uart2Handler;     //!< UART2句柄
static UART_HandleTypeDef Uart6Handler;     //!< UART6句柄

static __IO ITStatus Uart2Ready = RESET;

/////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
 */
#pragma import(__use_no_semihosting)             
//!< 标准库需要的支持函数                 
struct __FILE 
{
	int handle; 
}; 

FILE __stdout;       
//!< 定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

/////////////////////////////////////////////////////////////////////////////////

/**
 * @fn Init_UART2()
 * @brief 初始化串口2
 * @prarm bound  波特率
 * @note 增加DMA
 */
void Init_UART2(uint32_t bound)
{
	Uart2Handler.Instance = USART2;                        //!< 初始化串口2
	Uart2Handler.Init.BaudRate = bound;                    //!< 设置波特率
	Uart2Handler.Init.WordLength = UART_WORDLENGTH_8B;     //!< 长度为8位模式
	Uart2Handler.Init.StopBits = UART_STOPBITS_1;          //!< 停止位1位
	Uart2Handler.Init.Parity = UART_PARITY_NONE;           //!< 无校验
	Uart2Handler.Init.Mode = UART_MODE_TX_RX;              //!< 收发模式
	Uart2Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;     //!< 无硬件数据流控制
	Uart2Handler.Init.OverSampling = UART_OVERSAMPLING_16; //!< 
	if (HAL_UART_Init(&Uart2Handler) != HAL_OK)            //!< 检查初始化是否成功
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}


/**
 * @fn Init_UART6()
 * @brief 初始化串口6
 * @prarm bound  波特率
 */
void Init_UART6(uint32_t bound)
{	
    Uart6Handler.Instance = USART6;                        //!< 初始化串口2
	Uart6Handler.Init.BaudRate = bound;                    //!< 设置波特率
	Uart6Handler.Init.WordLength = UART_WORDLENGTH_8B;     //!< 长度为8位模式
	Uart6Handler.Init.StopBits = UART_STOPBITS_1;          //!< 停止位1位
	Uart6Handler.Init.Parity = UART_PARITY_NONE;           //!< 无校验
	Uart6Handler.Init.Mode = UART_MODE_TX_RX;              //!< 收发模式
	Uart6Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;     //!< 无硬件数据流控制
	Uart6Handler.Init.OverSampling = UART_OVERSAMPLING_16; //!< 
	if (HAL_UART_Init(&Uart6Handler) != HAL_OK)            //!< 检查初始化是否成功
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}

/**
 * @fn HAL_UART_MspInit()
 * @brief UART底层初始化，时钟使能，引脚配置，中断配置
 * 此函数会被HAL_UART_Init()调用
 * @prarm huart 串口句柄
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //!< GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if(huart->Instance == USART2)                 	//!< 如果是串口1，进行串口1 MSP初始化
	{
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();
        /* DMA controller clock enable */
        __HAL_RCC_DMA1_CLK_ENABLE();

        /**USART2 GPIO Configuration    
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            
        /*##-3- Configure the DMA streams ##########################################*/
        /* Configure the DMA handler for Transmission process */
        UART2TxDMA_Handler.Instance                 = DMA1_Stream6;

        UART2TxDMA_Handler.Init.Channel             = DMA_CHANNEL_4;
        UART2TxDMA_Handler.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        UART2TxDMA_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
        UART2TxDMA_Handler.Init.MemInc              = DMA_MINC_ENABLE;
        UART2TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        UART2TxDMA_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        UART2TxDMA_Handler.Init.Mode                = DMA_NORMAL;
        UART2TxDMA_Handler.Init.Priority            = DMA_PRIORITY_LOW;
        UART2TxDMA_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        UART2TxDMA_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        UART2TxDMA_Handler.Init.MemBurst            = DMA_MBURST_INC4;
        UART2TxDMA_Handler.Init.PeriphBurst         = DMA_PBURST_INC4;

        HAL_DMA_Init(&UART2TxDMA_Handler);   

        /* Associate the initialized DMA handle to the the UART handle */
        __HAL_LINKDMA(huart, hdmatx, UART2TxDMA_Handler);

        /* Configure the DMA handler for Transmission process */
        UART2RxDMA_Handler.Instance                 = DMA1_Stream5;

        UART2RxDMA_Handler.Init.Channel             = DMA_CHANNEL_4;
        UART2RxDMA_Handler.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        UART2RxDMA_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
        UART2RxDMA_Handler.Init.MemInc              = DMA_MINC_ENABLE;
        UART2RxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        UART2RxDMA_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        UART2RxDMA_Handler.Init.Mode                = DMA_NORMAL;
        UART2RxDMA_Handler.Init.Priority            = DMA_PRIORITY_HIGH;
        UART2RxDMA_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
        UART2RxDMA_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        UART2RxDMA_Handler.Init.MemBurst            = DMA_MBURST_INC4;
        UART2RxDMA_Handler.Init.PeriphBurst         = DMA_PBURST_INC4; 

        HAL_DMA_Init(&UART2RxDMA_Handler);

        /* Associate the initialized DMA handle to the the UART handle */
        __HAL_LINKDMA(huart, hdmarx, UART2RxDMA_Handler);

        /*##-4- Configure the NVIC for DMA #########################################*/
        /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
        HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 1);
        HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

        /* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
        HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);   
        HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

        /* NVIC configuration for USART TC interrupt */
        HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
  
	}
    else if(huart->Instance == USART6)
    {
        /* USER CODE BEGIN USART6_MspInit 0 */

        /* USER CODE END USART6_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART6_CLK_ENABLE();

        /**USART6 GPIO Configuration    
        PA11     ------> USART6_TX
        PA12     ------> USART6_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART6 interrupt Init */
        HAL_NVIC_SetPriority(USART6_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART6_IRQn);
        /* USER CODE BEGIN USART6_MspInit 1 */

        /* USER CODE END USART6_MspInit 1 */
    }
}

/**
 * @fn DMA_USART2_Transmit
 * @brief 开启一次DMA传输
 * @param pData 传输的数据指针
 * @param Size 传输的数据长度
 */
uint8_t statea = 0;
uint8_t DMA_USART2_Transmit(uint8_t *pData, uint16_t size)
{
    statea = HAL_UART_Transmit_DMA(&Uart2Handler, pData, size);
    if(statea != HAL_OK)
    {
        Error_Handler();
    }
    return HAL_OK;
}

/**
 * @fn DMA_USART2_Receive
 * @brief 检测DMA接收数据是否成功
 * @param pRecviveData 接收的数据指针
 * @param Size 接收的数据长度
 */
uint8_t DMA_USART2_Receive(uint8_t *pRecviveData, uint16_t size)
{
    if(HAL_UART_Receive_DMA(&Uart2Handler, pRecviveData, size) != HAL_OK)
    {
        Error_Handler();
    }
    Uart2Ready = RESET;
    return HAL_OK;
}

/**
 * @fn WaitSendOrReceive
 * @brief 等待DMA完成接收或发送
 */
inline uint8_t WaitSendOrReceive(void)
{
    static uint8_t result = 0;
    g_TimeStampCollect.usart2DmaTime.startTime = xTaskGetTickCount();
    /*##-3- Wait for the end of the transfer ###################################*/  
    while (Uart2Ready != SET)
    {
        if(IsOverTimeStamp(&g_TimeStampCollect.usart2DmaTime))
        {
            result = 1;
            break;
        }
    }
    /* Reset transmission flag */
    Uart2Ready = RESET;
    return result;
}


/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{
    /* USER CODE BEGIN USART2_IRQn 0 */

    /* USER CODE END USART2_IRQn 0 */
    HAL_UART_IRQHandler(&Uart2Handler);
    /* USER CODE BEGIN USART2_IRQn 1 */

    /* USER CODE END USART2_IRQn 1 */
}

/**
* @brief This function handles DMA2 stream2 global interrupt.
*/
void DMA1_Stream5_IRQHandler(void)
{
    /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

    /* USER CODE END DMA2_Stream2_IRQn 0 */
    HAL_DMA_IRQHandler(Uart2Handler.hdmarx);
    /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

    /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
* @brief This function handles DMA2 stream7 global interrupt.
*/
void DMA1_Stream6_IRQHandler(void)
{
    /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */
    
    /* USER CODE END DMA1_Stream6_IRQn 0 */
    HAL_DMA_IRQHandler(Uart2Handler.hdmatx);
    /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

    /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete*/
    Uart2Ready = SET;

}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    /* Set transmission flag: transfer complete*/
    Uart2Ready = SET;
  
}

