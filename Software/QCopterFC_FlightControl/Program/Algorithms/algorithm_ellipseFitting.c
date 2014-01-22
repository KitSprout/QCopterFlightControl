/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "algorithm_ellipseFitting.h"
#include "algorithm_mathUnit.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
//#define USE_ARM_MATH_LIB
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : EllipseFitting
**功能 : Ellipse Fitting
**輸入 : Ans, *MagDataX, *MagDataY, Sample
**輸出 : None
**使用 : EllipseFitting(Ellipse, MagDataX, MagDataY, 8);
**=====================================================================================================*/
/*=====================================================================================================*/
void EllipseFitting( float *Ellipse, s16 *MagDataX, s16 *MagDataY, u8 Sample )
{
  s8 i, j, k;
  float temp, temp1, temp2;
  float tempArrX[36] = {0};
  float tempArrY[36] = {0};

  float MAG_X1Y0 = 0.0f;
  float MAG_X2Y0 = 0.0f;
  float MAG_X3Y0 = 0.0f;
  float MAG_X0Y1 = 0.0f;
  float MAG_X0Y2 = 0.0f;
  float MAG_X0Y3 = 0.0f;
  float MAG_X0Y4 = 0.0f;
  float MAG_X1Y1 = 0.0f;
  float MAG_X2Y1 = 0.0f;
  float MAG_X1Y2 = 0.0f;
  float MAG_X1Y3 = 0.0f;
  float MAG_X2Y2 = 0.0f;
  float MAG_X3Y1 = 0.0f;

  float MagArr[5][6] = {0};

  for(i=0; i<Sample; i++) {
    tempArrX[i] = (float)MagDataX[i]/1000.0f;
    tempArrY[i] = (float)MagDataY[i]/1000.0f;
  }

  for(i=0; i<Sample; i++) {
    MAG_X1Y0 += tempArrX[i];
    MAG_X2Y0 += tempArrX[i]*tempArrX[i];
    MAG_X3Y0 += tempArrX[i]*tempArrX[i]*tempArrX[i];
    MAG_X0Y1 += tempArrY[i];
    MAG_X0Y2 += tempArrY[i]*tempArrY[i];
    MAG_X0Y3 += tempArrY[i]*tempArrY[i]*tempArrY[i];
    MAG_X0Y4 += tempArrY[i]*tempArrY[i]*tempArrY[i]*tempArrY[i];
    MAG_X1Y1 += tempArrX[i]*tempArrY[i];
    MAG_X2Y1 += tempArrX[i]*tempArrX[i]*tempArrY[i];
    MAG_X1Y2 += tempArrX[i]*tempArrY[i]*tempArrY[i];
    MAG_X1Y3 += tempArrX[i]*tempArrY[i]*tempArrY[i]*tempArrY[i];
    MAG_X2Y2 += tempArrX[i]*tempArrX[i]*tempArrY[i]*tempArrY[i];
    MAG_X3Y1 += tempArrX[i]*tempArrX[i]*tempArrX[i]*tempArrY[i];
  }

  MagArr[0][0] = MAG_X2Y2;
  MagArr[0][1] = MAG_X1Y3;
  MagArr[0][2] = MAG_X2Y1;
  MagArr[0][3] = MAG_X1Y2;
  MagArr[0][4] = MAG_X1Y1;
  MagArr[0][5] = -MAG_X3Y1;

  MagArr[1][0] = MAG_X1Y3;
  MagArr[1][1] = MAG_X0Y4;
  MagArr[1][2] = MAG_X1Y2;
  MagArr[1][3] = MAG_X0Y3;
  MagArr[1][4] = MAG_X0Y2;
  MagArr[1][5] = -MAG_X2Y2;

  MagArr[2][0] = MAG_X2Y1;
  MagArr[2][1] = MAG_X1Y2;
  MagArr[2][2] = MAG_X2Y0;
  MagArr[2][3] = MAG_X1Y1;
  MagArr[2][4] = MAG_X1Y0;
  MagArr[2][5] = -MAG_X3Y0;

  MagArr[3][0] = MAG_X1Y2;
  MagArr[3][1] = MAG_X0Y3;
  MagArr[3][2] = MAG_X1Y1;
  MagArr[3][3] = MAG_X0Y2;
  MagArr[3][4] = MAG_X0Y1;
  MagArr[3][5] = -MAG_X2Y1;

  MagArr[4][0] = MAG_X1Y1;
  MagArr[4][1] = MAG_X0Y2;
  MagArr[4][2] = MAG_X1Y0;
  MagArr[4][3] = MAG_X0Y1;
  MagArr[4][4] = Sample;
  MagArr[4][5] = -MAG_X2Y0;

  for(i=0; i<5; i++)
    for(j=i+1; j<6; j++)
      for(k=5; k>i-1; k--)
        MagArr[j][k] = MagArr[j][k] - MagArr[j][i]/MagArr[i][i]*MagArr[i][k];
  for(i=0; i<5; i++) {
    temp = MagArr[i][i];
    for(j=i; j<6; j++)
      MagArr[i][j] = MagArr[i][j]/temp;
  }
  for(j=4; j>0; j--)
    for(i=0; i<j; i++)
      MagArr[i][5] = MagArr[i][5] - MagArr[i][j]*MagArr[j][5];

  temp = (1.0f-MagArr[1][5])/MagArr[0][5];
  temp1 = temp + sqrtf(temp*temp+1.0f);
  Ellipse[0] = atanf(temp1);	// Theta

  temp = MagArr[0][5]*MagArr[0][5]-4*MagArr[1][5];
  Ellipse[1] = (2.0f*MagArr[1][5]*MagArr[2][5]-MagArr[0][5]*MagArr[3][5])/temp;	// X0
  Ellipse[2] = (2.0f*MagArr[3][5]-MagArr[0][5]*MagArr[2][5])/temp;	            // Y0
#ifndef USE_ARM_MATH_LIB
  temp = cosf(Ellipse[0]);
#else
  temp = arm_cos_f32(Ellipse[0]);
#endif
  temp2 = (Ellipse[1]*Ellipse[1]+MagArr[0][5]*Ellipse[1]*Ellipse[2]+MagArr[1][5]*Ellipse[2]*Ellipse[2]-MagArr[4][5])*(temp1*temp1*temp1*temp1-1.0f);
  Ellipse[3] = temp/sqrtf((MagArr[1][5]*temp1*temp1-1)/temp2);	// a
  Ellipse[4] = temp/sqrtf((temp1*temp1-MagArr[1][5])/temp2);    // b

  Ellipse[1] = Ellipse[1]*1000.0f;
  Ellipse[2] = Ellipse[2]*1000.0f;
  Ellipse[3] = Ellipse[3]*1000.0f;
  Ellipse[4] = Ellipse[4]*1000.0f;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
