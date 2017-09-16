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
extern IMU_DataTypeDef imu;
float32_t FIFO_P[16] = {0};

/* Private function prototypes -------------------------------------------------------------*/
/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  HAL_Init();
  BSP_GPIO_Config();
  BSP_UART_SERIAL_Config(NULL);
  BSP_SPI_IMU_Config();

  while (1) {

#if 0
    LED_B_Toggle();
    delay_ms(100);
    IMU_GetRealData(&imu);
    printf("GX:%5i\tGY:%5i\tGZ:%5i\tAX:%5i\tAY:%5i\tAZ:%5i\tMX:%5i\tMY:%5i\tMZ:%5i\r\n",
      imu.gyrRaw[0], imu.gyrRaw[1], imu.gyrRaw[2],
      imu.accRaw[0], imu.accRaw[1], imu.accRaw[2],
      imu.magRaw[0], imu.magRaw[1], imu.magRaw[2]
    );
#endif

#if 1
    LED_B_Toggle();
    delay_ms(100);
    IMU_GetRealData(&imu);

    float32_t tmp = 0;
    float32_t pressure, temperature, altitude;
    for (uint8_t i = 0; i < 15; i++) {
      FIFO_P[i] = FIFO_P[i + 1];
    }
    FIFO_P[15] = imu.baroData[0];

    for (uint8_t i = 0; i < 16; i++) {
      tmp += FIFO_P[i];
    }
    pressure    = tmp / 16.0f;
    temperature = imu.baroData[1];
    altitude    = (powf(1015.7f / pressure, 0.1902630958f) - 1.0f) * 45869.2308f;

    printf("[GYR]%8.2f, %8.2f, %8.2f [ACC]%7.4f, %7.4f, %7.4f [MAG]%6.1f, %6.1f, %6.1f [BAR] %8.2f, %7.2f, %8.2f\r\n",
      imu.gyrData[0], imu.gyrData[1], imu.gyrData[2],
      imu.accData[0], imu.accData[1], imu.accData[2],
      imu.magData[0], imu.magData[1], imu.magData[2],
      pressure, temperature, altitude
    );
#endif

#if 0
    int16_t sendBuf[PACKET_LENS];
    LED_B_Toggle();
    IMU_GetRealData(&imu);
    sendBuf[0] = imu.gyrRaw[0];
    sendBuf[1] = imu.gyrRaw[1];
    sendBuf[2] = imu.gyrRaw[2];
    sendBuf[3] = imu.accRaw[0];
    sendBuf[4] = imu.accRaw[1];
    sendBuf[5] = imu.accRaw[2];
    sendBuf[6] = imu.magRaw[0];
    sendBuf[7] = imu.magRaw[1];
    sendBuf[8] = imu.magRaw[2];
    kSerial_SendData(sendBuf, PACKET_LENS, KS_INT16);
#endif

  }
}

/*************************************** END OF FILE ****************************************/
