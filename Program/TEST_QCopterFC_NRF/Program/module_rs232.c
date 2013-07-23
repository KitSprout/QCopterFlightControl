/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "module_rs232.h"
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_Config
**功能 : 
**輸入 : 
**輸出 : 
**使用 : RS232_Config()
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

 	/* USART3 Tx PB10 */	/* USART3 Rx PB11 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStruct);
	USART_Cmd(USART3, ENABLE);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : RS232_Print
**功能 : 傳送資料
**輸入 : *USARTx, *pWord
**輸出 : None
**使用 : RS232_Print(USART1, "Hello World\r\n")
**=====================================================================================================*/
/*=====================================================================================================*/
void RS232_Print( USART_TypeDef* USARTx, u8 *pWord )
{
	while(*pWord != '\0') {
		USART_SendData(USARTx, *pWord);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		pWord++;
	}
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
