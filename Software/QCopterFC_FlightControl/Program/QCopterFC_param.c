/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC_param.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
//PARAM_ST PARAMS[PARAM_SIZE] = {
//  [TIME_MIN]      = {.TYPE = TYPE_U8,   .NAME = (u8*)"TIME.M",  .ADDR = NULL},
//  [TIME_SEC]      = {.TYPE = TYPE_U8,   .NAME = (u8*)"TIME.S",  .ADDR = NULL},
//  [TIME_MSEC]     = {.TYPE = TYPE_U8,   .NAME = (u8*)"TIME.MS", .ADDR = NULL},
//  // Parket 1
//  [PID_KP]        = {.TYPE = TYPE_FP32, .NAME = (u8*)"PID.KP",  .ADDR = NULL},
//  [PID_KI]        = {.TYPE = TYPE_FP32, .NAME = (u8*)"PID.KI",  .ADDR = NULL},
//  [PID_KD]        = {.TYPE = TYPE_FP32, .NAME = (u8*)"PID.KD",  .ADDR = NULL},
//  [THROTTLE_CH1]  = {.TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH1", .ADDR = NULL},
//  [THROTTLE_CH2]  = {.TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH2", .ADDR = NULL},
//  [THROTTLE_CH3]  = {.TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH3", .ADDR = NULL},
//  [THROTTLE_CH4]  = {.TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH4", .ADDR = NULL},
//  [THROTTLE_CH5]  = {.TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH5", .ADDR = NULL},
//  [THROTTLE_CH6]  = {.TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH6", .ADDR = NULL},
//  [BATTERY_VOL]   = {.TYPE = TYPE_FP32, .NAME = (u8*)"BAT.VOL", .ADDR = NULL},
//  [BATTERY_CUR]   = {.TYPE = TYPE_FP32, .NAME = (u8*)"BAT.CUR", .ADDR = NULL},
//  [BATTERY_CAP]   = {.TYPE = TYPE_FP32, .NAME = (u8*)"BAT.CAP", .ADDR = NULL},
//  // Parket 2
//  [SENSOR_ACC_X]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"ACC.X",   .ADDR = NULL},
//  [SENSOR_ACC_Y]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"ACC.Y",   .ADDR = NULL},
//  [SENSOR_ACC_Z]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"ACC.Z",   .ADDR = NULL},
//  [SENSOR_GYR_X]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"GYR.X",   .ADDR = NULL},
//  [SENSOR_GYR_Y]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"GYR.Y",   .ADDR = NULL},
//  [SENSOR_GYR_Z]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"GYR.Z",   .ADDR = NULL},
//  [SENSOR_MAG_X]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"MAG.X",   .ADDR = NULL},
//  [SENSOR_MAG_Y]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"MAG.Y",   .ADDR = NULL},
//  [SENSOR_MAG_Z]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"MAG.Z",   .ADDR = NULL},
//  [BARO_TEMP]     = {.TYPE = TYPE_FP32, .NAME = (u8*)"BARO.T",  .ADDR = NULL},
//  [BARO_PRESS]    = {.TYPE = TYPE_FP32, .NAME = (u8*)"BARO.P",  .ADDR = NULL},
//  [BARO_HEIGHT]   = {.TYPE = TYPE_FP32, .NAME = (u8*)"BARO.H",  .ADDR = NULL},
//  // Parket 3
//  [MOTION_ANG_X]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"ANG.X",   .ADDR = NULL},
//  [MOTION_ANG_Y]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"ANG.Y",   .ADDR = NULL},
//  [MOTION_ANG_Z]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"ANG.Z",   .ADDR = NULL},
//  [MOTION_VEL_X]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"VEL.X",   .ADDR = NULL},
//  [MOTION_VEL_Y]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"VEL.Y",   .ADDR = NULL},
//  [MOTION_VEL_Z]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"VEL.Z",   .ADDR = NULL},
//  [MOTION_POS_X]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"POS.X",   .ADDR = NULL},
//  [MOTION_POS_Y]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"POS.Y",   .ADDR = NULL},
//  [MOTION_POS_Z]  = {.TYPE = TYPE_FP32, .NAME = (u8*)"POS.Z",   .ADDR = NULL},
//  [GPS_LON]       = {.TYPE = TYPE_U32,  .NAME = (u8*)"GPS.LON", .ADDR = NULL},
//  [GPS_LAT]       = {.TYPE = TYPE_U32,  .NAME = (u8*)"GPS.LAT", .ADDR = NULL}
//};
/*=====================================================================================================*/
/*=====================================================================================================*/
//void Param_Init( PARAM_ST *PARAM_INIT )
//{
//  PARAM_INIT[TIME_MIN].TYPE  = TYPE_U8;
//  PARAM_INIT[TIME_MIN].NAME  = (u8*)"TIME.M";
//  PARAM_INIT[TIME_MIN].ADDR  = NULL;

//  PARAM_INIT[TIME_SEC].TYPE  = TYPE_U8;
//  PARAM_INIT[TIME_SEC].NAME  = (u8*)"TIME.S";
//  PARAM_INIT[TIME_SEC].ADDR  = NULL;

//  PARAM_INIT[TIME_MSEC].TYPE = TYPE_U8;
//  PARAM_INIT[TIME_MSEC].NAME = (u8*)"TIME.MS";
//  PARAM_INIT[TIME_MSEC].ADDR = NULL;
//}
/*=====================================================================================================*/
/*=====================================================================================================*/
