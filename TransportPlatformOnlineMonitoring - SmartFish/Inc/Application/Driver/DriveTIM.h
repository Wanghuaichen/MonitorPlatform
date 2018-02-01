/**
 * @file DriveTIM.h
 * @brief IO初始化操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.09.06
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _DRIVER_TIM_H
#define _DRIVER_TIM_H
#include "SystemConfig.h"

void Init_TIM2(uint16_t arr,uint16_t psc);
void ConfigureTimeForRunTimeStats(void);
extern volatile unsigned long long FreeRTOSRunTimeTicks;

#endif

