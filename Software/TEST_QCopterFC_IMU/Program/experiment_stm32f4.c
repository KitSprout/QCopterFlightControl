/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "stm32f4_i2c.h"
#include "experiment_stm32f4.h"
#include "QCopterFC_board.h"
#include "QCopterFC_ahrs.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_mpu9150.h"
#include "module_ms5611.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
SEN_MODE SEN_STATE = SEN_CORR;
/*=====================================================================================================*/
/*=====================================================================================================*/
void System_Init( void )
{
  /* System Setup */
  SystemInit();

  /* Device Config */
  LED_Config();
  KEY_Config();
  RS232_Config();
  Sensor_Config();

  Delay_10ms(5);

  /* Device Init */
  Sensor_Init();

  /* Select Correct */
  LED_G = 0;
  Delay_100ms(5);
  LED_G = 1;

  /* Systick Setup */
  if(SysTick_Config((u32)(SystemCoreClock/SampleRateFreg)))
    while(1);

  /* Wait Correct */
  while(SEN_STATE != SEN_ALG);

  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_ON;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u8 UART_BUF[32] = {0};

  /* System Init */
  System_Init();

  while(1) {
    LED_B = !LED_B;
    Delay_10ms(1);

    UART_BUF[0]  = Byte8L((s16)(Acc.TrueX*1000)); // Acc.X 1 mg/LSB
    UART_BUF[1]  = Byte8H((s16)(Acc.TrueX*1000)); 
    UART_BUF[2]  = Byte8L((s16)(Acc.TrueY*1000)); // Acc.Y 1 mg/LSB
    UART_BUF[3]  = Byte8H((s16)(Acc.TrueY*1000));   
    UART_BUF[4]  = Byte8L((s16)(Acc.TrueZ*1000)); // Acc.Z 1 mg/LSB
    UART_BUF[5]  = Byte8H((s16)(Acc.TrueZ*1000));

    UART_BUF[6]  = Byte8L((s16)(Gyr.TrueX*100));  // Gyr.X 10 mdps/LSB
    UART_BUF[7]  = Byte8H((s16)(Gyr.TrueX*100));
    UART_BUF[8]  = Byte8L((s16)(Gyr.TrueY*100));  // Gyr.Y 10 mdps/LSB
    UART_BUF[9]  = Byte8H((s16)(Gyr.TrueY*100));
    UART_BUF[10] = Byte8L((s16)(Gyr.TrueZ*100));  // Gyr.Z 10 mdps/LSB
    UART_BUF[11] = Byte8H((s16)(Gyr.TrueZ*100));

    UART_BUF[12] = Byte8L((s16)(Mag.TrueX));      // 100 nTesla/LSB
    UART_BUF[13] = Byte8L((s16)(Mag.TrueX));
    UART_BUF[14] = Byte8L((s16)(Mag.TrueY));      // 100 nTesla/LSB
    UART_BUF[15] = Byte8H((s16)(Mag.TrueY));
    UART_BUF[16] = Byte8L((s16)(Mag.TrueZ));      // 100 nTesla/LSB
    UART_BUF[17] = Byte8H((s16)(Mag.TrueZ));

    UART_BUF[18] = Byte8L((s16)(Temp.TrueT*100)); // 0.01 degC/LSB
    UART_BUF[19] = Byte8H((s16)(Temp.TrueT*100));

    UART_BUF[20] = Byte8L((s16)(AngE.Pitch*100)); // 0.01 deg/LSB
    UART_BUF[21] = Byte8H((s16)(AngE.Pitch*100)); // 0.01 deg/LSB
    UART_BUF[22] = Byte8L((s16)(AngE.Roll*100));  // 0.01 deg/LSB
    UART_BUF[23] = Byte8H((s16)(AngE.Roll*100));  // 0.01 deg/LSB
    UART_BUF[24] = Byte8L((s16)(AngE.Yaw*10));    // 100 nTesla/LSB
    UART_BUF[25] = Byte8H((s16)(AngE.Yaw*10));    // 0.1 deg/LSB

    UART_BUF[26] = Byte8L((s16)(Baro.Temp*100));  // 0.01 degC/LSB
    UART_BUF[27] = Byte8H((s16)(Baro.Temp*100));

    UART_BUF[28] = Byte8L((s16)(Baro.Press*10));  // 0.1 mbar/LSB
    UART_BUF[29] = Byte8H((s16)(Baro.Press*10));

    UART_BUF[30] = Byte8L((s16)(Baro.Height));
    UART_BUF[31] = Byte8H((s16)(Baro.Height));

    if(KEY == 1)
      RS232_VisualScope(UART_BUF);    // Print Acc
    else
      RS232_VisualScope(UART_BUF+20); // Print AngE
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
