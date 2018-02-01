/**
 * @file DriveRS485.h
 * @brief DriveRS485读写操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2018.01.17
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _DRIVE_RS485_H
#define _DRIVE_RS485_H
#include "SystemConfig.h"


typedef struct tagQueue
{
    uint8_t* pData;     //指向队列的指针
    uint16_t front;     //队列的头部
    uint16_t rear;      //队列的尾部
}Queue;

void RS485_Init(void);
void RS485_SendMessage(uint8_t* pData, uint16_t dataLen);

#endif

