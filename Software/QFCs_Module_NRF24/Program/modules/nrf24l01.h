/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    nrf24l01.h
  * @author  KitSprout
  * @date    22-Mar-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF24L01_H
#define __NRF24L01_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"

/* Exported types --------------------------------------------------------------------------*/

typedef enum {
  NRF_RXP0 = 0x01,
  NRF_RXP1 = 0x02,
  NRF_RXP2 = 0x04,
  NRF_RXP3 = 0x08,
  NRF_RXP4 = 0x10,
  NRF_RXP5 = 0x20
} NRF_RxDataPipe_TypeDef;

typedef enum {
  NRF_ARD_250us  = 0x00,
  NRF_ARD_500us  = 0x10,
  NRF_ARD_750us  = 0x20,
  NRF_ARD_1000us = 0x30,
  NRF_ARD_1250us = 0x40,
  NRF_ARD_1500us = 0x50,
  NRF_ARD_1750us = 0x60,
  NRF_ARD_2000us = 0x70,
  NRF_ARD_2250us = 0x80,
  NRF_ARD_2500us = 0x90,
  NRF_ARD_2750us = 0xA0,
  NRF_ARD_3000us = 0xB0,
  NRF_ARD_3250us = 0xC0,
  NRF_ARD_3500us = 0xD0,
  NRF_ARD_3750us = 0xE0,
  NRF_ARD_4000us = 0xF0,
} NRF_AutoReTxDelay_TypeDef;

typedef enum {
  NRF_ARC_Disable = 0x00,
  NRF_ARC_UpTo1   = 0x10,
  NRF_ARC_UpTo2   = 0x20,
  NRF_ARC_UpTo3   = 0x30,
  NRF_ARC_UpTo4   = 0x40,
  NRF_ARC_UpTo5   = 0x50,
  NRF_ARC_UpTo6   = 0x60,
  NRF_ARC_UpTo7   = 0x70,
  NRF_ARC_UpTo8   = 0x80,
  NRF_ARC_UpTo9   = 0x90,
  NRF_ARC_UpTo10  = 0xA0,
  NRF_ARC_UpTo11  = 0xB0,
  NRF_ARC_UpTo12  = 0xC0,
  NRF_ARC_UpTo13  = 0xD0,
  NRF_ARC_UpTo14  = 0xE0,
  NRF_ARC_UpTo15  = 0xF0,
} NRF_AutoReTxCount_TypeDef;

typedef enum {
  NRF_DataRate_250Kbps = 0x20,
  NRF_DataRate_1Mbps   = 0x00,
  NRF_DataRate_2Mbps   = 0x08
} NRF_DataRate_TypeDef;

typedef enum {
  NRF_TxPower_N18dBm = 0x00,
  NRF_TxPower_N12dBm = 0x02,
  NRF_TxPower_N6dBm  = 0x04,
  NRF_TxPower_0dBm   = 0x06
} NRF_TxPower_TypeDef;

typedef enum {
  NRF_Mode_TX = 0x00,
  NRF_Mode_RX = 0x01,
} NRF_Mode_TypeDef;

typedef enum {
  NRF_OPMODE_POWER_DOWN = 0x00,
  NRF_OPMODE_STANDBY    = 0x01,
  NRF_OPMODE_RX         = 0x02,
  NRF_OPMODE_TX         = 0x03
} NRF_OPMODE_TypeDef;

typedef struct {
  uint8_t *TxAddr;
  uint8_t *RxP0Addr;
  uint8_t *RxP1Addr;
  uint8_t *RxP2Addr;
  uint8_t *RxP3Addr;
  uint8_t *RxP4Addr;
  uint8_t *RxP5Addr;
  int8_t NRF_DynamicPayload;
  uint8_t NRF_FixPacketLens;
  uint8_t NRF_Channel;

  NRF_AutoReTxDelay_TypeDef NRF_AutoReTxDelay;
  NRF_AutoReTxCount_TypeDef NRF_AutoReTxCount;
  NRF_DataRate_TypeDef NRF_DataRate;
  NRF_TxPower_TypeDef NRF_TxPower;
} NRF_ConfigTypeDef;

typedef struct {
  __IO uint8_t txFlag;
  __IO uint8_t rxFlag;
  uint8_t txLens;
  uint8_t rxLens;
  uint8_t *txBuff;
  uint8_t *rxBuff;
  uint8_t irqStatus;
  uint32_t txTimeout;
  uint32_t rxTimeout;
  pFunc txEventCallback;
  pFunc rxEventCallback;
} __attribute__((aligned)) NRF_DataTypeDef;

/* Exported constants ----------------------------------------------------------------------*/

#define NRF_ADDR_WIDTHS         (5)
#define NRF_TX_MAX_PL           (32)
#define NRF_RX_MAX_PL           (32)

