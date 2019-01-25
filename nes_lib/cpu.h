//
// Created by Kyle Miller on 1/25/19.
//

#ifndef NESEMU_CPU_H
#define NESEMU_CPU_H

// Set Some Mask Definitons
#define CARRY_MASK 0x1
#define ZERO_MASK 0x2
#define INTERRUPT_DISABLE_MASK 0x4
#define DECIMAL_MASK 0x8

// The B Flag which doesn't actually exist in the P Register
// Instead these bits only exist when the flags are pushed to
// the stack, depending on how they were pushed determines which
// bit is 1 and 0

#define B_MASK 0x10
#define I_MASK 0x20
#define OVERFLOW_MASK 0x40
#define NEGATIVE_MASK 0x80

// Define addresses for the interrupt vectors

#define NMI_INTERRUPT_VECTOR 0xFFFA
#define RESET_INTERRUPT_VECTOR 0xFFFC
#define IRQ_INTERRUPT_VECTOR 0xFFFE



#endif //NESEMU_CPU_H
