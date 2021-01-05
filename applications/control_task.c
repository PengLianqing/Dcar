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

/**
  * @brief          freertos��ʼ��
  * @param[in]      null
  * @retval         none
  */
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

/**
  * @brief          init task
	* �������������ʱ��
  * @param[in]      null
  * @retval         none
  */
void init_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //�����ٽ���
	
	//����remote control����
	xTaskCreate( usb_task, 		/* ������ */
							"usb_task", 	/* ������ */
							256, 											/* ����ջ��С����λword��Ҳ����4�ֽ� */
							NULL, 										/* ������� */
							2, 												/* �������ȼ������ΪconfigMAX_PRIORITIES-1�����λ0*/
							&xHandleUSB ); 		/* ������ */
	
	//����test����
	xTaskCreate( test_task, 		/* ������ */
							"test_task", 	/* ������ */
							100, 											/* ����ջ��С����λword��Ҳ����4�ֽ� */
							NULL, 										/* ������� */
							configMAX_PRIORITIES-1, 	/* �������ȼ������ΪconfigMAX_PRIORITIES-1�����λ0*/
							&xHandleTest ); 		/* ������ */
	
	//����soft timer����500Hz
	SoftTimer500HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)2,
																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
																(void*)1, //��ʱ��ID
																(TimerCallbackFunction_t)SoftTimer500HzCallback); //���ڶ�ʱ��������1ms(1��ʱ�ӽ���)������ģʽ
	xTimerStart(SoftTimer500HzHandle,0); //�������ڶ�ʱ��
	
	//����soft timer����200Hz																
	SoftTimer200HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)5,
																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
																(void*)2, //��ʱ��ID
																(TimerCallbackFunction_t)SoftTimer200HzCallback); //���ڶ�ʱ��������10ms(10��ʱ�ӽ���)������ģʽ
	xTimerStart(SoftTimer200HzHandle,0); //�������ڶ�ʱ��
	
	//����soft timer����50Hz																
	SoftTimer50HzHandle=xTimerCreate((const char*)"AutoReloadTimer", (TickType_t)20,
																(UBaseType_t )pdTRUE, //���ڶ�ʱ��
																(void*)3, //��ʱ��ID
																(TimerCallbackFunction_t)SoftTimer50HzCallback); //���ڶ�ʱ��������100ms(100��ʱ�ӽ���)������ģʽ
	xTimerStart(SoftTimer50HzHandle,0); //�������ڶ�ʱ��													


	vTaskDelete(xHandleInit); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
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

