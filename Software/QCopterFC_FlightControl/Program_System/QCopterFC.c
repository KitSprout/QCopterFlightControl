/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC.h"
#include "QCopterFC_board.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_ahrs.h"
#include "QCopterFC_transport.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_nrf24l01.h"
#include "algorithm_pid.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
void VisualScope_Send( USART_TypeDef* USARTx );
/*=====================================================================================================*/
/*=====================================================================================================*/
void QCopterFC_Init( void )
{
  u8 Sta = ERROR;

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

  PID_Pitch.Kp = +1.5f;
  PID_Pitch.Ki = +0.002f;
  PID_Pitch.Kd = +1.0f;

  PID_Roll.Kp  = +1.5f;
  PID_Roll.Ki  = +0.002f;
  PID_Roll.Kd  = +1.0f;

  PID_Yaw.Kp   = +0.0f;
  PID_Yaw.Ki   = +0.0f;
  PID_Yaw.Kd   = +0.0f;

  RF_SendData.Packet = 0x00;

  /* Throttle Config */
  if(KEY == KEY_ON) {
    LED_B = 0;
    BLDC_CtrlPWM(BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX, BLDC_PWM_MAX);
  }
  while(KEY == KEY_ON);
  LED_B = 1;
  BLDC_CtrlPWM(BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN, BLDC_PWM_MIN);

  /* nRF Check */
  while(Sta == ERROR)
    Sta = nRF_Check();

  Delay_10ms(10);

  /* Sensor Init */
  if(Sensor_Init() == SUCCESS)
    LED_G = 0;

  Delay_10ms(10);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u8 i = 0;
  u8 Sta = 0;

  FSM_Mode FSM_State = FSM_Rx;

  /* System Init */
  QCopterFC_Init();

  /* Systick Config */
  if(SysTick_Config(SystemCoreClock/SampleRateFreg)) {  // SampleRateFreg = 500 Hz
    while(1);
  }

  /* Wait Correction */
  while(SensorMode != Mode_Algorithm);

  /* Lock */
  LED_R = 1;
  LED_G = 1;
  LED_B = 1;
  while(KEY != KEY_ON) {
    LED_B = ~LED_B;
    Delay_10ms(1);
    VisualScope_Send(USART3);
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
      for(i=0; i<3; i++) {
        RF_SendData.Packet++;
        Transport_Send(TxBuf);
        do {
          Sta = nRF_Tx_Data(TxBuf);
        } while(Sta == MAX_RT);
      }
      RF_SendData.Packet = 0x00;
      // FSM_Tx End
      FSM_State = FSM_Rx;
      break;

    /************************** FSM Rx ****************************************/
    case FSM_Rx:
      // FSM_Rx
      nRF_RX_Mode();
      Sta = nRF_Rx_Data(RxBuf);
      if(Sta == RX_DR) {
        Transport_Recv(RxBuf);
      }
      // FSM_Rx End
      FSM_State = FSM_CTRL;
      break;

    /************************** FSM CTRL **************************************/
    case FSM_CTRL:
      // FSM_CTRL
      Ctrl_BasicThr();
      // FSM_CTRL End
      FSM_State = FSM_UART;
      break;

    /************************** FSM UART ***************************************/
    case FSM_UART:
      // FSM_USART
      RS232_VisualScope(USART3, TxBuf+20, 8);
      // FSM_USART End
      FSM_State = FSM_DATA;
      break;

    /************************** FSM DATA **************************************/
    case FSM_DATA:
      // FSM_DATA

      // FSM_DATA End
      FSM_State = FSM_Tx;
      break;
    }
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void VisualScope_Send( USART_TypeDef* USARTx )
{
  u8 VisualScopeBuf[8] = {0};

  RF_SendData.Ang.X = (s16)(AngE.Pitch*100);  // 10 mdeg/LSB
  RF_SendData.Ang.Y = (s16)(AngE.Roll*100);   // 10 mdeg/LSB
  RF_SendData.Ang.Z = (s16)(AngE.Yaw*10);     // 100 mdeg/LSB

  VisualScopeBuf[0] = (u8)Byte8L(RF_SendData.Ang.X);
  VisualScopeBuf[1] = (u8)Byte8H(RF_SendData.Ang.X);
  VisualScopeBuf[2] = (u8)Byte8L(RF_SendData.Ang.Y);
  VisualScopeBuf[3] = (u8)Byte8H(RF_SendData.Ang.Y);
  VisualScopeBuf[4] = (u8)Byte8L(RF_SendData.Ang.Z);
  VisualScopeBuf[5] = (u8)Byte8H(RF_SendData.Ang.Z);
  VisualScopeBuf[6] = (u8)(0);
  VisualScopeBuf[7] = (u8)(0);

  RS232_VisualScope(USARTx, VisualScopeBuf, 8);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
