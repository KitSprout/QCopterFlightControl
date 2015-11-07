/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "experiment_stm32f4.h"

#include "FreeRTOS.h"
#include "task.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define TIME_TASK_PRIORITY  ( tskIDLE_PRIORITY + 4UL )

xTaskHandle TimeTask;

void Task_LEDR( void )
{
  portTickType xLastWakeTime = xTaskGetTickCount();

  while(1){
    LED_R = !LED_R;
    vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_RATE_MS ) );
  }
}

void Task_LEDG( void )
{
  portTickType xLastWakeTime = xTaskGetTickCount();

  while(1){
    LED_G = !LED_G;
    vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_RATE_MS ) );
  }
}

void Task_LEDB( void )
{
  portTickType xLastWakeTime = xTaskGetTickCount();

  while(1){
    LED_B = !LED_B;
    vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_RATE_MS ) );
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  SystemInit();
  GPIO_Config();

  xTaskCreate((TaskFunction_t)Task_LEDR, (const char *)"TASK_LEDR", configMINIMAL_STACK_SIZE, NULL, TIME_TASK_PRIORITY, &TimeTask);
  xTaskCreate((TaskFunction_t)Task_LEDG, (const char *)"TASK_LEDG", configMINIMAL_STACK_SIZE, NULL, TIME_TASK_PRIORITY, &TimeTask);
  xTaskCreate((TaskFunction_t)Task_LEDB, (const char *)"TASK_LEDB", configMINIMAL_STACK_SIZE, NULL, TIME_TASK_PRIORITY, &TimeTask);

  vTaskStartScheduler();

  while(1) {
    
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED_R PC15 */  /* LED_G PC14 */  /* LED_B PC13 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* KEY PB2 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_OFF;
}
/*====================================================================================================*/
/*====================================================================================================*/
void vApplicationTickHook( void )
{

}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/
