/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
#include "module_sensor.h"
#include "module_mpu9150.h"
#include "module_ms5611.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
SensorAcc Acc = {0};
SensorGyr Gyr = {0};
SensorMag Mag = {0};
SensorTemp Temp = {0};
SensorBaro Baro = {0};
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Sensor_Config
**功能 : Sensor Config
**輸入 : None
**輸出 : None
**使用 : Sensor_Config();
**=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* INT PC3 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  I2C_Config();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Sensor_Init
**功能 : 初始化 Sensor
**輸入 : None
**輸出 : None
**使用 : Sensor_Init();
**=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_Init( void )
{
  Acc.X = 0;
  Acc.Y = 0;
  Acc.Z = 0;
  Acc.OffsetX = +7;
  Acc.OffsetY = +146;
  Acc.OffsetZ = -291;
  Acc.TrueX = 0.0f;
  Acc.TrueY = 0.0f;
  Acc.TrueZ = 0.0f;

  Gyr.X = 0;
  Gyr.Y = 0;
  Gyr.Z = 0;
  Gyr.OffsetX = 0;
  Gyr.OffsetY = 0;
  Gyr.OffsetZ = 0;
  Gyr.TrueX = 0.0f;
  Gyr.TrueY = 0.0f;
  Gyr.TrueZ = 0.0f;

  Mag.X = 0;
  Mag.Y = 0;
  Mag.Z = 0;
  Mag.AdjustX = 0;
  Mag.AdjustY = 0;
  Mag.AdjustZ = 0;
  Mag.TrueX = 0.0f;
  Mag.TrueY = 0.0f;
  Mag.TrueZ = 0.0f;

  Temp.T = 0;
  Temp.OffsetT = TEMP_OFFSET;
  Temp.TrueT = 0.0f;

  Baro.Temp = 0.0f;
  Baro.Press = 0.0f;
  Baro.Height = 0.0f;

  while(MPU9150_Init() != SUCCESS);
  MS5611_Init();
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Sensor_Read
**功能 : Sensor Read
**輸入 : ReadBuf
**輸出 : None
**使用 : Sensor_Read(SampleRateFreg);
**=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_Read( u16 ReadFreg )
{
  u8 ReadBuf[20] = {0};

  static s8 ReadCount = 0;
  static s32 Baro_Buf[2] = {0}; // 沒加 static 資料會有問題

  MPU9150_Read(ReadBuf);

  Acc.X  = Byte16(s16, ReadBuf[0],  ReadBuf[1]);  // Acc.X
  Acc.Y  = Byte16(s16, ReadBuf[2],  ReadBuf[3]);  // Acc.Y
  Acc.Z  = Byte16(s16, ReadBuf[4],  ReadBuf[5]);  // Acc.Z
  Gyr.X  = Byte16(s16, ReadBuf[8],  ReadBuf[9]);  // Gyr.X
  Gyr.Y  = Byte16(s16, ReadBuf[10], ReadBuf[11]); // Gyr.Y
  Gyr.Z  = Byte16(s16, ReadBuf[12], ReadBuf[13]); // Gyr.Z
  Mag.Z  = Byte16(s16, ReadBuf[14], ReadBuf[15]); // Mag.X
  Mag.Z  = Byte16(s16, ReadBuf[16], ReadBuf[17]); // Mag.Y
  Mag.Z  = Byte16(s16, ReadBuf[18], ReadBuf[19]); // Mag.Z
  Temp.T = Byte16(s16, ReadBuf[6],  ReadBuf[7]);  // Temp

  if(ReadCount == 0) {
    MS5611_Read(Baro_Buf, MS5611_D1_OSR_4096);
    Baro.Temp   = (fp32)(Baro_Buf[0]*0.01f);
    Baro.Press  = (fp32)(Baro_Buf[1]*0.01f);
    Baro.Height = (fp32)((Baro_Buf[1]-101333)*9.5238f);
    ReadCount = (u16)(ReadFreg/MS5611_RespFreq_4096)+1;
  }
  ReadCount--;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
