/**
 * @file TimeOutDetection.h
 * @brief ʹ��FreeRTOS�е�OSTimeGet������Ƴ�ʱ��⺯��
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
    uint32_t startTime;    //��ʱ�ж���ʼʱ��
    uint32_t delayTime;

    //ϵͳ����ʱ��Ҫ��ʱ��ֵ

} TimeStamp;

typedef struct TagTimeStampCollect
{
    TimeStamp usart2DmaTime;    //����1DMA���͡����ճ�ʱ���
    
} TimeStampCollect;


void InitTimeOutStampCollect(void);
uint8_t IsOverTimeStamp(TimeStamp* pStamp);

extern TimeStampCollect g_TimeStampCollect;    //״̬ʱ��

#endif

