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
#include "bsp_beagbone.h"

/**
  * @brief          uart1��dma���
  */
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

/**
  * @brief          uart1����˫����buffer
  */
uint8_t rs232_rx_buf[2][RS232_RX_BUF_NUM];

/**
  * @brief          beagboneͨ�ų�ʼ��
  * @param[in]      none
  * @param[out]     none
  * @retval         none
  */
void beagbone_rs232_init(void)
{
	beagbone_rx_dma_init(rs232_rx_buf[0], rs232_rx_buf[1], RS232_RX_BUF_NUM);
}

/**
  * @brief          beagboneͨ�Ŵ��ڳ�ʼ��
  * @param[in]      none
  * @param[out]     none
  * @retval         none
  */
void beagbone_rx_dma_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
    //enable the DMA transfer for the receiver request
    //ʹ��DMA���ڽ���
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);

    //enalbe idle interrupt
    //ʹ�ܿ����ж�
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    //disable DMA
    //ʧЧDMA
    __HAL_DMA_DISABLE(&hdma_usart1_rx);
    while(hdma_usart1_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart1_rx);
    }

    hdma_usart1_rx.Instance->PAR = (uint32_t) & (USART1->DR);
    //memory buffer 1
    //�ڴ滺����1
    hdma_usart1_rx.Instance->M0AR = (uint32_t)(rx1_buf);
    //memory buffer 2
    //�ڴ滺����2
    hdma_usart1_rx.Instance->M1AR = (uint32_t)(rx2_buf);
    //data length
    //���ݳ���
    hdma_usart1_rx.Instance->NDTR = dma_buf_num;
    //enable double memory buffer
    //ʹ��˫������
    SET_BIT(hdma_usart1_rx.Instance->CR, DMA_SxCR_DBM);

    //enable DMA
    //ʹ��DMA
    __HAL_DMA_ENABLE(&hdma_usart1_rx);

}


/**
  * @brief          uart1����˫������Թ�����
  */
char tooler[100];
char rs232_receive[RS232_RX_BUF_NUM];

/**
  * @brief          ���ڿ����жϴ���
	* �����ڿ����ж�.˫���巽ʽ��������
	* ��buffer��Ѱ��beagbone���͵���Ч�ַ���.
  * @param[in]      none
  * @retval         none
  */
