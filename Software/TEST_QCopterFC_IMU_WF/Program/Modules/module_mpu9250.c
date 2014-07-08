/*====================================================================================================*/
/*====================================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_spi.h"
#include "module_mpu9250.h"

#include "experiment_stm32f4.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define SPIx                  SPI2
#define SPIx_CLK              RCC_APB1Periph_SPI2

#define SPIx_CSM_PIN          GPIO_Pin_2
#define SPIx_CSM_GPIO_PORT    GPIOC
#define SPIx_CSM_GPIO_CLK     RCC_AHB1Periph_GPIOC
#define IMU_CSM               PCO(2)

#define SPIx_SCK_PIN          GPIO_Pin_13
#define SPIx_SCK_GPIO_PORT    GPIOB
#define SPIx_SCK_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE       GPIO_PinSource13
#define SPIx_SCK_AF           GPIO_AF_SPI2

#define SPIx_SDO_PIN          GPIO_Pin_14
#define SPIx_SDO_GPIO_PORT    GPIOB
#define SPIx_SDO_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SPIx_SDO_SOURCE       GPIO_PinSource14
#define SPIx_SDO_AF           GPIO_AF_SPI2

#define SPIx_SDI_PIN          GPIO_Pin_15
#define SPIx_SDI_GPIO_PORT    GPIOB
#define SPIx_SDI_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define SPIx_SDI_SOURCE       GPIO_PinSource15
#define SPIx_SDI_AF           GPIO_AF_SPI2

#define SPI_SPEED_LOW         0
#define SPI_SPEED_HIGH        1
/*====================================================================================================*/
/*====================================================================================================*/
static u8 MPU9250_Check( void );
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_ReadReg
**功能 : 讀暫存器
**輸入 : ReadAddr
**輸出 : ReadData
**使用 : MPU9250_ReadReg(ReadAddr, &DeviceID);
**====================================================================================================*/
/*====================================================================================================*/
static void MPU9250_ReadReg( u8 ReadAddr, u8 *ReadData )
{
  IMU_CSM = 0;
  SPI_RW(SPIx, 0x80 | ReadAddr);
  *ReadData = SPI_RW(SPIx, 0xFF);
  IMU_CSM = 1;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_WriteReg
**功能 : 寫暫存器
**輸入 : WriteAddr, WriteData
**輸出 : None
**使用 : MPU9250_WriteReg(WriteAddr, WriteData);
**====================================================================================================*/
/*====================================================================================================*/
static void MPU9250_WriteReg( u8 WriteAddr, u8 WriteData )
{
  IMU_CSM = 0;
  SPI_RW(SPIx, WriteAddr);
  SPI_RW(SPIx, WriteData);
  IMU_CSM = 1;
}
/*=====================================================================================================*/
/*=====================================================================================================*
**函數 : MPU9250_ReadRegs
**功能 : 連續讀暫存器
**輸入 : ReadAddr, *ReadBuf, Bytes
**輸出 : None
**使用 : MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, ReadBuf, 14);
**=====================================================================================================*/
/*=====================================================================================================*/
static void MPU9250_ReadRegs( u8 ReadAddr, u8 *ReadBuf, u8 Bytes )
{
  u8 i = 0;

  IMU_CSM = 0;
  SPI_RW(SPIx, 0x80 | ReadAddr);
  for(i=0; i<Bytes; i++)
    ReadBuf[i] = SPI_RW(SPIx, 0xFF);
  IMU_CSM = 1;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Config
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_Config();
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk Init ************************************************************/
  RCC_AHB1PeriphClockCmd(SPIx_CSM_GPIO_CLK | SPIx_SCK_GPIO_CLK |
                         SPIx_SDO_GPIO_CLK | SPIx_SDI_GPIO_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(SPIx_CLK, ENABLE);

  /* SPI AF ******************************************************************/
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_SDO_GPIO_PORT, SPIx_SDO_SOURCE, SPIx_SDO_AF);
  GPIO_PinAFConfig(SPIx_SDI_GPIO_PORT, SPIx_SDI_SOURCE, SPIx_SDI_AF);

  /* CSM PA4  */
  GPIO_InitStruct.GPIO_Pin = SPIx_CSM_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_CSM_GPIO_PORT, &GPIO_InitStruct);
  /* SCK PA5  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
  /* SDO PA6  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SDO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SDO_GPIO_PORT, &GPIO_InitStruct);
  /* SDI PA7  */
  GPIO_InitStruct.GPIO_Pin = SPIx_SDI_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPIx_SDI_GPIO_PORT, &GPIO_InitStruct);

  IMU_CSM = 1;  // 低電位有效

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // Full Duplex
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;                        // Master Mode
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;                    // Data Size 8 bit
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;                          // Transitioned On The Falling Edge
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;                         // Latched On the Rising Edge
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;                            // Software NSS Signal
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // fsck = APB2 84MHz / 256 = 328kHz
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;                   // MSB First
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx, &SPI_InitStruct);

  SPI_Cmd(SPIx, ENABLE);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_SetSpeedSPI
