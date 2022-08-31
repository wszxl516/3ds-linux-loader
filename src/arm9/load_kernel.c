#include "load_kernel.h"

void start_linux(){
    *SYNC = 1;
    /* Make the ARM11 jump to the Linux payload */
	flush_caches();
	/* Jump to the arm9linuxfw */
	(ARM9LINUXFW_START)();
}

u32 linux_loader(void) {
    u32 ret =0;
    // initialization
    show_info("Initializing fs, please wait...");
    // init/check SD card
    u64 sdFree = 0;
    u64 sdTotal = 0;
    if ((fs_init() != FR_OK) || (f_getfreebyte("0:", &sdFree) != FR_OK) || (f_gettotalbyte("0:", &sdTotal) != FR_OK)) {
        show_warn("%s init failed!\n", MSG_SDCARD);
        ret = 1;
        goto out;
    }
    char size_tmp[32];
    readable_size(sdFree, size_tmp, 16);
    readable_size(sdTotal, size_tmp + 16, 16); 
    show_info("SDCard Usage: %s/%s\n", size_tmp, size_tmp + 16);
    UINT read_size=0;
    if ((f_qread(LINUXIMAGE_FILENAME, ZIMAGE_BUF , 0, 0, &read_size) != FR_OK) || (read_size < MIN_FILE_SIZE)) {
        show_warn("%s not found !\n", LINUXIMAGE_FILENAME);
        ret = 1;
        goto out;
    }
    show_info("kernel: %s loaded size: %s!\n", LINUXIMAGE, readable_size(read_size, size_tmp, 32));

    read_size=0;
    if ((f_qread(INITRAMFS_FILENAME, INITRAMFS_BUF , 0, 0, &read_size) != FR_OK) || (read_size < MIN_FILE_SIZE)) {
        show_warn("%s not found !\n", INITRAMFS_FILENAME);
    }
    else{
        show_info("initramfs: %s loaded size: %s!\n", INITRAMFS, readable_size(read_size, size_tmp, 32));
    }

    read_size=0;
    if ((f_qread(is_lgr() ? KTR_DTB_FILENAME : CTR_DTB_FILENAME, DTB_BUF , 0, 0, &read_size) != FR_OK) || (read_size < MIN_FILE_SIZE)) {
        show_warn("%s not found !\n", is_lgr() ? KTR_DTB_FILENAME : CTR_DTB_FILENAME);
        ret = 1;
        goto out;
    }
    show_info("DTB: %s loaded size: %s!\n", is_lgr() ? KTR_DTB : CTR_DTB, readable_size(read_size, size_tmp, 32));
    read_size=0;
    if ((f_qread(ARM9LINUXFW_FILENAME, ARM9LINUXFW_BUF , 0, 0, &read_size) != FR_OK) || (read_size < MIN_FILE_SIZE)) {
        show_warn("%s not found !\n", ARM9LINUXFW_FILENAME);
        ret = 1;
        goto out;
    }
    show_info("Linux firm %s loaded size: %s!\n", ARM9LINUXFW, readable_size(read_size, size_tmp, 32));
    flush_caches();
    if (fs_deinit() != FR_OK)
    {
        show_warn("%s deinit failed!\n", MSG_SDCARD);
    }
    if (warn_prompt(true, "All ready, whether to start linux ?")) 
        start_linux();

out:
    flush_caches();
    if (fs_deinit() != FR_OK)
    {
        show_info("%s deinit failed!\n", MSG_SDCARD);
    }
    return ret;
}