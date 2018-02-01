/**
 * @file DriverUSART.c
 * @brief 串口1的操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
#ifndef __USART_H
#define __USART_H

#include "SystemConfig.h" 
#include "stdio.h"	

void Init_UART2(uint32_t bound);
void Init_UART6(uint32_t bound);
extern uint8_t DMA_USART2_Transmit(uint8_t *pData, uint16_t size);
extern uint8_t DMA_USART2_Receive(uint8_t *pRecviveData, uint16_t size);
extern uint8_t StopUSART2_DMA_Send(void);
extern uint8_t WaitSendOrReceive(void);

#endif


