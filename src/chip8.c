#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "chip8.h"
#include "utils/mystdint.h"

const u8 font_sheet[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

bool init_chip8(chip8_s *chip8,const char *rom_name){
    chip8->pc=PROGRAM_START;
    chip8->opcode=0;
    chip8->stack_pointer=0;
    chip8->index=0;

    memcpy(&chip8->memory[FONT_START], font_sheet, FONT_SIZE);

    // carrega a ROM
    FILE *file_rom = fopen(rom_name, "rb");
    if (!file_rom) {
        printf("Erro ao abrir a ROM: %s\n", rom_name);
        return false;
    }

    size_t max_size = MEMORY_RAM - PROGRAM_START;   // 3584 bytes
    size_t lidos = fread(&chip8->memory[PROGRAM_START], 1, max_size, file_rom);
    fclose(file_rom);

    if (lidos == 0) {
        printf("ROM vazia ou erro de leitura\n");
        return false;
    }

    return true;
}

bool emulation_cycle(chip8_s *chip8){

    // colocar 2 intruções de 1 byte no opcode 2 bytes 
    chip8->opcode=chip8->memory[chip8->pc]<<8 | chip8->memory[chip8->pc+1];

    chip8->pc+=2; //avançar 2 registros

    // registers sheet (não é a forma mais performatico mas é melhor
    // para escrita e entendimento das funcionalides dos registros)

    u8  x   = (chip8->opcode >> 8) & 0xF;
    u8  y   = (chip8->opcode >> 4) & 0xF;
    u8  n   = chip8->opcode       & 0xF;
    u8  nn  = chip8->opcode       & 0xFF;
    u16 nnn = chip8->opcode       & 0xFFF;

    switch (chip8->opcode & 0xF000) {
        case 0x0000:
            if (chip8->opcode == 0x00E0) {          // 00E0: limpa a tela
                memset(chip8->display, 0, sizeof(chip8->display));
            } else if (chip8->opcode == 0x00EE) {   // 00EE: return
                chip8->stack_pointer--;
                chip8->pc = chip8->stack[chip8->stack_pointer];
            } else {
                printf("Opcode desconhecido: 0x%04X\n", chip8->opcode);
                return false;
            }
            break;

        case 0x1000:                                // 1NNN: jump
            chip8->pc = nnn;
            break;

        case 0x2000:                                // 2NNN: call
            chip8->stack[chip8->stack_pointer] = chip8->pc;
            chip8->stack_pointer++;
            chip8->pc = nnn;
            break;

        case 0x3000:                                // 3XNN: pula se Vx == NN
            if (chip8->v_registers[x] == nn) chip8->pc += 2;
            break;

        case 0x4000:                                // 4XNN: pula se Vx != NN
            if (chip8->v_registers[x] != nn) chip8->pc += 2;
            break;

        case 0x5000:                                // 5XY0: pula se Vx == Vy
            if (chip8->v_registers[x] == chip8->v_registers[y]) chip8->pc += 2;
            break;

        case 0x6000:                                // 6XNN: Vx = NN
            chip8->v_registers[x] = nn;
            break;

        case 0x7000:                                // 7XNN: Vx += NN (sem flag)
            chip8->v_registers[x] += nn;
            break;

        case 0x8000:
            switch (n) {
            case 0x0: chip8->v_registers[x]  = chip8->v_registers[y]; break;
            case 0x1: chip8->v_registers[x] |= chip8->v_registers[y]; break;
            case 0x2: chip8->v_registers[x] &= chip8->v_registers[y]; break;
            case 0x3: chip8->v_registers[x] ^= chip8->v_registers[y]; break;

            case 0x4: {                         // 8XY4: soma com carry
                u16 soma = chip8->v_registers[x] + chip8->v_registers[y];
                chip8->v_registers[x] = soma & 0xFF;
                chip8->v_registers[0xF] = (soma > 0xFF);
                break;
            }
            case 0x5: {                         // 8XY5: Vx -= Vy
                u8 flag = (chip8->v_registers[x] >= chip8->v_registers[y]);
                chip8->v_registers[x] -= chip8->v_registers[y];
                chip8->v_registers[0xF] = flag;
                break;
            }
            case 0x6: {                         // 8XY6: Vx >>= 1
                u8 flag = chip8->v_registers[x] & 0x1;
                chip8->v_registers[x] >>= 1;
                chip8->v_registers[0xF] = flag;
                break;
            }
            case 0x7: {                         // 8XY7: Vx = Vy - Vx
                u8 flag = (chip8->v_registers[y] >= chip8->v_registers[x]);
                chip8->v_registers[x] = chip8->v_registers[y] - chip8->v_registers[x];
                chip8->v_registers[0xF] = flag;
                break;
            }
            case 0xE: {                         // 8XYE: Vx <<= 1
                u8 flag = (chip8->v_registers[x] >> 7) & 0x1;
                chip8->v_registers[x] <<= 1;
                chip8->v_registers[0xF] = flag;
                break;
            }
            default:
                printf("Opcode desconhecido: 0x%04X\n", chip8->opcode);
                return false;
            }
            break;

        case 0x9000:                                // 9XY0: pula se Vx != Vy
            if (chip8->v_registers[x] != chip8->v_registers[y]) chip8->pc += 2;
            break;

        case 0xA000:                                // ANNN: I = NNN
            chip8->index = nnn;
            break;

        case 0xB000:                                // BNNN: pc = NNN + V0
            chip8->pc = nnn + chip8->v_registers[0];
            break;

        case 0xC000:                                // CXNN: Vx = rand & NN
            chip8->v_registers[x] = (rand() & 0xFF) & nn;
            break;

        case 0xD000: {                              // DXYN: desenha sprite
            u8 px = chip8->v_registers[x] % DISPLAY_WIDTH;
            u8 py = chip8->v_registers[y] % DISPLAY_HEIGHT;
            chip8->v_registers[0xF] = 0;

            for (int row = 0; row < n; row++) {
                if (py + row >= DISPLAY_HEIGHT) break;      // corta na borda
                u8 sprite = chip8->memory[chip8->index + row];

                for (int col = 0; col < 8; col++) {
                    if (px + col >= DISPLAY_WIDTH) break;   // corta na borda
                    if (sprite & (0x80 >> col)) {
                        int pos = (px + col) + (py + row) * DISPLAY_WIDTH;
                        if (chip8->display[pos]) chip8->v_registers[0xF] = 1;
                        chip8->display[pos] ^= 1;
                    }
                }
            }
            break;
        }

        case 0xE000:
            if (nn == 0x9E) {                       // EX9E: pula se tecla Vx pressionada
                if (chip8->keypad[chip8->v_registers[x]]) chip8->pc += 2;
            } else if (nn == 0xA1) {                // EXA1: pula se tecla Vx NÃO pressionada
                if (!chip8->keypad[chip8->v_registers[x]]) chip8->pc += 2;
            } else {
                printf("Opcode desconhecido: 0x%04X\n", chip8->opcode);
                return false;
            }
            break;

        case 0xF000:
            switch (nn) {
            case 0x07:                              // FX07: Vx = delay
                chip8->v_registers[x] = chip8->delay_timer;
                break;

            case 0x0A: {                            // FX0A: espera uma tecla
                int achou = 0;
                for (int k = 0; k < 16; k++) {
                    if (chip8->keypad[k]) {
                        chip8->v_registers[x] = k;
                        achou = 1;
                        break;
                    }
                }
                if (!achou) chip8->pc -= 2;         // repete a instrução
                break;
            }
            case 0x15: chip8->delay_timer = chip8->v_registers[x]; break;
            case 0x18: chip8->sound_timer = chip8->v_registers[x]; break;
            case 0x1E: chip8->index += chip8->v_registers[x];      break;
            case 0x29: chip8->index = FONT_START + chip8->v_registers[x] * 5; break;

            case 0x33:                              // FX33: BCD
                chip8->memory[chip8->index]     =  chip8->v_registers[x] / 100;
                chip8->memory[chip8->index + 1] = (chip8->v_registers[x] / 10) % 10;
                chip8->memory[chip8->index + 2] =  chip8->v_registers[x] % 10;
                break;

            case 0x55:                              // FX55: salva V0..Vx na memória
                for (int i = 0; i <= x; i++)
                    chip8->memory[chip8->index + i] = chip8->v_registers[i];
                break;

            case 0x65:                              // FX65: carrega V0..Vx da memória
                for (int i = 0; i <= x; i++)
                    chip8->v_registers[i] = chip8->memory[chip8->index + i];
                break;

            default:
                printf("Opcode desconhecido: 0x%04X\n", chip8->opcode);
                return false;
            }
            break;

        default:
            printf("Opcode desconhecido: 0x%04X\n", chip8->opcode);
            return false;
    }

    return true;
}
