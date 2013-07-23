/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
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
  I2C_Config();
  Motor_Config();
  nRF24L01_Config();

  Delay_10ms(2);
}
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
	u8 Sta = ERROR;
  u8 FSM_Sta = FSM_Rx;

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
  Sensor_Init();
  Delay_10ms(2);

  /* Systick Config */
  if(SysTick_Config(420000)) {    // 168MHz / 420000 = 400Hz = 2.5ms
    while(1);
  }

  /* Wait Correction */
  while(SensorMode == Mode_OffSet || SensorMode == Mode_Magnetic);

  /* Lock */
  while(!KEY){
    LED_R = ~LED_R;
    Delay_100ms(5);
  }
  LED_R = 1;

  /* Final State Machine */
	while(1) {
		LED_G = ~LED_G;
		switch(FSM_Sta) {

/************************** FSM Tx ****************************************/
			case FSM_Tx:
				// FSM_Tx
				nRF_TX_Mode();
				do {
					Sta = nRF_Tx_Data(TxBuf[0]);
				} while(Sta == MAX_RT);
				// FSM_Tx End
				FSM_Sta = FSM_Rx;
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
				FSM_Sta = FSM_CTRL;
				break;

/************************** FSM CTRL **************************************/
			case FSM_CTRL:
				// FSM_CTRL
        CTRL_FlightControl();
				// FSM_CTRL End
				FSM_Sta = FSM_UART;
				break;

/************************** FSM UART ***************************************/
			case FSM_UART:
				// FSM_USART
        RS232_VisualScope(USART3, TxBuf[0]+2, 8);
				// FSM_USART End
				FSM_Sta = FSM_DATA;
				break;

/************************** FSM DATA **************************************/
			case FSM_DATA:
				// FSM_DATA
        Transport_Send(TxBuf[0]);
				// FSM_DATA End
				FSM_Sta = FSM_Tx;
				break;
		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
