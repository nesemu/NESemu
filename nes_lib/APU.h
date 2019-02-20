//
// Created by Kyle Miller on 2019-02-14.
//

#ifndef NESEMU_APU_H
#define NESEMU_APU_H

#include "../nes_apu/Nes_Apu.h"
#include "memory.h"

class NesCPUMemory;


int dmc_read(void *, cpu_addr_t addr);

class APU {
public:
    APU();
    ~APU();
    void power_up();
    uint8_t apu_read(uint16_t address);
    void apu_write(uint16_t address, uint8_t value);
    void assign_memory(NesCPUMemory *);
    friend int dmc_read(void *, cpu_addr_t addr);
    size_t step(int length);
    blip_sample_t *getSamples();
    void setTime(nes_cpu_clock_t time);
private:
    Nes_Apu *nes_apu;
    Blip_Buffer *buf;
    static const int OUT_SIZE = 4096;
    blip_sample_t outBuf[OUT_SIZE];
    NesCPUMemory * memory;
    nes_cpu_clock_t time;
};


#endif //NESEMU_APU_H
