/**
  *      __            ____
  *     / /__ _  __   / __/                      __  
  *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
  *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
  *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
  *                    /_/   github.com/KitSprout    
  * 
  * @file    main.c
  * @author  KitSprout
  * @date    16-Mar-2017
  * @brief   
  * 
  */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\stm32f4_system.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "modules\imu.h"
#include "algorithms\ahrs.h"
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define PACKET_LENS 15

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
ahrs_t ahrs;

static uint16_t sec = 0;
static uint16_t msec = 0;
static int16_t sendBuf[PACKET_LENS] = {0};

__IO static uint8_t flag = ERROR;

/* Private function prototypes -------------------------------------------------------------*/
void IRQEvent_AHRS( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();
  BSP_GPIO_Config();
  BSP_UART_SERIAL_Config(NULL);
  BSP_SPI_IMU_Config();
  BSP_AHRS_Config(IRQEvent_AHRS, SAMPLE_RATE);

  while (1) {

#if 0
    LED_B_Toggle();
    delay_ms(100);
#if 0
    printf("GX:%5i\tGY:%5i\tGZ:%5i\tAX:%5i\tAY:%5i\tAZ:%5i\tMX:%6.1f\tMY:%6.1f\tMZ:%6.1f\r\n",
      ahrs.imu.gyrInt[0], ahrs.imu.gyrInt[1], ahrs.imu.gyrInt[2],
      ahrs.imu.accInt[0], ahrs.imu.accInt[1], ahrs.imu.accInt[2],
      ahrs.imu.magInt[0], ahrs.imu.magInt[1], ahrs.imu.magInt[2]
    );
#else
    printf("GX:%8.2f\tGY:%8.2f\tGZ:%5.2f\tAX:%7.4f\tAY:%7.4f\tAZ:%7.4f\tMX:%6.1f\tMY:%6.1f\tMZ:%6.1f\r\n",
      ahrs.imu.gyrData[0], ahrs.imu.gyrData[1], ahrs.imu.gyrData[2],
      ahrs.imu.accData[0], ahrs.imu.accData[1], ahrs.imu.accData[2],
//      ahrs.imu.magData[0], ahrs.imu.magData[1], ahrs.imu.magData[2]
      ahrs.angE.pitch, ahrs.angE.roll, ahrs.angE.yaw
    );
#endif

#else
    LED_G_Toggle();
    if (flag == ENABLE) {
      flag = DISABLE;
      sendBuf[0]  = ahrs.imu.gyrInt[0];
      sendBuf[1]  = ahrs.imu.gyrInt[1];
      sendBuf[2]  = ahrs.imu.gyrInt[2];
      sendBuf[3]  = ahrs.imu.accInt[0];
      sendBuf[4]  = ahrs.imu.accInt[1];
      sendBuf[5]  = ahrs.imu.accInt[2];
      sendBuf[6]  = ahrs.imu.magInt[0];
      sendBuf[7]  = ahrs.imu.magInt[1];
      sendBuf[8]  = ahrs.imu.magInt[2];
      sendBuf[9]  = ahrs.numQ.q0 * 10000;
      sendBuf[10] = ahrs.numQ.q1 * 10000;
      sendBuf[11] = ahrs.numQ.q2 * 10000;
      sendBuf[12] = ahrs.numQ.q3 * 10000;
      sendBuf[13] = sec;
      sendBuf[14] = msec;
      kSerial_SendData(sendBuf, PACKET_LENS, KS_INT16);
    }
#endif
  }
}

void IRQEvent_AHRS( void )
{
  if (++msec >= SAMPLE_RATE) {
    msec = 0;
    sec++;
  }

  IMU_GetRealData(&ahrs.imu);
  AHRS_GyroBiasCorrection(ahrs.imu.gyrRaw, ahrs.imu.gyrOffset);
  AHRS_Update(&ahrs);
  flag = ENABLE;
}

/*************************************** END OF FILE ****************************************/
