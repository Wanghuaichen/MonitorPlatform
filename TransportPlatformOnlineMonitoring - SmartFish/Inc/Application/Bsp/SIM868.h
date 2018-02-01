/**
 * @file SIM868.h
 * @brief SIM868读写操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.26
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _SIM868_H
#define _SIM868_H
#include "SystemConfig.h"

#define WAIT_ACK    0xA1
#define WAIT_NO_ACK 0xA2

extern uint8_t Sim868CheckOnline(void);

#endif

