/*====================================================================================================*/
/*====================================================================================================*/
#include <stdlib.h>
#include <string.h>

#include "stm32f4_system.h"
#include "experiment_stm32f4.h"
#include "module_imu.h"
#include "algorithm_moveAve.h"
#include "algorithm_mathUnit.h"
#include "algorithm_ahrs.h"
/*====================================================================================================*/
/*====================================================================================================*/
void SysTick_Handler( void )
{
  static s16 *FIFO_X, *FIFO_Y, *FIFO_Z;
  static s16 FIFO_ACC[3][16] = {0}, FIFO_GYR[3][16] = {0}/*, FIFO_MAG[3][16] = {0}*/;
  static u32 Correction_Time = 0;

  /* 500Hz, Read Accelerometer, Gyroscope, Magnetometer */
  Sensor_Read(SampleRateFreg);

  /* Offset */
  Acc.X -= Acc.OffsetX;
  Acc.Y -= Acc.OffsetY;
  Acc.Z -= Acc.OffsetZ;
  Gyr.X -= Gyr.OffsetX;
  Gyr.Y -= Gyr.OffsetY;
  Gyr.Z -= Gyr.OffsetZ;
//  Mag.X *= Mag.AdjustX;
//  Mag.Y *= Mag.AdjustY;
//  Mag.Z *= Mag.AdjustZ;
//  Temp.T -= Temp.OffsetT;

  #define MAFIFO_SIZE 250
  switch(SEN_STATE) {

    /************************** CorrectSelect ***********************************/
    case SEN_CORR:
      SEN_STATE = (KEY == KEY_ON) ? SEN_GYR : SEN_NUMQ;
      break;

    /************************** CorrectGyr **************************************/
    case SEN_GYR:
      switch((u16)(Correction_Time/SampleRateFreg)) {
        case 0:   // 分配記憶體給 MaveAve 使用
          FIFO_X = (s16*)malloc(MAFIFO_SIZE*sizeof(s16));
          FIFO_Y = (s16*)malloc(MAFIFO_SIZE*sizeof(s16));
          FIFO_Z = (s16*)malloc(MAFIFO_SIZE*sizeof(s16));
          memset(FIFO_X, 0, MAFIFO_SIZE*sizeof(s16));
          memset(FIFO_Y, 0, MAFIFO_SIZE*sizeof(s16));
          memset(FIFO_Z, 0, MAFIFO_SIZE*sizeof(s16));
          Correction_Time = SampleRateFreg;
          break;
        case 1:   // 等待 FIFO 填滿靜態資料
          /* 移動平均 Simple Moving Average */
          Gyr.X = (s16)MoveAve_SMA(Gyr.X, FIFO_X, MAFIFO_SIZE);
          Gyr.Y = (s16)MoveAve_SMA(Gyr.Y, FIFO_Y, MAFIFO_SIZE);
          Gyr.Z = (s16)MoveAve_SMA(Gyr.Z, FIFO_Z, MAFIFO_SIZE);
          Correction_Time++;
          break;
        case 2:   // 釋放記憶體 & 計算陀螺儀偏移量
          free(FIFO_X);
          free(FIFO_Y);
          free(FIFO_Z);
          Gyr.OffsetX += (Gyr.X - GYR_X_OFFSET);  // 角速度為 0dps
          Gyr.OffsetY += (Gyr.Y - GYR_Y_OFFSET);  // 角速度為 0dps
          Gyr.OffsetZ += (Gyr.Z - GYR_Z_OFFSET);  // 角速度為 0dps
          Correction_Time = 0;
          SEN_STATE = SEN_ACC;
          break;
      }
      break;

    /************************** CorrectAcc **************************************/
    case SEN_ACC:
      switch((u16)(Correction_Time/SampleRateFreg)) {
        case 0:   // 分配記憶體給 MaveAve 使用
          FIFO_X = (s16*)malloc(MAFIFO_SIZE*sizeof(s16));
          FIFO_Y = (s16*)malloc(MAFIFO_SIZE*sizeof(s16));
          FIFO_Z = (s16*)malloc(MAFIFO_SIZE*sizeof(s16));
          memset(FIFO_X, 0, MAFIFO_SIZE*sizeof(s16));
          memset(FIFO_Y, 0, MAFIFO_SIZE*sizeof(s16));
          memset(FIFO_Z, 0, MAFIFO_SIZE*sizeof(s16));
          Correction_Time = SampleRateFreg;
          break;
        case 1:   // 等待 FIFO 填滿靜態資料
          /* 移動平均 Simple Moving Average */
          Acc.X = (s16)MoveAve_SMA(Acc.X, FIFO_X, MAFIFO_SIZE);
          Acc.Y = (s16)MoveAve_SMA(Acc.Y, FIFO_Y, MAFIFO_SIZE);
          Acc.Z = (s16)MoveAve_SMA(Acc.Z, FIFO_Z, MAFIFO_SIZE);
          Correction_Time++;
          break;
        case 2:   // 釋放記憶體 & 計算加速度計偏移量
          free(FIFO_X);
          free(FIFO_Y);
          free(FIFO_Z);
          Acc.OffsetX += (Acc.X - ACC_X_OFFSET);  // 重力加速度為 0g
          Acc.OffsetY += (Acc.Y - ACC_Y_OFFSET);  // 重力加速度為 0g
          Acc.OffsetZ += (Acc.Z - ACC_Z_OFFSET);  // 重力加速度為 1g
          Correction_Time = 0;

//          SEN_STATE = SEN_MAG;
          SEN_STATE = SEN_NUMQ;
          break;
      }
      break;

    /************************** CorrectMag **************************************/
    case SEN_MAG:
      SEN_STATE = SEN_NUMQ;
      break;

    /************************** Quaternion **************************************/
    case SEN_NUMQ:
      switch((u16)(Correction_Time/SampleRateFreg)) {
        case 0:   // 等待 FIFO 填滿靜態資料
          /* 加權移動平均法 Weighted Moving Average */
          Acc.X = (s16)MoveAve_WMA(Acc.X, FIFO_ACC[0], 16);
          Acc.Y = (s16)MoveAve_WMA(Acc.Y, FIFO_ACC[1], 16);
          Acc.Z = (s16)MoveAve_WMA(Acc.Z, FIFO_ACC[2], 16);
          Gyr.X = (s16)MoveAve_WMA(Gyr.X, FIFO_GYR[0], 16);
          Gyr.Y = (s16)MoveAve_WMA(Gyr.Y, FIFO_GYR[1], 16);
          Gyr.Z = (s16)MoveAve_WMA(Gyr.Z, FIFO_GYR[2], 16);
//          Mag.X = (s16)MoveAve_WMA(Mag.X, FIFO_MAG[0], 16);
//          Mag.Y = (s16)MoveAve_WMA(Mag.Y, FIFO_MAG[1], 16);
//          Mag.Z = (s16)MoveAve_WMA(Mag.Z, FIFO_MAG[2], 16);
          Correction_Time++;
          break;
        case 1:

          /* 加權移動平均法 Weighted Moving Average */
          Acc.X = (s16)MoveAve_WMA(Acc.X, FIFO_ACC[0], 16);
          Acc.Y = (s16)MoveAve_WMA(Acc.Y, FIFO_ACC[1], 16);
          Acc.Z = (s16)MoveAve_WMA(Acc.Z, FIFO_ACC[2], 16);
          Gyr.X = (s16)MoveAve_WMA(Gyr.X, FIFO_GYR[0], 16);
          Gyr.Y = (s16)MoveAve_WMA(Gyr.Y, FIFO_GYR[1], 16);
          Gyr.Z = (s16)MoveAve_WMA(Gyr.Z, FIFO_GYR[2], 16);
//          Mag.X = (s16)MoveAve_WMA(Mag.X, FIFO_MAG[0], 16);
//          Mag.Y = (s16)MoveAve_WMA(Mag.Y, FIFO_MAG[1], 16);
//          Mag.Z = (s16)MoveAve_WMA(Mag.Z, FIFO_MAG[2], 16);

          /* To Physical */
          Acc.TrueX  = Acc.X*MPU9250A_4g;       // g/LSB
          Acc.TrueY  = Acc.Y*MPU9250A_4g;       // g/LSB
          Acc.TrueZ  = Acc.Z*MPU9250A_4g;       // g/LSB
          Gyr.TrueX  = Gyr.X*MPU9250G_2000dps;  // dps/LSB
          Gyr.TrueY  = Gyr.Y*MPU9250G_2000dps;  // dps/LSB
          Gyr.TrueZ  = Gyr.Z*MPU9250G_2000dps;  // dps/LSB
//          Mag.TrueX  = Mag.X*MPU9250M_4800uT;   // uT/LSB
//          Mag.TrueY  = Mag.Y*MPU9250M_4800uT;   // uT/LSB
//          Mag.TrueZ  = Mag.Z*MPU9250M_4800uT;   // uT/LSB

          AngE.Pitch = toDeg(atan2f(Acc.TrueY, Acc.TrueZ));
          AngE.Roll  = toDeg(-asinf(Acc.TrueX));
//          AngE.Yaw   = toDeg(atan2f(Ellipse[3], Ellipse[4]))+180.0f;

          Quaternion_ToNumQ(&NumQ, &AngE);

          Correction_Time = 0;
          SEN_STATE = SEN_ALG;
          break;
      }
      break;

    /************************** Algorithm ***************************************/
    case SEN_ALG:

      /* 加權移動平均法 Weighted Moving Average */
      Acc.X = (s16)MoveAve_WMA(Acc.X, FIFO_ACC[0], 8);
      Acc.Y = (s16)MoveAve_WMA(Acc.Y, FIFO_ACC[1], 8);
      Acc.Z = (s16)MoveAve_WMA(Acc.Z, FIFO_ACC[2], 8);
      Gyr.X = (s16)MoveAve_WMA(Gyr.X, FIFO_GYR[0], 8);
      Gyr.Y = (s16)MoveAve_WMA(Gyr.Y, FIFO_GYR[1], 8);
      Gyr.Z = (s16)MoveAve_WMA(Gyr.Z, FIFO_GYR[2], 8);
//      Mag.X = (s16)MoveAve_WMA(Mag.X, FIFO_MAG[0], 16);
//      Mag.Y = (s16)MoveAve_WMA(Mag.Y, FIFO_MAG[1], 16);
//      Mag.Z = (s16)MoveAve_WMA(Mag.Z, FIFO_MAG[2], 16);

      /* To Physical */
      Acc.TrueX  = Acc.X*MPU9250A_4g;       // g/LSB
      Acc.TrueY  = Acc.Y*MPU9250A_4g;       // g/LSB
      Acc.TrueZ  = Acc.Z*MPU9250A_4g;       // g/LSB
      Gyr.TrueX  = Gyr.X*MPU9250G_2000dps;  // dps/LSB
      Gyr.TrueY  = Gyr.Y*MPU9250G_2000dps;  // dps/LSB
      Gyr.TrueZ  = Gyr.Z*MPU9250G_2000dps;  // dps/LSB
//      Mag.TrueX  = Mag.X*MPU9250M_4800uT;   // uT/LSB
//      Mag.TrueY  = Mag.Y*MPU9250M_4800uT;   // uT/LSB
//      Mag.TrueZ  = Mag.Z*MPU9250M_4800uT;   // uT/LSB
//      Temp.TrueT = Temp.T*MPU9250T_85degC;  // degC/LSB

      /* Get Attitude */
      AHRS_Update();
      break;

    /************************** Error *******************************************/
    default:
      while(1);
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
void HardFault_Handler( void )
{
  while(1);
}
void MemManage_Handler( void )
{
  while(1);
}
void BusFault_Handler( void )
{
  while(1);
}
void UsageFault_Handler( void )
{
  while(1);
}
void SVC_Handler( void )
{
  while(1);
}
void DebugMon_Handler( void )
{
  while(1);
}
void PendSV_Handler( void )
{
  while(1);
}
void NMI_Handler( void )
{
  while(1);
}
/*====================================================================================================*/
/*====================================================================================================*/
