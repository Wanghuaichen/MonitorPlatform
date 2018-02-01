/**
 * @file TimeOutDetection.c
 * @brief 使用FreeRTOS中的OSTimeGet函数设计超时检测函数
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.1.9
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "TimeOutDetection.h"
/**
 * @brief FreeRTOS Includes
 */
#include "FreeRTOS.h"
#include "task.h"

#define WAIT_USART1_DMA_TIME_OUT     500;   //设置串口1的DMA发送接收等待时间为500ms

TimeStampCollect g_TimeStampCollect;    //状态时间



/**
 * @fn InitTimeOutDetection()
 * @brief 初始化超时检测时间戳
 */
void InitTimeOutStampCollect(void)
{    
    g_TimeStampCollect.usart2DmaTime.startTime = UINT32_MAX;                //不开启超时检测
    g_TimeStampCollect.usart2DmaTime.delayTime = WAIT_USART1_DMA_TIME_OUT;
    
}

/**
 * 判断时间是否超时
 * @brief   比较时间是否达到设定值，对溢出进行超时判断
 * 在测试过程中发现，使启动时间为MAX会产生误判断，所以
 * 加入一段判读语句，防止该情况发生。
 * @param   startTime 启动时间
 * @param   delayTime 延时时间
 * @return  0xFF-时间到达 0-时间还未到达
 *
 */
inline uint8_t IsOverTimeStamp(TimeStamp* pStamp)
{
    uint32_t msTicks = xTaskGetTickCount();     //获取当前系统时钟节拍数
	if(pStamp->startTime == UINT32_MAX)
	{
		return 0x00;
	}
    if (UINT32_MAX - pStamp->delayTime < pStamp->startTime) //!< 判断是否溢出,若溢出则先进行判断是否超出一个周期
    {
        if(msTicks < pStamp->startTime)  //!< 先判断是否小于startTime
        {
            if (msTicks>= (pStamp->startTime + pStamp->delayTime))
            {
                return 0xFF;
            }
        }
    }
    else
    {
        if (msTicks >= pStamp->startTime + pStamp->delayTime)
        {
            return 0xFF;
        }                
    }
    return 0;
}



