/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    ahrs.c
  * @author  KitSprout
  * @date    29-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "algorithms\ahrs.h"

/** @addtogroup STM32_Algorithms
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define MF_KP 0.4f
//#define MF_KI 0.001f

//#define MAHONY_FILTER_9DOF

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
static TIM_HandleTypeDef HTIM_AHRS;

TimHandle_st hTimAhrs = {
  .handle        = &HTIM_AHRS,
  .EventCallback = NULL,
};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

/**
  * @brief  AHRS_Config
  */
void AHRS_Config( void )
{
  /* TIM Clk ********************************************************************/
  AHRS_TIMx_CLK_ENABLE();

  /* TIM NVIC Config ************************************************************/
  HAL_NVIC_SetPriority(AHRS_TIMx_IRQn, AHRS_TIMx_IRQn_PREEMPT, AHRS_TIMx_IRQn_SUB);
  HAL_NVIC_EnableIRQ(AHRS_TIMx_IRQn);

  /* TIM Base Config ************************************************************/
  HTIM_AHRS.Instance               = AHRS_TIMx;
  HTIM_AHRS.Init.Prescaler         = (SystemCoreClock / (SAMPLE_RATE * 1000)) - 1;
  HTIM_AHRS.Init.Period            = 1000 - 1;
  HTIM_AHRS.Init.ClockDivision     = 0;
  HTIM_AHRS.Init.CounterMode       = TIM_COUNTERMODE_UP;
  HTIM_AHRS.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&HTIM_AHRS);

  /* TIM Enable *****************************************************************/
  HAL_TIM_Base_Start_IT(&HTIM_AHRS);
}

/**
  * @brief  AHRS_Init
  */
void AHRS_Init( ahrs_t *ahrs )
{
  ahrs->angE.pitch = 0.0f;
  ahrs->angE.roll  = 0.0f;
  ahrs->angE.yaw   = 0.0f;

  Quaternion_Clear(&ahrs->numQ);

  ahrs->sampleTime     = 1.0f / SAMPLE_RATE;
  ahrs->helfSampleTime = ahrs->sampleTime * 0.5f;
}

/**
  * @brief  AHRS_Cmd
  */
void AHRS_Cmd( uint8_t cmd )
{
  if (cmd == ENABLE) {
    HAL_TIM_Base_Start_IT(&HTIM_AHRS);
  }
  else {
    HAL_TIM_Base_Stop_IT(&HTIM_AHRS);
  }
}

/**
  * @brief  AHRS_Update
  * Use Mahony Filter
  * detail : http://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
  */
