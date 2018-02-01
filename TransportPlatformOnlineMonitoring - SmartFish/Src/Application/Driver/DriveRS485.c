/**
 * @file DriveRS485.c
 * @brief DriveRS485 读写操作
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

/************************ 关于RS485通信相关定义 ***********************/

/**
 * @brief 使用时需要注意配对使用
 */
#define RS485_RX()    (RS485_RE = GPIO_PIN_RESET) //默认置为接收状态
#define RS485_TX()    (RS485_RE = GPIO_PIN_SET)   //置为发送状态

#define QUEUE_DEPTH     128     //队列深度为128字节
/*********************************************************************/

static void DelayNop(void);                     //延时函数
static void InitQueue(Queue* pQueueData);       //初始化队列
static uint8_t QueueFull(Queue* pQueueData);    //检查队列是否充满
static uint8_t QueueEmput(Queue* pQueueData);   //检测队列是否为空
static uint8_t QueurEnter(Queue* pQueueData, uint8_t data);     //将数据入队
static uint8_t QueurOutput(Queue* pQueueData, uint8_t* databuffer, uint16_t dataLen);    //将数据出队
static uint8_t QueurOutputOneWord(Queue* pQueueData, uint8_t* pVal);    //将数据出队


static uint8_t QueueDataBuffer[QUEUE_DEPTH] = {0};

static Queue Formation;   //定义队列

static uint8_t Flag_QueueState = (uint8_t)TRUE;     //队列状态标志位

/**
 * @fn USART6_IRQHandler()
 * @brief 串口6中断服务程序
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
 * @brief 延时几个指令时间
 */
static void DelayNop(void)
{
    uint8_t i = 50;
    while(i--){}
}

/**
 * @fn RS485_Init
 * @brief 初始化485
 */
void RS485_Init(void)
{
    RS485_RX();                 //默认为接收状态
    
    Flag_QueueState = (uint8_t)TRUE;    //无错误
    
    InitQueue(&Formation);      //队列初始化
}

/**
 * @fn fputc
 * @brief 重定义fputc函数 
 */
int fputc(int ch, FILE *f)
{      
    RS485_TX();     //更改为发送状态
    
    DelayNop();     //延时以方便转换
    
		while((USART6->SR & 0x40) == 0) //!< 循环发送,直到发送完毕
    {
    }
    USART6->DR = (uint8_t)ch;     

		while((USART6->SR & 0x40) == 0) //!< 循环发送,直到发送完毕
    {
    }
    
    RS485_RX();   //默认为接收状态 
    
	return ch;
}

/**
 * @fn RS485_SendMessage
 * @brief RS485发送数据
 * @param pData 指向要发送的数组
 * @param dataLen 要发送的数据长度
 */
void RS485_SendMessage(uint8_t* pData, uint16_t dataLen)
{
    uint16_t i = 0;
    RS485_TX();     //更改为发送状态
    
    DelayNop();     //延时以方便转换
    
    for(i = 0; i < dataLen; i++)
    {
        while((USART6->SR & 0x40) == 0) //!< 循环发送,直到发送完毕
        {
        }
        USART6->DR = (uint8_t)pData[i];
    }
    
    while((USART6->SR & 0x40) == 0) //!< 循环发送,直到发送完毕
    {
    }
    
    RS485_RX();   //默认为接收状态
}

/**
 * @fn InitQueue
 * @brief 初始化队列
 * @param pQueueData 指向要初始化的队列
 */
void InitQueue(Queue* pQueueData)
{
    //可以增加内存管理函数，方便以后队列扩展
    pQueueData->pData = QueueDataBuffer;    //队列深度为该数据的长度
    pQueueData->front = 0;                  //队列头
    pQueueData->rear = 0;                   //队列尾部
    
}

/**
 * @fn QueueFull
 * @brief 检测队列是否满
 * @param pQueueData 指向要检查的队列
 * @return 0 未满，其他，队列已满
 */
uint8_t QueueFull(Queue* pQueueData)
{
    if((pQueueData->rear + 1) % QUEUE_DEPTH == pQueueData->front)   //队列满的标志
    {
        return (uint8_t)TRUE;
    }
    return (uint8_t)FALSE;
}

/**
 * @fn QueueEmput
 * @brief 检测队列是否为空的
 * @param pQueueData 指向要检查的队列
 * @return 0 未满，其他，队列已满
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
 * @brief 数据进入队列
 * @param pQueueData 指向要增加数据的队列
 * @param data 要增加的数据
 * @return 0 入队失败，其他，入队成功
 */
uint8_t QueurEnter(Queue* pQueueData, uint8_t data)
{
    if(QueueFull(pQueueData))
    {
        return (uint8_t)FALSE;
    }
    
    pQueueData->pData[pQueueData->rear] = data; //将数据入队
    pQueueData->rear++; //队列尾部加1
    
    return (uint8_t)TRUE;
}

/**
 * @fn QueurOutputOneWord
 * @brief 数据导出队列
 * @param pQueueData 指向要导出数据的队列
 * @param pVal 指向要导出数据的位置
 * @return 0队列为空  其他成功出队
 */
uint8_t QueurOutputOneWord(Queue* pQueueData, uint8_t* pVal)
{    
    if(QueueEmput(pQueueData))      //检测队列是否为空
    {
        return (uint8_t)FALSE;  
    }
    
    *pVal = pQueueData->pData[pQueueData->front];   //出队，从头开始出队将出队的值保存到pVal中
    pQueueData->front ++; 
    
    return (uint8_t)TRUE;
}

/**
 * @fn QueurOutput
 * @brief 将出队的值保存在databuffer中
 * @param pQueueData 指向要导出数据的队列
 * @param pVal 指向要导出数据的位置
 * @param dataLen 指向要导出数据的长度
 * @return 0队列为空  其他成功出队
 */
uint8_t QueurOutput(Queue* pQueueData, uint8_t* pVal, uint16_t dataLen)
{
    uint16_t i = 0;
    if(QueueEmput(pQueueData))      //检测队列是否为空
    {
        return (uint8_t)FALSE;  
    }
    else
    {
        for(i = 0; i < dataLen; i++)    //保存固定长度的数据
        {
            pVal[i] = pQueueData->pData[pQueueData->front];   //出队，从头开始出队将出队的值保存到pVal中
            pQueueData->front ++; 
        }
        return (uint8_t)TRUE;
    }
}





