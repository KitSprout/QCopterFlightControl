/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC_board.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_transport.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu16 BasicThr = 0;  // 0 ~ 10000 對應到 0.00% ~ 100.00%
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : BLDC_Config
**功能 : 設定PWM輸出訊號
**輸入 : None
**輸出 : None
**使用 : BLDC_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void BLDC_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  /* TIM Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);

  /* TIM2 PWM1 PA0  */ /* TIM2 PWM2 PA1  */ /* TIM2 PWM3 PA2  */ /* TIM2 PWM6 PA3 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* TIM3 PWM5 PB0  */ /* TIM3 PWM4 PB1  */ /* TIM4 PWM10 PB8 */ /* TIM4 PWM11 PB9 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* TIM3 PWM8 PC6  */ /* TIM3 PWM7 PC7 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  TIM_DeInit(TIM2);
  TIM_DeInit(TIM3);
  TIM_DeInit(TIM4);

  /************************** PWM Output **************************************/
  /* TIM2 TIM3 TIM4 Time Base */
  TIM_TimeBaseStruct.TIM_Period = (u16)(2500-1);              // Period = 2.5ms => Freq = 400Hz
  TIM_TimeBaseStruct.TIM_Prescaler = (u16)(84-1);             // fCK_PSC / 84 = 1M ( 1us )
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // Count Up
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);

  /* TIM2 TIM3 TIM4 OC */
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;              // Set PWM1 Mode
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // Enable OC
  TIM_OCInitStruct.TIM_Pulse = BLDC_PWM_MIN;                  // Set Pulse
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;      // when Count < PWM_MOTOR_MIN => High
  TIM_OC1Init(TIM2, &TIM_OCInitStruct);                       // Init TIM2 OC1
  TIM_OC2Init(TIM2, &TIM_OCInitStruct);                       // Init TIM2 OC2
  TIM_OC3Init(TIM2, &TIM_OCInitStruct);                       // Init TIM2 OC3
  TIM_OC4Init(TIM2, &TIM_OCInitStruct);                       // Init TIM2 OC4
  TIM_OC1Init(TIM3, &TIM_OCInitStruct);                       // Init TIM3 OC1
  TIM_OC2Init(TIM3, &TIM_OCInitStruct);                       // Init TIM3 OC2
  TIM_OC3Init(TIM3, &TIM_OCInitStruct);                       // Init TIM3 OC3
  TIM_OC4Init(TIM3, &TIM_OCInitStruct);                       // Init TIM3 OC4
  TIM_OC3Init(TIM4, &TIM_OCInitStruct);                       // Init TIM4 OC3
  TIM_OC4Init(TIM4, &TIM_OCInitStruct);                       // Init TIM4 OC4
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);           // Enable TIM2 OC1 Preload
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);           // Enable TIM2 OC2 Preload
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);           // Enable TIM2 OC3 Preload
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);           // Enable TIM2 OC4 Preload
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);           // Enable TIM3 OC1 Preload
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);           // Enable TIM3 OC2 Preload
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);           // Enable TIM3 OC3 Preload
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);           // Enable TIM3 OC4 Preload
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);           // Enable TIM4 OC3 Preload
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);           // Enable TIM4 OC4 Preload

  /* Enable */
  TIM_ARRPreloadConfig(TIM2, ENABLE);                         // Enable TIM2 ARR Preload
  TIM_ARRPreloadConfig(TIM3, ENABLE);                         // Enable TIM3 ARR Preload
  TIM_ARRPreloadConfig(TIM4, ENABLE);                         // Enable TIM4 ARR Preload
  TIM_Cmd(TIM2, ENABLE);                                      // Start TIM2
  TIM_Cmd(TIM3, ENABLE);                                      // Start TIM3
  TIM_Cmd(TIM4, ENABLE);                                      // Start TIM4

  PWM1  = 0;
  PWM2  = 0;
  PWM3  = 0;
  PWM4  = 0;
  PWM9  = 0;
  PWM10 = 0;

  BLDC_M1 = BLDC_PWM_MIN;
  BLDC_M2 = BLDC_PWM_MIN;
  BLDC_M3 = BLDC_PWM_MIN;
  BLDC_M4 = BLDC_PWM_MIN;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : BLDC_CtrlPWM
