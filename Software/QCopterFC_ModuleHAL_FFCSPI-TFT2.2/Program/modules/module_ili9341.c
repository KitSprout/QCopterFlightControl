/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "algorithms\algorithm_string.h"

#include "module_ili9341.h"
#include "module_fontlib.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define LCD_SPIx                    SPI1
#define LCD_SPIx_CLK_ENABLE()       __HAL_RCC_SPI1_CLK_ENABLE()

#define LCD_LIG_PIN                 GPIO_PIN_14
#define LCD_LIG_GPIO_PORT           GPIOB
#define LCD_LIG_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

//#define BLIGHT_PWM
#ifdef BLIGHT_PWM
  #define LCD_LIG                   TIM2->CCR2
  #define BLIGHT_MAX                255
  #define BLIGHT_MIN                0
  #define BLIGHT_DEFAULT            200
#else
  #define LCD_LIG_H                 __GPIO_SET(LCD_LIG_GPIO_PORT, LCD_LIG_PIN)
  #define LCD_LIG_L                 __GPIO_RST(LCD_LIG_GPIO_PORT, LCD_LIG_PIN)
  #define BLIGHT_MAX                1
  #define BLIGHT_MIN                0
  #define BLIGHT_DEFAULT            1
#endif

#define LCD_DC_PIN                  GPIO_PIN_0
#define LCD_DC_GPIO_PORT            GPIOC
#define LCD_DC_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define LCD_DC_H                    __GPIO_SET(LCD_DC_GPIO_PORT, LCD_DC_PIN)
#define LCD_DC_L                    __GPIO_RST(LCD_DC_GPIO_PORT, LCD_DC_PIN)

#define LCD_RST_PIN                 GPIO_PIN_15
#define LCD_RST_GPIO_PORT           GPIOB
#define LCD_RST_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_RST_H                   __GPIO_SET(LCD_RST_GPIO_PORT, LCD_RST_PIN)
#define LCD_RST_L                   __GPIO_RST(LCD_RST_GPIO_PORT, LCD_RST_PIN)

#define LCD_CST_PIN                 GPIO_PIN_4
#define LCD_CST_GPIO_PORT           GPIOA
#define LCD_CST_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_CST_AF                  GPIO_AF5_SPI1
#define LCD_CST_H                   __GPIO_SET(LCD_CST_GPIO_PORT, LCD_CST_PIN)
#define LCD_CST_L                   __GPIO_RST(LCD_CST_GPIO_PORT, LCD_CST_PIN)

#define LCD_SCK_PIN                 GPIO_PIN_5
#define LCD_SCK_GPIO_PORT           GPIOA
#define LCD_SCK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_SCK_AF                  GPIO_AF5_SPI1

#define LCD_SDO_PIN                 GPIO_PIN_6
#define LCD_SDO_GPIO_PORT           GPIOA
#define LCD_SDO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_SDO_AF                  GPIO_AF5_SPI1

#define LCD_SDI_PIN                 GPIO_PIN_7
#define LCD_SDI_GPIO_PORT           GPIOA
#define LCD_SDI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define LCD_SDI_AF                  GPIO_AF5_SPI1
/*====================================================================================================*/
/*====================================================================================================*/
static void ILI9341_LigConfig( void );
static void LCD_WriteCmd( uint8_t WriteCmd );
static void LCD_WriteData( uint16_t WriteData );
static void LCD_WriteColor( uint16_t Color );

