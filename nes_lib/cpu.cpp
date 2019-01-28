// cpu.cpp contains the code to successfully emulate the 6502 that was inside the NES
// YAY FOR THIS BEING FUN!
#include "cpu.h"

NesCpu::NesCpu() {

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
