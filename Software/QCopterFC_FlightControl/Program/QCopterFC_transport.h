/* #include "QCopterNano_transport.h" */

#ifndef __QCOPTERNANO_TRANSPORT_H
#define __QCOPTERNANO_TRANSPORT_H

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

#define EXP_THR_B (RF_RecvData.Ctrl.ThrB)
#define EXP_THR_S (RF_RecvData.Ctrl.ThrS)
#define EXP_PITCH (RF_RecvData.Ctrl.Pitch)
#define EXP_ROLL  (RF_RecvData.Ctrl.Roll)
#define EXP_YAW   (RF_RecvData.Ctrl.Yaw)
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef __IO struct {
  u8 Packet;                  // Byt[00:00] Packet *****
  struct {                    // Byt[01:03] Time *******
    u8 Min;                   //    [01:01] Time_Min   
    u8 Sec;                   //    [02:02] Time_Sec  
    u8 Cnt;                   //    [03:03] Time_Cnt  
  } Time;
  struct {                    // Byt[04:06] KeyR *******
    u8 KeyRead;               //    [04:04] KeyRP       Bit[7:1] PressTime    Bit[0:0] KeyRead
    u8 PressTime;             //    [05:05] KeyRR       Bit[7:1] PressTime    Bit[0:0] KeyRead
  } KeyR[3];                  //    [06:06] KeyRL       Bit[7:1] PressTime    Bit[0:0] KeyRead
  struct {                    // Byt[07:09] KeyL *******
    u8 KeyRead;               //    [07:07] KeyLP       Bit[7:1] PressTime    Bit[0:0] KeyRead
    u8 PressTime;             //    [08:08] KeyLR       Bit[7:1] PressTime    Bit[0:0] KeyRead
  } KeyL[3];                  //    [09:09] KeyLL       Bit[7:1] PressTime    Bit[0:0] KeyRead
  struct {                    // Byt[10:12] KeyP *******
    u8 KeyRead;               //    [10:10] KeyPP       Bit[7:1] PressTime    Bit[0:0] KeyRead
    u8 PressTime;             //    [11:11] KeyPR       Bit[7:1] PressTime    Bit[0:0] KeyRead
  } KeyP[3];                  //    [12:12] KeyPL       Bit[7:1] PressTime    Bit[0:0] KeyRead
  struct {                    // Byt[13:15] KeyS *******
    u8 KeyRead;               //    [13:13] KeyS1       Bit[7:1] PressTime    Bit[0:0] KeyRead
    u8 PressTime;             //    [14:14] KeyS2       Bit[7:1] PressTime    Bit[0:0] KeyRead
  } KeyS[3];                  //    [15:15] KeyS3       Bit[7:1] PressTime    Bit[0:0] KeyRead
  struct {                    // Byt[16:27] Ctrl *******
    s16 ThrB;                 //    [16:17] ThrB        [16] ThrB_H     [17] ThrB_L
    s16 ThrS;                 //    [18:19] ThrS        [18] ThrS_H     [19] ThrS_L
    s16 Pitch;                //    [20:21] Pitch       [20] Pitch_H    [21] Pitch_L
    s16 Roll;                 //    [22:23] Roll        [22] Roll_H     [23] Roll_L
    s16 Yaw;                  //    [24:25] Yaw         [24] Yaw_H      [25] Yaw_L
    s16 Corr;                 //    [26:27] Corr        [26] Corr_H     [27] Corr_L
  } Ctrl;   
  u8                 Cmd;     // Byt[28:28] Cmd ********
  u8                 Data[3]; // Byt[29:31] Data *******
} TR_RECV_DATA;
/*=====================================================================================================*/
/*=====================================================================================================*/
void Transport_Recv( u8* RecvBuf );
void Transport_Send( u8* SendBuf );
/*=====================================================================================================*/
/*=====================================================================================================*/
extern TR_RECV_DATA RF_RecvData;
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