static SPI_HandleTypeDef SPI_HandleStruct;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ILI9341_Config
**功能 : 配置IO
**輸入 : None
**輸出 : None
**使用 : ILI9341_Config();
**====================================================================================================*/
/*====================================================================================================*/
void ILI9341_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Clk ******************************************************************/
  LCD_DC_GPIO_CLK_ENABLE();
  LCD_RST_GPIO_CLK_ENABLE();
  LCD_CST_GPIO_CLK_ENABLE();
  LCD_SCK_GPIO_CLK_ENABLE();
  LCD_SDO_GPIO_CLK_ENABLE();
  LCD_SDI_GPIO_CLK_ENABLE();
  LCD_SPIx_CLK_ENABLE();

  ILI9341_LigConfig();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = LCD_DC_PIN;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = LCD_RST_PIN;
  HAL_GPIO_Init(LCD_RST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = LCD_CST_PIN;
  HAL_GPIO_Init(LCD_CST_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = LCD_SCK_PIN;
  GPIO_InitStruct.Alternate = LCD_SCK_AF;
  HAL_GPIO_Init(LCD_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = LCD_SDO_PIN;
  GPIO_InitStruct.Alternate = LCD_SDO_AF;
  HAL_GPIO_Init(LCD_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = LCD_SDI_PIN;
  GPIO_InitStruct.Alternate = LCD_SDI_AF;
  HAL_GPIO_Init(LCD_SDI_GPIO_PORT, &GPIO_InitStruct);

  LCD_CST_H;  // 低電位有效

  /* SPI Init ****************************************************************/
  SPI_HandleStruct.Instance               = LCD_SPIx;
  SPI_HandleStruct.Init.Mode              = SPI_MODE_MASTER;
  SPI_HandleStruct.Init.Direction         = SPI_DIRECTION_2LINES;
  SPI_HandleStruct.Init.DataSize          = SPI_DATASIZE_8BIT;
  SPI_HandleStruct.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  SPI_HandleStruct.Init.CLKPhase          = SPI_PHASE_2EDGE;
  SPI_HandleStruct.Init.NSS               = SPI_NSS_SOFT;
  SPI_HandleStruct.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  SPI_HandleStruct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SPI_HandleStruct.Init.TIMode            = SPI_TIMODE_DISABLE;
  SPI_HandleStruct.Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  SPI_HandleStruct.Init.CRCPolynomial     = 7;
  HAL_SPI_Init(&SPI_HandleStruct);

  __HAL_SPI_ENABLE(&SPI_HandleStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ILI9341_LigConfig
**功能 : 配置調光 IO
**輸入 : None
**輸出 : None
**使用 : ILI9341_LigConfig();
**====================================================================================================*/
/*====================================================================================================*/
static void ILI9341_LigConfig( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

#ifdef BLIGHT_PWM
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

  /* PWM Clk Init *************************************************************/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_LIG_GPIO_CLK, ENABLE);

  /* LIG PA1  */
  GPIO_InitStruct.GPIO_Pin = LCD_LIG_PIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(LCD_LIG_GPIO_PORT, &GPIO_InitStruct);

  TIM_DeInit(TIM2);

  /************************** PWM Output **************************************/
  /* 設定 TIM2 Time Base */
  TIM_TimeBaseStruct.TIM_Period = (uint16_t)(256-1);          // Set Period
  TIM_TimeBaseStruct.TIM_Prescaler = (uint16_t)(0);           // Set Freq
  TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;    // Count Up
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);

  /* 設定 TIM2 OC2 */
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;              // PWM1 Mode
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;  // Enable OC
  TIM_OCInitStruct.TIM_Pulse = 0;                             // Period Pulse
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;      // 當計數值小於 PWM_MOTOR_MIN 時為高電平
  TIM_OC2Init(TIM2, &TIM_OCInitStruct);                       // 初始化 TIM2 OC2
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);           // Enable TIM2 OC2 Preload

  /* 啟動 */
  TIM_ARRPreloadConfig(TIM2, ENABLE);                         // Enable TIM2 ARR Preload
  TIM_Cmd(TIM2, ENABLE);                                      // Enable TIM2

  LCD_LIG = BLIGHT_MIN;

#else
  /* GPIO Clk ******************************************************************/
  LCD_LIG_GPIO_CLK_ENABLE();

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin       = LCD_LIG_PIN;
  HAL_GPIO_Init(LCD_LIG_GPIO_PORT, &GPIO_InitStruct);

  LCD_LIG_L;

#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_WriteCmd
**功能 : Write Command or Address
**輸入 : WriteCmd
**輸出 : None
**使用 : LCD_WriteCmd(0xCB);
**====================================================================================================*/
/*====================================================================================================*/
static void LCD_WriteCmd( uint8_t WriteCmd )
{
  LCD_CST_L;
  LCD_DC_L;
  SPI_RW(LCD_SPIx, WriteCmd);
  LCD_CST_H;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_WriteData
**功能 : Write Data
**輸入 : WriteData
**輸出 : None
**使用 : LCD_WriteData(Byte8H(Color));
**====================================================================================================*/
/*====================================================================================================*/
static void LCD_WriteData( uint16_t WriteData )
{
  LCD_CST_L;
  LCD_DC_H;
  SPI_RW(LCD_SPIx, WriteData);
  LCD_CST_H;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_WriteColor
**功能 : Write Color
**輸入 : Color
**輸出 : None
**使用 : LCD_WriteColor(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
static void LCD_WriteColor( uint16_t Color )
{
  LCD_CST_L;
  LCD_DC_H;
  SPI_RW(LCD_SPIx, Byte8H(Color));
  SPI_RW(LCD_SPIx, Byte8L(Color));
  LCD_CST_H;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : ILI9341_Init
**功能 : 初始化 ILI9341
**輸入 : None
**輸出 : None
**使用 : ILI9341_Init();
**====================================================================================================*/
/*====================================================================================================*/
void ILI9341_Init( void )
{
  // Reset
  LCD_RST_L;
  Delay_100ms(1);
  LCD_RST_H;
  Delay_10ms(5);

  LCD_WriteCmd(0xCB);
  LCD_WriteData(0x39);
  LCD_WriteData(0x2C);
  LCD_WriteData(0x00);
  LCD_WriteData(0x34);
  LCD_WriteData(0x02);

  LCD_WriteCmd(0xCF);
  LCD_WriteData(0x00);
  LCD_WriteData(0XC1);
  LCD_WriteData(0X30);

  LCD_WriteCmd(0xE8);
  LCD_WriteData(0x85);
  LCD_WriteData(0x00);
  LCD_WriteData(0x78);

  LCD_WriteCmd(0xEA);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);

  LCD_WriteCmd(0xED);
  LCD_WriteData(0x64);
  LCD_WriteData(0x03);
  LCD_WriteData(0X12);
  LCD_WriteData(0X81);

  LCD_WriteCmd(0xF7);
  LCD_WriteData(0x20);

  LCD_WriteCmd(0xC0);   // Power control
  LCD_WriteData(0x23);  // VRH[5:0]

  LCD_WriteCmd(0xC1);   // Power control
  LCD_WriteData(0x10);  // SAP[2:0];BT[3:0]

  LCD_WriteCmd(0xC5);   // VCM control
  LCD_WriteData(0x3e);  // 對比度調節
  LCD_WriteData(0x28); 

  LCD_WriteCmd(0xC7);   // VCM control2
  LCD_WriteData(0x86);  // --

  LCD_WriteCmd(0x36);   // Memory Access Control
#ifdef H_VIEW
  LCD_WriteData(0x48);  // 豎屏
#else
  LCD_WriteData(0xE8);  // 橫屏
#endif

  LCD_WriteCmd(0x3A);
  LCD_WriteData(0x55);

  LCD_WriteCmd(0xB1);
  LCD_WriteData(0x00);
  LCD_WriteData(0x18);

  LCD_WriteCmd(0xB6);   // Display Function Control
  LCD_WriteData(0x08);
  LCD_WriteData(0x82);
  LCD_WriteData(0x27);

  LCD_WriteCmd(0xF2);   // 3Gamma Function Disable
  LCD_WriteData(0x00);

  LCD_WriteCmd(0x26);   // Gamma curve selected
  LCD_WriteData(0x01);

  LCD_WriteCmd(0xE0);   // Set Gamma
  LCD_WriteData(0x0F);
  LCD_WriteData(0x31);
  LCD_WriteData(0x2B);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x0E);
  LCD_WriteData(0x08);
  LCD_WriteData(0x4E);
  LCD_WriteData(0xF1);
  LCD_WriteData(0x37);
  LCD_WriteData(0x07);
  LCD_WriteData(0x10);
  LCD_WriteData(0x03);
  LCD_WriteData(0x0E);
  LCD_WriteData(0x09);
  LCD_WriteData(0x00);

  LCD_WriteCmd(0XE1);   // Set Gamma
  LCD_WriteData(0x00);
  LCD_WriteData(0x0E);
  LCD_WriteData(0x14);
  LCD_WriteData(0x03);
  LCD_WriteData(0x11);
  LCD_WriteData(0x07);
  LCD_WriteData(0x31);
  LCD_WriteData(0xC1);
  LCD_WriteData(0x48);
  LCD_WriteData(0x08);
  LCD_WriteData(0x0F);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x31);
  LCD_WriteData(0x36);
  LCD_WriteData(0x0F);

  LCD_WriteCmd(0x11);   // Exit Sleep
  Delay_10ms(12);

  LCD_WriteCmd(0x29);   // Display on
  LCD_WriteCmd(0x2C);

  LCD_Clear(BLACK);
  LCD_SetBackLight(BLIGHT_DEFAULT);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_Clear
**功能 : Clear Windows
**輸入 : Color
**輸出 : None
**使用 : LCD_Clear(BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_Clear( uint16_t Color )
{
  uint32_t Point = LCD_W * LCD_H;

  LCD_SetWindow(0, 0, LCD_W-1, LCD_H-1);

  while(Point--)
    LCD_WriteColor(Color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_SetCursor
**功能 : Set Cursor
**輸入 : CoordiX, CoordiY
**輸出 : None
**使用 : LCD_SetCursor(StartX, StartY);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_SetCursor( uint16_t CoordiX, uint16_t CoordiY )
{
  LCD_WriteCmd(0x2A);
  LCD_WriteData(Byte8H(CoordiX));
  LCD_WriteData(Byte8L(CoordiX));
  LCD_WriteCmd(0x2B);
  LCD_WriteData(Byte8H(CoordiY));
  LCD_WriteData(Byte8L(CoordiY));
  LCD_WriteCmd(0x2C);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_SetWindow
**功能 : Set Window
**輸入 : StartX, StartY, EndX, EndY
**輸出 : None
**使用 : LCD_SetWindow(X0, Y0, X1, Y1);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_SetWindow( uint16_t StartX, uint16_t StartY, uint16_t EndX, uint16_t EndY )
{
  LCD_WriteCmd(0x2A);
  LCD_WriteData(Byte8H(StartX));
  LCD_WriteData(Byte8L(StartX));
  LCD_WriteData(Byte8H(EndX));
  LCD_WriteData(Byte8L(EndX));
  LCD_WriteCmd(0x2B);
  LCD_WriteData(Byte8H(StartY));
  LCD_WriteData(Byte8L(StartY));
  LCD_WriteData(Byte8H(EndY));
  LCD_WriteData(Byte8L(EndY));
  LCD_WriteCmd(0x2C);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_SetBackLight
**功能 : Set LCD BackLight
**輸入 : BackLight
**輸出 : None
**使用 : LCD_SetBackLight(BLIGHT_DEFAULT);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_SetBackLight( uint16_t BackLight )
{
#ifdef BLIGHT_PWM
  if(BackLight <= 0)                LCD_LIG = BLIGHT_MIN;
  else if(BackLight >= BLIGHT_MAX)  LCD_LIG = BLIGHT_MAX;
  else                              LCD_LIG = BackLight;

#else
  if(BackLight == 0)  LCD_LIG_L;
  else                LCD_LIG_H;

#endif
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawPixel
**功能 : Draw a Pixel
**輸入 : CoordiX, CoordiY, Color
**輸出 : None
**使用 : LCD_DrawPixel(CoordiX, CoordiY, Color);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawPixel( uint16_t CoordiX, uint16_t CoordiY, uint16_t Color )
{
  LCD_SetCursor(CoordiX, CoordiY);
  LCD_WriteColor(Color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawLineX
**功能 : Draw X-Axis Line
**輸入 : CoordiX, CoordiY, Length, Color
**輸出 : None
**使用 : LCD_DrawLine(CoordiX, CoordiY, Length, Color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawLineX( uint16_t CoordiX, uint16_t CoordiY, uint16_t Length, uint16_t Color )
{
  uint16_t i;

  for(i = 0; i < Length; i++)
    LCD_DrawPixel(CoordiX+i, CoordiY, Color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawLineY
**功能 : Draw Y-Axis Line
**輸入 : CoordiX, CoordiY, Length, Color
**輸出 : None
**使用 : LCD_DrawLine(CoordiX, CoordiY, Length, Color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawLineY( uint16_t CoordiX, uint16_t CoordiY, uint16_t Length, uint16_t Color )
{
  uint16_t i;

  for(i = 0; i < Length; i++)
    LCD_DrawPixel(CoordiX, CoordiY+i, Color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawRectangle
**功能 : Draw Rectangle
**輸入 : CoordiX, CoordiY, Length, Width, Color
**輸出 : None
**使用 : LCD_DrawRectangle(CoordiX, CoordiY, Width, Height, Color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawRectangle( uint16_t CoordiX, uint16_t CoordiY, uint16_t Width, uint16_t Height, uint16_t Color )
{
  uint16_t i;

  for(i = 0; i <= Height; i++) {
    LCD_DrawPixel(CoordiX+i, CoordiY, Color);
    LCD_DrawPixel(CoordiX+i, CoordiY+Width, Color);
  }
  for(i = 1; i < Width; i++) {
    LCD_DrawPixel(CoordiX, CoordiY+i, Color);
    LCD_DrawPixel(CoordiX+Height, CoordiY+i, Color);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawRectangleFill
**功能 : Draw Rectangle
**輸入 : CoordiX, CoordiY, Length, Width, Color
**輸出 : None
**使用 : LCD_DrawRectangleFill(CoordiX, CoordiY, Width, Height, Color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawRectangleFill( uint16_t CoordiX, uint16_t CoordiY, uint16_t Width, uint16_t Height, uint16_t Color )
{
  uint16_t i, j;

  for(i = 0; i < Width; i++)
    for(j = 0; j < Height; j++)
      LCD_DrawPixel(CoordiX+j, CoordiY+i, Color);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawCircle
**功能 : Draw Circle
**輸入 : CoordiX, CoordiY, Radius, Color
**輸出 : None
**使用 : LCD_DrawCircle(CoordiX, CoordiY, Radius, Color)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawCircle( uint16_t CoordiX, uint16_t CoordiY, uint16_t Radius, uint16_t Color )
{
  int a, b;
  int di;

  a = 0;
  b = Radius;
  di = 3 - (Radius << 1);

  while(a <= b) {
    LCD_DrawPixel(CoordiX-b, CoordiY-a, Color); //3
    LCD_DrawPixel(CoordiX+b, CoordiY-a, Color); //0
    LCD_DrawPixel(CoordiX-a, CoordiY+b, Color); //1
    LCD_DrawPixel(CoordiX-b, CoordiY-a, Color); //7
    LCD_DrawPixel(CoordiX-a, CoordiY-b, Color); //2
    LCD_DrawPixel(CoordiX+b, CoordiY+a, Color); //4
    LCD_DrawPixel(CoordiX+a, CoordiY-b, Color); //5
    LCD_DrawPixel(CoordiX+a, CoordiY+b, Color); //6
    LCD_DrawPixel(CoordiX-b, CoordiY+a, Color);
    a++;
    // 使用Bresenham算法畫圓
    if(di < 0)
      di += 4 * a + 6;
    else {
      di += 10 + 4* (a - b);
      b--;
    }
    LCD_DrawPixel(CoordiX+a, CoordiY+b, Color);
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_DrawPicture
**功能 : Draw Picture
**輸入 : CoordiX, CoordiY, Length, Width, Pic
**輸出 : None
**使用 : LCD_DrawPicture(CoordiX, CoordiY, Width, Height, Pic);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_DrawPicture( uint16_t CoordiX, uint16_t CoordiY, uint16_t Width, uint16_t Height, const uint8_t *Pic )
{
  uint16_t *readPixel = (uint16_t*)Pic;
  uint32_t i = 0, j = Height * Width;

  LCD_SetWindow(CoordiX, CoordiY, CoordiX+Width-1, CoordiY+Height-1);

  for(i = 0; i < j; i++)
    LCD_WriteColor(readPixel[i]);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_PutChar
**功能 : Put Char
**輸入 : CoordiX, CoordiY, ChWord, FontColor, BackColor
**輸出 : None
**使用 : LCD_PutChar(x, y, 'C', WHITE, BLACK)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_PutChar( uint16_t CoordiX, uint16_t CoordiY, char *ChWord, uint16_t FontColor, uint16_t BackColor )
{
  uint8_t Tmp_Char = 0;
  uint16_t i = 0, j = 0;

  for(i = 0; i < 16; i++) {
    Tmp_Char = ASCII_16x8[*ChWord-32][i];
    for(j = 0; j < 8; j++) {
      if(((Tmp_Char >> (7 - j)) & 0x01) == 0x01)
        LCD_DrawPixel(CoordiX+j, CoordiY+i, FontColor); // 字符顏色
      else
        LCD_DrawPixel(CoordiX+j, CoordiY+i, BackColor); // 背景顏色
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_PutStr
**功能 : Put String
**輸入 : CoordiX, CoordiY, ChWord, FontStyle, FontColor, BackColor
**輸出 : None
**使用 : LCD_PutStr(10, 10, (int8_t*)"TFT LCD TEST ... ", WHITE, BLACK);
**====================================================================================================*/
/*====================================================================================================*/
void LCD_PutStr( uint16_t CoordiX, uint16_t CoordiY, char *ChWord, uint16_t FontColor, uint16_t BackColor )
{
  uint16_t OffsetX = 0;

  while(*ChWord) {
    LCD_PutChar(CoordiX+OffsetX, CoordiY, ChWord, FontColor, BackColor);
    ChWord++;
    OffsetX += 8;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_PutNum
**功能 : Put Number
**輸入 : oordiX, CoordiY, Type, Length, NumData, FontColor, BackColor
**輸出 : None
**使用 : LCD_PutNum(CoordiX, CoordiY, Type_D, Length, NumData, WHITE, BLACK)
**====================================================================================================*/
/*====================================================================================================*/
void LCD_PutNum( uint16_t CoordiX, uint16_t CoordiY, StringType Type, uint8_t Length, uint32_t NumData, uint16_t FontColor, uint16_t BackColor )
{
  char TmpNumber[16] = {0};

  num2Str(Type, Length, TmpNumber, NumData);
  LCD_PutStr(CoordiX, CoordiY, TmpNumber, FontColor, BackColor);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : LCD_TestColoBar
**功能 : Draw Color Bar
**輸入 : None
**輸出 : None
**使用 : LCD_TestColoBar();
**====================================================================================================*/
/*====================================================================================================*/
#define COLOR_TYPE 20
void LCD_TestColoBar( void )
{
  uint32_t i = 0, j = 0;
  uint16_t drawColor[COLOR_TYPE] = {
    WHITE,
    RED,
    GREEN,
    BLUE,
    MAGENTA,
    GRED,
    GBLUE,
    BRED,
    BROWN,
    BRRED,
    CYAN,
    GRAY,
    YELLOW,
    DARKBLUE,
    LIGHTBLUE,
    GRAYBLUE,
    LIGHTGREEN,
    LGRAY,
    LGRAYBLUE,
    LBBLUE
  };

  LCD_SetWindow(0, 0, LCD_W-1, LCD_H-1);

  for(i = 0; i < 20; i++) {
    j = (LCD_H / 20) * LCD_W;
    while(j--)
      LCD_WriteColor(drawColor[i]);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
