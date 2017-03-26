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
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
#define PACKET_LENS 9

/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
extern IMU_DataTypeDef IMU;
static int16_t sendBuf[PACKET_LENS] = {0};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();
  BSP_GPIO_Config();
  BSP_UART_SERIAL_Config(NULL);
  BSP_SPI_IMU_Config();

  while (1) {

#if 1
    LED_B_Toggle();
    delay_ms(100);
    IMU_GetRawData(&IMU);
    printf("GX:%5.0f\tGY:%5.0f\tGZ:%5.0f\tAX:%5.0f\tAY:%5.0f\tAZ:%5.0f\tMX:%5.0f\tMY:%5.0f\tMZ:%5.0f\r\n",
      IMU.gyrRaw[0], IMU.gyrRaw[1], IMU.gyrRaw[2],
      IMU.accRaw[0], IMU.accRaw[1], IMU.accRaw[2],
      IMU.magRaw[0], IMU.magRaw[1], IMU.magRaw[2]
    );
#endif

#if 0
    LED_B_Toggle();
    delay_ms(100);
    IMU_GetScaleData(&IMU);
    printf("GX:%8.2f\tGY:%8.2f\tG8:%5.2f\tAX:%7.4f\tAY:%7.4f\tAZ:%7.4f\tMX:%6.1f\tMY:%6.1f\tMZ:%6.1f\r\n",
      IMU.gyrData[0], IMU.gyrData[1], IMU.gyrData[2],
      IMU.accData[0], IMU.accData[1], IMU.accData[2],
      IMU.magData[0], IMU.magData[1], IMU.magData[2]
    );
#endif

#if 0
    LED_B_Toggle();
    IMU_GetRawData(&IMU);
    sendBuf[0] = IMU.gyrRaw[0];
    sendBuf[1] = IMU.gyrRaw[1];
    sendBuf[2] = IMU.gyrRaw[2];
    sendBuf[3] = IMU.accRaw[0];
    sendBuf[4] = IMU.accRaw[1];
    sendBuf[5] = IMU.accRaw[2];
    sendBuf[6] = IMU.magRaw[0];
    sendBuf[7] = IMU.magRaw[1];
    sendBuf[8] = IMU.magRaw[2];
    kSerial_SendData(sendBuf, PACKET_LENS, KS_INT16);

#endif

  }
}

/*************************************** END OF FILE ****************************************/
