/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define I2Cx                  I2C1
#define I2Cx_CLK              RCC_APB1Periph_I2C1

#define I2Cx_SCL_PIN          GPIO_Pin_8
#define I2Cx_SCL_GPIO_PORT    GPIOB
#define I2Cx_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define I2Cx_SCL_SOURCE       GPIO_PinSource8
#define I2Cx_SCL_AF           GPIO_AF_I2C1

#define I2Cx_SDA_PIN          GPIO_Pin_9
#define I2Cx_SDA_GPIO_PORT    GPIOB
#define I2Cx_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define I2Cx_SDA_SOURCE       GPIO_PinSource9
#define I2Cx_SDA_AF           GPIO_AF_I2C1

#define I2Cx_DR_ADDR          ((u32)0x40005410)
#define I2Cx_SPEED            ((u32)400000)

#define DMAx                  DMA1
#define DMAx_CLK              RCC_AHB1Periph_DMA1

#define DMAx_TX_CHANNEL       DMA_Channel_1
#define DMAx_TX_STREAM        DMA1_Stream6
#define DMAx_TX_FLAG_FEIF     DMA_FLAG_FEIF6
#define DMAx_TX_FLAG_DMEIF    DMA_FLAG_DMEIF6
#define DMAx_TX_FLAG_TEIF     DMA_FLAG_TEIF6
#define DMAx_TX_FLAG_HTIF     DMA_FLAG_HTIF6
#define DMAx_TX_FLAG_TCIF     DMA_FLAG_TCIF6
#define DMAx_TX_IRQn          DMA1_Stream6_IRQn

#define DMAx_RX_CHANNEL       DMA_Channel_1
#define DMAx_RX_STREAM        DMA1_Stream0
#define DMAx_RX_FLAG_FEIF     DMA_FLAG_FEIF0
#define DMAx_RX_FLAG_DMEIF    DMA_FLAG_DMEIF0
#define DMAx_RX_FLAG_TEIF     DMA_FLAG_TEIF0
#define DMAx_RX_FLAG_HTIF     DMA_FLAG_HTIF0
#define DMAx_RX_FLAG_TCIF     DMA_FLAG_TCIF0
#define DMAx_RX_IRQn          DMA1_Stream0_IRQn

#define I2C_TIMEOUT           ((u32)65535)
/*=====================================================================================================*/
/*=====================================================================================================*/
vu8* I2C_ReadPtr;
vu8* I2C_WritePtr;
vu32 I2C_TimeCnt = I2C_TIMEOUT;
/*=====================================================================================================*/
/*=====================================================================================================*/
DMA_InitTypeDef DMA_InitStruct;
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_Config
**功能 : I2C 設定 & 配置
**輸入 : None
**輸出 : None
**使用 : I2C_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void I2C_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
  RCC_AHB1PeriphClockCmd(DMAx_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(I2Cx_SCL_GPIO_CLK | I2Cx_SDA_GPIO_CLK, ENABLE);

  RCC_APB1PeriphResetCmd(I2Cx_CLK, DISABLE);

	GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
	GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);

  /* SCL I2Cx_SCL_PIN */  /* SDA I2Cx_SDA_PIN*/
  GPIO_InitStruct.GPIO_Pin = I2Cx_SCL_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = I2Cx_SDA_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

  NVIC_InitStruct.NVIC_IRQChannel = DMAx_RX_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  NVIC_InitStruct.NVIC_IRQChannel = DMAx_TX_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);

  DMA_ClearFlag(DMAx_TX_STREAM, DMAx_TX_FLAG_FEIF | DMAx_TX_FLAG_DMEIF | DMAx_TX_FLAG_TEIF |
                                DMAx_TX_FLAG_HTIF | DMAx_TX_FLAG_TCIF);
  DMA_Cmd(DMAx_TX_STREAM, DISABLE);
  DMA_DeInit(DMAx_TX_STREAM);
  DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)0;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = 0xFFFF;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

  DMA_ClearFlag(DMAx_RX_STREAM, DMAx_RX_FLAG_FEIF | DMAx_RX_FLAG_DMEIF | DMAx_RX_FLAG_TEIF |
                                DMAx_RX_FLAG_HTIF | DMAx_RX_FLAG_TCIF);
  DMA_Cmd(DMAx_RX_STREAM, DISABLE);
  DMA_DeInit(DMAx_RX_STREAM);
  DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)0;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = 0xFFFF;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);

  DMA_ITConfig(DMAx_TX_STREAM, DMA_IT_TC, ENABLE);
  DMA_ITConfig(DMAx_RX_STREAM, DMA_IT_TC, ENABLE);

  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_OwnAddress1 = 0x00;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStruct.I2C_ClockSpeed = I2Cx_SPEED;
  I2C_Cmd(I2Cx, ENABLE);
  I2C_Init(I2Cx, &I2C_InitStruct);

  I2C_DMACmd(I2Cx, ENABLE);

}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_ReadBytes
**功能 : I2C Read Bytes
**輸入 : SlaveAddr, ReadBuf, NumByte
**輸出 : Status
**使用 : I2C_ReadBytes(SlaveAddr, ReadBuf, NumByte);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_ReadBytes( u8 SlaveAddr, u8* ReadBuf, u8 NumByte )
{
  I2C_ReadPtr = &NumByte;

  I2C_GenerateSTART(I2Cx, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Receiver);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)ReadBuf;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);

  I2C_DMALastTransferCmd(I2Cx, ENABLE);

  DMA_Cmd(DMAx_RX_STREAM, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(NumByte > 0)
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_WriteBytes
**功能 : I2C Write Bytes
**輸入 : SlaveAddr, WriteBuf, NumByte
**輸出 : Status
**使用 : I2C_WriteByte(SlaveAddr, WriteBuf, NumByte);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_WriteBytes( u8 SlaveAddr, u8* WriteBuf, u8 NumByte )
{
  NumByte--;
  I2C_WritePtr = &NumByte;

  I2C_TimeCnt = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  I2C_GenerateSTART(I2Cx, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  I2C_TimeCnt = I2C_TIMEOUT;
  I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Transmitter);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  I2C_SendData(I2Cx, WriteBuf[0]);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  if(NumByte > 0) {
    DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
    DMA_InitStruct.DMA_Memory0BaseAddr = (u32)(WriteBuf+1);
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

    DMA_Cmd(DMAx_TX_STREAM, ENABLE);
  }
  else {
    I2C_GenerateSTOP(I2Cx, ENABLE);
  }

  I2C_TimeCnt = I2C_TIMEOUT;
  while(NumByte > 0)
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_DMA_ReadReg
**功能 : I2C DMA Read Reg
**輸入 : *ReadBuf, SlaveAddr, ReadAddr, NumByte
**輸出 : Status
**使用 : I2C_DMA_Read(SlaveAddr, ReadAddr, ReadBuf, NumByte);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_DMA_ReadReg( u8 SlaveAddr, u8 ReadAddr, u8* ReadBuf, u8 NumByte )
{
  I2C_ReadPtr = &NumByte;

  I2C_TimeCnt = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_GenerateSTART(I2Cx, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Transmitter);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_SendData(I2Cx, ReadAddr);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == RESET)
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_GenerateSTART(I2Cx, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Receiver);

  if(NumByte < 2) {
  I2C_TimeCnt = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == RESET)
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_AcknowledgeConfig(I2Cx, DISABLE);
  (void)I2Cx->SR2;

  I2C_GenerateSTOP(I2Cx, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET)
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  *ReadBuf = I2C_ReceiveData(I2Cx);

  NumByte--;

  I2C_TimeCnt = I2C_TIMEOUT;
  while(I2Cx->CR1 & I2C_CR1_STOP)
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  }
  else {
    I2C_TimeCnt = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
      if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

    DMA_InitStruct.DMA_Channel = DMAx_RX_CHANNEL;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
    DMA_InitStruct.DMA_Memory0BaseAddr = (u32)ReadBuf;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMAx_RX_STREAM, &DMA_InitStruct);

    I2C_DMALastTransferCmd(I2Cx, ENABLE);

    DMA_Cmd(DMAx_RX_STREAM, ENABLE);
  }

  I2C_TimeCnt = I2C_TIMEOUT;
  while(NumByte > 0)
  if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_DMA_WriteReg
