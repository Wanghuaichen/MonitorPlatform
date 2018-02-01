/**
 * @file UserFreeRTOS.h
 * @brief 用户使用的配置文件
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company  SOJO
 * @date 2018.1.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_FREERTOS_H
#define __USER_FREERTOS_H

/* Includes ------------------------------------------------------------------*/
/**
 * @brief FreeRTOS Includes
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "SysParam.h"

#ifdef  __HANDLER_GLOBALS
#define __HANDLER_EXT  
#else
#define __HANDLER_EXT extern
#endif

__HANDLER_EXT xQueueHandle Queue_AttiAndAccMsg;
__HANDLER_EXT xQueueHandle Queue_TempMsg;
__HANDLER_EXT xQueueHandle Queue_GpsMsg;

__HANDLER_EXT EventGroupHandle_t EventGroup_SystemStatus;

/**
 * @brief 开始任务，用于创建其他任务
 */
#define START_TASK_PRIO		1             //<! 任务优先级

#define START_STK_SIZE 		128           //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t StartTask_Handler;         //<! 任务句柄

void StartTask(void const * argument);  //<! 任务函数


#if (DEBUG_MODE == 1)
/**
 * @brief 调试任务，用于调试系统各功能
 */
#define DEBUG_TASK_PRIO		29           //<! 任务优先级

#define DEBUG_STK_SIZE 		256          //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t DebugTask_Handler;        //<! 任务句柄

void Task_Debug(void const *argument); //<! 任务函数

#endif

/**
 * @brief 用于监控各传感器状态并喂狗
 */
#define MONITOR_TASK_PRIO		9             //<! 任务优先级

#define MONITOR_STK_SIZE 		128           //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t MonitorTask_Handler;         //<! 任务句柄

void Task_Monitor(void const *argument);  //<! 任务函数

/**
 * @brief 用于获取倾角和加速度
 */
#define ATTI_ACC_TASK_PRIO		10       //<! 任务优先级

#define ATTI_ACC_STK_SIZE 		256      //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t GetAttiAndAccMsgTask_Handler;        //<! 任务句柄

void Task_GetAttiAndAccMsg(void const *argument); //<! 任务函数

/**
 * @brief 用于获取温度
 */
#define GET_TEMP_TASK_PRIO		11       //<! 任务优先级

#define GET_TEMP_STK_SIZE 		128      //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t GetTempMsgTask_Handler;   //<! 任务句柄

void Task_GetTempMsg(void const *argument);  //<! 任务函数

/**
 * @brief 用于获取GPS信息
 */
#define GET_GPS_MSG_TASK_PRIO		12     //<! 任务优先级

#define GET_GPS_MSG_STK_SIZE 		256    //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t GetGpsMsgTask_Handler;    //<! 任务句柄

void Task_GetGpsMsg(void const *argument);   //<! 任务函数

/**
 * @brief 用于数据管理，将各传感器数据写入flash
 */
#define MSG_MANAGER_TASK_PRIO		13     //<! 任务优先级

#define MSG_MANAGER_STK_SIZE 		512    //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t MsgManagerTask_Handler;   //<! 任务句柄

void Task_MsgManager(void const *argument);   //<! 任务函数

/**
 * @brief 用于控制LED和继电器
 */
#define LED_RELAY_TASK_PRIO		14           //<! 任务优先级

#define LED_RELAY_STK_SIZE 		64           //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t LedAndRelayTask_Handler;      //<! 任务句柄

void Task_LedAndRelay(void const *argument);   //<! 任务函数

/**
 * @brief 用于打印输出各所需数据
 */
#define MSG_PRINT_TASK_PRIO		15          //<! 任务优先级

#define MSG_PRINT_STK_SIZE 		256         //<! 任务堆栈大小	

__HANDLER_EXT TaskHandle_t MsgPrintTask_Handler;        //<! 任务句柄

void Task_MsgPrint(void const *argument); //<! 任务函数


#endif /* __HEADER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
