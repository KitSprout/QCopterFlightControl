/* #include "stm32f4_system.h" */

#ifndef __STM32F4_SYSTEM_H
#define __STM32F4_SYSTEM_H

#include "stm32f4xx.h"
#include "stm32f4_delay.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define __BitBand(Addr, BitNum)   ((Addr & 0xF0000000)+0x2000000+((Addr &0xFFFFF)<<5)+(BitNum<<2))
#define __MemAddr(Addr)           *((vu16 *)(Addr))
#define __BitAddr(Addr, BitNum)   __MemAddr(__BitBand(Addr, BitNum))

#define PAO(Pin)  __BitAddr(GPIOA_BASE+0x14, Pin)
#define PAI(Pin)  __BitAddr(GPIOA_BASE+0x10, Pin)
#define PBO(Pin)  __BitAddr(GPIOB_BASE+0x14, Pin)
#define PBI(Pin)  __BitAddr(GPIOB_BASE+0x10, Pin)
#define PCO(Pin)  __BitAddr(GPIOC_BASE+0x14, Pin)
#define PCI(Pin)  __BitAddr(GPIOC_BASE+0x10, Pin)
#define PDO(Pin)  __BitAddr(GPIOD_BASE+0x14, Pin)
#define PDI(Pin)  __BitAddr(GPIOD_BASE+0x10, Pin)
#define PEO(Pin)  __BitAddr(GPIOE_BASE+0x14, Pin)
#define PEI(Pin)  __BitAddr(GPIOE_BASE+0x10, Pin)
#define PFO(Pin)  __BitAddr(GPIOF_BASE+0x14, Pin)
#define PFI(Pin)  __BitAddr(GPIOF_BASE+0x10, Pin)
#define PGO(Pin)  __BitAddr(GPIOG_BASE+0x14, Pin)
#define PGI(Pin)  __BitAddr(GPIOG_BASE+0x10, Pin)
/*====================================================================================================*/
/*====================================================================================================*/
#define U8_MAX    ((u8)255)
#define S8_MAX    ((s8)127)
#define S8_MIN    ((s8)-128)
#define U16_MAX   ((u16)65535u)
#define S16_MAX   ((s16)32767)
#define S16_MIN   ((s16)-32768)
#define U32_MAX   ((u32)4294967295uL)
#define S32_MAX   ((s32)2147483647)
#define S32_MIN   ((s32)-2147483648)

#ifndef NULL
  #define NULL    (void *) 0
#endif

#define Byte32(Type, Byte4, Byte3, Byte2, Byte1)  ((Type)((((u8)(Byte4))<<24) | (((u8)(Byte3))<<16) | (((u8)(Byte2))<<8) | ((u8)(Byte1))))
#define Byte16(Type, ByteH, ByteL)  ((Type)((((u16)(ByteH))<<8) | ((u16)(ByteL))))
#define Byte8H(ByteH)   ((u8)(((u16)(ByteH))>>8))
#define Byte8L(ByteL)   ((u8)(ByteL))
/*====================================================================================================*/
/*====================================================================================================*/
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int64_t sc64;  /*!< Read Only */
typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t  sc8;   /*!< Read Only */

typedef __IO int64_t vs64;
typedef __IO int32_t vs32;
typedef __IO int16_t vs16;
typedef __IO int8_t  vs8;

typedef __I int64_t vsc64;  /*!< Read Only */
typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t  vsc8;   /*!< Read Only */

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint64_t uc64;  /*!< Read Only */
typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t  uc8;   /*!< Read Only */

typedef __IO uint64_t vu64;
typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint64_t vuc64;  /*!< Read Only */
typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t  vuc8;   /*!< Read Only */

typedef float  fp32;
typedef double fp64;

typedef const float  fpc32;
typedef const double fpc64;

typedef __IO float  vfp32;
typedef __IO double vfp64;

typedef __I float  vfpc32;
typedef __I double vfpc64;
/*====================================================================================================*/
/*====================================================================================================*/
#endif
