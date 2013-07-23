/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "algorithm_kalman.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define Kalman_dt 0.0025

const float Kalman_Q  = 0.002;
const float Kalman_R  = 0.001;
const float Kalman_B[2] = {Kalman_dt, 0};
const float Kalman_H[2] = {1, 0};
const float Kalman_A[2][2] = {{1, -Kalman_dt}, {0, 1}};
/*=====================================================================================================*/
/*=====================================================================================================*/
float xKalmanFilter( float Angle, float Angular )
{
	float Kalman_K[2] = {0};
	float Kalman_Y = 0;
	float Kalman_S = 0;
	static float Kalman_X[2] = {0};
	static float Kalman_P[2][2] = {{1, 1}, {1, 1}};

	// StateUpdate
	// X = AX + BU
	Kalman_X[0] += (double)(Angular - Kalman_X[1])*Kalman_dt;
	// P = APA' + Q
	Kalman_P[0][0] += (double)(Kalman_Q - Kalman_P[0][1] - Kalman_P[1][0]) * Kalman_dt;
	Kalman_P[0][1] += (double)(- Kalman_P[1][1]) * Kalman_dt;
	Kalman_P[1][0] += (double)(- Kalman_P[1][1]) * Kalman_dt;
	Kalman_P[1][1] += (double)Kalman_Q * Kalman_dt;

	// KalmanUpdate
	// Y = Z - HX = Z - X1
	Kalman_Y = Angle - Kalman_X[0];
	// S = HPH' + R
	Kalman_S = Kalman_P[1][1] + Kalman_R;
	// K = PH'/S
	Kalman_K[0] = Kalman_P[0][0] / Kalman_S;
	Kalman_K[1] = Kalman_P[1][0] / Kalman_S;
	// X = X + KY
	Kalman_X[0] += Kalman_K[0]*Kalman_Y;
	Kalman_X[1] += Kalman_K[1]*Kalman_Y;
	// P = (I - KH)P = P - KHP
	Kalman_P[0][0] -= Kalman_K[0]*Kalman_P[0][0];
	Kalman_P[0][1] -= Kalman_K[0]*Kalman_P[0][1];
	Kalman_P[1][0] -= Kalman_K[1]*Kalman_P[0][0];
	Kalman_P[1][1] -= Kalman_K[1]*Kalman_P[0][1];

	return Kalman_X[0];
}

float yKalmanFilter( float Angle, float Angular )
{
	float Kalman_K[2] = {0};
	float Kalman_Y = 0;
	float Kalman_S = 0;
	static float Kalman_X[2] = {0};
	static float Kalman_P[2][2] = {{1, 1}, {1, 1}};

	// StateUpdate
	// X = AX + BU
	Kalman_X[0] += (double)(Angular - Kalman_X[1])*Kalman_dt;
	// P = APA' + Q
	Kalman_P[0][0] += (double)(Kalman_Q - Kalman_P[0][1] - Kalman_P[1][0]) * Kalman_dt;
	Kalman_P[0][1] += (double)(- Kalman_P[1][1]) * Kalman_dt;
	Kalman_P[1][0] += (double)(- Kalman_P[1][1]) * Kalman_dt;
	Kalman_P[1][1] += (double)Kalman_Q * Kalman_dt;

	// KalmanUpdate
	// Y = Z - HX = Z - X1
	Kalman_Y = Angle - Kalman_X[0];
	// S = HPH' + R
	Kalman_S = Kalman_P[1][1] + Kalman_R;
	// K = PH'/S
	Kalman_K[0] = Kalman_P[0][0] / Kalman_S;
	Kalman_K[1] = Kalman_P[1][0] / Kalman_S;
	// X = X + KY
	Kalman_X[0] += Kalman_K[0]*Kalman_Y;
	Kalman_X[1] += Kalman_K[1]*Kalman_Y;
	// P = (I - KH)P = P - KHP
	Kalman_P[0][0] -= Kalman_K[0]*Kalman_P[0][0];
	Kalman_P[0][1] -= Kalman_K[0]*Kalman_P[0][1];
	Kalman_P[1][0] -= Kalman_K[1]*Kalman_P[0][0];
	Kalman_P[1][1] -= Kalman_K[1]*Kalman_P[0][1];

	return Kalman_X[0];
}

