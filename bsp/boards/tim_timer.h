/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       tim_timer.c/h
  * @brief      FreeRTOS任务
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jan-1-2021      Peng            1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2021 Peng****************************
	*/
#ifndef TIM_TIMER_INIT_H_
#define TIM_TIMER_INIT_H_

#include "main.h"

/**
  * @brief          定时器2句柄
  */  
extern TIM_HandleTypeDef htim2;

/**
  * @brief          定时器初始化
	* 通用定时器2中断初始化,定时器2时钟为160MHz
	* arr：自动重装值。
	* psc：时钟预分频数
	* 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
	* Ft=定时器工作频率,单位:Mhz
  * @param[in]      null
  * @retval         null
  */
void tim2_Init(uint16_t arr,uint16_t psc);

#endif
