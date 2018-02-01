/**
 * @file TimeOutDetection.c
 * @brief ʹ��FreeRTOS�е�OSTimeGet������Ƴ�ʱ��⺯��
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

#define WAIT_USART1_DMA_TIME_OUT     500;   //���ô���1��DMA���ͽ��յȴ�ʱ��Ϊ500ms

TimeStampCollect g_TimeStampCollect;    //״̬ʱ��



/**
 * @fn InitTimeOutDetection()
 * @brief ��ʼ����ʱ���ʱ���
 */
void InitTimeOutStampCollect(void)
{    
    g_TimeStampCollect.usart2DmaTime.startTime = UINT32_MAX;                //��������ʱ���
    g_TimeStampCollect.usart2DmaTime.delayTime = WAIT_USART1_DMA_TIME_OUT;
    
}

/**
 * �ж�ʱ���Ƿ�ʱ
 * @brief   �Ƚ�ʱ���Ƿ�ﵽ�趨ֵ����������г�ʱ�ж�
 * �ڲ��Թ����з��֣�ʹ����ʱ��ΪMAX��������жϣ�����
 * ����һ���ж���䣬��ֹ�����������
 * @param   startTime ����ʱ��
 * @param   delayTime ��ʱʱ��
 * @return  0xFF-ʱ�䵽�� 0-ʱ�仹δ����
 *
 */
inline uint8_t IsOverTimeStamp(TimeStamp* pStamp)
{
    uint32_t msTicks = xTaskGetTickCount();     //��ȡ��ǰϵͳʱ�ӽ�����
	if(pStamp->startTime == UINT32_MAX)
	{
		return 0x00;
	}
    if (UINT32_MAX - pStamp->delayTime < pStamp->startTime) //!< �ж��Ƿ����,��������Ƚ����ж��Ƿ񳬳�һ������
    {
        if(msTicks < pStamp->startTime)  //!< ���ж��Ƿ�С��startTime
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



