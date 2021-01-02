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

/* Includes ------------------------------------------------------------------*/
#include "usb_task.h"
//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//usb
#include "usb_device.h"
#include "usbd_cdc_if.h"
//string
#include <stdio.h>
#include <stdarg.h>
#include "string.h"
//led
#include "bsp_led.h"

/**
  * @brief          usb printf buffer
  */
static uint8_t usb_buf[256];

/**
  * @brief          usb task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void usb_task(void *pvParameters)
{
	MX_USB_DEVICE_Init();
	while(1)
	{
		vTaskDelay(100);
		led3_toggle();
		usb_printf("Hello-world\r\n");
		
	}
}

/**
  * @brief          usb printf
  * @param[in]      用法同printf()
  * @retval         none
  */
static void usb_printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;

    va_start(ap, fmt);

    len = vsprintf((char *)usb_buf, fmt, ap);

    va_end(ap);

    CDC_Transmit_FS(usb_buf, len);
}


/**
  * @brief          usb的中断服务函数
  * @param[in]      none
  * @retval         none
  */
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}