void AHRS_Update( ahrs_t *ahrs )
{
  float32_t gVect[3];
  float32_t gyr[3], acc[3];

#if defined(MAHONY_FILTER_9DOF)
  float32_t hVect[3], wVect[3];
  float32_t mag[3];
#endif

  float32_t err[3];
#if defined(MF_KI)
  static float32_t errInt[3] = {0};
#endif

  gyr[0] = toRad(ahrs->imu.gyrData[0]);
  gyr[1] = toRad(ahrs->imu.gyrData[1]);
  gyr[2] = toRad(ahrs->imu.gyrData[2]);
  acc[0] = ahrs->imu.accData[0];
  acc[1] = ahrs->imu.accData[1];
  acc[2] = ahrs->imu.accData[2];

  /* Normalise accelerometer measurement */
  const float32_t normAcc = invSqrtf(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);
  acc[0] *= normAcc;
  acc[1] *= normAcc;
  acc[2] *= normAcc;

  /* Estimated direction of gravity */
  gVect[0] = ahrs->numQ.rMat[0][2];
  gVect[1] = ahrs->numQ.rMat[1][2];
  gVect[2] = ahrs->numQ.rMat[2][2];

  err[0] = acc[1] * gVect[2] - acc[2] * gVect[1];
  err[1] = acc[2] * gVect[0] - acc[0] * gVect[2];
  err[2] = acc[0] * gVect[1] - acc[1] * gVect[0];

#if defined(MAHONY_FILTER_9DOF)
  mag[0] = ahrs->imu.magData[0];
  mag[1] = ahrs->imu.magData[1];
  mag[2] = ahrs->imu.magData[2];

//  const float32_t normMag = invSqrtf(mag[0] * mag[0] + mag[1] * mag[1] + mag[2] * mag[2]);
//  if ((normMag < fusionAhrs->minMagneticFieldSquared) ||
//      (normMag > fusionAhrs->maxMagneticFieldSquared)) {
//    break;
//  }

  /* Reference direction of Earth's magnetic field */
  wVect[0] = acc[1] * mag[2] - acc[2] * mag[1];
  wVect[1] = acc[2] * mag[0] - acc[0] * mag[2];
  wVect[2] = acc[0] * mag[1] - acc[1] * mag[0];

  /* Normalise magnetometer measurement */
  const float32_t normVect = invSqrtf(wVect[0] * wVect[0] + wVect[1] * wVect[1] + wVect[2] * wVect[2]);
  wVect[2] *= normVect;
  wVect[2] *= normVect;
  wVect[2] *= normVect;

  /* Estimated direction of Earth's magnetic field  */
  hVect[0] = ahrs->numQ.rMat[0][1];
  hVect[1] = ahrs->numQ.rMat[1][1];
  hVect[2] = ahrs->numQ.rMat[2][1];

  /* Error is sum of cross product between estimated direction and measured direction of field vectors */
  err[0] += wVect[1] * hVect[2] - wVect[2] * hVect[1];
  err[1] += wVect[2] * hVect[0] - wVect[0] * hVect[2];
  err[2] += wVect[0] * hVect[1] - wVect[1] * hVect[0];

#endif

#if defined(MF_KI)
  /* Compute and apply integral feedback */
  errInt[0] += (ahrs->sampleTime * MF_KI) * err[0];
  errInt[1] += (ahrs->sampleTime * MF_KI) * err[1];
  errInt[2] += (ahrs->sampleTime * MF_KI) * err[2];

  /* Apply proportional feedback */
  gyr[0] += MF_KP * err[0] + errInt[0];
  gyr[1] += MF_KP * err[1] + errInt[1];
  gyr[2] += MF_KP * err[2] + errInt[2];

#else
  gyr[0] += MF_KP * err[0];
  gyr[1] += MF_KP * err[1];
  gyr[2] += MF_KP * err[2];

#endif

  /* Integrate rate of change of quaternion */
  Quaternion_RungeKutta(&ahrs->numQ, gyr, ahrs->helfSampleTime);
  Quaternion_Norm(&ahrs->numQ, &ahrs->numQ);
  Quaternion_UpdateRotMatrix(&ahrs->numQ);
  Quaternion_ToAngE(&ahrs->angE, &ahrs->numQ);
}

/**
  * @brief  AHRS_GyroBiasCorrection
  */
#define CUTOFF_FREQ     (0.02f)
#define TIME_CONST      (1.0f / (2.0f * PI * CUTOFF_FREQ))
#define THRESHOLD       (30)
#define STIRLESS_TIME   (SAMPLE_RATE << 1)  // 2 second
void AHRS_GyroBiasCorrection( int16_t *gyro, float32_t *bias )
{
  static const float32_t alpha = TIME_CONST / (TIME_CONST + SAMPLE_TIME);
  static const float32_t alpha_n = 1.0f - alpha;
  static uint16_t stirlessTime = 0;

  if ((gyro[0] > THRESHOLD) || (gyro[0] < -THRESHOLD) ||
      (gyro[1] > THRESHOLD) || (gyro[1] < -THRESHOLD) ||
      (gyro[2] > THRESHOLD) || (gyro[2] < -THRESHOLD)) {
    stirlessTime = 0.0f;
  }
  else {
    if (stirlessTime >= STIRLESS_TIME) {
      bias[0] = alpha * bias[0] + alpha_n * gyro[0];
      bias[1] = alpha * bias[1] + alpha_n * gyro[1];
      bias[2] = alpha * bias[2] + alpha_n * gyro[2];
    }
    else {
      stirlessTime++;
    }
  }
}

/*************************************** END OF FILE ****************************************/
