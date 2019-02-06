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
			result = memory.read_byte(vram_address.data);
			vram_address.data += reg.Inc ? 32 : 1;
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
			memory.write_byte(vram_address.data, value);
			vram_address.data += reg.Inc ? 32 : 1;
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
	/* Advance Cycle */
	pixel++;
	bool isOdd = ((frame_counter & 0x1) != 0);
	if (scanline == PRERENDER_SCANLINE && (pixel == PIXELS_PER_LINE || (pixel == (PIXELS_PER_LINE-1) && isOdd))) {
		pixel = 0;
		scanline = 0;
		frame_counter++;
	}
	else if (pixel == PRERENDER_SCANLINE) {
		scanline++;
		pixel = 0;
	}

	bool isRendering = (bool)reg.ShowBGSP;
	bool isVisible = scanline < POSTRENDER_SCANLINE;
	bool isVBlank = scanline == (POSTRENDER_SCANLINE+1);
	bool isPrerender = scanline == PRERENDER_SCANLINE;

	bool isDrawing = isRendering && isVBlank && ((pixel > 0 && pixel < 256) || (pixel > 320 && pixel < 337));
	bool isFetching = isRendering && isVBlank && ((pixel > 0 && pixel < 256) || (pixel > 320 && pixel < 337)) && pixel%8==0;

	if (isDrawing) {
		render_pixel();
	}

	if (isFetching) {
		load_bg_tile();

		if (pixel == 256) {
			increment_y();
		}
		else {
			increment_x();
		}
	}

	if (isVBlank && pixel == 1) {
		reg.VBlank = 1;
		if (reg.NMIenabled) cpu->requestNMI();
		//TODO: OUTPUT IMAGE HERE
	}
	else if (isPrerender && pixel == 1) {
		reg.VBlank = 0;
		reg.SP0hit = 0;
		reg.SPoverflow = 0;
	}

	if (isRendering && pixel == 257) {
		evaluate_sprites(scanline); //TODO: @ETHAN Check to make srue this is okay
	}

	if (isRendering) {
		if ((isVisible || isPrerender) && pixel == 257) {
			h_to_v();
		}
		else if (isPrerender && pixel == 304) {
			v_to_v();
		}
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
	uint16_t nametable_tile_address = (uint16_t)(0x2000 | (vram_address.data & 0x0FFF));
	uint16_t nametable_attribute_address = (uint16_t)(0x23C0 | (vram_address.data & 0x0C00) | \
		((vram_address.data >> 4) & 0x38) | ((vram_address.data >> 2) & 0x07));
	uint16_t shift = (uint16_t)((vram_address.data & 0x2) | ((vram_address.data & 0x40) >> 4));
	uint8_t attribute_bits = (uint8_t )((memory.read_byte(nametable_attribute_address) >> shift) & 0x3);

	uint8_t pattern_tile = memory.read_byte(nametable_tile_address);

	uint16_t pattern_table_address = (uint16_t)((reg.BGaddr << 12) + (pattern_tile << 4) + vram_address.fineY);
	populateShiftRegister(pattern_tile, attribute_bits, false, vram_address.fineY);
	bg_tile_shift_reg[0] = (memory.read_byte(pattern_table_address) << 8) | (bg_tile_shift_reg[0] & 0xFFFF0000);
	bg_tile_shift_reg[1] = (memory.read_byte(pattern_table_address + 0x1000) << 8) | (bg_tile_shift_reg[1] & 0xFFFF0000); // next bitplane


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
		unsigned tile_index = (reg.SPaddr << 12) | (secondary_OAM[i]->tile_number << 8) | (scanline - secondary_OAM[i]->y_coordinate);
		sprite_data->bitmap_shift_reg[0] = memory.read_byte((uint16_t)tile_index);
		sprite_data->bitmap_shift_reg[1] = memory.read_byte((uint16_t)(tile_index+8));
		sprite_data[i].attribute.data = secondary_OAM[i]->attribute;
		sprite_data[i].x_position = secondary_OAM[i]->x_coordinate;
	}
}

void PPU::render_pixel() {

}

void PPU::populateShiftRegister(uint8_t pattern_tile, uint16_t attribute_bits, bool is_foreground, int y_offset) {
	uint16_t base_address;
	uint16_t base_palette_address;
	bool show_pixels;

	if (is_foreground) {
		if (reg.SPsize) {
			if ((pattern_tile&0x1) == 0 ){
				base_address = 0x0000;
			}
			else {
				base_address = 0x1000;
			}

			if (y_offset > 7) {
				pattern_tile |= 0x1;
				y_offset -= 8;
			}
			else {
				pattern_tile &=0x10;
			}
		}
		else {
			base_address = reg.SPaddr ? (uint16_t) 0x1000 : (uint16_t) 0x0000;
		}
		base_palette_address = (uint16_t)SPRITE_PALETTE_ADDRESS;
		show_pixels = (bool)reg.ShowSP;
	}
	else {
		base_address = reg.BGaddr ? (uint16_t) 0x1000 : (uint16_t) 0x0000;
		base_palette_address = (uint16_t)BACKGROUND_PALETTE_ADDRESS;
		show_pixels = (bool)reg.ShowBG;
	}

	uint8_t low = memory.read_byte(base_address + (uint16_t)pattern_tile*16 + (uint16_t)y_offset);
	uint8_t high = memory.read_byte(base_address + (uint16_t)pattern_tile*16 + (uint16_t)y_offset + (uint16_t)8);

	for (int i = 0; i < 8; i++) {
		bool lowBit = ((low>>uint(7-i))&0x1) != 0;
		bool highBit = ((low>>uint(7-i))&0x1) != 0;

		uint16_t index = 0;
		if (highBit) {
			index |= 0x2;
		}
		if (lowBit) {
			index |= 0x1;
		}

		if (index == 0 || !show_pixels) {
			bg_pixels[i+8] = 0;
		}
		else {
			uint8_t palette_index = (memory.read_byte(base_palette_address+(attribute_bits<<2)+index) & (uint8_t) 0x3F);
			bg_pixels[i+8] = ntsc_palette[palette_index];
		}
	}
}
