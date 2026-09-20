/*
	Header file of the Chip-8 Emulator
	by:Felipe Vieira
	Last Update:19/08/2026
*/

#ifndef _CHIP_8_H_
#define _CHIP_8_H_

#include "utils/mystdint.h"
#include "stdbool.h"

#define PROGRAM_START 0x200
#define MEMORY_RAM 4096	// 4 kbyte of memory RAM

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define FONT_SIZE  80
#define FONT_START 0x50

typedef struct {
	u16 opcode;
	u16 index;
	u16 pc;
	u16 stack_pointer;
	u16 stack[16];
	u8 memory[MEMORY_RAM];
	u8 v_registers[16];

	u8 display[DISPLAY_WIDTH * DISPLAY_HEIGHT];
	u8 delay_timer;
	u8 sound_timer;

	u8 keypad[16];
}chip8_s;

bool init_chip8(chip8_s *chip8,const char *rom_name);
bool emulation_cycle(chip8_s *chip8);
//void unload_chip8(chip8_s *chip8);

#endif