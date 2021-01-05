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

/* Includes ------------------------------------------------------------------*/
#include "control_task.h"

/**
  * @brief          freertos初始化
  * @param[in]      null
  * @retval         none
  */
void freertos_init(void)
{
	xTaskCreate( init_task, 		/* 任务函数 */
							"init_task", 	/* 任务名 */
							100, 								/* 任务栈大小，单位word，也就是4字节 */
							NULL, 							/* 任务参数 */
							0, 									/* 任务优先级*/
							&xHandleInit ); /* 任务句柄 */
	vTaskStartScheduler();          //开启任务调度
}

/**
  * @brief          init task
	* 创建任务、软件定时器
  * @param[in]      null
  * @retval         none
  */
void init_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区
	
	//创建remote control任务
	xTaskCreate( usb_task, 		/* 任务函数 */
							"usb_task", 	/* 任务名 */
							256, 											/* 任务栈大小，单位word，也就是4字节 */
							NULL, 										/* 任务参数 */
							2, 												/* 任务优先级，最高为configMAX_PRIORITIES-1，最低位0*/
							&xHandleUSB ); 		/* 任务句柄 */
	
	//创建test任务
	xTaskCreate( test_task, 		/* 任务函数 */
							"test_task", 	/* 任务名 */
							100, 											/* 任务栈大小，单位word，也就是4字节 */
							NULL, 										/* 任务参数 */
							configMAX_PRIORITIES-1, 	/* 任务优先级，最高为configMAX_PRIORITIES-1，最低位0*/
							&xHandleTest ); 		/* 任务句柄 */
	
	//创建soft timer任务500Hz
	SoftTimer500HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)2,
																(UBaseType_t )pdTRUE, //周期定时器
																(void*)1, //定时器ID
																(TimerCallbackFunction_t)SoftTimer500HzCallback); //周期定时器，周期1ms(1个时钟节拍)，周期模式
	xTimerStart(SoftTimer500HzHandle,0); //开启周期定时器
	
	//创建soft timer任务200Hz																
	SoftTimer200HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)5,
																(UBaseType_t )pdTRUE, //周期定时器
																(void*)2, //定时器ID
																(TimerCallbackFunction_t)SoftTimer200HzCallback); //周期定时器，周期10ms(10个时钟节拍)，周期模式
	xTimerStart(SoftTimer200HzHandle,0); //开启周期定时器
	
	//创建soft timer任务50Hz																
	SoftTimer50HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)20,
																(UBaseType_t )pdTRUE, //周期定时器
																(void*)3, //定时器ID
																(TimerCallbackFunction_t)SoftTimer50HzCallback); //周期定时器，周期100ms(100个时钟节拍)，周期模式
	xTimerStart(SoftTimer50HzHandle,0); //开启周期定时器													


	vTaskDelete(xHandleInit); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}

/**
  * @brief          test task
  * @param[in]      null
  * @retval         none
  */
void test_task(void *pvParameters)
{
	while(1)
	{
		led2_toggle();
		vTaskDelay(1000);
	}
}

