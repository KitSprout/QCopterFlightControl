/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_transport.h"
#include "module_ms5611.h"
#include "module_sensor.h"
#include "module_nrf24l01.h"
#include "algorithm_PID.h"
#include "algorithm_quaternion.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
TR_RECV_DATA RF_RecvData;
TR_SEND_DATA RF_SendData;
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
  RF_RecvData.Time.mSec         = (u8)(RecvBuf[3]);

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

  RF_RecvData.Ctrl.ThrB         = (s16)(Byte16(RecvBuf[16], RecvBuf[17]));
  RF_RecvData.Ctrl.ThrS         = (s16)(Byte16(RecvBuf[18], RecvBuf[19]));
  RF_RecvData.Ctrl.Pitch        = (s16)(Byte16(RecvBuf[20], RecvBuf[21]));
  RF_RecvData.Ctrl.Roll         = (s16)(Byte16(RecvBuf[22], RecvBuf[23]));
  RF_RecvData.Ctrl.Yaw          = (s16)(Byte16(RecvBuf[24], RecvBuf[25]));
  RF_RecvData.Ctrl.Corr         = (s16)(Byte16(RecvBuf[26], RecvBuf[27]));

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
  SendBuf[0] = (u8)(RF_SendData.Packet);
  SendBuf[1] = (u8)(RF_SendData.Time.Min);
  SendBuf[2] = (u8)(RF_SendData.Time.Sec);
  SendBuf[3] = (u8)(RF_SendData.Time.mSec);

  switch(RF_SendData.Packet) {

    case 0x01:

      RF_SendData.PID.KP = (u16)(PID_Pitch.Kp*1000);
      RF_SendData.PID.KI = (u16)(PID_Pitch.Ki*1000);
      RF_SendData.PID.KD = (u16)(PID_Pitch.Kd*1000);
//      RF_SendData.Thr.CH1 = Final_M1;
//      RF_SendData.Thr.CH2 = Final_M2;
//      RF_SendData.Thr.CH3 = Final_M3;
//      RF_SendData.Thr.CH4 = Final_M4;

      SendBuf[4]  = (u8)Byte8H(RF_SendData.PID.KP);
      SendBuf[5]  = (u8)Byte8L(RF_SendData.PID.KP);
      SendBuf[6]  = (u8)Byte8H(RF_SendData.PID.KI);
      SendBuf[7]  = (u8)Byte8L(RF_SendData.PID.KI);
      SendBuf[8]  = (u8)Byte8H(RF_SendData.PID.KD);
      SendBuf[9]  = (u8)Byte8L(RF_SendData.PID.KD);
      SendBuf[10] = (u8)Byte8H(RF_SendData.Thr.CH1);
      SendBuf[11] = (u8)Byte8L(RF_SendData.Thr.CH1);
      SendBuf[12] = (u8)Byte8H(RF_SendData.Thr.CH2);
      SendBuf[13] = (u8)Byte8L(RF_SendData.Thr.CH2);
      SendBuf[14] = (u8)Byte8H(RF_SendData.Thr.CH3);
      SendBuf[15] = (u8)Byte8L(RF_SendData.Thr.CH3);
      SendBuf[16] = (u8)Byte8H(RF_SendData.Thr.CH4);
      SendBuf[17] = (u8)Byte8L(RF_SendData.Thr.CH4);
      SendBuf[18] = (u8)Byte8H(RF_SendData.Thr.CH5);
      SendBuf[19] = (u8)Byte8L(RF_SendData.Thr.CH5);
      SendBuf[20] = (u8)Byte8H(RF_SendData.Thr.CH6);
      SendBuf[21] = (u8)Byte8L(RF_SendData.Thr.CH6);
      SendBuf[22] = (u8)RF_SendData.Batery.Vol;
      SendBuf[23] = (u8)RF_SendData.Batery.Cur;
      SendBuf[24] = (u8)Byte8H(RF_SendData.Batery.Cap);
      SendBuf[25] = (u8)Byte8L(RF_SendData.Batery.Cap);
      SendBuf[26] = (u8)(0);
      SendBuf[27] = (u8)(0);
      SendBuf[28] = (u8)(0);
      SendBuf[29] = (u8)(0);
      SendBuf[30] = (u8)(0);
      SendBuf[31] = (u8)(0);
      break;

    case 0x02:

      RF_SendData.Acc.X       = (s16)(Acc.TrueX*1000);  // 1 mg/LSB
      RF_SendData.Acc.Y       = (s16)(Acc.TrueY*1000);  // 1 mg/LSB
      RF_SendData.Acc.Z       = (s16)(Acc.TrueZ*1000);  // 1 mg/LSB
      RF_SendData.Gyr.X       = (s16)(Gyr.TrueX*100);   // 10 mdps/LSB
      RF_SendData.Gyr.Y       = (s16)(Gyr.TrueY*100);   // 10 mdps/LSB
      RF_SendData.Gyr.Z       = (s16)(Gyr.TrueZ*100);   // 10 mdps/LSB
      RF_SendData.Mag.X       = (s16)(Mag.TrueX);       // 10 nTesla/LSB
      RF_SendData.Mag.Y       = (s16)(Mag.TrueY);       // 10 nTesla/LSB
      RF_SendData.Mag.Z       = (s16)(Mag.TrueZ);       // 10 nTesla/LSB
      RF_SendData.Baro.Temp   = (u16)(Baro.Temp*100);   // 0.01 degC/LSB
      RF_SendData.Baro.Press  = (u32)(Baro.Press*100);  // 0.1 mbar/LSB
      RF_SendData.Baro.Height = (u32)(Baro.Height); // 

      SendBuf[4]  = (u8)Byte8H(RF_SendData.Acc.X);
      SendBuf[5]  = (u8)Byte8L(RF_SendData.Acc.X);
      SendBuf[6]  = (u8)Byte8H(RF_SendData.Acc.Y);
      SendBuf[7]  = (u8)Byte8L(RF_SendData.Acc.Y);
      SendBuf[8]  = (u8)Byte8H(RF_SendData.Acc.Z);
      SendBuf[9]  = (u8)Byte8L(RF_SendData.Acc.Z);
      SendBuf[10] = (u8)Byte8H(RF_SendData.Gyr.X);
      SendBuf[11] = (u8)Byte8L(RF_SendData.Gyr.X);
      SendBuf[12] = (u8)Byte8H(RF_SendData.Gyr.Y);
      SendBuf[13] = (u8)Byte8L(RF_SendData.Gyr.Y);
      SendBuf[14] = (u8)Byte8H(RF_SendData.Gyr.Z);
      SendBuf[15] = (u8)Byte8L(RF_SendData.Gyr.Z);
      SendBuf[16] = (u8)Byte8H(RF_SendData.Mag.X);
      SendBuf[17] = (u8)Byte8L(RF_SendData.Mag.X);
      SendBuf[18] = (u8)Byte8H(RF_SendData.Mag.Y);
      SendBuf[19] = (u8)Byte8L(RF_SendData.Mag.Y);
      SendBuf[20] = (u8)Byte8H(RF_SendData.Mag.Z);
      SendBuf[21] = (u8)Byte8L(RF_SendData.Mag.Z);
      SendBuf[22] = (u8)Byte8H(RF_SendData.Baro.Temp);
      SendBuf[23] = (u8)Byte8L(RF_SendData.Baro.Temp);
      SendBuf[24] = (u8)(RF_SendData.Baro.Press>>16);
      SendBuf[25] = (u8)(RF_SendData.Baro.Press>>8);
      SendBuf[26] = (u8)(RF_SendData.Baro.Press);
      SendBuf[27] = (u8)(RF_SendData.Baro.Height>>16);
      SendBuf[28] = (u8)(RF_SendData.Baro.Height>>8);
      SendBuf[29] = (u8)(RF_SendData.Baro.Height);
      SendBuf[30] = (u8)(0);
      SendBuf[31] = (u8)(0);
      break;

    case 0x03:

      RF_SendData.Ang.X = (s16)(AngE.Pitch*100);  // 10 mdeg/LSB
      RF_SendData.Ang.Y = (s16)(AngE.Roll*100);   // 10 mdeg/LSB
      RF_SendData.Ang.Z = (s16)(AngE.Yaw*10);     // 100 mdeg/LSB

      SendBuf[4]  = (u8)Byte8H(RF_SendData.Ang.X);
      SendBuf[5]  = (u8)Byte8L(RF_SendData.Ang.X);
      SendBuf[6]  = (u8)Byte8H(RF_SendData.Ang.Y);
      SendBuf[7]  = (u8)Byte8L(RF_SendData.Ang.Y);
      SendBuf[8]  = (u8)Byte8H(RF_SendData.Ang.Z);
      SendBuf[9]  = (u8)Byte8L(RF_SendData.Ang.Z);
      SendBuf[10] = (u8)Byte8H(RF_SendData.Vel.X);
      SendBuf[11] = (u8)Byte8L(RF_SendData.Vel.X);
      SendBuf[12] = (u8)Byte8H(RF_SendData.Vel.Y);
      SendBuf[13] = (u8)Byte8L(RF_SendData.Vel.Y);
      SendBuf[14] = (u8)Byte8H(RF_SendData.Vel.Z);
      SendBuf[15] = (u8)Byte8L(RF_SendData.Vel.Z);
      SendBuf[16] = (u8)Byte8H(RF_SendData.Pos.X);
      SendBuf[17] = (u8)Byte8L(RF_SendData.Pos.X);
      SendBuf[18] = (u8)Byte8H(RF_SendData.Pos.Y);
      SendBuf[19] = (u8)Byte8L(RF_SendData.Pos.Y);
      SendBuf[20] = (u8)Byte8H(RF_SendData.Pos.Z);
      SendBuf[21] = (u8)Byte8L(RF_SendData.Pos.Z);
      SendBuf[22] = (u8)(RF_SendData.Baro.Press>>16);
      SendBuf[23] = (u8)(RF_SendData.Baro.Press>>8);
      SendBuf[24] = (u8)(RF_SendData.Baro.Press);
      SendBuf[25] = (u8)(RF_SendData.Baro.Height>>16);
      SendBuf[26] = (u8)(RF_SendData.Baro.Height>>8);
      SendBuf[27] = (u8)(RF_SendData.Baro.Height);
      SendBuf[28] = (u8)(0);
      SendBuf[29] = (u8)(0);
      SendBuf[30] = (u8)(0);
      SendBuf[31] = (u8)(0);
      break;

    default:
      // ERROR
      break;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
