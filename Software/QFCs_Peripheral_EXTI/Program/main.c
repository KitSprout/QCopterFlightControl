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
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static __IO uint8_t flag = 0;

/* Private function prototypes -------------------------------------------------------------*/
void IRQEvent_EXTIx_KEY( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();
  BSP_GPIO_Config();
  BSP_EXTI_KEY_Config(IRQEvent_EXTIx_KEY);

  while (1) {
    LED_R_Toggle();
    delay_ms(80);
    LED_G_Toggle();
    delay_ms(80);
    LED_B_Toggle();
    delay_ms(80);
    while (flag) {
      delay_ms(2000);
      flag = 0;
    }
  }
}

void IRQEvent_EXTIx_KEY( void )
{
  flag = 1;
}

/*************************************** END OF FILE ****************************************/
