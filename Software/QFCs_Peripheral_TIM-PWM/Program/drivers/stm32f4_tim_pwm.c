/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_tim_pwm.c
  * @author  KitSprout
  * @date    27-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_tim_pwm.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static TIM_HandleTypeDef HTIM_PWM;

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void HAL_TIM_PWM_MspInit( TIM_HandleTypeDef *htim )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if (htim->Instance == TIMx_PWM) {

    /* TIM Clk ***********************************************************/
    TIMx_PWM_CLK_ENABLE();

    /* TIM PWM Pin *******************************************************/
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin       = TIMx_PWM1_GPIO_PIN;
    GPIO_InitStruct.Alternate = TIMx_PWM1_GPIO_AF;
    HAL_GPIO_Init(TIMx_PWM1_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = TIMx_PWM2_GPIO_PIN;
    GPIO_InitStruct.Alternate = TIMx_PWM2_GPIO_AF;
    HAL_GPIO_Init(TIMx_PWM2_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = TIMx_PWM3_GPIO_PIN;
    GPIO_InitStruct.Alternate = TIMx_PWM3_GPIO_AF;
    HAL_GPIO_Init(TIMx_PWM3_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = TIMx_PWM4_GPIO_PIN;
    GPIO_InitStruct.Alternate = TIMx_PWM4_GPIO_AF;
    HAL_GPIO_Init(TIMx_PWM4_GPIO_PORT, &GPIO_InitStruct);
  }
}

void TIM_PWM_Config( void )
{
  TIM_OC_InitTypeDef TIM_OC_InitStruct;

  /* TIM Base Config ************************************************************/
  HTIM_PWM.Instance               = TIMx_PWM;
  HTIM_PWM.Init.Prescaler         = TIMx_PWM_PRESCALER - 1;
  HTIM_PWM.Init.Period            = TIMx_PWM_PERIOD - 1;
  HTIM_PWM.Init.ClockDivision     = 0;
  HTIM_PWM.Init.CounterMode       = TIM_COUNTERMODE_UP;
  HTIM_PWM.Init.RepetitionCounter = 0;
  HAL_TIM_PWM_Init(&HTIM_PWM);

  /* PWM Config *****************************************************************/
  TIM_OC_InitStruct.OCMode       = TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCFastMode   = TIM_OCFAST_DISABLE;
  TIM_OC_InitStruct.OCPolarity   = TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState  = TIM_OCIDLESTATE_RESET;
  TIM_OC_InitStruct.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  TIM_OC_InitStruct.Pulse        = TIMx_PWM_MIN;
  HAL_TIM_PWM_ConfigChannel(&HTIM_PWM, &TIM_OC_InitStruct, TIMx_PWM1_CHANNEL);

  TIM_OC_InitStruct.Pulse        = TIMx_PWM_MIN;
  HAL_TIM_PWM_ConfigChannel(&HTIM_PWM, &TIM_OC_InitStruct, TIMx_PWM2_CHANNEL);

  TIM_OC_InitStruct.Pulse        = TIMx_PWM_MIN;
  HAL_TIM_PWM_ConfigChannel(&HTIM_PWM, &TIM_OC_InitStruct, TIMx_PWM3_CHANNEL);

  TIM_OC_InitStruct.Pulse        = TIMx_PWM_MIN;
  HAL_TIM_PWM_ConfigChannel(&HTIM_PWM, &TIM_OC_InitStruct, TIMx_PWM4_CHANNEL);

  /* PWM Start ******************************************************************/
  HAL_TIM_PWM_Start(&HTIM_PWM, TIMx_PWM1_CHANNEL);
  HAL_TIM_PWM_Start(&HTIM_PWM, TIMx_PWM2_CHANNEL);
  HAL_TIM_PWM_Start(&HTIM_PWM, TIMx_PWM3_CHANNEL);
  HAL_TIM_PWM_Start(&HTIM_PWM, TIMx_PWM4_CHANNEL);
}

void TIM_PWM_SetDuty( uint8_t channel, uint16_t duty )
{
  switch(channel) {
    case 1:
      TIMx_PWM1_DUTY = (duty > TIMx_PWM_MAX) ? (TIMx_PWM_MIN) : (duty);
      break;
    case 2:
      TIMx_PWM2_DUTY = (duty > TIMx_PWM_MAX) ? (TIMx_PWM_MIN) : (duty);
      break;
    case 3:
      TIMx_PWM3_DUTY = (duty > TIMx_PWM_MAX) ? (TIMx_PWM_MIN) : (duty);
      break;
    case 4:
      TIMx_PWM4_DUTY = (duty > TIMx_PWM_MAX) ? (TIMx_PWM_MIN) : (duty);
      break;
  }
  
}

/*************************************** END OF FILE ****************************************/
