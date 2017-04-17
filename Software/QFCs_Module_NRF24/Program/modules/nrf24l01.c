/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    nrf24l01.c
  * @author  KitSprout
  * @date    22-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\nrf24l01.h"

#include <stdio.h>
#include <string.h>

/** @addtogroup STM32_Module
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
//static uint8_t NRF_SPI_TX_BUFFER[NRF24_MAX_TXBUF] = {0};
//static uint8_t NRF_SPI_RX_BUFFER[NRF24_MAX_RXBUF] = {0};

static __IO NRF_OPMODE_TypeDef opmode = NRF_OPMODE_POWER_DOWN;

static uint8_t NRF_TX_BUFFER[NRF_TX_MAX_PL] = {0};
static uint8_t NRF_RX_BUFFER[NRF_RX_MAX_PL] = {0};

NRF_DataTypeDef nrf = {
  .txFlag    = NRF_STA_NULL,
  .rxFlag    = NRF_STA_NULL,
  .txLens    = 0,
  .rxLens    = 0,
  .txTimeout = 1000,
  .rxTimeout = 1000,
  .txBuff    = NRF_TX_BUFFER,
  .rxBuff    = NRF_RX_BUFFER,
  .irqStatus = DISABLE,
  .txEventCallback = NULL,
  .rxEventCallback = NULL
};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  NRF24_WriteCommand
  */
uint8_t NRF24_WriteCommand( uint8_t writeCommand )
{
  uint8_t readByte;

  NRF24_CSN_L();
  readByte = SPI_RW(hNrf24.handle, writeCommand);
  NRF24_CSN_H();

  return readByte;
}

/**
  * @brief  NRF24_WriteReg
  */
void NRF24_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  NRF24_CSN_L();
  SPI_RW(hNrf24.handle, CMD_W_REG | writeAddr);
  SPI_RW(hNrf24.handle, writeData);
  NRF24_CSN_H();
}

/**
  * @brief  NRF24_WriteRegs
  */
void NRF24_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  NRF24_CSN_L();
  SPI_RW(hNrf24.handle, CMD_W_REG | writeAddr);
  for (uint8_t i = 0; i < lens; i++) {
    SPI_RW(hNrf24.handle, writeData[i]);
  }
  NRF24_CSN_H();
}

/**
  * @brief  NRF24_ReadReg
  */
uint8_t NRF24_ReadReg( uint8_t readAddr )
{
  uint8_t readData;

  NRF24_CSN_L();
  SPI_RW(hNrf24.handle, CMD_R_REG | readAddr);
  readData = SPI_RW(hNrf24.handle, 0xFF);
  NRF24_CSN_H();

  return readData;
}

/**
  * @brief  NRF24_ReadRegs
  */
void NRF24_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  NRF24_CSN_L();
  SPI_RW(hNrf24.handle, CMD_R_REG | readAddr);
  for (uint8_t i = 0; i < lens; i++) {
    readData[i] = SPI_RW(hNrf24.handle, 0xFF);
  }
  NRF24_CSN_H();
}

/**
  * @brief  NRF24_Config
  */
