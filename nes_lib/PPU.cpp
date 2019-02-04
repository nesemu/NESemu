//
// Created by Ethan Williams on 2019-01-29.
//

#include "PPU.h"

uint8_t PPU::read_register(uint8_t address) {
	uint8_t result;
	switch (address) {
		case 2:
			PPU_address = 0;
			scroll_position = 0; // not sure about this one
			result = (uint8_t)reg.status;
			reg.InVBlank = 0;
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
