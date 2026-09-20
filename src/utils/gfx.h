#ifndef GFX_H
#define GFX_H

#include "../chip8.h"
#include "../emulation.h"

#define UPSCALE_SIZE 25

#define SCREEN_WIDHT    DISPLAY_WIDTH*UPSCALE_SIZE
#define SCREEN_HEIGHT   DISPLAY_HEIGHT*UPSCALE_SIZE

void draw_debug(chip8_s *chip8,emulator_s *emulator);

#endif