/*
 * memory.h file contains the data structure for the NES's internal RAM
 */

#ifndef NESEMU_MEMORY_H
#define NESEMU_MEMORY_H

#include <cstdint>
#include <cstdlib>

#define RAM_SIZE 0x10000 // 64K

class NesMemory {
private:
    uint8_t ram[RAM_SIZE] = {0};
public:
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t value);
    uint16_t read_word(uint16_t address);
    void write_word(uint16_t address, uint16_t value);
    void stack_write_word(uint16_t address, uint16_t value);
    void map_memory(uint16_t address, char * data, size_t size);
};

#endif //NESEMU_MEMORY_H
