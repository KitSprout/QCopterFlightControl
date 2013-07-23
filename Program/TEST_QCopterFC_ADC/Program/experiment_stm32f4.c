/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
#include "stm32f4_usart.h"
#include "module_rs232.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KEY   PBI(2)
#define LED_R PCO(15)
#define LED_G PCO(14)
#define LED_B PCO(13)
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );

u16 ADC_AveTr[ADC_Channel] = {0};
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u8 TrData[8] = {0};

	SystemInit();
	GPIO_Config();
	ADC_Config();
	RS232_Config();

  while(1) {
    LED_G = ~LED_G;
    ADC_Average(ADC_AveTr);

    TrData[0] = (u8)(ADC_AveTr[0]);
    TrData[1] = (u8)(ADC_AveTr[0]>>8);
    TrData[2] = (u8)(ADC_AveTr[1]);
    TrData[3] = (u8)(ADC_AveTr[1]>>8);
    TrData[4] = (u8)(0);
    TrData[5] = (u8)(0);
    TrData[6] = (u8)(0);
    TrData[7] = (u8)(0);
    RS232_VisualScope(USART3, TrData, 8);
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