void USART1_IRQHandler(void)
{
    if(huart1.Instance->SR & UART_FLAG_RXNE)//���յ�����
    {
        __HAL_UART_CLEAR_PEFLAG(&huart1);
    }
    else if(USART1->SR & UART_FLAG_IDLE)
    {
        static uint16_t this_time_rx_len = 0;

        __HAL_UART_CLEAR_PEFLAG(&huart1);

        if ((hdma_usart1_rx.Instance->CR & DMA_SxCR_CT) == RESET)
        {
            /* Current memory buffer used is Memory 0 */
    
            //disable DMA
            //ʧЧDMA
            __HAL_DMA_DISABLE(&hdma_usart1_rx);

            //get receive data length, length = set_data_length - remain_length
            //��ȡ�������ݳ���,���� = �趨���� - ʣ�೤��
            this_time_rx_len = RS232_RX_BUF_NUM - hdma_usart1_rx.Instance->NDTR;

            //reset set_data_lenght
            //�����趨���ݳ���
            hdma_usart1_rx.Instance->NDTR = RS232_RX_BUF_NUM;

            //set memory buffer 1
            //�趨������1
            hdma_usart1_rx.Instance->CR |= DMA_SxCR_CT;
            
            //enable DMA
            //ʹ��DMA
            __HAL_DMA_ENABLE(&hdma_usart1_rx);

						//��DMA�����е����ݽ��м���ʹ���
						for(int j=128 ;j>0 ;j--)
						{
							if( rs232_rx_buf[0][j]=='$' )
							{
								for(int i=j ;i>0 ;i--)
								{
									if( rs232_rx_buf[0][i]=='B' && rs232_rx_buf[0][i+1]==':' )
									{
										BBB_data_receive(&(rs232_rx_buf[0][i]),rs232_receive);
										sprintf(tooler ,"%s\r\n" ,&(rs232_rx_buf[1][i]));
									}
								}
							}
						}
//            if( (rs232_rx_buf[0][0] == '\n' && rs232_rx_buf[1][1] == 'B')||(rs232_rx_buf[0][this_time_rx_len-1] == '\n' && rs232_rx_buf[0][this_time_rx_len-2] == '$') )
//            {
//                //���ݴ�����
//								BBB_data_receive(rs232_rx_buf[0],rs232_receive);
//            }
        }
        else
        {
            /* Current memory buffer used is Memory 1 */
            //disable DMA
            //ʧЧDMA
            __HAL_DMA_DISABLE(&hdma_usart1_rx);

            //get receive data length, length = set_data_length - remain_length
            //��ȡ�������ݳ���,���� = �趨���� - ʣ�೤��
            this_time_rx_len = RS232_RX_BUF_NUM - hdma_usart1_rx.Instance->NDTR;

            //reset set_data_lenght
            //�����趨���ݳ���
            hdma_usart1_rx.Instance->NDTR = RS232_RX_BUF_NUM;

            //set memory buffer 0
            //�趨������0
            //DMA2_Stream4->CR &= ~(DMA_SxCR_CT);
						hdma_usart1_rx.Instance->CR &= ~(DMA_SxCR_CT);
            
            //enable DMA
            //ʹ��DMA
            __HAL_DMA_ENABLE(&hdma_usart1_rx);
						
						//��DMA�����е����ݽ��м���ʹ���
						for(int j=128 ;j>0 ;j--)
						{
							if( rs232_rx_buf[1][j]=='$' )
							{
								for(int i=j ;i>0 ;i--)
								{
									if( rs232_rx_buf[1][i]=='B' && rs232_rx_buf[1][i+1]==':' )
									{
										BBB_data_receive(&(rs232_rx_buf[1][i]),rs232_receive);
										sprintf(tooler ,"%s\r\n" ,&(rs232_rx_buf[1][i]));
									}
								}
							}
						}
//            if( (rs232_rx_buf[1][0] == '\n' && rs232_rx_buf[1][1] == 'B')||(rs232_rx_buf[1][this_time_rx_len-1] == '\n' && rs232_rx_buf[1][this_time_rx_len-2] == '$') )
//            {
//                //��������
//                BBB_data_receive(rs232_rx_buf[1],rs232_receive);
//            }
        }
				
				//this_time_rx_len��DMA���յ������ݳ���
				//sprintf(tooler ,"%d , %d\r\n" ,this_time_rx_len ,RS232_FRAME_LENGTH);			
    }
}

/**
  * @brief          ���ڿ����жϴ��� - ��ʹ��˫�������
	* �����ڿ����ж�.˫���巽ʽ��������
	* ��buffer��Ѱ��beagbone���͵���Ч�ַ���.
  * @param[in]      none
  * @retval         none
  */
//void USART1_IRQHandler(void)
//{
//    if(huart1.Instance->SR & UART_FLAG_RXNE)//���յ�����
//    {
//        __HAL_UART_CLEAR_PEFLAG(&huart1);
//    }
//    else if(USART1->SR & UART_FLAG_IDLE)
//    {
//        static uint16_t this_time_rx_len = 0;

//        __HAL_UART_CLEAR_PEFLAG(&huart1);

//        if ((hdma_usart1_rx.Instance->CR & DMA_SxCR_CT) == RESET)
//        {
//            /* Current memory buffer used is Memory 0 */
//    
//            //disable DMA
//            //ʧЧDMA
//            __HAL_DMA_DISABLE(&hdma_usart1_rx);

//            //get receive data length, length = set_data_length - remain_length
//            //��ȡ�������ݳ���,���� = �趨���� - ʣ�೤��
//            this_time_rx_len = RS232_FRAME_LENGTH - hdma_usart1_rx.Instance->NDTR;

//            //reset set_data_lenght
//            //�����趨���ݳ���
//            hdma_usart1_rx.Instance->NDTR = RS232_RX_BUF_NUM;
//            
//            //enable DMA
//            //ʹ��DMA
//            __HAL_DMA_ENABLE(&hdma_usart1_rx);

//            if(this_time_rx_len == RS232_RX_BUF_NUM)
//            {
//                //���ݴ�����
//							
//            }
//        }
//    }
//}
