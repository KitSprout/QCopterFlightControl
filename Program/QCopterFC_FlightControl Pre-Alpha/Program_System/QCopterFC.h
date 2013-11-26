/* #include "QCopterFC.h" */

#ifndef __QCOPTERFC_H
#define __QCOPTERFC_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef __IO enum {
  FSM_Tx,
  FSM_Rx,
  FSM_CTRL,
  FSM_UART,
  FSM_DATA
} FSM_Mode;

typedef __IO enum {
  Mode_GyrCorrect,
  Mode_AccCorrect,  // 僅在水平狀態下做校正
  Mode_MagCorrect,
  Mode_Quaternion,
  Mode_Algorithm
} Sensor_Mode;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern vu8 Time_Sec;
extern vu8 Time_Min;
extern vu8 RecvTime_Sec;
extern vu8 RecvTime_Min;
extern vu16 SysTick_Cnt;

extern Sensor_Mode SensorMode;
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif	