void NRF24_Config( void )
{
 	GPIO_InitTypeDef  GPIO_InitStruct;

//  hNrf24.pTxBuf = NRF_SPI_TX_BUFFER;
//  hNrf24.pRxBuf = NRF_SPI_RX_BUFFER;

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;

  GPIO_InitStruct.Pin   = NRF24_CSN_PIN;
  HAL_GPIO_Init(NRF24_CSN_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = NRF24_CE_PIN;
  HAL_GPIO_Init(NRF24_CE_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;

  GPIO_InitStruct.Pin  = NRF24_IRQ_PIN;
  HAL_GPIO_Init(NRF24_IRQ_GPIO_PORT, &GPIO_InitStruct);

  NRF24_CE_L();
  NRF24_CSN_H();  /* LOW ENABLE */

  /* SPI Init ****************************************************************/
  hNrf24.handle->Instance               = NRF24_SPIx;
  hNrf24.handle->Init.Mode              = SPI_MODE_MASTER;
  hNrf24.handle->Init.Direction         = SPI_DIRECTION_2LINES;
  hNrf24.handle->Init.DataSize          = SPI_DATASIZE_8BIT;
  hNrf24.handle->Init.CLKPolarity       = SPI_POLARITY_LOW;
  hNrf24.handle->Init.CLKPhase          = SPI_PHASE_1EDGE;
  hNrf24.handle->Init.NSS               = SPI_NSS_SOFT;
  hNrf24.handle->Init.BaudRatePrescaler = NRF24_SPIx_SPEED;
  hNrf24.handle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hNrf24.handle->Init.TIMode            = SPI_TIMODE_DISABLE;
  hNrf24.handle->Init.CRCCalculation    = SPI_CRCCALCULATION_ENABLE;
  hNrf24.handle->Init.CRCPolynomial     = 7;
  HAL_SPI_Init(hNrf24.handle);

  __HAL_SPI_ENABLE(hNrf24.handle);

  /* IRQ EXTI IT ***************************************************************/
  HAL_NVIC_SetPriority(NRF24_IRQ_EXTIx_IRQ, NRF24_IRQ_EXTIx_IRQn_PREEMPT, NRF24_IRQ_EXTIx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(NRF24_IRQ_EXTIx_IRQ);
}

/**
  * @brief  NRF24_Init
  */
int8_t NRF24_Init( NRF_ConfigTypeDef *NRFx )
{
  int8_t status;

  status = NRF24_DeviceCheck();
  if (status == ERROR) {
    return ERROR;
  }

  NRF24_CE_L();
  NRF24_ClearIRQFlag(NRF_STA_RX_DR | NRF_STA_TX_DS | NRF_STA_MAX_RT);

  if (NRFx->NRF_DynamicPayload == ENABLE) {
    NRF24_WriteReg(NRF24L01_DYNPD, 0x01);
    NRF24_WriteReg(NRF24L01_FEATURE, 0x07);
  }
  else {
    NRF24_WriteReg(NRF24L01_RX_PW_P0, NRFx->NRF_FixPacketLens);
  }

  NRF24_WriteReg(NRF24L01_CONFIG, 0x0A);    /* power up, enable 1 byte CRC, no mask */
  NRF24_EnableRxAutoAck(NRF_RXP0);
  NRF24_EnableRxAddress(NRF_RXP0);
  NRF24_WriteReg(NRF24L01_SETUP_AW, 0x03);  /* 5 bytes address widths  */
  NRF24_WriteReg(NRF24L01_SETUP_RETR, NRFx->NRF_AutoReTxDelay | NRFx->NRF_AutoReTxCount);
  NRF24_SetChannel(NRFx->NRF_Channel);
  NRF24_SetTxPower(NRFx->NRF_TxPower);
  NRF24_SetDataRate(NRFx->NRF_DataRate);
  NRF24_SetTxAddress(NRFx->TxAddr);
  NRF24_SetRxAddress(NRF_RXP0, NRFx->RxP0Addr);

  opmode = NRF_OPMODE_STANDBY;

  return SUCCESS;
}

/**
  * @brief  NRF24_DeviceCheck
  */
int8_t NRF24_DeviceCheck( void )
{
  uint8_t sendBuf[NRF_ADDR_WIDTHS] = "KSNRF";
  uint8_t recvBuf[NRF_ADDR_WIDTHS] = { 0 };

  NRF24_WriteRegs(NRF24L01_TX_ADDR, sendBuf, NRF_ADDR_WIDTHS);
  NRF24_ReadRegs(NRF24L01_TX_ADDR, recvBuf, NRF_ADDR_WIDTHS);

  for (uint8_t i = 0; i < 5; i++) {
    if (recvBuf[i] != sendBuf[i]) {
      return ERROR;
    }
  }

  return SUCCESS;
}

/**
  * @brief  NRF24_ClearIRQFlag
  */
uint8_t NRF24_ClearIRQFlag( uint8_t irqSource )
{
  uint8_t status;

  status = NRF24_WriteCommand(CMD_NOP);
  irqSource &= (NRF_STA_RX_DR | NRF_STA_TX_DS | NRF_STA_MAX_RT);
  NRF24_WriteReg(NRF24L01_STATUS, irqSource | status);

  return (NRF24_WriteCommand(CMD_NOP));
}

/**
  * @brief  NRF24_SetChannel
  */
void NRF24_SetChannel( uint8_t channel )
{
  NRF24_WriteReg(NRF24L01_RF_CH, channel & 0x7F);
}

/**
  * @brief  NRF24_SetTxAddress
  */
void NRF24_SetTxAddress( uint8_t *address )
{
  NRF24_WriteRegs(NRF24L01_TX_ADDR, address, 5);
}

/**
  * @brief  NRF24_SetRxAddress
  */
void NRF24_SetRxAddress( uint8_t index, uint8_t *address )
{
  switch (index) {
    case NRF_RXP0:  NRF24_WriteRegs(NRF24L01_RX_ADDR_P0, address, 5);   break;
    case NRF_RXP1:  NRF24_WriteRegs(NRF24L01_RX_ADDR_P1, address, 5);   break;
    case NRF_RXP2:  NRF24_WriteRegs(NRF24L01_RX_ADDR_P2, address, 1);   break;
    case NRF_RXP3:  NRF24_WriteRegs(NRF24L01_RX_ADDR_P3, address, 1);   break;
    case NRF_RXP4:  NRF24_WriteRegs(NRF24L01_RX_ADDR_P4, address, 1);   break;
    case NRF_RXP5:  NRF24_WriteRegs(NRF24L01_RX_ADDR_P5, address, 1);   break;
  }
}

/**
  * @brief  NRF24_SetDataRate
  */
void NRF24_SetDataRate( NRF_DataRate_TypeDef dataRate )
{
  uint8_t tmp;

  tmp = NRF24_ReadReg(NRF24L01_RF_SETUP) & ~0x28;
  NRF24_WriteReg(NRF24L01_RF_SETUP, tmp | dataRate);
}

/**
  * @brief  NRF24_SetTxPower
  */
void NRF24_SetTxPower( NRF_TxPower_TypeDef txPower )
{
  uint8_t tmp;

  tmp = NRF24_ReadReg(NRF24L01_RF_SETUP) & ~0x06;
  NRF24_WriteReg(NRF24L01_RF_SETUP, tmp | txPower);
}

/**
  * @brief  NRF24_SetMode
  */
void NRF24_SetMode( NRF_Mode_TypeDef mode )
{
  NRF24_WriteReg(NRF24L01_CONFIG, 0x0A | mode);
}

/**
  * @brief  NRF24_EnableRxAutoAck
  */
void NRF24_EnableRxAutoAck( uint8_t index )
{
  NRF24_WriteReg(NRF24L01_EN_AA, index & 0x3F);
}

/**
  * @brief  NRF24_EnableRxAddress
  */
void NRF24_EnableRxAddress( uint8_t index )
{
  NRF24_WriteReg(NRF24L01_EN_RXADDR, index & 0x3F);
}

/**
  * @brief  NRF24_ReceivedPowerDetector
  */
uint8_t NRF24_ReceivedPowerDetector( void )
{
  return (NRF24_ReadReg(NRF24L01_RPD) & 0xFE);
}

/**
  * @brief  NRF24_OperationMode
  */
void NRF24_OperationMode( NRF_OPMODE_TypeDef mode )
{
  NRF24_CE_L();
  switch (mode) {
    case NRF_OPMODE_POWER_DOWN:
      NRF24_WriteReg(NRF24L01_CONFIG, 0x0A & ~0x02);
      break;
    case NRF_OPMODE_STANDBY:
      NRF24_WriteCommand(CMD_FLUSH_TX);
      NRF24_WriteCommand(CMD_FLUSH_RX);
      break;
    case NRF_OPMODE_RX:
      NRF24_SetMode(NRF_Mode_RX);
      NRF24_WriteCommand(CMD_FLUSH_RX);
      NRF24_CE_H();
//      delay_us(150);
      break;
    case NRF_OPMODE_TX:
      NRF24_SetMode(NRF_Mode_TX);
      NRF24_CE_H();
//      delay_us(150);
      break;
  }
  opmode = mode;
}

/**
  * @brief  NRF24_TxPacket
  */
uint8_t NRF24_TxPacket( uint8_t *txBuff, uint8_t lens, uint32_t timeout )
{
  uint8_t status;
  uint32_t tickStart = 0;

  switch (opmode) {
    case NRF_OPMODE_POWER_DOWN:
      return NRF_STA_ERROR;
    case NRF_OPMODE_RX:
      NRF24_WriteCommand(CMD_FLUSH_RX);
    case NRF_OPMODE_STANDBY:
      NRF24_CE_L();
      NRF24_WriteRegs(CMD_W_TX_PLOAD, txBuff, lens);
      NRF24_SetMode(NRF_Mode_TX);
      NRF24_CE_H();
      delay_us(150);
      opmode = NRF_OPMODE_TX;
      break;
    case NRF_OPMODE_TX:
      NRF24_WriteRegs(CMD_W_TX_PLOAD, txBuff, lens);
      break;
  }

  tickStart = HAL_GetTick();
	while (NRF24_IRQ_Read()) {
    if ((HAL_GetTick() - tickStart) > timeout) {
      NRF24_WriteCommand(CMD_FLUSH_TX);
      NRF24_CE_L();
      opmode = NRF_OPMODE_STANDBY;
      return NRF_STA_TIMUOUT;
    }
	}

	status = NRF24_ReadReg(NRF24L01_STATUS);
  NRF24_WriteReg(NRF24L01_STATUS, status);

	if (status & NRF_STA_MAX_RT) {
    NRF24_WriteCommand(CMD_FLUSH_TX);
    return NRF_STA_MAX_RT; 
	}
	else if (status & NRF_STA_TX_DS) {
		return NRF_STA_TX_DS;
	}

  NRF24_WriteCommand(CMD_FLUSH_TX);
  NRF24_CE_L();
  opmode = NRF_OPMODE_STANDBY;

	return NRF_STA_ERROR;
}

/**
  * @brief  NRF24_TxPacketDef
  */
uint8_t NRF24_TxPacketDef( void )
{
  uint8_t status;
  uint32_t tickStart = 0;

  switch (opmode) {
    case NRF_OPMODE_POWER_DOWN:
      nrf.txFlag = NRF_STA_ERROR;
      return NRF_STA_ERROR;
    case NRF_OPMODE_RX:
      NRF24_WriteCommand(CMD_FLUSH_RX);
    case NRF_OPMODE_STANDBY:
      NRF24_CE_L();
      NRF24_WriteRegs(CMD_W_TX_PLOAD, nrf.txBuff, nrf.txLens);
      NRF24_SetMode(NRF_Mode_TX);
      NRF24_CE_H();
      delay_us(200);
      opmode = NRF_OPMODE_TX;
      break;
    case NRF_OPMODE_TX:
      NRF24_WriteRegs(CMD_W_TX_PLOAD, nrf.txBuff, nrf.txLens);
      break;
  }

  tickStart = HAL_GetTick();
	while (NRF24_IRQ_Read()) {
    if ((HAL_GetTick() - tickStart) > nrf.txTimeout) {
      nrf.txFlag = NRF_STA_TIMUOUT;
      NRF24_WriteCommand(CMD_FLUSH_TX);
      NRF24_CE_L();
      opmode = NRF_OPMODE_STANDBY;
      return NRF_STA_TIMUOUT;
    }
	}

	status = NRF24_ReadReg(NRF24L01_STATUS);
  NRF24_WriteReg(NRF24L01_STATUS, status);

	if (status & NRF_STA_MAX_RT) {
    nrf.txFlag = NRF_STA_MAX_RT;
    NRF24_WriteCommand(CMD_FLUSH_TX);
    return NRF_STA_MAX_RT; 
	}
	else if (status & NRF_STA_TX_DS) {
    nrf.txFlag = NRF_STA_TX_DS;
		return NRF_STA_TX_DS;
	}

  nrf.txFlag = NRF_STA_ERROR;
  NRF24_WriteCommand(CMD_FLUSH_TX);
  NRF24_CE_L();
  opmode = NRF_OPMODE_STANDBY;

	return NRF_STA_ERROR;
}

/**
  * @brief  NRF24_TxPacketIRQ
  */
void NRF24_TxPacketIRQ( void )
{
  if (nrf.txLens != 0) {
    nrf.irqStatus = DISABLE;
    switch (opmode) {
      case NRF_OPMODE_POWER_DOWN:
        nrf.txFlag = NRF_STA_ERROR;
        return;
      case NRF_OPMODE_RX:
        NRF24_WriteCommand(CMD_FLUSH_RX);
      case NRF_OPMODE_STANDBY:
        NRF24_CE_L();
        NRF24_WriteRegs(CMD_W_TX_PLOAD, nrf.txBuff, nrf.txLens);
        NRF24_SetMode(NRF_Mode_TX);
        NRF24_CE_H();
        delay_us(150);
        opmode = NRF_OPMODE_TX;
        break;
      case NRF_OPMODE_TX:
        NRF24_WriteRegs(CMD_W_TX_PLOAD, nrf.txBuff, nrf.txLens);
        break;
    }
    nrf.txLens = 0;
    nrf.irqStatus = ENABLE;
  }
}

/**
  * @brief  NRF24_RxPacket
  */
uint8_t NRF24_RxPacket( uint8_t *rxBuff, uint8_t *lens, uint32_t timeout )
{
  uint8_t status;
  uint32_t tickStart = 0;

  *lens = 0;

  switch (opmode) {
    case NRF_OPMODE_POWER_DOWN:
      return NRF_STA_ERROR;
    case NRF_OPMODE_TX:
      NRF24_WriteCommand(CMD_FLUSH_TX);
    case NRF_OPMODE_STANDBY:
      NRF24_CE_L();
      NRF24_SetMode(NRF_Mode_RX);
      NRF24_CE_H();
      delay_us(150);
      opmode = NRF_OPMODE_RX;
      break;
    case NRF_OPMODE_RX:
      break;
  }

  tickStart = HAL_GetTick();
  while (NRF24_IRQ_Read()) {
    if ((HAL_GetTick() - tickStart) > timeout) {
      NRF24_WriteCommand(CMD_FLUSH_RX);
      NRF24_CE_L();
      opmode = NRF_OPMODE_STANDBY;
      return NRF_STA_TIMUOUT;
    }
  }

  status = NRF24_ReadReg(NRF24L01_STATUS);
  NRF24_WriteReg(NRF24L01_STATUS, status);

  if (status & NRF_STA_RX_DR) {
    *lens = NRF24_ReadReg(CMD_RX_PL_WID);
    if (*lens > 32) {
      *lens = 32;
    }
    NRF24_ReadRegs(CMD_R_RX_PLOAD, rxBuff, *lens);
    return NRF_STA_RX_DR;
  }

  NRF24_WriteCommand(CMD_FLUSH_RX);
  NRF24_CE_L();
  opmode = NRF_OPMODE_STANDBY;

  return NRF_STA_ERROR;
}

/**
  * @brief  NRF24_RxPacketDef
  */
uint8_t NRF24_RxPacketDef( void )
{
  uint8_t status;
  uint32_t tickStart = 0;

  nrf.rxLens = 0;

  switch (opmode) {
    case NRF_OPMODE_POWER_DOWN:
      return NRF_STA_ERROR;
    case NRF_OPMODE_TX:
      NRF24_WriteCommand(CMD_FLUSH_TX);
    case NRF_OPMODE_STANDBY:
      NRF24_CE_L();
      NRF24_SetMode(NRF_Mode_RX);
      NRF24_CE_H();
      delay_us(150);
      opmode = NRF_OPMODE_RX;
      break;
    case NRF_OPMODE_RX:
      break;
  }

  tickStart = HAL_GetTick();
  while (NRF24_IRQ_Read()) {
    if ((HAL_GetTick() - tickStart) > nrf.rxTimeout) {
      nrf.rxFlag = NRF_STA_TIMUOUT;
      NRF24_WriteCommand(CMD_FLUSH_RX);
      NRF24_CE_L();
      opmode = NRF_OPMODE_STANDBY;
      return NRF_STA_TIMUOUT;
    }
  }

  status = NRF24_ReadReg(NRF24L01_STATUS);
  NRF24_WriteReg(NRF24L01_STATUS, status);

  if (status & NRF_STA_RX_DR) {
    nrf.rxFlag = NRF_STA_RX_DR;
    nrf.rxLens = NRF24_ReadReg(CMD_RX_PL_WID);
    if (nrf.rxLens > 32)
      nrf.rxLens = 32;
    NRF24_ReadRegs(CMD_R_RX_PLOAD, nrf.rxBuff, nrf.rxLens);
    NRF24_WriteCommand(CMD_FLUSH_RX);
    return NRF_STA_RX_DR;
  }

  nrf.rxFlag = NRF_STA_ERROR;
  NRF24_WriteCommand(CMD_FLUSH_RX);
  NRF24_CE_L();
  opmode = NRF_OPMODE_STANDBY;

  return NRF_STA_ERROR;
}

/**
  * @brief  NRF24_IrqEvent
  */
void NRF24_IrqEvent( void )
{
  uint8_t status;

  if (nrf.irqStatus == ENABLE) {
    status = NRF24_ReadReg(NRF24L01_STATUS);
    NRF24_WriteReg(NRF24L01_STATUS, status);
    switch (opmode) {
      case NRF_OPMODE_POWER_DOWN:
      case NRF_OPMODE_STANDBY:
        NRF24_CE_L();
        break;

      case NRF_OPMODE_RX:
        if (status & NRF_STA_RX_DR) {
          nrf.rxFlag = NRF_STA_RX_DR;
          nrf.rxLens = NRF24_ReadReg(CMD_RX_PL_WID);
          if (nrf.rxLens > 32)
            nrf.rxLens = 32;
          NRF24_ReadRegs(CMD_R_RX_PLOAD, nrf.rxBuff, nrf.rxLens);
          if (nrf.rxEventCallback != NULL) {
            nrf.rxEventCallback();
          }
        }
        else {
          nrf.rxFlag = NRF_STA_ERROR;
          NRF24_WriteCommand(CMD_FLUSH_RX);
          NRF24_CE_L();
          NRF24_SetMode(NRF_Mode_RX);
          NRF24_CE_H();
        }
        break;

      case NRF_OPMODE_TX:
        if (status & NRF_STA_MAX_RT) {
          nrf.txFlag = NRF_STA_MAX_RT;
          NRF24_WriteCommand(CMD_FLUSH_TX);
        }
        else if (status & NRF_STA_TX_DS) {
          nrf.txFlag = NRF_STA_TX_DS;
          if (nrf.txEventCallback != NULL) {
            nrf.txEventCallback();
          }
        }
        else {
          nrf.txFlag = NRF_STA_ERROR;
          NRF24_WriteCommand(CMD_FLUSH_TX);
//          NRF24_CE_L();
//          NRF24_SetMode(NRF_Mode_TX);
//          NRF24_CE_H();
        }
        break;
    }
  }
}

/*************************************** END OF FILE ****************************************/
