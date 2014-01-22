/* #include "QCopterFC_param.h" */

#ifndef __QCOPTERFC_PARAM_H
#define __QCOPTERFC_PARAM_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define PARAM_BYTE_MASK   0x03
#define PARAM_SIZE_1BYTE  0x00
#define PARAM_SIZE_2BYTE  0x01
#define PARAM_SIZE_4BYTE  0x02
#define PARAM_SIZE_8BYTE  0x03

#define PARAM_TYPE_INT    (0x00<<2)
#define PARAM_TYPE_FLOAT  (0x01<<2)

#define PARAM_SIGNED      (0x00<<3)
#define PARAM_UNSIGNED    (0x01<<3)

#define TYPE_S8   ((u8)(PARAM_SIZE_1BYTE | PARAM_TYPE_INT   | PARAM_SIGNED))
#define TYPE_U8   ((u8)(PARAM_SIZE_1BYTE | PARAM_TYPE_INT   | PARAM_UNSIGNED))
#define TYPE_S16  ((u8)(PARAM_SIZE_2BYTE | PARAM_TYPE_INT   | PARAM_SIGNED))
#define TYPE_U16  ((u8)(PARAM_SIZE_2BYTE | PARAM_TYPE_INT   | PARAM_UNSIGNED))
#define TYPE_S32  ((u8)(PARAM_SIZE_4BYTE | PARAM_TYPE_INT   | PARAM_SIGNED))
#define TYPE_U32  ((u8)(PARAM_SIZE_4BYTE | PARAM_TYPE_INT   | PARAM_UNSIGNED))
#define TYPE_S64  ((u8)(PARAM_SIZE_8BYTE | PARAM_TYPE_INT   | PARAM_SIGNED))
#define TYPE_U64  ((u8)(PARAM_SIZE_8BYTE | PARAM_TYPE_INT   | PARAM_UNSIGNED))
#define TYPE_FP32 ((u8)(PARAM_SIZE_4BYTE | PARAM_TYPE_FLOAT | PARAM_SIGNED))
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
  u8 TYPE;
  u8 *NAME;
  void *ADDR;
} PARAM_ST;

enum {
  TIME_MIN = 0,
  TIME_SEC,
  TIME_MSEC,
  PID_KP,
  PID_KI,
  PID_KD,
  THROTTLE_CH1,
  THROTTLE_CH2,
  THROTTLE_CH3,
  THROTTLE_CH4,
  THROTTLE_CH5,
  THROTTLE_CH6,
  BATTERY_VOL,
  BATTERY_CUR,
  BATTERY_CAP,
  SENSOR_ACC_X,
  SENSOR_ACC_Y,
  SENSOR_ACC_Z,
  SENSOR_GYR_X,
  SENSOR_GYR_Y,
  SENSOR_GYR_Z,
  SENSOR_MAG_X,
  SENSOR_MAG_Y,
  SENSOR_MAG_Z,
  BARO_TEMP,
  BARO_PRESS,
  BARO_HEIGHT,
  MOTION_ANG_X,
  MOTION_ANG_Y,
  MOTION_ANG_Z,
  MOTION_VEL_X,
  MOTION_VEL_Y,
  MOTION_VEL_Z,
  MOTION_POS_X,
  MOTION_POS_Y,
  MOTION_POS_Z,
  GPS_LON,
  GPS_LAT,
  PARAM_SIZE
};

extern PARAM_ST PARAMS[];
/*====================================================================================================*/
/*====================================================================================================*/
#endif
