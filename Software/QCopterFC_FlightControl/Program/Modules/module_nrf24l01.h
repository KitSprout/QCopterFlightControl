/* #include "module_nrf24l01.h" */

#ifndef __MODULE_NRF24L01_H
#define __MODULE_NRF24L01_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define NRF_SPI           SPI2    // nRF 使用 SPI2

#define NRF_PIPE_WIDTH    (5)     // 發射/接收地址寬度
#define NRF_CHANAL        (40)    // 頻道選擇

#define NRF_TX_PL_WIDTH   (32)    // 發射數據通道有效數據寬度 0~32Byte
#define NRF_RX_PL_WIDTH   (32)

#define NRF_MODE_FTLR     (0x00)  // First Tx Last Rx
#define NRF_MODE_FRLT     (0x01)  // First Rx Last Tx

#define NRF_RATE_250Kbps  ((u8)0x26)
#define NRF_RATE_1Mbps    ((u8)0x06)
#define NRF_RATE_2Mbps    ((u8)0x0E)

#define NRF_STA_MAX_RT    ((u8)0x10)  // Maximum number of TX retransmits interrupt Write 1 to clear bit.
#define NRF_STA_TX_DS     ((u8)0x20)  // Data Sent TX FIFO interrupt. If AUTO_ACK is activated, this bit is set high only when ACK is received.
#define NRF_STA_RX_DR     ((u8)0x40)  // Data Ready RX FIFO interrupt. Asserted when new data arrives RX FIFO.

// nRF24L01P Register
#define NRF_CONFIG        ((u8)0x00)
#define NRF_EN_AA         ((u8)0x01)
#define NRF_EN_RXADDR     ((u8)0x02)
#define NRF_SETUP_AW      ((u8)0x03)
#define NRF_SETUP_RETR    ((u8)0x04)
#define NRF_RF_CH         ((u8)0x05)
#define NRF_RF_SETUP      ((u8)0x06)
#define NRF_STATUS        ((u8)0x07)
#define NRF_OBSERVE_TX    ((u8)0x08)
#define NRF_RPD           ((u8)0x09)
#define NRF_RX_ADDR_P0    ((u8)0x0A)
#define NRF_RX_ADDR_P1    ((u8)0x0B)
#define NRF_RX_ADDR_P2    ((u8)0x0C)
#define NRF_RX_ADDR_P3    ((u8)0x0D)
#define NRF_RX_ADDR_P4    ((u8)0x0E)
#define NRF_RX_ADDR_P5    ((u8)0x0F)
#define NRF_TX_ADDR       ((u8)0x10)
#define NRF_RX_PW_P0      ((u8)0x11)
#define NRF_RX_PW_P1      ((u8)0x12)
#define NRF_RX_PW_P2      ((u8)0x13)
#define NRF_RX_PW_P3      ((u8)0x14)
#define NRF_RX_PW_P4      ((u8)0x15)
#define NRF_RX_PW_P5      ((u8)0x16)
#define NRF_FIFO_STATUS   ((u8)0x17)
#define NRF_DYNPD         ((u8)0x1C)
#define NRF_FEATURE       ((u8)0x1D)
/*=====================================================================================================*/
/*=====================================================================================================*/
#define NRF_CSN   PBO(12) // PB12
#define NRF_CE    PCO(4)  // PC4
#define NRF_IRQ   PCI(5)  // PC5
/*=====================================================================================================*/
/*=====================================================================================================*/
void NRF24L01_Config( void );
void NRF24L01_Init( u8 NRF_INIT_MODE );

static u8 NRF_ReadReg( u8 ReadAddr );
static void NRF_WriteReg( u8 WriteAddr, u8 WriteData );
static void NRF_ReadBuf( u8 ReadAddr, u8 *ReadBuf, u8 Bytes );
static void NRF_WriteBuf( u8 WriteAddr, u8 *WriteBuf, u8 Bytes );

//static u8 NRF_NOP( void );
//static u8 NRF_GetStatus( void );
static void NRF_SetChannel( u8 Channel );
static void NRF_SetDataRate( u8 DataRate );
static void NRF_SetAddr( u8 RegAddr, u8 *Address );
static void NRF_FlushTxFIFO( void );
static void NRF_FlushRxFIFO( void );

u8 NRF_Check( void );

//void NRF_TX_Mode( void );   // 直接在 NRF_Tx_Data 中實現
void NRF_RX_Mode( void );
void NRF_TxData( u8 *TxBuf );
void NRF_RxData( u8 *RxBuf );
u8 NRF_TxPacket( u8 *TxBuf );
u8 NRF_RxPacket( u8 *RxBuf );

static u32 NRF_TimeOut( void );
/*====================================================================================================*/
/*====================================================================================================*/
extern u8 TxBuf[NRF_TX_PL_WIDTH];
extern u8 RxBuf[NRF_RX_PL_WIDTH];
/*====================================================================================================*/
/*====================================================================================================*/
#endif
