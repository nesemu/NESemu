//
//  cpu.m
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import "NESKitCPU.h"

#import <NESKit/cpu.h>

@implementation NESKitCPU {
NesCpu *cpu;
}

- (id) initWithMemory:(NESKitMemory *)memory {
    self = [super init];
    cpu = new NesCpu((NesCPUMemory *)memory.cppObject);
    self.cppObject = (void *)cpu;
    return self;
}

- (void) powerup {
    cpu->power_up();
}

- (unsigned long) step {
    nes_cpu_clock_t clock = cpu->step();
    return clock.count();
}

- (void) setPCtoAddress:(unsigned int)address {
    cpu->setPC(address);
}

- (void) requestNMI {
    cpu->requestNMI();
}


@end
