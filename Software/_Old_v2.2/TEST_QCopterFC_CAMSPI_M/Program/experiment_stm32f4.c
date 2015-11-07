/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC_board.h"
#include "module_camspi.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u8 TxBuffer[128] = {0};
  u8 RxBuffer[128] = {0};

  SystemInit();
  LED_Config();
  KEY_Config();
  CamSPI_Config();

  /* Test 1 */
  while(KEY != KEY_ON) {
    LED_R = !LED_R;
    Delay_100ms(2);
  }
  do {
    TxBuffer[0] = 0xAF;
    CamSPI_RW(TxBuffer, RxBuffer, 10);
  } while(RxBuffer[0] != SUCCESS);
  TxBuffer[0] = 0;
  CamSPI_RW(TxBuffer, RxBuffer, 100);
  if(RxBuffer[0] == 0x0F && RxBuffer[99] == 0x0F)
    LED_R = 0;
  else
    LED_R = 1;
  Delay_100ms(5);

  /* Test 2 */
  while(KEY != KEY_ON) {
    LED_G = !LED_G;
    Delay_100ms(2);
  }
  do {
    TxBuffer[0] = 0xCD;
    CamSPI_RW(TxBuffer, RxBuffer, 10);
  } while(RxBuffer[0] != SUCCESS);
  TxBuffer[0] = 0;
  CamSPI_RW(TxBuffer, RxBuffer, 100);
  if(RxBuffer[0] == 0xF0 && RxBuffer[99] == 0xF0)
    LED_G = 0;
  else
    LED_G = 1;
  Delay_100ms(5);

  /* Test 3 */
  while(KEY != KEY_ON) {
    LED_B = !LED_B;
    Delay_100ms(2);
  }
  do {
    TxBuffer[0] = 0x23;
    CamSPI_RW(TxBuffer, RxBuffer, 10);
  } while(RxBuffer[0] != SUCCESS);
  TxBuffer[0] = 0;
  CamSPI_RW(TxBuffer, RxBuffer, 100);
  if(RxBuffer[0] == 0xFF && RxBuffer[99] == 0xFF)
    LED_B = 0;
  else
    LED_B = 1;
  Delay_100ms(5);

  /* Final */
  while(1) {
    LED_R = !LED_R;
    LED_G = !LED_G;
    LED_B = !LED_B;
    Delay_100ms(2);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
