#include "memory.h"

uint8_t NesMemory::read_byte(uint16_t address) {
    return this->ram[address];
}

void NesMemory::write_byte(uint16_t address, uint8_t value) {
    if (address > 0x7FFF) { // non-writable Gamepak ROM
        return
    }
    this->ram[address] = value;
}

uint16_t NesMemory::read_word(uint16_t address) {
    return (read_byte(address) + (uint16_t(read_byte(address+uint16_t(1))) << 8));
}

void NesMemory::write_word(uint16_t address, uint16_t value) {
    write_byte(address, value & 0xff); //TODO: Make sure this is okay
    write_byte(address+1, (value >> 8));
}

void NesMemory::stack_write_word(uint16_t address, uint16_t value) {
    write_byte(address, (value >> 8));
    write_byte(address-1, value & 0xff);
}

void NesMemory::map_memory(uint16_t address, char *data, std::size_t size) {
    for (int i = 0; i < size; ++i) {
        write_byte(address+i, data[i]);
    }
}
