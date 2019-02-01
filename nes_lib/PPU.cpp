//
// Created by Ethan Williams on 2019-01-29.
//

#include "PPU.h"

uint8_t PPU::read_register(uint8_t addr) {
	switch (addr) {
		case 0: return reg.sysctrl;
		case 1: return reg.dispctrl;
		case 2: return reg.status;
		case 3: return reg.OAMaddr;
		case 4: return reg.OAMdata;
		case 5: return reg.scroll;
		case 6: return reg.PPUaddr;
		case 7: return reg.PPUdata;
	}
	return 0;
}

void PPU::write_register(unsigned char, unsigned char) {

}