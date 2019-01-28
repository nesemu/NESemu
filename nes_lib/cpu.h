/*
 * Header File for CPU.cpp that defines the CPU Class Plus other Necessary Things
 */


#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

#include <cstdint>
#include <string>
#include "memory.h"

// Set Some Mask Definitons
#define CARRY_MASK 0x1
#define ZERO_MASK 0x2
#define INTERRUPT_DISABLE_MASK 0x4
#define DECIMAL_MASK 0x8

// The B Flag which doesn't actually exist in the P Register
// Instead these bits only exist when the flags are pushed to
// the stack, depending on how they were pushed determines which
// bit is 1 myand 0

#define B_MASK 0x10
#define I_MASK 0x20
#define OVERFLOW_MASK 0x40
#define NEGATIVE_MASK 0x80

// Define addresses for the interrupt vectors

#define NMI_INTERRUPT_VECTOR 0xFFFA
#define RESET_INTERRUPT_VECTOR 0xFFFC
#define IRQ_INTERRUPT_VECTOR 0xFFFE

// Define Stack Offset
#define STACK_OFFSET 0x100

struct CpuRegisters {
    uint8_t A; // Accumlator
    uint8_t X; // X Register
    uint8_t Y; // Y Register
    uint8_t S; // Stack Pointer
    uint8_t P; // Status Register
    uint16_t PC; // Program Counter
};

enum AddressingMode {
    ADDR_MODE_IMPLICIT,
    ADDR_MODE_ACCUMULATOR,
    ADDR_MODE_IMMEDIATE,
    ADDR_MODE_ZP,
    ADDR_MODE_ABSOLUTE,
    ADDR_MODE_RELATIVE,
    ADDR_MODE_INDIRECT,
    ADDR_MODE_ZPX,
    ADDR_MODE_ZPY,
    ADDR_MODE_ABSOLUTEX,
    ADDR_MODE_ABSOLUTEY,
    ADDR_MODE_INDIRECTX,
    ADDR_MODE_INDIRECTY,
    INVALID_OPCODE
};

struct Instruction {
     std::string name;
     void (*instFunc)(uint16_t address);
     uint16_t size;
     int baseNumCycles;
     int numPageCrossCycles;
     AddressingMode addrMode;
};

// Function Prototypes for all of the Specific OPCODES
void brk(uint16_t address);
void ora(uint16_t address);
void slo(uint16_t address);
void dop(uint16_t address);
void asl(uint16_t address);
void asla(uint16_t address);
void php(uint16_t address);
void anc(uint16_t address);
void top(uint16_t address);
void bpl(uint16_t address);
void clc(uint16_t address);
void nop(uint16_t address);
void jsr(uint16_t address);
void myand(uint16_t address);
void rla(uint16_t address);
void bit(uint16_t address);
void rol(uint16_t address);
void rola(uint16_t address);
void plp(uint16_t address);
void bmi(uint16_t address);
void sec(uint16_t address);
void rti(uint16_t address);
void eor(uint16_t address);
void sre(uint16_t address);
void lsr(uint16_t address);
void lsra(uint16_t address);
void pha(uint16_t address);
void alr(uint16_t address);
void jmp(uint16_t address);
void bvc(uint16_t address);
void cli(uint16_t address);
void rts(uint16_t address);
void adc(uint16_t address);
void rra(uint16_t address);
void ror(uint16_t address);
void rora(uint16_t address);
void pla(uint16_t address);
void arr(uint16_t address);
void bvs(uint16_t address);
void sei(uint16_t address);
void sta(uint16_t address);
void aax(uint16_t address);
void sty(uint16_t address);
void stx(uint16_t address);
void dey(uint16_t address);
void txa(uint16_t address);
void bcc(uint16_t address);
void tya(uint16_t address);
void txs(uint16_t address);
void ldy(uint16_t address);
void lda(uint16_t address);
void ldx(uint16_t address);
void lax(uint16_t address);
void tay(uint16_t address);
void tax(uint16_t address);
void lxa(uint16_t address);
void bcs(uint16_t address);
void clv(uint16_t address);
void tsx(uint16_t address);
void cpy(uint16_t address);
void cmp(uint16_t address);
void dcp(uint16_t address);
void dec(uint16_t address);
void iny(uint16_t address);
void dex(uint16_t address);
void sax(uint16_t address);
void bne(uint16_t address);
void cld(uint16_t address);
void cpx(uint16_t address);
void sbc(uint16_t address);
void isc(uint16_t address);
void inc(uint16_t address);
void inx(uint16_t address);
void beq(uint16_t address);
void sed(uint16_t address);

