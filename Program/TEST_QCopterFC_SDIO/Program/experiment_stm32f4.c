/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "stm32f4_sdio.h"
#include "module_rs232.h"
#include "ff.h"
#include "diskio.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KEY   PBI(2)
#define LED_R PCO(15)
#define LED_G PCO(14)
#define LED_B PCO(13)
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
FATFS FatFs;
FRESULT res;
FILINFO finfo;
DIR dirs;
FIL file;

int main( void )
{
  u32 i = 0;
  u8 ReadBuf[128] = {0};
  u8 TrData[8][32] = {0};
  u8 WriteData[50] = "ABCDEFG";

  SystemInit();
  GPIO_Config();
  RS232_Config();

  LED_R = 1;
  LED_G = 1;

  // SD Card Init Info
  RS232_SendStr(USART3, (u8*)" \r\n");
  RS232_SendStr(USART3, (u8*)"----------------------\r\n");
  RS232_SendStr(USART3, (u8*)"----------------------\r\n");
  RS232_SendStr(USART3, (u8*)" SDIO SD FatFs demo\r\n");
  RS232_SendStr(USART3, (u8*)"----------------------\r\n");
  RS232_SendStr(USART3, (u8*)"----------------------\r\n\r\n");
  RS232_SendStr(USART3, (u8*)" SD Init ... ");
  while(SD_Init() != SD_OK) {
    RS232_SendStr(USART3, (u8*)"Failed!!\r\n");
    while(1) {
      LED_R = ~LED_R;
      Delay_100ms(2);
    }
  }
  RS232_SendStr(USART3, (u8*)"OK!!\r\n\r\n");

  RS232_SendStr(USART3, (u8*)"-----SD Init Info-----\r\n");

  RS232_SendStr(USART3, (u8*)" Capacity : ");
  NumToChar(Type_D, 5, TrData[0], SDCardInfo.CardCapacity>>20);
  RS232_SendStr(USART3, TrData[0]);
  RS232_SendStr(USART3, (u8*)" MB\r\n");

  RS232_SendStr(USART3, (u8*)" CardBlockSize : ");
  NumToChar(Type_D, 5, TrData[1], SDCardInfo.CardBlockSize);
  RS232_SendStr(USART3, TrData[1]);
  RS232_SendStr(USART3, (u8*)"\r\n");

  RS232_SendStr(USART3, (u8*)" CardType : ");
  NumToChar(Type_D, 5, TrData[2], SDCardInfo.CardType);
  RS232_SendStr(USART3, TrData[2]);
  RS232_SendStr(USART3, (u8*)"\r\n");

  RS232_SendStr(USART3, (u8*)" RCA : ");
  NumToChar(Type_D, 5, TrData[3], SDCardInfo.RCA);
  RS232_SendStr(USART3, TrData[3]);
  RS232_SendStr(USART3, (u8*)"\r\n");
  RS232_SendStr(USART3, (u8*)"----------------------\r\n");
  RS232_SendStr(USART3, (u8*)"\r\n");

  // Wait
  while(KEY != 1){
    LED_G = ~LED_G;
    Delay_100ms(2);
  }

  // Read Directory File
  RS232_SendStr(USART3, (u8*)"----------------------\r\n");
  RS232_SendStr(USART3, (u8*)" SD_Card Read Directory File\r\n");
  RS232_SendStr(USART3, (u8*)"----------------------\r\n\r\n");

  res = f_mount(&FatFs, "", 1);
  res = f_opendir(&dirs, "0:/");
  res = f_readdir(&dirs, &finfo);
  while(res!= FR_OK) {
    RS232_SendStr(USART3, (u8*)" FatFs failed!!\r\n");
    while(1) {
      LED_R = ~LED_R;
      Delay_100ms(2);
    }
  }
  RS232_SendStr(USART3, (u8*)" File name : \r\n");

  while(finfo.fname[0]) {
    f_readdir(&dirs, &finfo);
    if(!finfo.fname[0])
      break;
    RS232_SendStr(USART3, (u8*)" ... ");
    RS232_SendStr(USART3, (u8*)finfo.fname);
    RS232_SendStr(USART3, (u8*)"\r\n");
  }

  // Wait
  while(KEY != 1){
    LED_G = ~LED_G;
    Delay_100ms(2);
  }

  // Write File
  RS232_SendStr(USART3, (u8*)" f_open ... ");
  res = f_open(&file,"SDCard_K.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
  if(res==FR_OK)
    RS232_SendStr(USART3, (u8*)"OK!!\r\n");
  else
    RS232_SendStr(USART3, (u8*)"failed!!\r\n");

  RS232_SendStr(USART3, (u8*)" f_write ... ");
  res = f_write(&file, WriteData, 20, &i); 
  if(res==FR_OK)
    RS232_SendStr(USART3, (u8*)"OK!!\r\n");
  else
    RS232_SendStr(USART3, (u8*)"failed!!\r\n");

  file.fptr = 0;

  RS232_SendStr(USART3, (u8*)" f_read ... ");
  res = f_read(&file, ReadBuf, 100, &i);
  if(res==FR_OK)
    RS232_SendStr(USART3, (u8*)"OK!!\r\n");
  else
    RS232_SendStr(USART3, (u8*)"failed!!\r\n");

  f_close(&file);

  RS232_SendStr(USART3, ReadBuf);

  while(1) {
    LED_G = ~LED_G;
    Delay_100ms(2);
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* LED_R PC15 */  /* LED_G PC14 */  /* LED_B PC13 */
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
/*=====================================================================================================*/
/*=====================================================================================================*/
