/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "stm32f4_i2c.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_mpu6050.h"
#include "module_hmc5883.h"
#include "module_ms5611.h"
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
/*
int main( void )
{
  u16 i = 0;
  u8 USART_BUF[8] = {0};

	SystemInit();
	GPIO_Config();
	RS232_Config();

  while(1) {
    LED_G = ~LED_G;

    i++;
    if(i==65535)	i = 0;

    USART_BUF[0]  = (u8)(i);
    USART_BUF[1]  = (u8)(i >> 8);
		USART_BUF[2]  = (u8)(0);
		USART_BUF[3]  = (u8)(0);
		USART_BUF[4]  = (u8)(0);
		USART_BUF[5]  = (u8)(0);
		USART_BUF[6]  = (u8)(0);
		USART_BUF[7]  = (u8)(0);
		RS232_VisualScope(USART3, USART_BUF, 8);
	}
}
*/

int main( void )
{
  u8 USART_BUF[24] = {0};

	SystemInit();
	GPIO_Config();
	RS232_Config();
 	I2C_Config();

  MPU6050_Init();
  HMC5883_Init();
//  MS5611_Init(&Baro);

 	if(SysTick_Config(420000)) {		// 168MHz / 420000 = 400Hz = 2.5ms
		while(1);
	}

	while(1) {
		LED_G = ~LED_G;

    USART_BUF[0]  = (u8)(Acc.X);
    USART_BUF[1]  = (u8)(Acc.X >> 8);
    USART_BUF[2]  = (u8)(Acc.Y);
    USART_BUF[3]  = (u8)(Acc.Y >> 8);
    USART_BUF[4]  = (u8)(Acc.Z);
    USART_BUF[5]  = (u8)(Acc.Z >> 8);
    USART_BUF[6]  = (u8)(Gyr.X);
    USART_BUF[7]  = (u8)(Gyr.X >> 8);
    USART_BUF[8]  = (u8)(Gyr.Y);
    USART_BUF[9]  = (u8)(Gyr.Y >> 8);
    USART_BUF[10] = (u8)(Gyr.Z);
    USART_BUF[11] = (u8)(Gyr.Z >> 8);
    USART_BUF[12] = (u8)(Mag.X);
    USART_BUF[13] = (u8)(Mag.X >> 8);
    USART_BUF[14] = (u8)(Mag.Y);
    USART_BUF[15] = (u8)(Mag.Y >> 8);
    USART_BUF[16] = (u8)(Mag.Z);
    USART_BUF[17] = (u8)(Mag.Z >> 8);

		if(KEY == 0)
			RS232_VisualScope(USART3, USART_BUF, 8);
		else
			RS232_VisualScope(USART3, USART_BUF+10, 8);
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;

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

  LED_R = 1;
  LED_G = 1;
  LED_B = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
