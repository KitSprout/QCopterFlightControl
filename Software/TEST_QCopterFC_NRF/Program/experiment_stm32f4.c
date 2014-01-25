/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC_board.h"
#include "module_nrf24l01.h"
#include "module_rs232.h"
#include "algorithm_string.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define NRF_MODE  NRF_MODE_FTLR
//#define NRF_MODE  NRF_MODE_FRLT
/*=====================================================================================================*/
/*=====================================================================================================*/
#if (NRF_MODE == NRF_MODE_FTLR)
void NRF_FTLR( void ) // First Tx Last Rx
{
  u8 i = 0;
  u8 Sta = ERROR;

  static u8 FSM_STA = 0;

  switch(FSM_STA) {

    /************************** FSM Tx **************************************/
    case 0:
      // FSM_Tx
      for(i=0; i<32; i++) {
        TxBuf[i] = RxBuf[i] + i;
        if(TxBuf[i]>220)
          TxBuf[i] = 0;
      }
      do {
        Sta = NRF_TxPacket(TxBuf);
      } while(Sta == NRF_STA_MAX_RT);
      // FSM_Tx End
      FSM_STA = 1;
      break;

    /************************** FSM Rx **************************************/
    case 1:
      // FSM_Rx
      NRF_RX_Mode();
      Sta = NRF_RxPacket(RxBuf);
      if(Sta == NRF_STA_RX_DR) {
        LED_B = !LED_B;
      }
      // FSM_Rx End
      FSM_STA = 2;
      break;

    /************************** FSM USART **************************************/
    case 2:
      // FSM_USART
//      Delay_10ms(18);
//      RS232_SendStr((u8*)"\f");
      for(i=0; i<1; i++) {
        RS232_SendStr((u8*)"RxBuf[");
        RS232_SendNum(Type_D, 2, i);
        RS232_SendStr((u8*)"] = ");
        RS232_SendNum(Type_D, 3, RxBuf[i]);
        RS232_SendStr((u8*)"\r\n");
      }
      RS232_SendStr((u8*)"\r\n");
      // FSM_USART End
      FSM_STA = 0;
      break;
  }
}
#endif
/*=====================================================================================================*/
/*=====================================================================================================*/
#if (NRF_MODE == NRF_MODE_FRLT)
void NRF_FRLT( void )   // First Rx Last Tx
{
  u8 i = 0;
  u8 Sta = ERROR;

  static u8 FSM_STA = 1;

  switch(FSM_STA) {

    /************************** FSM Tx **************************************/
    case 0:
      // FSM_Tx
      for(i=0; i<32; i++)
        TxBuf[i] = RxBuf[i];
      do {
        Sta = NRF_TxPacket(TxBuf);
      } while(Sta == NRF_STA_MAX_RT);
      // FSM_Tx End
      FSM_STA = 1;
      break;

    /************************** FSM Rx **************************************/
    case 1:
      // FSM_Rx
      NRF_RX_Mode();
      Sta = NRF_RxPacket(RxBuf);
      if(Sta == NRF_STA_RX_DR) {
        LED_B = !LED_B;
      }
      // FSM_Rx End
      FSM_STA = 2;
      break;

    /************************** FSM USART **************************************/
    case 2:
      // FSM_USART
//      RS232_SendStr((u8*)"\f");
      for(i=0; i<32; i++) {
        RS232_SendStr((u8*)"RxBuf[");
        RS232_SendNum(Type_D, 2, i);
        RS232_SendStr((u8*)"] = ");
        RS232_SendNum(Type_D, 3, RxBuf[i]);
        RS232_SendStr((u8*)"\r\n");
      }
      RS232_SendStr((u8*)"\r\n");
      // FSM_USART End
      FSM_STA = 0;
      break;
  }
}
#endif
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  SystemInit();
  LED_Config();
  KEY_Config();
  RS232_Config();
  NRF24L01_Config();

#if (NRF_MODE == NRF_MODE_FTLR)
  NRF24L01_Init(NRF_MODE_FTLR);

  while(1) {
    LED_G = !LED_G;
    NRF_FTLR();
  }
#endif

#if (NRF_MODE == NRF_MODE_FRLT)
  NRF24L01_Init(NRF_MODE_FRLT);

  while(1) {
    LED_R = !LED_R;
    NRF_FRLT();
  }
#endif
}
/*=====================================================================================================*/
/*=====================================================================================================*/
