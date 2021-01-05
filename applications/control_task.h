/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       control_task.c/h
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

//FreeRTOS头文件
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
//led
#include "bsp_led.h"

/**
  * @brief          init task
  * @param[in]      null
  * @retval         none
  */
void init_task(void *pvParameters);
TaskHandle_t xHandleInit = NULL;

/**
  * @brief          test task
  * @param[in]      null
  * @retval         none
  */
void test_task(void *pvParameters);
TaskHandle_t xHandleTest = NULL;

/**
  * @brief          USB task
  * @param[in]      null
  * @retval         none
  */
void usb_task(void *pvParameters);
TaskHandle_t xHandleUSB = NULL;

/**
  * @brief          soft timer
  * @param[in]      null
  * @retval         none
  */
TimerHandle_t SoftTimer500HzHandle;
void SoftTimer500HzCallback(TimerHandle_t xTimer);

TimerHandle_t SoftTimer200HzHandle;
void SoftTimer200HzCallback(TimerHandle_t xTimer);

TimerHandle_t SoftTimer50HzHandle;
void SoftTimer50HzCallback(TimerHandle_t xTimer);

#endif

