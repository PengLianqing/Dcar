#include "tim_timer.h"

/*********************************************************
TIM2 ��ʱ�������ʱ�ж�

**********************************************************/

TIM_HandleTypeDef htim2;      
//ͨ�ö�ʱ��2�жϳ�ʼ��,��ʱ��2ʱ��Ϊ160MHz
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
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

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
	}  
}

//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

