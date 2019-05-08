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

- (id) initwithMemory:(NESKitMemory *)memory;

- (void) powerup;
- (void) reset;
- (unsigned long) step;
- (void) setPCtoaddress:(unsigned) address;
- (void) requestNMI;
@end

NS_ASSUME_NONNULL_END
