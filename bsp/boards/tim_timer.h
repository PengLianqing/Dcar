/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       tim_timer.c/h
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
#ifndef TIM_TIMER_INIT_H_
#define TIM_TIMER_INIT_H_

#include "main.h"

/**
  * @brief          ��ʱ��2���
  */  
extern TIM_HandleTypeDef htim2;

/**
  * @brief          ��ʱ����ʼ��
	* ͨ�ö�ʱ��2�жϳ�ʼ��,��ʱ��2ʱ��Ϊ160MHz
	* arr���Զ���װֵ��
	* psc��ʱ��Ԥ��Ƶ��
	* ��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
	* Ft=��ʱ������Ƶ��,��λ:Mhz
  * @param[in]      null
  * @retval         null
  */
void tim2_Init(uint16_t arr,uint16_t psc);

#endif
