/**
 * @file TimeOutDetection.h
 * @brief 使用FreeRTOS中的OSTimeGet函数设计超时检测函数
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.26
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _TIME_OUT_DETECTION_H
#define _TIME_OUT_DETECTION_H
#include "SystemConfig.h"


typedef struct TagTimeStamp
{
    uint32_t startTime;    //超时判断起始时间
    uint32_t delayTime;

    //系统运行时需要的时间值

} TimeStamp;

typedef struct TagTimeStampCollect
{
    TimeStamp usart2DmaTime;    //串口1DMA发送、接收超时检测
    
} TimeStampCollect;


void InitTimeOutStampCollect(void);
uint8_t IsOverTimeStamp(TimeStamp* pStamp);

extern TimeStampCollect g_TimeStampCollect;    //状态时间

#endif

