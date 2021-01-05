/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       control_isr.c/h
  * @brief      FreeRTOS����
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
#ifndef CONTROL_ISR_H_
#define CONTROL_ISR_H_

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
  * @brief          ��ʼ��pidָ��
  * @param[in]      pidָ�롢����
  * @retval         null
  */
void control_init(void);

/**
  * @brief          ������ԭʼ����
  * @param[in]      �����������ָ��, ���ԭʼ����ָ��
  * @retval         null
  */
void moto_control_data_process(moto_position_t *moto,motor_measure_t *ptr);

/**
  * @brief          freertos 200hz �����ʱ������
	* ʵ��ң�����ݵ�ƽ�����任����λ����
  * @param[in]      null
  * @retval         null
  */
void SoftTimer200HzCallback(TimerHandle_t xTimer);

/**
  * @brief          freertos 500hz �����ʱ������
	* ʵ�ֵ������
  * @param[in]      null
  * @retval         null
  */
void SoftTimer500HzCallback(TimerHandle_t xTimer);
	
/**
  * @brief          ���ص����������ָ��
  * @param[in]      none
  * @retval         �����������ָ��
  */
const moto_position_t *get_moto1_position_point(void);

/**
  * @brief          ����pid����ָ��
  * @param[in]      none
  * @retval         pid����ָ��
  */
pid_t *get_moto1_position_pid_point(void);
pid_t *get_moto1_velocity_pid_point(void);
	
#endif
