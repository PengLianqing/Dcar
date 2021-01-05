/**
  ****************************(C) COPYRIGHT 2021 Peng****************************
  * @file       beagbone_task.c/h
  * @brief      ��beagbone��ͨ������
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
#include "beagbone_task.h"

/**
  * @brief          ����usart1���
  */
extern UART_HandleTypeDef huart1;

/**
* @brief          �Ƿ�ʹ���ַ�����ʽ����
* 1 ʹ��printf�ַ�����ʽ��������
* 0 ʹ�ö����Ʒ�ʽ��������
  */
#define USE_PRINTF_TRANSLATE 1

/**
* @brief          rs232���ڷ���buffer
  */
#define RS232_TX_BUF_LEN 128
char rs232_tx_buf[RS232_TX_BUF_LEN];

/**
* @brief          rs232���ݷ��ͽṹ��
  */
struct feedback_data_t feedback_data;

/**
* @brief          rs232���ݷ��ͽṹ��
  */
struct control_date_t control_date;


/**
  * @brief          freertos 500hz �����ʱ������
	* ��beagbone��������ͨ��.
  * @param[in]      none
  * @retval         
  */
void SoftTimer50HzCallback(TimerHandle_t xTimer)
{
	/*
	Ҫ���͵����ݣ�
	������������.xTimeNow ,imu_angel[0] ,imu_angel[1] ,imu_angel[2]
	�����������.moto_position.result_angle ,
	ָ������.mode0 ,mode1 ,channel[0] ,channel[1]
	*/
	volatile static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
	#if USE_PRINTF_TRANSLATE
	//ʹ��printf�ַ�����ʽ��������
		sprintf(rs232_tx_buf ,"D:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%d\n",
			(float)(rand()%1024*0.1f),
			(float)(rand()%1024*0.1f),
			(float)(rand()%1024*0.1f),
			(float)(rand()%1024*0.1f),
			(float)(rand()%1024*0.1f),
			(float)(rand()%1024*0.1f),
			rand()%1024,
			rand()%1024,
			rand()%1024,
			rand()%1024,
			rand()%1024,
			rand()%1024);		
		/*
		���ڲ���beagbone�����������ݣ�beagbone�Ĵ���..������������......
		*/
//		1.0f,//(float)(rand()%1024*0.1f),
//		2.0f,//(float)(rand()%1024*0.1f),
//		3.0f,//(float)(rand()%1024*0.1f),
//		4.0f,//(float)(rand()%1024*0.1f),
//		5.0f,//(float)(rand()%1024*0.1f),
//		6.0f,//(float)(rand()%1024*0.1f),
//		7,//rand()%1024,
//		8,//rand()%1024,
//		9,//rand()%1024,
//		10,//rand()%1024,
//		11,//rand()%1024,
//		12);//rand()%1024);		
		HAL_UART_Transmit(&huart1, (uint8_t *)rs232_tx_buf, strlen(rs232_tx_buf), 10);
		
	#else
	//ʹ�ö����Ʒ�ʽ��������
		feedback_data.channel[0]=(float)(rand()%1024*0.1f);
		feedback_data.channel[1]=(float)(rand()%1024*0.1f);
		feedback_data.channel[2]=(float)(rand()%1024*0.1f);
		feedback_data.imu_angel [0] = (float)(rand()%1024*0.1f);
		feedback_data.imu_angel [1] = (float)(rand()%1024*0.1f);
		feedback_data.imu_angel [2] = (float)(rand()%1024*0.1f);
		feedback_data.mode0 = rand()%1024,
		feedback_data.mode1 = rand()%1024,
		
		memcpy((void*)(rs232_tx_buf), (void*)(&feedback_data), sizeof(struct feedback_data_t));
		rs232_tx_buf[sizeof(struct feedback_data_t)] = '\n';
		HAL_UART_Transmit(&huart1, (uint8_t *)rs232_tx_buf, sizeof(struct feedback_data_t)+1,10);
	
	#endif
		//���ڴ�memset����ֹ���ճ��ֶ��'\n'�����
		memset(rs232_tx_buf ,0 ,128);
}

/**
  * @brief          �Ѵ�beagbone��ȡ�����ݽ���.
  * @param[in]      rxbuf�Ǵ����dma����ƫ�õ�ַ��pData�ǹ���������
  * @retval         
  */
void BBB_data_receive(uint8_t *rx_buf, char* pData)
{
	memcpy((void*)(pData), (void*)(rx_buf), strlen( (const char *)rx_buf));
	sscanf((const char *)rx_buf , "B:%d,%d,%f,%f,%d,%d$\n"
		,&control_date.data1,&control_date.data2,&control_date.data3,&control_date.data4,&control_date.mode1,&control_date.mode2);
	memset(pData ,0 ,strlen(pData));
}
