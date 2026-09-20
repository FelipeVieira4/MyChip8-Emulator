#include<raylib.h>

#include "gfx.h"
#include "../chip8.h"
#include "../emulation.h"

void draw_debug(chip8_s *chip8,emulator_s *emulator){
    DrawText(TextFormat("DEBUG ON"),10,10,32,GREEN);
    
    DrawText(TextFormat("CHIP-8 INFO:"),10,44,26,GREEN);
    DrawText(TextFormat("PC: 0x%04X\n", chip8->opcode),10,74,22,RED);
    DrawText(TextFormat("SP: 0x%04X\n", chip8->stack_pointer),10,96,22,RED);
    DrawText(TextFormat("I: 0x%04X\n", chip8->index),10,118,22,RED);
    
    DrawText(TextFormat("EMULATOR INFO:"),10,140,32,GREEN);
    DrawText(TextFormat("CYCLES: %hhd\n", emulator->cycles_per_frames),10,172,22,RED);
    DrawText(TextFormat("PAUSED: %hhd\n", emulator->paused),10,194,22,RED);

    return;
}