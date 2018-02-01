/**
 * @file DS18B20.c
 * @brief �¶ȴ�������д����
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.01.26
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#ifndef _DS18B20_H
#define _DS18B20_H
#include "SystemConfig.h"

/***************************************************/
/*                �ο�����ԭ�ӳ���                  */
/***************************************************/

/**
 * @brief IO��������
 */
#define DS18B20_IO_IN()     {GPIOC->MODER&=~(3<<(11*2));GPIOC->MODER|=0<<11*2;}	    //PC12����ģʽ
#define DS18B20_IO_OUT()    {GPIOC->MODER&=~(3<<(11*2));GPIOC->MODER|=1<<11*2;} 	//PC12���ģʽ

/**
 * @brief IO��������	
 */									   
#define	DS18B20_DQ_OUT      PCout(11) //���ݶ˿�	PC12
#define	DS18B20_DQ_IN       PCin(11)  //���ݶ˿�	PC12 

extern uint8_t DS18B20_Init(void);
extern float DS18B20_Get_Temp(void);
uint8_t DS18B20_Check(void);     //���DS18B20�Ƿ���������

#endif

