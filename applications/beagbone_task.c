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
#include "beagbone_task.h"

/**
  * @brief          串口usart1句柄
  */
extern UART_HandleTypeDef huart1;

/**
* @brief          是否使用字符串方式发送
* 1 使用printf字符串方式发送数据
* 0 使用二进制方式发送数据
  */
#define USE_PRINTF_TRANSLATE 1

/**
* @brief          rs232串口发送buffer
  */
#define RS232_TX_BUF_LEN 128
char rs232_tx_buf[RS232_TX_BUF_LEN];

/**
* @brief          rs232数据发送结构体
  */
struct feedback_data_t feedback_data;

/**
* @brief          rs232数据发送结构体
  */
struct control_date_t control_date;


/**
  * @brief          freertos 500hz 软件定时器任务
	* 与beagbone进行数据通信.
  * @param[in]      none
  * @retval         
  */
void SoftTimer50HzCallback(TimerHandle_t xTimer)
{
	/*
	要发送的数据：
	程序运行数据.xTimeNow ,imu_angel[0] ,imu_angel[1] ,imu_angel[2]
	电机控制数据.moto_position.result_angle ,
	指令数据.mode0 ,mode1 ,channel[0] ,channel[1]
	*/
	volatile static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
	#if USE_PRINTF_TRANSLATE
	//使用printf字符串方式发送数据
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
		用于测试beagbone发回来的数据，beagbone的串口..我真是无语了......
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
	//使用二进制方式发送数据
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
		//清内存memset，防止接收出现多个'\n'的情况
		memset(rs232_tx_buf ,0 ,128);
}

/**
  * @brief          把从beagbone获取的数据解析.
  * @param[in]      rxbuf是传入的dma数组偏置地址，pData是工具人数组
  * @retval         
  */
void BBB_data_receive(uint8_t *rx_buf, char* pData)
{
	memcpy((void*)(pData), (void*)(rx_buf), strlen( (const char *)rx_buf));
	sscanf((const char *)rx_buf , "B:%d,%d,%f,%f,%d,%d$\n"
		,&control_date.data1,&control_date.data2,&control_date.data3,&control_date.data4,&control_date.mode1,&control_date.mode2);
	memset(pData ,0 ,strlen(pData));
}
