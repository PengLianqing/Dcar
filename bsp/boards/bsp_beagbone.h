/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       bsp_beagbone.c/h
  * @brief      
  * @note       该任务是通过串口中断启动，不是freeRTOS任务
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
#ifndef BSP_BEAGBONE_H
#define BSP_BEAGBONE_H

//struct_typedef
#include "struct_typedef.h"
//usart
#include "usart.h"
//beagbone_task
#include "beagbone_task.h"
//usb_task
#include "usb_task.h"

/**
  * @brief          uart1串口双缓冲buffer
	* 接收原始数据，为128个字节，防止DMA传输越界
  */
#define RS232_RX_BUF_NUM 128
extern uint8_t rs232_rx_buf[2][RS232_RX_BUF_NUM];

/**
  * @brief          uart1串口双缓冲调试工具人
  */
extern char tooler[100];
extern char rs232_receive[RS232_RX_BUF_NUM];

/**
  * @brief          接受的数据结构体
  */
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

/**
* @brief          	发送的数据结构体
  */
struct control_date_t{
	int data1;
	int data2;
	float data3;
	float data4; //%5.2f -300.0f-300.0f
	int mode1;
	int mode2;
};
extern struct control_date_t control_date;


/**
  * @brief          beagbone通信初始化
  * @param[in]      none
  * @param[out]     none
  * @retval         none
  */
void beagbone_rs232_init(void);
	
/**
  * @brief          beagbone通信串口初始化
  * @param[in]      none
  * @param[out]     none
  * @retval         none
  */
void beagbone_rx_dma_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

	
#endif
