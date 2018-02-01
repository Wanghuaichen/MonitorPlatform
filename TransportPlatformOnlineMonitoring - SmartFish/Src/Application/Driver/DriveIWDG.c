/**
 * @file DriveIWDG.c
 * @brief 独立看门狗初始化操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2017.12.07
 *
 * @note 设置看门狗的方式可以采用如下方式：初始化一个具有最高优先级的任务，
 *       该任务主要作用就是进行喂狗操作，根据其他任务的反馈的信号量进行喂
 *       狗，只有所有的任务的信号量都到达才可以执行喂狗操作
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
 
#include "DriveIWDG.h"

IWDG_HandleTypeDef hiwdg;

/**
 * @fn Init_IWDG
 * @brief 初始化独立看门狗
 * @note 时间计算(大概):Tout=((4*2^Prescaler)*Reload)/32 (ms).
 */
void Init_IWDG(void)
{
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_64;   //!< 分频系数为64
    hiwdg.Init.Reload = 500;                    //!< 重装载值
    if (HAL_IWDG_Init(&hiwdg) != HAL_OK)        //!< 初始化IWDG,默认会开启独立看门狗
    {
        _Error_Handler(__FILE__, __LINE__);
    }
    //__HAL_IWDG_START(&hiwdg);                   //!< 启动看门狗
}

