//
// Created by Kyle Miller on 2/4/19.
//

#include "../include/InputDevice.h"
#include <iostream>

InputDevice::InputDevice(int num) {
//    if (SDL_WasInit(0) == 0) {
//        SDL_Init(SDL_INIT_JOYSTICK);
//    }
    keyboardState = nullptr;
//    if (num == -1) {
//        inputType = INPUT_TYPE_KEYBOARD_PLAYER_1;
//    }
//    else if (num == -2) {
//        inputType = INPUT_TYPE_KEYBOARD_PLAYER_2;
//    }
//    else {
//        joypad = SDL_JoystickOpen(num);
//        id = SDL_JoystickInstanceID(joypad);
//        inputType = INPUT_TYPE_JOYPAD;
//    }
    strobe = false;
    buttonNum = 0;
}

InputDevice::~InputDevice() {
//    if (inputType == INPUT_TYPE_JOYPAD) {
//        SDL_JoystickClose(joypad);
//    }
}

uint8_t InputDevice::readController() {
    if (strobe) {
        reset();
    }
    
    bool pressed = true;
    if (buttonNum < 8) pressed = virtualState[buttonNum];

//    if (inputType == INPUT_TYPE_JOYPAD) {
//        switch (buttonNum) {
//            case 0:
//                pressed = (bool) SDL_JoystickGetButton(joypad, NES_A_BUTTON);
//                break;
//            case 1:
//                pressed = (bool) SDL_JoystickGetButton(joypad, NES_B_BUTTON);
//                break;
//            case 2:
//                pressed = (bool) SDL_JoystickGetButton(joypad, NES_SELECT_BUTTON);
//                break;
//            case 3:
//                pressed = (bool) SDL_JoystickGetButton(joypad, NES_START_BUTTON);
//                break;
//            case 4:
//                pressed = (SDL_JoystickGetAxis(joypad, 1) < -8000);
//                break;
//            case 5:
//                pressed = (SDL_JoystickGetAxis(joypad, 1) > 8000);
//                break;
//            case 6:
//                pressed = (SDL_JoystickGetAxis(joypad, 0) < -8000);
//                break;
//            case 7:
//                pressed = (SDL_JoystickGetAxis(joypad, 0) > 8000);
//                break;
//            default:
//                pressed = true;
//                break;
//        }
//    }
//    else if (inputType == INPUT_TYPE_KEYBOARD_PLAYER_1) {
//        switch (buttonNum) {
//            case 0:
//                pressed = (bool)keyboardState[SDL_SCANCODE_N];
//                break;
//            case 1:
//                pressed = (bool)keyboardState[SDL_SCANCODE_M];
//                break;
//            case 2:
//                pressed = (bool)keyboardState[SDL_SCANCODE_C];
//                break;
//            case 3:
//                pressed = (bool)keyboardState[SDL_SCANCODE_V];
//                break;
//            case 4:
//                pressed = (bool)keyboardState[SDL_SCANCODE_W];
//                break;
//            case 5:
//                pressed = (bool)keyboardState[SDL_SCANCODE_S];
//                break;
//            case 6:
//                pressed = (bool)keyboardState[SDL_SCANCODE_A];
//                break;
//            case 7:
//                pressed = (bool)keyboardState[SDL_SCANCODE_D];
//                break;
//            default:
//                pressed = true;
//                break;
//        }
//    }
//    else if (inputType == INPUT_TYPE_KEYBOARD_PLAYER_2) {
//        switch (buttonNum) {
//            case 0:
//                pressed = (bool)keyboardState[SDL_SCANCODE_N];
//                break;
//            case 1:
//                pressed = (bool)keyboardState[SDL_SCANCODE_M];
//                break;
//            case 2:
//                pressed = (bool)keyboardState[SDL_SCANCODE_SPACE];
//                break;
//            case 3:
//                pressed = (bool)keyboardState[SDL_SCANCODE_RETURN];
//                break;
//            case 4:
//                pressed = (bool)keyboardState[SDL_SCANCODE_I];
//                break;
//            case 5:
//                pressed = (bool)keyboardState[SDL_SCANCODE_K];
//                break;
//            case 6:
//                pressed = (bool)keyboardState[SDL_SCANCODE_J];
//                break;
//            case 7:
//                pressed = (bool)keyboardState[SDL_SCANCODE_L];
//                break;
//            default:
//                pressed = true;
//                break;
//        }
//    }
    buttonNum++;
    if (pressed) {
        return 0x1;
    }
    else {
        return 0x0;
    }
}

void InputDevice::writeController(uint8_t byte) {
    buttonNum = 0;
    strobe = ((byte&0x1) != 0);
    reset();
}

void InputDevice::reset() {
//    if (inputType == INPUT_TYPE_JOYPAD) {
//        SDL_JoystickUpdate();
//    }
//    else if (inputType == INPUT_TYPE_KEYBOARD_PLAYER_1 || inputType == INPUT_TYPE_KEYBOARD_PLAYER_2 ) {
//        SDL_PumpEvents();
//        keyboardState = SDL_GetKeyboardState(nullptr);
//    }
    memcpy(virtualState, externState, 8);
    buttonNum = 0;
}

