/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       bsp_beagbone.c/h
  * @brief      
  * @note       ��������ͨ�������ж�����������freeRTOS����
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
  * @brief          uart1����˫����buffer
	* ����ԭʼ���ݣ�Ϊ128���ֽڣ���ֹDMA����Խ��
  */
#define RS232_RX_BUF_NUM 128
extern uint8_t rs232_rx_buf[2][RS232_RX_BUF_NUM];

/**
  * @brief          uart1����˫������Թ�����
  */
extern char tooler[100];
extern char rs232_receive[RS232_RX_BUF_NUM];

/**
  * @brief          ���ܵ����ݽṹ��
  */
struct feedback_data_t{
	float imu_angel[3]; //pitch,yaw,roll
	float imu_speed[3];
	int channel[3]; //ǰ����ɲ����ת�� -30000~+30000
	int mode0;
	int mode1;
	int mode2;
};
extern struct feedback_data_t feedback_data;
extern struct feedback_data_t feedback_data_recv;

/**
* @brief          	���͵����ݽṹ��
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
  * @brief          beagboneͨ�ų�ʼ��
  * @param[in]      none
  * @param[out]     none
  * @retval         none
  */
void beagbone_rs232_init(void);
	
/**
  * @brief          beagboneͨ�Ŵ��ڳ�ʼ��
  * @param[in]      none
  * @param[out]     none
  * @retval         none
  */
void beagbone_rx_dma_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

	
#endif
