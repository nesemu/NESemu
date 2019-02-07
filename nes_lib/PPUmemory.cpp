//
// Created by Ethan Williams on 2019-02-04.
//

#include "PPUmemory.h"

PPUmemory::PPUmemory(Gamepak * gamepak) {
	this->gamepak = gamepak;
	for (int i = 0; i < VRAM_SIZE; i++) {
		nametable_vram[i] = 0;
	}
	for (int i = 0; i < 32; i++) {
		palette_RAM[i] = 0;
	}
}

PPUmemory::~PPUmemory() {

}

uint8_t PPUmemory::read_byte(uint16_t address) {
	// decode address, update/return read buffer
	address = map_memory(address);

	uint8_t result = 0;
	if (address < 0x2000) {
		result = gamepak->read_CHR(address);
	}
	else {
		result = nametable_vram[address];
	}
	return result;
}

void PPUmemory::write_byte(uint16_t address, uint8_t value) {
	address = map_memory(address);

	if (address < 0x2000) {
		gamepak->write_CHR(address, value);
	}
	else {
		nametable_vram[address] = value;
	}
}

uint8_t PPUmemory::read_byte_OAM(uint8_t address) {
	return OAM.data[address];
}

void PPUmemory::write_byte_OAM(uint8_t address, uint8_t value) {
	OAM.data[address] = value;
}

OAM_entry * PPUmemory::read_entry_OAM(uint8_t index) {
	return &OAM.entries[index];
}

uint16_t PPUmemory::map_memory(uint16_t address) {
	address &= 0x3FFF;

	if (address == 0x3F10 || address == 0x3F14 || address == 0x3F18 || address == 0x3F1C) {
		address -= 0x10;
	}
	else if (address >= 0x2000 && address <= 0x2FFF) {
		address = gamepak->translate_nametable_address(address);
	}
	return address;
}
