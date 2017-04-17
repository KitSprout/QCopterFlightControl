/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    ahrs.h
  * @author  KitSprout
  * @date    29-Mar-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __AHRS_H
#define __AHRS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\imu.h"
#include "algorithms\quaternion.h"

/* Exported types --------------------------------------------------------------------------*/
typedef struct {
  TIM_HandleTypeDef *handle;
  pFunc EventCallback;
} TimHandle_st;

typedef struct {
  float32_t sampleTime;
  float32_t helfSampleTime;
  quaternion_t numQ;
  eulerAngle_t angE;
  imu_t imu;
} __attribute__((aligned)) ahrs_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/
void AHRS_Config( void );
void AHRS_Init( ahrs_t *ahrs );
void AHRS_Cmd( uint8_t cmd );
void AHRS_Update( ahrs_t *ahrs );
void AHRS_GyroBiasCorrection( int16_t *gyro, float32_t *bias );

extern TimHandle_st hTimAhrs;

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
