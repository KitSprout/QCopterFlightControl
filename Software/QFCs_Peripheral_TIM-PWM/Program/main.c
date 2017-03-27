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
  * @date    27-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_tim_pwm.h"
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
  uint8_t state = 0;
  uint32_t i = TIMx_PWM_MIN;

  HAL_Init();
  BSP_GPIO_Config();
  BSP_TIM_PWM_Config();

  while(1) {
    if (KEY_Read()) {
      i = (state) ? (i - 1) : (i + 1);
      if (i == TIMx_PWM_MAX){ state = 1; }
      if (i == TIMx_PWM_MIN){ state = 0; }

      TIM_PWM_SetDuty(1, i);
      TIM_PWM_SetDuty(2, i);
      TIM_PWM_SetDuty(3, i);
      TIM_PWM_SetDuty(4, i);

      LED_G_Toggle();
      delay_ms(1);
    }
  }
}

/*************************************** END OF FILE ****************************************/
