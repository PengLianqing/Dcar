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
#include "tim_timer.h"

/**
  * @brief          ��ʱ��2���
  */
TIM_HandleTypeDef htim2;      

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
void tim2_Init(uint16_t arr,uint16_t psc)
{  
    htim2.Instance=TIM2;                          //ͨ�ö�ʱ��3
    htim2.Init.Prescaler=psc;                     //��Ƶ
    htim2.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    htim2.Init.Period=arr;                        //�Զ�װ��ֵ
    htim2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&htim2);
    
    HAL_TIM_Base_Start_IT(&htim2); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE    
}

/**
  * @brief          HAL_TIM_Base_MspInit
	* ��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
	* �˺����ᱻHAL_TIM_Base_Init()��������
  * @param[in]      null
  * @retval         null
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
	}  
}

/**
  * @brief          ��ʱ��2�жϷ�����
  * @param[in]      null
  * @retval         null
  */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

/**
  * @brief          ��ʱ���жϴ�����
  * @param[in]      null
  * @retval         null
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static int time=0;
	if(htim==(&htim2))
	{
		if(++time>=1000 ) time=0;
		/****************************1000HZ***********************/
		/****************************1000HZ***********************/
		if(time%2==0)
		{
		/***************************500HZ***********************/
		/***************************500HZ***********************/
		}
		if(time%5==0)
		{
		/***************************200HZ***********************/		
		/***************************200HZ***********************/
		}
		if(time%10==0)
		{
		/***************************100HZ***********************/
		/***************************100HZ***********************/
		}
		if(time%100==0)
		{
		/***************************10HZ************************/
		/***************************10HZ************************/
		}
	}
}










