//
//  NESKitGamepak.m
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import "NESKitGamepak.h"
#import <NESKit/Gamepak.h>

@implementation NESKitGamepak
Gamepak *gamepak;

- (id) initWithFile:(NSString *)filename {
    self = [super init];
    gamepak = new Gamepak([filename cStringUsingEncoding:NSUTF8StringEncoding]);
    self.cppObject = gamepak;
    return self;
}

- (void) powerup {
    gamepak->initialize();
}

- (void) writePRGAtAddress:(unsigned short)address withValue:(unsigned char)value {
    gamepak->write_PRG(address, value);
}

- (unsigned char) readPRGAtAddress:(unsigned short)address {
    return gamepak->read_PRG(address);
}

- (void) writeCHRAtAddress:(unsigned short)address withValue:(unsigned char)value {
    gamepak->write_CHR(address, value);
}

- (unsigned char) readCHRAtAddress:(unsigned short)address {
    return gamepak->read_CHR(address);
}

- (unsigned short) translateNametableAddress:(unsigned short)address {
    return gamepak->translate_nametable_address(address);
}

@end
