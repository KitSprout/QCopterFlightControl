/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_transport.h"
#include "module_board.h"
#include "module_rs232.h"
#include "module_motor.h"
#include "module_sensor.h"
#include "module_nrf24l01.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void System_Init( void )
{
  SystemInit();

  LED_Config();
  KEY_Config();
  RS232_Config();
  Motor_Config();
  Sensor_Config();
  nRF24L01_Config();

  PID_Init(&PID_Yaw);
  PID_Init(&PID_Roll);
  PID_Init(&PID_Pitch);

  PID_Pitch.Kp = +3.5f;
  PID_Pitch.Ki = +0.004f;
  PID_Pitch.Kd = +4.0f;

  PID_Roll.Kp  = +3.5f;
  PID_Roll.Ki  = +0.004f;
  PID_Roll.Kd  = +4.0f;

  PID_Yaw.Kp   = +0.0f;
  PID_Yaw.Ki   = +0.0f;
  PID_Yaw.Kd   = +0.25f;

  Delay_10ms(2);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u8 Sta = ERROR;
  FSM_Mode FSM_State = FSM_Rx;

  /* System Init */
  System_Init();

  /* Throttle Config */
  if(KEY == 1) {
    LED_B = 0;
    Motor_Control(PWM_MOTOR_MAX, PWM_MOTOR_MAX, PWM_MOTOR_MAX, PWM_MOTOR_MAX);
  }
  while(KEY == 1);
  LED_B = 1;
  Motor_Control(PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN, PWM_MOTOR_MIN);

  /* nRF Check */
  while(Sta == ERROR)
    Sta = nRF_Check();

  /* Sensor Init */
  if(Sensor_Init() == SUCCESS)
    LED_G = 0;
  Delay_10ms(10);

  /* Systick Config */
  if(SysTick_Config(420000)) {    // 168MHz / 420000 = 400Hz = 2.5ms
    while(1);
  }

  /* Wait Correction */
  while(SensorMode != Mode_Algorithm);

  /* Lock */
  LED_R = 1;
  LED_G = 1;
  LED_B = 1;
  while(!KEY) {
    LED_B = ~LED_B;
    Delay_10ms(1);
    Transport_Send(TxBuf[0]);
    RS232_VisualScope(USART3, TxBuf[0]+20, 8);
  }
  LED_B = 1;

  /* Final State Machine */
  while(1) {
    LED_G = ~LED_G;
    switch(FSM_State) {

    /************************** FSM Tx ****************************************/
    case FSM_Tx:
      // FSM_Tx
      nRF_TX_Mode();
      do {
        Sta = nRF_Tx_Data(TxBuf[0]);
      } while(Sta == MAX_RT);
      // FSM_Tx End
      FSM_State = FSM_Rx;
      break;

    /************************** FSM Rx ****************************************/
    case FSM_Rx:
      // FSM_Rx
      nRF_RX_Mode();
      Sta = nRF_Rx_Data(RxBuf[0]);
      if(Sta == RX_DR) {
        Transport_Recv(RxBuf[0]);
      }
      // FSM_Rx End
      FSM_State = FSM_CTRL;
      break;

    /************************** FSM CTRL **************************************/
    case FSM_CTRL:
      // FSM_CTRL
      CTRL_FlightControl();
      // FSM_CTRL End
      FSM_State = FSM_UART;
      break;

    /************************** FSM UART ***************************************/
    case FSM_UART:
      // FSM_USART
      RS232_VisualScope(USART3, TxBuf[0]+2, 8);
      // FSM_USART End
      FSM_State = FSM_DATA;
      break;

    /************************** FSM DATA **************************************/
    case FSM_DATA:
      // FSM_DATA
      Transport_Send(TxBuf[0]);
      // FSM_DATA End
      FSM_State = FSM_Tx;
      break;
    }
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
