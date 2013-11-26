/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_transport.h"
#include "module_ms5611.h"
#include "module_sensor.h"
#include "module_nrf24l01.h"
#include "algorithm_quaternion.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
extern vs16 Tmp_PID_KP;
extern vs16 Tmp_PID_KI;
extern vs16 Tmp_PID_KD;
extern vs16 Tmp_PID_Pitch;

vu16 RecvData[KeyNums] = {0};
u8 TxBuf[SendTimes][TxBufSize] = {0};
u8 RxBuf[ReadTimes][RxBufSize] = {0};
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Transport_Recv
**功能 : 紀錄接收資料
**輸入 : Recv_Buf
**輸出 : None
**使用 : Transport_Recv(RxBuf[0]);
**=====================================================================================================*/
/*=====================================================================================================*/
void Transport_Recv( u8* RecvBuf )
{
  KEYR_J    = (u16)RecvBuf[0];
  KEYR_U    = (u16)RecvBuf[1];
  KEYR_D    = (u16)RecvBuf[2];
  KEYR_L    = (u16)RecvBuf[3];
  KEYR_R    = (u16)RecvBuf[4];
  KEYR_S1   = (u16)RecvBuf[5];
  KEYR_S2   = (u16)RecvBuf[6];
  KEYL_J    = (u16)RecvBuf[7];
  KEYL_U    = (u16)RecvBuf[8];
  KEYL_D    = (u16)RecvBuf[9];
  KEYL_L    = (u16)RecvBuf[10];
  KEYL_R    = (u16)RecvBuf[11];
  KEYL_S1   = (u16)RecvBuf[12];
  KEYL_S2   = (u16)RecvBuf[13];
  Exp_Pitch = (u16)((RecvBuf[15] << 8) | RecvBuf[14]);
  Exp_Roll  = (u16)((RecvBuf[17] << 8) | RecvBuf[16]);
//   JSR_X     = (u16)((RecvBuf[15] << 8) | RecvBuf[14]);
//   JSR_Y     = (u16)((RecvBuf[17] << 8) | RecvBuf[16]);
  JSR_Z     = (u16)((RecvBuf[19] << 8) | RecvBuf[18]);
  Exp_Yaw   = (u16)((RecvBuf[21] << 8) | RecvBuf[20]);
  Exp_Thr   = (u16)((RecvBuf[23] << 8) | RecvBuf[22]);
//   JSL_X     = (u16)((RecvBuf[21] << 8) | RecvBuf[20]);
//   JSL_Y     = (u16)((RecvBuf[23] << 8) | RecvBuf[22]);
  JSL_Z     = (u16)((RecvBuf[25] << 8) | RecvBuf[24]);
//   Exp_Pitch = (u16)((RecvBuf[27] << 8) | RecvBuf[26]);
//   Exp_Roll  = (u16)((RecvBuf[29] << 8) | RecvBuf[28]);
//   Exp_Thr   = (u16)((RecvBuf[31] << 8) | RecvBuf[30]);
  RecvTime_Sec = (u8)(RecvBuf[30]);
  RecvTime_Min = (u8)(RecvBuf[31]);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Transport_Send
**功能 : 紀錄發送資料
**輸入 : SendBuf
**輸出 : None
**使用 : Transport_Send(TxBuf[0]);
**=====================================================================================================*/
/*=====================================================================================================*/
void Transport_Send( u8* SendBuf )
{
  s16 TMP_BUF[16] = {0};

  TMP_BUF[0]  = (s16)(Acc.TrueX*1000);  // 1 mg/LSB
  TMP_BUF[1]  = (s16)(Acc.TrueY*1000);  // 1 mg/LSB
  TMP_BUF[2]  = (s16)(Acc.TrueZ*1000);  // 1 mg/LSB
//  TMP_BUF[0]  = (s16)(Acc.X);  // 1 mg/LSB
//  TMP_BUF[1]  = (s16)(Acc.Y);  // 1 mg/LSB
//  TMP_BUF[2]  = (s16)(Acc.Z);  // 1 mg/LSB
  TMP_BUF[3]  = (s16)(Gyr.TrueX*100);   // 10 mdps/LSB
  TMP_BUF[4]  = (s16)(Gyr.TrueY*100);   // 10 mdps/LSB
  TMP_BUF[5]  = (s16)(Gyr.TrueZ*100);   // 10 mdps/LSB
  TMP_BUF[6]  = (s16)(Mag.TrueX);       // 10 nTesla/LSB
  TMP_BUF[7]  = (s16)(Mag.TrueY);       // 10 nTesla/LSB
  TMP_BUF[8]  = (s16)(Mag.TrueZ);       // 10 nTesla/LSB
//  TMP_BUF[6]  = (s16)(Acc.OffsetX);       // 10 nTesla/LSB
//  TMP_BUF[7]  = (s16)(Acc.OffsetY);       // 10 nTesla/LSB
//  TMP_BUF[8]  = (s16)(Acc.OffsetZ);       // 10 nTesla/LSB
  TMP_BUF[9]  = (s16)(AngE.Pitch*100);  // 10 mdeg/LSB
  TMP_BUF[10] = (s16)(AngE.Roll*100);   // 10 mdeg/LSB
  TMP_BUF[11] = (s16)(AngE.Yaw*10);     // 100 mdeg/LSB
  TMP_BUF[12] = (s16)(Temp.TrueT*100);  // 0.01 degC/LSB
  TMP_BUF[13] = (s16)(Baro.Temp*100);   // 0.01 degC/LSB
  TMP_BUF[14] = (s16)(Baro.Press*10);   // 0.1 mbar/LSB
  TMP_BUF[15] = (s16)(Baro.High);

  SendBuf[0]  = (u8)(0x01);
  SendBuf[1]  = (u8)(0x02);
  SendBuf[2]  = (u8)(TMP_BUF[0]);       // Acc X-Axis
  SendBuf[3]  = (u8)(TMP_BUF[0] >> 8);  // ACC X-Axis
  SendBuf[4]  = (u8)(TMP_BUF[1]);       // ACC Y-Axis
  SendBuf[5]  = (u8)(TMP_BUF[1] >> 8);  // ACC Y-Axis
  SendBuf[6]  = (u8)(TMP_BUF[2]);       // ACC Z-Axis
  SendBuf[7]  = (u8)(TMP_BUF[2] >> 8);  // ACC Z-Axis
  SendBuf[8]  = (u8)(TMP_BUF[3]);       // Gyr X-Axis
  SendBuf[9]  = (u8)(TMP_BUF[3] >> 8);  // Gyr X-Axis
  SendBuf[10] = (u8)(TMP_BUF[4]);       // Gyr Y-Axis
  SendBuf[11] = (u8)(TMP_BUF[4] >> 8);  // Gyr Y-Axis
  SendBuf[12] = (u8)(TMP_BUF[5]);       // Gyr Z-Axis
  SendBuf[13] = (u8)(TMP_BUF[5] >> 8);  // Gyr Z-Axis
//  SendBuf[14] = (u8)(TMP_BUF[6]);       // Mag X-Axis
//  SendBuf[15] = (u8)(TMP_BUF[6] >> 8);  // Mag X-Axis
//  SendBuf[16] = (u8)(TMP_BUF[7]);       // Mag Y-Axis
//  SendBuf[17] = (u8)(TMP_BUF[7] >> 8);  // Mag Y-Axis
//  SendBuf[18] = (u8)(TMP_BUF[8]);       // Mag Z-Axis
//  SendBuf[19] = (u8)(TMP_BUF[8] >> 8);  // Mag Z-Axis
  SendBuf[14] = (u8)(Tmp_PID_KP);
  SendBuf[15] = (u8)(Tmp_PID_KP >> 8);
  SendBuf[16] = (u8)(Tmp_PID_KI);
  SendBuf[17] = (u8)(Tmp_PID_KI >> 8);
  SendBuf[18] = (u8)(Tmp_PID_KD);
  SendBuf[19] = (u8)(Tmp_PID_KD >> 8);

  SendBuf[20] = (u8)(TMP_BUF[9]);       // Ang Pitch
  SendBuf[21] = (u8)(TMP_BUF[9] >> 8);  // Ang Pitch
  SendBuf[22] = (u8)(TMP_BUF[10]);      // Ang Roll
  SendBuf[23] = (u8)(TMP_BUF[10] >> 8); // Ang Roll
  SendBuf[24] = (u8)(TMP_BUF[11]);      // Ang Yaw
  SendBuf[25] = (u8)(TMP_BUF[11] >> 8); // Ang Yaw
  SendBuf[28] = (u8)(Tmp_PID_Pitch);
  SendBuf[29] = (u8)(Tmp_PID_Pitch >> 8);
  SendBuf[30] = (u8)(Time_Sec);
  SendBuf[31] = (u8)(Time_Min);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
