//
//  cpu.h
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <NESKit/NESKitMemory.h>

NS_ASSUME_NONNULL_BEGIN

@interface NESKitCPU : NSObject

- (id) initWithMemory:(NESKitMemory *)memory;
@property void *cppObject;

- (void) powerup;
- (unsigned long) step;
- (void) setPCtoAddress:(unsigned) address;
- (void) requestNMI;
@end

NS_ASSUME_NONNULL_END
