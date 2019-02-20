//
// Created by Kyle Miller on 2019-02-14.
//

#include "APU.h"

APU::APU() {
    nes_apu = new Nes_Apu();
    buf = new Blip_Buffer();

}

APU::~APU() {
    delete nes_apu;
    delete buf;
}
void APU::power_up() {
    buf -> sample_rate(96000);
    buf-> clock_rate(1789773);
    nes_apu ->output(buf);
    nes_apu -> dmc_reader(dmc_read,(void *)this);
}

void APU::assign_memory(NesCPUMemory * memory) {
    this->memory = memory;
}

uint8_t APU::apu_read(uint16_t address) {
    nes_apu->read_status(time.count());
}

void APU::apu_write(uint16_t address, uint8_t value) {
    nes_apu->write_register(time.count(), address, value);
}

size_t APU::step(int length) {
    nes_apu->end_frame(length);
    buf->end_frame(length);

    if (buf->samples_avail() >= OUT_SIZE) {
        return (size_t)buf->read_samples(outBuf, OUT_SIZE);
    }
    return 0;
}

blip_sample_t* APU::getSamples() {
    return outBuf;
}

void APU::setTime(nes_cpu_clock_t time) {
    this->time = time;
}

int dmc_read(void * apu, cpu_addr_t addr) {
    APU * newapu = (APU*)apu;
    return newapu->memory->read_byte(addr);
}
