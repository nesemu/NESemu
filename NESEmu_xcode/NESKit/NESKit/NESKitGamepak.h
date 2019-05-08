//
//  NESKitGamepak.h
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NESKitGamepak : NSObject

- (id) initWithFile:(NSString *)filename;

- (id) powerup;
- (void) writePRGAtAddress:(unsigned short)address withValue:(unsigned char)value;
- (unsigned char) readPRGAtAddress:(unsigned short)address;
- (void) writeCHRAtAddress:(unsigned short)address withValue:(unsigned char)value;
- (unsigned char) readCHRAtAddress:(unsigned short)address;
- (unsigned short) translateNametableAddress:(unsigned short)address;
@end

NS_ASSUME_NONNULL_END
