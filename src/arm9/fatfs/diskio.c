/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sdmmc.h"


#define PART_INFO(pdrv) (DriveInfo + pdrv)
#define PART_TYPE(pdrv) (DriveInfo[pdrv].type)

#define TYPE_NONE       0
#define TYPE_SDCARD     (1UL<<4)

#define SUBTYPE_CTRN    1
#define SUBTYPE_TWLN    2
#define SUBTYPE_TWLP    3
#define SUBTYPE_NONE    0

typedef struct {
    BYTE  type;
    BYTE  subtype;
    DWORD offset;
    DWORD size;
    BYTE  keyslot;
} FATpartition;

FATpartition DriveInfo[13] = {
    { TYPE_SDCARD,  SUBTYPE_NONE, 0, 0, 0xFF },     // 0 - SDCARD
};



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	__attribute__((unused))
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	__attribute__((unused))
	BYTE pdrv				/* Physical drive number to identify the drive */
)
{
    FATpartition* fat_info = PART_INFO(pdrv);
    BYTE type = PART_TYPE(pdrv);
    
    fat_info->offset = fat_info->size = 0;
    fat_info->keyslot = 0xFF;
    
    if (type == TYPE_SDCARD) {
        if (sdmmc_sdcard_init() != 0) return STA_NOINIT|STA_NODISK;
        fat_info->size = getMMCDevice(1)->total_size;
    } else {
        return RES_PARERR;
    }
    
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	__attribute__((unused))
	BYTE pdrv,		/* Physical drive number to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{   
    BYTE type = PART_TYPE(pdrv);
    
    if (type == TYPE_NONE) {
        return RES_PARERR;
    } else if (type == TYPE_SDCARD) {
        if (sdmmc_sdcard_readsectors(sector, count, buff))
            return RES_PARERR;
    } else {
            return RES_PARERR;
    }

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	__attribute__((unused))
	BYTE pdrv,			/* Physical drive number to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
    BYTE type = PART_TYPE(pdrv);
    
    if (type == TYPE_NONE) {
        return RES_PARERR;
    } else if (type == TYPE_SDCARD) {
        if (sdmmc_sdcard_writesectors(sector, count, (BYTE *)buff))
            return RES_PARERR;
    } else {
        return RES_PARERR; // unstubbed!
    }

	return RES_OK;
}
#endif



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	__attribute__((unused))
	BYTE pdrv,		/* Physical drive number (0..) */
	__attribute__((unused))
	BYTE cmd,		/* Control code */
	__attribute__((unused))
	void *buff		/* Buffer to send/receive control data */
)
{
    switch (cmd) {
        case GET_SECTOR_SIZE:
            *((DWORD*) buff) = 0x200;
            return RES_OK;
        case GET_SECTOR_COUNT:
            *((DWORD*) buff) = PART_INFO(pdrv)->size;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *((DWORD*) buff) = 0x2000;
            return RES_OK;
        case CTRL_SYNC:
            // nothing else to do here - sdmmc.c handles the rest
            return RES_OK;
    }
    
	return RES_PARERR;
}
#endif
