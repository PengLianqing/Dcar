/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       usb_task.c/h
  * @brief      FreeRTOS USB����
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jan-1-2021      Peng            1. ���
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
  const RC_ctrl_t *chassis_RC;              //ң����ָ��
	const motor_measure_t *motor_measure;			//�������������ָ��
	const moto_position_t *moto_position;			//�������ָ��
	const pid_t *pid_position;
	const pid_t *pid_velocity;			//pid����ָ��
} volt_transmit_t; //volt+����ָ��

/**
  * @brief          usb task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void usb_task(void *pvParameters);

/**
  * @brief          usb printf
  * @param[in]      �÷�ͬprintf()
  * @retval         none
  */
void usb_printf(const char *fmt,...);

/**
  * @brief          usb���жϷ�����
  * @param[in]      none
  * @retval         none
  */
void OTG_FS_IRQHandler(void);

/**
  * @brief          ��ʼ��"volt_transmit"����������pidָ�룬 ң����ָ�룬�������ָ��ȣ�������λ��volt+.
  * @param[out]     volt_init:"volt_transmit"����ָ��.
  * @retval         none
  */
static void volt_init(volt_transmit_t *volt_transmit);
	
#endif

