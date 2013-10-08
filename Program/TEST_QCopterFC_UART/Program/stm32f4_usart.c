/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : USART_SendByte
**功能 : 發送 1Byte 資料
**輸入 : USARTx, *SendData
**輸出 : None
**使用 : USART_SendByte(USART1, 'A');
**=====================================================================================================*/
/*=====================================================================================================*/
void USART_SendByte( USART_TypeDef* USARTx, uc8 *SendData )
{
  USART_SendData(USARTx, *SendData);
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : USART_RecvByte
**功能 : 接收 1Byte 資料
**輸入 : USARTx
**輸出 : RecvData
**使用 : RecvData = USART_RecvByte(USART1);
**=====================================================================================================*/
/*=====================================================================================================*/
u16 USART_RecvByte( USART_TypeDef* USARTx )
{
  while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

  return USART_ReceiveData(USARTx);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : NumToChar
**功能 : 數值轉字串
**輸入 : Type, NumLen, *CharData, NumData
**輸出 : None
**使用 : NumToChar(Type_O, 6, TrData, Data)
**=====================================================================================================*/
/*=====================================================================================================*/
void NumToChar( u8 Type, u8 NumLen, u8 *CharData, s32 NumData )
{
  u8 i = 0;
  u32 Tmp[48] = {0};
  u32 NumTmp = 1;

  switch(Type) {

    case Type_B:
    case Type_O:
    case Type_D:
    case Type_H:
      for(i=0; i<NumLen; i++) {
        Tmp[i] = ((u32)NumData) / NumTmp;
        NumTmp = NumTmp * Type;
      }
      for(i=0; i<NumLen; i++) {
        CharData[NumLen-i-1] = Tmp[i] - Tmp[i+1]*Type;
        if(CharData[NumLen-i-1]>9)
          CharData[NumLen-i-1] += 55;	// 65-10
        else
          CharData[NumLen-i-1] += 48;
      }
      CharData[NumLen] = '\0';
    break;

    case Type_I:
      if(NumData<0) {
        CharData[0] = '-';
        NumData = (~NumData) + 1;
        for(i=1; i<NumLen+1; i++) {
          Tmp[i-1] = ((u32)NumData) / NumTmp;
          NumTmp = NumTmp * 10;
        }
        for(i=1; i<NumLen+1; i++) {
          CharData[NumLen-i+1] = Tmp[i-1] - Tmp[i]*10;
          CharData[NumLen-i+1] += 48;
        }
        CharData[NumLen+1] = '\0';
      }
      else {
        CharData[0] = '+';
        //NumData = (~NumData) + 1;
        for(i=1; i<NumLen+1; i++) {
          Tmp[i-1] = ((u32)NumData) / NumTmp;
          NumTmp = NumTmp*10;
        }
        for(i=1; i<NumLen+1; i++) {
          CharData[NumLen-i+1] = Tmp[i-1] - Tmp[i]*10;
          CharData[NumLen-i+1] += 48;
        }
        CharData[NumLen+1] = '\0';
      }
      break;

    case Type_F:
      break;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
