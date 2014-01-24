/* #include "algorithm_quaternion.h" */

#ifndef __ALGORITHM_QUATERNION_H
#define __ALGORITHM_QUATERNION_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
  float Pitch;
  float Roll;
  float Yaw;
} EulerAngle;

typedef struct {
  float q0;
  float q1;
  float q2;
  float q3;
} Quaternion;
/*=====================================================================================================*/
/*=====================================================================================================*/
void Quaternion_ToNumQ( Quaternion *pNumQ, EulerAngle *pAngE );
void Quaternion_ToAngE( Quaternion *pNumQ, EulerAngle *pAngE );
Quaternion Quaternion_Multiply( Quaternion NowQ, Quaternion OldQ );
void Quaternion_Normalize( Quaternion *pNumQ );
void Quaternion_RungeKutta( Quaternion *pNumQ, float GyrX, float GyrY, float GyrZ, float helfTimes );
/*=====================================================================================================*/
/*=====================================================================================================*/
extern Quaternion NumQ;
extern EulerAngle AngE;
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
