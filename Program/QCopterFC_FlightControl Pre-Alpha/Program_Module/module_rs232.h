/* #include "module_rs232.h" */

#ifndef __MODULE_RS232_H
#define __MODULE_RS232_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void RS232_Config( void );
void RS232_DMA_Config( void );
void RS232_SendStr( USART_TypeDef* USARTx, uc8 *pWord );
void RS232_SendNum( USART_TypeDef* USARTx, u8 Type, u8 NumLen, s32 SendData );
void RS232_SendData( USART_TypeDef* USARTx, uc8 *SendData, u16 DataLen );
void RS232_RecvStr( USART_TypeDef* USARTx, u8 *pWord );
void RS232_RecvData( USART_TypeDef* USARTx, u8 *RecvData, u16 DataLen );
void RS232_VisualScope( USART_TypeDef*, u8*, u16 );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
