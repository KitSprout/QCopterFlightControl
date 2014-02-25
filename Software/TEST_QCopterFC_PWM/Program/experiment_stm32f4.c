/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC_board.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define PWM_MOTOR_MIN 800
#define PWM_MOTOR_MED 1400
#define PWM_MOTOR_MAX 2000  // 32~80%

#define PWM1  TIM2->CCR1
#define PWM2  TIM2->CCR2
#define PWM3  TIM2->CCR3
#define PWM4  TIM2->CCR4
#define PWM5  TIM3->CCR1
#define PWM6  TIM3->CCR2
#define PWM7  TIM3->CCR3
#define PWM8  TIM3->CCR4
#define PWM9  TIM4->CCR1
#define PWM10 TIM4->CCR2
#define PWM11 TIM8->CCR1
#define PWM12 TIM8->CCR2
/*=====================================================================================================*/
/*=====================================================================================================*/
void PWM_Config( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u32 i = PWM_MOTOR_MIN;

  SystemInit();
  LED_Config();
  KEY_Config();
  PWM_Config();

  while(1) {
    LED_G = !LED_G;

    while(KEY == 1) {
      PWM1  = i;
      PWM2  = i;
      PWM3  = i;
      PWM4  = i;
      PWM5  = i;
      PWM6  = i;
      PWM7  = i;
      PWM8  = i;
      PWM9  = i;
      PWM10 = i;
      PWM11 = i;
      PWM12 = i;
      i++;
      if(i>=PWM_MOTOR_MAX) {
        i = PWM_MOTOR_MIN;
        LED_R = !LED_R;
      }
      Delay_100us(5);
    }

    Delay_100ms(1);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void PWM_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

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
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

  /* TIM2 PWM1 PA0 */	/* TIM2 PWM2 PA1 */	/* TIM2 PWM3 PA2 */	/* TIM2 PWM4 PA3 */
  /* TIM3 PWM5 PA6 */	/* TIM3 PWM6 PA7 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* TIM3 PWM7 PB0 */	/* TIM3 PWM8 PB1 */	/* TIM4 PWM9 PB6 */	/* TIM4 PWM10 PB7 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* TIM8 PWM11 PC6 */	/* TIM8 PWM12 PC7 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  TIM_DeInit(TIM2);
  TIM_DeInit(TIM3);
  TIM_DeInit(TIM4);
  TIM_DeInit(TIM8);

  /************************** PWM Output **************************************/
  /* 設定 TIM2 TIM3 TIM4 Time Base */
  TIM_TimeBaseStruct.TIM_Period = (u16)(2500-1);              // 週期 = 2.5ms, 400Hz
  TIM_TimeBaseStruct.TIM_Prescaler = (u16)(84-1);             // 除頻84 = 1M ( 1us )
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 上數
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct);
  /* 設定TIM8 Time Base */
  TIM_TimeBaseStruct.TIM_Period = (u16)(2500-1);              // 週期 = 2.5ms, 400kHz
  TIM_TimeBaseStruct.TIM_Prescaler = (u16)(168-1);            // 除頻42 = 1M ( 1us )
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // 上數
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStruct);

  /* 設定 TIM2 TIM3 TIM4 TIM8 OC */
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;              // 配置為 PWM1 模式
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // 致能 OC
  TIM_OCInitStruct.TIM_Pulse = PWM_MOTOR_MAX;                 // 設置跳變值
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
  TIM_OC1Init(TIM8, &TIM_OCInitStruct);                       // 初始化 TIM8 OC1
  TIM_OC2Init(TIM8, &TIM_OCInitStruct);                       // 初始化 TIM8 OC2
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
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);           // 致能 TIM8 OC1 預裝載
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);           // 致能 TIM8 OC2 預裝載

  /* 啟動 */
  TIM_ARRPreloadConfig(TIM2, ENABLE);                         // 致能 TIM2 重載寄存器ARR
  TIM_ARRPreloadConfig(TIM3, ENABLE);                         // 致能 TIM3 重載寄存器ARR
  TIM_ARRPreloadConfig(TIM4, ENABLE);                         // 致能 TIM4 重載寄存器ARR
  TIM_ARRPreloadConfig(TIM8, ENABLE);                         // 致能 TIM8 重載寄存器ARR
  TIM_Cmd(TIM2, ENABLE);                                      // 致能 TIM2
  TIM_Cmd(TIM3, ENABLE);                                      // 致能 TIM3
  TIM_Cmd(TIM4, ENABLE);                                      // 致能 TIM4
  TIM_Cmd(TIM8, ENABLE);                                      // 致能 TIM8
  TIM_CtrlPWMOutputs(TIM8, ENABLE);

  TIM2->CCR1 = PWM_MOTOR_MIN;
  TIM2->CCR2 = PWM_MOTOR_MIN;
  TIM2->CCR3 = PWM_MOTOR_MIN;
  TIM2->CCR4 = PWM_MOTOR_MIN;
  TIM3->CCR1 = PWM_MOTOR_MIN;
  TIM3->CCR2 = PWM_MOTOR_MIN;
  TIM3->CCR3 = PWM_MOTOR_MIN;
  TIM3->CCR4 = PWM_MOTOR_MIN;
  TIM4->CCR1 = PWM_MOTOR_MIN;
  TIM4->CCR2 = PWM_MOTOR_MIN;
  TIM8->CCR1 = PWM_MOTOR_MIN;
  TIM8->CCR2 = PWM_MOTOR_MIN;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
