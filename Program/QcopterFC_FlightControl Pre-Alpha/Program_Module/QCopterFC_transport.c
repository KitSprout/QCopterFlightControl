/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_transport.h"
#include "module_sensor.h"
#include "module_nrf24l01.h"
#include "algorithm_quaternion.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vu16 RecvData[KeyNums] = {0};
u8 TxBuf[SendTimes][TxBufSize] = {0};
u8 RxBuf[ReadTimes][RxBufSize] = {0};
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Transport_Recv
**功能 : 接收
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
//  Exp_Pitch = (u16)((RecvBuf[15] << 8) | RecvBuf[14]);
//  Exp_Roll  = (u16)((RecvBuf[17] << 8) | RecvBuf[16]);
  JSR_X     = (u16)((RecvBuf[15] << 8) | RecvBuf[14]);
  JSR_Y     = (u16)((RecvBuf[17] << 8) | RecvBuf[16]);
  JSR_Z     = (u16)((RecvBuf[19] << 8) | RecvBuf[18]);
//  Exp_Yaw   = (u16)((RecvBuf[21] << 8) | RecvBuf[20]);
//  Exp_Thr   = (u16)((RecvBuf[23] << 8) | RecvBuf[22]);
  JSL_X     = (u16)((RecvBuf[21] << 8) | RecvBuf[20]);
  JSL_Y     = (u16)((RecvBuf[23] << 8) | RecvBuf[22]);
  JSL_Z     = (u16)((RecvBuf[25] << 8) | RecvBuf[24]);
//  Exp_Pitch = (u16)((RecvBuf[27] << 8) | RecvBuf[26]);
//  Exp_Roll  = (u16)((RecvBuf[29] << 8) | RecvBuf[28]);
//  Exp_Thr   = (u16)((RecvBuf[31] << 8) | RecvBuf[30]);
  RecvTime_Sec = (u8)(RecvBuf[30]);
  RecvTime_Min = (u8)(RecvBuf[31]);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Transport_Send
**功能 : 接收
**輸入 : SendBuf
**輸出 : None
**使用 : Transport_Send(TxBuf[0]);
**=====================================================================================================*/
/*=====================================================================================================*/
void Transport_Send( u8* SendBuf )
{
 Acc.X = (s16)(Acc.TrueX*1000);  // 1 mg/LSB
 Acc.Y = (s16)(Acc.TrueY*1000);  // 1 mg/LSB
 Acc.Z = (s16)(Acc.TrueZ*1000);  // 1 mg/LSB
 Gyr.X = (s16)(Gyr.TrueX*100);   // 10 mdps/LSB
 Gyr.Y = (s16)(Gyr.TrueY*100);   // 10 mdps/LSB
 Gyr.Z = (s16)(Gyr.TrueZ*100);   // 10 mdps/LSB
 Mag.X = (s16)(Mag.TrueX*10000); // 100 ugauss/LSB
 Mag.Y = (s16)(Mag.TrueY*10000); // 100 ugauss/LSB
 Mag.Z = (s16)(Mag.TrueZ*10000); // 100 ugauss/LSB
 Ang.X = (s16)(AngE.Pitch*100);  // 10 mdeg/LSB
 Ang.Y = (s16)(AngE.Roll*100);   // 10 mdeg/LSB
 Ang.Z = (s16)(AngE.Yaw*10);     // 100 mdeg/LSB

//  Mag.X = (s16)(Mag.X*100); // 100 ugauss/LSB
//  Mag.Y = (s16)(Mag.Y*100); // 100 ugauss/LSB
//  Mag.Z = (s16)(Mag.Z*100); // 100 ugauss/LSB

  SendBuf[0]  = (u8)(0x01);
  SendBuf[1]  = (u8)(0x02);
  SendBuf[2]  = (u8)(Acc.X);
  SendBuf[3]  = (u8)(Acc.X >> 8);
  SendBuf[4]  = (u8)(Acc.Y);
  SendBuf[5]  = (u8)(Acc.Y >> 8);
  SendBuf[6]  = (u8)(Acc.Z);
  SendBuf[7]  = (u8)(Acc.Z >> 8);
  SendBuf[8]  = (u8)(Gyr.X);
  SendBuf[9]  = (u8)(Gyr.X >> 8);
  SendBuf[10] = (u8)(Gyr.Y);
  SendBuf[11] = (u8)(Gyr.Y >> 8);
  SendBuf[12] = (u8)(Gyr.Z);
  SendBuf[13] = (u8)(Gyr.Z >> 8);
  SendBuf[14] = (u8)(Mag.X);
  SendBuf[15] = (u8)(Mag.X >> 8);
  SendBuf[16] = (u8)(Mag.Y);
  SendBuf[17] = (u8)(Mag.Y >> 8);
  SendBuf[18] = (u8)(Mag.Z);
  SendBuf[19] = (u8)(Mag.Z >> 8);
  SendBuf[20] = (u8)(Ang.X);
  SendBuf[21] = (u8)(Ang.X >> 8);
  SendBuf[22] = (u8)(Ang.Y);
  SendBuf[23] = (u8)(Ang.Y >> 8);
  SendBuf[24] = (u8)(Ang.Z);
  SendBuf[25] = (u8)(Ang.Z >> 8);
  SendBuf[30] = (u8)(Time_Sec);
  SendBuf[31] = (u8)(Time_Min);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
