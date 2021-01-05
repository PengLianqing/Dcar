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


void SoftTimer50HzCallback(TimerHandle_t xTimer);


//接受的数据
struct feedback_data_t{
	float imu_angel[3]; //pitch,yaw,roll
	float imu_speed[3];
	int channel[3]; //前进，刹车，转向 -30000~+30000
	int mode0;
	int mode1;
	int mode2;
};
extern struct feedback_data_t feedback_data;
extern struct feedback_data_t feedback_data_recv;

//发送的数据：
struct control_date_t{
	int data1;
	int data2;
	float data3;
	float data4; //%5.2f -300.0f-300.0f
	int mode1;
	int mode2;
};
extern struct control_date_t control_date;

//接收原始数据，为128个字节，防止DMA传输越界
#define RS232_RX_BUF_NUM 128
extern uint8_t rs232_rx_buf[2][RS232_RX_BUF_NUM];

void BBB_data_receive(uint8_t *rx_buf,  char* pData);
void BBB_data_send(struct feedback_data_t* pData);

#endif







