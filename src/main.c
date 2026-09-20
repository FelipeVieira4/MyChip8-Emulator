#include<stdlib.h>
#include<stdio.h>
#include<raylib.h>

#include "chip8.h"
#include "utils/gfx.h"
#include "utils/gfx.h"

const int teclas[16] = {
    KEY_X,   KEY_ONE, KEY_TWO,   KEY_THREE,   // 0, 1, 2, 3
    KEY_Q,   KEY_W,   KEY_E,     KEY_A,        // 4, 5, 6, 7
    KEY_S,   KEY_D,   KEY_Z,     KEY_C,        // 8, 9, A, B
    KEY_FOUR, KEY_R,  KEY_F,     KEY_V         // C, D, E, F
};

int main(int argc, char *argv[])
{
    chip8_s chip8;
    
    if (argc < 2){
        printf("ROM não informado!");
    }

    if (!init_chip8(&chip8,argv[1])) return 0;


    InitWindow(SCREEN_WIDHT, SCREEN_HEIGHT, "Chip-8 Emulator");
    SetTargetFPS(60);


    while (!WindowShouldClose()){

        for (int k = 0; k < 16; k++) {
            chip8.keypad[k]=IsKeyDown(teclas[k]);
        }

        for (int i = 0; i < 10; i++)emulation_cycle(&chip8);

        // 3. timers a 60 Hz (uma vez por frame)
        if (chip8.delay_timer > 0) chip8.delay_timer--;
        if (chip8.sound_timer > 0) chip8.sound_timer--;

        BeginDrawing();
            ClearBackground(BLACK);
            for(int x=0;x<DISPLAY_WIDTH;x++){
                for(int y=0;y<DISPLAY_HEIGHT;y++){
                    u8 display_pixel=chip8.display[x+y*DISPLAY_WIDTH];
                    if (display_pixel) DrawRectangle(x*UPSCALE_SIZE,y*UPSCALE_SIZE,UPSCALE_SIZE,UPSCALE_SIZE,WHITE);
                }
            }
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}