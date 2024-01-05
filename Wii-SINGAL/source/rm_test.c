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
#include <time.h>

#include "all_gfx.h"
#include "room.h"
#include "fot_ttf.h"
#include "bgm_testcotton_mp3.h"

#define JUMP_FORCE -15.0f

bool Widescreen = false;

float StandardRes = 640;

float WidescreenRes = 854;

#define SCREEN_WIDTH StandardRes //640 // Set your screen width
#define SCREEN_HEIGHT 480 // Set your screen height

#define ROOM_WIDTH 4000 // Set your game world width
#define ROOM_HEIGHT 1024 // Set your game world height

#define MAX_PLATFORMS 6 // Define the maximum number of platforms

float playerX = 320;   // Initial player X position
float playerY = 240;   // Initial player Y position
float playerVelocityY = 0;  // Initial vertical velocity
float gravity = 1;   // Gravity strength
float hsp = 0; // Horizontal speed
float vsp = 0; // Vertical speed
float playerSpeed = 5.0;
float playerRuntimenum = 50.0;
float sec_spd_timer = 50.0;
float ssp_stop = 10;
bool isJumping = false;
const int deadZone = 30; // Adjust this value as needed

int RoomChanged = 0;

// Define a structure to hold platform properties
typedef struct {
    float x;
    float y;
    float width;
    float height;
} Platform;

// Create an array of Platform structures to hold multiple platforms
Platform platforms[MAX_PLATFORMS];

enum Direction {
    LEFT,
    RIGHT
};

enum Direction ballDirection = RIGHT; // Initialize ball direction

// Initialize platform properties (positions and dimensions)
void initPlatforms() {
    platforms[0].x = 0;
    platforms[0].y = 340;
    platforms[0].width = 700;
    platforms[0].height = 1;

    platforms[1].x = 190;
    platforms[1].y = 240;
    platforms[1].width = 1400;
    platforms[1].height = 1;

    platforms[2].x = 92;
    platforms[2].y = 988;
    platforms[2].width = 200;
    platforms[2].height = 1;

    platforms[3].x = 486;
    platforms[3].y = 884;
    platforms[3].width = 53;
    platforms[3].height = 1;

    platforms[4].x = 120;
    platforms[4].y = 772;
    platforms[4].width = 57;
    platforms[4].height = 1;

    platforms[5].x = 532;
    platforms[5].y = 648;
    platforms[5].width = 69;
    platforms[5].height = 1;

    // Add more platforms as needed
}

