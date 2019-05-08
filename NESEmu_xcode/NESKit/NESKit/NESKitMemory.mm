//
//  NESKitMemory.m
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import "NESKitMemory.h"
#import <NESKit/memory.h>

@implementation NESKitMemory
NesCPUMemory *memory;

- (id) initWithPPU:(NESKitPPU *)ppu Gamepak:(NESKitGamepak *)gamepak InputDevice1:(NESKitInputDevice *)input1 InputDevice2:(NESKitInputDevice *)input2 {
    self = [super init];
    memory = new NesCPUMemory((PPU *)ppu.cppObject, (Gamepak *)gamepak.cppObject, (InputDevice *)input1.cppObject, (InputDevice *)input2.cppObject);
    self.cppObject = (void *)memory;
    return self;
}

- (unsigned int) readByteFromAddress:(unsigned short)address {
    return memory->read_byte(address);
}

- (unsigned long) writeByteAtAddress:(unsigned short)address withValue:(unsigned char)value {
    nes_cpu_clock_t clock = memory->write_byte(address, value);
    return clock.count();
}

- (unsigned short) readWordFromAddress:(unsigned short)address {
    return memory->read_word(address);
}

- (unsigned short) readWordPageBugFromAddress:(unsigned short)address {
    return memory->read_word_page_bug(address);
}

- (void) writeWordAtAddress:(unsigned short)address withValue:(unsigned short)value {
    memory->write_word(address, value);
}

- (void) stackWriteWordAtAddress:(unsigned short) address withValue:(unsigned short)value {
    memory->stack_write_word(address, value);
}

- (void) mapMemoryAtAddress:(unsigned short)address withData:(unsigned char *)data ofSize:(unsigned long)size {
    memory->map_memory(address, data, size);
}

@end
