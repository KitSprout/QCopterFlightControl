/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "stm32f4_i2c.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_mpu9150.h"
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
int main( void )
{
  u8 IMU_Buf[24] = {0};
  u8 USART_BUF[24] = {0};

  u16 Tmep = 0;

  SystemInit();
  GPIO_Config();
  RS232_Config();
  I2C_Config();

  LED_G = (Sensor_Init() == SUCCESS) ? 0 : 1;

  Delay_100ms(5);

  LED_R = 1;
  LED_G = 1;
  LED_B = 1;

  while(1) {
    LED_B = ~LED_B;

    MPU9150_Read(IMU_Buf);

    Acc.X = (s16)((IMU_Buf[0]  << 8) | IMU_Buf[1]);
    Acc.Y = (s16)((IMU_Buf[2]  << 8) | IMU_Buf[3]);
    Acc.Z = (s16)((IMU_Buf[4]  << 8) | IMU_Buf[5]);
    Tmep  = (s16)((IMU_Buf[6]  << 8) | IMU_Buf[7]);
    Gyr.X = (s16)((IMU_Buf[8]  << 8) | IMU_Buf[9]);
    Gyr.Y = (s16)((IMU_Buf[10] << 8) | IMU_Buf[11]);
    Gyr.Z = (s16)((IMU_Buf[12] << 8) | IMU_Buf[13]);
    Mag.X = (s16)((IMU_Buf[15] << 8) | IMU_Buf[14]);
    Mag.Y = (s16)((IMU_Buf[17] << 8) | IMU_Buf[16]);
    Mag.Z = (s16)((IMU_Buf[19] << 8) | IMU_Buf[18]);

    USART_BUF[0]  = (u8)(Acc.X);
    USART_BUF[1]  = (u8)(Acc.X >> 8);
    USART_BUF[2]  = (u8)(Acc.Y);
    USART_BUF[3]  = (u8)(Acc.Y >> 8);
    USART_BUF[4]  = (u8)(Acc.Z);
    USART_BUF[5]  = (u8)(Acc.Z >> 8);
    USART_BUF[6]  = (u8)(Tmep);
    USART_BUF[7]  = (u8)(Tmep >> 8);
//    USART_BUF[6]  = (u8)(Gyr.X);
//    USART_BUF[7]  = (u8)(Gyr.X >> 8);
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

    if(KEY == 1)
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
