/**
 * @file DriveIWDG.h
 * @brief 独立看门狗初始化操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
 
#ifndef _DRIVER_IWDG_H
#define _DRIVER_IWDG_H
#include "SystemConfig.h"

/**
 * @brief 喂独立看门狗
 */
#define IWDG_Feed() HAL_IWDG_Refresh(&hiwdg)

void Init_IWDG(void);

#endif

