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

#define JUMP_FORCE -15.0f

#define SCREEN_WIDTH 640 // Set your screen width
#define SCREEN_HEIGHT 480 // Set your screen height

#define ROOM_WIDTH 1080 // Set your game world width
#define ROOM_HEIGHT 480 // Set your game world height

float playerX = 320;   // Initial player X position
float playerY = 240;   // Initial player Y position
float playerVelocityY = 0;  // Initial vertical velocity
float gravity = 1;   // Gravity strength
float playerSpeed = 5.0;
float playerRuntimenum = 50.0;
float sec_spd_timer = 50.0;
float ssp_stop = 10;
bool isJumping = false;
const int deadZone = 30; // Adjust this value as needed

// Define platform properties
float platformX = 0;
float platformY = 340;
float platform1X = 190;
float platform1Y = 240;
float platformWidth = 700;
float platformHeight = 20;

int RoomChanged = 0;

void TEST0()
{
    while (RoomChanged == 1)
    {
    ASND_Init();
    MP3Player_Init();
    MP3Player_PlayBuffer(bgm_testcotton_mp3, bgm_testcotton_mp3_size, NULL);

    GRRLIB_texImg *tex_ball = GRRLIB_LoadTexture(ball);
    GRRLIB_texImg *tex_solid = GRRLIB_LoadTexture(solid);
    GRRLIB_texImg *tex_stupidRat = GRRLIB_LoadTexture(stupidRat);
    GRRLIB_texImg *tex_TESTBG = GRRLIB_LoadTexture(TESTBG);
    //GRRLIB_texImg *tex_bg = GRRLIB_LoadTexture(bgImg);

    // Ensure the camera stays within boundaries (if needed)
    // For example, to keep the camera within the game world:
    // Replace these lines with your game's world boundaries check

    while (1)
    {
        WPAD_ScanPads();
        PAD_ScanPads(); // Update controller status
        u32 pressed = WPAD_ButtonsHeld(0);
        u32 Btndown = WPAD_ButtonsDown(0);//Variable de control

        u16 pressedGC = PAD_ButtonsHeld(0);
        u16 BtndownGC = PAD_ButtonsDown(0);//Variable de control

        // Get main control stick input
        s8 mainStickX = PAD_StickX(0); // Get X-axis position (-80 to 80)
        //s8 mainStickY = PAD_StickY(0); // Get Y-axis position (-80 to 80)

        // Get C-Stick input
        //s8 cStickX = PAD_SubStickX(0); // Get C-Stick X-axis position (-80 to 80)
        //s8 cStickY = PAD_SubStickY(0); // Get C-Stick Y-axis position (-80 to 80)

        // Move left and right
        if ((pressed & WPAD_BUTTON_UP) || (pressedGC & PAD_BUTTON_LEFT) || (mainStickX < -deadZone)) {
            playerX -= playerSpeed;
        } else if ((pressed & WPAD_BUTTON_DOWN) || (pressedGC & PAD_BUTTON_RIGHT) || (mainStickX > deadZone)) {
            playerX += playerSpeed;
        }

        if ((Btndown & WPAD_BUTTON_2 && !isJumping) || (BtndownGC & PAD_BUTTON_A && !isJumping)) {
        playerVelocityY = JUMP_FORCE; // Apply vertical force to jump
        isJumping = true; // Set jumping state to true
        }

        if ((pressed & WPAD_BUTTON_1) || (pressedGC & PAD_BUTTON_B))
        {
            if (sec_spd_timer <= 0)
            {
                playerSpeed = 10;
            }
            else
            {
                playerSpeed = 7;
                if (playerX != 0 || playerY != 0)
                {
                    sec_spd_timer--;
                }
            }
            if ((playerX == 0) && (platformY == 0))
            {
                sec_spd_timer = playerRuntimenum;
                playerSpeed = 7;
            }
        }
        else
        {
            ssp_stop--;
            if (ssp_stop <= 0)
            {
                playerSpeed = 5;
                sec_spd_timer = playerRuntimenum;
                ssp_stop = 10;
            }
        }

        // Check for collisions with the platforms
        if ((playerY > platformY) && (playerX > platformX) && (playerX < platformX + platformWidth)) {
            playerY = platformY;  // Set player on top of the platform
            playerVelocityY = 0;  // Stop vertical movement
            isJumping = false; // Reset jumping state
        } else if ((playerY > platform1Y) && (playerX > platform1X) && (playerX < platform1X + platformWidth)) {
            playerY = platform1Y;  // Set player on top of the platform
            playerVelocityY = 0;  // Stop vertical movement
            isJumping = false; // Reset jumping state
        } else {
            // Apply gravity if the player is not touching any platform
            isJumping = true; // Consider the player as jumping
            // Update player position based on physics
            playerY += playerVelocityY;
            playerVelocityY += gravity;
        }


        float cameraX = playerX - SCREEN_WIDTH / 2;
        float cameraY = playerY - SCREEN_HEIGHT / 2;

        GRRLIB_FillScreen(GRRLIB_BLACK); // Clear the screen
        // Draw the ball with adjusted scaling
        // Calculate the number of times to repeat the background image

        if (cameraX < 0) {
            cameraX = 0;
        } else if (cameraX > ROOM_WIDTH - SCREEN_WIDTH) {
            cameraX = ROOM_WIDTH - SCREEN_WIDTH;
        }

        if (cameraY < 0) {
            cameraY = 0;
        } else if (cameraY > ROOM_HEIGHT - SCREEN_HEIGHT) {
            cameraY = ROOM_HEIGHT - SCREEN_HEIGHT;
        }

        int numTilesX = SCREEN_WIDTH / tex_TESTBG->w + 2; // Repeat the background to cover the screen width

        // Draw the background image tiles based on the camera position
        for (int i = 0; i < numTilesX; i++) {
            GRRLIB_DrawImg(i * tex_TESTBG->w - cameraX, 0 - cameraY, tex_TESTBG, 0, 1, 1, GRRLIB_WHITE);
        }
        GRRLIB_DrawImg(playerX - cameraX, playerY - cameraY, tex_ball, 0, 1, 1, GRRLIB_WHITE);
        GRRLIB_DrawImg(670 - cameraX, 310 - cameraY, tex_stupidRat, 0, 1, 1, GRRLIB_WHITE);
        GRRLIB_DrawImg(platformX  - cameraX, (platformY + 64) - cameraY, tex_solid, 0, platformWidth / 10, 1, GRRLIB_WHITE);
        GRRLIB_DrawImg(platform1X  - cameraX, (platform1Y + 64) - cameraY, tex_solid, 0, platformWidth / 10, 1, GRRLIB_WHITE);

        if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) || 
            ((PAD_ButtonsHeld(0) & PAD_BUTTON_START) && 
            (PAD_ButtonsHeld(0) & PAD_BUTTON_X) && 
            (PAD_ButtonsHeld(0) & PAD_TRIGGER_Z))) {
            exit(0); // Exit from the game loop
        }
            GRRLIB_Render();
    }

    GRRLIB_FreeTexture(tex_ball);
    GRRLIB_FreeTexture(tex_solid);
    GRRLIB_FreeTexture(tex_TESTBG);
    GRRLIB_FreeTexture(tex_stupidRat);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    }
}