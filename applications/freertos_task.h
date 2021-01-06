/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       freertos_task.c/h
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

#ifndef FREERTOS_TASK_H
#define FREERTOS_TASK_H

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
//led
#include "bsp_led.h"

/**
  * @brief          freertos初始化
  * @param[in]      null
  * @retval         none
  */
void freertos_init(void);

/**
  * @brief          init task
  * @param[in]      null
  * @retval         none
  */
void init_task(void *pvParameters);

/**
  * @brief          test task
  * @param[in]      null
  * @retval         none
  */
void test_task(void *pvParameters);

/**
  * @brief          USB task
  * @param[in]      null
  * @retval         none
  */
void usb_task(void *pvParameters);

/**
  * @brief          soft timer
  * @param[in]      null
  * @retval         none
  */
void SoftTimer500HzCallback(TimerHandle_t xTimer);
void SoftTimer200HzCallback(TimerHandle_t xTimer);
void SoftTimer50HzCallback(TimerHandle_t xTimer);

#endif

