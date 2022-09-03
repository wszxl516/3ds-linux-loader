#include <stdint.h>
#include <stdbool.h>
#include <linux.h>
#include <string.h>
#ifdef __LOGO__
    #include "splash.h"
#endif

//Common data types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
#define REG32(addr)             (*(vu32 *)(addr))
//https://www.3dbrew.org/wiki/GPU/External_Registers#Framebuffer_format
// LCD Frambuffers stuff (Physical Addresses)
//set LCD fill black to hide potential garbage
#define TOP_FILL   REG32(0x10202204)
#define BOT_FILL   REG32(0x10202A04)


//Framebuffer Setup "PDC0"
#define TOP_FB_SETTING_ADDR           (0x10400400)
#define BOT_FB_SETTING_ADDR           (0x10400500)

#define REG_TOP_FB_A (TOP_FB_SETTING_ADDR + 0x68)
//left eye 3D framebuffer 0.
#define TOP_FB_A0_REG  (REG32(REG_TOP_FB_A + 0))
//left eye 3D framebuffer 1.
#define TOP_FB_A1_REG  (REG32(REG_TOP_FB_A + 4))
//   Top Screen
//Framebuffer B first address
#define REG_TOP_FB_B  (TOP_FB_SETTING_ADDR + 0x94)
//right eye 3D framebuffer 0.
#define TOP_FB_B0_REG  (REG32(REG_TOP_FB_B + 0))
//right eye 3D framebuffer 1.
#define TOP_FB_B1_REG  (REG32(REG_TOP_FB_B + 4))
//Framebuffer format and other settings
#define TOP_FB_FORMAT_REG  (REG32(TOP_FB_SETTING_ADDR + 0x70))
//Framebuffer select and status
#define TOP_FB_SELECT_REG  (REG32(TOP_FB_SETTING_ADDR + 0x78))
//Framebuffer stride
#define TOP_FB_STRIDE_REG  (REG32(TOP_FB_SETTING_ADDR + 0x90))

//Bottom Screen
//Framebuffer B first address
#define REG_BOT_FB_A (BOT_FB_SETTING_ADDR + 0x68)
#define BOT_FB_A0_REG  (REG32(REG_BOT_FB_A + 0))
#define BOT_FB_A1_REG  (REG32(REG_BOT_FB_A + 4))

//Framebuffer format and other settings
#define BOT_FB_FORMAT_REG  (REG32(BOT_FB_SETTING_ADDR + 0x70))
//Framebuffer select and status
#define BOT_FB_SELECT_REG  (REG32(BOT_FB_SETTING_ADDR + 0x78))
//Framebuffer stride
#define BOT_FB_STRIDE_REG  (REG32(BOT_FB_SETTING_ADDR + 0x90))

// 0 GL_RGBA8_OES 1 GL_RGB8_OES
#define COLOR_FMT (1)
//output image as normal
#define TOP_OUTPUT_MODE (0)
#define BOT_OUTPUT_MODE (0)
//Scan doubling enable
#define TOP_DOUBLE_SCAN (1)
#define BOT_DOUBLE_SCAN (0)
//DMA size
#define DMA_SIZE (3)
//Framebuffer format
//0x80341
#define TOP_FB_FORMAT (1<<19 | DMA_SIZE<<8 | TOP_DOUBLE_SCAN<<6 | TOP_OUTPUT_MODE<<4 | COLOR_FMT)
#define BOT_FB_FORMAT (1<<19 | DMA_SIZE<<8 | BOT_DOUBLE_SCAN<<6 | BOT_OUTPUT_MODE<<4 | COLOR_FMT)
//Framebuffer stride mode
#define FB_STRIDE    (0x2D0)


#define TOP_FB_SIZE           (400 * 240 * 3)
#define BOT_FB_SIZE           (320 * 240 * 3)
#define FB_BASE_PA            (VRAM_BASE)
#define TOP_FB_LEFT1          (FB_BASE_PA)
#define TOP_FB_LEFT2          (TOP_FB_LEFT1  + TOP_FB_SIZE)
#define TOP_FB_RIGHT1         (TOP_FB_LEFT2  + TOP_FB_SIZE)
#define TOP_FB_RIGHT2         (TOP_FB_RIGHT1 + TOP_FB_SIZE)
#define BOT_FB_1              (TOP_FB_RIGHT2 + TOP_FB_SIZE)
#define BOT_FB_2              (BOT_FB_1      + BOT_FB_SIZE)
extern void waitcycles(u32 us);
inline void delay(u32 ms){waitcycles(ms * 1000);}
#define clear_screen() \
    do { \
        memset((void*)TOP_FB_LEFT1, 0, TOP_FB_SIZE);\
        memset((void*)TOP_FB_RIGHT1, 0, TOP_FB_SIZE);\
        memset((void*)BOT_FB_1, 0, BOT_FB_SIZE);\
    }while(0)

#define draw_splash() do{ memcpy((void*)TOP_FB_LEFT1, splash_bin, splash_len); delay(1000);}while(0)