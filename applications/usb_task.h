/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       usb_task.c/h
  * @brief      FreeRTOS USB����
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

#ifndef USB_TASK_H
#define USB_TASK_H

/**
  * @brief          usb task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void usb_task(void *pvParameters);

/**
  * @brief          usb printf
  * @param[in]      �÷�ͬprintf()
  * @retval         none
  */
static void usb_printf(const char *fmt,...);

/**
  * @brief          usb���жϷ�����
  * @param[in]      none
  * @retval         none
  */
void OTG_FS_IRQHandler(void);

#endif

