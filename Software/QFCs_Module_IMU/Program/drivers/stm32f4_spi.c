/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    stm32f4_spi.c
  * @author  KitSprout
  * @date    18-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"

/** @addtogroup STM32_Driver
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
SPI_HandleTypeDef HSPI_IMU;
SpiHandle_st hImu = {
  .handle          = &HSPI_IMU,
  .txBufLens       = IMU_MAX_TXBUF,
  .rxBufLens       = IMU_MAX_RXBUF,
  .pTxBuf          = NULL,
  .pRxBuf          = NULL,
  .txEventCallback = NULL,
  .rxEventCallback = NULL
};

/* Private function prototypes -------------------------------------------------------------*/
/* MSP functions ---------------------------------------------------------------------------*/

/**
  * @brief SPI MSP Initialization
  */
void HAL_SPI_MspInit( SPI_HandleTypeDef *hspi )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(hspi->Instance == IMU_SPIx) {
    /* SPI Clk *******************************************************************/
    IMU_SPIx_CLK_ENABLE(); 

    /* SPI Pin *******************************************************************/
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin       = IMU_SCK_PIN;
    GPIO_InitStruct.Alternate = IMU_SCK_AF;
    HAL_GPIO_Init(IMU_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = IMU_SDO_PIN;
    GPIO_InitStruct.Alternate = IMU_SDO_AF;
    HAL_GPIO_Init(IMU_SDO_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = IMU_SDI_PIN;
    GPIO_InitStruct.Alternate = IMU_SDI_AF;
    HAL_GPIO_Init(IMU_SDI_GPIO_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(IMU_SPIx_IRQn, IMU_SPIx_IRQn_PREEMPT, IMU_SPIx_IRQn_SUB);
//    HAL_NVIC_EnableIRQ(IMU_SPIx_IRQn);
  }
}

/**
  * @brief SPI MSP De-Initialization
  */
void HAL_SPI_MspDeInit( SPI_HandleTypeDef *hspi )
{
  if (hspi->Instance == IMU_SPIx) {
    IMU_SPIx_FORCE_RESET();
    IMU_SPIx_RELEASE_RESET();
    HAL_GPIO_DeInit(IMU_SCK_GPIO_PORT, IMU_SCK_PIN);
    HAL_GPIO_DeInit(IMU_SDI_GPIO_PORT, IMU_SDI_PIN);
    HAL_GPIO_DeInit(IMU_SDI_GPIO_PORT, IMU_SDI_PIN);
    HAL_NVIC_DisableIRQ(IMU_SPIx_IRQn);
  }
}

/**
  * @brief Tx Transfer completed callback.
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{

}

/**
  * @brief Rx Transfer completed callback.
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{

}

/**
  * @brief Tx and Rx Transfer completed callback.
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

}

/**
  * @brief Tx Half Transfer completed callback.
  */
void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{

}

/**
  * @brief Rx Half Transfer completed callback.
  */
void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{

}

/**
  * @brief Tx and Rx Half Transfer callback.
  */
void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{

}

/**
  * @brief SPI error callback.
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{

}

/* Private functions -----------------------------------------------------------------------*/

#if !defined(KS_HW_SPI_HAL_LIBRARY)
/**
  * @brief  SPI Send Data
  */
int8_t SPI_SendData( SPI_HandleTypeDef *hspi, uint8_t *sendData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if((sendData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    tickStart = HAL_GetTick();
    while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    hspi->Instance->DR = *sendData++;
  }

  return HAL_OK;
}

/**
  * @brief  SPI Recv Data
  */
int8_t SPI_RecvData( SPI_HandleTypeDef *hspi, uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if((recvData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    tickStart = HAL_GetTick();
    while ((hspi->Instance->SR & SPI_FLAG_RXNE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    *recvData++ = (hspi->Instance->DR);
  }

  return HAL_OK;
}

/**
  * @brief  SPI_SendRecv
  */
int8_t SPI_SendRecv( SPI_HandleTypeDef *hspi, uint8_t *sendData, uint8_t *recvData, uint16_t lens, uint32_t timeout )
{
  uint32_t tickStart = 0;

  if((sendData == NULL) || (recvData == NULL) || (lens == 0)) {
    return HAL_ERROR;
  }

  while (lens--) {
    tickStart = HAL_GetTick();
    while ((hspi->Instance->SR & SPI_FLAG_TXE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    hspi->Instance->DR = *sendData++;
    tickStart = HAL_GetTick();
    while ((hspi->Instance->SR & SPI_FLAG_RXNE) == RESET) {
      if ((HAL_GetTick() - tickStart) > timeout) {
        return HAL_TIMEOUT;
      }
    }
    *recvData++ = (hspi->Instance->DR);
  }

  return HAL_OK;
}

#endif

/**
  * @brief  SPI_SetSpeed
  */
void SPI_SetSpeed( SPI_HandleTypeDef *hspi, uint8_t speedSel )
{
  __HAL_SPI_DISABLE(hspi);
  hspi->Init.BaudRatePrescaler = speedSel;
  HAL_SPI_Init(hspi);
  __HAL_SPI_ENABLE(hspi);
}

/**
  * @brief  SPI_RW
  */
uint8_t SPI_RW( SPI_HandleTypeDef *hspi, uint8_t writeByte )
{
  while ((hspi->Instance->SR & SPI_FLAG_TXE) == (uint16_t)RESET);
  hspi->Instance->DR = writeByte;
  while ((hspi->Instance->SR & SPI_FLAG_RXNE) == (uint16_t)RESET);
  return hspi->Instance->DR;
}

/*************************************** END OF FILE ****************************************/
