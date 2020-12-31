#include "tim_timer.h"

/*********************************************************
TIM2 定时器输出定时中断

**********************************************************/

TIM_HandleTypeDef htim2;      
//通用定时器2中断初始化,定时器2时钟为160MHz
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
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

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	}  
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

