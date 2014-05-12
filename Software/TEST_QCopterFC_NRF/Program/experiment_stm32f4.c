/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "experiment_stm32f4.h"
#include "module_rs232.h"
#include "module_nrf24l01.h"
#include "algorithm_string.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define NRF_MODE  NRF_MODE_FTLR
//#define NRF_MODE  NRF_MODE_FRLT
/*====================================================================================================*/
/*====================================================================================================*/
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
      // 資料寫入 TX BUF
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
      Delay_10ms(20);
      RS232_SendStr((u8*)"\f");
      RS232_SendStr((u8*)"***NRF_MODE_FTLR\r\n");
      for(i=0; i<32; i++) {
        RS232_SendStr((u8*)"RxBuf[");
        RS232_SendNum(Type_D, 2, i);
        RS232_SendStr((u8*)"] = ");
        RS232_SendNum(Type_D, 3, TxBuf[i]);
        RS232_SendStr((u8*)"\r\n");
      }
      RS232_SendStr((u8*)"\r\n");
      // FSM_USART End
      FSM_STA = 0;
      break;
  }
}
#endif
/*====================================================================================================*/
/*====================================================================================================*/
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
      RS232_SendStr((u8*)"\f");
      RS232_SendStr((u8*)"***NRF_MODE_FRLT\r\n");
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
/*====================================================================================================*/
/*====================================================================================================*/
int main( void )
{
  SystemInit();
  GPIO_Config();
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
/*====================================================================================================*/
/*====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Clk Init *************************************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED_R PC13 */  /* LED_G PC14 */  /* LED_B PC15 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* KEY PB2 */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  LED_R = 1;
  LED_G = 1;
  LED_B = 1;
}
/*====================================================================================================*/
/*====================================================================================================*/
