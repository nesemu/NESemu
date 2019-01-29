#include <iostream>
#include <SDL.h>
#include "nes_lib/Gamepak.h"

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

    /*
    if (SDL_NumJoysticks() < 1) {
        SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Usage error",
                "No Joypad Connected",
                nullptr);
        return -1;
    }
    */

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

    SDL_Delay(1000);

    //TODO: Implement the rest of the main including the game loop

    std::string rom_filename = argv[1];

		NesMemory memory;

    Gamepak rom_gamepak = Gamepak(rom_filename);
    if (rom_gamepak.initialize()) return EXIT_FAILURE;

    rom_gamepak.loadPRG(&memory);
    return 0;
}