//
// Created by Ethan Williams on 2019-01-29.
//

#ifndef NESEMU_PPU_H
#define NESEMU_PPU_H

#include <stdint.h>
#include "utils.h"

/* union regtype taken from https://bisqwit.iki.fi/jutut/kuvat/programming_examples/nesemu1/nesemu1.cc */

union PPUregtype // PPU register file
{
		uint64_t value;
		// Reg0 (write)                  // Reg1 (write)                  // Reg2 (read)
		RegBit<0,8,uint64_t> sysctrl;    RegBit< 8,8,uint64_t> dispctrl;  RegBit<16,8,uint64_t> status;
		RegBit<0,2,uint64_t> BaseNTA;    RegBit< 8,1,uint64_t> Grayscale; RegBit<21,1,uint64_t> SPoverflow;
		RegBit<2,1,uint64_t> Inc;        RegBit< 9,1,uint64_t> ShowBG8;   RegBit<22,1,uint64_t> SP0hit;
		RegBit<3,1,uint64_t> SPaddr;     RegBit<10,1,uint64_t> ShowSP8;   RegBit<23,1,uint64_t> InVBlank;
		RegBit<4,1,uint64_t> BGaddr;     RegBit<11,1,uint64_t> ShowBG;    // Reg3 (write)
		RegBit<5,1,uint64_t> SPsize;     RegBit<12,1,uint64_t> ShowSP;    RegBit<24,8,uint64_t> OAMaddr;
		RegBit<6,1,uint64_t> SlaveFlag;  RegBit<11,2,uint64_t> ShowBGSP;  //RegBit<24,2,uint64_t> OAMdata;
		RegBit<7,1,uint64_t> NMIenabled; RegBit<13,3,uint64_t> EmpRGB;    RegBit<26,6,uint64_t> OAMindex;

		RegBit<32,8,uint64_t> OAMdata; // Reg4 (read/write)
		RegBit<40,8,uint64_t> scroll; // Reg5 (write x2)
		RegBit<48,8,uint64_t> PPUaddr; // Reg6 (write x2)
		RegBit<56,8,uint64_t> PPUdata; // Reg7 (read/write)
};


class PPU {
public:
		uint8_t read_register(uint8_t addr);
		void write_register(uint8_t addr, uint8_t value);

private:
		PPUregtype reg;
		int scanline_count;
		int pixel_count;

};


#endif //NESEMU_PPU_H
