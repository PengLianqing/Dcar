#include "bsp_led.h"


TIM_HandleTypeDef htim5;
/**
  * @brief          aRGB show
  * @param[in]      aRGB: 0xaaRRGGBB, 'aa' is alpha, 'RR' is red, 'GG' is green, 'BB' is blue
  * @retval         none
  */
/**
  * @brief          显示RGB
  * @param[in]      aRGB:0xaaRRGGBB,'aa' 是透明度,'RR'是红色,'GG'是绿色,'BB'是蓝色
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
	
	/* STEP1:使能IO所在端口GPIOH的时钟 */
	__HAL_RCC_GPIOH_CLK_ENABLE();									

	/* STEP2:配置IO */
	GPIO_InitStruct.Pin = GPIO_PIN_10 + GPIO_PIN_11 + GPIO_PIN_12;		
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		/* 推挽输出模式 */
	GPIO_InitStruct.Pull = GPIO_PULLUP;						/*配置为上拉，即默认输出高电平 */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	/* 配置IO速度为高速 */
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);				/* 初始化GPIO*/
	
	/* STEP3:GPIO输出高 */
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET);
}

void gpio_test_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* STEP1:使能IO所在端口GPIOE的时钟 */
	__HAL_RCC_GPIOE_CLK_ENABLE();									

	/* STEP2:配置IO */
	GPIO_InitStruct.Pin = GPIO_PIN_9 + GPIO_PIN_11;		
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;		/* 推挽输出模式 */
	GPIO_InitStruct.Pull = GPIO_PULLUP;						/*配置为上拉，即默认输出高电平 */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;	/* 配置IO速度为高速 */
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);				/* 初始化GPIO*/
	
	/* STEP3:GPIO输出高 */
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