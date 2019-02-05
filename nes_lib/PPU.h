//
// Created by Ethan Williams on 2019-01-29.
//

#ifndef NESEMU_PPU_H
#define NESEMU_PPU_H

#include <stdint.h>
#include <vector>
#include "utils.h"
#include "PPUmemory.h"
#include "nes_clock.h"

class NesCpu;

#define POSTRENDER_SCANLINE 240
#define PRERENDER_SCANLINE 261
#define PIXELS_PER_LINE 341

/* union regtype taken from https://bisqwit.iki.fi/jutut/kuvat/programming_examples/nesemu1/nesemu1.cc */

union PPUregtype // PPU register file
{
		uint32_t value;
		// Reg0 (write)                  // Reg1 (write)                  // Reg2 (read)
		RegBit<0,8,uint32_t> sysctrl;    RegBit< 8,8,uint32_t> dispctrl;  RegBit<16,8,uint32_t> status;
		RegBit<0,2,uint32_t> BaseNTA;    RegBit< 8,1,uint32_t> Grayscale; RegBit<21,1,uint32_t> SPoverflow;
		RegBit<2,1,uint32_t> Inc;        RegBit< 9,1,uint32_t> ShowBG8;   RegBit<22,1,uint32_t> SP0hit;
		RegBit<3,1,uint32_t> SPaddr;     RegBit<10,1,uint32_t> ShowSP8;   RegBit<23,1,uint32_t> VBlank;
		RegBit<4,1,uint32_t> BGaddr;     RegBit<11,1,uint32_t> ShowBG;    // Reg3 (write)
		RegBit<5,1,uint32_t> SPsize;     RegBit<12,1,uint32_t> ShowSP;    RegBit<24,8,uint32_t> OAMaddr;
		RegBit<6,1,uint32_t> SlaveFlag;  RegBit<11,2,uint32_t> ShowBGSP;  RegBit<24,2,uint32_t> OAMfield;
		RegBit<7,1,uint32_t> NMIenabled; RegBit<13,3,uint32_t> EmpRGB;    RegBit<26,6,uint32_t> OAMindex;
};

struct sprite_data_t {
		uint8_t bitmap_shift_reg[2];
		union {
			uint8_t data;
			RegBit<0,2,uint8_t> palette;
			RegBit<5,1,uint8_t> priority;
			RegBit<6,1,uint8_t> flip_H;
			RegBit<7,1,uint8_t> flip_V;
		} attribute;
		uint8_t x_position;
};


class PPU {
public:
		void assign_cpu(NesCpu * cpu);

		uint8_t read_register(uint8_t address);
		void write_register(uint16_t address, uint8_t value);
		void OAM_DMA(uint8_t *CPU_memory);

		void tick();
		void step(nes_ppu_clock_t cycles);
		void power_up();

private:
		NesCpu * cpu;
		PPUregtype reg;
		PPUmemory memory;
		uint16_t PPU_address;
		uint16_t scroll_position;

		unsigned pixel;
		unsigned scanline;
		size_t frame_counter = 0;
		nes_ppu_clock_t cycle_counter;

		uint16_t vram_address;
		uint16_t temp_vram_address;
		uint8_t x_fine_scroll;
		bool write_toggle;

		uint16_t bg_tile_shift_reg[2];
		uint16_t bg_palette_shift_reg[2];

		uint8_t pixels[256*240];

		OAM_entry * secondary_OAM[8];
		sprite_data_t sprite_data[8];

		void load_scanline(unsigned scanline);
		void evaluate_sprites(unsigned scanline);
		void render_pixel();
		void loadTile();
};


#endif //NESEMU_PPU_H