/* nrf24l01 command */
#define CMD_R_REG               ((uint8_t)0x00)
#define CMD_W_REG               ((uint8_t)0x20)
#define CMD_R_RX_PLOAD          ((uint8_t)0x61)
#define CMD_W_TX_PLOAD          ((uint8_t)0xA0)
#define CMD_FLUSH_TX            ((uint8_t)0xE1)
#define CMD_FLUSH_RX            ((uint8_t)0xE2)
#define CMD_REUSE_TX_PL         ((uint8_t)0xE3)
#define CMD_RX_PL_WID           ((uint8_t)0x60)
#define CMD_W_ACK_PLOAD(__P)    ((uint8_t)(0xA8 | (__P & 0x07)))
#define CMD_W_TX_PLOAD_NOACK    ((uint8_t)0xB0)
#define CMD_NOP                 ((uint8_t)0xFF)

/* nrf24l01 state */
#define NRF_STA_TX_FULL         ((uint8_t)0x01)
#define NRF_STA_RX_P_NO         ((uint8_t)0x0E)
#define NRF_STA_MAX_RT          ((uint8_t)0x10)
#define NRF_STA_TX_DS           ((uint8_t)0x20)
#define NRF_STA_RX_DR           ((uint8_t)0x40)

#define NRF_STA_NULL            ((uint8_t)0x00)
#define NRF_STA_BUSY            ((uint8_t)0x08)
#define NRF_STA_TIMUOUT         ((uint8_t)0x80)
#define NRF_STA_OK              ((uint8_t)0x0D)
#define NRF_STA_ERROR           ((uint8_t)0xFF)

/* nrf24l01 register */
#define NRF24L01_CONFIG         ((uint8_t)0x00)
#define NRF24L01_EN_AA          ((uint8_t)0x01)
#define NRF24L01_EN_RXADDR      ((uint8_t)0x02)
#define NRF24L01_SETUP_AW       ((uint8_t)0x03)
#define NRF24L01_SETUP_RETR     ((uint8_t)0x04)
#define NRF24L01_RF_CH          ((uint8_t)0x05)
#define NRF24L01_RF_SETUP       ((uint8_t)0x06)
#define NRF24L01_STATUS         ((uint8_t)0x07)
#define NRF24L01_OBSERVE_TX     ((uint8_t)0x08)
#define NRF24L01_RPD            ((uint8_t)0x09)
#define NRF24L01_RX_ADDR_P0     ((uint8_t)0x0A)
#define NRF24L01_RX_ADDR_P1     ((uint8_t)0x0B)
#define NRF24L01_RX_ADDR_P2     ((uint8_t)0x0C)
#define NRF24L01_RX_ADDR_P3     ((uint8_t)0x0D)
#define NRF24L01_RX_ADDR_P4     ((uint8_t)0x0E)
#define NRF24L01_RX_ADDR_P5     ((uint8_t)0x0F)
#define NRF24L01_TX_ADDR        ((uint8_t)0x10)
#define NRF24L01_RX_PW_P0       ((uint8_t)0x11)
#define NRF24L01_RX_PW_P1       ((uint8_t)0x12)
#define NRF24L01_RX_PW_P2       ((uint8_t)0x13)
#define NRF24L01_RX_PW_P3       ((uint8_t)0x14)
#define NRF24L01_RX_PW_P4       ((uint8_t)0x15)
#define NRF24L01_RX_PW_P5       ((uint8_t)0x16)
#define NRF24L01_FIFO_STATUS    ((uint8_t)0x17)
#define NRF24L01_DYNPD          ((uint8_t)0x1C)
#define NRF24L01_FEATURE        ((uint8_t)0x1D)

/* Exported functions ----------------------------------------------------------------------*/

uint8_t NRF24_WriteCommand( uint8_t writeCommand );

void    NRF24_Config( void );
int8_t  NRF24_Init( NRF_ConfigTypeDef *NRFx );
int8_t  NRF24_DeviceCheck( void );
uint8_t NRF24_ClearIRQFlag( uint8_t IRQSource );
void    NRF24_SetChannel( uint8_t channel );
void    NRF24_SetTxAddress( uint8_t *address );
void    NRF24_SetRxAddress( uint8_t index, uint8_t *address );
void    NRF24_EnableRxAutoAck( uint8_t index );
void    NRF24_EnableRxAddress( uint8_t index );
void    NRF24_SetDataRate( NRF_DataRate_TypeDef dataRate );
void    NRF24_SetTxPower( NRF_TxPower_TypeDef txPower );
void    NRF24_SetMode( NRF_Mode_TypeDef mode );
uint8_t NRF24_ReceivedPowerDetector( void );
void    NRF24_OperationMode( NRF_OPMODE_TypeDef mode );
uint8_t NRF24_TxPacket( uint8_t *txBuff, uint8_t lens, uint32_t timeout );
uint8_t NRF24_TxPacketDef( void );
void    NRF24_TxPacketIRQ( void );
uint8_t NRF24_RxPacket( uint8_t *rxBuff, uint8_t *lens, uint32_t timeout );
uint8_t NRF24_RxPacketDef( void );
void    NRF24_IrqEvent( void );

void    NRF24_TxCallback( void );
void    NRF24_RxCallback( void );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
