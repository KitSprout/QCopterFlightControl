/*==============================================================================================*/
/*==============================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
#include "module_ms5611.h"
/*==============================================================================================*/
/*==============================================================================================*/
MS5611_ST Baro;
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MS5611_WriteCom
**功能 : MS5611 I2C Write
**輸入 : SlaveAddr, WriteCom
**輸出 : Status
**使用 : I2C_Write(SlaveAddr, WriteCom);
**=====================================================================================================*/
/*=====================================================================================================*/
// u32 MS5611_WriteCom( u8 SlaveAddr, u8 WriteCom )
// {
//   vu32 I2C_TimeCnt;

// 	I2C_TimeCnt = I2C_TIME;
// 	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
// 		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_GenerateSTART(I2C1, ENABLE);

// 	I2C_TimeCnt = I2C_TIME;
// 	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
// 		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_TimeCnt = I2C_TIME;
// 	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Transmitter);

// 	I2C_TimeCnt = I2C_TIME;
// 	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
// 		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_SendData(I2C1, WriteCom);

// 	I2C_TimeCnt = I2C_TIME;
//   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
// 		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_GenerateSTOP(I2C1, ENABLE);

//   return SUCCESS;
// }
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MS5611_ReadData
**功能 : MS5611 I2C Read
**輸入 : *ReadBuf, SlaveAddr, NumByte
**輸出 : Status
**使用 : I2C_MS5611_Read(ReadBuf, SlaveAddr, NumByte);
**=====================================================================================================*/
/*=====================================================================================================*/
// u32 MS5611_ReadData( u8* ReadBuf, u8 SlaveAddr, u8 NumByte )
// {
//   vu32 I2C_TimeCnt;

// 	I2C_TimeCnt = I2C_TIME;
// 	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
// 		if((I2C_TimeCnt--) == 0) return I2C_TimeOut();

// 	I2C_GenerateSTART(I2C1, ENABLE);

// 	I2C_TimeCnt = I2C_TIME;
// 	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
// 		if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	I2C_Send7bitAddress(I2C1, SlaveAddr, I2C_Direction_Receiver);

//   I2C_TimeCnt = I2C_TIME;
//   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//     if((I2C_TimeCnt--) == 0)	return I2C_TimeOut();

// 	while(NumByte) {
// 		if(NumByte == 1) {
// 			I2C_GenerateSTOP(I2C1, ENABLE);
// 		}
// 		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {      
// 			*ReadBuf = I2C_ReceiveData(I2C1);
// 			ReadBuf++;
// 			NumByte--;
// 		}
// 	}

