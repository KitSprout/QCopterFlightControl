/* #include "module_ili9341.h" */

#ifndef __MODULE_ILI9341_H
#define __MODULE_ILI9341_H

#include "stm32f4xx.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define LCD_CST   PAO(4)
#define LCD_RST   PCO(0)
#define LCD_DC    PCO(1)

//#define BLIGHT_PWM
#ifdef BLIGHT_PWM
  #define LCD_LIG   TIM2->CCR2
  #define BLIGHT_MAX      255
  #define BLIGHT_MIN      0
  #define BLIGHT_DEFAULT  200
#else
  #define LCD_LIG   PAO(1)
  #define BLIGHT_MAX      1
  #define BLIGHT_MIN      0
  #define BLIGHT_DEFAULT  1
#endif

#define H_VIEW
#ifdef H_VIEW
  #define LCD_W 320
  #define LCD_H 240
#else
  #define LCD_W 240
  #define LCD_H 320 
#endif

/* LCD Size&Type */
#define ASCII1206   ((u8)0x00)
#define ASCII1608   ((u8)0x04)
#define ASCII1608_  ((u8)0x05)

#define RGB_TO_GARY(C_R, C_G, C_B)  ((u8)(0.299f*C_R + 0.587f*C_G + 0.114f*C_B))
#define ToRGB565(RGB888)            ((u16)((RGB888&0xF80000>>8)|(RGB888&0x00FC00>>5)|(RGB888&0x0000F8>>3)))
#define ToRGB888(RGB565)            ((u16)((RGB565&0xF800<<8)|(RGB565&0x07E0<<5)|(RGB565&0x001F<<3)))

#define RGB565_R(RGB565)            ((u8)((Color&0xF800)>>11))
#define RGB565_G(RGB565)            ((u8)((Color&0x07E0)>>5))
#define RGB565_B(RGB565)            ((u8)(Color&0x001F))

// 輸入 R G B，輸出 RGB565
#define RGB(C_R, C_G, C_B)  ((u32)(((C_R<<8)&0xF800)|((C_G<<3)&0x07E0)|((C_B>>3)&0x001F)))

#define RED         ((u16)0xF800) /* 紅色 */
#define GREEN       ((u16)0x07E0) /* 綠色 */
#define BLUE        ((u16)0x001F) /* 藍色 */
#define MAGENTA     ((u16)0xF81F) /* 紅紫色，洋紅色 */
#define GRED        ((u16)0xFFE0)
#define GBLUE       ((u16)0x07FF) 
#define BRED        ((u16)0XF81F)
#define BROWN       ((u16)0xBC40) /* 棕色 */
#define BRRED       ((u16)0xFC07) /* 棕紅色 */
#define CYAN        ((u16)0x7FFF)	/* 藍綠色，青色 */
#define BLACK       ((u16)0x0000) /* 黑色 */
#define GRAY        ((u16)0X8430) /* 灰色 */
#define WHITE       ((u16)0xFFFF) /* 白色 */
#define YELLOW      ((u16)0xFFE0) /* 黃色 */
#define DARKBLUE    ((u16)0x01CF)	/* 深藍色 */
#define LIGHTBLUE   ((u16)0x7D7C)	/* 淺藍色 */
#define GRAYBLUE    ((u16)0x5458) /* 灰藍色 */
#define LIGHTGREEN  ((u16)0X841F) /* 淺綠色 */
#define LGRAY       ((u16)0XC618) /* 淺灰色 */
#define LGRAYBLUE   ((u16)0XA651) /* 淺灰藍色 */
#define LBBLUE      ((u16)0X2B12) /* 淺棕藍色 */
/*====================================================================================================*/
/*====================================================================================================*/
void ILI9341_Config( void );
void ILI9341_LigConfig( void );
void ILI9341_Init( void );

void LCD_Clear( u16 Color );
void LCD_SetCursor( u16 CoordiX, u16 CoordiY );
void LCD_SetWindow( u16 StartX, u16 StartY, u16 EndX, u16 EndY );
void LCD_SetBackLight( u16 BackLight );

void LCD_DrawPixel( u16 CoordiX, u16 CoordiY, u16 Color );
void LCD_DrawLineX( u16 CoordiX, u16 CoordiY, u16 Length, u16 Color );
void LCD_DrawLineY( u16 CoordiX, u16 CoordiY, u16 Length, u16 Color );
void LCD_DrawRectangle( u16 CoordiX, u16 CoordiY, u16 Width, u16 Height, u16 Color );
void LCD_DrawRectangleFill( u16 CoordiX, u16 CoordiY, u16 Width, u16 Height, u16 Color );
void LCD_DrawCircle( u16 CoordiX, u16 CoordiY, u16 Radius, u16 Color );
void LCD_DrawPicture( u16 CoordiX, u16 CoordiY, u16 Width, u16 Height, uc8 *Pic );

void LCD_PutChar1206( u16 CoordiX, u16 CoordiY, u8* ChWord, u16 FontColor, u16 BackColor );
void LCD_PutChar1608( u16 CoordiX, u16 CoordiY, u8* ChWord, u16 FontColor, u16 BackColor );
void LCD_PutChar1608_( u16 CoordiX, u16 CoordiY, u8* ChWord, u16 FontColor, u16 BackColor );
void LCD_PutStr( u16 CoordiX, u16 CoordiY, u8 *ChWord, u8 FontStyle, u16 FontColor, u16 BackColor );
void LCD_PutNum( u16 CoordiX, u16 CoordiY, u8 Type, u8 Length, u32 NumData, u16 FontColor, u16 BackColor );

void LCD_TestColoBar( void );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
