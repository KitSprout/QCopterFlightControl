/* #include "stm32f4_system.h" */

#ifndef __STM32F4_SYSTEM_H
#define __STM32F4_SYSTEM_H

#include "stm32f4xx.h"
#include "stm32f4_delay.h"
/*=====================================================================================================*/
/*=====================================================================================================*/
#define Bit_Band(Addr, BitNum)  ((Addr & 0xF0000000)+0x2000000+((Addr &0xFFFFF)<<5)+(BitNum<<2))
#define Mem_Addr(Addr)          *((vu16 *)(Addr))
#define Bit_Addr(Addr, BitNum)  Mem_Addr(Bit_Band(Addr, BitNum))

#define GPIOA_ODR_Addr  (GPIOA_BASE+0x14)
#define GPIOB_ODR_Addr  (GPIOB_BASE+0x14)
#define GPIOC_ODR_Addr  (GPIOC_BASE+0x14)
#define GPIOD_ODR_Addr  (GPIOD_BASE+0x14)
#define GPIOE_ODR_Addr  (GPIOE_BASE+0x14)
#define GPIOF_ODR_Addr  (GPIOF_BASE+0x14)
#define GPIOG_ODR_Addr  (GPIOG_BASE+0x14)
#define GPIOA_IDR_Addr  (GPIOA_BASE+0x10)
#define GPIOB_IDR_Addr  (GPIOB_BASE+0x10)
#define GPIOC_IDR_Addr  (GPIOC_BASE+0x10)
#define GPIOD_IDR_Addr  (GPIOD_BASE+0x10)
#define GPIOE_IDR_Addr  (GPIOE_BASE+0x10)
#define GPIOF_IDR_Addr  (GPIOF_BASE+0x10)
#define GPIOG_IDR_Addr  (GPIOG_BASE+0x10)

#define PAO(Pin)  Bit_Addr(GPIOA_ODR_Addr, Pin)
#define PAI(Pin)  Bit_Addr(GPIOA_IDR_Addr, Pin)
#define PBO(Pin)  Bit_Addr(GPIOB_ODR_Addr, Pin)
#define PBI(Pin)  Bit_Addr(GPIOB_IDR_Addr, Pin)
#define PCO(Pin)  Bit_Addr(GPIOC_ODR_Addr, Pin)
#define PCI(Pin)  Bit_Addr(GPIOC_IDR_Addr, Pin)
#define PDO(Pin)  Bit_Addr(GPIOD_ODR_Addr, Pin)
#define PDI(Pin)  Bit_Addr(GPIOD_IDR_Addr, Pin)
#define PEO(Pin)  Bit_Addr(GPIOE_ODR_Addr, Pin)
#define PEI(Pin)  Bit_Addr(GPIOE_IDR_Addr, Pin)
#define PFO(Pin)  Bit_Addr(GPIOF_ODR_Addr, Pin)
#define PFI(Pin)  Bit_Addr(GPIOF_IDR_Addr, Pin)
#define PGO(Pin)  Bit_Addr(GPIOG_ODR_Addr, Pin)
#define PGI(Pin)  Bit_Addr(GPIOG_IDR_Addr, Pin)
/*=====================================================================================================*/
/*=====================================================================================================*/
#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

#define Byte16(ByteH, ByteL) ((u16)(((ByteH)<<8) | (ByteL)))
#define Byte8H(ByteH)        ((u8)((ByteH)>>8))
#define Byte8L(ByteL)        ((u8)(ByteL))

#define NULL 0

typedef float  fp32;
typedef double fp64;

typedef volatile float  vfp32;
typedef volatile double vfp64;

typedef float  const fpc32;
typedef double const fpc64;

typedef enum {FALSE = 0, TRUE = !FALSE} bool;
/*=====================================================================================================*/
/*=====================================================================================================*/
typedef struct {
  u8 Bit0:1;
  u8 Bit1:1;
  u8 Bit2:1;
  u8 Bit3:1;
  u8 Bit4:1;
  u8 Bit5:1;
  u8 Bit6:1;
  u8 Bit7:1;
} BitField;
typedef struct {
  u16 L:8;
  u16 H:8;
}	HL_Struct;
/*=====================================================================================================*/
/*=====================================================================================================*/
#endif