class NesCpu {
private:
    // Things the CPU Needs to operate
    CpuRegisters registers;
    //TODO: Need some kind of time keeping thing to keep track of cycles
    //TODO: Need Some Kind of Memory (READ RAM) Access Here
    NesMemory *RAM;
    const Instruction instructions[256] = {
        /* 0x00 */ {"BRK", brk, 1, 7, 0, ADDR_MODE_IMPLICIT},
        /* 0x01 */ {"ORA", ora, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0x02 */ {"0x02", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x03 */ {"SLO", slo, 2, 8, 0, ADDR_MODE_INDIRECTX},
        /* 0x04 */ {"DOP", dop, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x05 */ {"ORA", ora, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x06 */ {"ASL", asl, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x07 */ {"SLO", slo, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x08 */ {"PHP", php, 1, 3, 0, ADDR_MODE_IMPLICIT},
        /* 0x09 */ {"ORA", ora, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x0A */ {"ASL", asla, 1, 2, 0, ADDR_MODE_ACCUMULATOR},
        /* 0x0B */ {"ANC", anc, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x0C */ {"TOP", top, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x0D */ {"ORA", ora, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x0E */ {"ASL", asl, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x0F */ {"SLO", slo, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x10 */ {"BPL", bpl, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0x11 */ {"ORA", ora, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0x12 */ {"x12", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x13 */ {"SLO", slo, 2, 8, 0, ADDR_MODE_INDIRECTY},
        /* 0x14 */ {"DOP", dop, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x15 */ {"ORA", ora, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x16 */ {"ASL", asl, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x17 */ {"SLO", slo, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x18 */ {"CLC", clc, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x19 */ {"ORA", ora, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0x1A */ {"NOP", nop, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x1B */ {"SLO", slo, 3, 7, 0, ADDR_MODE_ABSOLUTEY},
        /* 0x1C */ {"TOP", top, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x1D */ {"ORA", ora, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x1E */ {"ASL", asl, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x1F */ {"SLO", slo, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x20 */ {"JSR", jsr, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x21 */ {"AND", myand, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0x22 */ {"x22", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x23 */ {"RLA", rla, 2, 8, 0, ADDR_MODE_INDIRECTX},
        /* 0x24 */ {"BIT", bit, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x25 */ {"AND", myand, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x26 */ {"ROL", rol, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x27 */ {"RLA", rla, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x28 */ {"PLP", plp, 1, 4, 0, ADDR_MODE_IMPLICIT},
        /* 0x29 */ {"AND", myand, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x2A */ {"ROL", rola, 1, 2, 0, ADDR_MODE_ACCUMULATOR},
        /* 0x2B */ {"ANC", anc, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x2C */ {"BIT", bit, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x2D */ {"AND", myand, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x2E */ {"ROL", rol, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x2F */ {"RLA", rla, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x30 */ {"BMI", bmi, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0x31 */ {"AND", myand, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0x32 */ {"x32", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x33 */ {"RLA", rla, 2, 8, 0, ADDR_MODE_INDIRECTY},
        /* 0x34 */ {"DOP", dop, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x35 */ {"AND", myand, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x36 */ {"ROL", rol, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x37 */ {"RLA", rla, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x38 */ {"SEC", sec, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x39 */ {"AND", myand, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0x3A */ {"NOP", nop, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x3B */ {"RLA", rla, 3, 7, 0, ADDR_MODE_ABSOLUTEY},
        /* 0x3C */ {"TOP", top, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x3D */ {"AND", myand, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x3E */ {"ROL", rol, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x3F */ {"RLA", rla, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x40 */ {"RTI", rti, 1, 6, 0, ADDR_MODE_IMPLICIT},
        /* 0x41 */ {"EOR", eor, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0x42 */ {"x42", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x43 */ {"SRE", sre, 2, 8, 0, ADDR_MODE_INDIRECTX},
        /* 0x44 */ {"DOP", dop, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x45 */ {"EOR", eor, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x46 */ {"LSR", lsr, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x47 */ {"SRE", sre, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x48 */ {"PHA", pha, 1, 3, 0, ADDR_MODE_IMPLICIT},
        /* 0x49 */ {"EOR", eor, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x4A */ {"LSR", lsra, 1, 2, 0, ADDR_MODE_ACCUMULATOR},
        /* 0x4B */ {"ALR", alr, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x4C */ {"JMP", jmp, 3, 3, 0, ADDR_MODE_ABSOLUTE},
        /* 0x4D */ {"EOR", eor, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x4E */ {"LSR", lsr, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x4F */ {"SRE", sre, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x50 */ {"BVC", bvc, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0x51 */ {"EOR", eor, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0x52 */ {"x52", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x53 */ {"SRE", sre, 2, 8, 0, ADDR_MODE_INDIRECTY},
        /* 0x54 */ {"DOP", dop, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x55 */ {"EOR", eor, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x56 */ {"LSR", lsr, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x57 */ {"SRE", sre, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x58 */ {"CLI", cli, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x59 */ {"EOR", eor, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0x5A */ {"NOP", nop, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x5B */ {"SRE", sre, 3, 7, 0, ADDR_MODE_ABSOLUTEY},
        /* 0x5C */ {"TOP", top, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x5D */ {"EOR", eor, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x5E */ {"LSR", lsr, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x5F */ {"SRE", sre, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x60 */ {"RTS", rts, 1, 6, 0, ADDR_MODE_IMPLICIT},
        /* 0x61 */ {"ADC", adc, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0x62 */ {"x62", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x63 */ {"RRA", rra, 2, 8, 0, ADDR_MODE_INDIRECTX},
        /* 0x64 */ {"DOP", dop, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x65 */ {"ADC", adc, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x66 */ {"ROR", ror, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x67 */ {"RRA", rra, 2, 5, 0, ADDR_MODE_ZP},
        /* 0x68 */ {"PLA", pla, 1, 4, 0, ADDR_MODE_IMPLICIT},
        /* 0x69 */ {"ADC", adc, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x6A */ {"ROR", rora, 1, 2, 0, ADDR_MODE_ACCUMULATOR},
        /* 0x6B */ {"ARR", arr, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x6C */ {"JMP", jmp, 3, 5, 0, ADDR_MODE_INDIRECT},
        /* 0x6D */ {"ADC", adc, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x6E */ {"ROR", ror, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x6F */ {"RRA", rra, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0x70 */ {"BVS", bvs, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0x71 */ {"ADC", adc, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0x72 */ {"x72", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x73 */ {"RRA", rra, 2, 8, 0, ADDR_MODE_INDIRECTY},
        /* 0x74 */ {"DOP", dop, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x75 */ {"ADC", adc, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x76 */ {"ROR", ror, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x77 */ {"RRA", rra, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0x78 */ {"SEI", sei, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x79 */ {"ADC", adc, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0x7A */ {"NOP", nop, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x7B */ {"RRA", rra, 3, 7, 0, ADDR_MODE_ABSOLUTEY},
        /* 0x7C */ {"TOP", top, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x7D */ {"ADC", adc, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0x7E */ {"ROR", ror, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x7F */ {"RRA", rra, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x80 */ {"DOP", dop, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x81 */ {"STA", sta, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0x82 */ {"DOP", dop, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x83 */ {"AAX", aax, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0x84 */ {"STY", sty, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x85 */ {"STA", sta, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x86 */ {"STX", stx, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x87 */ {"AAX", aax, 2, 3, 0, ADDR_MODE_ZP},
        /* 0x88 */ {"DEY", dey, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x89 */ {"DOP", dop, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0x8A */ {"TXA", txa, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x8B */ {"x8B", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x8C */ {"STY", sty, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x8D */ {"STA", sta, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x8E */ {"STX", stx, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x8F */ {"AAX", aax, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0x90 */ {"BCC", bcc, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0x91 */ {"STA", sta, 2, 6, 0, ADDR_MODE_INDIRECTY},
        /* 0x92 */ {"x92", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x93 */ {"x93", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x94 */ {"STY", sty, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x95 */ {"STA", sta, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0x96 */ {"STX", stx, 2, 4, 0, ADDR_MODE_ZPY},
        /* 0x97 */ {"AAX", aax, 2, 4, 0, ADDR_MODE_ZPY},
        /* 0x98 */ {"TYA", tya, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x99 */ {"STA", sta, 3, 5, 0, ADDR_MODE_ABSOLUTEY},
        /* 0x9A */ {"TXS", txs, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0x9B */ {"x9B", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x9C */ {"x9C", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x9D */ {"STA", sta, 3, 5, 0, ADDR_MODE_ABSOLUTEX},
        /* 0x9E */ {"x9E", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0x9F */ {"x9F", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0xA0 */ {"LDY", ldy, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xA1 */ {"LDA", lda, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0xA2 */ {"LDX", ldx, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xA3 */ {"LAX", lax, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0xA4 */ {"LDY", ldy, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xA5 */ {"LDA", lda, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xA6 */ {"LDX", ldx, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xA7 */ {"LAX", lax, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xA8 */ {"TAY", tay, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xA9 */ {"LDA", lda, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xAA */ {"TAX", tax, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xAB */ {"LXA", lxa, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xAC */ {"LDY", ldy, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xAD */ {"LDA", lda, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xAE */ {"LDX", ldx, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xAF */ {"LAX", lax, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xB0 */ {"BCS", bcs, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0xB1 */ {"LDA", lda, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0xB2 */ {"xB2", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0xB3 */ {"LAX", lax, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0xB4 */ {"LDY", ldy, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0xB5 */ {"LDA", lda, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0xB6 */ {"LDX", ldx, 2, 4, 0, ADDR_MODE_ZPY},
        /* 0xB7 */ {"LAX", lax, 2, 4, 0, ADDR_MODE_ZPY},
        /* 0xB8 */ {"CLV", clv, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xB9 */ {"LDA", lda, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0xBA */ {"TSX", tsx, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xBB */ {"xBB", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0xBC */ {"LDY", ldy, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0xBD */ {"LDA", lda, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0xBE */ {"LDX", ldx, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0xBF */ {"LAX", lax, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0xC0 */ {"CPY", cpy, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xC1 */ {"CMP", cmp, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0xC2 */ {"DOP", dop, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xC3 */ {"DCP", dcp, 2, 8, 0, ADDR_MODE_INDIRECTX},
        /* 0xC4 */ {"CPY", cpy, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xC5 */ {"CMP", cmp, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xC6 */ {"DEC", dec, 2, 5, 0, ADDR_MODE_ZP},
        /* 0xC7 */ {"DCP", dcp, 2, 5, 0, ADDR_MODE_ZP},
        /* 0xC8 */ {"INY", iny, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xC9 */ {"CMP", cmp, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xCA */ {"DEX", dex, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xCB */ {"SAX", sax, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xCC */ {"CPY", cpy, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xCD */ {"CMP", cmp, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xCE */ {"DEC", dec, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0xCF */ {"DCP", dcp, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0xD0 */ {"BNE", bne, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0xD1 */ {"CMP", cmp, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0xD2 */ {"xD2", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0xD3 */ {"DCP", dcp, 2, 8, 0, ADDR_MODE_INDIRECTY},
        /* 0xD4 */ {"DOP", dop, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0xD5 */ {"CMP", cmp, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0xD6 */ {"DEC", dec, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0xD7 */ {"DCP", dcp, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0xD8 */ {"CLD", cld, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xD9 */ {"CMP", cmp, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0xDA */ {"NOP", nop, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xDB */ {"DCP", dcp, 3, 7, 0, ADDR_MODE_ABSOLUTEY},
        /* 0xDC */ {"TOP", top, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0xDD */ {"CMP", cmp, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0xDE */ {"DEC", dec, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0xDF */ {"DCP", dcp, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0xE0 */ {"CPX", cpx, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xE1 */ {"SBC", sbc, 2, 6, 0, ADDR_MODE_INDIRECTX},
        /* 0xE2 */ {"DOP", dop, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xE3 */ {"ISC", isc, 2, 8, 0, ADDR_MODE_INDIRECTX},
        /* 0xE4 */ {"CPX", cpx, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xE5 */ {"SBC", sbc, 2, 3, 0, ADDR_MODE_ZP},
        /* 0xE6 */ {"INC", inc, 2, 5, 0, ADDR_MODE_ZP},
        /* 0xE7 */ {"ISC", isc, 2, 5, 0, ADDR_MODE_ZP},
        /* 0xE8 */ {"INX", inx, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xE9 */ {"SBC", sbc, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xEA */ {"NOP", nop, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xEB */ {"SBC", sbc, 2, 2, 0, ADDR_MODE_IMMEDIATE},
        /* 0xEC */ {"CPX", cpx, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xED */ {"SBC", sbc, 3, 4, 0, ADDR_MODE_ABSOLUTE},
        /* 0xEE */ {"INC", inc, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0xEF */ {"ISC", isc, 3, 6, 0, ADDR_MODE_ABSOLUTE},
        /* 0xF0 */ {"BEQ", beq, 2, 2, 0, ADDR_MODE_RELATIVE},
        /* 0xF1 */ {"SBC", sbc, 2, 5, 1, ADDR_MODE_INDIRECTY},
        /* 0xF2 */ {"xF2", nullptr, 0, 0, 0, INVALID_OPCODE},
        /* 0xF3 */ {"ISC", isc, 2, 8, 0, ADDR_MODE_INDIRECTY},
        /* 0xF4 */ {"DOP", dop, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0xF5 */ {"SBC", sbc, 2, 4, 0, ADDR_MODE_ZPX},
        /* 0xF6 */ {"INC", inc, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0xF7 */ {"ISC", isc, 2, 6, 0, ADDR_MODE_ZPX},
        /* 0xF8 */ {"SED", sed, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xF9 */ {"SBC", sbc, 3, 4, 1, ADDR_MODE_ABSOLUTEY},
        /* 0xFA */ {"NOP", nop, 1, 2, 0, ADDR_MODE_IMPLICIT},
        /* 0xFB */ {"ISC", isc, 3, 7, 0, ADDR_MODE_ABSOLUTEY},
        /* 0xFC */ {"TOP", top, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0xFD */ {"SBC", sbc, 3, 4, 1, ADDR_MODE_ABSOLUTEX},
        /* 0xFE */ {"INC", inc, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
        /* 0xFF */ {"ISC", isc, 3, 7, 0, ADDR_MODE_ABSOLUTEX},
    };
    uint16_t getAddrBasedOnMode(AddressingMode);
public:
    NesCpu(NesMemory *);
    ~NesCpu();
    void power_up();
    void step();
};



#endif //NESEMU_CPU_H
