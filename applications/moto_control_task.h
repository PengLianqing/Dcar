/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       moto_control_task.c/h
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
#ifndef MOTO_CONTROL_TASK_H_
#define MOTO_CONTROL_TASK_H_
//can
#include "bsp_can.h"

//pid
#include "math.h"
#include "pid.h"

//usb printf
#include "usb_task.h"

//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/**
  * @brief          初始化pid指针
  * @param[in]      pid指针、参数
  * @retval         null
  */
void control_init(void);

/**
  * @brief          处理电机原始数据
  * @param[in]      电机控制数据指针, 电机原始数据指针
  * @retval         null
  */
void moto_control_data_process(moto_position_t *moto,motor_measure_t *ptr);

/**
  * @brief          freertos 200hz 软件定时器任务
	* 实现遥控数据的平滑、变换、限位处理
  * @param[in]      null
  * @retval         null
  */
void SoftTimer200HzCallback(TimerHandle_t xTimer);

/**
  * @brief          freertos 500hz 软件定时器任务
	* 实现电机控制
  * @param[in]      null
  * @retval         null
  */
void SoftTimer500HzCallback(TimerHandle_t xTimer);
	
/**
  * @brief          返回电机控制数据指针
  * @param[in]      none
  * @retval         电机控制数据指针
  */
const moto_position_t *get_moto1_position_point(void);

/**
  * @brief          返回pid数据指针
  * @param[in]      none
  * @retval         pid数据指针
  */
pid_t *get_moto1_position_pid_point(void);
pid_t *get_moto1_velocity_pid_point(void);
	
#endif
