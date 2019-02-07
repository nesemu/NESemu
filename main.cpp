#include <iostream>
#include <unistd.h>
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
    SDL_CreateWindowAndRenderer(SCREEN_X, SCREEN_Y, SDL_WINDOW_SHOWN, &sdl_window, &sdl_renderer); //TODO: Sizes Need Changed to actual
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

    SDL_RenderSetLogicalSize(sdl_renderer, SCREEN_X, SCREEN_Y); //TODO: Sizes need to be changed to actual

    SDL_Texture * sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_X, SCREEN_Y); //TODO: Update Sizes

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
    

    //TODO: Implement the rest of the main including the game loop

    std::string rom_filename = argv[1];

    Gamepak rom_gamepak = Gamepak(rom_filename);
    if (rom_gamepak.initialize()) return EXIT_FAILURE;

    double expectedtime = (double)1/60 * 1000;

    PPU ppu = PPU(&rom_gamepak);

    NesCPUMemory memory(&ppu, &rom_gamepak, joypad1, joypad2);
    NesCpu cpu(&memory);

    ppu.assign_cpu(&cpu);

    cpu.power_up();
    ppu.power_up();


    SDL_Event e;
    bool quit = false;
    Uint64 prev_counter = SDL_GetPerformanceCounter();
    Uint64 count_per_second = SDL_GetPerformanceFrequency();

    nes_cpu_clock_t cpuclock = nes_cpu_clock_t(7);
    nes_ppu_clock_t ppuclock = nes_ppu_clock_t(0);
    bool imageReady = false;

    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        cpuclock += cpu.step();

        while (ppuclock.count() < cpuclock.count()*3) {
            imageReady = ppu.step();

            if (imageReady) {
                SDL_UpdateTexture(sdl_texture, nullptr, ppu.get_framebuffer(), SCREEN_X * sizeof(uint32_t));
                SDL_RenderClear(sdl_renderer);
                SDL_RenderCopy(sdl_renderer, sdl_texture, nullptr, nullptr);
                SDL_RenderPresent(sdl_renderer);
                Uint64 cur_counter = SDL_GetPerformanceCounter();
                Uint64 delta_ticks = cur_counter - prev_counter;
                auto time = (double)delta_ticks * 1000 / count_per_second;
                auto sleeptime = expectedtime - time;
                //SDL_Delay(sleeptime);
            }
            ppuclock++;
        }
    }

    delete joypad1;
    delete joypad2;
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyTexture(sdl_texture);
    SDL_DestroyWindow(sdl_window);

    SDL_Quit();

    return 0;
}