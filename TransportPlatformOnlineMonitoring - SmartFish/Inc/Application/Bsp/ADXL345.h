/**
 * @file ADXL345.h
 * @brief ADXL345读写操作
 * copyright: Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * company: SOJO
 * @date 2017.12.26
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _ADXL345_H
#define _ADXL345_H
#include "SystemConfig.h"

/**
 * @brief ADXL345寄存器操作指令
 */
#define MAC_ADXL_ID     0xE5
#define DEVICE_ID		0x00 	//!< 器件ID,0XE5
#define THRESH_TAP		0x1D   	//!< 敲击阀值
#define OFSX			0x1E
#define OFSY			0x1F
#define OFSZ			0x20
#define DUR				0x21
#define Latent			0x22
#define Window  		0x23 
#define THRESH_ACK		0x24
#define THRESH_INACT	0x25 
#define TIME_INACT		0x26
#define ACT_INACT_CTL	0x27	 
#define THRESH_FF		0x28	
#define TIME_FF			0x29 
#define TAP_AXES		0x2A  
#define ACT_TAP_STATUS  0x2B 
#define BW_RATE			0x2C 
#define POWER_CTL		0x2D 

#define INT_ENABLE		0x2E
#define INT_MAP			0x2F
#define INT_SOURCE  	0x30
#define DATA_FORMAT	    0x31
#define DATA_X0			0x32
#define DATA_X1			0x33
#define DATA_Y0			0x34
#define DATA_Y1			0x35
#define DATA_Z0			0x36
#define DATA_Z1			0x37
#define FIFO_CTL		0x38
#define FIFO_STATUS		0x39

#define START_ADDRESS	0x32


#define ADXL_FIRST_ADDRESS      0x53
#define ADXL_SECEND_ADDRESS     0x1D

//0X0B TO OX1F Factory Reserved	 
//如果ALT ADDRESS脚(12脚)接地,IIC地址为0X53(不包含最低位).
//如果接V3.3,则IIC地址为0X1D(不包含最低位).
//如果开发板接V3.3,所以转为读写地址后,为0X3B和0X3A(如果接GND,则为0XA7和0XA6)  
#define ADXL_FIRST_ADDRESS_READ     0xA7
#define ADXL_FIRST_ADDRESS_WRITE    0xA6

#define ADXL_SECOND_ADDRESS_READ    0x3B
#define ADXL_SECOND_ADDRESS_WRITE   0x3A

#define ADXL345_FIRST_ID    0x53
#define ADXL345_SECOND_ID   0x1D

uint8_t ADXL345_Init(uint8_t id);
uint8_t ADXL345_Check(uint8_t id);
void ADXL345_ReadTheEntireAxis(int16_t *data, uint8_t id);
void ADXL345_ReadAllAxisAverageValue(float *xAverage, float *yAverage, float *zAverage, uint8_t id);
void ADXL345_AutomaticCalibration(char *xval,char *yval,char *zval, uint8_t id);    //校准函数




#endif

