/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    quaternion.h
  * @author  KitSprout
  * @date    27-Mar-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __QUATERNION_H
#define __QUATERNION_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "algorithms\mathUnit.h"

/* Exported types --------------------------------------------------------------------------*/
typedef struct {
  float32_t q0;
  float32_t q1;
  float32_t q2;
  float32_t q3;
  float32_t rMat[3][3];
} __attribute__((aligned(4))) quaternion_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------------------*/  
void Quaternion_Clear( quaternion_t *pNumQ );
void Quaternion_ToNumQ( quaternion_t *pNumQ, const eulerAngle_t *pAngE );
void Quaternion_ToAngE( eulerAngle_t *pAngE, const quaternion_t *pNumQ );
void Quaternion_UpdateRotMatrix( quaternion_t *pNumQ );
void Quaternion_Add( quaternion_t *pNumQ, const quaternion_t *pNumA, const quaternion_t *pNumB );
void Quaternion_Sub( quaternion_t *pNumQ, const quaternion_t *pNumA, const quaternion_t *pNumB );
void Quaternion_Mult( quaternion_t *pNumQ, const quaternion_t *pNumA, const quaternion_t *pNumB );
void Quaternion_MultVector( quaternion_t *pNumQ, const quaternion_t *pNumA, const float32_t *pVect );
void Quaternion_Conj( quaternion_t *pNumQ, const quaternion_t *pNumC );
void Quaternion_Norm( quaternion_t *pNumQ, const quaternion_t *pNumN );
void Quaternion_FastNorm( quaternion_t *pNumQ, const quaternion_t *pNumN );
void Quaternion_RungeKutta( quaternion_t *pNumQ, const float32_t *gyro, const float32_t helfTime );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
