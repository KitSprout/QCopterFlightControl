/* #include "module_sensor.h" */

#ifndef __MODULE_SENSOR_H
#define __MODULE_SENSOR_H

#include "stm32f4xx.h"
#include "module_mpu9150.h"
#include "module_ms5611.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define ACC_X_OFFSET ((s16)0)
#define ACC_Y_OFFSET ((s16)0)
#define ACC_Z_OFFSET ((s16)8192)
#define GYR_X_OFFSET ((s16)0)
#define GYR_Y_OFFSET ((s16)0)
#define GYR_Z_OFFSET ((s16)0)
#define MAG_X_OFFSET ((s16)0)
#define MAG_Y_OFFSET ((s16)0)
#define MAG_Z_OFFSET ((s16)0)
#define TEMP_OFFSET  ((s16)(-12421))  // (340*35)+521
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
  s16 X;
  s16 Y;
  s16 Z;
  s16 OffsetX;
  s16 OffsetY;
  s16 OffsetZ;
  float TrueX;
  float TrueY;
  float TrueZ;
} SensorAcc;

typedef struct {
  s16 X;
  s16 Y;
  s16 Z;
  s16 OffsetX;
  s16 OffsetY;
  s16 OffsetZ;
  float TrueX;
  float TrueY;
  float TrueZ;
} SensorGyr;

typedef struct {
  s16 X;
  s16 Y;
  s16 Z;
  u16 AdjustX;
  u16 AdjustY;
  u16 AdjustZ;
  float TrueX;
  float TrueY;
  float TrueZ;
  float EllipseSita;
  float EllipseX0;
  float EllipseY0;
  float EllipseA;
  float EllipseB;
} SensorMag;

typedef struct {
  s16 T;
  s16 OffsetT;
  float TrueT;
} SensorTemp;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern SensorAcc Acc;
extern SensorGyr Gyr;
extern SensorMag Mag;
extern SensorTemp Temp;
/*=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_Config( void );
void Sensor_Init( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
