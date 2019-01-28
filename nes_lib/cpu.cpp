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
