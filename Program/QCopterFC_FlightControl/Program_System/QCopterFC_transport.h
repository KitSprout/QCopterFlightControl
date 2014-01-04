/* #include "QCopterFC_transport.h" */

#ifndef __QCOPTERFC_TRANSPORT_H
#define __QCOPTERFC_TRANSPORT_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KEY_RP    (RF_RecvData.KeyR[0].KeyRead)
#define KEY_RR    (RF_RecvData.KeyR[1].KeyRead)
#define KEY_RL    (RF_RecvData.KeyR[2].KeyRead)
#define KEY_LP    (RF_RecvData.KeyL[0].KeyRead)
#define KEY_LR    (RF_RecvData.KeyL[1].KeyRead)
#define KEY_LL    (RF_RecvData.KeyL[2].KeyRead)
#define KEY_PP    (RF_RecvData.KeyP[0].KeyRead)
#define KEY_PR    (RF_RecvData.KeyP[1].KeyRead)
#define KEY_PL    (RF_RecvData.KeyP[2].KeyRead)
#define KEY_S1    (RF_RecvData.KeyS[0].KeyRead)
#define KEY_S2    (RF_RecvData.KeyS[1].KeyRead)
#define KEY_S3    (RF_RecvData.KeyS[2].KeyRead)

#define KEY_RP_T  (RF_RecvData.KeyR[0].PressTime)
#define KEY_RR_T  (RF_RecvData.KeyR[1].PressTime)
#define KEY_RL_T  (RF_RecvData.KeyR[2].PressTime)
#define KEY_LP_T  (RF_RecvData.KeyL[0].PressTime)
#define KEY_LR_T  (RF_RecvData.KeyL[1].PressTime)
#define KEY_LL_T  (RF_RecvData.KeyL[2].PressTime)
#define KEY_PP_T  (RF_RecvData.KeyP[0].PressTime)
#define KEY_PR_T  (RF_RecvData.KeyP[1].PressTime)
#define KEY_PL_T  (RF_RecvData.KeyP[2].PressTime)
#define KEY_S1_T  (RF_RecvData.KeyS[0].PressTime)
#define KEY_S2_T  (RF_RecvData.KeyS[1].PressTime)
#define KEY_S3_T  (RF_RecvData.KeyS[2].PressTime)

#define Exp_ThrB  (RF_RecvData.Ctrl.ThrB)
#define Exp_ThrS  (RF_RecvData.Ctrl.ThrS)
#define Exp_Pitch (RF_RecvData.Ctrl.Pitch)
#define Exp_Roll  (RF_RecvData.Ctrl.Roll)
#define Exp_Yaw   (RF_RecvData.Ctrl.Yaw)
/*====================================================================================================*/
/*====================================================================================================*/
typedef struct {
  u8 Min;
  u8 Sec;
  u8 mSec;
} TRSTRUCT_TIME;
typedef struct {
  u16 KP;
  u16 KI;
  u16 KD;
} TRSTRUCT_PID;
typedef struct {
  u16 CH1;
  u16 CH2;
  u16 CH3;
  u16 CH4;
  u16 CH5;
  u16 CH6;
} TRSTRUCT_THROTTLE;
typedef struct {
  u8 Vol;
  u8 Cur;
  u16 Cap;
} TRSTRUCT_BATTERY;
typedef struct {
  s16 X;
  s16 Y;
  s16 Z;
} TRSTRUCT_SENSOR;
typedef struct {
  u16 Temp;
  u32 Press;
  u32 Height;
} TRSTRUCT_BAROMETER;
typedef struct {
  s16 X;
  s16 Y;
  s16 Z;
} TRSTRUCT_MOTION;
typedef struct {
  u32 Lon;
  u32 Lat;
} TRSTRUCT_GPS;
typedef struct {
  u8 KeyRead;
  u8 PressTime;
} TRSTRUCT_KEY;
typedef struct {
  s16 ThrB;
  s16 ThrS;
  s16 Pitch;
  s16 Roll;
  s16 Yaw;
  s16 Corr;
} TRSTRUCT_CTRL;
typedef __IO struct {
  u8 Packet;                  // Byt[00:00] Packet *****
  TRSTRUCT_TIME      Time;    // Byt[01:03] Time *******
                              //    [01:01] Time_Min    
                              //    [02:02] Time_Sec    
                              //    [03:03] Time_mSec   

  // Packet 1
  TRSTRUCT_PID       PID;     // Byt[04:09] PID ********
                              //    [04:05] PID_KP      [04] PID_KP_H   [05] PID_KP_L
                              //    [06:07] PID_KI      [06] PID_KI_H   [07] PID_KI_L
                              //    [08:09] PID_KD      [08] PID_KD_H   [09] PID_KD_L
  TRSTRUCT_THROTTLE  Thr;     // Byt[10:21] Thr_CH *****
                              //    [10:11] Thr_CH1     [04] Thr_CH1_H  [05] Thr_CH1_L
                              //    [12:13] Thr_CH2     [06] Thr_CH2_H  [07] Thr_CH2_L
                              //    [14:15] Thr_CH3     [08] Thr_CH3_H  [09] Thr_CH3_L
                              //    [16:17] Thr_CH4     [06] Thr_CH4_H  [07] Thr_CH4_L
                              //    [18:19] Thr_CH5     [08] Thr_CH5_H  [09] Thr_CH5_L
                              //    [20:21] Thr_CH6     [06] Thr_CH6_H  [07] Thr_CH6_L
  TRSTRUCT_BATTERY   Batery;  // Byt[22:21] Batery *****
                              //    [22:22] Bat_Vol
                              //    [23:23] Bat_Cur
                              //    [24:25] Bat_Cap     [08] Thr_CH3_H  [09] Thr_CH3_L
                              // Byt[26:31] Reserved ***

  // Packet 2
  TRSTRUCT_SENSOR    Acc;     // Byt[04:09] Acc ********
                              //    [04:05] Acc_X       [04] Acc_X_H    [05] Acc_X_L
                              //    [06:07] Acc_Y       [06] Acc_Y_H    [07] Acc_Y_L
                              //    [08:09] Acc_Z       [08] Acc_Z_H    [09] Acc_Z_L
  TRSTRUCT_SENSOR    Gyr;     // Byt[10:15] Gyr ********
                              //    [10:11] Gyr_X       [10] Gyr_X_H    [11] Gyr_X_L
                              //    [12:13] Gyr_Y       [12] Gyr_Y_H    [13] Gyr_Y_L
                              //    [14:15] Gyr_Z       [14] Gyr_Z_H    [15] Gyr_Z_L
  TRSTRUCT_SENSOR    Mag;     // Byt[16:21] Mag ********
                              //    [16:17] Mag_X       [16] Mag_X_H    [17] Mag_X_L
                              //    [18:19] Mag_Y       [18] Mag_Y_H    [19] Mag_Y_L
                              //    [20:21] Mag_Z       [20] Mag_Z_H    [21] Mag_Z_L
  TRSTRUCT_BAROMETER Baro;    // Byt[22:29] Baro *******
                              //    [22:23] Temp        [22] Temp_H     [23] Temp_L
                              //    [24:26] Press       [24] Press_HH   [25] Press_H    [26] Press_L
                              //    [27:29] Height      [27] Height_HH  [28] Height_H   [29] Height_L
                              // Byt[30:31] Reserved ***

  // Packet 3
  TRSTRUCT_MOTION    Ang;     // Byt[04:09] Ang ********
                              //    [04:05] Ang_X       [04] Ang_X_H    [05] Ang_X_L
                              //    [06:07] Ang_Y       [06] Ang_Y_H    [07] Ang_Y_L
                              //    [08:09] Ang_Z       [08] Ang_Z_H    [09] Ang_Z_L
  TRSTRUCT_MOTION    Vel;     // Byt[10:15] Vel ********
                              //    [10:11] Vel_X       [10] Vel_X_H    [11] Vel_X_L
                              //    [12:13] Vel_Y       [12] Vel_Y_H    [13] Vel_Y_L
                              //    [14:15] Vel_Z       [14] Vel_Z_H    [15] Vel_Z_L
  TRSTRUCT_MOTION    Pos;     // Byt[16:21] Pos ********
                              //    [16:17] Pos_X       [16] Pos_X_H    [17] Pos_X_L
                              //    [18:19] Pos_Y       [18] Pos_Y_H    [19] Pos_Y_L
                              //    [20:21] Pos_Z       [20] Pos_Z_H    [21] Pos_Z_L
  TRSTRUCT_GPS       GPS;     // Byt[22:27] GPS ********
                              //    [22:24] GPS_Lon     [22] Lon_HH     [23] Lon_H      [24] Lon_L
                              //    [25:27] GPS_Lat     [25] Lat_HH     [26] Lat_H      [27] Lat_L
                              // Byt[28:31] Reserved ***
} TR_SEND_DATA;

