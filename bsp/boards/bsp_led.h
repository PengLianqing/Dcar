#ifndef BSP_LED_H
#define BSP_LED_H
#include "struct_typedef.h"
#include "main.h"
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
extern TIM_HandleTypeDef htim5;

extern void aRGB_led_show(uint32_t aRGB);

void led_init(void);
void gpio_test_init(void);
void led1_toggle(void);
void led2_toggle(void);
void led3_toggle(void);

#endif
