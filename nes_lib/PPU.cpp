//
// Created by Ethan Williams on 2019-01-29.
//

#include "PPU.h"
#include "cpu.h"

void PPU::assign_cpu(NesCpu *cpu) {
	this->cpu = cpu;
}

void PPU::power_up() {
	cycle_counter = nes_ppu_clock_t(0);
	scanline = 241;
	reg.ShowBG = 0;
}

void PPU::step(nes_ppu_clock_t cycles) {
	while (this->cycle_counter < cycles) {
		tick();

		bool isVisible = (scanline < POSTRENDER_SCANLINE);
		bool isVBlankLine = (scanline == POSTRENDER_SCANLINE+1);
		bool isPrerender = (scanline == PRERENDER_SCANLINE);

		bool shouldDraw = reg.ShowBGSP && isVisible && ((pixel > 0 && pixel < 257) || (pixel > 320 && pixel < 337));
		bool shouldFetch = reg.ShowBGSP && (isVisible || isPrerender) && ((pixel > 0 && pixel < 257) || (pixel > 320 && pixel < 337)) \
		&& pixel%8 == 0;

		if (shouldDraw) {
			render_pixel();
		}

		if (shouldFetch) {
			loadTile();

			if (pixel == 256) {
				incrementVert();
			}
			else {
				incrementcorseHorz();
			}
		}

		if (isVBlankLine && pixel == 1) {
			reg.VBlank = 1;
			if (reg.NMIenabled) cpu->requestNMI();
			//TODO: set output image here
		}
		else if(scanline == PRERENDER_SCANLINE && pixel == 1) {
			reg.VBlank = 0;
			reg.SP0hit = 0;
			reg.SPoverflow = 0;
		}

		if (reg.ShowBGSP && pixel == 257) {
			evaluate_sprites();
		}

		if (reg.ShowBGSP) {
			if ((isVisible || isPrerender) && pixel == 257) {
				copyHtoV();
			}
			else if (isPrerender && pixel == 304) {
				copyVtoV();
			}
		}

		cycle_counter++;
	}

}

uint8_t PPU::read_register(uint8_t address) {
	uint8_t result;
	switch (address) {
		case 2:
			PPU_address = 0;
			scroll_position = 0; // not sure about this one
			result = (uint8_t)reg.status;
			reg.VBlank = 0;
			return result;
		case 4:
			return memory.read_byte_OAM((uint8_t)reg.OAMaddr);
		case 7:
			result = memory.read_byte(PPU_address);
			PPU_address += reg.Inc ? 32 : 1;
			return result;
		default: return 0;
	}
}

void PPU::write_register(uint16_t address, uint8_t value) {
	address %= 0x8;
	switch (address) {
		case 0:
			reg.sysctrl = value; break;
		case 1:
			reg.dispctrl = value; break;
		case 3:
			reg.OAMaddr = value; break;
		case 4:
			memory.write_byte_OAM((uint8_t)reg.OAMaddr, value);
			reg.OAMaddr++;
			break;
		case 5:
			scroll_position = scroll_position << 8 | value; break;
		case 6:
			PPU_address = PPU_address << 8 | value; break;
		case 7:
			memory.write_byte(PPU_address, value);
			PPU_address += reg.Inc ? 32 : 1;
			break;
		default: break;
	}
}

void PPU::OAM_DMA(uint8_t *CPU_memory) {
	uint8_t i = (uint8_t)reg.OAMaddr;
	do {
		memory.write_byte_OAM(i, *CPU_memory);
		CPU_memory++;
	} while (++i != reg.OAMaddr);
}

void PPU::tick() {
	if (scanline == POSTRENDER_SCANLINE) {
		// do nothing
	} else if (scanline > POSTRENDER_SCANLINE && scanline < PRERENDER_SCANLINE) { // VBlank
		if (scanline == (POSTRENDER_SCANLINE+1) && pixel == 1) {
			if (reg.NMIenabled) cpu->requestNMI();
			reg.VBlank = 1;
		}
	} else if (scanline == PRERENDER_SCANLINE) {
		if (pixel == 1) {
			reg.VBlank = 0;
			reg.SP0hit = 0;
			reg.SPoverflow = 0;
		}
	} else if (!reg.ShowBGSP) {
		// rendering disabled, do nothing
	} else { // Render scanlines
		render_pixel();
	}

	/* Advance Cycle */
	pixel++;

	bool isOdd = (frame_counter&0x1) != 0;
	if (scanline == PRERENDER_SCANLINE && (pixel == PIXELS_PER_LINE || (pixel == (PIXELS_PER_LINE-1) && isOdd))) {
		scanline = 0;
		pixel = 0;
		frame_counter++;
	}
	else if (pixel == PIXELS_PER_LINE) {
		scanline++;
		pixel = 0;
	}
}

void PPU::load_scanline(unsigned scanline) {
	if (scanline >= POSTRENDER_SCANLINE || !reg.ShowBGSP) return;
	evaluate_sprites(scanline);
}

void PPU::evaluate_sprites(unsigned scanline) {
	// TODO: Support 8x16 sprite mode
	int secondary_counter = 0;
	for (uint8_t i = 0; i < OAM_ENTRIES && secondary_counter < 8; i++) {
		OAM_entry * entry = memory.read_entry_OAM(i);
		int ydiff = scanline - entry->y_coordinate;
		if (ydiff >= 0 && ydiff < 8) secondary_OAM[secondary_counter++] = entry;
	}
	while (secondary_counter < 8) {
		secondary_OAM[secondary_counter++] = nullptr;
	}

	for (int i = 0; i < 8; i++) {
		unsigned tile_index = reg.SPaddr << 12 | secondary_OAM[i]->tile_number << 8 | (scanline - secondary_OAM[i]->y_coordinate);
		sprite_data->bitmap_shift_reg[0] = memory.read_byte((uint16_t)tile_index);
		sprite_data->bitmap_shift_reg[1] = memory.read_byte((uint16_t)(tile_index+8));
		sprite_data[i].attribute.data = secondary_OAM[i]->attribute;
		sprite_data[i].x_position = secondary_OAM[i]->x_coordinate;
	}
}

void PPU::render_pixel() {

}

void PPU::loadTile() {

}
