/* #include "stm32f4_flash.h" */

#ifndef __STM32F4_FLASH_H
#define __STM32F4_FLASH_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define FLASH_SECTOR_0          ((u32)0x08000000) // Sector 0,   16 Kbytes,   16 KBytes
#define FLASH_SECTOR_1          ((u32)0x08004000) // Sector 1,   16 Kbytes,   32 KBytes
#define FLASH_SECTOR_2          ((u32)0x08008000) // Sector 2,   16 Kbytes,   48 KBytes
#define FLASH_SECTOR_3          ((u32)0x0800C000) // Sector 3,   16 Kbytes,   64 KBytes
#define FLASH_SECTOR_4          ((u32)0x08010000) // Sector 4,   64 Kbytes,  128 KBytes
#define FLASH_SECTOR_5          ((u32)0x08020000) // Sector 5,  128 Kbytes,  256 KBytes
#define FLASH_SECTOR_6          ((u32)0x08040000) // Sector 6,  128 Kbytes,  384 KBytes
#define FLASH_SECTOR_7          ((u32)0x08060000) // Sector 7,  128 Kbytes,  512 KBytes
#define FLASH_SECTOR_8          ((u32)0x08080000) // Sector 8,  128 Kbytes,  640 KBytes
#define FLASH_SECTOR_9          ((u32)0x080A0000) // Sector 9,  128 Kbytes,  768 KBytes
#define FLASH_SECTOR_10         ((u32)0x080C0000) // Sector 10, 128 Kbytes,  896 KBytes
#define FLASH_SECTOR_11         ((u32)0x080E0000) // Sector 11, 128 Kbytes, 1024 KBytes
/*=====================================================================================================*/
/*=====================================================================================================*/
void Flash_WriteDataU8( u32 WriteAddr, uc8 *WriteData, u32 DataLen );
void Flash_WriteDataU16( u32 WriteAddr, uc16 *WriteData, u32 DataLen );
void Flash_WriteDataU32( u32 WriteAddr, uc32 *WriteData, u32 DataLen );
void Flash_ReadDataU8( u32 ReadAddr, u8 *ReadData, u32 DataLen );
void Flash_ReadDataU16( u32 ReadAddr, u16 *ReadData, u32 DataLen );
void Flash_ReadDataU32( u32 ReadAddr, u32 *ReadData, u32 DataLen );
u32 Flash_GetSector( u32 Address );
void Flash_EraseSector( u32 EraseSector );
void Flash_EraseSectors( u32 EraseSectorStart, u32 EraseSectorEnd );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif	 
