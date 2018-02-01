/**
 * @file DriveI2C.h
 * @brief I2C初始化等操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _DRIVER_I2C_H
#define _DRIVER_I2C_H
#include "SystemConfig.h"


void Init_I2C1(void);

extern I2C_HandleTypeDef g_hi2c1;

#endif

