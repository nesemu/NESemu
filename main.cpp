#include <iostream>
#include <SDL.h>
#include "nes_lib/Gamepak.h"
#include "nes_lib/PPU.h"
#include "nes_lib/memory.h"
#include "nes_lib/cpu.h"
#include "nes_lib/InputDevice.h"

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "SDL_Init Error",
                SDL_GetError(),
                nullptr);
        return -1;
    }

    const char *error = nullptr;

    if (argc != 2) {
        SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Usage error",
                "Usage: NESEmu <rom_file_path>",
                nullptr);
        return -1;
    }

    SDL_Window * sdl_window;
    SDL_Renderer * sdl_renderer;
    SDL_CreateWindowAndRenderer(100, 100, SDL_WINDOW_SHOWN, &sdl_window, &sdl_renderer); //TODO: Sizes Need Changed to actual
    if (!sdl_window || !sdl_renderer) {
        SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "System Error",
                "Could not create window or renderer",
                nullptr);
        return -1;
    }

    SDL_SetWindowTitle(sdl_window, "Ethan And Kyle NESEmu v0.1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    SDL_RenderSetLogicalSize(sdl_renderer, 100, 100); //TODO: Sizes need to be changed to actual

    SDL_Texture * sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 100, 100); //TODO: Update Sizes

    InputDevice * joypad1 = nullptr;
    InputDevice * joypad2 = nullptr;

    int numJoysticks = SDL_NumJoysticks();

    if (numJoysticks == 0) {
        joypad1 = new InputDevice(-1);
        joypad2 = new InputDevice(-2);
    }
    else if (numJoysticks == 1) {
        joypad1 = new InputDevice(0);
        joypad2 = new InputDevice(-1);
    }
    else if (numJoysticks == 2) {
        joypad1 = new InputDevice(0);
        joypad2 = new InputDevice(1);
    }

    SDL_JoystickEventState(SDL_DISABLE);

    SDL_Delay(1000);

    //TODO: Implement the rest of the main including the game loop

    std::string rom_filename = argv[1];

    Gamepak rom_gamepak = Gamepak(rom_filename);
    if (rom_gamepak.initialize()) return EXIT_FAILURE;

    PPU ppu;

    NesCPUMemory memory(&ppu, &rom_gamepak, joypad1, joypad2);
    NesCpu cpu(&memory);

    cpu.power_up();

    cpu.setPC(0xC000);

    size_t counter = 0;
    SDL_Event e;
    bool quit = false;

    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

        }
        cpu.step();
        //memory.printTest();
        counter++;
    }

    delete joypad1;
    delete joypad2;
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyTexture(sdl_texture);
    SDL_DestroyWindow(sdl_window);

    SDL_Quit();

    return 0;
}