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
**函數 : Motor_Config
**功能 : 設定PWM輸出訊號
**輸入 : None
**輸出 : None
**使用 : Motor_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void Motor_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

  /* TIM2 PWM3  PA0 */	/* TIM2 PWM4  PA1 */	/* TIM2 PWM5  PA2 */	/* TIM2 PWM8  PA3 */
  /* TIM3 PWM9  PA6 */	/* TIM3 PWM10 PA7 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* TIM3 PWM11 PB0 */	/* TIM3 PWM12 PB1 */	/* TIM4 PWM1  PB6 */	/* TIM4 PWM2  PB7 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  TIM_DeInit(TIM2);
  TIM_DeInit(TIM3);
  TIM_DeInit(TIM4);

  /************************** PWM Output **************************************/
  /* 設定 TIM2 TIM3 TIM4 Time Base */
  TIM_TimeBaseStruct.TIM_Period = (u16)(10000-1);             // 週期 0.25*10000 = 2.5ms, 400Hz
  TIM_TimeBaseStruct.TIM_Prescaler = (u16)(21-1);             // 除頻 84MHz/21 = 4MHz ( 0.25us )
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 上數
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);

  /* 設定 TIM2 TIM3 TIM4 TIM8 OC */
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;              // 配置為 PWM1 模式
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // 致能 OC
  TIM_OCInitStruct.TIM_Pulse = BLDC_PWM_MIN;                  // 設置跳變值
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;      // 當計數值小於 PWM_MOTOR_MIN 時為高電平
  TIM_OC1Init(TIM2, &TIM_OCInitStruct);                       // 初始化 TIM2 OC1
  TIM_OC2Init(TIM2, &TIM_OCInitStruct);                       // 初始化 TIM2 OC2
  TIM_OC3Init(TIM2, &TIM_OCInitStruct);                       // 初始化 TIM2 OC3
  TIM_OC4Init(TIM2, &TIM_OCInitStruct);                       // 初始化 TIM2 OC4
  TIM_OC1Init(TIM3, &TIM_OCInitStruct);                       // 初始化 TIM3 OC1
  TIM_OC2Init(TIM3, &TIM_OCInitStruct);                       // 初始化 TIM3 OC2
  TIM_OC3Init(TIM3, &TIM_OCInitStruct);                       // 初始化 TIM3 OC3
  TIM_OC4Init(TIM3, &TIM_OCInitStruct);                       // 初始化 TIM3 OC4
  TIM_OC1Init(TIM4, &TIM_OCInitStruct);                       // 初始化 TIM4 OC1
  TIM_OC2Init(TIM4, &TIM_OCInitStruct);                       // 初始化 TIM4 OC2
  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);           // 致能 TIM2 OC1 預裝載
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);           // 致能 TIM2 OC2 預裝載
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);           // 致能 TIM2 OC3 預裝載
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);           // 致能 TIM2 OC4 預裝載
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);           // 致能 TIM3 OC1 預裝載
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);           // 致能 TIM3 OC2 預裝載
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);           // 致能 TIM3 OC3 預裝載
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);           // 致能 TIM3 OC4 預裝載
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);           // 致能 TIM4 OC1 預裝載
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);           // 致能 TIM4 OC2 預裝載

  /* 啟動 */
  TIM_ARRPreloadConfig(TIM2, ENABLE);                         // 致能 TIM2 重載寄存器ARR
  TIM_ARRPreloadConfig(TIM3, ENABLE);                         // 致能 TIM3 重載寄存器ARR
  TIM_ARRPreloadConfig(TIM4, ENABLE);                         // 致能 TIM4 重載寄存器ARR
  TIM_Cmd(TIM2, ENABLE);                                      // 致能 TIM2
  TIM_Cmd(TIM3, ENABLE);                                      // 致能 TIM3
  TIM_Cmd(TIM4, ENABLE);                                      // 致能 TIM4

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
void BLDC_CtrlPWM( u16 Motor_1, u16 Motor_2, u16 Motor_3, u16 Motor_4 )
{
  if(Motor_1>BLDC_PWM_MAX)      Motor_1 = BLDC_PWM_MAX;
  else if(Motor_1<BLDC_PWM_MIN) Motor_1 = BLDC_PWM_MIN;

  if(Motor_2>BLDC_PWM_MAX)      Motor_2 = BLDC_PWM_MAX;
  else if(Motor_2<BLDC_PWM_MIN) Motor_2 = BLDC_PWM_MIN;

  if(Motor_3>BLDC_PWM_MAX)      Motor_3 = BLDC_PWM_MAX;
  else if(Motor_3<BLDC_PWM_MIN) Motor_3 = BLDC_PWM_MIN;

  if(Motor_4>BLDC_PWM_MAX)      Motor_4 = BLDC_PWM_MAX;
  else if(Motor_4<BLDC_PWM_MIN) Motor_4 = BLDC_PWM_MIN;

  BLDC_M1 = Motor_1;
  BLDC_M2 = Motor_2;
  BLDC_M3 = Motor_3;
  BLDC_M4 = Motor_4;
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
void BLDC_CtrlTHR( u16 Motor_1, u16 Motor_2, u16 Motor_3, u16 Motor_4 )
{
  u16 tempPWM[4] = {0};

  tempPWM[0] = (u16)(Motor_1*BLDC_THR_TO_PWM);
  tempPWM[1] = (u16)(Motor_2*BLDC_THR_TO_PWM);
  tempPWM[2] = (u16)(Motor_3*BLDC_THR_TO_PWM);
  tempPWM[3] = (u16)(Motor_4*BLDC_THR_TO_PWM);

  BLDC_CtrlPWM(tempPWM[0], tempPWM[1], tempPWM[2], tempPWM[3]);
}
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
  static s16 TempThrB = 0;

  // 粗調油門
  if(KEY_LP == KEY_ON) {
    TempThrB = (s16)Exp_ThrB;
    if(TempThrB>BLDC_THR_MAX)
      TempThrB = BLDC_THR_MAX;
    if(TempThrB<BLDC_THR_MIN)
      TempThrB = BLDC_THR_MIN;
  }

  // 細調油門
  TempThr = (s16)((s16)TempThrB + (s16)Exp_ThrS);
  if(TempThr>BLDC_THR_MAX)
    TempThr = BLDC_THR_MAX;
  if(TempThr<BLDC_THR_MIN)
    TempThr = BLDC_THR_MIN;

  BasicThr = TempThr;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
