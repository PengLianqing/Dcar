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

//FreeRTOS头文件
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#include "main.h"

//init task
static void TaskInitThread(void *pvParameters);
static TaskHandle_t xHandleTaskInit = NULL;

//test task
static void TaskTestThread(void *pvParameters);
static TaskHandle_t xHandleTest = NULL;

//remote control task
static void TaskRemoteCtrlThread(void *pvParameters);
static TaskHandle_t xHandleRemoteCtrl = NULL;

//M6020 control task
static void TaskMotoCtrlThread(void *pvParameters);
static TaskHandle_t xHandleMotoCtrl = NULL;

//soft timer
TimerHandle_t SoftTimer1000HzHandle;
void SoftTimer1000HzCallback(TimerHandle_t xTimer);

TimerHandle_t SoftTimer100HzHandle;
void SoftTimer100HzCallback(TimerHandle_t xTimer);

TimerHandle_t SoftTimer10HzHandle;
void SoftTimer10HzCallback(TimerHandle_t xTimer);

void freertos_init(void)
{
	xTaskCreate( TaskInitThread, 		/* 任务函数 */
							"vTaskInitThread", 	/* 任务名 */
							100, 								/* 任务栈大小，单位word，也就是4字节 */
							NULL, 							/* 任务参数 */
							0, 									/* 任务优先级*/
							&xHandleTaskInit ); /* 任务句柄 */
	vTaskStartScheduler();          //开启任务调度
}

void TaskInitThread(void *pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区
	
	//创建remote control任务
	xTaskCreate( TaskRemoteCtrlThread, 		/* 任务函数 */
							"vTaskRemoteCtrlThread", 	/* 任务名 */
							100, 											/* 任务栈大小，单位word，也就是4字节 */
							NULL, 										/* 任务参数 */
							0, 												/* 任务优先级*/
							&xHandleRemoteCtrl ); 		/* 任务句柄 */
	
	//创建test任务
	xTaskCreate( TaskTestThread, 		/* 任务函数 */
							"vTaskTestThread", 	/* 任务名 */
							100, 											/* 任务栈大小，单位word，也就是4字节 */
							NULL, 										/* 任务参数 */
							0, 												/* 任务优先级*/
							&xHandleTest ); 		/* 任务句柄 */
	
	//创建soft timer任务1000Hz
	SoftTimer1000HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)1,
																(UBaseType_t )pdTRUE, //周期定时器
																(void*)1, //定时器ID
																(TimerCallbackFunction_t)SoftTimer1000HzCallback); //周期定时器，周期1ms(1个时钟节拍)，周期模式
	xTimerStart(SoftTimer1000HzHandle,0); //开启周期定时器
	
	//创建soft timer任务100Hz																
	SoftTimer100HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)10,
																(UBaseType_t )pdTRUE, //周期定时器
																(void*)2, //定时器ID
																(TimerCallbackFunction_t)SoftTimer100HzCallback); //周期定时器，周期10ms(10个时钟节拍)，周期模式
	xTimerStart(SoftTimer100HzHandle,0); //开启周期定时器
	
	//创建soft timer任务10Hz																
	SoftTimer10HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)100,
																(UBaseType_t )pdTRUE, //周期定时器
																(void*)3, //定时器ID
																(TimerCallbackFunction_t)SoftTimer10HzCallback); //周期定时器，周期100ms(100个时钟节拍)，周期模式
	xTimerStart(SoftTimer10HzHandle,0); //开启周期定时器													


	vTaskDelete(xHandleTaskInit); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}


void TaskTestThread(void *pvParameters)
{
	while(1)
	{
		led2_toggle();
		vTaskDelay(1000);
	}
}

void TaskRemoteCtrlThread(void *pvParameters)
{
	while(1)
	{
		led3_toggle();
		vTaskDelay(100);
	}
}
