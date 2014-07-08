/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "experiment_stm32f4.h"
#include "system_waveForm.h"
#include "module_rs232.h"
#include "module_imu.h"
#include "module_mpu9250.h"
//#include "module_ms5611.h"
#include "module_ili9341.h"
#include "algorithm_string.h"
#include "algorithm_ahrs.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define WaveFromNum_X 266
#define WaveFromNum_Y 12

WaveForm_Struct WaveForm;
SEN_MODE SEN_STATE = SEN_CORR;
/*====================================================================================================*/
/*====================================================================================================*/
void System_Init( void )
{
  /* System Setup */
  SystemInit();

  /* Device Config */
  GPIO_Config();
  RS232_Config();
  Sensor_Config();

  ILI9341_Config();

  LED_R = LED_ON;

  Sensor_Init();
  ILI9341_Init();
  AHRS_Init(&NumQ, &AngE);

  LED_R = LED_OFF;

  // WaveFrom
	WaveForm.Channel = 3;
	WaveForm.WindowColor = WHITE;
	WaveForm.BackColor = BLACK;
	WaveForm.Data[0] = 0;
	WaveForm.Data[1] = 0;
	WaveForm.Data[2] = 0;
	WaveForm.Scale[0] = 10;
	WaveForm.Scale[1] = 10;
	WaveForm.Scale[2] = 10;
	WaveForm.PointColor[0] = RED;
	WaveForm.PointColor[1] = GREEN;
	WaveForm.PointColor[2] = BLUE;

  LCD_Clear(BLACK);

  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*0,  (u8*)"------", ASCII1608, WHITE, BLACK);
  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*2,  (u8*)" SINE ", ASCII1608, WHITE, BLACK);
  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*4,  (u8*)" DEMO ", ASCII1608, WHITE, BLACK);
  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*6,  (u8*)"------", ASCII1608, WHITE, BLACK);
  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*9,  (u8*)"Axis-X", ASCII1608, RED,   BLACK);
  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*14, (u8*)"Axis-Y", ASCII1608, GREEN, BLACK);
  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*19, (u8*)"Axis-Z", ASCII1608, BLUE,  BLACK);
  LCD_PutStr(WaveFromNum_X, WaveFromNum_Y+8*24, (u8*)"------", ASCII1608, WHITE, BLACK);
}
/*====================================================================================================*/
/*====================================================================================================*/
void System_Corr( u16 SystickFreq )
{
  /* Select Correct */
  LED_G = LED_ON;
  Delay_100ms(5);
  LED_G = LED_OFF;

  /* Systick Setup */
  if(SysTick_Config((u32)(SystemCoreClock/(float)SystickFreq)))
    while(1);

  /* Wait Correct */
  while(SEN_STATE != SEN_ALG);
}
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  u8 UART_BUF[32] = {0};

  System_Init();
  System_Corr(SampleRateFreg);   // SampleRateFreg = 500 Hz

  LED_G = LED_ON;
  Delay_100ms(5);
  LED_G = LED_OFF;

  while(1) {
    LED_B = !LED_B;

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

//    UART_BUF[12] = Byte8L((s16)(Mag.TrueX));      // 100 nTesla/LSB
//    UART_BUF[13] = Byte8L((s16)(Mag.TrueX));
//    UART_BUF[14] = Byte8L((s16)(Mag.TrueY));      // 100 nTesla/LSB
//    UART_BUF[15] = Byte8H((s16)(Mag.TrueY));
//    UART_BUF[16] = Byte8L((s16)(Mag.TrueZ));      // 100 nTesla/LSB
//    UART_BUF[17] = Byte8H((s16)(Mag.TrueZ));

    UART_BUF[18] = Byte8L((s16)(Temp.TrueT*100)); // 0.01 degC/LSB
    UART_BUF[19] = Byte8H((s16)(Temp.TrueT*100));

    UART_BUF[20] = Byte8L((s16)(AngE.Pitch*100)); // 0.01 deg/LSB
    UART_BUF[21] = Byte8H((s16)(AngE.Pitch*100)); // 0.01 deg/LSB
    UART_BUF[22] = Byte8L((s16)(AngE.Roll*100));  // 0.01 deg/LSB
    UART_BUF[23] = Byte8H((s16)(AngE.Roll*100));  // 0.01 deg/LSB
    UART_BUF[24] = Byte8L((s16)(AngE.Yaw*10));    // 100 nTesla/LSB
    UART_BUF[25] = Byte8H((s16)(AngE.Yaw*10));    // 0.1 deg/LSB

//    UART_BUF[26] = Byte8L((s16)(Baro.Temp*100));  // 0.01 degC/LSB
//    UART_BUF[27] = Byte8H((s16)(Baro.Temp*100));

//    UART_BUF[28] = Byte8L((s16)(Baro.Press*10));  // 0.1 mbar/LSB
//    UART_BUF[29] = Byte8H((s16)(Baro.Press*10));

//    UART_BUF[30] = Byte8L((s16)(Baro.Height));
//    UART_BUF[31] = Byte8H((s16)(Baro.Height));

    if(KEY == 1) {
      WaveForm.Data[0] = (s16)(Acc.TrueX*1000);
      WaveForm.Data[1] = (s16)(Acc.TrueY*1000);
      WaveForm.Data[2] = (s16)(Acc.TrueZ*1000);
      RS232_VisualScope(UART_BUF);    // Print Acc
    }
    else {
      WaveForm.Data[0] = (s16)(AngE.Pitch*10);
      WaveForm.Data[1] = (s16)(AngE.Roll*10);
      WaveForm.Data[2] = (s16)(AngE.Yaw*10);
      RS232_VisualScope(UART_BUF+20); // Print AngE
    }

    LCD_PutNum(WaveFromNum_X, WaveFromNum_Y+8*11, Type_I, 5, WaveForm.Data[0], RED,   BLACK);
    LCD_PutNum(WaveFromNum_X, WaveFromNum_Y+8*16, Type_I, 5, WaveForm.Data[1], GREEN, BLACK);
    LCD_PutNum(WaveFromNum_X, WaveFromNum_Y+8*21, Type_I, 5, WaveForm.Data[2], BLUE,  BLACK);

    WaveFormPrint(&WaveForm);
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

  LED_R = 1;
  LED_G = 1;
  LED_B = 1;



  /* IMU_Config */

  /* GPIO Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2  | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/*====================================================================================================*/
/*====================================================================================================*/
