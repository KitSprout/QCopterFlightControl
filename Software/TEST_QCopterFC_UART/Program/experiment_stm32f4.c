/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "experiment_stm32f4.h"
#include "module_rs232.h"
#include "module_visualScope.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define USE_RS232
//#define USE_VISUALSCOPE

#ifdef USE_RS232
int UART_RUN( void )
{
  u8 RecvData = 0;

  RS232_SendStr((u8*)"\r\nHellow World!\r\n\r\n");

  while(1) {
    LED_G = !LED_G;

    RS232_RecvData(&RecvData, 1);
    if(RecvData == 0x0D)  // if press enter
      RS232_SendStr((u8*)"\r\n");
    else
      RS232_SendData(&RecvData, 1);
  }
}
#endif

#ifdef USE_VISUALSCOPE
int UART_RUN( void )
{
  s16 i = 0;
  u8 SendBuf[8] = {0};

  while(1) {
    LED_G = !LED_G;
    Delay_100ms(1);

    i += 1000;
    if(i==S16_MAX)  i = S16_MIN;

    SendBuf[0] = Byte8L(i);
    SendBuf[1] = Byte8H(i);
    VisualScope(SendBuf);
  }
}
#endif
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  SystemInit();
  GPIO_Config();
  RS232_Config();

  UART_RUN();
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

  LED_R = 1;
  LED_G = 1;
  LED_B = 1;
}
/*====================================================================================================*/
/*====================================================================================================*/
