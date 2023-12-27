#include <grrlib.h>
#include <ogc/video.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>
#include <mp3player.h>
#include <stdio.h>
#include <asndlib.h>
#include <gctypes.h>
#include <gccore.h>
#include "System/graphics/all_gfx.h"
#include "room.h"

void rm_printtest() {
    while (RoomChanged == 1)
    {
    // Initialise GRRLIB and the video system
    VIDEO_Init();

    // This function initialises the attached controllers
    WPAD_Init();

    // Get the preferred video mode from the system
    GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);

    // Allocate memory for the display in the uncached region
    void *xfb[2];
    for (int i = 0; i < 2; i++) {
        xfb[i] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    }

    // Set up the video registers with the chosen mode
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb[0]);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();

    // Make the display visible
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    // Set up the console, required for printf
    CON_InitEx(rmode, 20, 20, rmode->fbWidth - 40, rmode->xfbHeight - 40);

    printf("\x1b[2;0H");
    printf("Hello World!\n");

    while (1) {

        GRRLIB_FillScreen(GRRLIB_BLACK);
        WPAD_ScanPads();
        PAD_ScanPads();

        /*u32 pressed = WPAD_ButtonsHeld(0);
        u32 Btndown = WPAD_ButtonsDown(0);//Variable de control

        u16 pressedGC = PAD_ButtonsHeld(0);
        u16 BtndownGC = PAD_ButtonsDown(0);//Variable de control

        // Get main control stick input
        s8 mainStickX = PAD_StickX(0);*/

        if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) || 
            ((PAD_ButtonsHeld(0) & PAD_BUTTON_START) && 
            (PAD_ButtonsHeld(0) & PAD_BUTTON_X) && 
            (PAD_ButtonsHeld(0) & PAD_TRIGGER_Z))) {
            exit(0); // Exit from the game loop
        }

        GRRLIB_Render();
        
    }

    // Clean up and exit
    GRRLIB_Exit();
    }
}
