// cpu.cpp contains the code to successfully emulate the 6502 that was inside the NES
// YAY FOR THIS BEING FUN!
#include "cpu.h"
#include <iostream>

NesCpu::NesCpu(NesMemory * memory) {
    this -> RAM = memory;
}

NesCpu::~NesCpu() {

}

void NesCpu::power_up() {
    this->registers.P = 0x34;
    this->registers.A = 0x00;
    this->registers.X = 0x00;
    this->registers.Y = 0x00;
    this->registers.S = 0xFD;
}

uint16_t NesCpu::getAddrBasedOnMode(AddressingMode mode) {
    //TODO: Return the Proper Address based on PC and increment Program Counter so that it at the proper spot
    uint16_t finaladdr = 0;
    switch (mode) {
        case ADDR_MODE_IMPLICIT: {
            finaladdr = 0;
            break;
        }
        case ADDR_MODE_ACCUMULATOR: {
            finaladdr = 0;
            break;
        }
        case ADDR_MODE_IMMEDIATE: {
            finaladdr = uint16_t(this->registers.PC++);
            break;
        }
        case ADDR_MODE_ZP: {
            finaladdr = this->RAM->read_byte(this->registers.PC++);
            break;
        }
        case ADDR_MODE_ZPX: {
            finaladdr = ((this->RAM->read_byte(this->registers.PC++)) + this->registers.X);
            break;
        }
        case ADDR_MODE_ZPY: {
            finaladdr = ((this->RAM->read_byte(this->registers.PC++)) + this->registers.Y);
            break;
        }
        case ADDR_MODE_RELATIVE: {
            uint8_t offset = this->RAM->read_byte(this->registers.PC++);
            finaladdr = this->registers.PC;
            finaladdr += offset;
            break;
        }
        case ADDR_MODE_ABSOLUTE: {
            finaladdr = this->RAM->read_word(this->registers.PC);
            this->registers.PC += 2;
            break;
        }
    }
}


void NesCpu::step() {
    // TODO: Implement Interrupts

    //Fetch Op Code
    uint8_t opcode = this->RAM->read_byte(this->registers.PC++);
    const Instruction *currentInstruction = &this->instructions[opcode];

    if (currentInstruction -> addrMode == INVALID_OPCODE) {
        std::cerr << "Invalid OpCode Used" << currentInstruction -> name << std::endl;
        return;
    }

    uint16_t address = getAddrBasedOnMode(currentInstruction -> addrMode);

    currentInstruction -> instFunc(address);

}
