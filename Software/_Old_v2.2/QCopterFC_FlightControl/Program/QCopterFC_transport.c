/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_transport.h"
#include "module_nrf24l01.h"
#include "module_imu.h"
#include "algorithm_pid.h"
#include "algorithm_ahrs.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
TR_RECV_DATA RF_RecvData = {0};

extern u8 Time_Min;
extern u8 Time_Sec;
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
  RF_RecvData.Packet            = (u8)(RecvBuf[0]);
  RF_RecvData.Time.Min          = (u8)(RecvBuf[1]);
  RF_RecvData.Time.Sec          = (u8)(RecvBuf[2]);
  RF_RecvData.Time.Cnt          = (u8)(RecvBuf[3]);

  RF_RecvData.KeyR[0].KeyRead   = (u8)(RecvBuf[4]&0x01);
  RF_RecvData.KeyR[0].PressTime = (u8)(RecvBuf[4]>>1);
  RF_RecvData.KeyR[1].KeyRead   = (u8)(RecvBuf[5]&0x01);
  RF_RecvData.KeyR[1].PressTime = (u8)(RecvBuf[5]>>1);
  RF_RecvData.KeyR[2].KeyRead   = (u8)(RecvBuf[6]&0x01);
  RF_RecvData.KeyR[2].PressTime = (u8)(RecvBuf[6]>>1);

  RF_RecvData.KeyL[0].KeyRead   = (u8)(RecvBuf[7]&0x01);
  RF_RecvData.KeyL[0].PressTime = (u8)(RecvBuf[7]>>1);
  RF_RecvData.KeyL[1].KeyRead   = (u8)(RecvBuf[8]&0x01);
  RF_RecvData.KeyL[1].PressTime = (u8)(RecvBuf[8]>>1);
  RF_RecvData.KeyL[2].KeyRead   = (u8)(RecvBuf[9]&0x01);
  RF_RecvData.KeyL[2].PressTime = (u8)(RecvBuf[9]>>1);

  RF_RecvData.KeyP[0].KeyRead   = (u8)(RecvBuf[10]&0x01);
  RF_RecvData.KeyP[0].PressTime = (u8)(RecvBuf[10]>>1);
  RF_RecvData.KeyP[1].KeyRead   = (u8)(RecvBuf[11]&0x01);
  RF_RecvData.KeyP[1].PressTime = (u8)(RecvBuf[11]>>1);
  RF_RecvData.KeyP[2].KeyRead   = (u8)(RecvBuf[12]&0x01);
  RF_RecvData.KeyP[2].PressTime = (u8)(RecvBuf[12]>>1);

  RF_RecvData.KeyS[0].KeyRead   = (u8)(RecvBuf[13]&0x01);
  RF_RecvData.KeyS[0].PressTime = (u8)(RecvBuf[13]>>1);
  RF_RecvData.KeyS[1].KeyRead   = (u8)(RecvBuf[14]&0x01);
  RF_RecvData.KeyS[1].PressTime = (u8)(RecvBuf[14]>>1);
  RF_RecvData.KeyS[2].KeyRead   = (u8)(RecvBuf[15]&0x01);
  RF_RecvData.KeyS[2].PressTime = (u8)(RecvBuf[15]>>1);

  RF_RecvData.Ctrl.ThrB         = (s16)(Byte16(u16, RecvBuf[16], RecvBuf[17]));
  RF_RecvData.Ctrl.ThrS         = (s16)(Byte16(u16, RecvBuf[18], RecvBuf[19]));
  RF_RecvData.Ctrl.Pitch        = (s16)(Byte16(u16, RecvBuf[20], RecvBuf[21]));
  RF_RecvData.Ctrl.Roll         = (s16)(Byte16(u16, RecvBuf[22], RecvBuf[23]));
  RF_RecvData.Ctrl.Yaw          = (s16)(Byte16(u16, RecvBuf[24], RecvBuf[25]));
  RF_RecvData.Ctrl.Corr         = (s16)(Byte16(u16, RecvBuf[26], RecvBuf[27]));

  RF_RecvData.Cmd               = (u8)(RecvBuf[28]);
  RF_RecvData.Data[0]           = (u8)(RecvBuf[29]);
  RF_RecvData.Data[1]           = (u8)(RecvBuf[30]);
  RF_RecvData.Data[2]           = (u8)(RecvBuf[31]);
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : Transport_Send
**功能 : 紀錄發送資料
**輸入 : SendBuf
**輸出 : None
**使用 : Transport_Send(TxBuf);
**=====================================================================================================*/
/*=====================================================================================================*/
void Transport_Send( u8* SendBuf )
{
  s16 TMP_BUF[16] = {0};

//  TMP_BUF[0]  = (s16)(Acc.X); // 1 mg/LSB
//  TMP_BUF[1]  = (s16)(Acc.Y); // 1 mg/LSB
//  TMP_BUF[2]  = (s16)(Acc.Z); // 1 mg/LSB
//  TMP_BUF[3]  = (s16)(Gyr.X); // 10 mdps/LSB
//  TMP_BUF[4]  = (s16)(Gyr.Y); // 10 mdps/LSB
//  TMP_BUF[5]  = (s16)(Gyr.Z); // 10 mdps/LSB
  TMP_BUF[0]  = (s16)(Acc.TrueX*1000);  // 1 mg/LSB
  TMP_BUF[1]  = (s16)(Acc.TrueY*1000);  // 1 mg/LSB
  TMP_BUF[2]  = (s16)(Acc.TrueZ*1000);  // 1 mg/LSB
  TMP_BUF[3]  = (s16)(Gyr.TrueX*10);   // 10 mdps/LSB
  TMP_BUF[4]  = (s16)(Gyr.TrueY*10);   // 10 mdps/LSB
  TMP_BUF[5]  = (s16)(Gyr.TrueZ*10);   // 10 mdps/LSB
//  TMP_BUF[6]  = (s16)(PID_Pitch.Kp);
//  TMP_BUF[7]  = (s16)(PID_Pitch.Ki);
//  TMP_BUF[8]  = (s16)(PID_Pitch.Kd);
//  TMP_BUF[6]  = (s16)(PID_Pitch.Kp*1000);
//  TMP_BUF[7]  = (s16)(PID_Pitch.Ki*1000);
//  TMP_BUF[8]  = (s16)(PID_Pitch.Kd*1000);
  TMP_BUF[6]  = (s16)(PID_Yaw.Kp*1000);
  TMP_BUF[7]  = (s16)(PID_Yaw.Ki*1000);
  TMP_BUF[8]  = (s16)(PID_Yaw.Kd*1000);
  TMP_BUF[9]  = (s16)(AngE.Pitch*100);
  TMP_BUF[10] = (s16)(AngE.Roll*100);
  TMP_BUF[11] = (s16)(AngE.Yaw*10);
  TMP_BUF[12] = (s16)((BasicThr-BLDC_PWM_MIN)/0.12f);
  TMP_BUF[13] = (s16)(Time_Sec);
  TMP_BUF[14] = (s16)(Time_Min);

  SendBuf[0]  = (u8)(0x01);
  SendBuf[1]  = (u8)(0x02);
  SendBuf[2]  = (u8)Byte8L(TMP_BUF[0]);
  SendBuf[3]  = (u8)Byte8H(TMP_BUF[0]);
  SendBuf[4]  = (u8)Byte8L(TMP_BUF[1]);
  SendBuf[5]  = (u8)Byte8H(TMP_BUF[1]);
  SendBuf[6]  = (u8)Byte8L(TMP_BUF[2]);
  SendBuf[7]  = (u8)Byte8H(TMP_BUF[2]);
  SendBuf[8]  = (u8)Byte8L(TMP_BUF[3]);
  SendBuf[9]  = (u8)Byte8H(TMP_BUF[3]);
  SendBuf[10] = (u8)Byte8L(TMP_BUF[4]);
  SendBuf[11] = (u8)Byte8H(TMP_BUF[4]);
  SendBuf[12] = (u8)Byte8L(TMP_BUF[5]);
  SendBuf[13] = (u8)Byte8H(TMP_BUF[5]);
  SendBuf[14] = (u8)Byte8L(TMP_BUF[6]);
  SendBuf[15] = (u8)Byte8H(TMP_BUF[6]);
  SendBuf[16] = (u8)Byte8L(TMP_BUF[7]);
  SendBuf[17] = (u8)Byte8H(TMP_BUF[7]);
  SendBuf[18] = (u8)Byte8L(TMP_BUF[8]);
  SendBuf[19] = (u8)Byte8H(TMP_BUF[8]);
  SendBuf[20] = (u8)Byte8L(TMP_BUF[9]);
  SendBuf[21] = (u8)Byte8H(TMP_BUF[9]);
  SendBuf[22] = (u8)Byte8L(TMP_BUF[10]);
  SendBuf[23] = (u8)Byte8H(TMP_BUF[10]);
  SendBuf[24] = (u8)Byte8L(TMP_BUF[11]);
  SendBuf[25] = (u8)Byte8H(TMP_BUF[11]);
  SendBuf[26] = (u8)Byte8L(TMP_BUF[12]);
  SendBuf[27] = (u8)Byte8H(TMP_BUF[12]);
  SendBuf[28] = (u8)(0);
  SendBuf[29] = (u8)(0);
  SendBuf[30] = (u8)(TMP_BUF[13]);
  SendBuf[31] = (u8)(TMP_BUF[14]);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
