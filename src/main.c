#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>

#include "chip8.h"
#include "emulation.h"
#include "utils/gfx.h"

const int teclas[16] = {
    KEY_X,   KEY_ONE, KEY_TWO,   KEY_THREE,   // 0, 1, 2, 3
    KEY_Q,   KEY_W,   KEY_E,     KEY_A,        // 4, 5, 6, 7
    KEY_S,   KEY_D,   KEY_Z,     KEY_C,        // 8, 9, A, B
    KEY_FOUR, KEY_R,  KEY_F,     KEY_V         // C, D, E, F
};

bool init(emulator_s *emulator,chip8_s *chip8,const char *patch_rom){
    init_emulator(emulator);
    
    if (!init_chip8(chip8,patch_rom)) return false;
    return true;
}

int main(int argc, char *argv[])
{
    chip8_s chip8;
    emulator_s emulator;

    if (argc < 2){
        printf("ROM não informado!");
    }

    if (!init(&emulator,&chip8,argv[1])) return 0;

    InitAudioDevice();

    Sound beep_sound = LoadSound("resources/beep.mp3");
    if (!IsSoundValid(beep_sound)) {
        printf("Erro ao carregar o som\n");

        CloseAudioDevice();
        return 1;
    }

    InitWindow(SCREEN_WIDHT, SCREEN_HEIGHT, "Chip-8 Emulator");
    
    SetTargetFPS(60);


    while (!WindowShouldClose()){
        if (IsKeyPressed(KEY_BACKSPACE)) init(&emulator,&chip8,argv[1]);

        emulator_input(&emulator);
        if (!emulator.paused){
            for (int k = 0; k < 16; k++) {
                chip8.keypad[k]=IsKeyDown(teclas[k]);
            }

            for (int cycles = 0; cycles < emulator.cycles_per_frames; cycles++) {
                if (!emulation_cycle(&chip8)) break;
            }

            if (chip8.delay_timer > 0) chip8.delay_timer--;

            if (chip8.sound_timer > 0) {
                if (!IsSoundPlaying(beep_sound)) {
                    PlaySound(beep_sound);
                }
                chip8.sound_timer--;
            }else StopSound(beep_sound);
        }
        BeginDrawing();
            ClearBackground(BLACK);
            for(int x=0;x<DISPLAY_WIDTH;x++){
                for(int y=0;y<DISPLAY_HEIGHT;y++){
                    u8 display_pixel=chip8.display[x+y*DISPLAY_WIDTH];
                    if (display_pixel) DrawRectangle(x*UPSCALE_SIZE,y*UPSCALE_SIZE,UPSCALE_SIZE,UPSCALE_SIZE,WHITE);
                }
            }

            if (emulator.debug_mode) draw_debug(&chip8,&emulator);
        EndDrawing();
    }

    CloseWindow();
    CloseAudioDevice();

    return 0;
}