/**
 * @file DriveRS485.h
 * @brief DriveRS485��д����
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
    uint8_t* pData;     //ָ����е�ָ��
    uint16_t front;     //���е�ͷ��
    uint16_t rear;      //���е�β��
}Queue;

void RS485_Init(void);
void RS485_SendMessage(uint8_t* pData, uint16_t dataLen);

#endif

