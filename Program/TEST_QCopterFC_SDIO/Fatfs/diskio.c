/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/
#include "stm32f4_system.h"
#include "stm32f4_sdio.h"
#include "diskio.h"

/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	SD_Error Status;

	Status = SD_Init();
	if(Status == SD_OK)
		return SD_OK;
  else
		return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	switch (pdrv) {
	case ATA :
		return 0;

	case MMC :
		return 0;

	case USB :
		return 0;
	}

	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	SD_Error Status;

  if(pdrv || !count)
    return RES_PARERR;

	if(count==1)
		Status = SD_ReadBlock(buff, sector<<9 ,SDCardInfo.CardBlockSize);
	else
		Status = SD_ReadMultiBlocks(buff, sector<< 9, SDCardInfo.CardBlockSize, count);

  /* Check if the Transfer is finished */
  Status = SD_WaitReadOperation();
  while(SD_GetStatus() != SD_TRANSFER_OK);

	if(Status == SD_OK)
		return RES_OK;
	else
		return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	SD_Error Status;

  if(pdrv || !count)
    return RES_PARERR;

	if(count==1)
		Status = SD_WriteBlock((u8 *)buff, sector << 9 ,SDCardInfo.CardBlockSize);
	else {
		Status = SD_WriteMultiBlocks((u8 *)buff, sector << 9 ,SDCardInfo.CardBlockSize, count);
		Status = SD_WaitWriteOperation();
	}

  /* Check if the Transfer is finished */
  Status = SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
	
	if(Status == SD_OK)
		return RES_OK;
	else
		return RES_ERROR;
}
#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(pdrv)
		return RES_PARERR;  /* 僅支持單磁盤操作，否則返回參數錯誤 */

	switch(cmd) {
	  case CTRL_SYNC:
			return RES_OK;
	  case GET_SECTOR_COUNT:
	    *(DWORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
	    return RES_OK;
	  case GET_BLOCK_SIZE:
	    *(WORD*)buff = SDCardInfo.CardBlockSize;
	    return RES_OK;
	  case CTRL_POWER:
			return RES_OK;
	  case CTRL_LOCK:
			return RES_OK;
	  case CTRL_EJECT:
			return RES_OK;
      /* MMC/SDC command */
	  case MMC_GET_TYPE:
			return RES_OK;
	  case MMC_GET_CSD:
			return RES_OK;
	  case MMC_GET_CID:
			return RES_OK;
	  case MMC_GET_OCR:
			return RES_OK;
	  case MMC_GET_SDSTAT:
			return RES_OK;
	}

	return RES_PARERR;
}
#endif

DWORD get_fattime( void )
{
	return 0;
}
