/**
 * @file DriveSPI.h
 * @brief SPI模块初始化，中断操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _DRIVER_SPI_H
#define _DRIVER_SPI_H

#include "SystemConfig.h"


void Init_SPI(void);
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);

#endif

