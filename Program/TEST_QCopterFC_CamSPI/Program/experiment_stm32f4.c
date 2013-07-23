/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "module_rs232.h"
#include "module_camspi.h"
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
int main(void)
{
  u8 TxBuffer[100] = {0};
  u8 RxBuffer[100] = {0};

  SystemInit();
  GPIO_Config();
  CamSPI_Config();

  while(KEY != 1) {
    LED_R = ~LED_R;
    Delay_100ms(2);
  }
  do {
    TxBuffer[0] = 0xAF;
    CamSPI_RW(TxBuffer, RxBuffer, 10);
  } while(RxBuffer[0] != SUCCESS);
  TxBuffer[0] = 0;
  CamSPI_RW(TxBuffer, RxBuffer, 100);
  if(RxBuffer[0] == 0x0F && RxBuffer[99] == 0x0F)
    LED_R = 0;
  else
    LED_R = 1;
  Delay_100ms(5);


  while(KEY != 1) {
    LED_G = ~LED_G;
    Delay_100ms(2);
  }
  do {
    TxBuffer[0] = 0xCD;
    CamSPI_RW(TxBuffer, RxBuffer, 10);
  } while(RxBuffer[0] != SUCCESS);
  TxBuffer[0] = 0;
  CamSPI_RW(TxBuffer, RxBuffer, 100);
  if(RxBuffer[0] == 0xF0 && RxBuffer[99] == 0xF0)
    LED_G = 0;
  else
    LED_G = 1;
  Delay_100ms(5);


  while(KEY != 1) {
    LED_B = ~LED_B;
    Delay_100ms(2);
  }
  do {
    TxBuffer[0] = 0x23;
    CamSPI_RW(TxBuffer, RxBuffer, 10);
  } while(RxBuffer[0] != SUCCESS);
  TxBuffer[0] = 0;
  CamSPI_RW(TxBuffer, RxBuffer, 100);
  if(RxBuffer[0] == 0xFF && RxBuffer[99] == 0xFF)
    LED_B = 0;
  else
    LED_B = 1;
  Delay_100ms(5);


  while(1) {
    LED_R = ~LED_R;
    LED_G = ~LED_G;
    LED_B = ~LED_B;
    Delay_100ms(2);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;

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

  LED_G = 1;
  LED_R = 1;
  LED_B = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
