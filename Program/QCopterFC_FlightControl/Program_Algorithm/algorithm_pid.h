/* #include "algorithm_pid.h" */

#ifndef __ALGORITHM_PID_H
#define __ALGORITHM_PID_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
  float Kp;
  float Ki;
  float Kd;
  float Err0;
  float Err1;
  float Err2;
  float SumErr;
  float ZeroErr;
  float Output;
  float KiMax;
  float KiMin;
  float OutMax;
  float OutMin;
} PID_Struct;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern PID_Struct PID_Yaw;
extern PID_Struct PID_Roll;
extern PID_Struct PID_Pitch;
/*=====================================================================================================*/
/*=====================================================================================================*/
void PID_Init( PID_Struct *PID );
float PID_IncCal( PID_Struct*, float );
float PID_PosCal( PID_Struct*, float );
float PID_AHRS_Cal( PID_Struct*, float, float );
float PID_AHRS_CalYaw( PID_Struct *PID, float Angle, float Gyroscope );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif	 
