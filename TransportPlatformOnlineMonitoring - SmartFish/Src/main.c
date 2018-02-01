/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


#define __HANDLER_GLOBALS  
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32f4xx_hal.h"
/**
 * @brief Application Includes
 */ 
#include "SystemConfig.h"
#include "DriveIO.h"
#include "DriveUSART.h"	
#include "DriveTIM.h"
#include "DriveI2C.h"
#include "DriveSPI.h"
#include "DriveIWDG.h"
#include "ADXL345.h"
#include "SIM868.h"
#include "SD_ReadWrite.h"
#include "DriveDelay.h"
#include "TimeOutDetection.h"
#include "DriveRS485.h"
#include "NandFlash.h"
#include "DS18B20.h"

#include "Task_GetAttiAndAccMsg.h"
#include "Task_GetGpsMsg.h"

/**
 * @brief FreeRTOS Includes
 * @note 定义__HANDLER_GLOBALS用于定义句柄
 */

#include "UserFreeRTOS.h"

/* FATFS includes. */
#include "ff.h"  
#include "diskio.h"		/* FatFs lower layer API */

/**
 * @brief 测试用代码，正式发布时需要删除掉
 */
/********************************************************************************/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))  /****/
uint8_t aTxBuffer[] = " **** UART_TwoBoards_DmaPolling **** ";              /****/
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)          /****/
                                                                            /****/
/********************************************************************************/

/**
 * @fn main
 * @brief 程序入口
 * @return 
 */