void TEST0()
{
    while (RoomChanged == 1)
    {
    VIDEO_Init();
    ASND_Init();
    MP3Player_Init();
    MP3Player_PlayBuffer(bgm_testcotton_mp3, bgm_testcotton_mp3_size, NULL);
    GRRLIB_FillScreen(GRRLIB_BLACK); // Clear the screen

    GRRLIB_texImg *tex_teoriIdle = GRRLIB_LoadTexturePNG(TeoriIdletest);
    GRRLIB_texImg *tex_solid = GRRLIB_LoadTexturePNG(solid);
    GRRLIB_texImg *tex_TESTBG = GRRLIB_LoadTexturePNG(bg_test);

    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(fot_ttf, fot_ttf_size);

    initPlatforms();

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

        int numTilesX = (ROOM_WIDTH / tex_TESTBG->w) + 2; // Number of tiles to cover the room width

        float maxCameraX = ROOM_WIDTH - SCREEN_WIDTH;
        float maxCameraY = ROOM_HEIGHT - SCREEN_HEIGHT;


        float cameraX = playerX - SCREEN_WIDTH / 2;
        float cameraY = playerY - SCREEN_HEIGHT / 2;
        // Draw the ball with adjusted scaling
        // Calculate the number of times to repeat the background image

        // Ensure camera stays within room boundaries
        if (cameraX < 0) {
            cameraX = 0;
        } else if (cameraX > maxCameraX) {
            cameraX = maxCameraX;
        }

        if (cameraY < 0) {
            cameraY = 0;
        } else if (cameraY > maxCameraY) {
            cameraY = maxCameraY;
        }

        int startTileX = cameraX / tex_TESTBG->w;
        int offsetX = (int)cameraX % tex_TESTBG->w;

        // Rendering loop for background tiles based on camera position
        for (int i = startTileX; i < startTileX + numTilesX + 1; i++) {
            int x = i * tex_TESTBG->w - offsetX;
            GRRLIB_DrawImg(x - cameraX, 0 - cameraY, tex_TESTBG, 0, 1, 1, GRRLIB_WHITE);
        }

        GRRLIB_DrawImg(platforms[0].x - cameraX, platforms[0].y - cameraY, tex_solid, 0, platforms[0].width / 32, platforms[0].height, GRRLIB_WHITE);
        GRRLIB_DrawImg(platforms[1].x - cameraX, platforms[1].y - cameraY, tex_solid, 0, platforms[1].width / 32, platforms[1].height, GRRLIB_WHITE);
        GRRLIB_DrawImg(platforms[2].x - cameraX, platforms[2].y - cameraY, tex_solid, 0, platforms[2].width / 32, platforms[2].height, GRRLIB_WHITE);
        GRRLIB_DrawImg(platforms[3].x - cameraX, platforms[3].y - cameraY, tex_solid, 0, platforms[3].width / 32, platforms[3].height, GRRLIB_WHITE);
        GRRLIB_DrawImg(platforms[4].x - cameraX, platforms[4].y - cameraY, tex_solid, 0, platforms[4].width / 32, platforms[4].height, GRRLIB_WHITE);
        GRRLIB_DrawImg(platforms[5].x - cameraX, platforms[5].y - cameraY, tex_solid, 0, platforms[5].width / 32, platforms[5].height, GRRLIB_WHITE);

        // Draw the ball with adjusted scaling and flip based on direction
        if (ballDirection == RIGHT || !ballDirection) {
            GRRLIB_DrawImg(playerX - cameraX, playerY - cameraY, tex_teoriIdle, 0, 1, 1, GRRLIB_WHITE);
        } else if (ballDirection == LEFT) { // Flip horizontally when moving left
            GRRLIB_DrawImg(playerX - cameraX + tex_teoriIdle->w, playerY - cameraY, tex_teoriIdle, -180, 1, 1, GRRLIB_WHITE);
        }

        // cameraX += 1; // Increment the camera's X position to scroll the background

        // Reset cameraX when it exceeds the texture width to prevent overflow
        if (cameraX >= tex_TESTBG->w) {
            cameraX -= tex_TESTBG->w;
        }

        // Move left
        if ((pressed & WPAD_BUTTON_UP) || (pressedGC & PAD_BUTTON_LEFT) || (mainStickX < -deadZone)) {
            hsp = -playerSpeed;
            ballDirection = LEFT; // Update direction when moving left
        } else if ((pressed & WPAD_BUTTON_DOWN) || (pressedGC & PAD_BUTTON_RIGHT) || (mainStickX > deadZone)) {
            hsp = playerSpeed;
            ballDirection = RIGHT; // Update direction when moving right
        } else {
            hsp = 0;
        }

        // Check for jump input and apply vertical speed
        if (((Btndown & WPAD_BUTTON_2) || (BtndownGC & PAD_BUTTON_A)) && !isJumping) {
            vsp = JUMP_FORCE;
            isJumping = true;
        }

        if ((pressed & WPAD_BUTTON_1) || (pressedGC & PAD_BUTTON_B)) {
            if (sec_spd_timer <= 0) {
                playerSpeed = 10;
            } else {
                playerSpeed = 7;
                if (playerX != 0 || playerY != 0) {
                    sec_spd_timer--;
                }
            }
            if ((playerX == 0) && (playerY == 0)) {
                sec_spd_timer = playerRuntimenum;
                playerSpeed = 7;
            }
        } else {
            ssp_stop--;
            if (ssp_stop <= 0) {
                playerSpeed = 5;
                sec_spd_timer = playerRuntimenum;
                ssp_stop = 10;
            }
        }

        playerX += hsp;
        playerY += vsp;

        vsp += gravity;

        // Collision detection with platforms
        for (int i = 0; i < MAX_PLATFORMS; i++) {
            if ((playerY + tex_teoriIdle->h > platforms[i].y) && (playerY < platforms[i].y + platforms[i].height) &&
                (playerX + tex_teoriIdle->w > platforms[i].x) && (playerX < platforms[i].x + platforms[i].width)) {
                // Collision detected with the i-th platform
                playerY = platforms[i].y - tex_teoriIdle->h; // Place player on top of the platform
                vsp = 0; // Stop vertical movement
                isJumping = false; // Reset jumping state
            }
        }

        // Horizontal boundary check
        if (playerX < 0) {
            playerX = 0;
        } else if (playerX > ROOM_WIDTH - tex_teoriIdle->w) {
            playerX = ROOM_WIDTH - tex_teoriIdle->w;
        }

        // Vertical boundary check
        if (playerY < 0) {
            playerY = 0;
            isJumping = false;
        } else if (playerY > ROOM_HEIGHT - tex_teoriIdle->h) {
            playerY = ROOM_HEIGHT - tex_teoriIdle->h;
            isJumping = false;
        }

        char playerPos[50]; // Create a buffer to hold the text

        // Format the player position into a string
        snprintf(playerPos, sizeof(playerPos), "playerX: %.2f, playerY: %.2f", playerX, playerY);

        GRRLIB_PrintfTTF(20, 20, myFont, playerPos, 14, GRRLIB_WHITE);

        /*if ((Btndown & WPAD_BUTTON_MINUS) || (BtndownGC & PAD_TRIGGER_L)) {
            
        }*/

        if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) || 
            ((PAD_ButtonsHeld(0) & PAD_BUTTON_START) && 
            (PAD_ButtonsHeld(0) & PAD_BUTTON_X) && 
            (PAD_ButtonsHeld(0) & PAD_TRIGGER_Z))) {
            exit(0); // Exit from the game loop
        }

        GRRLIB_Render();
    }
    GRRLIB_FreeTTF(myFont); // Free the loaded TTF font when done
    GRRLIB_FreeTexture(tex_teoriIdle);
    GRRLIB_FreeTexture(tex_solid);
    GRRLIB_FreeTexture(tex_TESTBG);
    GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB
    }
}