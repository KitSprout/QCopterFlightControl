/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    boardConfig.h
  * @author  KitSprout
  * @date    16-Mar-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
/* Exported types --------------------------------------------------------------------------*/
/* Exported constants ----------------------------------------------------------------------*/

#define KS_HW_BOARD_NAME              "QCopterFCs"
#define KS_HW_MCU_NAME                "STM32F411CE"

#define KS_HW_CLOCK_SOUCE_HSE
//#define KS_HW_USE_CLOCK_SOUCE_HSI


/* -------- LED and KEY */

#define LED_R_PIN                     GPIO_PIN_13
#define LED_R_GPIO_PORT               GPIOC
#define LED_R_Set()                   __GPIO_SET(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Reset()                 __GPIO_RST(LED_R_GPIO_PORT, LED_R_PIN)
#define LED_R_Toggle()                __GPIO_TOG(LED_R_GPIO_PORT, LED_R_PIN)

#define LED_G_PIN                     GPIO_PIN_14
#define LED_G_GPIO_PORT               GPIOC
#define LED_G_Set()                   __GPIO_SET(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Reset()                 __GPIO_RST(LED_G_GPIO_PORT, LED_G_PIN)
#define LED_G_Toggle()                __GPIO_TOG(LED_G_GPIO_PORT, LED_G_PIN)

#define LED_B_PIN                     GPIO_PIN_15
#define LED_B_GPIO_PORT               GPIOC
#define LED_B_Set()                   __GPIO_SET(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Reset()                 __GPIO_RST(LED_B_GPIO_PORT, LED_B_PIN)
#define LED_B_Toggle()                __GPIO_TOG(LED_B_GPIO_PORT, LED_B_PIN)

#define KEY_PIN                       GPIO_PIN_2
#define KEY_GPIO_PORT                 GPIOB
#define KEY_Read()                    (__GPIO_READ(KEY_GPIO_PORT, KEY_PIN) == KEY_PIN)


/* -------- TIM Timer */

#define TIMx_PWM                      TIM2
#define TIMx_PWM_CLK_ENABLE()         __HAL_RCC_TIM2_CLK_ENABLE()
#define TIMx_PWM_PRESCALER            (uint32_t)(SystemCoreClock / 1000000)
#define TIMx_PWM_PERIOD               (uint32_t)(1000)
#define TIMx_PWM_MAX                  TIMx_PWM_PERIOD
#define TIMx_PWM_MIN                  0

#define TIMx_PWM_PERI                 TIMx->PSC
#define TIMx_PWM_PULSE                TIMx->ARR

#define TIMx_PWM1_GPIO_PIN            GPIO_PIN_0
#define TIMx_PWM1_GPIO_PORT           GPIOA
#define TIMx_PWM1_GPIO_AF             GPIO_AF1_TIM2
#define TIMx_PWM1_CHANNEL             TIM_CHANNEL_1
#define TIMx_PWM1_DUTY                TIMx_PWM->CCR1

#define TIMx_PWM2_GPIO_PIN            GPIO_PIN_1
#define TIMx_PWM2_GPIO_PORT           GPIOA
#define TIMx_PWM2_GPIO_AF             GPIO_AF1_TIM2
#define TIMx_PWM2_CHANNEL             TIM_CHANNEL_2
#define TIMx_PWM2_DUTY                TIMx_PWM->CCR2

#define TIMx_PWM3_GPIO_PIN            GPIO_PIN_2
#define TIMx_PWM3_GPIO_PORT           GPIOA
#define TIMx_PWM3_GPIO_AF             GPIO_AF1_TIM2
#define TIMx_PWM3_CHANNEL             TIM_CHANNEL_3
#define TIMx_PWM3_DUTY                TIMx_PWM->CCR3

#define TIMx_PWM4_GPIO_PIN            GPIO_PIN_3
#define TIMx_PWM4_GPIO_PORT           GPIOA
#define TIMx_PWM4_GPIO_AF             GPIO_AF1_TIM2
#define TIMx_PWM4_CHANNEL             TIM_CHANNEL_4
#define TIMx_PWM4_DUTY                TIMx_PWM->CCR4

/* Exported functions ----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
