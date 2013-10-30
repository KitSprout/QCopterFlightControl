/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
#include "QCopterFC.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_ahrs.h"
#include "QCopterFC_transport.h"
#include "module_board.h"
#include "module_motor.h"
#include "module_sensor.h"
#include "module_mpu9150.h"
#include "module_ms5611.h"
#include "algorithm_PID.h"
#include "algorithm_moveAve.h"
#include "algorithm_mathUnit.h"
#include "algorithm_quaternion.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu8 Time_Sec = 0;
vu8 Time_Min = 0;
vu8 RecvTime_Sec = 0;
vu8 RecvTime_Min = 0;
vu16 SysTick_Cnt = 0;

vs16 Tmp_PID_KP;
vs16 Tmp_PID_KI;
vs16 Tmp_PID_KD;
vs16 Tmp_PID_Pitch;

Sensor_Mode SensorMode = Mode_GyrCorrect;
/*=====================================================================================================*/
/*=====================================================================================================*/
void SysTick_Handler( void )
{
  u8 IMU_Buf[20] = {0};

  u16 Final_M1 = 0;
  u16 Final_M2 = 0;
  u16 Final_M3 = 0;
  u16 Final_M4 = 0;

  s16 Thr = 0, Pitch = 0, Roll = 0, Yaw = 0;

  static u8  BaroCnt = 0;

  static s16 ACC_FIFO[3][256] = {0};
  static s16 GYR_FIFO[3][256] = {0};
  static s16 MAG_FIFO[3][256] = {0};

  static s16 MagDataX[8] = {0};
  static s16 MagDataY[8] = {0};

  static u16 Correction_Time = 0;

  /* Time Count */
  SysTick_Cnt++;
  if(SysTick_Cnt == 400) {
    SysTick_Cnt = 0;
    Time_Sec++;
    if(Time_Sec == 60) {	// 0~59
      Time_Sec = 0;
      Time_Min++;
      if(Time_Sec == 60)
      Time_Min = 0;
    }
  }

  /* 400Hz, Read Sensor ( Accelerometer, Gyroscope, Magnetometer ) */
  MPU9150_Read(IMU_Buf);

  /* 100Hz, Read Barometer */
  BaroCnt++;
  if(BaroCnt == 4) {
    MS5611_Read(&Baro, MS5611_D1_OSR_4096);
    BaroCnt = 0;
  }

  Acc.X  = (s16)((IMU_Buf[0]  << 8) | IMU_Buf[1]);
  Acc.Y  = (s16)((IMU_Buf[2]  << 8) | IMU_Buf[3]);
  Acc.Z  = (s16)((IMU_Buf[4]  << 8) | IMU_Buf[5]);
  Temp.T = (s16)((IMU_Buf[6]  << 8) | IMU_Buf[7]);
  Gyr.X  = (s16)((IMU_Buf[8]  << 8) | IMU_Buf[9]);
  Gyr.Y  = (s16)((IMU_Buf[10] << 8) | IMU_Buf[11]);
  Gyr.Z  = (s16)((IMU_Buf[12] << 8) | IMU_Buf[13]);
  Mag.X  = (s16)((IMU_Buf[14] << 8) | IMU_Buf[15]);
  Mag.Y  = (s16)((IMU_Buf[16] << 8) | IMU_Buf[17]);
  Mag.Z  = (s16)((IMU_Buf[18] << 8) | IMU_Buf[19]);

  /* Offset */
  Acc.X -= Acc.OffsetX;
  Acc.Y -= Acc.OffsetY;
  Acc.Z -= Acc.OffsetZ;
  Gyr.X -= Gyr.OffsetX;
  Gyr.Y -= Gyr.OffsetY;
  Gyr.Z -= Gyr.OffsetZ;
  Mag.X -= Mag.OffsetX;
  Mag.Y -= Mag.OffsetY;
  Mag.Z -= Mag.OffsetZ;

  #define MovegAveFIFO_Size 250
  switch(SensorMode) {

  /************************** Mode_CorrectGyr **************************************/
    case Mode_GyrCorrect:
      LED_R = !LED_R;
      /* Simple Moving Average */
      Gyr.X = (s16)MoveAve_SMA(Gyr.X, GYR_FIFO[0], MovegAveFIFO_Size);
      Gyr.Y = (s16)MoveAve_SMA(Gyr.Y, GYR_FIFO[1], MovegAveFIFO_Size);
      Gyr.Z = (s16)MoveAve_SMA(Gyr.Z, GYR_FIFO[2], MovegAveFIFO_Size);

      Correction_Time++;  // 等待 FIFO 填滿空值 or 填滿靜態資料
      if(Correction_Time == 400) {
        Gyr.OffsetX += (Gyr.X - GYR_X_OFFSET);  // 角速度為 0dps
        Gyr.OffsetY += (Gyr.Y - GYR_Y_OFFSET);  // 角速度為 0dps
        Gyr.OffsetZ += (Gyr.Z - GYR_Z_OFFSET);  // 角速度為 0dps

        Correction_Time = 0;
        SensorMode = Mode_AccCorrect;
      }
      break;
 
  /************************** Mode_CorrectAcc **************************************/
    case Mode_AccCorrect:
      LED_R = ~LED_R;
      Acc.X = (s16)MoveAve_SMA(Acc.X, ACC_FIFO[0], MovegAveFIFO_Size);
      Acc.Y = (s16)MoveAve_SMA(Acc.Y, ACC_FIFO[1], MovegAveFIFO_Size);
      Acc.Z = (s16)MoveAve_SMA(Acc.Z, ACC_FIFO[2], MovegAveFIFO_Size);

      Correction_Time++;  // 等待 FIFO 填滿空值 or 填滿靜態資料
      if(Correction_Time == 400) {
        Acc.OffsetX += (Acc.X - ACC_X_OFFSET);  // 重力加速度為 0g
        Acc.OffsetY += (Acc.Y - ACC_Y_OFFSET);  // 重力加速度為 0g
        Acc.OffsetZ += (Acc.Z - ACC_Z_OFFSET);  // 重力加速度為 1g

        Correction_Time = 0;
        SensorMode = Mode_Quaternion;   // Mode_CorrectMag
      }
      break;

  /************************** Mode_CorrectMag **************************************/
    #define MagCorrect_Ave    100
    #define MagCorrect_Delay  600   // 2.5ms * 600 = 1.5s
    case Mode_MagCorrect:
      LED_R = !LED_R;
      Correction_Time++;
      switch((u16)(Correction_Time/MagCorrect_Delay)) {
        case 0:
          LED_B = 0;
          MagDataX[0] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[0] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        case 1:
          LED_B = 1;
          MagDataX[1] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[1] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        case 2:
          LED_B = 0;
          MagDataX[2] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[2] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        case 3:
          LED_B = 1;
          MagDataX[3] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[3] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        case 4:
          LED_B = 0;
          MagDataX[4] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[4] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        case 5:
          LED_B = 1;
          MagDataX[5] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[5] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        case 6:
          LED_B = 0;
          MagDataX[6] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[6] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        case 7:
          LED_B = 1;
          MagDataX[7] = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], MagCorrect_Ave);
          MagDataY[7] = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], MagCorrect_Ave);
          break;
        default:
          LED_B = 1;
          EllipseFitting(Ellipse, MagDataX, MagDataY, 8);
          Mag.OffsetX = Ellipse[1];
          Mag.OffsetY = Ellipse[2];

          Correction_Time = 0;
          SensorMode = Mode_Quaternion;
          break;
      }
      break;

  /************************** Algorithm Mode **************************************/
    case Mode_Quaternion:
      LED_R = !LED_R;
      /* To Physical */
      Acc.TrueX = Acc.X*MPU9150A_4g;      // g/LSB
      Acc.TrueY = Acc.Y*MPU9150A_4g;      // g/LSB
      Acc.TrueZ = Acc.Z*MPU9150A_4g;      // g/LSB
      Gyr.TrueX = Gyr.X*MPU9150G_2000dps; // dps/LSB
      Gyr.TrueY = Gyr.Y*MPU9150G_2000dps; // dps/LSB
      Gyr.TrueZ = Gyr.Z*MPU9150G_2000dps; // dps/LSB
      Mag.TrueX = Mag.X*MPU9150M_1200uT;  // uT/LSB
      Mag.TrueY = Mag.Y*MPU9150M_1200uT;  // uT/LSB
      Mag.TrueZ = Mag.Z*MPU9150M_1200uT;  // uT/LSB

      AngE.Pitch = toDeg(atan2f(Acc.TrueY, Acc.TrueZ));
      AngE.Roll  = toDeg(-asinf(Acc.TrueX));
      AngE.Yaw   = 0;

      Quaternion_ToNumQ(&NumQ, &AngE);

      SensorMode = Mode_Algorithm;
      break;

  /************************** Algorithm Mode ****************************************/
    case Mode_Algorithm:

      /* 加權移動平均法 Weighted Moving Average */
      Acc.X = (s16)MoveAve_WMA(Acc.X, ACC_FIFO[0], 8);
      Acc.Y = (s16)MoveAve_WMA(Acc.Y, ACC_FIFO[1], 8);
      Acc.Z = (s16)MoveAve_WMA(Acc.Z, ACC_FIFO[2], 8);
      Gyr.X = (s16)MoveAve_WMA(Gyr.X, GYR_FIFO[0], 8);
      Gyr.Y = (s16)MoveAve_WMA(Gyr.Y, GYR_FIFO[1], 8);
      Gyr.Z = (s16)MoveAve_WMA(Gyr.Z, GYR_FIFO[2], 8);
      Mag.X = (s16)MoveAve_WMA(Mag.X, MAG_FIFO[0], 8);
      Mag.Y = (s16)MoveAve_WMA(Mag.Y, MAG_FIFO[1], 8);
      Mag.Z = (s16)MoveAve_WMA(Mag.Z, MAG_FIFO[2], 8);

      /* To Physical */
      Acc.TrueX = Acc.X*MPU9150A_4g;        // g/LSB
      Acc.TrueY = Acc.Y*MPU9150A_4g;        // g/LSB
      Acc.TrueZ = Acc.Z*MPU9150A_4g;        // g/LSB
      Gyr.TrueX = Gyr.X*MPU9150G_2000dps;   // dps/LSB
      Gyr.TrueY = Gyr.Y*MPU9150G_2000dps;   // dps/LSB
      Gyr.TrueZ = Gyr.Z*MPU9150G_2000dps;   // dps/LSB
      Mag.TrueX = Mag.X*MPU9150M_1200uT;    // uT/LSB
      Mag.TrueY = Mag.Y*MPU9150M_1200uT;    // uT/LSB
      Mag.TrueZ = Mag.Z*MPU9150M_1200uT;    // uT/LSB
      Temp.TrueT = Temp.T*MPU9150T_85degC;  // degC/LSB

      /* Get Attitude Angle */
      AHRS_Update();

