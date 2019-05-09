//
//  NESKitInputDevice.m
//  NESKit
//
//  Created by Ethan Williams on 5/7/19.
//  Copyright © 2019 NESEmu. All rights reserved.
//

#import "NESKitInputDevice.h"
#import <NESKit/InputDevice.h>

@implementation NESKitInputDevice
InputDevice *joypad;

- (id) initWithNum:(int)num {
    self = [super init];
    joypad = new InputDevice(num);
    self.cppObject = joypad;
    return self;
}

- (unsigned char) readController {
    return joypad->readController();
}

- (void) writeControllerWithByte:(unsigned char)byte {
    joypad->writeController(byte);
}

- (void) updateExternalStateForButton:(int)num withState:(bool)state{
    if (num >= 0) {
        joypad->externState[num] = state;
    }
}

@end
