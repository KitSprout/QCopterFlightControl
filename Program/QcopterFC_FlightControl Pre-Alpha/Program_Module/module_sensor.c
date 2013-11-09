/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
#include "module_sensor.h"
#include "module_mpu9150.h"
#include "module_ms5611.h"
#include "algorithm_mathUnit.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
SensorAcc Acc = {0};
SensorGyr Gyr = {0};
SensorMag Mag = {0};
SensorTemp Temp = {0};
float Ellipse[5] = {0};
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
**功能 : Sensor Init
**輸入 : None
**輸出 : None
**使用 : Sensor_Init();
**=====================================================================================================*/
/*=====================================================================================================*/
u8 Sensor_Init( void )
{
  u8 Sta = ERROR;

  Acc.X = 0;
  Acc.Y = 0;
  Acc.Z = 0;
  Acc.OffsetX = 0;
  Acc.OffsetY = 0;
  Acc.OffsetZ = 0;
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

  Sta = MPU9150_Init();
  MS5611_Init(&Baro);

  return Sta;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : EllipseFitting
**功能 : Ellipse Fitting
**輸入 : Ans, MagDataX, MagDataY, Num
**輸出 : None
**使用 : EllipseFitting(Ellipse, MagDataX, MagDataY, 8);
**=====================================================================================================*/
/*=====================================================================================================*/
void EllipseFitting( float* Ans, s16* MagDataX, s16* MagDataY, u8 Num )
{
  s8 i, j, k;
  float temp, temp1, temp2;
  float tempArrX[8] = {0};
  float tempArrY[8] = {0};

  float MAG_X1Y0 = 0.0f;
  float MAG_X2Y0 = 0.0f;
  float MAG_X3Y0 = 0.0f;
  float MAG_X0Y1 = 0.0f;
  float MAG_X0Y2 = 0.0f;
  float MAG_X0Y3 = 0.0f;
  float MAG_X0Y4 = 0.0f;
  float MAG_X1Y1 = 0.0f;
  float MAG_X2Y1 = 0.0f;
  float MAG_X1Y2 = 0.0f;
  float MAG_X1Y3 = 0.0f;
  float MAG_X2Y2 = 0.0f;
  float MAG_X3Y1 = 0.0f;

  float MagArr[5][6] = {0};

  for(i=0; i<Num; i++) {
    tempArrX[i] = (float)MagDataX[i]/1000.0f;
    tempArrY[i] = (float)MagDataY[i]/1000.0f;
  }

  for(i=0; i<Num; i++) {
    MAG_X1Y0 += tempArrX[i];
    MAG_X2Y0 += tempArrX[i]*tempArrX[i];
    MAG_X3Y0 += tempArrX[i]*tempArrX[i]*tempArrX[i];
    MAG_X0Y1 += tempArrY[i];
    MAG_X0Y2 += tempArrY[i]*tempArrY[i];
    MAG_X0Y3 += tempArrY[i]*tempArrY[i]*tempArrY[i];
    MAG_X0Y4 += tempArrY[i]*tempArrY[i]*tempArrY[i]*tempArrY[i];
    MAG_X1Y1 += tempArrX[i]*tempArrY[i];
    MAG_X2Y1 += tempArrX[i]*tempArrX[i]*tempArrY[i];
    MAG_X1Y2 += tempArrX[i]*tempArrY[i]*tempArrY[i];
    MAG_X1Y3 += tempArrX[i]*tempArrY[i]*tempArrY[i]*tempArrY[i];
    MAG_X2Y2 += tempArrX[i]*tempArrX[i]*tempArrY[i]*tempArrY[i];
    MAG_X3Y1 += tempArrX[i]*tempArrX[i]*tempArrX[i]*tempArrY[i];
  }

  MagArr[0][0] = MAG_X2Y2;
  MagArr[0][1] = MAG_X1Y3;
  MagArr[0][2] = MAG_X2Y1;
  MagArr[0][3] = MAG_X1Y2;
  MagArr[0][4] = MAG_X1Y1;
  MagArr[0][5] = -MAG_X3Y1;

  MagArr[1][0] = MAG_X1Y3;
  MagArr[1][1] = MAG_X0Y4;
  MagArr[1][2] = MAG_X1Y2;
  MagArr[1][3] = MAG_X0Y3;
  MagArr[1][4] = MAG_X0Y2;
  MagArr[1][5] = -MAG_X2Y2;

  MagArr[2][0] = MAG_X2Y1;
  MagArr[2][1] = MAG_X1Y2;
  MagArr[2][2] = MAG_X2Y0;
  MagArr[2][3] = MAG_X1Y1;
  MagArr[2][4] = MAG_X1Y0;
  MagArr[2][5] = -MAG_X3Y0;

  MagArr[3][0] = MAG_X1Y2;
  MagArr[3][1] = MAG_X0Y3;
  MagArr[3][2] = MAG_X1Y1;
  MagArr[3][3] = MAG_X0Y2;
  MagArr[3][4] = MAG_X0Y1;
  MagArr[3][5] = -MAG_X2Y1;

  MagArr[4][0] = MAG_X1Y1;
  MagArr[4][1] = MAG_X0Y2;
  MagArr[4][2] = MAG_X1Y0;
  MagArr[4][3] = MAG_X0Y1;
  MagArr[4][4] = Num;
  MagArr[4][5] = -MAG_X2Y0;

  for(i=0; i<5; i++)
    for(j=i+1; j<6; j++)
      for(k=5; k>i-1; k--)
        MagArr[j][k] = MagArr[j][k] - MagArr[j][i]/MagArr[i][i]*MagArr[i][k];
  for(i=0; i<5; i++) {
    temp = MagArr[i][i];
    for(j=i; j<6; j++)
      MagArr[i][j] = MagArr[i][j]/temp;
  }
  for(j=4; j>0; j--)
    for(i=0; i<j; i++)
      MagArr[i][5] = MagArr[i][5] - MagArr[i][j]*MagArr[j][5];

  temp = (1.0f-MagArr[1][5])/MagArr[0][5];
  temp1 = temp + sqrtf(temp*temp+1.0f);
  Ans[0] = atanf(temp1);	// Theta

  temp = MagArr[0][5]*MagArr[0][5]-4*MagArr[1][5];
  Ans[1] = (2.0f*MagArr[1][5]*MagArr[2][5]-MagArr[0][5]*MagArr[3][5])/temp;	// X0
  Ans[2] = (2.0f*MagArr[3][5]-MagArr[0][5]*MagArr[2][5])/temp;	            // Y0

  temp = arm_cos_f32(Ans[0]);
  temp2  = (Ans[1]*Ans[1]+MagArr[0][5]*Ans[1]*Ans[2]+MagArr[1][5]*Ans[2]*Ans[2]-MagArr[4][5])*(temp1*temp1*temp1*temp1-1.0f);
  Ans[3] = temp/sqrtf((MagArr[1][5]*temp1*temp1-1)/temp2);	// a
  Ans[4] = temp/sqrtf((temp1*temp1-MagArr[1][5])/temp2);    // b

  Ans[1] = Ans[1]*1000.0f;
  Ans[2] = Ans[2]*1000.0f;
  Ans[3] = Ans[3]*1000.0f;
  Ans[4] = Ans[4]*1000.0f;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
