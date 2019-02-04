//
// Created by Ethan Williams on 2019-01-29.
//

#ifndef NESEMU_PPU_H
#define NESEMU_PPU_H

#include <stdint.h>
#include "utils.h"
#include "PPUmemory.h"

/* union regtype taken from https://bisqwit.iki.fi/jutut/kuvat/programming_examples/nesemu1/nesemu1.cc */

union PPUregtype // PPU register file
{
		uint32_t value;
		// Reg0 (write)                  // Reg1 (write)                  // Reg2 (read)
		RegBit<0,8,uint32_t> sysctrl;    RegBit< 8,8,uint32_t> dispctrl;  RegBit<16,8,uint32_t> status;
		RegBit<0,2,uint32_t> BaseNTA;    RegBit< 8,1,uint32_t> Grayscale; RegBit<21,1,uint32_t> SPoverflow;
		RegBit<2,1,uint32_t> Inc;        RegBit< 9,1,uint32_t> ShowBG8;   RegBit<22,1,uint32_t> SP0hit;
		RegBit<3,1,uint32_t> SPaddr;     RegBit<10,1,uint32_t> ShowSP8;   RegBit<23,1,uint32_t> InVBlank;
		RegBit<4,1,uint32_t> BGaddr;     RegBit<11,1,uint32_t> ShowBG;    // Reg3 (write)
		RegBit<5,1,uint32_t> SPsize;     RegBit<12,1,uint32_t> ShowSP;    RegBit<24,8,uint32_t> OAMaddr;
		RegBit<6,1,uint32_t> SlaveFlag;  RegBit<11,2,uint32_t> ShowBGSP;  RegBit<24,2,uint32_t> OAMfield;
		RegBit<7,1,uint32_t> NMIenabled; RegBit<13,3,uint32_t> EmpRGB;    RegBit<26,6,uint32_t> OAMindex;
};


class PPU {
public:
		uint8_t read_register(uint8_t address);
		void write_register(uint16_t address, uint8_t value);

		void OAM_DMA(uint8_t *CPU_memory);
private:
		PPUregtype reg;
		PPUmemory memory;
		uint16_t PPU_address;
		uint16_t scroll_position;
		bool even_odd;


};


#endif //NESEMU_PPU_H
