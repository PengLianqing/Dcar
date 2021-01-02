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
void init_task(void *pvParameters);
TaskHandle_t xHandleInit = NULL;

//test task
void test_task(void *pvParameters);
TaskHandle_t xHandleTest = NULL;

//USB task
void usb_task(void *pvParameters);
TaskHandle_t xHandleUSB = NULL;

//soft timer
TimerHandle_t SoftTimer1000HzHandle;
void SoftTimer1000HzCallback(TimerHandle_t xTimer);

TimerHandle_t SoftTimer100HzHandle;
void SoftTimer100HzCallback(TimerHandle_t xTimer);

TimerHandle_t SoftTimer10HzHandle;
void SoftTimer10HzCallback(TimerHandle_t xTimer);

void freertos_init(void)
{
	xTaskCreate( init_task, 		/* ������ */
							"init_task", 	/* ������ */
							100, 								/* ����ջ��С����λword��Ҳ����4�ֽ� */
							NULL, 							/* ������� */
							0, 									/* �������ȼ�*/
							&xHandleInit ); /* ������ */
	vTaskStartScheduler();          //�����������
}

void init_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //�����ٽ���
	
	//����remote control����
	xTaskCreate( usb_task, 		/* ������ */
							"usb_task", 	/* ������ */
							128, 											/* ����ջ��С����λword��Ҳ����4�ֽ� */
							NULL, 										/* ������� */
							2, 												/* �������ȼ������ΪconfigMAX_PRIORITIES-1�����λ0*/
							&xHandleUSB ); 		/* ������ */
	
//	//����test����
//	xTaskCreate( test_task, 		/* ������ */
//							"test_task", 	/* ������ */
//							100, 											/* ����ջ��С����λword��Ҳ����4�ֽ� */
//							NULL, 										/* ������� */
//							configMAX_PRIORITIES-1, 	/* �������ȼ������ΪconfigMAX_PRIORITIES-1�����λ0*/
//							&xHandleTest ); 		/* ������ */
//	
//	//����soft timer����1000Hz
//	SoftTimer1000HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)1,
//																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
//																(void*)1, //��ʱ��ID
//																(TimerCallbackFunction_t)SoftTimer1000HzCallback); //���ڶ�ʱ��������1ms(1��ʱ�ӽ���)������ģʽ
//	xTimerStart(SoftTimer1000HzHandle,0); //�������ڶ�ʱ��
//	
//	//����soft timer����100Hz																
//	SoftTimer100HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)10,
//																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
//																(void*)2, //��ʱ��ID
//																(TimerCallbackFunction_t)SoftTimer100HzCallback); //���ڶ�ʱ��������10ms(10��ʱ�ӽ���)������ģʽ
//	xTimerStart(SoftTimer100HzHandle,0); //�������ڶ�ʱ��
//	
//	//����soft timer����10Hz																
//	SoftTimer10HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)100,
//																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
//																(void*)3, //��ʱ��ID
//																(TimerCallbackFunction_t)SoftTimer10HzCallback); //���ڶ�ʱ��������100ms(100��ʱ�ӽ���)������ģʽ
//	xTimerStart(SoftTimer10HzHandle,0); //�������ڶ�ʱ��													


	vTaskDelete(xHandleInit); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
}


void test_task(void *pvParameters)
{
	while(1)
	{
		led2_toggle();
		vTaskDelay(1000);
	}
}

