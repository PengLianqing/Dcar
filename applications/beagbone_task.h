/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       beagbone_task.c/h
  * @brief      与beagbone的通信任务
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

#ifndef BEAGBONE_TASK_H
#define BEAGBONE_TASK_H

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
//usart
#include "usart.h"
//string
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "string.h"
//led
#include "bsp_led.h"
//bsp_beagbone
#include "bsp_beagbone.h"

/**
  * @brief          把从beagbone获取的数据解析.
  * @param[in]      rxbuf是传入的dma数组偏置地址，pData是工具人数组
  * @retval         
  */
void BBB_data_receive(uint8_t *rx_buf,  char* pData);


#endif







