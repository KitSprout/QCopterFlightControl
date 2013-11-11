/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "stm32f4_i2c.h"
#include "experiment_stm32f4.h"
#include "QCopterFC_ahrs.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_mpu9150.h"
#include "module_ms5611.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
void System_Init( void )
{
  SystemInit();
  GPIO_Config();
  RS232_Config();
  I2C_Config();

  /* Sensor Init */
  LED_G = (Sensor_Init() == SUCCESS) ? 0 : 1;
  Delay_10ms(10);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u8 i = 0;
  u8 UART_BUF[32] = {0};
  u16 TMP_BUF[16] = {0};

  /* System Init */
  System_Init();

  /* Systick Config */
  if(SysTick_Config(SystemCoreClock/SampleRateFreg)) {  // SampleRateFreg = 500 Hz
    while(1);
  }

  /* Wait Correction */
  while(SensorMode != Mode_Algorithm);

  LED_R = 1;
  LED_G = 1;
  LED_B = 1;

  while(1) {
    LED_B = ~LED_B;

    TMP_BUF[0]  = (s16)(Acc.TrueX*1000);  // 1 mg/LSB
    TMP_BUF[1]  = (s16)(Acc.TrueY*1000);  // 1 mg/LSB
    TMP_BUF[2]  = (s16)(Acc.TrueZ*1000);  // 1 mg/LSB
    TMP_BUF[3]  = (s16)(Gyr.TrueX*100);   // 10 mdps/LSB
    TMP_BUF[4]  = (s16)(Gyr.TrueY*100);   // 10 mdps/LSB
    TMP_BUF[5]  = (s16)(Gyr.TrueZ*100);   // 10 mdps/LSB
    TMP_BUF[6]  = (s16)(Mag.TrueX);       // 100 nTesla/LSB
    TMP_BUF[7]  = (s16)(Mag.TrueY);       // 100 nTesla/LSB
    TMP_BUF[8]  = (s16)(Mag.TrueZ);       // 100 nTesla/LSB
    TMP_BUF[9]  = (s16)(Temp.TrueT*100);  // 0.01 degC/LSB
    TMP_BUF[10] = (s16)(AngE.Pitch*100);  // 0.01 deg/LSB
    TMP_BUF[11] = (s16)(AngE.Roll*100);   // 0.01 deg/LSB
    TMP_BUF[12] = (s16)(AngE.Yaw*10);     // 0.1 deg/LSB
    TMP_BUF[13] = (s16)(Baro.Temp*100);   // 0.01 degC/LSB
    TMP_BUF[14] = (s16)(Baro.Press*10);   // 0.1 mbar/LSB
    TMP_BUF[15] = (s16)(Baro.High);

    for(i=0; i<16; i++) {
      UART_BUF[(i<<1)]   = Byte8L(TMP_BUF[i]);
      UART_BUF[(i<<1)+1] = Byte8H(TMP_BUF[i]);
    }

    if(KEY == 1)
      RS232_VisualScope(USART3, UART_BUF, 8);
    else
      RS232_VisualScope(USART3, UART_BUF+20, 8);
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
