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
  * @brief          volt+数据指针
  */
volt_transmit_t volt_transmit; 

/**
  * @brief          usb task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
void usb_task(void *pvParameters)
{
	MX_USB_DEVICE_Init();
	
	//volt+数据初始化
	volt_init(&volt_transmit);
	
	while(1)
	{
		static TickType_t xTimeNow;
		xTimeNow = xTaskGetTickCountFromISR(); //整型
		
		//usb_printf("d:%4.2f,%4.2f\n",(float)(rand()%1024*1.0f) ,(float)(rand()%1024*1.0f));
		usb_printf("S:%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%4.2f,%ld\n"
				/*RC通道0 			*/,(float)(volt_transmit.chassis_RC->rc.ch[0]*1.0f)
				/*目标控制角度 	*/,(float)(volt_transmit.moto_position[4].moto_target_angle*1.0f)
				/*实际控制角度 	*/,(float)(volt_transmit.moto_position[4].moto_result_angle*1.0f)
				/*目标控制速度 	*/,(float)(volt_transmit.moto_position[4].moto_target_speed*1.0f)
				/*实际控制速度 	*/,(float)(volt_transmit.moto_position[4].moto_result_speed*1.0f)
				/*位置环pid err */,(float)(volt_transmit.pid_position->err[0]*1.0f)
				/*速度环pid err */,(float)(volt_transmit.pid_velocity->err[0]*1.0f)	
				/*系统运行时间 	*/,xTimeNow);
			
		led3_toggle();
		vTaskDelay(10);
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

/**
  * @brief          初始化"volt_transmit"变量，包括pid指针， 遥控器指针，电机数据指针等，传给上位机volt+.
  * @param[out]     volt_init:"volt_transmit"变量指针.
  * @retval         none
  */
static void volt_init(volt_transmit_t *volt_transmit)
{
	volt_transmit->chassis_RC = get_remote_control_point();
	volt_transmit->motor_measure = get_6020_moto1_measure_point();
	volt_transmit->moto_position = get_moto1_position_point();
	volt_transmit->pid_position = get_moto1_position_pid_point();
	volt_transmit->pid_velocity = get_moto1_velocity_pid_point();
}



