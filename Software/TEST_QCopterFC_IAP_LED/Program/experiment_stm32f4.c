/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_iap.h"
#include "experiment_stm32f4.h"
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  SystemInit();
  IAP_SetVectorTable(IAP_BOOT_SIZE);
  GPIO_Config();

  while(1) {
    LED_R = !LED_R;
    Delay_10ms(5);
    LED_R = !LED_R;
    Delay_10ms(5);
    LED_G = !LED_G;
    Delay_10ms(5);
    LED_G = !LED_G;
    Delay_10ms(5);
    LED_B = !LED_B;
    Delay_10ms(5);
    LED_B = !LED_B;
    Delay_10ms(5);

    while(KEY == KEY_ON) {
      LED_R = !LED_R;
      LED_G = !LED_G;
      LED_B = !LED_B;
      Delay_10ms(5);
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED_R PC13 */  /* LED_G PC14 */  /* LED_B PC15 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* KEY PB2 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_OFF;
}
/*====================================================================================================*/
/*====================================================================================================*/
