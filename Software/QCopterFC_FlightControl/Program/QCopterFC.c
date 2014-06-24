/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_board.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_transport.h"
#include "module_rs232.h"
#include "module_nrf24l01.h"
#include "algorithm_pid.h"
#include "algorithm_ahrs.h"
/*====================================================================================================*/
/*====================================================================================================*/
FSM_MODE FSM_STATE = FSM_TXRX;
SEN_MODE SEN_STATE = SEN_CORR;
/*====================================================================================================*/
/*====================================================================================================*/
void QCopterFC_Init( void )
{
  /* System Setup */
  SystemInit();

  /* Device Config */
  Board_Config();

  LED_R = LED_ON;

  /* ESC Setting */
  if(KEY == KEY_ON) {
    LED_B = LED_ON;
    BLDC_CtrlPWM(BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX);
    while(KEY == KEY_ON);
  }
  BLDC_CtrlPWM(BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN);
  LED_B = LED_OFF;

  Delay_100ms(1);

  /* Parameter Init */
  // ****Read Flash

  PID_Init(&PID_Pitch, +1.500f, +0.000f, +1.000f);
  PID_Init(&PID_Roll,  +1.500f, +0.000f, +1.000f);
  PID_Init(&PID_Yaw,   +0.000f, +0.000f, +0.000f);

  /* Device Init */
  Board_Init();
  Delay_100ms(5);

  LED_R = LED_OFF;
}
/*====================================================================================================*/
/*====================================================================================================*/
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
    UART_BUF[0] = Byte8L((s16)(AngE.Pitch*100));  // 0.01 deg/LSB
    UART_BUF[1] = Byte8H((s16)(AngE.Pitch*100));  // 0.01 deg/LSB
    UART_BUF[2] = Byte8L((s16)(AngE.Roll*100));   // 0.01 deg/LSB
    UART_BUF[3] = Byte8H((s16)(AngE.Roll*100));   // 0.01 deg/LSB
    UART_BUF[4] = Byte8L((s16)(AngE.Yaw*10));     // 0.1  deg/LSB
    UART_BUF[5] = Byte8H((s16)(AngE.Yaw*10));     // 0.1  deg/LSB
    RS232_VisualScope(UART_BUF);
  }
  LED_R = LED_OFF;
  LED_G = LED_OFF;
  LED_B = LED_OFF;
}
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  u8 Status = 0;

  /* QCopterFC Init */
  QCopterFC_Init();

  /* QCopterFC Corr */
  QCopterFC_Corr(SampleRateFreg);   // SampleRateFreg = 500 Hz

  /* QCopterFC Lock */
  QCopterFC_Lock();

  /* QCopterFC FSM */
  while(1) {
    LED_G = !LED_G;
    switch(FSM_STATE) {

      /************************** FSM TXRX ****************************************/
      case FSM_TXRX:
        // FSM_TXRX
        Transport_Send(TxBuf);
        do {
          Status = NRF_TxPacket(TxBuf);
        } while(Status == NRF_STA_MAX_RT);
        NRF_RX_Mode();
        Status = NRF_RxPacket(RxBuf);
        if(Status == NRF_STA_RX_DR) {
          Transport_Recv(RxBuf);
        }
        // FSM_TXRX End
        FSM_STATE = FSM_CTRL;
        break;

      /************************** FSM CTRL ****************************************/
      case FSM_CTRL:
        // FSM_CTRL
        Ctrl_BasicThr();
        // FSM_CTRL End
        FSM_STATE = FSM_UART;
        break;

      /************************** FSM UART ****************************************/
      case FSM_UART:
        // FSM_UART
        Delay_1ms(10);
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
          Delay_100ms(10);
        }
    }
  }
}
/*====================================================================================================*/
/*====================================================================================================*/
