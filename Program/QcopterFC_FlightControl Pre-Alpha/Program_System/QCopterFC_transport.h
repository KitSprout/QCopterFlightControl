/* #include "QCopterFC_transport.h" */

#ifndef __QCOPTERFC_TRANSPORT_H
#define __QCOPTERFC_TRANSPORT_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KeyNums			32

#define KEYR_J      RecvData[0]
#define KEYR_U      RecvData[1]
#define KEYR_D      RecvData[2]
#define KEYR_L      RecvData[3]
#define KEYR_R      RecvData[4]
#define KEYR_S1     RecvData[5]
#define KEYR_S2     RecvData[6]
#define KEYL_J      RecvData[7]
#define KEYL_U      RecvData[8]
#define KEYL_D      RecvData[9]
#define KEYL_L      RecvData[10]
#define KEYL_R      RecvData[11]
#define KEYL_S1     RecvData[12]
#define KEYL_S2     RecvData[13]
#define JSR_X       RecvData[14]
#define JSR_Y       RecvData[15]
#define JSR_Z       RecvData[16]
#define JSL_X       RecvData[17]
#define JSL_Y       RecvData[18]
#define JSL_Z       RecvData[19]

#define Exp_Pitch   RecvData[20]
#define Exp_Roll    RecvData[21]
#define Exp_Yaw     RecvData[22]
#define Exp_Thr     RecvData[23]
/*====================================================================================================*/
/*====================================================================================================*/
void Transport_Recv( u8* RecvBuf );
void Transport_Send( u8* SendBuf );
/*====================================================================================================*/
/*====================================================================================================*/
extern vu16 RecvData[KeyNums];
/*====================================================================================================*/
/*====================================================================================================*/
#endif
