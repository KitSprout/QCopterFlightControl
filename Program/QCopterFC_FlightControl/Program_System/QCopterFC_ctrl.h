/* #include "QCopterFC_ctrl.h" */

#ifndef __QCOPTERFC_CTRL_H
#define __QCOPTERFC_CTRL_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define BLDC_THR_MIN    0
#define BLDC_THR_MED    4000
#define BLDC_THR_MAX    10000
#define BLDC_THR_RANGE  10000   // 10000 - 0 = 10000

#define BLDC_PWM_MIN    4000   // Duty Cycle 40%
#define BLDC_PWM_MED    6000   // Duty Cycle 60%
#define BLDC_PWM_MAX    8000   // Duty Cycle 80%
#define BLDC_PWM_RANGE  4000   // 8000 - 4000 = 4000

#define BLDC_THR_TO_PWM 0.4f   // BLDC_PWM_RANGE / BLDC_THR_RANGE

#define PWM1 TIM4->CCR1
#define PWM2 TIM4->CCR2
#define PWM3 TIM2->CCR1
#define PWM4 TIM2->CCR2
#define PWM5 TIM2->CCR3
#define PWM6 TIM8->CCR1
#define PWM7 TIM8->CCR2
#define PWM8 TIM2->CCR3

#define BLDC_M1 TIM3->CCR1   // 無刷 M1
#define BLDC_M2 TIM3->CCR2   // 無刷 M2
#define BLDC_M3 TIM3->CCR3   // 無刷 M3
#define BLDC_M4 TIM3->CCR4   // 無刷 M4
/*=====================================================================================================*/
/*=====================================================================================================*/
void Motor_Config( void );

void BLDC_CtrlPWM( s16 Motor_1, s16 Motor_2, s16 Motor_3, s16 Motor_4 );
void BLDC_CtrlTHR( s16 Motor_1, s16 Motor_2, s16 Motor_3, s16 Motor_4 );

void Ctrl_BasicThr( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
extern vu16 BasicThr;
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif	