typedef __IO struct {
  u8 Packet;                  // Byt[00:00] Packet *****
  TRSTRUCT_TIME      Time;    // Byt[01:03] Time *******
                              //    [01:01] Time_Min    
                              //    [02:02] Time_Sec    
                              //    [03:03] Time_mSec   
  TRSTRUCT_KEY       KeyR[3]; // Byt[04:06] KeyR *******
                              //    [04:04] KeyRP       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [05:05] KeyRR       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [06:06] KeyRL       Bit[7:1] PressTime    Bit[0:0] KeyRead
  TRSTRUCT_KEY       KeyL[3]; // Byt[07:09] KeyL *******
                              //    [07:07] KeyLP       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [08:08] KeyLR       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [09:09] KeyLL       Bit[7:1] PressTime    Bit[0:0] KeyRead
  TRSTRUCT_KEY       KeyP[3]; // Byt[10:12] KeyP *******
                              //    [10:10] KeyPP       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [11:11] KeyPR       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [12:12] KeyPL       Bit[7:1] PressTime    Bit[0:0] KeyRead
  TRSTRUCT_KEY       KeyS[3]; // Byt[13:15] KeyS *******
                              //    [13:13] KeyS1       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [14:14] KeyS2       Bit[7:1] PressTime    Bit[0:0] KeyRead
                              //    [15:15] KeyS3       Bit[7:1] PressTime    Bit[0:0] KeyRead
  TRSTRUCT_CTRL      Ctrl;    // Byt[16:27] Ctrl *******
                              //    [16:17] ThrB        [16] ThrB_H     [17] ThrB_L
                              //    [18:19] ThrS        [18] ThrS_H     [19] ThrS_L
                              //    [20:21] Pitch       [20] Pitch_H    [21] Pitch_L
                              //    [22:23] Roll        [22] Roll_H     [23] Roll_L
                              //    [24:25] Yaw         [24] Yaw_H      [25] Yaw_L
                              //    [26:27] Corr        [26] Corr_H     [27] Corr_L
  u8                 Cmd;     // Byt[28:28] Cmd ********
  u8                 Data[3]; // Byt[29:31] Data *******
} TR_RECV_DATA;
/*====================================================================================================*/
/*====================================================================================================*/
extern TR_RECV_DATA RF_RecvData;
extern TR_SEND_DATA RF_SendData;
/*====================================================================================================*/
/*====================================================================================================*/
void Transport_Recv( u8* RecvBuf );
void Transport_Send( u8* SendBuf );
/*====================================================================================================*/
/*====================================================================================================*/
#endif