**功能 : 設置 SPI 速度
**輸入 : None
**輸出 : None
**使用 : MPU9250_SetSpeedSPI();
**====================================================================================================*/
/*====================================================================================================*/
static void MPU9250_SetSpeedSPI( u8 SpeedSel )
{
  SPI_InitTypeDef SPI_InitStruct;

  u16 SetSpeedSPI = (SpeedSel==SPI_SPEED_LOW) ? SPI_BaudRatePrescaler_256 : SPI_BaudRatePrescaler_4;

  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // Full Duplex
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;                        // Master Mode
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;                    // Data Size 8 bit
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;                          // Transitioned On The Falling Edge
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;                         // Latched On the Rising Edge
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;                            // Software NSS Signal
  SPI_InitStruct.SPI_BaudRatePrescaler = SetSpeedSPI;               // SpeedSel=0 -> LS, SpeedSel!=0 -> HS
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;                   // MSB First
  SPI_InitStruct.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx, &SPI_InitStruct);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Init
**功能 : 初始化 MPU9250
**輸入 : None
**輸出 : None
**使用 : MPU9250_Init();
**====================================================================================================*/
/*====================================================================================================*/
#define MPU9250_InitRegNum 9
void MPU9250_Init( MPU_InitTypeDef *MPUx )
{
  u8 i = 0;

  u8 MPU6500_Init_Data[MPU9250_InitRegNum][2] = {
      {0x10, MPU6500_USER_CTRL},      // 
      {0x00, MPU6500_PWR_MGMT_1},     // 
      {0x00, MPU6500_PWR_MGMT_2},     // Enable Acc & Gyro
      {0x07, MPU6500_CONFIG},         // DLPF_CFG[2:0] = 111;
      {0x18, MPU6500_GYRO_CONFIG},    // +-2000dps
      {0x08, MPU6500_ACCEL_CONFIG},   // +-4G
      {0x48, MPU6500_ACCEL_CONFIG_2}, // Set Acc Data Rates
      {0x30, MPU6500_INT_PIN_CFG},    // 
      {0x4D, MPU6500_I2C_MST_CTRL},   // I2C Speed 400 kHz

//      // Set Slave to Read AK8963
//      {0x8C, MPU6500_I2C_SLV0_ADDR},  // AK8963_I2C_ADDR ( 7'b000_1100 )
//      {0x00, MPU6500_I2C_SLV0_REG},   // AK8963_WIA ( 0x00 )
//      {0x81, MPU6500_I2C_SLV0_CTRL},  // Enable
//      {0x01, MPU6500_I2C_MST_DELAY_CTRL}
    };

  MPU6500_Init_Data[4][0] = MPUx->MPU_Gyr_FullScale;      // MPU6500_GYRO_CONFIG
  MPU6500_Init_Data[3][0] = MPUx->MPU_Gyr_LowPassFilter;  // MPU6500_CONFIG
  MPU6500_Init_Data[5][0] = MPUx->MPU_Acc_FullScale;      // MPU6500_ACCEL_CONFIG
  MPU6500_Init_Data[6][0] = MPUx->MPU_Acc_LowPassFilter;  // MPU6500_ACCEL_CONFIG_2
LED_B = 0;
  for(i=0; i<MPU9250_InitRegNum; i++) {
    MPU9250_WriteReg(MPU6500_Init_Data[i][1], MPU6500_Init_Data[i][0]);
    LED_G = 0;
    Delay_1ms(5);
  }

  while(MPU9250_Check() != SUCCESS);

  MPU9250_SetSpeedSPI(SPI_SPEED_HIGH);
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Check
**功能 : MPU9250 Check
**輸入 : None
**輸出 : Status
**使用 : Status = MPU9250_Check();
**====================================================================================================*/
/*====================================================================================================*/
static u8 MPU9250_Check( void )
{
  u8 DeviceID = 0x00;

  /* MPU6500 */
  MPU9250_ReadReg(MPU6500_WHO_AM_I, &DeviceID);
  if(DeviceID != MPU6500_Device_ID)
    return ERROR;

//  /* AK8963 */
//  MPU9250_WriteReg(MPU6500_I2C_SLV0_ADDR, 0x8C);          // Set AK8963_I2C_ADDR = 7'b000_1100
//  Delay_1us(10);
//  MPU9250_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_WIA);     // Set Write Reg
//  MPU9250_WriteReg(MPU6500_I2C_SLV0_CTRL, 0x81);          // Start Read
//  Delay_1ms(1);
//  MPU9250_ReadReg(MPU6500_EXT_SENS_DATA_00, &DeviceID);   // Read Data
//  if(DeviceID != AK8963_Device_ID)
//    return ERROR;

  return SUCCESS;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_Read
**功能 : 讀取感測器資料
**輸入 : *ReadBuf
**輸出 : None
**使用 : MPU9250_Read(ReadBuf);
**====================================================================================================*/
/*====================================================================================================*/
void MPU9250_Read( u8 *ReadBuf )
{
  MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, ReadBuf, 14);
}
/*====================================================================================================*/
/*====================================================================================================*/
