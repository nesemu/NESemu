//
// Created by Ethan Williams on 2019-02-04.
//

#include "PPUmemory.h"

PPUmemory::PPUmemory() {}

PPUmemory::~PPUmemory() {

}

uint8_t PPUmemory::read_byte(uint16_t address) {
	// decode address, update/return read buffer
	uint8_t result = 0;
	if (address < 0x3F00) {
		result = read_buffer;
		if (address < 0x2000) {
			read_buffer = gamepak->read_CHR(address);
		} else {
			uint16_t index = gamepak->translate_nametable_address(address);
			read_buffer = nametable_vram[index];
		}
	} else {
		if (address % 4 == 0) {
			result = palette_RAM[0];
		} else result = palette_RAM[address % 32];
		uint16_t index = gamepak->translate_nametable_address(address);
		read_buffer = nametable_vram[index];
	}
	return result;
}

void PPUmemory::write_byte(uint16_t address, uint8_t value) {
	if (address < 0x2000) { // Pattern table stored on Gamepak
		gamepak->write_CHR(address, value);
	}
	else if (address < 0x3000) { // Nametable RAM
		uint16_t index = gamepak->translate_nametable_address(address);
		nametable_vram[index] = value;
	} else if (address >= 0x3F00 && address < 0x4000) { // Palette table
		if (address % 4 == 0) { // BG color
			palette_RAM[0] = value;
		} else palette_RAM[address % 32] = value;
	}
}

uint8_t PPUmemory::read_byte_OAM(uint8_t address) {
	return OAM.data[address];
}

void PPUmemory::write_byte_OAM(uint8_t address, uint8_t value) {
	OAM.data[address] = value;
}
