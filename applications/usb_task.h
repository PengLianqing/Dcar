/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       usb_task.c/h
  * @brief      FreeRTOS USB任务
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

#ifndef USB_TASK_H
#define USB_TASK_H

#include "bsp_can.h"
#include "control_isr.h"
#include "bsp_rc.h"
#include "pid.h"
typedef struct
{
  const RC_ctrl_t *chassis_RC;              //遥控器指针
	const motor_measure_t *motor_measure;			//电机传感器数据指针
	const moto_position_t *moto_position;			//电机控制指针
	const pid_t *pid_position;
	const pid_t *pid_velocity;			//pid控制指针
} volt_transmit_t; //volt+数据指针

/**
  * @brief          usb task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void usb_task(void *pvParameters);

/**
  * @brief          usb printf
  * @param[in]      用法同printf()
  * @retval         none
  */
void usb_printf(const char *fmt,...);

/**
  * @brief          usb的中断服务函数
  * @param[in]      none
  * @retval         none
  */
void OTG_FS_IRQHandler(void);

/**
  * @brief          初始化"volt_transmit"变量，包括pid指针， 遥控器指针，电机数据指针等，传给上位机volt+.
  * @param[out]     volt_init:"volt_transmit"变量指针.
  * @retval         none
  */
static void volt_init(volt_transmit_t *volt_transmit);
	
#endif

