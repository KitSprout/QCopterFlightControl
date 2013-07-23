/*=====================================================================================================*/
/*=====================================================================================================*/
#include <stdlib.h>
#include "stm32f4_system.h"
#include "stm32f4_usart.h"
#include "stm32f4_i2c.h"
#include "stm32f4_sdio.h"
#include "module_rs232.h"
#include "module_sensor.h"
#include "module_mpu6050.h"
#include "module_hmc5883.h"
#include "module_ms5611.h"

#include "ff.h"
#include "diskio.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define KEY   PBI(2)
#define LED_R PCO(15)
#define LED_G PCO(14)
#define LED_B PCO(13)

#define SD_BUF_SIZE 128
/*=====================================================================================================*/
/*=====================================================================================================*/
void GPIO_Config( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
vu16* SD_Buf = 0;
vu8 SD_Flag = 0;
/*=====================================================================================================*/
/*=====================================================================================================*/
FATFS fatfs;
FRESULT res;
FILINFO finfo;
DIR dirs;
FIL file;
	
int main( void )
{
  u32 i = 0;
  u8 TrData[16][32] = {0};
  u8 SD_Buf_Info[16] = {0};
  s16 SD_WriteBuf[SD_BUF_SIZE] = {0};

	SystemInit();
	GPIO_Config();
	RS232_Config();
 	I2C_Config();

  LED_R = 1;
  LED_G = 1;

  MPU6050_Init();
  HMC5883_Init();

	SD_Buf = (vu16*)malloc(SD_BUF_SIZE*sizeof(vu16));

	// SD Card Init Info
  RS232_Print(USART3, (u8*)" \r\n");
  RS232_Print(USART3, (u8*)"----------------------\r\n");
  RS232_Print(USART3, (u8*)"----------------------\r\n");
  RS232_Print(USART3, (u8*)" SDIO SD Fatfs demo\r\n");
  RS232_Print(USART3, (u8*)"----------------------\r\n");
  RS232_Print(USART3, (u8*)"----------------------\r\n\r\n");
  RS232_Print(USART3, (u8*)" SD Init ... ");
  while(SD_Init() != SD_OK) {
    RS232_Print(USART3, (u8*)"Failed!!\r\n");
    while(1) {
      LED_R = ~LED_R;
      Delay_100ms(2);
    }
  }
  RS232_Print(USART3, (u8*)"OK!!\r\n\r\n");
  RS232_Print(USART3, (u8*)"-----SD Init Info-----\r\n");
  RS232_Print(USART3, (u8*)" Capacity : ");
  NumToChar(Type_D, 5, TrData[0], SDCardInfo.CardCapacity>>20);
  RS232_Print(USART3, TrData[0]);
  RS232_Print(USART3, (u8*)" MB\r\n");
  RS232_Print(USART3, (u8*)" CardBlockSize : ");
  NumToChar(Type_D, 5, TrData[1], SDCardInfo.CardBlockSize);
  RS232_Print(USART3, TrData[1]);
  RS232_Print(USART3, (u8*)"\r\n");
  RS232_Print(USART3, (u8*)" CardType : ");
  NumToChar(Type_D, 5, TrData[2], SDCardInfo.CardType);
  RS232_Print(USART3, TrData[2]);
  RS232_Print(USART3, (u8*)"\r\n");
  RS232_Print(USART3, (u8*)" RCA : ");
  NumToChar(Type_D, 5, TrData[3], SDCardInfo.RCA);
  RS232_Print(USART3, TrData[3]);
  RS232_Print(USART3, (u8*)"\r\n");
  RS232_Print(USART3, (u8*)"----------------------\r\n");
  RS232_Print(USART3, (u8*)"\r\n");

	// Wait
  while(KEY != 1){
    LED_G = ~LED_G;
    Delay_100ms(2);
  }

	// Open File
  RS232_Print(USART3, (u8*)"----------------------\r\n");
  RS232_Print(USART3, (u8*)" SD_Card Open File\r\n");
  RS232_Print(USART3, (u8*)"----------------------\r\n\r\n");

  res = f_mount(0, &fatfs);
  res = f_opendir(&dirs, "0:/");

  RS232_Print(USART3, (u8*)" f_open ... ");
  res = f_open(&file, "BlackBox.dat", FA_OPEN_EXISTING | FA_READ | FA_WRITE);

//   SD_Buf_Info[0] = 120;
//   SD_Buf_Info[2] = 'H';
//   SD_Buf_Info[3] = 'z';
//   SD_Buf_Info[6] = 'S';
//   SD_Buf_Info[7] = 'i';
//   SD_Buf_Info[8] = 'z';
//   SD_Buf_Info[9] = 'e';

  if(res==FR_OK) {
    RS232_Print(USART3, (u8*)"OK!!\r\n");
    f_lseek(&file, 0);
    res = f_write(&file, SD_Buf_Info, 16, &i);
  }
  else if(res==FR_NO_FILE) {
    f_open(&file, "BlackBox.dat", FA_CREATE_NEW | FA_READ | FA_WRITE);
    RS232_Print(USART3, (u8*)"Create New File\r\n");
    res = f_write(&file, SD_Buf_Info, 16, &i);
    if(res==FR_OK)
      RS232_Print(USART3, (u8*)"OK!!\r\n");
    else
      RS232_Print(USART3, (u8*)"failed!!\r\n");
  }
  else
    RS232_Print(USART3, (u8*)"failed!!\r\n");

  f_close(&file);

	// Wait
  while(KEY != 1){
    LED_G = ~LED_G;
    Delay_100ms(2);
  }

  if(SysTick_Config(420000)) {		// 168MHz / 420000 = 400Hz = 2.5ms
		while(1);
	}

  while(1) {
    if(SD_Flag==1) {
			
      SD_Flag = 0;

      for(i=0; i<SD_BUF_SIZE; i++)
        SD_WriteBuf[i] = SD_Buf[i];

      RS232_Print(USART3, (u8*)"\f");
      RS232_Print(USART3, (u8*)" --->Size : ");
      NumToChar(Type_D, 8, TrData[0], file.fsize);
      RS232_Print(USART3, TrData[0]);
      RS232_Print(USART3, (u8*)"\r\n");

      f_open(&file, "BlackBox.dat", FA_OPEN_EXISTING | FA_READ | FA_WRITE);
      f_lseek(&file , file.fsize);
      f_write(&file, SD_WriteBuf, SD_BUF_SIZE, &i);
      f_close(&file);

      RS232_Print(USART3, (u8*)" Acc_X : ");
      NumToChar(Type_I, 5, TrData[0], SD_WriteBuf[0]);
      RS232_Print(USART3, TrData[0]);
      RS232_Print(USART3, (u8*)"\r\n");
    }
    LED_G = ~LED_G;
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
