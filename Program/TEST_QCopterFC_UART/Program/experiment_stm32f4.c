/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "module_rs232.h"
#include "algorithm_string.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KEY   PBI(2)
#define LED_R PCO(15)
#define LED_G PCO(14)
#define LED_B PCO(13)
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u8 i = 0;

  SystemInit();
  GPIO_Config();
  RS232_Config();

  while(1) {
    LED_G = ~LED_G;

    i++;

    RS232_SendStr(USART3, (u8*)"i = ");
    RS232_SendNum(USART3, Type_D, 3, i);
    RS232_SendStr(USART3, (u8*)"\r\n");

    if(i==255)  i = 0;

    Delay_100ms(1);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED_R PC15 */  /* LED_G PC14 */  /* LED_B PC13 */
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

  LED_G = 1;
  LED_R = 1;
  LED_B = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
