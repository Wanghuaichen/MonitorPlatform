/**
 * @file SIM868.c
 * @brief SIM868读写操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.1.7
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "SIM868.h"
#include "DriveUSART.h"
#include "DriveDelay.h"

#include "stdarg.h"	
#include "string.h"	

//串口发送缓存区 

#define USART1_MAX_SEND_LEN		1024				    //最大发送缓存字节数
__align(8) uint8_t USART2_TX_BUF[USART1_MAX_SEND_LEN]; 	//发送缓冲,最大USART1_MAX_SEND_LEN字节
static uint8_t USART2_RX_BUF[USART1_MAX_SEND_LEN]; 	    //接收缓冲,最大USART1_MAX_SEND_LEN字节


static void Usart2_DMA_Printf(char* fmt,...);
static uint8_t* Sim868CheckCmd(uint8_t* str);
static uint8_t Sim868CharToHex(uint8_t hex);
static uint8_t Sim868HexToChar(uint8_t hex);
static uint8_t Sim868SendCmd(uint8_t* cmd, uint8_t* ack, uint16_t waitTime, uint8_t mode);

/**
 * @fn SendPrintf
 * @brief 串口1的DMA printf函数，格式化发送字符
 * @param cmd 发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串
 */
void Usart2_DMA_Printf(char* fmt,...)
{
    uint8_t state = 0;
    va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF, fmt, ap);
	va_end(ap);
    state = DMA_USART2_Transmit(USART2_TX_BUF, strlen((const char*)USART2_TX_BUF)); //通过DMA发送
    if(state)   //重新发送一次
    {
        DMA_USART2_Transmit(USART2_TX_BUF, strlen((const char*)USART2_TX_BUF)); //通过DMA发送
    }
}

/**
 * @fn Sim868CheckCmd
 * @brief SIM868模块命令发送后，检测接收到的应答
 * @param str 期待的应答结果
 * @return 0,没有得到期待的应答结果
 *         其他,期待应答结果的位置(str的位置)
 */
uint8_t* Sim868CheckCmd(uint8_t* str)
{
    char *strx=0;
    uint16_t len = strlen((const char*)str) + 2;
    DMA_USART2_Receive(USART2_RX_BUF, len);
	strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
    
	return (uint8_t*)strx;
}

/**
 * @fn Sim868CharToHex
 * @brief 将1个字符转换为16进制数字
 * @param chr 字符,0~9/A~F/a~F
 * @return chr对应的16进制数值
 */
uint8_t Sim868CharToHex(uint8_t chr)
{
    if(chr>='0' && chr<='9')
    {
        return (chr-'0');
    }
	else if((chr>='A') && (chr<='F'))
    {
        return (chr-'A'+10);
    }
	else if((chr>='a') && (chr<='f'))
    {
        return (chr-'a'+10); 
    }
	return 0;
}

/**
 * @fn Sim868HexToChar
 * @brief 将1个16进制数字转换为字符
 * @param hex  16进制数字,0~15;
 * @return 字符
 */
uint8_t Sim868HexToChar(uint8_t hex)
{
    if(hex <= 9)
    {
        return hex+'0';
    }
	else if((hex >= 10) && (hex <= 15))
    {
        return (hex-10+'A');
    }
	return '0';
}

/**
 * @fn Sim868SendCmd
 * @brief SIM868模块命令发送
 * 使用DMA发送
 * @param cmd 发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串
 * @param ack 期待的应答结果,如果为空,则表示不需要等待应答
 * @param waitTime 等待时间(单位:10ms)
 * @param mode 选择模式，0xA1等待应答模式 0xA2不等待应答模式(单位:10ms)
 * @return 0,成功停止返回(得到了期待的应答结果)
 *         1,否则返回其他
 */
uint8_t Sim868SendCmd(uint8_t* cmd, uint8_t* ack, uint16_t waitTime, uint8_t mode)
{
    uint8_t result = 0;
    if((uint32_t)cmd <= 0xFF)
    {
        DMA_USART2_Transmit(cmd, 1);    //只发送数字，且长度为1
    }
    else
    {
        Usart2_DMA_Printf("%s\r\n",cmd);
    }
    WaitSendOrReceive();    //等待发送完成
    if(mode == WAIT_NO_ACK) //无应答模式s    
    {
        return 0;
    }
    if(ack && waitTime)     //等待应答
    {
        DelayMs(10);
        while(--waitTime)
        {
            if(Sim868CheckCmd(ack))
            {
                break;
            }
            if(WaitSendOrReceive() == 1)    //等待接收完成
            {
                waitTime = 0;   //超时跳出
                break;
            }
        }
		if(!waitTime)
        {
            result = 1;     //超时
        }
    }
    
    return result;
}

/**
 * @fn Sim868CheckOnline
 * @brief 检查SIM868在线状态
 * @return 返回状态
 *          0：正常响应
 *          1：通信不上
 *          2：没有SIM卡
 *          3：等待附着到网络
 */
uint8_t SIM900_CSQ[3];
uint8_t Sim868CheckOnline(void)
{
    if(Sim868SendCmd((uint8_t *)"AT", (uint8_t *)"OK", 100, WAIT_ACK))
	{
        return 1;	//通信不上
	}		
	if(Sim868SendCmd((uint8_t *)"AT+CPIN?", (uint8_t *)"READY", 200, WAIT_ACK))
    {
        return 2;	//没有SIM卡
    }
	if(Sim868SendCmd((uint8_t *)"AT+CREG?", (uint8_t *)"0,1", 200, WAIT_ACK))
	{
		 if(!Sim868SendCmd((uint8_t *)"AT+CSQ", (uint8_t *)"OK", 100, WAIT_ACK))	
		 {
             memcpy(SIM900_CSQ, USART2_RX_BUF+15, 2);
		 }
		 return 3;	//等待附着到网络
	}	
	return 0;
}



