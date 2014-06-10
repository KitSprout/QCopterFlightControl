/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "experiment_stm32f4.h"
#include "system_waveForm.h"
#include "module_ili9341.h"
#include "algorithm_string.h"
#include "algorithm_mathUnit.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define WaveFromNum_X 266
#define WaveFromNum_Y 12

WaveForm_Struct WaveForm;
/*====================================================================================================*/
/*====================================================================================================*/
void WaveForm_Init( void )
{
	WaveForm.Channel = 3;
	WaveForm.WindowColor = WHITE;
	WaveForm.BackColor = BLACK;
	WaveForm.Data[0] = 0;
	WaveForm.Data[1] = 0;
	WaveForm.Data[2] = 0;
	WaveForm.Scale[0] = 100;
	WaveForm.Scale[1] = 100;
	WaveForm.Scale[2] = 100;
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
int main( void )
{
  s32 Sita = 0;

  SystemInit();
  GPIO_Config();
  ILI9341_Config();
  ILI9341_Init();
  WaveForm_Init();

  LED_R = 0;

  while(1) {
    LED_G = !LED_G;
    LED_B = !LED_B;

    Sita++;
    if(Sita == 360) Sita = 0;
    WaveForm.Data[0] = sinf(toRad(Sita))*10000;
    WaveForm.Data[1] = sinf(toRad(Sita-120))*10000;
    WaveForm.Data[2] = sinf(toRad(Sita-240))*10000;

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
}
/*====================================================================================================*/
/*====================================================================================================*/
