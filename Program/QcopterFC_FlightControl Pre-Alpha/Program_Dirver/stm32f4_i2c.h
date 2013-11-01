/* #include "stm32f4_i2c.h" */

#ifndef __STM32F4_I2C_H
#define __STM32F4_I2C_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void I2C_Config( void );

u32 I2C_DMA_Read( u8* ReadBuf, u8 SlaveAddr, u8 ReadAddr, u8* NumByte );
u32 I2C_DMA_ReadReg( u8 SlaveAddr, u8 ReadAddr, u8* ReadBuf, u8 NumByte );
u32 I2C_DMA_Write( u8* WriteBuf, u8 SlaveAddr, u8 WriteAddr, u8* NumByte );
u32 I2C_DMA_WriteReg( u8 SlaveAddr, u8 WriteAddr, u8* WriteBuf, u8 NumByte );

void I2C_TX_DMA_IRQ( void );
void I2C_RX_DMA_IRQ( void );

u32 I2C_TimeOut( void );

u32 I2C_MS5611_Read( u8* ReadBuf, u8 SlaveAddr, u8* NumByte );
u32 I2C_MS5611_ReadData( u8* ReadBuf, u8 SlaveAddr, u8 NumByte );
u32 I2C_MS5611_WriteByte( u8 SlaveAddr, u8 WriteByte );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