//      if(KEYL_U == 0)	{	PID_Roll.Kp += 0.001f;	PID_Pitch.Kp += 0.001f;  }
//      if(KEYL_L == 0)	{	PID_Roll.Kp -= 0.001f;	PID_Pitch.Kp -= 0.001f;  }
//      if(KEYL_R == 0)	{	PID_Roll.Ki += 0.0001f;	PID_Pitch.Ki += 0.0001f; }
//      if(KEYL_D == 0)	{	PID_Roll.Ki -= 0.0001f;	PID_Pitch.Ki -= 0.0001f; }
//      if(KEYR_R == 0)	{	PID_Roll.Kd += 0.0001f;	PID_Pitch.Kd += 0.0001f; }
//      if(KEYR_D == 0)	{	PID_Roll.Kd -= 0.0001f;	PID_Pitch.Kd -= 0.0001f; }
//      if(KEYR_L == 0)	{	PID_Roll.SumErr = 0.0f;	PID_Pitch.SumErr = 0.0f; }
//      if(KEYL_U == 0)	{	PID_Yaw.Kp += 0.001f;    }
//      if(KEYL_L == 0)	{	PID_Yaw.Kp -= 0.001f;    }
//      if(KEYL_R == 0)	{	PID_Yaw.Ki += 0.0001f;	 }
//      if(KEYL_D == 0)	{	PID_Yaw.Ki -= 0.0001f;	 }
//      if(KEYR_R == 0)	{	PID_Yaw.Kd += 0.0001f;	 }
//      if(KEYR_D == 0)	{	PID_Yaw.Kd -= 0.0001f;	 }
//      if(KEYR_L == 0)	{	PID_Roll.SumErr = 0.0f;	 }

      /* Get ZeroErr */
      PID_Pitch.ZeroErr = (float)((s16)Exp_Pitch/2.5f);
      PID_Roll.ZeroErr  = (float)((s16)Exp_Roll/2.5f);
      PID_Yaw.ZeroErr   = 180.0f+(float)((s16)Exp_Yaw);

      /* PID */
      Roll  = (s16)PID_AHRS_Cal(&PID_Roll,  AngE.Roll,  Gyr.TrueX);
      Pitch = (s16)PID_AHRS_Cal(&PID_Pitch, AngE.Pitch, Gyr.TrueY);
