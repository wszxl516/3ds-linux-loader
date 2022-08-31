#pragma once
#include "common.h"
#include "sdmmc.h"
#include "ui.h"
#include "qff.h"
#include "unittype.h"
#include "cache.h"
#include "../config.h"

#define SD_MNT  "0:"

#define LINUXIMAGE_FILENAME   SD_MNT LINUXIMAGE_PATH
#define INITRAMFS_FILENAME    SD_MNT INITRAMFS_PATH
#define CTR_DTB_FILENAME      SD_MNT CTR_DTB_PATH
#define KTR_DTB_FILENAME      SD_MNT KTR_DTB_PATH
#define ARM9LINUXFW_FILENAME  SD_MNT ARM9LINUXFW_PATH

#define DTB_BUF          ((u8*)DTB_ADDR)
#define ZIMAGE_BUF       ((u8*)ZIMAGE_ADDR)
#define INITRAMFS_BUF    ((u8*)INITRAMFS_ADDR)
#define ARM9LINUXFW_BUF  ((u8*)ARM9LINUXFW_ADDR)
#define SYNC        ((uint32_t *)SYNC_ADDR)
#define ARM9LINUXFW_START ((void (*)(void))ARM9LINUXFW_ADDR)

#define MIN_FILE_SIZE    0x1000

#define MSG_SDCARD       "SDCard"
#define MSG_LINUX        "Linux"

#define CFG11_SOCINFO			(0x10140FFC)


static inline short get_cfg11_socinfo(void)
{
	return *(short*)CFG11_SOCINFO & 0x7;
}
static inline bool is_lgr1(void) { return get_cfg11_socinfo() == 0x3; }
static inline bool is_lgr2(void) { return get_cfg11_socinfo() == 0x7; }
static inline bool is_lgr(void) { return is_lgr1() || is_lgr2(); }

u32 linux_loader(void);
