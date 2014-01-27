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
typedef __IO struct {
  s16 X;
  s16 Y;
  s16 Z;
  s16 OffsetX;
  s16 OffsetY;
  s16 OffsetZ;
  fp32 TrueX;
  fp32 TrueY;
  fp32 TrueZ;
} SensorAcc;

typedef __IO struct {
  s16 X;
  s16 Y;
  s16 Z;
  s16 OffsetX;
  s16 OffsetY;
  s16 OffsetZ;
  fp32 TrueX;
  fp32 TrueY;
  fp32 TrueZ;
} SensorGyr;

typedef __IO struct {
  s16 X;
  s16 Y;
  s16 Z;
  u16 AdjustX;
  u16 AdjustY;
  u16 AdjustZ;
  fp32 TrueX;
  fp32 TrueY;
  fp32 TrueZ;
  fp32 EllipseSita;
  fp32 EllipseX0;
  fp32 EllipseY0;
  fp32 EllipseA;
  fp32 EllipseB;
} SensorMag;

typedef __IO struct {
  s16 T;
  s16 OffsetT;
  fp32 TrueT;
} SensorTemp;

typedef __IO struct {
  fp32 Temp;
  fp32 Press;
  fp32 Height;
} SensorBaro;
/*=====================================================================================================*/
/*=====================================================================================================*/
extern SensorAcc Acc;
extern SensorGyr Gyr;
extern SensorMag Mag;
extern SensorTemp Temp;
extern SensorBaro Baro;
/*=====================================================================================================*/
/*=====================================================================================================*/
void Sensor_Config( void );
void Sensor_Init( void );
void Sensor_Read( u16 ReadFreg );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