//      Yaw   = (s16)PID_AHRS_Cal(&PID_Yaw,   AngE.Yaw,   Gyr.TrueZ);
      Yaw   = (s16)(PID_Yaw.Kd*Gyr.TrueZ);
      Thr   = (s16)Exp_Thr;

      /* Motor Ctrl */
      Final_M1 = PWM_M1 + Thr + Pitch + Roll + Yaw;
      Final_M2 = PWM_M2 + Thr - Pitch + Roll - Yaw;
      Final_M3 = PWM_M3 + Thr - Pitch - Roll + Yaw;
      Final_M4 = PWM_M4 + Thr + Pitch - Roll - Yaw;

      /* Check Connection */
      #define NoSignal 1
      if(KEYR_L == 0)
        Motor_Control(PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN);
      else if(((Time_Sec-RecvTime_Sec)>NoSignal) || ((Time_Sec-RecvTime_Sec)<-NoSignal))
        Motor_Control(PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN);
      else
        Motor_Control(Final_M1, Final_M2, Final_M3, Final_M4);

      /* DeBug */
      Tmp_PID_KP = PID_Pitch.Kp*1000;
      Tmp_PID_KI = PID_Pitch.Ki*1000;
      Tmp_PID_KD = PID_Pitch.Kd*1000;
      Tmp_PID_Pitch = Roll;

      break;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void DMA1_Stream0_IRQHandler( void )
{
	I2C1_Recv_DMA_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void DMA1_Stream6_IRQHandler( void )
{
	I2C1_Send_DMA_IRQ();
}
/*=====================================================================================================*/
/*=====================================================================================================*/
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
/*=====================================================================================================*/
/*=====================================================================================================*/
