/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    mathUnit.h
  * @author  KitSprout
  * @date    16-Mar-2017
  * @brief   
  * 
  */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __MATHUNIT_H
#define __MATHUNIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "core_cm4.h"
#include "arm_math.h"

/* Exported types --------------------------------------------------------------------------*/

/** 
  * @brief  euler angle structure definition
  */ 
typedef struct {
  float32_t pitch;
  float32_t roll;
  float32_t yaw;
} __attribute__((aligned(4))) eulerAngle_t;

typedef struct {
  float32_t x;
  float32_t y;
  float32_t z;
} __attribute__((aligned(4))) vector_t;

/* Exported constants ----------------------------------------------------------------------*/
/* Exported macro --------------------------------------------------------------------------*/
#define invSqrtf( __iSq )  (1.0f / sqrtf((float32_t)(__iSq)))
#define squa( __Sq )       (((float32_t)(__Sq)) * ((float32_t)(__Sq)))
#define toRad( __mathD )   ((__mathD) * 0.0174532925f)
#define toDeg( __mathR )   ((__mathR) * 57.2957795f)
#define absInt( __n )      (((__n) > 0) ? (__n) : -(__n))
#define arrayLens(__arr)   (sizeof(__arr) / sizeof((__arr)[0]))

__STATIC_INLINE void swapU8( uint8_t *a, uint8_t *b )
{
  if (*a != *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
  }
}

__STATIC_INLINE void swapS8( int8_t *a, int8_t *b )
{
  if (*a != *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
  }
}

__STATIC_INLINE float32_t invSqrtf_fast( const float32_t __iSq )
{
  float32_t x2 = 0.5f * __iSq;
  float32_t y = __iSq;
  int32_t i = *(int32_t*)&y;
  i = 0x5F3759DF - (i >> 1);
  y = *(float32_t*)&i;
  y = y * (1.5f - (x2 * y * y));
  return y;
}

/* Exported functions ----------------------------------------------------------------------*/  

#ifdef __cplusplus
}
#endif

#endif