**功能 : I2C DMA Write Reg
**輸入 : WriteAddr, WriteAddr, *WriteBuf, NumByte
**輸出 : Status
**使用 : I2C_DMA_Write(SlaveAddr, WriteAddr, WriteBuf, NumByte);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_DMA_WriteReg( u8 SlaveAddr, u8 WriteAddr, u8* WriteBuf, u8 NumByte )
{
  I2C_WritePtr = &NumByte;

  I2C_TimeCnt = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  I2C_GenerateSTART(I2Cx, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  I2C_TimeCnt = I2C_TIMEOUT;
  I2C_Send7bitAddress(I2Cx, SlaveAddr, I2C_Direction_Transmitter);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  I2C_SendData(I2Cx, WriteAddr);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

  DMA_InitStruct.DMA_Channel = DMAx_TX_CHANNEL;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2Cx_DR_ADDR;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)WriteBuf;
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStruct.DMA_BufferSize = (u32)(NumByte);
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMAx_TX_STREAM, &DMA_InitStruct);

  DMA_Cmd(DMAx_TX_STREAM, ENABLE);

  I2C_TimeCnt = I2C_TIMEOUT;
  while(NumByte > 0)
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_TimeOut
**功能 : I2C TimeOut
**輸入 : None
**輸出 : None
**使用 : I2C_TimeOut();
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_TimeOut( void )
{
  while(1) {
    Delay_1ms(200);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_TX_DMA_IRQ
**功能 : I2C Send DMA IRQ
**輸入 : None
**輸出 : None
**使用 : I2C_TX_DMA_IRQ();
**=====================================================================================================*/
/*=====================================================================================================*/
void I2C_TX_DMA_IRQ( void )
{
  if(DMA_GetFlagStatus(DMAx_TX_STREAM, DMAx_TX_FLAG_TCIF) != RESET) {
    DMA_Cmd(DMAx_TX_STREAM, DISABLE);
    DMA_ClearFlag(DMAx_TX_STREAM, DMAx_TX_FLAG_TCIF);

    I2C_TimeCnt = I2C_TIMEOUT;
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF))
      if((I2C_TimeCnt--) == 0) I2C_TimeOut();

    I2C_GenerateSTOP(I2Cx, ENABLE);
    *I2C_WritePtr = 0;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_RX_DMA_IRQ
**功能 : I2C Recv DMA IRQ
**輸入 : None
**輸出 : None
**使用 : I2C_RX_DMA_IRQ();
**=====================================================================================================*/
/*=====================================================================================================*/
void I2C_RX_DMA_IRQ( void )
{
  if(DMA_GetFlagStatus(DMAx_RX_STREAM, DMAx_RX_FLAG_TCIF) != RESET) {
    I2C_GenerateSTOP(I2Cx, ENABLE);
    DMA_Cmd(DMAx_RX_STREAM, DISABLE);
    DMA_ClearFlag(DMAx_RX_STREAM, DMAx_RX_FLAG_TCIF);
    *I2C_ReadPtr = 0;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
