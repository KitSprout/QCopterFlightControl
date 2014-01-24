/*=====================================================================================================*/
/*=====================================================================================================*/
#include "stm32f4_system.h"
#include "QCopterFC_board.h"
#include "module_camspi.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
int main(void)
{
  u8 CAM_CMD = 0x00;

  u8 TxBuffer[100] = {0};
  u8 RxBuffer[100] = {0};

  SystemInit();

  CamSPI_Config();

  while(1) {
    switch(CAM_CMD) {
      case 0xAF:
        TxBuffer[0]  = 0x0F;
        TxBuffer[99] = 0x0F;
        CamSPI_RW(TxBuffer, RxBuffer, 100);
        break;
      case 0xCD:
        TxBuffer[0]  = 0xF0;
        TxBuffer[99] = 0xF0;
        CamSPI_RW(TxBuffer, RxBuffer, 100);
        break;
      case 0x23:
        TxBuffer[0]  = 0xFF;
        TxBuffer[99] = 0xFF;
        CamSPI_RW(TxBuffer, RxBuffer, 100);
        break;
      case 0x48:
        TxBuffer[0]  = 0x00;
        TxBuffer[99] = 0x00;
        CamSPI_RW(TxBuffer, RxBuffer, 100);
        break;
      default:
        TxBuffer[0] = SUCCESS;
        CamSPI_RW(TxBuffer, RxBuffer, 10);
        break;
    }
    CAM_CMD = RxBuffer[0];
  }
}
/*=====================================================================================================*/
/*=====================================================================================================*/