float zKalmanFilter( float Angle, float Angular )
{
	float Kalman_K[2] = {0};
	float Kalman_Y = 0;
	float Kalman_S = 0;
	static float Kalman_X[2] = {0};
	static float Kalman_P[2][2] = {{1, 1}, {1, 1}};

	// StateUpdate
	// X = AX + BU
	Kalman_X[0] += (double)(Angular - Kalman_X[1])*Kalman_dt;
	// P = APA' + Q
	Kalman_P[0][0] += (double)(Kalman_Q - Kalman_P[0][1] - Kalman_P[1][0]) * Kalman_dt;
	Kalman_P[0][1] += (double)(- Kalman_P[1][1]) * Kalman_dt;
	Kalman_P[1][0] += (double)(- Kalman_P[1][1]) * Kalman_dt;
	Kalman_P[1][1] += (double)Kalman_Q * Kalman_dt;

	// KalmanUpdate
	// Y = Z - HX = Z - X1
	Kalman_Y = Angle - Kalman_X[0];
	// S = HPH' + R
	Kalman_S = Kalman_P[1][1] + Kalman_R;
	// K = PH'/S
	Kalman_K[0] = Kalman_P[0][0] / Kalman_S;
	Kalman_K[1] = Kalman_P[1][0] / Kalman_S;
	// X = X + KY
	Kalman_X[0] += Kalman_K[0]*Kalman_Y;
	Kalman_X[1] += Kalman_K[1]*Kalman_Y;
	// P = (I - KH)P = P - KHP
	Kalman_P[0][0] -= Kalman_K[0]*Kalman_P[0][0];
	Kalman_P[0][1] -= Kalman_K[0]*Kalman_P[0][1];
	Kalman_P[1][0] -= Kalman_K[1]*Kalman_P[0][0];
	Kalman_P[1][1] -= Kalman_K[1]*Kalman_P[0][1];

	return Kalman_X[0];
}
/*=====================================================================================================*/
/*=====================================================================================================*/
// float KalmanFilter( float Angle, float Angular )
// {
// 	float Kalman_K[2] = {0};
// 	float Kalman_Y = 0;
// 	float Kalman_S = 0;
// 	static float Kalman_X[2] = {0};
// 	static float Kalman_P[2][2] = {{1, 1}, {1, 1}};

// 	// StateUpdate
//	// X = AX + BU
// 	Kalman_X[0] += (double)( Angular - Kalman_X[1] )*Kalman_dt;
//	// P = APA' + Q
//	//	Kalman_P[0][0] = Kalman_A[0][0]*Kalman_X[0] + Kalman_A[0][1]*Kalman_X[1] + Kalman_B[0]*Angular;
//	//	Kalman_P[0][1] = Kalman_A[1][0]*Kalman_X[0] + Kalman_A[1][1]*Kalman_X[1] + Kalman_B[0]*Angular;
//	//	Kalman_P[1][0] = Kalman_A[0][0]*Kalman_X[0] + Kalman_A[0][1]*Kalman_X[1] + Kalman_B[0]*Angular;
//	//	Kalman_P[1][1] = Kalman_A[1][0]*Kalman_X[0] + Kalman_A[1][1]*Kalman_X[1] + Kalman_B[0]*Angular;

// 	Kalman_P[0][0] += (double)(Kalman_Q -Kalman_P[0][1] - Kalman_P[1][0]) * Kalman_dt;
// 	Kalman_P[0][1] += (double)(- Kalman_P[1][1]) * Kalman_dt;
// 	Kalman_P[1][0] += (double)(- Kalman_P[1][1]) * Kalman_dt;
// 	Kalman_P[1][1] += (double)(Kalman_Q) * Kalman_dt;

// 	// KalmanUpdate
// 	// Y = Z - HX = Z - X1
// 	Kalman_Y = Angle - Kalman_X[0];
// 	// S = HPH' + R
// 	Kalman_S = Kalman_P[1][1] + Kalman_R;
// 	// K = PH'/S
// 	Kalman_K[0] = Kalman_P[0][0] / Kalman_S;
// 	Kalman_K[1] = Kalman_P[1][0] / Kalman_S;
// 	// X = X + KY
// 	Kalman_X[0] += Kalman_K[0]*Kalman_Y;
// 	Kalman_X[1] += Kalman_K[1]*Kalman_Y;
// 	// P = (I - KH)P = P - KHP
// 	Kalman_P[0][0] -= Kalman_K[0]*Kalman_P[0][0];
// 	Kalman_P[0][1] -= Kalman_K[0]*Kalman_P[0][1];
// 	Kalman_P[1][0] -= Kalman_K[1]*Kalman_P[0][0];
// 	Kalman_P[1][1] -= Kalman_K[1]*Kalman_P[0][1];

// 	return Kalman_X[0];
// }
