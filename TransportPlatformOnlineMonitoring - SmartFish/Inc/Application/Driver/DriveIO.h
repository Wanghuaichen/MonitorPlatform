/**
 * @file DriveIO.h
 * @brief IO初始化操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _DRIVER_IO_H
#define _DRIVER_IO_H
#include "SystemConfig.h"


#define F_CS        PBout(1)    //Flash片选信号
#define RS485_RE    PAout(10)   //RS485接收

#define EN_D12V     PBout(14)   //数字12V电源驱动
#define EN_D5V      PBout(15)   //数字5V电源驱动
#define EN_D3V      PBout(10)   //数字3V电源驱动

#define RUN_LED     PCout(12)   //运行指示灯
#define SD_CS       PCout(4)    //SD卡片选信号

#define RELAY1      PCout(10)   //继电器1
#define RELAY2      PAout(9)    //继电器2
#define RELAY3      PAout(8)    //继电器3

#define TEMP        PCout(11)   //温度传感器

void Init_DriveIO(void);

#endif

