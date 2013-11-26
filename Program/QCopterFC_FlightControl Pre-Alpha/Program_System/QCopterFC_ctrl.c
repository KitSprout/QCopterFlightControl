/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC_ctrl.h"
#include "QCopterFC_transport.h"
#include "module_board.h"
#include "module_motor.h"
#include "algorithm_pid.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
vs16 PWM_M1 = PWM_MOTOR_MIN;
vs16 PWM_M2 = PWM_MOTOR_MIN;
vs16 PWM_M3 = PWM_MOTOR_MIN;
vs16 PWM_M4 = PWM_MOTOR_MIN;
/*=====================================================================================================*/
/*=====================================================================================================*/
void CTRL_FlightControl( void )
{
  u16 TempM1 = 0;
  u16 TempM2 = 0;
  u16 TempM3 = 0;
  u16 TempM4 = 0;
  u16 TempThr = 0;

  if( KEYR_L == 0 ) {
    TempM1 = PWM_MOTOR_MIN;
    TempM2 = PWM_MOTOR_MIN;
    TempM3 = PWM_MOTOR_MIN;
    TempM4 = PWM_MOTOR_MIN;
    PWM_M1 = TempM1;
    PWM_M2 = TempM2;
    PWM_M3 = TempM3;
    PWM_M4 = TempM4;
  }
  else if( KEYL_J == 0 ) {
    TempThr = PWM_MOTOR_MIN + JSR_Z*0.30379746835443037974683544303797f - 70;
    if(TempThr<PWM_MOTOR_MIN)
      TempThr = PWM_MOTOR_MIN;
    if(TempThr>PWM_MOTOR_MAX)
      TempThr = PWM_MOTOR_MAX;

    TempM1 = TempThr;
    TempM2 = TempThr;
    TempM3 = TempThr;
    TempM4 = TempThr;
    PWM_M1 = TempM1;
    PWM_M2 = TempM2;
    PWM_M3 = TempM3;
    PWM_M4 = TempM4;
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
