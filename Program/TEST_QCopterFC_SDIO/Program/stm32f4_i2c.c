/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu8* I2C_ReadPtr;
vu8* I2C_WritePtr;
vu32 I2C_TimeCnt = I2C_TIME;
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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);  

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_FEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_TEIF6 |
															DMA_FLAG_HTIF6 | DMA_FLAG_TCIF6);
	DMA_Cmd(DMA1_Stream6, DISABLE);
	DMA_DeInit(DMA1_Stream6);
	DMA_InitStruct.DMA_Channel = DMA_Channel_1;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;
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
	DMA_Init(DMA1_Stream6, &DMA_InitStruct);

	DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_TEIF0 |
															DMA_FLAG_HTIF0 | DMA_FLAG_TCIF0);
	DMA_Cmd(DMA1_Stream0, DISABLE);
	DMA_DeInit(DMA1_Stream0);
	DMA_InitStruct.DMA_Channel = DMA_Channel_1;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;
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
	DMA_Init(DMA1_Stream0, &DMA_InitStruct);

	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);

	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = I2C1_SPEED;
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStruct);

	I2C_DMACmd(I2C1, ENABLE);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_DMA_Read
**功能 : I2C DMA Read
**輸入 : *ReadBuf, SlaveAddr, ReadAddr, *NumByte
**輸出 : Status
**使用 : I2C_DMA_Read(ReadBuf, SlaveAddr, ReadAddr, (u8*)(&NumByte));
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_DMA_Read( u8* ReadBuf, u8 SlaveAddr, u8 ReadAddr, u8* NumByte )
{
	I2C_ReadPtr = NumByte;

	I2C_TimeCnt = I2C_TIME;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	I2C_GenerateSTART(I2C1, ENABLE);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	I2C_SendData(I2C1, ReadAddr);

	I2C_TimeCnt = I2C_TIME;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	I2C_GenerateSTART(I2C1, ENABLE);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Receiver);

	if((u16)(*NumByte) < 2) {
		I2C_TimeCnt = I2C_TIME;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
			if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

		I2C_AcknowledgeConfig(I2C1, DISABLE);
		(void)I2C1->SR2;

		I2C_GenerateSTOP(I2C1, ENABLE);

		I2C_TimeCnt = I2C_TIME;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
			if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

		*ReadBuf = I2C_ReceiveData(I2C1);

		(u16)(*NumByte)--;

		I2C_TimeCnt = I2C_TIME;
		while(I2C1->CR1 & I2C_CR1_STOP)
			if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}
	else {
		I2C_TimeCnt = I2C_TIME;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
			if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

		DMA_InitStruct.DMA_Channel = DMA_Channel_1;
		DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;
		DMA_InitStruct.DMA_Memory0BaseAddr = (u32)ReadBuf;
		DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStruct.DMA_BufferSize = (u32)(*NumByte);
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
		DMA_Init(DMA1_Stream0, &DMA_InitStruct);

		I2C_DMALastTransferCmd(I2C1, ENABLE);

		DMA_Cmd(DMA1_Stream0, ENABLE);
	}

	I2C_TimeCnt = I2C_TIME;
	while(*NumByte > 0)
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_DMA_ReadReg
**功能 : I2C DMA Read Reg
**輸入 : *ReadBuf, SlaveAddr, ReadAddr, NumByte
**輸出 : Status
**使用 : I2C_DMA_Read(ReadBuf, SlaveAddr, ReadAddr, NumByte);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_DMA_ReadReg( u8* ReadBuf, u8 SlaveAddr, u8 ReadAddr, u8 NumByte )
{
	I2C_DMA_Read(ReadBuf, SlaveAddr, ReadAddr, (u8*)(&NumByte));

	I2C_TimeCnt = I2C_TIME;
	while(NumByte > 0)
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_DMA_Write
**功能 : I2C DMA Write
**輸入 : *WriteBuf, SlaveAddr, WriteAddr, *NumByte
**輸出 : Status
**使用 : I2C_DMA_Write(WriteBuf, SlaveAddr, WriteAddr, (u8*)(&NumByte));
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_DMA_Write( u8* WriteBuf, u8 SlaveAddr, u8 WriteAddr, u8* NumByte )
{
	I2C_WritePtr = NumByte;

	I2C_TimeCnt = I2C_TIME;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	I2C_GenerateSTART(I2C1, ENABLE);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	I2C_TimeCnt = I2C_TIME;
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	I2C_SendData(I2C1, WriteAddr);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	DMA_InitStruct.DMA_Channel = DMA_Channel_1;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;
	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)WriteBuf;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize = (u32)(*NumByte);
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
	DMA_Init(DMA1_Stream6, &DMA_InitStruct);

	DMA_Cmd(DMA1_Stream6, ENABLE);

	return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_DMA_WriteReg
**功能 : I2C DMA Write Reg
**輸入 : *WriteBuf, SlaveAddr, WriteAddr, NumByte
**輸出 : Status
**使用 : I2C_DMA_Write(WriteBuf, SlaveAddr, WriteAddr, NumByte);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 I2C_DMA_WriteReg( u8* WriteBuf, u8 SlaveAddr, u8 WriteAddr, u8 NumByte )
{
	I2C_DMA_Write(WriteBuf, SlaveAddr, WriteAddr, (u8*)(&NumByte));

	I2C_TimeCnt = I2C_TIME;
	while(NumByte > 0)
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C1_Send_DMA_IRQ
**功能 : I2C1 Send DMA IRQ
**輸入 : None
**輸出 : None
**使用 : I2C1_Send_DMA_IRQ();
**=====================================================================================================*/
/*=====================================================================================================*/
void I2C1_Send_DMA_IRQ( void )
{
	if(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) {
		DMA_Cmd(DMA1_Stream6, DISABLE);
		DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);

		I2C_TimeCnt = I2C_TIME;
		while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF))
			if((I2C_TimeCnt--) == 0) I2C_TimeOut();

		I2C_GenerateSTOP(I2C1, ENABLE);
		*I2C_WritePtr = 0;
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C1_Recv_DMA_IRQ
**功能 : I2C1 Recv DMA IRQ
**輸入 : None
**輸出 : None
**使用 : I2C1_Recv_DMA_IRQ();
**=====================================================================================================*/
/*=====================================================================================================*/
void I2C1_Recv_DMA_IRQ( void )
{
	if(DMA_GetFlagStatus(DMA1_Stream0, DMA_FLAG_TCIF0) != RESET) {
		I2C_GenerateSTOP(I2C1, ENABLE);
		DMA_Cmd(DMA1_Stream0, DISABLE);
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
		*I2C_ReadPtr = 0;
	}
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
**函數 : I2C_MS5611_Read
**功能 : MS5611 I2C Read
**輸入 : *ReadBuf, SlaveAddr, ReadAddr, *NumByte
**輸出 : Status
**使用 : I2C_MS5611_Read(ReadBuf, SlaveAddr, ReadAddr, (u8*)(&NumByte));
**=====================================================================================================*/
/*=====================================================================================================*/
#define LED_R PAO(11)   // On:0  Off:1
#define LED_G PAO(12)   // On:0  Off:1
u32 MS5611_Read( u8* ReadBuf, u8 SlaveAddr, u8* NumByte )
{
	I2C_ReadPtr = NumByte;

	I2C_GenerateSTART(I2C1, ENABLE);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Receiver);

  I2C_TimeCnt = I2C_TIME;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

  DMA_InitStruct.DMA_Channel = DMA_Channel_1;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;
  DMA_InitStruct.DMA_Memory0BaseAddr = (u32)ReadBuf;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = (u32)(*NumByte);
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
  DMA_Init(DMA1_Stream0, &DMA_InitStruct);

  I2C_DMALastTransferCmd(I2C1, ENABLE);

  DMA_Cmd(DMA1_Stream0, ENABLE);

	I2C_TimeCnt = I2C_TIME;
	while(*NumByte > 0)
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	return SUCCESS;
}


u32 MS5611_ReadData( u8* ReadBuf, u8 SlaveAddr, u8 NumByte )
{
	MS5611_Read(ReadBuf, SlaveAddr, (u8*)(&NumByte));

	I2C_TimeCnt = I2C_TIME;
	while(NumByte > 0)
		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

	return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : I2C_MS5611_Write
**功能 : MS5611 I2C Write
**輸入 : SlaveAddr, WriteCom
**輸出 : Status
**使用 : I2C_Write(SlaveAddr, WriteCom);
**=====================================================================================================*/
/*=====================================================================================================*/
u32 MS5611_WriteCom( u8 SlaveAddr, u8 WriteCom )
{
	I2C_TimeCnt = I2C_TIME;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	I2C_GenerateSTART(I2C1, ENABLE);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	I2C_TimeCnt = I2C_TIME;
	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);

	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	I2C_SendData(I2C1, WriteCom);

	I2C_TimeCnt = I2C_TIME;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

	I2C_GenerateSTOP(I2C1, ENABLE);

  return SUCCESS;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
