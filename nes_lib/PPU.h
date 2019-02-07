//
// Created by Ethan Williams on 2019-01-29.
//

#ifndef NESEMU_PPU_H
#define NESEMU_PPU_H

#include <stdint.h>
#include "utils.h"
#include "PPUmemory.h"
#include "nes_clock.h"

class NesCpu;

#define POSTRENDER_SCANLINE 240
#define PRERENDER_SCANLINE 261
#define PIXELS_PER_LINE 341
#define BACKGROUND_PALETTE_ADDRESS 0x3F00
#define SPRITE_PALETTE_ADDRESS 0x3F10
#define SCREEN_X 256
#define SCREEN_Y 240

#define MAKE_ARGB(R,G,B) (R<<16)|(G<<8)|B

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
		uint8_t y_position;
		uint8_t tile_index;
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

        explicit PPU(Gamepak * gamepak);
        ~PPU();

        void power_up();
        void assign_cpu(NesCpu * cpu);

		uint8_t read_register(uint8_t address);
		void write_register(uint16_t address, uint8_t value);
		void OAM_DMA(uint8_t *CPU_memory);
		uint32_t * get_frambuffer();

		bool step();

private:
		NesCpu * cpu;
		PPUregtype reg;
		PPUmemory * memory;

		unsigned pixel;
		unsigned scanline;
		size_t frame_counter = 0;
		nes_ppu_clock_t cycle_counter;

		union {
				uint16_t data;
				RegBit<0,5,uint16_t> coarseX;
				RegBit<5,5,uint16_t> coarseY;
				RegBit<10,2,uint16_t> NTselect;
				RegBit<12,3,uint16_t> fineY;
		} vram_address, temp_vram_address;

		uint8_t x_fine_scroll;
		bool write_toggle;

		uint32_t bg_pixels[16]{0};
		bool bg_pixel_valid[16]{false};

		uint32_t fg_sr_pixels[8]{0};
		bool fg_sr_pixels_valid[8]{false};

		uint32_t fg_pixels[256];
		bool fg_pixel_valid[256]{false};
		bool fg_pixel_infront[256]{false};
		bool fg_pixel_sp0[256]{false};

		uint32_t *frame_buffer;

		uint8_t read_buffer;

		OAM_entry * secondary_OAM[8];
		union {
			sprite_data_t entries[OAM_ENTRIES];
			uint8_t data[OAM_ENTRIES * sizeof(sprite_data_t)];
		} sprite_data;
		constexpr static uint32_t ntsc_palette [64] = {
				MAKE_ARGB(84,  84,  84),    MAKE_ARGB(0,  30, 116),    MAKE_ARGB(8,  16, 144),    MAKE_ARGB(48,   0, 136),   MAKE_ARGB(68,   0, 100),   MAKE_ARGB(92,   0,  48),   MAKE_ARGB(84,   4,   0),   MAKE_ARGB(60,  24,   0),   MAKE_ARGB(32,  42,   0),   MAKE_ARGB(8,  58,   0),   MAKE_ARGB(0,  64,   0),    MAKE_ARGB(0,  60,   0),    MAKE_ARGB(0,  50,  60),    MAKE_ARGB(0,   0,   0),   MAKE_ARGB(0, 0, 0), MAKE_ARGB(0, 0, 0),
				MAKE_ARGB(152, 150, 152),   MAKE_ARGB(8,  76, 196),    MAKE_ARGB(48,  50, 236),   MAKE_ARGB(92,  30, 228),   MAKE_ARGB(136,  20, 176),  MAKE_ARGB(160,  20, 100),  MAKE_ARGB(152,  34,  32),  MAKE_ARGB(120,  60,   0),  MAKE_ARGB(84,  90,   0),   MAKE_ARGB(40, 114,   0),  MAKE_ARGB(8, 124,   0),    MAKE_ARGB(0, 118,  40),    MAKE_ARGB(0, 102, 120),    MAKE_ARGB(0,   0,   0),   MAKE_ARGB(0, 0, 0), MAKE_ARGB(0, 0, 0),
				MAKE_ARGB(236, 238, 236),   MAKE_ARGB(76, 154, 236),   MAKE_ARGB(120, 124, 236),  MAKE_ARGB(176,  98, 236),  MAKE_ARGB(228,  84, 236),  MAKE_ARGB(236,  88, 180),  MAKE_ARGB(236, 106, 100),  MAKE_ARGB(212, 136,  32),  MAKE_ARGB(160, 170,   0),  MAKE_ARGB(116, 196,   0), MAKE_ARGB(76, 208,  32),   MAKE_ARGB(56, 204, 108),   MAKE_ARGB(56, 180, 204),   MAKE_ARGB(60,  60,  60),  MAKE_ARGB(0, 0, 0), MAKE_ARGB(0, 0, 0),
				MAKE_ARGB(236, 238, 236),   MAKE_ARGB(168, 204, 236),  MAKE_ARGB(188, 188, 236),  MAKE_ARGB(212, 178, 236),  MAKE_ARGB(236, 174, 236),  MAKE_ARGB(236, 174, 212),  MAKE_ARGB(236, 180, 176),  MAKE_ARGB(228, 196, 144),  MAKE_ARGB(204, 210, 120),  MAKE_ARGB(180, 222, 120), MAKE_ARGB(168, 226, 144),  MAKE_ARGB(152, 226, 180),  MAKE_ARGB(160, 214, 228),  MAKE_ARGB(160, 162, 160), MAKE_ARGB(0, 0, 0), MAKE_ARGB(0, 0, 0)
		};

		void evaluate_sprites();
		void render_pixel();

		void load_bg_tile();
		void populateShiftRegister(uint8_t, uint16_t, bool, int);

		void increment_x();
		void increment_y();
		void h_to_v();
		void v_to_v();
};


#endif //NESEMU_PPU_H
