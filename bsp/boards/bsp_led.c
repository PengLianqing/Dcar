#include "bsp_led.h"


TIM_HandleTypeDef htim5;
/**
  * @brief          aRGB show
  * @param[in]      aRGB: 0xaaRRGGBB, 'aa' is alpha, 'RR' is red, 'GG' is green, 'BB' is blue
  * @retval         none
  */
/**
  * @brief          ��ʾRGB
  * @param[in]      aRGB:0xaaRRGGBB,'aa' ��͸����,'RR'�Ǻ�ɫ,'GG'����ɫ,'BB'����ɫ
  * @retval         none
  */
void aRGB_led_show(uint32_t aRGB)
{
    static uint8_t alpha;
    static uint16_t red,green,blue;

    alpha = (aRGB & 0xFF000000) >> 24;
    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
    green = ((aRGB & 0x0000FF00) >> 8) * alpha;
    blue = ((aRGB & 0x000000FF) >> 0) * alpha;

    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, blue);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, green);
    __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, red);
}

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* STEP1:ʹ��IO���ڶ˿�GPIOH��ʱ�� */
	__HAL_RCC_GPIOH_CLK_ENABLE();									

	/* STEP2:����IO */
	GPIO_InitStruct.Pin = GPIO_PIN_10 + GPIO_PIN_11 + GPIO_PIN_12;		
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		/* �������ģʽ */
	GPIO_InitStruct.Pull = GPIO_PULLUP;						/*����Ϊ��������Ĭ������ߵ�ƽ */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	/* ����IO�ٶ�Ϊ���� */
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);				/* ��ʼ��GPIO*/
	
	/* STEP3:GPIO����� */
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET);
}

void gpio_test_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* STEP1:ʹ��IO���ڶ˿�GPIOE��ʱ�� */
	__HAL_RCC_GPIOE_CLK_ENABLE();									

	/* STEP2:����IO */
	GPIO_InitStruct.Pin = GPIO_PIN_9 + GPIO_PIN_11;		
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		/* �������ģʽ */
	GPIO_InitStruct.Pull = GPIO_PULLUP;						/*����Ϊ��������Ĭ������ߵ�ƽ */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	/* ����IO�ٶ�Ϊ���� */
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);				/* ��ʼ��GPIO*/
	
	/* STEP3:GPIO����� */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
}

void led1_toggle(void)
{
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
	//GPIOE->ODR^=GPIO_PIN_9; 
}

void led2_toggle(void)
{
	HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_10);
}

void led3_toggle(void)
{
	HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_11);
}