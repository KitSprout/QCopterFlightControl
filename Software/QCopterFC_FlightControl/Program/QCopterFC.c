/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_adc.h"
#include "QCopterFC.h"
#include "QCopterFC_board.h"
#include "QCopterFC_param.h"
#include "QCopterFC_ahrs.h"
#include "QCopterFC_ctrl.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_nrf24l01.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
FSM_MODE FSM_STATE = FSM_TXRX;
SEN_MODE SEN_STATE = SEN_CORR;
/*=====================================================================================================*/
/*=====================================================================================================*/
void QCopterFC_Init( void )
{
  u8 Status = ERROR;

  /* System Setup */
  SystemInit();

  /* Device Config */
  LED_Config();
  KEY_Config();
  ADC_Config();
  RS232_Config();
  Sensor_Config();
  NRF24L01_Config();
  BLDC_Config();

  LED_R = LED_ON;

  /* Throttle Config */
  if(KEY == KEY_ON) {
    LED_B = LED_ON;
    BLDC_CtrlPWM(BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX);
    while(KEY == KEY_ON);
  }
  BLDC_CtrlPWM(BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN);
  LED_B = LED_OFF;

  Delay_10ms(5);

  /* Parameter Init */
  Param_Init(PARAM);
  // Read Flash

  /* Device Init */
  Sensor_Init();
  NRF24L01_Init(NRF_MODE_FTLR); // First TX Last RX

  // nRF Check
  do {
    Status = NRF_Check();
  } while(Status != SUCCESS);

  LED_R = LED_OFF;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void QCopterFC_Corr( u16 SystickFreq )
{
  /* Select Correct */
  LED_G = LED_ON;
  Delay_100ms(5);
  LED_G = LED_OFF;

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
  u8 UART_BUF[8] = {0};

  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_OFF;
  while(KEY != KEY_ON) {
    LED_B = !LED_B;
    UART_BUF[0] = Byte8L((s16)(AngE.Pitch*100));
    UART_BUF[1] = Byte8H((s16)(AngE.Pitch*100));
    UART_BUF[2] = Byte8L((s16)(AngE.Roll*100));
    UART_BUF[3] = Byte8H((s16)(AngE.Roll*100));
    UART_BUF[4] = Byte8L((s16)(AngE.Yaw*10));
    UART_BUF[5] = Byte8H((s16)(AngE.Yaw*10));
    UART_BUF[6] = Byte8L((s16)(Temp.TrueT*100));
    UART_BUF[7] = Byte8H((s16)(Temp.TrueT*100));
    RS232_VisualScope(UART_BUF);
    // NRF TX ONLY
//    Delay_100ms(4);
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

  /* QCopterFC FSM */
  while(1) {
//    LED_G = !LED_G;
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

#define LED_DELAY 5
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
        LED_R = LED_OFF;
        LED_G = LED_OFF;
        LED_B = LED_OFF;
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
