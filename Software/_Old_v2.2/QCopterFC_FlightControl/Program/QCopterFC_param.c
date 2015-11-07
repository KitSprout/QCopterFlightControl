/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_param.h"
#include "module_imu.h"
#include "algorithm_pid.h"
#include "algorithm_quaternion.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
PARAM_ST PARAM[PARAM_SIZE] =
{
  /* Board Info*/
  [INFO]         = { .TYPE = TYPE_U8,   .NAME = (u8*)"INFO",     (void*)(NULL) },

  /* Time */
  [TIME_MIN]     = { .TYPE = TYPE_U8,   .NAME = (u8*)"TIME.M",   (void*)(&Time_Min) },
  [TIME_SEC]     = { .TYPE = TYPE_U8,   .NAME = (u8*)"TIME.S",   (void*)(&Time_Sec) },

  /* PID Paramter */
  [PID_P_KP]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_P.KP", (void*)(&PID_Pitch.Kp) },
  [PID_P_KI]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_P.KI", (void*)(&PID_Pitch.Ki) },
  [PID_P_KD]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_P.KD", (void*)(&PID_Pitch.Kd) },
  [PID_R_KP]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_I.KP", (void*)(&PID_Roll.Kp) },
  [PID_R_KI]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_I.KI", (void*)(&PID_Roll.Ki) },
  [PID_R_KD]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_I.KD", (void*)(&PID_Roll.Kd) },
  [PID_Y_KP]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_D.KP", (void*)(&PID_Yaw.Kp) },
  [PID_Y_KI]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_D.KI", (void*)(&PID_Yaw.Ki) },
  [PID_Y_KD]     = { .TYPE = TYPE_FP32, .NAME = (u8*)"PID_D.KD", (void*)(&PID_Yaw.Kd) },

  /* Throttle */
  [THROTTLE_CH1] = { .TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH1",  (void*)(NULL) },
  [THROTTLE_CH2] = { .TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH2",  (void*)(NULL) },
  [THROTTLE_CH3] = { .TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH3",  (void*)(NULL) },
  [THROTTLE_CH4] = { .TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH4",  (void*)(NULL) },
  [THROTTLE_CH5] = { .TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH5",  (void*)(NULL) },
  [THROTTLE_CH6] = { .TYPE = TYPE_U16,  .NAME = (u8*)"THR.CH6",  (void*)(NULL) },

  /* Battery */
  [BATTERY_VOL]  = { .TYPE = TYPE_FP32, .NAME = (u8*)"BAT.VOL",  (void*)(NULL) },
  [BATTERY_CUR]  = { .TYPE = TYPE_FP32, .NAME = (u8*)"BAT.CUR",  (void*)(NULL) },
  [BATTERY_CAP]  = { .TYPE = TYPE_FP32, .NAME = (u8*)"BAT.CAP",  (void*)(NULL) },

  /* Accelerometer */
  [SENSOR_ACC_X] = { .TYPE = TYPE_FP32, .NAME = (u8*)"ACC.X",    (void*)(&Acc.TrueX) },
  [SENSOR_ACC_Y] = { .TYPE = TYPE_FP32, .NAME = (u8*)"ACC.Y",    (void*)(&Acc.TrueY) },
  [SENSOR_ACC_Z] = { .TYPE = TYPE_FP32, .NAME = (u8*)"ACC.Z",    (void*)(&Acc.TrueZ) },

  /* Gyroscope */
  [SENSOR_GYR_X] = { .TYPE = TYPE_FP32, .NAME = (u8*)"GYR.X",    (void*)(&Gyr.TrueX) },
  [SENSOR_GYR_Y] = { .TYPE = TYPE_FP32, .NAME = (u8*)"GYR.Y",    (void*)(&Gyr.TrueY) },
  [SENSOR_GYR_Z] = { .TYPE = TYPE_FP32, .NAME = (u8*)"GYR.Z",    (void*)(&Gyr.TrueZ) },

#ifdef USE_SENSOR_MAG
  /* Magnetometer */
  [SENSOR_MAG_X] = { .TYPE = TYPE_FP32, .NAME = (u8*)"MAG.X",    (void*)(&Mag.TrueX) },
  [SENSOR_MAG_Y] = { .TYPE = TYPE_FP32, .NAME = (u8*)"MAG.Y",    (void*)(&Mag.TrueY) },
  [SENSOR_MAG_Z] = { .TYPE = TYPE_FP32, .NAME = (u8*)"MAG.Z",    (void*)(&Mag.TrueZ) },
#endif

  /* Barometer */
#ifdef USE_SENSOR_BARO
  [BARO_TEMP]    = { .TYPE = TYPE_FP32, .NAME = (u8*)"BARO.T",   (void*)(&Baro.Temp) },
  [BARO_PRESS]   = { .TYPE = TYPE_FP32, .NAME = (u8*)"BARO.P",   (void*)(&Baro.Press) },
  [BARO_HEIGHT]  = { .TYPE = TYPE_FP32, .NAME = (u8*)"BARO.H",   (void*)(&Baro.Height) },
#endif

  /* Attitude Angle */
  [MOTION_ANG_X] = { .TYPE = TYPE_FP32, .NAME = (u8*)"ANG.X",    (void*)(&AngE.Pitch) },
  [MOTION_ANG_Y] = { .TYPE = TYPE_FP32, .NAME = (u8*)"ANG.Y",    (void*)(&AngE.Roll) },
  [MOTION_ANG_Z] = { .TYPE = TYPE_FP32, .NAME = (u8*)"ANG.Z",    (void*)(&AngE.Yaw) },

  /* Velocity */
  [MOTION_VEL_X] = { .TYPE = TYPE_FP32, .NAME = (u8*)"VEL.X",    (void*)(NULL) },
  [MOTION_VEL_Y] = { .TYPE = TYPE_FP32, .NAME = (u8*)"VEL.Y",    (void*)(NULL) },
  [MOTION_VEL_Z] = { .TYPE = TYPE_FP32, .NAME = (u8*)"VEL.Z",    (void*)(NULL) },

  /* Position */
  [MOTION_POS_X] = { .TYPE = TYPE_FP32, .NAME = (u8*)"POS.X",    (void*)(NULL) },
  [MOTION_POS_Y] = { .TYPE = TYPE_FP32, .NAME = (u8*)"POS.Y",    (void*)(NULL) },
  [MOTION_POS_Z] = { .TYPE = TYPE_FP32, .NAME = (u8*)"POS.Z",    (void*)(NULL) },

  /* GPS */
  [GPS_LON]      = { .TYPE = TYPE_FP32, .NAME = (u8*)"GPS.LON",  (void*)(NULL) },
  [GPS_LAT]      = { .TYPE = TYPE_FP32, .NAME = (u8*)"GPS.LAT",  (void*)(NULL) },
};
/*=====================================================================================================*/
/*=====================================================================================================*/
