/* #include "module_camspi.h" */

#ifndef __MODULE_CAMSPI_H
#define __MODULE_CAMSPI_H

#include "stm32f4xx.h"       
/*=====================================================================================================*/
/*=====================================================================================================*/
#define CAMSPI_MODE_SLAVE   0x0000
#define CAMSPI_MODE_MSATER  0x0104
#define CAMSPI_MODE         CAMSPI_MODE_SLAVE

#if (CAMSPI_MODE == CAMSPI_MODE_MSATER)
  #define CamSPI_CS PAO(15)
#elif  (CAMSPI_MODE == CAMSPI_MODE_SLAVE)
  #define CamSPI_CS PAI(15)
#endif
/*=====================================================================================================*/
/*=====================================================================================================*/
void CamSPI_Config( void );
void CamSPI_RW( u8 *WriteData, u8 *ReadData, u16 BufSize );
static void CamSPI_TimeOut( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
