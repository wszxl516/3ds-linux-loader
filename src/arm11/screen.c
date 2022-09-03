#include "screen.h"

void init_screens(void)
{
	//Map Framebuffers
    //Top screen
    //left
    clear_screen();
    #ifdef __LOGO__
        draw_splash();
    #endif
    TOP_FB_A0_REG = TOP_FB_LEFT1;
    TOP_FB_A1_REG = TOP_FB_LEFT2;
    
	//right
    TOP_FB_B0_REG = TOP_FB_RIGHT1;
    TOP_FB_B1_REG = TOP_FB_RIGHT2;

	//Select framebuffer 0 and adjust format/stride
    TOP_FB_SELECT_REG = 0;
    TOP_FB_FORMAT_REG = TOP_FB_FORMAT;
    TOP_FB_STRIDE_REG = FB_STRIDE;

    //@@@ Bottom screen @@@
    BOT_FB_A0_REG = BOT_FB_1;
    BOT_FB_A1_REG = BOT_FB_2;
	//Select framebuffer 0
    BOT_FB_SELECT_REG = 0;
    return;
}