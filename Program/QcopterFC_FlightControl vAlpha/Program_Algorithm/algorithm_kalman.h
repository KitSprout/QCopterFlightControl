/* #include "algorithm_kalman.h" */

#ifndef __ALGORITHM_KALMAN_H
#define __ALGORITHM_KALMAN_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
// float KalmanFilter( const float Angle, const float Angular );

float xKalmanFilter( const float Angle, const float Angular );
float yKalmanFilter( const float Angle, const float Angular );
float zKalmanFilter( const float Angle, const float Angular );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif	 
