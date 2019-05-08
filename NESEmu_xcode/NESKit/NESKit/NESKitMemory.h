//
//  NESKitMemory.h
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <NESKit/NESKitGamepak.h>
#import <NESKit/NESKitPPU.h>
#import <NESKit/NESKitInputDevice.h>


NS_ASSUME_NONNULL_BEGIN

@interface NESKitMemory : NSObject

- (id) initWithPPU:(NESKitPPU *)ppu Gamepak:(NESKitGamepak *)gamepak InputDevice1: (NESKitInputDevice *)input1 InputDevice2: (NESKitInputDevice *)input2;

- (unsigned int) readByteFromAddress:(unsigned short)address;
- (unsigned long) writeByteAtAddress:(unsigned short)address withValue:(unsigned char)value;
- (unsigned short) readWordFromAddress:(unsigned short)address;
- (unsigned short) readWordPageBugFromAddress:(unsigned short)address;
- (void) writeWordAtAddress:(unsigned short)address withValue:(unsigned short)value;
- (void) stackWriteWordAtAddress:(unsigned short) address withValue:(unsigned short)value;
- (void) mapMemoryAtAddress:(unsigned short)address withData:(unsigned char *)data ofSize:(unsigned long)size;

@end

NS_ASSUME_NONNULL_END
