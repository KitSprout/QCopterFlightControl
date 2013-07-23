/* #include "module_hmc5883.h" */

#ifndef __MODULE_HMC5883_H
#define __MODULE_HMC5883_H

#include "stm32f4xx.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define HMC5883_I2C_ADDR				    ((u8)0x3C)

#define	HMC5883_REG_CONF_A          ((u8)0x00)
#define	HMC5883_REG_CONF_B          ((u8)0x01)
#define	HMC5883_REG_MODE            ((u8)0x02)
#define	HMC5883_REG_DATA_X_H        ((u8)0x03)
#define	HMC5883_REG_DATA_X_L        ((u8)0x04)
#define	HMC5883_REG_DATA_Y_H        ((u8)0x05)
#define	HMC5883_REG_DATA_Y_L        ((u8)0x06)
#define	HMC5883_REG_DATA_Z_H        ((u8)0x07)
#define	HMC5883_REG_DATA_Z_L        ((u8)0x08)
#define	HMC5883_REG_STATUS          ((u8)0x09)
#define	HMC5883_REG_IDENTF_A        ((u8)0x0A)
#define	HMC5883_REG_IDENTF_B        ((u8)0x0B)
#define	HMC5883_REG_IDENTF_C        ((u8)0x0C)

#define HMC5883_1p3G_XY ((float)0.00094787f)  // 0.00094787 guess/LSB
#define HMC5883_1p3G_Z  ((float)0.00105263f)  // 0.00105263 guess/LSB
#define HMC5883_1p9G_XY ((float)0.00125786f)  // 0.00125786 guess/LSB
#define HMC5883_1p9G_Z  ((float)0.00140845f)  // 0.00140845 guess/LSB
#define HMC5883_2p5G_XY ((float)0.00157480f)  // 0.00157480 guess/LSB
#define HMC5883_2p5G_Z  ((float)0.00175439f)  // 0.00175439 guess/LSB
#define HMC5883_4p0G_XY ((float)0.00232558f)  // 0.00232558 guess/LSB
#define HMC5883_4p0G_Z  ((float)0.00259740f)  // 0.00259740 guess/LSB
#define HMC5883_4p7G_XY ((float)0.00266667f)  // 0.00266667 guess/LSB
#define HMC5883_4p7G_Z  ((float)0.00298507f)  // 0.00298507 guess/LSB
#define HMC5883_5p6G_XY ((float)0.00312500f)  // 0.00312500 guess/LSB
#define HMC5883_5p6G_Z  ((float)0.00350877f)  // 0.00350877 guess/LSB
#define HMC5883_8p1G_XY ((float)0.00434783f)  // 0.00434783 guess/LSB
#define HMC5883_8p1G_Z  ((float)0.00487805f)  // 0.00487805 guess/LSB
/*=====================================================================================================*/
/*=====================================================================================================*/
void HMC5883_Init( void );
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
