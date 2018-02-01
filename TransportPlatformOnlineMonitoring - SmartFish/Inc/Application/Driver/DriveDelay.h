/**
 * @file DriveDelay.h
 * @brief 延时函数
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company  SOJO
 * @date 2018.1.7
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef __DRIVER_DELAY_H
#define __DRIVER_DELAY_H

#include "SystemConfig.h" 

void IniutDelay(uint8_t sysclk);
void DelayUs(uint32_t nus);
void DelayMs(uint32_t nms);


#endif


