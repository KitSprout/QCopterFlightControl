/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "module_rs232.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_Config
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,  ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

  /* USART1 Tx PB10 */	/* USART1 Rx PB11 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStruct);
  USART_Cmd(USART3, ENABLE);

  USART_ClearFlag(USART3, USART_FLAG_TC);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_SendStr
**功能 : 發送字串
**輸入 : USARTx, *pWord
**輸出 : None
**使用 : RS232_SendStr(USART1, (u8*)"Hellow World!");
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendStr( USART_TypeDef* USARTx, uc8 *pWord )
{
  while(*pWord != '\0') {
    USART_SendByte(USARTx, (u8*)pWord);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_SendNum
**功能 : 將數值轉字串發送
**輸入 : USARTx, Type, NumLen, SendData
**輸出 : None
**使用 : RS232_SendNum(USART1, Type_O, 6, 1024);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendNum( USART_TypeDef* USARTx, u8 Type, u8 NumLen, s32 SendData )
{
  u8 TrData[32] = {0};
  u8 *pWord = TrData;

  NumToChar(Type, NumLen, TrData, SendData);
  
  while(*pWord != '\0') {
    USART_SendByte(USARTx, (u8*)pWord);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_SendData
**功能 : 發送資料
**輸入 : USARTx, *SendData, DataLen
**輸出 : None
**使用 : RS232_SendData(USART1, SendData, DataLen);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_SendData( USART_TypeDef* USARTx, uc8 *SendData, u16 DataLen )
{
  do {
    USART_SendByte(USARTx, SendData);
    SendData++;
    DataLen--;
  } while(DataLen);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_RecvStr
**功能 : 接收字串
**輸入 : USARTx, *pWord
**輸出 : None
**使用 : RS232_RecvStr(USART1, Stirng);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_RecvStr( USART_TypeDef* USARTx, u8 *pWord )
{
  do {
    *pWord = USART_RecvByte(USARTx);
    pWord++;
  } while(*(pWord-1) != '\0');
  *pWord = '\0';
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_RecvData
**功能 : 接收資料
**輸入 : USARTx, *RecvData, DataLen
**輸出 : None
**使用 : RS232_RecvData(USART1, RecvData, DataLen);
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_RecvData( USART_TypeDef* USARTx, u8 *RecvData, u16 DataLen )
{
  do {
    *RecvData = USART_RecvByte(USARTx);
    RecvData++;
    DataLen--;
  } while(DataLen);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_VisualScope_CRC16
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
static u16 RS232_VisualScope_CRC16( u8 *Array, u16 Len )
{
  u16 USART_IX, USART_IY, USART_CRC;

  USART_CRC = 0xffff;
  for(USART_IX=0; USART_IX<Len; USART_IX++) {
    USART_CRC = USART_CRC^(u16)(Array[USART_IX]);
    for(USART_IY=0; USART_IY<=7; USART_IY++) {
      if((USART_CRC&1)!=0)
        USART_CRC = (USART_CRC>>1)^0xA001;
      else
        USART_CRC = USART_CRC>>1;
    }
  }
  return(USART_CRC);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_VisualScope
**功能 : 
**輸入 : 
**輸出 : 
**使用 : 
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_VisualScope( USART_TypeDef* USARTx, u8 *pWord, u16 Len )
{
  u8 i = 0;
  u16 Temp = 0;

  Temp = RS232_VisualScope_CRC16(pWord, Len);
  pWord[8] = Temp&0x00ff;
  pWord[9] = (Temp&0xff00)>>8;

  for(i=0; i<10; i++) {
    USART_SendData(USARTx, *pWord);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    pWord++;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
