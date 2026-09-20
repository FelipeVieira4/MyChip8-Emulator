#ifndef EMULATOR_H
#define EMULATOR_H

#include "utils/mystdint.h"
#include "chip8.h"

#define MAX_CYCLES 100
#define MIN_CYCLES 1

typedef struct
{
    u8 cycles_per_frames;
    bool debug_mode;
    bool paused;
}emulator_s;

bool init_emulator(emulator_s *emulator);
void emulator_input(emulator_s *emulator);

#endif