int main(void)
{
    HAL_Init();                     //!< HAL库初始化
    SystemClock_Config();           //!< 系统时钟初始化 84MHz
    IniutDelay(SYSCLK);             //!< 初始化延时函数
    Init_DriveIO();                 //!< IO口初始化
    Init_UART2(USART2_BOUND);       //!< 串口2初始化
    Init_UART6(USART6_BOUND);       //!< 串口6初始化
    Init_TIM2(TIM2_ARR, TIM2_PSC);  //!< 定时器2初始化
    Init_I2C1();                    //!< I2C1初始化
    InitTimeOutStampCollect();      //!< 超时检测时间戳初始化

    RS485_Init();                   //!< 485通信初始化
    
    //获取内存
    g_pfs[0]=(FATFS*)ff_memalloc(sizeof(FATFS));
    g_pfs[1]=(FATFS*)ff_memalloc(sizeof(FATFS));
	
		//创建消息队列
		Queue_AttiAndAccMsg = xQueueCreate(5,sizeof(ADXL345StructTypeDef));
    Queue_TempMsg = xQueueCreate(5,sizeof(float));
		Queue_GpsMsg = xQueueCreate(5,sizeof(SIM868StuctTypeDef));
	
		//创建事件标志组
		EventGroup_SystemStatus = xEventGroupCreate();
    
    //创建开始任务
    xTaskCreate((TaskFunction_t )StartTask,             //!< 任务函数
                (const char*    )"StartTask",           //!< 任务名称
                (uint16_t       )START_STK_SIZE,        //!< 任务堆栈大小
                (void*          )NULL,                  //!< 传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //!< 任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //!< 任务句柄                
    vTaskStartScheduler();          //!< 开启任务调度
    while(1);
}

/**
 * @fn StartTask
 * @brief 开始任务函数
 * @param 
 */
void StartTask(void const * argument)
{
    taskENTER_CRITICAL();           //!< 进入临界区

#if (DEBUG_MODE == 1)
	
    //!< 调试任务
    xTaskCreate((TaskFunction_t )Task_Debug,            //!< 任务函数
                (const char*    )"Task_Debug",          //!< 任务名称
                (uint16_t       )DEBUG_STK_SIZE,        //!< 任务堆栈大小
                (void*          )NULL,                  //!< 传递给任务函数的参数
                (UBaseType_t    )DEBUG_TASK_PRIO,       //!< 任务优先级
                (TaskHandle_t*  )&DebugTask_Handler);   //!< 任务句柄
#endif
								
    //!< 获取倾角和加速度任务
    xTaskCreate((TaskFunction_t )Task_GetAttiAndAccMsg,     //!< 任务函数
                (const char*    )"Task_GetAttiAndAccMsg",   //!< 任务名称
                (uint16_t       )ATTI_ACC_STK_SIZE,         //!< 任务堆栈大小
                (void*          )NULL,                      //!< 传递给任务函数的参数
                (UBaseType_t    )ATTI_ACC_TASK_PRIO,        //!< 任务优先级
                (TaskHandle_t*  )&GetAttiAndAccMsgTask_Handler);   //!< 任务句柄
							
    //!< 获取温度任务
    xTaskCreate((TaskFunction_t )Task_GetTempMsg,           //!< 任务函数
                (const char*    )"Task_GetTempMsg",         //!< 任务名称
                (uint16_t       )GET_TEMP_STK_SIZE,         //!< 任务堆栈大小
                (void*          )NULL,                      //!< 传递给任务函数的参数
                (UBaseType_t    )GET_TEMP_TASK_PRIO,        //!< 任务优先级
                (TaskHandle_t*  )&GetTempMsgTask_Handler);  //!< 任务句柄

    //!< 获取GPS信息任务
    xTaskCreate((TaskFunction_t )Task_GetGpsMsg,            //!< 任务函数
                (const char*    )"Task_GetGpsMsg",          //!< 任务名称
                (uint16_t       )GET_GPS_MSG_STK_SIZE,      //!< 任务堆栈大小
                (void*          )NULL,                      //!< 传递给任务函数的参数
                (UBaseType_t    )GET_GPS_MSG_TASK_PRIO,     //!< 任务优先级
                (TaskHandle_t*  )&GetGpsMsgTask_Handler);   //!< 任务句柄

    //!< 数据管理任务
    xTaskCreate((TaskFunction_t )Task_MsgManager,           //!< 任务函数
                (const char*    )"Task_MsgManager",         //!< 任务名称
                (uint16_t       )MSG_MANAGER_STK_SIZE,      //!< 任务堆栈大小
                (void*          )NULL,                      //!< 传递给任务函数的参数
                (UBaseType_t    )MSG_MANAGER_TASK_PRIO,     //!< 任务优先级
                (TaskHandle_t*  )&MsgManagerTask_Handler);  //!< 任务句柄

//    //!< LED和继电器任务
//    xTaskCreate((TaskFunction_t )Task_LedAndRelay,          //!< 任务函数
//                (const char*    )"Task_LedAndRelay",        //!< 任务名称
//                (uint16_t       )LED_RELAY_STK_SIZE,        //!< 任务堆栈大小
//                (void*          )NULL,                      //!< 传递给任务函数的参数
//                (UBaseType_t    )LED_RELAY_TASK_PRIO,       //!< 任务优先级
//                (TaskHandle_t*  )&LedAndRelayTask_Handler); //!< 任务句柄		

    //!< 监控任务
    xTaskCreate((TaskFunction_t )Task_Monitor,              //!< 任务函数
                (const char*    )"Task_Monitor",            //!< 任务名称
                (uint16_t       )MONITOR_STK_SIZE,          //!< 任务堆栈大小
                (void*          )NULL,                      //!< 传递给任务函数的参数
                (UBaseType_t    )MONITOR_TASK_PRIO,         //!< 任务优先级
                (TaskHandle_t*  )&MonitorTask_Handler);     //!< 任务句柄	
								
//    //!< 数据输出任务
//    xTaskCreate((TaskFunction_t )Task_MsgPrint,             //!< 任务函数
//                (const char*    )"Task_MsgPrint",           //!< 任务名称
//                (uint16_t       )MSG_PRINT_STK_SIZE,        //!< 任务堆栈大小
//                (void*          )NULL,                      //!< 传递给任务函数的参数
//                (UBaseType_t    )MSG_PRINT_TASK_PRIO,       //!< 任务优先级
//                (TaskHandle_t*  )&MsgPrintTask_Handler);    //!< 任务句柄					
           
    vTaskDelete(StartTask_Handler); //!< 删除开始任务
    taskEXIT_CRITICAL();            //!< 退出临界区
}



///**
// * @fn FatfsTask
// * @brief 文件系统处理任务，任务内进行文件系统初始化等操作
// * @param 
// */
//void FatfsTask(void const * argument)
//{
//    const TickType_t delay = 5000 / portTICK_PERIOD_MS; //5000ms
//    const TickType_t successDelay = 1000 / portTICK_PERIOD_MS; //1000ms初始化成功后的
//    static uint8_t result = 0xFF;
//    while(result)
//    {
//        //    result = FAT_Mount(DEV_SD_CARD, "1:");
////        result = FAT_Mount(DEV_FLASH, "0:");
//        if(!result)
//        {
//            return;
//        }
//        vTaskDelay(delay); //!< 延时
//    }
//    while(1)
//	{
//        vTaskDelay(successDelay); //!< 延时
//	}
//}

///**
// * @fn ReadAcceleration
// * @brief 读取加速度任务函数
// * @param 
// */
//void ReadAcceleration(void const * argument)
//{
//    const TickType_t delay = 5000 / portTICK_PERIOD_MS;
//    static uint8_t state = 0xFF;
//    static uint16_t data[3] = {0};
//    while(state)                //!< ADXL345初始化检测
//    {
//        state = ADXL345_Init(ADXL345_SECOND_ID);
//        vTaskDelay(delay);     //!< 延时500ms
//    }
//    while(1)
//	{
//        ADXL345_ReadTheEntireAxis(data, ADXL345_SECOND_ID);
//        //!< LED1状态切换
//        vTaskDelay(delay);         //!< 延时5s
//	}
//}

///**
// * @fn ReadGpsMessage
// * @brief 读取GPS信息任务函数
// * @param 
// */
//void ReadGpsMessage(void const * argument)
//{
//    const TickType_t delay = 4000 / portTICK_PERIOD_MS;
//    static uint8_t state = 0xFF;
//    while(state)                //!< 初始化GPS模块
//    {
////        state = Sim868CheckOnline();
//        vTaskDelay(delay);     //!< 延时4s
//    }
//    while(1)
//	{
//        //!< LED2状态切换
//        vTaskDelay(delay);     //!< 延时4s
//	}
//}

///**
// * @fn ReadGpsMessage
// * @brief Rtu信息处理服务
// * @param 
// */
//void RtuTask(void const * argument)
//{
//    const TickType_t delay = 500 / portTICK_PERIOD_MS; //500ms
//    uint8_t sendData[5] = {0xA5, 0x5A, 0x55, 0xAA, 0xFF};
//    while(1)
//	{
//        RS485_SendMessage(sendData, 5); //做接收处理
//        vTaskDelay(delay); //!< 延时
//	}
//}

///**
// * @fn GetTempTask
// * @brief 获取板子温度
// * @param 
// */
//void GetTempTask(void const * argument)
//{
//    const TickType_t delay = 20000 / portTICK_PERIOD_MS;     //20s
//    float tempValue = 0;
//    /**
//     * @note 由于DS18B20时序要求严格，所以需要进行临界区处理
//     */
//    taskENTER_CRITICAL();           //!< 进入临界区
//    while(DS18B20_Init())
//    {
//        RUN_LED = (RUN_LED == 0) ? 1 : 0;   //!< LED0状态切换
//        vTaskDelay(100); //!< 延时
//    }
//    taskEXIT_CRITICAL();            //!< 退出临界区
//    while(1)
//	{
//        taskENTER_CRITICAL();           //!< 进入临界区
//        tempValue = DS18B20_Get_Temp();
//        taskEXIT_CRITICAL();            //!< 退出临界区
//        vTaskDelay(delay); //!< 延时
//	}
//}

///**
// * @fn RunLedTask
// * @brief 运行指示灯任务，优先级最低
// * @param 
// */
//void RunLedTask(void const * argument)
//{
//    uint8_t cn = 0;
//    const TickType_t delay = 500 / portTICK_PERIOD_MS;
//    while(1)
//	{
//        RUN_LED = (RUN_LED == 0) ? 1 : 0;   //!< LED0状态切换
///************************************************************************/
//        /*测试内容*/
//        cn++;
//        if(cn == 1)
//        {
//            RELAY1 = (RELAY1 == 0) ? 1 : 0;   //!< LED0状态切换
//        }
//        else if(cn == 2)
//        {
//            RELAY2 = (RELAY1 == 0) ? 1 : 0;   //!< LED0状态切换
//        }
//        else if(cn == 3)
//        {
//            RELAY3 = (RELAY1 == 0) ? 1 : 0;   //!< LED0状态切换
//        }
//        else if(cn == 4)
//        {
//            EN_D3V = (EN_D3V == 0) ? 1 : 0;   //!< LED0状态切换
//        }
//        else if(cn == 5)
//        {
//            EN_D5V = (EN_D5V == 0) ? 1 : 0;   //!< LED0状态切换
//        }
//        else if(cn == 6)
//        {
//            EN_D12V = (EN_D12V == 0) ? 1 : 0;   //!< LED0状态切换
//            cn = 0;
//        }
///************************************************************************/
//        vTaskDelay(delay);                 //!< 延时500ms
//	}
//}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
