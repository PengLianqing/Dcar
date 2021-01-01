/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       control_task.c/h
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

/* Includes ------------------------------------------------------------------*/
#include "control_task.h"

//FreeRTOSͷ�ļ�
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
	xTaskCreate( TaskInitThread, 		/* ������ */
							"vTaskInitThread", 	/* ������ */
							100, 								/* ����ջ��С����λword��Ҳ����4�ֽ� */
							NULL, 							/* ������� */
							0, 									/* �������ȼ�*/
							&xHandleTaskInit ); /* ������ */
	vTaskStartScheduler();          //�����������
}

void TaskInitThread(void *pvParameters)
{
	taskENTER_CRITICAL();           //�����ٽ���
	
	//����remote control����
	xTaskCreate( TaskRemoteCtrlThread, 		/* ������ */
							"vTaskRemoteCtrlThread", 	/* ������ */
							100, 											/* ����ջ��С����λword��Ҳ����4�ֽ� */
							NULL, 										/* ������� */
							0, 												/* �������ȼ�*/
							&xHandleRemoteCtrl ); 		/* ������ */
	
	//����test����
	xTaskCreate( TaskTestThread, 		/* ������ */
							"vTaskTestThread", 	/* ������ */
							100, 											/* ����ջ��С����λword��Ҳ����4�ֽ� */
							NULL, 										/* ������� */
							0, 												/* �������ȼ�*/
							&xHandleTest ); 		/* ������ */
	
	//����soft timer����1000Hz
	SoftTimer1000HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)1,
																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
																(void*)1, //��ʱ��ID
																(TimerCallbackFunction_t)SoftTimer1000HzCallback); //���ڶ�ʱ��������1ms(1��ʱ�ӽ���)������ģʽ
	xTimerStart(SoftTimer1000HzHandle,0); //�������ڶ�ʱ��
	
	//����soft timer����100Hz																
	SoftTimer100HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)10,
																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
																(void*)2, //��ʱ��ID
																(TimerCallbackFunction_t)SoftTimer100HzCallback); //���ڶ�ʱ��������10ms(10��ʱ�ӽ���)������ģʽ
	xTimerStart(SoftTimer100HzHandle,0); //�������ڶ�ʱ��
	
	//����soft timer����10Hz																
	SoftTimer10HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)100,
																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
																(void*)3, //��ʱ��ID
																(TimerCallbackFunction_t)SoftTimer10HzCallback); //���ڶ�ʱ��������100ms(100��ʱ�ӽ���)������ģʽ
	xTimerStart(SoftTimer10HzHandle,0); //�������ڶ�ʱ��													


	vTaskDelete(xHandleTaskInit); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
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
