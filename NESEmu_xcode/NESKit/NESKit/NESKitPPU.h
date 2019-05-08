//
//  NESKitPPU.h
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <NESKit/NESKitGamepak.h>

@class NESKitCPU;

NS_ASSUME_NONNULL_BEGIN

@interface NESKitPPU : NSObject

- (id) initWithGamepak:(NESKitGamepak *)gamepak;
@property void *cppObject;

- (void) powerup;
- (void) assign_cpu:(NESKitCPU *)cpu;
- (unsigned char) readRegisterAtAddress:(unsigned char)address;
- (void) writeRegisterAtAddress:(unsigned short)address withValue:(unsigned char)value;
- (void) OAM_DMA_withMemory:(unsigned char *)CPUmemory;
- (unsigned int *) getFramebuffer;

- (bool) step;

@end

NS_ASSUME_NONNULL_END
