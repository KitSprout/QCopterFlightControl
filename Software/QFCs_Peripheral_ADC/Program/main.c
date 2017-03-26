/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    main.c
  * @author  KitSprout
  * @date    16-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_adc.h"
#include "modules\serial.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  static uint16_t ad_bat;

  HAL_Init();
  BSP_GPIO_Config();
  BSP_ADC_Config();
  BSP_UART_SERIAL_Config(NULL);

  while (1) {
    ADC_GetAveValue(&ad_bat);
    ad_bat = ad_bat * (3300.0f / 2048);
    printf("BAT = %4d mV\r\n", ad_bat);
    LED_G_Toggle();
    delay_ms(200);
  }
}

/*************************************** END OF FILE ****************************************/
