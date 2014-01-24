/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
#include "QCopterFC.h"
#include "QCopterFC_board.h"
#include "QCopterFC_param.h"
#include "QCopterFC_ctrl.h"
#include "module_rs232.h"
#include "module_sensor.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
FSM_MODE FSM_STATE = FSM_TXRX;
SEN_MODE SEN_STATE = SEN_CORR;

// **************************** 測試變數 START

#define SampleRateFreg  ((u16)500)         // 500Hz
#define SampleRate      ((float)0.002f)    // 2.0ms
#define SampleRateHelf  ((float)0.001f)    // 1.0ms
  
extern vu8 Time_mSec;
extern vu8 Time_Sec;
extern vu8 Time_Min;

// **************************** 測試變數 END
/*=====================================================================================================*/
/*=====================================================================================================*/
void QCopterFC_Init( void )
{
  /* System Setup */
  SystemInit();

  /* Device Config */
  LED_Config();
  KEY_Config();
  ADC_Config();
  RS232_Config();
  Sensor_Config();
//  nRF24L01_Config();
  BLDC_Config();

  /* Throttle Config */
  if(KEY == KEY_ON) {
    LED_B = 0;
    BLDC_CtrlPWM(BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX);
    while(KEY == KEY_ON);
  }
  BLDC_CtrlPWM(BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN);
  LED_B = 1;

  Delay_10ms(5);

  /* Device Init */
  Sensor_Init();
//  nRF24L01_Init();

  // NRF CHECK!!

  /* Parameter Init */
  // Read Flash
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void QCopterFC_Corr( u16 SystickFreq )
{
  /* Select Correct */
  LED_G = 0;
  Delay_100ms(5);
  LED_G = 1;

  /* Systick Setup */
  if(SysTick_Config((u32)(SystemCoreClock/(float)SystickFreq)))
    while(1);

  /* Wait Correct */
  while(SEN_STATE != SEN_ALG);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void QCopterFC_Lock( void )
{
  u8 tempCnt = 0;
  u8 UART_BUF[8] = {0};

  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_OFF;
  while(KEY != KEY_ON) {
    if((tempCnt!=Time_Sec)) {
      tempCnt = Time_Sec;
      LED_B = !LED_B;
    }

    UART_BUF[0] = Byte8L(Acc.TrueX*1000);
    UART_BUF[1] = Byte8H(Acc.TrueX*1000);
    UART_BUF[2] = Byte8L(Acc.TrueY*1000);
    UART_BUF[3] = Byte8H(Acc.TrueY*1000);
    UART_BUF[4] = Byte8L(Acc.TrueZ*1000);
    UART_BUF[5] = Byte8H(Acc.TrueZ*1000);
    UART_BUF[6] = 0;
    UART_BUF[7] = 0;
    RS232_VisualScope(UART_BUF);

    // NRF TX ONLY

  }
  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_OFF;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  /* QCopterFC Init */
  QCopterFC_Init();

  /* QCopterFC Corr */
  QCopterFC_Corr(SampleRateFreg);   // SampleRateFreg = 500 Hz

  /* QCopterFC Lock */
  QCopterFC_Lock();

#define LED_DELAY 5
while(1) {
  LED_R = !LED_R;
  Delay_10ms(LED_DELAY);
  LED_R = !LED_R;
  Delay_10ms(LED_DELAY);
  LED_G = !LED_G;
  Delay_10ms(LED_DELAY);
  LED_G = !LED_G;
  Delay_10ms(LED_DELAY);
  LED_B = !LED_B;
  Delay_10ms(LED_DELAY);
  LED_B = !LED_B;
  Delay_10ms(LED_DELAY);
}

  /* QCopterFC FSM */
  while(1) {
    LED_G = !LED_G;
    switch(FSM_STATE) {

      /************************** FSM TXRX ****************************************/
      case FSM_TXRX:
        // FSM_TXRX
        
        // FSM_TXRX End
        FSM_STATE = FSM_CTRL;
        break;

      /************************** FSM CTRL ****************************************/
      case FSM_CTRL:
        // FSM_CTRL
        
        // FSM_CTRL End
        FSM_STATE = FSM_UART;
        break;

      /************************** FSM UART ****************************************/
      case FSM_UART:
        // FSM_UART
        
        // FSM_UART End
        FSM_STATE = FSM_DATA;
        break;

      /************************** FSM DATA ****************************************/
      case FSM_DATA:
        // FSM_DATA
        
        // FSM_DATA End
        FSM_STATE = FSM_TXRX;
        break;

      /************************** FSM Err *****************************************/
      default:
        LED_R = 1;
        LED_G = 1;
        LED_B = 1;
        while(1) {
          LED_R = !LED_R;
          LED_B = !LED_B;
          Delay_100ms(10);
        }
    }
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
