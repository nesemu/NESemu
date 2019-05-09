//
// Created by Kyle Miller on 2/4/19.
//

#ifndef NESEMU_INPUTDEVICE_H
#define NESEMU_INPUTDEVICE_H

#include "/usr/local/Cellar/sdl2/2.0.9_1/include/SDL2/SDL.h"
#include <cstdint>

#define NES_A_BUTTON 1
#define NES_B_BUTTON 0
#define NES_START_BUTTON 9
#define NES_SELECT_BUTTON 8
#define JOYSTICK_DEAD_ZONE 8000

enum InputType {
    INPUT_TYPE_KEYBOARD_PLAYER_1,
    INPUT_TYPE_KEYBOARD_PLAYER_2,
    INPUT_TYPE_JOYPAD
};


class InputDevice {
public:
    InputDevice(int num);
    ~InputDevice();
    uint8_t readController();
    void writeController(uint8_t byte);
    bool externState[8];
private:
//    SDL_Joystick * joypad;
//    SDL_JoystickID id;
    bool virtualState[8];
    int buttonNum;
    bool strobe;

    const uint8_t * keyboardState;

    InputType inputType;

    void reset();
};


#endif //NESEMU_INPUTDEVICE_H
