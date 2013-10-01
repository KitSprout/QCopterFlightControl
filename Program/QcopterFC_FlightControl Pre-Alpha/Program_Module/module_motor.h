/* #include "module_motor.h" */

#ifndef __MODULE_MOTOR_H
#define __MODULE_MOTOR_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define PWM_MOTOR_MIN 800
#define PWM_MOTOR_MED 1400
#define PWM_MOTOR_MAX 2000  // 32~80%

#define PWM_SEVRO_MIN 500
#define PWM_SEVRO_MED 1500
#define PWM_SEVRO_MAX 2500

#define PWM1 TIM4->CCR1
#define PWM2 TIM4->CCR2
#define PWM3 TIM2->CCR1
#define PWM4 TIM2->CCR2
#define PWM5 TIM2->CCR3
#define PWM6 TIM8->CCR1
#define PWM7 TIM8->CCR2
#define PWM8 TIM2->CCR3

#define PWM_Motor1 TIM3->CCR1   // 無刷 PWM
#define PWM_Motor2 TIM3->CCR2	  // 無刷 PWM
#define PWM_Motor3 TIM3->CCR3	  // 無刷 PWM
#define PWM_Motor4 TIM3->CCR4	  // 無刷 PWM
/*=====================================================================================================*/
/*=====================================================================================================*/
void Motor_Config( void );
void Motor_Control( u16, u16, u16, u16 );
void Sevro_Control( u16, u16 );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
