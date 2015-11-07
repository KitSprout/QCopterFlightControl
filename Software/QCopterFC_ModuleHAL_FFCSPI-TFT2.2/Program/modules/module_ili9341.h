/* #include "module_ili9341.h" */

#ifndef __MODULE_ILI9341_H
#define __MODULE_ILI9341_H

#include "stm32f4xx_hal.h"
#include "algorithms\algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define H_VIEW
#ifdef H_VIEW
  #define LCD_W 240
  #define LCD_H 320
#else
  #define LCD_W 320
  #define LCD_H 240
#endif

#define RGB_TO_GARY(C_R, C_G, C_B)  ((uint8_t)(0.299f*C_R + 0.587f*C_G + 0.114f*C_B))
#define ToRGB565(RGB888)            ((uint16_t)((RGB888&0xF80000>>8)|(RGB888&0x00FC00>>5)|(RGB888&0x0000F8>>3)))
#define ToRGB888(RGB565)            ((uint16_t)((RGB565&0xF800<<8)|(RGB565&0x07E0<<5)|(RGB565&0x001F<<3)))

#define RGB565_R(RGB565)            ((uint8_t)((Color&0xF800)>>11))
#define RGB565_G(RGB565)            ((uint8_t)((Color&0x07E0)>>5))
#define RGB565_B(RGB565)            ((uint8_t)(Color&0x001F))

// 輸入 R G B，輸出 RGB565
#define RGB(C_R, C_G, C_B)  ((uint32_t)(((C_R<<8)&0xF800)|((C_G<<3)&0x07E0)|((C_B>>3)&0x001F)))

#define RED         ((uint16_t)0xF800)  /* 紅色 */
#define GREEN       ((uint16_t)0x07E0)  /* 綠色 */
#define BLUE        ((uint16_t)0x001F)  /* 藍色 */
#define MAGENTA     ((uint16_t)0xF81F)  /* 紅紫色，洋紅色 */
#define GRED        ((uint16_t)0xFFE0)
#define GBLUE       ((uint16_t)0x07FF) 
#define BRED        ((uint16_t)0XF81F)
#define BROWN       ((uint16_t)0xBC40)  /* 棕色 */
#define BRRED       ((uint16_t)0xFC07)  /* 棕紅色 */
#define CYAN        ((uint16_t)0x7FFF)  /* 藍綠色，青色 */
#define BLACK       ((uint16_t)0x0000)  /* 黑色 */
#define GRAY        ((uint16_t)0X8430)  /* 灰色 */
#define WHITE       ((uint16_t)0xFFFF)  /* 白色 */
#define YELLOW      ((uint16_t)0xFFE0)  /* 黃色 */
#define DARKBLUE    ((uint16_t)0x01CF)  /* 深藍色 */
#define LIGHTBLUE   ((uint16_t)0x7D7C)  /* 淺藍色 */
#define GRAYBLUE    ((uint16_t)0x5458)  /* 灰藍色 */
#define LIGHTGREEN  ((uint16_t)0X841F)  /* 淺綠色 */
#define LGRAY       ((uint16_t)0XC618)  /* 淺灰色 */
#define LGRAYBLUE   ((uint16_t)0XA651)  /* 淺灰藍色 */
#define LBBLUE      ((uint16_t)0X2B12)  /* 淺棕藍色 */
/*====================================================================================================*/
/*====================================================================================================*/
static void ILI9341_LigConfig( void );
static void LCD_WriteCmd( uint8_t WriteCmd );
static void LCD_WriteData( uint16_t WriteData );
static void LCD_WriteColor( uint16_t Color );

void ILI9341_Config( void );
void ILI9341_Init( void );

void LCD_Clear( uint16_t Color );

void LCD_SetCursor( uint16_t CoordiX, uint16_t CoordiY );
void LCD_SetWindow( uint16_t StartX, uint16_t StartY, uint16_t EndX, uint16_t EndY );
void LCD_SetBackLight( uint16_t BackLight );

void LCD_DrawPixel( uint16_t CoordiX, uint16_t CoordiY, uint16_t Color );
void LCD_DrawLineX( uint16_t CoordiX, uint16_t CoordiY, uint16_t Length, uint16_t Color );
void LCD_DrawLineY( uint16_t CoordiX, uint16_t CoordiY, uint16_t Length, uint16_t Color );
void LCD_DrawRectangle( uint16_t CoordiX, uint16_t CoordiY, uint16_t Width, uint16_t Height, uint16_t Color );
void LCD_DrawRectangleFill( uint16_t CoordiX, uint16_t CoordiY, uint16_t Width, uint16_t Height, uint16_t Color );
void LCD_DrawCircle( uint16_t CoordiX, uint16_t CoordiY, uint16_t Radius, uint16_t Color );
void LCD_DrawPicture( uint16_t CoordiX, uint16_t CoordiY, uint16_t Width, uint16_t Height, const uint8_t *Pic );

void LCD_PutChar( uint16_t CoordiX, uint16_t CoordiY, char *ChWord, uint16_t FontColor, uint16_t BackColor );
void LCD_PutStr( uint16_t CoordiX, uint16_t CoordiY, char *ChWord, uint16_t FontColor, uint16_t BackColor );
void LCD_PutNum( uint16_t CoordiX, uint16_t CoordiY, StringType Type, uint8_t Length, uint32_t NumData, uint16_t FontColor, uint16_t BackColor );

void LCD_TestColoBar( void );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
