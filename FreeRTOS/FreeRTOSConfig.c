//FreeRTOS相关函数
//FreeRTOS头文件
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "cmsis_os.h"
#include "FreeRTOSConfig.h"

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	while(1);
}

void vApplicationMallocFailedHook( void )
{
	while(1);
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
	
	//FreeRTOS配置
	#if (INCLUDE_xTaskGetSchedulerState == 1 )
		if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		{
	#endif /* INCLUDE_xTaskGetSchedulerState */
		xPortSysTickHandler();
	#if (INCLUDE_xTaskGetSchedulerState == 1 )
		}
	#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}