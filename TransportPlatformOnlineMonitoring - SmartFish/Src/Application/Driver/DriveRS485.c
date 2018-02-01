/**
 * @file DriveRS485.c
 * @brief DriveRS485 ��д����
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.01.17
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "DriveRS485.h"
#include "DriveUSART.h"	
#include "DriveIO.h"

/************************ ����RS485ͨ����ض��� ***********************/

/**
 * @brief ʹ��ʱ��Ҫע�����ʹ��
 */
#define RS485_RX()    (RS485_RE = GPIO_PIN_RESET) //Ĭ����Ϊ����״̬
#define RS485_TX()    (RS485_RE = GPIO_PIN_SET)   //��Ϊ����״̬

#define QUEUE_DEPTH     128     //�������Ϊ128�ֽ�
/*********************************************************************/

static void DelayNop(void);                     //��ʱ����
static void InitQueue(Queue* pQueueData);       //��ʼ������
static uint8_t QueueFull(Queue* pQueueData);    //�������Ƿ����
static uint8_t QueueEmput(Queue* pQueueData);   //�������Ƿ�Ϊ��
static uint8_t QueurEnter(Queue* pQueueData, uint8_t data);     //���������
static uint8_t QueurOutput(Queue* pQueueData, uint8_t* databuffer, uint16_t dataLen);    //�����ݳ���
static uint8_t QueurOutputOneWord(Queue* pQueueData, uint8_t* pVal);    //�����ݳ���


static uint8_t QueueDataBuffer[QUEUE_DEPTH] = {0};

static Queue Formation;   //�������

static uint8_t Flag_QueueState = (uint8_t)TRUE;     //����״̬��־λ

/**
 * @fn USART6_IRQHandler()
 * @brief ����6�жϷ������
 * 
 */
void USART6_IRQHandler(void)                	
{	
	if(USART6->SR & (1 << 5))
	{
        Flag_QueueState = QueurEnter(&Formation, USART6->DR);
	}
}

/**
 * @fn DelayNop
 * @brief ��ʱ����ָ��ʱ��
 */
static void DelayNop(void)
{
    uint8_t i = 50;
    while(i--){}
}

/**
 * @fn RS485_Init
 * @brief ��ʼ��485
 */
void RS485_Init(void)
{
    RS485_RX();                 //Ĭ��Ϊ����״̬
    
    Flag_QueueState = (uint8_t)TRUE;    //�޴���
    
    InitQueue(&Formation);      //���г�ʼ��
}

/**
 * @fn fputc
 * @brief �ض���fputc���� 
 */
int fputc(int ch, FILE *f)
{      
    RS485_TX();     //����Ϊ����״̬
    
    DelayNop();     //��ʱ�Է���ת��
    
		while((USART6->SR & 0x40) == 0) //!< ѭ������,ֱ���������
    {
    }
    USART6->DR = (uint8_t)ch;     

		while((USART6->SR & 0x40) == 0) //!< ѭ������,ֱ���������
    {
    }
    
    RS485_RX();   //Ĭ��Ϊ����״̬ 
    
	return ch;
}

/**
 * @fn RS485_SendMessage
 * @brief RS485��������
 * @param pData ָ��Ҫ���͵�����
 * @param dataLen Ҫ���͵����ݳ���
 */
void RS485_SendMessage(uint8_t* pData, uint16_t dataLen)
{
    uint16_t i = 0;
    RS485_TX();     //����Ϊ����״̬
    
    DelayNop();     //��ʱ�Է���ת��
    
    for(i = 0; i < dataLen; i++)
    {
        while((USART6->SR & 0x40) == 0) //!< ѭ������,ֱ���������
        {
        }
        USART6->DR = (uint8_t)pData[i];
    }
    
    while((USART6->SR & 0x40) == 0) //!< ѭ������,ֱ���������
    {
    }
    
    RS485_RX();   //Ĭ��Ϊ����״̬
}

/**
 * @fn InitQueue
 * @brief ��ʼ������
 * @param pQueueData ָ��Ҫ��ʼ���Ķ���
 */
void InitQueue(Queue* pQueueData)
{
    //���������ڴ�������������Ժ������չ
    pQueueData->pData = QueueDataBuffer;    //�������Ϊ�����ݵĳ���
    pQueueData->front = 0;                  //����ͷ
    pQueueData->rear = 0;                   //����β��
    
}

/**
 * @fn QueueFull
 * @brief �������Ƿ���
 * @param pQueueData ָ��Ҫ���Ķ���
 * @return 0 δ������������������
 */
uint8_t QueueFull(Queue* pQueueData)
{
    if((pQueueData->rear + 1) % QUEUE_DEPTH == pQueueData->front)   //�������ı�־
    {
        return (uint8_t)TRUE;
    }
    return (uint8_t)FALSE;
}

/**
 * @fn QueueEmput
 * @brief �������Ƿ�Ϊ�յ�
 * @param pQueueData ָ��Ҫ���Ķ���
 * @return 0 δ������������������
 */
uint8_t QueueEmput(Queue* pQueueData)
{
    if(pQueueData->rear == pQueueData->front)
    {
        return (uint8_t)TRUE;
    }
    return (uint8_t)FALSE;
}

/**
 * @fn QueurEnter
 * @brief ���ݽ������
 * @param pQueueData ָ��Ҫ�������ݵĶ���
 * @param data Ҫ���ӵ�����
 * @return 0 ���ʧ�ܣ���������ӳɹ�
 */
uint8_t QueurEnter(Queue* pQueueData, uint8_t data)
{
    if(QueueFull(pQueueData))
    {
        return (uint8_t)FALSE;
    }
    
    pQueueData->pData[pQueueData->rear] = data; //���������
    pQueueData->rear++; //����β����1
    
    return (uint8_t)TRUE;
}

/**
 * @fn QueurOutputOneWord
 * @brief ���ݵ�������
 * @param pQueueData ָ��Ҫ�������ݵĶ���
 * @param pVal ָ��Ҫ�������ݵ�λ��
 * @return 0����Ϊ��  �����ɹ�����
 */
uint8_t QueurOutputOneWord(Queue* pQueueData, uint8_t* pVal)
{    
    if(QueueEmput(pQueueData))      //�������Ƿ�Ϊ��
    {
        return (uint8_t)FALSE;  
    }
    
    *pVal = pQueueData->pData[pQueueData->front];   //���ӣ���ͷ��ʼ���ӽ����ӵ�ֵ���浽pVal��
    pQueueData->front ++; 
    
    return (uint8_t)TRUE;
}

/**
 * @fn QueurOutput
 * @brief �����ӵ�ֵ������databuffer��
 * @param pQueueData ָ��Ҫ�������ݵĶ���
 * @param pVal ָ��Ҫ�������ݵ�λ��
 * @param dataLen ָ��Ҫ�������ݵĳ���
 * @return 0����Ϊ��  �����ɹ�����
 */
uint8_t QueurOutput(Queue* pQueueData, uint8_t* pVal, uint16_t dataLen)
{
    uint16_t i = 0;
    if(QueueEmput(pQueueData))      //�������Ƿ�Ϊ��
    {
        return (uint8_t)FALSE;  
    }
    else
    {
        for(i = 0; i < dataLen; i++)    //����̶����ȵ�����
        {
            pVal[i] = pQueueData->pData[pQueueData->front];   //���ӣ���ͷ��ʼ���ӽ����ӵ�ֵ���浽pVal��
            pQueueData->front ++; 
        }
        return (uint8_t)TRUE;
    }
}





