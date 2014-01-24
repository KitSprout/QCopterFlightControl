/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"
#include "module_camspi.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define SPIx                  SPI3
#define SPIx_CLK              RCC_APB1Periph_SPI3

#define SPIx_CS_PIN           GPIO_Pin_15
#define SPIx_CS_GPIO_PORT     GPIOA
#define SPIx_CS_GPIO_CLK      RCC_AHB1Periph_GPIOA
#define SPIx_CS_SOURCE        GPIO_PinSource15
#define SPIx_CS_AF            GPIO_AF_SPI3

#define SPIx_SCK_PIN          GPIO_Pin_3
#define SPIx_SCK_GPIO_PORT    GPIOB
#define SPIx_SCK_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE       GPIO_PinSource3
#define SPIx_SCK_AF           GPIO_AF_SPI3

#define SPIx_MISO_PIN         GPIO_Pin_4
#define SPIx_MISO_GPIO_PORT   GPIOB
#define SPIx_MISO_GPIO_CLK    RCC_AHB1Periph_GPIOB
#define SPIx_MISO_SOURCE      GPIO_PinSource4
#define SPIx_MISO_AF          GPIO_AF_SPI3

#define SPIx_MOSI_PIN         GPIO_Pin_5
#define SPIx_MOSI_GPIO_PORT   GPIOB
#define SPIx_MOSI_GPIO_CLK    RCC_AHB1Periph_GPIOB
#define SPIx_MOSI_SOURCE      GPIO_PinSource5
#define SPIx_MOSI_AF          GPIO_AF_SPI3

#define SPI_SPEED             SPI_BaudRatePrescaler_256  // SPI3 Clock = 42MHz/256
#define SPI_TIMEOUT           ((u32)65535)

#define DMAx                  DMA1
#define DMAx_CLK              RCC_AHB1Periph_DMA1

#define DMAx_TX_CHANNEL       DMA_Channel_0
#define DMAx_TX_STREAM        DMA1_Stream5
#define DMAx_TX_FLAG_FEIF     DMA_FLAG_FEIF5
#define DMAx_TX_FLAG_DMEIF    DMA_FLAG_DMEIF5
#define DMAx_TX_FLAG_TEIF     DMA_FLAG_TEIF5
#define DMAx_TX_FLAG_HTIF     DMA_FLAG_HTIF5
#define DMAx_TX_FLAG_TCIF     DMA_FLAG_TCIF5
#define DMAx_TX_IRQn          DMA1_Stream5_IRQn

#define DMAx_RX_CHANNEL       DMA_Channel_0
#define DMAx_RX_STREAM        DMA1_Stream2
#define DMAx_RX_FLAG_FEIF     DMA_FLAG_FEIF2
#define DMAx_RX_FLAG_DMEIF    DMA_FLAG_DMEIF2
#define DMAx_RX_FLAG_TEIF     DMA_FLAG_TEIF2
#define DMAx_RX_FLAG_HTIF     DMA_FLAG_HTIF2
#define DMAx_RX_FLAG_TCIF     DMA_FLAG_TCIF2
#define DMAx_RX_IRQn          DMA1_Stream2_IRQn
/*=====================================================================================================*/
/*=====================================================================================================*/
static u32 SPI_TimeCnt = SPI_TIMEOUT;
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CamSPI_Config
**功能 : CamSPI 配置 & 設定
**輸入 : None
**輸出 : None
**使用 : CamSPI_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void CamSPI_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;
  DMA_InitTypeDef DMA_InitStruct;

  RCC_AHB1PeriphClockCmd(SPIx_CS_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(SPIx_MISO_GPIO_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(SPIx_MOSI_GPIO_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(SPIx_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(DMAx_CLK, ENABLE);

  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT,  SPIx_SCK_SOURCE,  SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

#if (CAMSPI_MODE == CAMSPI_MODE_MSATER)
  GPIO_InitStruct.GPIO_Pin = SPIx_CS_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);
  CamSPI_CS = 0;
#else
  GPIO_InitStruct.GPIO_Pin = SPIx_CS_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStruct);
#endif

  GPIO_InitStruct.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = SPIx_MOSI_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = SPIx_MISO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

  SPI_I2S_DeInit(SPIx);
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = CAMSPI_MODE;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_SPEED;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  DMA_DeInit(DMAx_TX_STREAM);
  DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&(SPIx->DR));
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)0;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = 0xFFFF;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

  DMA_DeInit(DMAx_RX_STREAM);
  DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&(SPIx->DR));
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)0;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = 0xFFFF;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CamSPI_RW
**功能 : 讀寫暫存器
**輸入 : WriteData, ReadData, WriteNum
**輸出 : None
**使用 : CamSPI_RW(WriteData, ReadData, BufSize);
**=====================================================================================================*/
/*=====================================================================================================*/
void CamSPI_RW( u8 *WriteData, u8 *ReadData, u16 BufSize )
{
  DMA_InitTypeDef DMA_InitStruct;

  DMA_DeInit(DMAx_TX_STREAM);
  DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&(SPIx->DR));
  DMA_InitStruct.DMA_Memory0BaseAddr =(u32)(WriteData);
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = BufSize;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

  DMA_DeInit(DMAx_RX_STREAM);
  DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&(SPIx->DR));
  DMA_InitStruct.DMA_Memory0BaseAddr =(u32)(ReadData);
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = BufSize;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_High;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);

  DMA_Cmd(DMAx_TX_STREAM, ENABLE);
  DMA_Cmd(DMAx_RX_STREAM, ENABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
  SPI_Cmd(SPIx, ENABLE);

  SPI_TimeCnt = SPI_TIMEOUT;
  while(DMA_GetFlagStatus(DMAx_TX_STREAM, DMAx_TX_FLAG_TCIF)==RESET)
    if((SPI_TimeCnt--) == 0) CamSPI_TimeOut();

  SPI_TimeCnt = SPI_TIMEOUT;
  while(DMA_GetFlagStatus(DMAx_RX_STREAM, DMAx_RX_FLAG_TCIF)==RESET)
    if((SPI_TimeCnt--) == 0) CamSPI_TimeOut();

  DMA_ClearFlag(DMAx_TX_STREAM, DMAx_TX_FLAG_TCIF);
  DMA_ClearFlag(DMAx_RX_STREAM, DMAx_RX_FLAG_TCIF);
  DMA_Cmd(DMAx_TX_STREAM, DISABLE);
  DMA_Cmd(DMAx_RX_STREAM, DISABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
  SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
  SPI_Cmd(SPIx, DISABLE);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : CamSPI_TimeOut
**功能 : 超時
**輸入 : None
**輸出 : None
**使用 : CamSPI_TimeOut();
**=====================================================================================================*/
/*=====================================================================================================*/
static void CamSPI_TimeOut( void )
{
#if (CAMSPI_MODE == CAMSPI_MODE_MSATER)
  while(1) {
    Delay_1ms(200);
  }
#else
  
#endif
}
/*=====================================================================================================*/
/*=====================================================================================================*/
