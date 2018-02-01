/**
 * @file DriveTIM.c
 * @brief 定时器初始化，中断操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.07
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "FreeRTOS.h"
#include "task.h"
#include "DriveTIM.h"
#include "DriveIO.h"

/**
 * @fn Init_TIM3
 * @brief 通用定时器3初始化
 */
static void Init_TIM3(uint16_t arr,uint16_t psc);

TIM_HandleTypeDef htim2;    //!< 定时器2句柄
TIM_HandleTypeDef htim3;    //!< 定时器2句柄

volatile unsigned long long FreeRTOSRunTimeTicks;

void ConfigureTimeForRunTimeStats(void)
{
	FreeRTOSRunTimeTicks=0;
	Init_TIM3(50,84);   //!< (50*84)/84M s
}

/**
 * @fn Init_TIM2
 * @brief 通用定时器2初始化
 * @param arr 自动重装值
 * @param psc 时钟预分频系数
 * @param time 定时器名称
 * @note 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft (s)
 *       Ft=定时器工作频率,单位:Mhz
 */
void Init_TIM2(uint16_t arr,uint16_t psc)   
{
    
    htim2.Instance = TIM2;  
    htim2.Init.Prescaler = (psc - 1);                   //!< 分频系数
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;        //!< 向上计数器
    htim2.Init.Period = (arr - 1);                      //!< 自动装载值
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //!< 时钟分频因子
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    HAL_TIMEx_HallSensor_Start_IT(&htim2);              //!< 使能定时器2和定时器2更新中断：TIM_IT_UPDATE  
}

/**
 * @fn Init_TIM3
 * @brief 通用定时器3初始化
 * @param arr 自动重装值
 * @param psc 时钟预分频系数
 * @param time 定时器名称
 * @note 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft (s)
 *       Ft=定时器工作频率,单位:Mhz
 */
void Init_TIM3(uint16_t arr,uint16_t psc)   
{
    
    htim3.Instance = TIM3;  
    htim3.Init.Prescaler = (psc - 1);                   //!< 分频系数
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;        //!< 向上计数器
    htim3.Init.Period = (arr - 1);                      //!< 自动装载值
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //!< 时钟分频因子
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    HAL_TIMEx_HallSensor_Start_IT(&htim3);              //!< 使能定时器3和定时器3更新中断：TIM_IT_UPDATE  
}

/**
 * @fn HAL_TIM_Base_MspInit
 * @brief 定时器底册驱动，开启时钟，设置中断优先级
 * 此函数会被HAL_TIM_Base_Init()函数调用
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //!< 使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn, 8, 0);  //!< 设置中断优先级，抢占优先级8，子优先级0
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //!< 开启ITM2中断   
	}
    else if(htim->Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //!< 使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn, 13, 0); //!< 设置中断优先级，抢占优先级13，子优先级0
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //!< 开启ITM3中断   
	}
}

/**
 * @fn TIM2_IRQHandler
 * @brief 定时器2中断服务函数
 * 
 */
void TIM2_IRQHandler(void)
{
    //加入临界区
	HAL_TIM_IRQHandler(&htim2);
}
/**
 * @fn TIM2_IRQHandler
 * @brief 定时器2中断服务函数
 * 
 */
void TIM3_IRQHandler(void)
{
    //加入临界区
	HAL_TIM_IRQHandler(&htim3);
}
/**
 * @fn HAL_TIM_PeriodElapsedCallback
 * @brief 回调函数，定时器中断服务函数调用
 * 
 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&htim2))
    {
       //!< LED3状态切换
    }
    else if(htim==(&htim3))
    {
        FreeRTOSRunTimeTicks++;
    }
}
