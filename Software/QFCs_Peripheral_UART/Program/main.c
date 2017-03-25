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
#include "stm32f4xx_bsp.h"

/** @addtogroup STM32_Program
  * @{
  */

/* Private typedef -------------------------------------------------------------------------*/
/* Private define --------------------------------------------------------------------------*/
/* Private macro ---------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------*/
/* Private function prototypes -------------------------------------------------------------*/
void IRQEvent_SerialRecv( void );

/* Private functions -----------------------------------------------------------------------*/

int main( void )
{
  static uint8_t i = 0;

  HAL_Init();
  BSP_GPIO_Config();

  if (!KEY_Read()) {
    BSP_UART_SERIAL_Config(NULL);

    while (1) {
      int8_t status = Serial_RecvData(hSerial.pRxBuf, 1, 500);

      if(status == HAL_TIMEOUT) {
        LED_R_Toggle();
        printf("Timeout ... %d\r\n", i);
        i = (i == 255) ? 0 : i + 1;
      }
      else if(hSerial.pRxBuf[0] == 0x0D) { // if press enter
        LED_G_Toggle();
        printf("\r\n");
      }
      else {
        LED_B_Toggle();
        Serial_SendData(hSerial.pRxBuf, 1, 2000);
      }
      if(KEY_Read()) {
        printf("\f");
      }
    }
  }
  else {
    BSP_UART_SERIAL_Config(IRQEvent_SerialRecv);

#if defined(KS_HW_UART_HAL_LIBRARY)
    Serial_RecvDataIT(hSerial.pRxBuf, 1);
#endif

    while (1) {
      LED_B_Toggle();
      delay_ms(100);
      while (KEY_Read()) {
        printf("\f");
      }
    }
  }
}

void IRQEvent_SerialRecv( void )
{
#if defined(KS_HW_UART_HAL_LIBRARY)
  Serial_RecvDataIT(hSerial.pRxBuf, 1);

#else
  Serial_RecvData(hSerial.pRxBuf, 1, 500);

#endif

  LED_G_Toggle();
  if (hSerial.pRxBuf[0] == 0x0D)
    printf("\r\n");
  else
    Serial_SendData(hSerial.pRxBuf, 1, 1000);
}

/*************************************** END OF FILE ****************************************/
