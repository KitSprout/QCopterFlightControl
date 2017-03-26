/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_tim.c
  * @author  KitSprout
  * @date    16-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_tim.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static TIM_HandleTypeDef HTIM_TIMER2;
static TIM_HandleTypeDef HTIM_TIMER3;
static TIM_HandleTypeDef HTIM_TIMER4;

TimHandle_st hTimer2 = {
  .handle        = &HTIM_TIMER2,
  .EventCallback = NULL,
};

TimHandle_st hTimer3 = {
  .handle        = &HTIM_TIMER3,
  .EventCallback = NULL,
};

TimHandle_st hTimer4 = {
  .handle        = &HTIM_TIMER4,
  .EventCallback = NULL,
};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

void Timer2_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER2_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER2_IRQn, TIMER2_TIMx_IRQn_PREEMPT, TIMER2_TIMx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(TIMER2_IRQn);

  /* TIM Base Config ************************************************************/
  HTIM_TIMER2.Instance               = TIMER2;
  HTIM_TIMER2.Init.Prescaler         = prescaler - 1;
  HTIM_TIMER2.Init.Period            = period - 1;
  HTIM_TIMER2.Init.ClockDivision     = 0;
  HTIM_TIMER2.Init.CounterMode       = TIM_COUNTERMODE_UP;
  HTIM_TIMER2.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&HTIM_TIMER2);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&HTIM_TIMER2);
}

void Timer3_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER3_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER3_IRQn, TIMER3_TIMx_IRQn_PREEMPT, TIMER3_TIMx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(TIMER3_IRQn);

  /* TIM Base Config ************************************************************/
  HTIM_TIMER3.Instance               = TIMER3;
  HTIM_TIMER3.Init.Prescaler         = prescaler - 1;
  HTIM_TIMER3.Init.Period            = period - 1;
  HTIM_TIMER3.Init.ClockDivision     = 0;
  HTIM_TIMER3.Init.CounterMode       = TIM_COUNTERMODE_UP;
  HTIM_TIMER3.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&HTIM_TIMER3);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&HTIM_TIMER3);
}

void Timer4_Config( uint32_t prescaler, uint32_t period )
{
  /* TIMX Clk ******************************************************************/
  TIMER4_CLK_ENABLE();

  /* NVIC Config ***************************************************************/
  HAL_NVIC_SetPriority(TIMER4_IRQn, TIMER4_TIMx_IRQn_PREEMPT, TIMER4_TIMx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(TIMER4_IRQn);

  /* TIM Base Config ************************************************************/
  HTIM_TIMER4.Instance               = TIMER4;
  HTIM_TIMER4.Init.Prescaler         = prescaler - 1;
  HTIM_TIMER4.Init.Period            = period - 1;
  HTIM_TIMER4.Init.ClockDivision     = 0;
  HTIM_TIMER4.Init.CounterMode       = TIM_COUNTERMODE_UP;
  HTIM_TIMER4.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&HTIM_TIMER4);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&HTIM_TIMER4);
}

void Timer2_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(&HTIM_TIMER2);
  }
  else {
    HAL_TIM_Base_Stop_IT(&HTIM_TIMER2);
  }
}

void Timer3_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(&HTIM_TIMER3);
  }
  else {
    HAL_TIM_Base_Stop_IT(&HTIM_TIMER3);
  }
}
void Timer4_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(&HTIM_TIMER4);
  }
  else {
    HAL_TIM_Base_Stop_IT(&HTIM_TIMER4);
  }
}

/*************************************** END OF FILE ****************************************/
