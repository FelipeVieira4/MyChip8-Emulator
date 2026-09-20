#include <raylib.h>

#include "emulation.h"
#include "chip8.h"
#include "utils/mystdint.h"

bool init_emulator(emulator_s *emulator){
    emulator->cycles_per_frames=10;
    emulator->debug_mode=false;
    emulator->paused=false;

    return true;
}

void emulator_input(emulator_s *emulator){
    if (IsKeyPressed(KEY_ENTER)) emulator->debug_mode=!emulator->debug_mode;
    if (IsKeyPressed(KEY_P)) emulator->paused=!emulator->paused;

    if (IsKeyDown(KEY_UP)&(emulator->cycles_per_frames++<MAX_CYCLES))
        emulator->cycles_per_frames++;
    if (IsKeyDown(KEY_DOWN)&(emulator->cycles_per_frames-->MIN_CYCLES))
        emulator->cycles_per_frames--;
}