/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_flash.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WritePageU8
**功能 : U8 寫入 FLASH
**輸入 : WritePage, *WriteData, DataLen
**輸出 : None
**使用 : Flash_WritePageU8(FLASH_PAGE_ADDR(12), WriteData, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_WritePageU8( u32 WritePage, uc8 *WriteData, u32 DataLen )
{
  u16 Count = 0;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  // Erase Flash
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  while(FLASH_EraseSector(WritePage, VoltageRange_3) != FLASH_COMPLETE);

  // Write Data
  while(Count < DataLen) {
    FLASHStatus = FLASH_ProgramByte(WritePage+Count, WriteData[Count]);
    while(FLASHStatus != FLASH_COMPLETE);
    Count++;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WritePageU16
**功能 : U16 寫入 FLASH
**輸入 : WritePage, *WriteData, DataLen
**輸出 : None
**使用 : Flash_WritePageU16(FLASH_PAGE_ADDR(12), WriteData, 512);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_WritePageU16( u32 WritePage, uc16 *WriteData, u32 DataLen )
{
  u16 Count = 0;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  // Erase Flash
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  while(FLASH_EraseSector(WritePage, VoltageRange_3) != FLASH_COMPLETE);

  // Write Data
  while(Count < DataLen) {
    FLASHStatus = FLASH_ProgramWord(WritePage+(Count<<1), WriteData[Count]);
    while(FLASHStatus != FLASH_COMPLETE);
    Count++;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_WritePageU32
**功能 : U32 寫入 FLASH
**輸入 : WritePage, *WriteData, DataLen
**輸出 : None
**使用 : Flash_WritePageU32(FLASH_PAGE_ADDR(12), WriteData, 256);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_WritePageU32( u32 WritePage, uc32 *WriteData, u32 DataLen )
{
  u16 Count = 0;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  // Erase Flash
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  while(FLASH_EraseSector(WritePage, VoltageRange_3) != FLASH_COMPLETE);

  // Write Data
  while(Count < DataLen) {
    FLASHStatus = FLASH_ProgramHalfWord(WritePage+(Count<<2), WriteData[Count]);
    while(FLASHStatus != FLASH_COMPLETE);
    Count++;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadPageU8
**功能 : U8 讀取 FLASH
**輸入 : ReadPage, *ReadData, DataLen
**輸出 : None
**使用 : Flash_ReadPageU8(FLASH_PAGE_ADDR(12), ReadData, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadPageU8( u32 ReadPage, u8 *ReadData, u32 DataLen )
{
  u16 Count = 0;
  u16 ReadBuf = 0;

  while(Count<DataLen) {
    ReadBuf = (u16)(*(vu32*)(ReadPage+Count));
    ReadData[Count++] = Byte8H(ReadBuf);
    ReadData[Count++] = Byte8L(ReadBuf);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadPageU16
**功能 : U16 讀取 FLASH
**輸入 : ReadPage, *ReadData, DataLen
**輸出 : None
**使用 : Flash_ReadPageU16(FLASH_PAGE_ADDR(12), ReadData, 512);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadPageU16( u32 ReadPage, u16 *ReadData, u32 DataLen )
{
  u16 Count = 0;

  while(Count<DataLen) {
    ReadData[Count] = (*(vu32*)(ReadPage+(Count<<1)));
    Count++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Flash_ReadPageU32
**功能 : U32 讀取 FLASH
**輸入 : ReadPage, *ReadData, DataLen
**輸出 : None
**使用 : Flash_ReadPageU32(FLASH_PAGE_ADDR(12), ReadData, 256);
**=====================================================================================================*/
/*=====================================================================================================*/
void Flash_ReadPageU32( u32 ReadPage, u32 *ReadData, u32 DataLen )
{
  u16 Count = 0;

  while(Count<DataLen) {
    ReadData[Count] = (*(vu32*)(ReadPage+(Count<<2)));
    Count++;
  }
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
void Flash_ErasePage( u32 ErasePage )
{
  FLASH_Status FLASHStatus;

  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  FLASHStatus = FLASH_EraseSector(ErasePage, VoltageRange_3);
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
void Flash_ErasePages( u32 ErasePageStart, u32 ErasePageEnd )
{
  u32 ErasePage = ErasePageStart;
  FLASH_Status FLASHStatus;

  FLASH_Unlock();

  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
  while(ErasePage <= ErasePageEnd) {
    FLASHStatus = FLASH_EraseSector(ErasePage, VoltageRange_3);
    while(FLASHStatus != FLASH_COMPLETE);
    ErasePage += (ErasePage == FLASH_Sector_11)? 40 : 8;
  }

  FLASH_Lock();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
