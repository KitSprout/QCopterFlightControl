/*==============================================================================================*/
/*==============================================================================================*/
#include "stm32f4_system.h"
#include "stm32f4_i2c.h"
#include "module_sensor.h"
#include "module_mpu9150.h"
/*==============================================================================================*/
/*==============================================================================================*
**函數 : MPU9150_Init
**功能 : 初始化 MPU9150
**輸入 : None
**輸出 : None
**使用 : MPU9150_Init()
**==============================================================================================*/
/*==============================================================================================*/
u8 MPU9150_Init( void )
{
  u8 i = 0;
  u8 ReadBuf[3] = {0};
  u8 MPU6050_Init_Data[6][2] = {
      {0x01, MPU6050_PWR_MGMT_1},   // Reset device
      {0x03, MPU6050_CONFIG},       // 
      {0x18, MPU6050_GYRO_CONFIG},  // +-2000dps
      {0x08, MPU6050_ACCEL_CONFIG}, // +-4G
      {0x32, MPU6050_INT_PIN_CFG},  // 
      {0x00, MPU6050_USER_CTRL}     // 
    };
  u8 AK8975_Init_Data[3][2] = {
      {0x0F, AK8975_CNTL},          // Set Fuse ROM access mode
      {0x00, AK8975_CNTL},          // Set Power Down Mode
      {0x01, AK8975_CNTL}           // Set Single Measurement Mode
    };

  /* MPU6050 */
  I2C_DMA_ReadReg(MPU6050_I2C_ADDR, MPU6050_WHO_AM_I, ReadBuf, 1);
  if(ReadBuf[0] != MPU6050_Device_ID)
    return ERROR;

  Delay_1ms(10);

  for(i=0; i<6; i++) {
    I2C_DMA_WriteReg(MPU6050_I2C_ADDR, MPU6050_Init_Data[i][1], MPU6050_Init_Data[i], 1);
    Delay_1ms(10);
  }

  /* AK8975 */
  I2C_DMA_ReadReg(AK8975_I2C_ADDR, AK8975_WIA, ReadBuf, 1);
  if(ReadBuf[0] != AK8975_Device_ID)
    return ERROR;

  I2C_DMA_WriteReg(AK8975_I2C_ADDR, AK8975_Init_Data[0][1], AK8975_Init_Data[0], 1);
  Delay_1ms(10);

  I2C_DMA_ReadReg(AK8975_I2C_ADDR, AK8975_ASAX, ReadBuf, 3);
  Delay_1ms(10);

  Mag.AdjustX = ((ReadBuf[0]+128)*30)>>8;
  Mag.AdjustY = ((ReadBuf[1]+128)*30)>>8;
  Mag.AdjustZ = ((ReadBuf[2]+128)*30)>>8;

  I2C_DMA_WriteReg(AK8975_I2C_ADDR, AK8975_Init_Data[1][1], AK8975_Init_Data[1], 1);
  Delay_1ms(10);
  I2C_DMA_WriteReg(AK8975_I2C_ADDR, AK8975_Init_Data[2][1], AK8975_Init_Data[2], 1);
  Delay_1ms(10);

  return SUCCESS;
}
/*==============================================================================================*/
/*==============================================================================================*
**函數 : MPU9150_Read
**功能 : 讀取感測器資料
**輸入 : *ReadBuf
**輸出 : None
**使用 : MPU9150_Read(IMU_Buf);
**==============================================================================================*/
/*==============================================================================================*/
void MPU9150_Read( s16 *IMU_Buf )
{
  u8 ReadBuf[20] = {0};
  u8 ReadData = 0x00;
  u8 WriteData = 0x01;

  I2C_DMA_ReadReg(MPU6050_I2C_ADDR, MPU6050_ACCEL_XOUT_H, ReadBuf, 14);

  I2C_DMA_ReadReg(AK8975_I2C_ADDR, AK8975_ST1, &ReadData, 1);       // Wait Prapare Data
  if(ReadData == 1) {
    I2C_DMA_ReadReg(AK8975_I2C_ADDR, AK8975_HXL, ReadBuf+14, 6);
    I2C_DMA_WriteReg(AK8975_I2C_ADDR, AK8975_CNTL, &WriteData, 1);  // Set Single Measurement Mode
  }

  IMU_Buf[0] = (s16)((ReadBuf[0]  << 8) | IMU_Buf[1]);    // Acc.X
  IMU_Buf[1] = (s16)((ReadBuf[2]  << 8) | ReadBuf[3]);    // Acc.Y
  IMU_Buf[2] = (s16)((ReadBuf[4]  << 8) | ReadBuf[5]);    // Acc.Z
  IMU_Buf[3] = (s16)((ReadBuf[8]  << 8) | ReadBuf[9]);    // Gyr.X
  IMU_Buf[4] = (s16)((ReadBuf[10] << 8) | ReadBuf[11]);   // Gyr.Y
  IMU_Buf[5] = (s16)((ReadBuf[12] << 8) | ReadBuf[13]);   // Gyr.Z
  IMU_Buf[6] = (s16)((ReadBuf[15] << 8) | ReadBuf[14]);   // Mag.X
  IMU_Buf[7] = (s16)((ReadBuf[17] << 8) | ReadBuf[16]);   // Mag.Y
  IMU_Buf[8] = (s16)((ReadBuf[19] << 8) | ReadBuf[18]);   // Mag.Z
  IMU_Buf[9] = (s16)((ReadBuf[6]  << 8) | ReadBuf[7]);    // Temp
}
/*==============================================================================================*/
/*==============================================================================================*/
