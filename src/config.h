/* Linux settings */
#define DTB_ADDR         (0x20000000)
#define ZIMAGE_ADDR      (0x20008000)
#define INITRAMFS_ADDR   (0x23208000)
#define MACHINE_NUMBER   (0xFFFFFFFF)
#define ARM9LINUXFW_ADDR (0x08080000)
#define SYNC_ADDR        (0x1FFFFFF0)

#define LINUX_PATH "/linux/"

#define LINUXIMAGE  "zImage"
#define INITRAMFS   "initramfs.cpio.gz"
#define CTR_DTB     "nintendo3ds_ctr.dtb"
#define KTR_DTB     "nintendo3ds_ktr.dtb"
#define ARM9LINUXFW "arm9linuxfw.bin"



#define LINUXIMAGE_PATH  LINUX_PATH LINUXIMAGE
#define INITRAMFS_PATH   LINUX_PATH INITRAMFS
#define CTR_DTB_PATH     LINUX_PATH CTR_DTB
#define KTR_DTB_PATH     LINUX_PATH KTR_DTB
#define ARM9LINUXFW_PATH LINUX_PATH ARM9LINUXFW

/* 3DS memory layout */
#define VRAM_BASE     (0x18000000)
#define VRAM_SIZE     (0x00600000)
#define AXI_WRAM_BASE (0x1FF80000)
#define AXI_WRAM_SIZE (0x00080000)
#define FCRAM_BASE    (0x20000000)
#define FCRAM_SIZE    (0x08000000)
