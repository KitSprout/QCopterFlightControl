/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_flash.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WriteDataU8
**功能 : U8 寫入 FLASH
**輸入 : WriteAddr, *WriteData, DataLen
**輸出 : None
**使用 : Flash_WriteDataU8(FLASH_SECTOR_6, WriteData, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_WriteDataU8( u32 WriteAddr, uc8 *WriteData, u32 DataLen )
{
  u16 Count = 0;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  while(Count < DataLen) {
    FLASHStatus = FLASH_ProgramByte(WriteAddr+Count, WriteData[Count]);
    while(FLASHStatus != FLASH_COMPLETE);
    Count++;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WriteDataU16
**功能 : U16 寫入 FLASH
**輸入 : WriteAddr, *WriteData, DataLen
**輸出 : None
**使用 : Flash_WriteDataU16(FLASH_SECTOR_6, WriteData, 512);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_WriteDataU16( u32 WriteAddr, uc16 *WriteData, u32 DataLen )
{
  u16 Count = 0;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  while(Count < DataLen) {
    FLASHStatus = FLASH_ProgramHalfWord(WriteAddr+(Count<<1), WriteData[Count]);
    while(FLASHStatus != FLASH_COMPLETE);
    Count++;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WriteDataU32
**功能 : U32 寫入 FLASH
**輸入 : WriteAddr, *WriteData, DataLen
**輸出 : None
**使用 : Flash_WriteDataU32(FLASH_SECTOR_6, WriteData, 256);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_WriteDataU32( u32 WriteAddr, uc32 *WriteData, u32 DataLen )
{
  u16 Count = 0;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  while(Count < DataLen) {
    FLASHStatus = FLASH_ProgramWord(WriteAddr+(Count<<2), WriteData[Count]);
    while(FLASHStatus != FLASH_COMPLETE);
    Count++;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadDataU8
**功能 : U8 讀取 FLASH
**輸入 : ReadAddr, *ReadData, DataLen
**輸出 : None
**使用 : Flash_ReadDataU8(FLASH_SECTOR_6, ReadData, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadDataU8( u32 ReadAddr, u8 *ReadData, u32 DataLen )
{
  u16 Count = 0;

  while(Count<DataLen) {
    ReadData[Count] = (u8)(*(vu32*)(ReadAddr+Count));
    Count++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadDataU16
**功能 : U16 讀取 FLASH
**輸入 : ReadAddr, *ReadData, DataLen
**輸出 : None
**使用 : Flash_ReadDataU16(FLASH_SECTOR_6, ReadData, 512);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadDataU16( u32 ReadAddr, u16 *ReadData, u32 DataLen )
{
  u16 Count = 0;

  while(Count<DataLen) {
    ReadData[Count] = (u16)(*(vu32*)(ReadAddr+(Count<<1)));
    Count++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadDataU32
**功能 : U32 讀取 FLASH
**輸入 : ReadAddr, *ReadData, DataLen
**輸出 : None
**使用 : Flash_ReadDataU32(FLASH_SECTOR_6, ReadData, 256);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadDataU32( u32 ReadAddr, u32 *ReadData, u32 DataLen )
{
  u16 Count = 0;

  while(Count<DataLen) {
    ReadData[Count] = (u32)(*(vu32*)(ReadAddr+(Count<<2)));
    Count++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_GetSector
**功能 : 取得所屬 Sector
**輸入 : Address
**輸出 : Sector
**使用 : Flash_GetSector(Address);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 Flash_GetSector( u32 Address )
{
  u32 Sector = 0;

  if((Address >= FLASH_SECTOR_0) && (Address < FLASH_SECTOR_1))
    Sector = FLASH_Sector_0;
  else if((Address >= FLASH_SECTOR_1) && (Address < FLASH_SECTOR_2))
    Sector = FLASH_Sector_1;
  else if((Address >= FLASH_SECTOR_2) && (Address < FLASH_SECTOR_3))
    Sector = FLASH_Sector_2;
  else if((Address >= FLASH_SECTOR_3) && (Address < FLASH_SECTOR_4))
    Sector = FLASH_Sector_3;
  else if((Address >= FLASH_SECTOR_4) && (Address < FLASH_SECTOR_5))
    Sector = FLASH_Sector_4;
  else if((Address >= FLASH_SECTOR_5) && (Address < FLASH_SECTOR_6))
    Sector = FLASH_Sector_5;
  else if((Address >= FLASH_SECTOR_6) && (Address < FLASH_SECTOR_7))
    Sector = FLASH_Sector_6;
  else if((Address >= FLASH_SECTOR_7) && (Address < FLASH_SECTOR_8))
    Sector = FLASH_Sector_7;
  else if((Address >= FLASH_SECTOR_8) && (Address < FLASH_SECTOR_9))
    Sector = FLASH_Sector_8;
  else if((Address >= FLASH_SECTOR_9) && (Address < FLASH_SECTOR_10))
    Sector = FLASH_Sector_9;
  else if((Address >= FLASH_SECTOR_10) && (Address < FLASH_SECTOR_11))
    Sector = FLASH_Sector_10;
  else
    Sector = FLASH_Sector_11;

  return Sector;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ErasePage
**功能 : 擦除 FLASH
**輸入 : ErasePage
**輸出 : None
**使用 : Flash_ErasePage(FLASH_Sector_8);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_EraseSector( u32 EraseSector )
{
  FLASH_Status FLASHStatus;
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  FLASHStatus = FLASH_EraseSector(EraseSector, VoltageRange_3);
  while(FLASHStatus != FLASH_COMPLETE);
  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ErasePages
**功能 : 擦除 FLASH
**輸入 : ErasePageStart, ErasePageEnd
**輸出 : None
**使用 : Flash_ErasePages(FLASH_Sector_8, FLASH_Sector_11);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_EraseSectors( u32 EraseSectorStart, u32 EraseSectorEnd )
{
  u32 EraseSector = EraseSectorStart;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  while(EraseSector <= EraseSectorEnd) {
    FLASHStatus = FLASH_EraseSector(EraseSector, VoltageRange_3);
    while(FLASHStatus != FLASH_COMPLETE);
    EraseSector += (EraseSector == FLASH_Sector_11)? 40 : 8;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
