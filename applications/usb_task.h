/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       usb_task.c/h
  * @brief      FreeRTOS USB任务
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
  * @param[in]      用法同printf()
  * @retval         none
  */
static void usb_printf(const char *fmt,...);

/**
  * @brief          usb的中断服务函数
  * @param[in]      none
  * @retval         none
  */
void OTG_FS_IRQHandler(void);

#endif