//   return SUCCESS;
// }
/*==============================================================================================*/
/*==============================================================================================*
**函數 : MS5611_Init
**功能 : 初始化MS5611
**輸入 : COEFF
**輸出 : None
**使用 : MS5611_Init(&Baro)
**==============================================================================================*/
/*==============================================================================================*/
void MS5611_Init( MS5611_ST* COEFF )
{
  u8 ReadC[6][2] = {0};
  u8 ReadD[2][3] = {0};

  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_RESET);
  Delay_1ms(10);

  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_1);
	MS5611_ReadData(ReadC[0], MS5611_I2C_ADDR, 2);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_2);
	MS5611_ReadData(ReadC[1], MS5611_I2C_ADDR, 2);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_3);
	MS5611_ReadData(ReadC[2], MS5611_I2C_ADDR, 2);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_4);
	MS5611_ReadData(ReadC[3], MS5611_I2C_ADDR, 2);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_5);
	MS5611_ReadData(ReadC[4], MS5611_I2C_ADDR, 2);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_6);
	MS5611_ReadData(ReadC[5], MS5611_I2C_ADDR, 2);
  Delay_1ms(10);

  COEFF->C[1] = (u16)((ReadC[0][0] << 7) | ReadC[0][1]);
  COEFF->C[2] = (u16)((ReadC[1][0] << 7) | ReadC[1][1]);
  COEFF->C[3] = (u16)((ReadC[2][0] << 7) | ReadC[2][1]);
  COEFF->C[4] = (u16)((ReadC[3][0] << 7) | ReadC[3][1]);
  COEFF->C[5] = (u16)((ReadC[4][0] << 7) | ReadC[4][1]);
  COEFF->C[6] = (u16)((ReadC[5][0] << 7) | ReadC[5][1]);

  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_D1_OSR_4096);
  Delay_1ms(10);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_ADC);
	MS5611_ReadData(ReadD[0], MS5611_I2C_ADDR, 3);

  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_D2_OSR_4096);
  Delay_1ms(10);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_ADC);
	MS5611_ReadData(ReadD[1], MS5611_I2C_ADDR, 3);

  COEFF->D[1] = (u32)((ReadD[0][0] << 15) | (ReadD[0][1] << 7) | ReadD[0][2]);
  COEFF->D[2] = (u32)((ReadD[1][0] << 15) | (ReadD[1][1] << 7) | ReadD[1][2]);

  MS5611_Calculate(COEFF);
}
/*==============================================================================================*/
/*==============================================================================================*
**函數 : MS5611_ReadADC
**功能 : 初始化MS5611
**輸入 : COEFF
**輸出 : None
**使用 : MS5611_Init(&Baro)
**==============================================================================================*/
/*==============================================================================================*/
void MS5611_ReadADC( MS5611_ST* COEFF )
{
  u8 ReadCoeff[6][2] = {0};

  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_ADC);
  Delay_1ms(10);

  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_1);
	MS5611_ReadData(ReadCoeff[0], MS5611_I2C_ADDR, 2);
  MS5611_WriteCom(MS5611_I2C_ADDR, MS5611_PROM_COEFF_2);
	MS5611_ReadData(ReadCoeff[1], MS5611_I2C_ADDR, 2);

  COEFF->C[1] = (u16)((ReadCoeff[0][0] << 7) | ReadCoeff[0][1]);
  COEFF->C[2] = (u16)((ReadCoeff[1][0] << 7) | ReadCoeff[1][1]);
  COEFF->C[3] = (u16)((ReadCoeff[2][0] << 7) | ReadCoeff[2][1]);
  COEFF->C[4] = (u16)((ReadCoeff[3][0] << 7) | ReadCoeff[3][1]);
  COEFF->C[5] = (u16)((ReadCoeff[4][0] << 7) | ReadCoeff[4][1]);
  COEFF->C[6] = (u16)((ReadCoeff[5][0] << 7) | ReadCoeff[5][1]);
}
/*==============================================================================================*/
/*==============================================================================================*
**函數 : MS5611_Calculate
**功能 : 
**輸入 : Baro
**輸出 : None
**使用 : MS5611_Calculate(&Baro)
**==============================================================================================*/
/*==============================================================================================*/
void MS5611_Calculate( MS5611_ST* COEFF )
{
  COEFF->dT = (s32)(COEFF->D[2] - (COEFF->C[5]*POW_2_8));          // dT = D2 - C5*2^8
  COEFF->rTemp = (s32)(2000 + (COEFF->dT*COEFF->C[5])/POW_2_23);   // Temp = 2000 + dT*C5/2^23

  COEFF->OFF = (int64_t)(COEFF->C[2]*POW_2_16 + (COEFF->C[4]*COEFF->dT)/POW_2_7);   // OFF = C2*2^16 + (C4*dT)/2^7
  COEFF->SENS = (int64_t)(COEFF->C[1]*POW_2_15 + (COEFF->C[3]*COEFF->dT)/POW_2_8);  // SENS = C1*2^15 + (C3*dT)/2^8
  COEFF->rPress = (s32)((COEFF->D[1]*COEFF->SENS)/POW_2_21 - COEFF->OFF)/POW_2_15;  // Press = (D1*SENS/2^21 - OFF)/2^15

  COEFF->Temp  = (float)(COEFF->rTemp/100.0f);
  COEFF->Press = (float)(COEFF->rPress/100.0f);
  COEFF->High  = (float)((COEFF->rPress-101333)*9.5238f);
}
/*==============================================================================================*/
/*==============================================================================================*/
