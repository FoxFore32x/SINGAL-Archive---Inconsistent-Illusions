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

#include "all_gfx.h"
#include "room.h"
#include "bgm_testcotton_mp3.h"
#include "fot_ttf.h"

#define SCREEN_WIDTH 640 // Set your screen width
#define SCREEN_HEIGHT 480 // Set your screen height

int main() {
    // Initialization code for GRRLIB, WPAD, etc.
    GRRLIB_Init();
    WPAD_Init();
    PAD_Init();

    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(fot_ttf, fot_ttf_size);

    GRRLIB_Settings.antialias = false;

    // Game loop
    while (1) {
        WPAD_ScanPads();
        PAD_ScanPads();

        u32 Btndown = WPAD_ButtonsDown(0);//Variable de control
        u16 BtndownGC = PAD_ButtonsDown(0);//Variable de control
        
        GRRLIB_PrintfTTF(100, 100, myFont, "Haha peepee poopoo", 24, GRRLIB_WHITE); // X=100, Y=100, Size=24, White color
        GRRLIB_PrintfTTF(100, 200, myFont, "SINGAL's Archive", 50, GRRLIB_WHITE); // X=100, Y=100, Size=24, White color

        if ((Btndown & WPAD_BUTTON_PLUS) || (BtndownGC & PAD_BUTTON_START)) {
            RoomChanged = 1;
            TEST0();
        }

        GRRLIB_Render();
    }

    // Cleanup code and return
    return 0;
}

//In this code is some sort of recreation of the Game Maker room handiling system, which i have yet to complete.