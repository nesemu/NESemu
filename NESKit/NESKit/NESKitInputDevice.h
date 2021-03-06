//
//  NESKitInputDevice.h
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NESKitInputDevice : NSObject
- (id) initWithNum:(int)num;
@property void *cppObject;

- (unsigned char) readController;
- (void) writeControllerWithByte:(unsigned char)byte;

- (void) updateExternalStateForButton:(int)num withState:(bool)state;
@end

NS_ASSUME_NONNULL_END
