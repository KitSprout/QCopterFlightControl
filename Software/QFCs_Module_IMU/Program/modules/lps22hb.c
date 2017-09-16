/**
 *       __            ____
 *      / /__ _  __   / __/                      __  
 *     / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *    / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *   /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                     /_/   github.com/KitSprout    
 * 
 *  @file    lps22hb.c
 *  @author  KitSprout
 *  @date    14-Sep-2017
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\lps22hb.h"

/** @addtogroup STM32_Module
 *  @{
 */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
 *  @brief  LPS22_WriteReg
 */
void LPS22_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  IMU_CSB_L();
  SPI_RW(hImu.handle, writeAddr);
  SPI_RW(hImu.handle, writeData);
  IMU_CSB_H();
}

/**
 *  @brief  LPS22_WriteRegs
 */
void LPS22_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  IMU_CSB_L();
  SPI_RW(hImu.handle, writeAddr);
  for (uint8_t i = 0; i < lens; i++) {
    SPI_RW(hImu.handle, writeData[i]);
  }
  IMU_CSB_H();
}

/**
 *  @brief  LPS22_ReadReg
 */
uint8_t LPS22_ReadReg( uint8_t readAddr )
{
  uint8_t readData;

  IMU_CSB_L();
  SPI_RW(hImu.handle, 0x80 | readAddr);
  readData = SPI_RW(hImu.handle, 0x00);
  IMU_CSB_H();

  return readData;
}

/**
 *  @brief  LPS22_ReadRegs
 */
void LPS22_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  IMU_CSB_L();
  SPI_RW(hImu.handle, 0x80 | readAddr);
  for (uint8_t i = 0; i < lens; i++) {
    readData[i] = SPI_RW(hImu.handle, 0x00);
  }
  IMU_CSB_H();
}

/**
 *  @brief  LPS22_Config
 */
void LPS22_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_InitStruct.Pin   = IMU_CSB_PIN;
  HAL_GPIO_Init(IMU_CSB_GPIO_PORT, &GPIO_InitStruct);

  IMU_CSB_H();    // LOW ENABLE
}

/**
 *  @brief  LPS22_Init
 */
//#define LPS22HB_InitRegNum    5
int8_t LPS22_Init( LPS_ConfigTypeDef *LPSx )
{
  uint8_t status = ERROR;
  uint8_t treg = 0;
//  uint8_t LPS22HB_InitData[LPS22HB_InitRegNum][2] = {
//    {0x00, LPS22HB_RES_CONF},       /* [0]  Normal mode (low-noise mode)  */
//  };

  delay_ms(2);
  status = LPS22_DeviceCheck();
  if (status != SUCCESS) {
    return ERROR;
  }

  /* Normal mode (low-noise mode) */
  treg = LPS22_ReadReg(LPS22HB_RES_CONF);
  treg &= 0x02;
  treg |= 0x00;
  LPS22_WriteReg(LPS22HB_RES_CONF, treg);
  delay_ms(5);

  /* Control register 1 */
  treg  = 0x00;
  treg |= 0x50; // Output data rate, 75 Hz
  treg |= 0x00; // Low-pass filter disabled
  treg |= 0x02; // Block data update, enable
  LPS22_WriteReg(LPS22HB_CTRL_REG1, treg);
  delay_ms(5);

  /* Control register 2 */
  treg = LPS22_ReadReg(LPS22HB_CTRL_REG2);
  treg &= 0xED;
  treg |= 0x10;
  LPS22_WriteReg(LPS22HB_RES_CONF, treg);
  delay_ms(5);

  return SUCCESS;
}

/**
 *  @brief  LPS22_DeviceCheck
 */
int8_t LPS22_DeviceCheck( void )
{
  uint8_t deviceID;

  deviceID = LPS22_ReadReg(LPS22HB_WHO_AM_I);
  if (deviceID != LPS22HB_DEVICE_ID) {
    return ERROR;
  }
//  printf("device id = 0x%02X\r\n", deviceID);

  return SUCCESS;
}

/**
 *  @brief  LPS22_GetSensitivity
 *  @param  sensitivity: point to float32_t
            sensitivity[0] - pressure
            sensitivity[1] - temperature
 *  @retval None
 */
void LPS22_GetSensitivity( float32_t *sensitivity )
{
  /* Set pressure sensitivity (hPa/LSB) */
  sensitivity[0] = 1.0 / 4096.0;

  /* Set temperature sensitivity (degC/LSB) */
  sensitivity[1] = 1.0 / 100.0;
}

/**
 *  @brief  LPS22_GetRawData
 *  @param  data: point to int32_t
 *  @retval state
 */
int8_t LPS22_GetRawData( int32_t *data )
{
  uint8_t buffer[3];
  uint32_t tmp = 0;

  LPS22_ReadRegs(LPS22HB_PRESS_OUT_XL, buffer, 3);

  for(uint32_t i = 0; i < 3; i++) {
    tmp |= (((uint32_t)buffer[i]) << (i << 3));
  }

  /* convert the 2's complement 24 bit to 2's complement 32 bit */
  if (tmp & 0x00800000) {
    tmp |= 0xFF000000;
  }

  data[0] = (int32_t)tmp;

  LPS22_ReadRegs(LPS22HB_TEMP_OUT_L, buffer, 2);
  tmp = ((uint16_t)buffer[1] << 8) + (uint16_t)buffer[0];

  data[1] = (int16_t)tmp;

  return SUCCESS;
}

/*************************************** END OF FILE ****************************************/
