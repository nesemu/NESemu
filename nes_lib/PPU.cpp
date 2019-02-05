//
// Created by Ethan Williams on 2019-01-29.
//

#include "PPU.h"
#include "cpu.h"

void PPU::assign_cpu(NesCpu *cpu) {
	this->cpu = cpu;
}

uint8_t PPU::read_register(uint8_t address) {
	uint8_t result;
	switch (address) {
		case 2:
			write_toggle = 0;
			result = (uint8_t)reg.status;
			reg.VBlank = 0;
			return result;
		case 4:
			return memory.read_byte_OAM((uint8_t)reg.OAMaddr);
		case 7:
			result = memory.read_byte(vram_address);
			vram_address += reg.Inc ? 32 : 1;
			return result;
		default: return 0;
	}
}

void PPU::write_register(uint16_t address, uint8_t value) {
	address %= 0x8;
	switch (address) {
		case 0:
			reg.sysctrl = value;
			temp_vram_address.NTselect = reg.BaseNTA;
			break;
		case 1:
			reg.dispctrl = value; break;
		case 3:
			reg.OAMaddr = value; break;
		case 4:
			memory.write_byte_OAM((uint8_t)reg.OAMaddr, value);
			reg.OAMaddr++;
			break;
		case 5:
			if (write_toggle == 0) {
				temp_vram_address.coarseX = value >> 3;
				x_fine_scroll = (uint8_t)(value % 0b111);
			} else {
				temp_vram_address.fineY = value & 0b111;
				temp_vram_address.coarseY = value >> 3;
			}
			write_toggle = !write_toggle;
			break;
		case 6:
			 if (write_toggle == 0) {
			 	temp_vram_address.data = ((value & 0x003F) << 8) + (temp_vram_address.data & 0xFF00);
			 } else {
			 	temp_vram_address.data = (temp_vram_address.data & 0xFF00) + value;
			 }
			 write_toggle = !write_toggle;
			 break;
		case 7:
			memory.write_byte(vram_address, value);
			vram_address += reg.Inc ? 32 : 1;
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
		if (scanline == POSTRENDER_SCANLINE && pixel == 1) {
			if (reg.NMIenabled) cpu->requestNMI();
			reg.VBlank = 1;
		}
	} else if (scanline == PRERENDER_SCANLINE) {
		if (pixel == 1) {
			reg.VBlank = 0;
			reg.SP0hit = 0;
			reg.SPoverflow = 0;
		}
	} else if (reg.ShowBGSP) {
		// rendering disabled, do nothing
	} else { // Render scanlines
		render_pixel();
	}

	/* Advance Cycle */
	pixel++;
	if (pixel == PIXELS_PER_LINE || (pixel < PIXELS_PER_LINE && scanline == PRERENDER_SCANLINE && !(frame_counter % 2))) {
		pixel = 0;
		load_scanline(++scanline);
	}
	if (scanline > PRERENDER_SCANLINE) {
		scanline = 0;
		frame_counter++;
	}
	cycle_counter++;
}

void PPU::load_scanline(unsigned scanline) {
	if (scanline >= POSTRENDER_SCANLINE || !reg.ShowBGSP) return;
	load_bg_tile();
	increment_x();
	load_bg_tile();

	evaluate_sprites(scanline);
}

void PPU::load_bg_tile() {
	uint16_t nametable_tile_address = 0x2000 | (vram_address.data & 0x0FFF);
	uint16_t nametable_attribute_address = 0x23C0 | (vram_address.data & 0x0C00) | \
		((vram_address.data >> 4) & 0x38) | ((vram_address.data >> 2) & 0x07);
	uint8_t pattern_tile = memory.read_byte(nametable_tile_address);
	uint16_t pattern_table_address = reg.BGaddr << 12 + pattern_tile << 4 + vram_address.fineY;
	bg_tile_shift_reg[0] = memory.read_byte(pattern_table_address) << 8 | (bg_tile_shift_reg[0] & 0xFFFF0000);
	bg_tile_shift_reg[1] = memory.read_byte(pattern_table_address + 0x1000) << 8 | (bg_tile_shift_reg[1] & 0xFFFF0000); // next bitplane


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
