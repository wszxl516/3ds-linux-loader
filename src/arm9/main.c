#include "ui.h"
#include "i2c.h"
#include "qff.h"
#include "common.h"
#include "load_kernel.h"

void Reboot()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(true);
}

void Poweroff()
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 0);
    while(true);
}


u8 *top_screen, *bottom_screen;

void main(int argc, char** argv)
{
    // Fetch the framebuffer addresses
    if(argc >= 2) {
        // newer entrypoints
        u8 **fb = (u8 **)(void *)argv[1];
        top_screen = fb[0];
        bottom_screen = fb[2];
    } else {
        // outdated entrypoints
        top_screen = (u8*)(*(u32*)0x23FFFE00);
        bottom_screen = (u8*)(*(u32*)0x23FFFE08);
    }
    ClearScreenF(true, true, COLOR_STD_BG);
    linux_loader();
    if(ShowPrompt(true, "yes for poweroff, no for reboot!"))
        Poweroff();
    Reboot();
}
