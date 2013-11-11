/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "module_rs232.h"
#include "module_nrf24l01.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KEY   PBI(2)
#define LED_R PCO(15)
#define LED_G PCO(14)
#define LED_B PCO(13)
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );

u8 TxBuf[SendTimes][TxBufSize] = {0};
u8 RxBuf[ReadTimes][RxBufSize] = {0};
/*=====================================================================================================*/
/*=====================================================================================================*/
int main( void )
{
  u16 i = 0;
	u8 Sta = ERROR;
  u8 FSM_Sta = 1;

	SystemInit();
	GPIO_Config();
	RS232_Config();
	nRF24L01_Config();

	while(Sta == ERROR) {
		RS232_SendStr(USART3, (u8*)"nRF24L01P CHECK ... ");
		Sta = nRF_Check();
		if(Sta == ERROR)
			RS232_SendStr(USART3, (u8*)"ERROR\r\n");
		else
			RS232_SendStr(USART3, (u8*)"SUCCESS\r\n");
	}

  RS232_SendStr(USART3, (u8*)"\r\n");

	while(1) {
		LED_G = ~LED_G;
		switch(FSM_Sta) {

/************************** FSM Tx **************************************/
			case 0:
				// FSM_Tx
        i++;
        if(i==65535)  i = 0;
        TxBuf[0][2] = Byte8L(i);
        TxBuf[0][3] = Byte8H(i);
				nRF_TX_Mode();
				do {
					Sta = nRF_Tx_Data(TxBuf[0]);
				} while(Sta == MAX_RT);
				// FSM_Tx End
				FSM_Sta = 1;
				break;

/************************** FSM Rx **************************************/
			case 1:
				// FSM_Rx
				nRF_RX_Mode();
				Sta = nRF_Rx_Data(RxBuf[0]);
				if(Sta == RX_DR) {
          
				}
				// FSM_Rx End
				FSM_Sta = 2;
				break;
/************************** FSM USART **************************************/
			case 2:
				// FSM_USART
        RS232_SendStr(USART3, (u8*)"RxDara[14] = ");
        RS232_SendNum(USART3, Type_D, 5, Byte16(RxBuf[0][15], RxBuf[0][14]));
        RS232_SendStr(USART3, (u8*)"\r\n");
        RS232_SendStr(USART3, (u8*)"RxDara[16] = ");
        RS232_SendNum(USART3, Type_D, 5, Byte16(RxBuf[0][17], RxBuf[0][16]));
        RS232_SendStr(USART3, (u8*)"\r\n\r\n");
				// FSM_USART End
				FSM_Sta = 0;
				break;
 		}
	}
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;

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

  LED_G = 1;
  LED_R = 1;
  LED_B = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*/
