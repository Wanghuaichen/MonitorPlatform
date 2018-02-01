/**
 * @file DriveDelay.h
 * @brief 延时函数
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company  SOJO
 * @date 2018.1.7
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "DriveDelay.h"


static uint32_t Fac_us = 0; //us延时倍乘数

/**
 * @fn IniutDelay()
 * @brief 初始化延迟函数当使用ucos的时候,此函数会初始化ucos的时钟节拍
 * SYSTICK的时钟固定为AHB时钟
 * @prarm SYSCLK  系统时钟频率
 * 
 */
void IniutDelay(uint8_t sysclk)
{
	uint32_t reload;
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);    //SysTick频率为HCLK
	Fac_us = sysclk;						        //不论是否使用OS,fac_us都需要使用
	reload = sysclk;					            //每秒钟的计数次数 单位为K	   
	reload *= 1000000/configTICK_RATE_HZ;		    //根据configTICK_RATE_HZ设定溢出时间
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;      //开启SYSTICK中断
	SysTick->LOAD = reload; 					    //每1/configTICK_RATE_HZ断一次	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;       //开启SYSTICK
}								    

/**
 * @fn DelayUs()
 * @brief 不会引起任务调度的微秒级延时
 * @prarm nus  延时微秒数
 * 0~190887435(最大值即2^32/Fac_us @Fac_us=22.5)	  
 */
void DelayUs(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*Fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};									    
}  

/**
 * @fn DelayMs()
 * @brief 不会引起任务调度的毫秒级延时
 * @prarm nms  延时毫秒数
 */
void DelayMs(uint32_t nms)
{
	uint32_t i = 0;
	for(i = 0; i < nms; i++)
    {
        DelayUs(1000);
    }
}


