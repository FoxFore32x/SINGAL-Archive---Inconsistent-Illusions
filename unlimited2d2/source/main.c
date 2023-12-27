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

#include "gfx/ball.h"
#include "gfx/solid.h"
#include "gfx/TESTBG.h"
#include "gfx/stupidRat.h"
#include "Sound/BGM/bgm_testcotton_mp3.h"

#define SCREEN_WIDTH 640 // Set your screen width
#define SCREEN_HEIGHT 480 // Set your screen height


int main() {
    // Initialization code for GRRLIB, WPAD, etc.
    GRRLIB_Init();
    WPAD_Init();
    PAD_Init();

    GRRLIB_Settings.antialias = false;

    // Game loop
    while (1) {
        WPAD_ScanPads();
        PAD_ScanPads();

        u32 Btndown = WPAD_ButtonsDown(0);//Variable de control
        u16 BtndownGC = PAD_ButtonsDown(0);//Variable de control

        if (RoomChanged != 1)
        {
            rm_printtest();
        }

        if ((Btndown & WPAD_BUTTON_PLUS) || (BtndownGC & PAD_BUTTON_START)) {
            RoomChanged = 1;
            TEST0();
        }
    }

    // Cleanup code and return
    return 0;
}

//In this code is some sort of recreation of the Game Maker room handiling system, which i have yet to complete.