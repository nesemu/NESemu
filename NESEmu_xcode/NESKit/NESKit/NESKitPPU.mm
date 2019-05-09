//
//  NESKitPPU.m
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import "NESKitPPU.h"
#import <NESKit/ppu.h>

#import "NESKitCPU.h"

@implementation NESKitPPU {
PPU *ppu;
}

- (id) initWithGamepak:(NESKitGamepak *)gamepak {
    self = [super init];
    ppu = new PPU((Gamepak *)gamepak.cppObject);
    self.cppObject = ppu;
    return self;
}

- (void) powerup {
    ppu->power_up();
}

- (void) assign_cpu:(NESKitCPU *)cpu {
    ppu->assign_cpu((NesCpu *)cpu.cppObject);
}

- (unsigned char) readRegisterAtAddress:(unsigned char)address {
    return ppu->read_register(address);
}

- (void) writeRegisterAtAddress:(unsigned short)address withValue:(unsigned char)value {
    ppu->write_register(address, value);
}

- (void) OAM_DMA_withMemory:(unsigned char *)CPUmemory {
    ppu->OAM_DMA(CPUmemory);
}

- (unsigned int *) getFramebuffer {
    return ppu->get_framebuffer();
}

- (bool) step {
    return ppu->step();
}

@end
