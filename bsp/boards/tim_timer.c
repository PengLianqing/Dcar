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
#include "tim_timer.h"

/**
  * @brief          定时器2句柄
  */
TIM_HandleTypeDef htim2;      

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
void tim2_Init(uint16_t arr,uint16_t psc)
{  
    htim2.Instance=TIM2;                          //通用定时器3
    htim2.Init.Prescaler=psc;                     //分频
    htim2.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    htim2.Init.Period=arr;                        //自动装载值
    htim2.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&htim2);
    
    HAL_TIM_Base_Start_IT(&htim2); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE    
}

/**
  * @brief          HAL_TIM_Base_MspInit
	* 定时器底册驱动，开启时钟，设置中断优先级
	* 此函数会被HAL_TIM_Base_Init()函数调用
  * @param[in]      null
  * @retval         null
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	}  
}

/**
  * @brief          定时器2中断服务函数
  * @param[in]      null
  * @retval         null
  */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

/**
  * @brief          定时器中断处理函数
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