**功能 : 無刷馬達脈波控制
**輸入 : BLDC_M
**輸出 : None
**使用 : BLDC_CtrlPWM(BLDC_M[0], BLDC_M[1], BLDC_M[2], BLDC_M[3]);
**=====================================================================================================*/
/*=====================================================================================================*/
void BLDC_CtrlPWM( s16 BLDC_1, s16 BLDC_2, s16 BLDC_3, s16 BLDC_4 )
{
  if(BLDC_1 > BLDC_PWM_MAX)       BLDC_1 = BLDC_PWM_MAX;
  else if(BLDC_1 < BLDC_PWM_MIN)  BLDC_1 = BLDC_PWM_MIN;
  if(BLDC_2 > BLDC_PWM_MAX)       BLDC_2 = BLDC_PWM_MAX;
  else if(BLDC_2 < BLDC_PWM_MIN)  BLDC_2 = BLDC_PWM_MIN;
  if(BLDC_3 > BLDC_PWM_MAX)       BLDC_3 = BLDC_PWM_MAX;
  else if(BLDC_3 < BLDC_PWM_MIN)  BLDC_3 = BLDC_PWM_MIN;
  if(BLDC_4 > BLDC_PWM_MAX)       BLDC_4 = BLDC_PWM_MAX;
  else if(BLDC_4 < BLDC_PWM_MIN)  BLDC_4 = BLDC_PWM_MIN;

  BLDC_M1 = BLDC_1;
  BLDC_M2 = BLDC_2;
  BLDC_M3 = BLDC_3;
  BLDC_M4 = BLDC_4;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : BLDC_CtrlTHR
**功能 : 無刷馬達油門控制
**輸入 : BLDC_M
**輸出 : None
**使用 : BLDC_CtrlTHR(BLDC_M[0], BLDC_M[1], BLDC_M[2], BLDC_M[3]);
**=====================================================================================================*/
/*=====================================================================================================*/
//void BLDC_CtrlTHR( s16 BLDC_1, s16 BLDC_2, s16 BLDC_3, s16 BLDC_4 )
//{
//  u16 tempPWM[4] = {0};

//  if(BLDC_1 > BLDC_THR_MAX)       BLDC_1 = BLDC_THR_MAX;
//  else if(BLDC_1 < BLDC_THR_MIN)  BLDC_1 = BLDC_THR_MIN;
//  if(BLDC_2 > BLDC_THR_MAX)       BLDC_2 = BLDC_THR_MAX;
//  else if(BLDC_2 < BLDC_THR_MIN)  BLDC_2 = BLDC_THR_MIN;
//  if(BLDC_3 > BLDC_THR_MAX)       BLDC_3 = BLDC_THR_MAX;
//  else if(BLDC_3 < BLDC_THR_MIN)  BLDC_3 = BLDC_THR_MIN;
//  if(BLDC_4 > BLDC_THR_MAX)       BLDC_4 = BLDC_THR_MAX;
//  else if(BLDC_4 < BLDC_THR_MIN)  BLDC_4 = BLDC_THR_MIN;

//  tempPWM[0] = (u16)(BLDC_PWM_MIN + BLDC_1*BLDC_THR_TO_PWM);
//  tempPWM[1] = (u16)(BLDC_PWM_MIN + BLDC_2*BLDC_THR_TO_PWM);
//  tempPWM[2] = (u16)(BLDC_PWM_MIN + BLDC_3*BLDC_THR_TO_PWM);
//  tempPWM[3] = (u16)(BLDC_PWM_MIN + BLDC_4*BLDC_THR_TO_PWM);

//  BLDC_CtrlPWM(tempPWM[0], tempPWM[1], tempPWM[2], tempPWM[3]);
//}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CTRL_SERVO
**功能 : 伺服馬達控制
**輸入 : SevroA, SevroB
**輸出 : None
**使用 : CTRL_SERVO( SevroA, SevroB );
**=====================================================================================================*/
/*=====================================================================================================*/
//void CTRL_SERVO( void )
//{

//}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Ctrl_BasicThr
**功能 : 飛行器基本油門控制
**輸入 : None
**輸出 : None
**使用 : Ctrl_BasicThr();
**=====================================================================================================*/
/*=====================================================================================================*/
void Ctrl_BasicThr( void )
{
  s16 TempThr = 0;
  static s16 TempThrB = BLDC_PWM_MIN;

  // 粗調油門
  if(KEY_LP == 1) {
    TempThrB = BLDC_PWM_MIN + (s16)(EXP_THR_B*0.12f);
    if(TempThrB>BLDC_PWM_MAX)
      TempThrB = BLDC_PWM_MAX;
    if(TempThrB<BLDC_PWM_MIN)
      TempThrB = BLDC_PWM_MIN;
  }

  // 細調油門
  TempThr = ((s16)TempThrB + (s16)(EXP_THR_S*0.12f));
  if(TempThr>BLDC_PWM_MAX)
    TempThr = BLDC_PWM_MAX;
  if(TempThr<BLDC_PWM_MIN)
    TempThr = BLDC_PWM_MIN;

  if(TempThr>BLDC_PWM_MAX)
    BasicThr = BLDC_PWM_MAX;
  else if(TempThr<BLDC_PWM_MIN)
    BasicThr = BLDC_PWM_MIN;
  else
    BasicThr = TempThr;
}
/*====================================================================================================*/
/*====================================================================================================*/
