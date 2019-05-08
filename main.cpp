#include <iostream>
#include <unistd.h>
#include <SDL.h>
#include "nes_lib/include/Gamepak.h"
#include "nes_lib/include/PPU.h"
#include "nes_lib/include/memory.h"
#include "nes_lib/include/cpu.h"
#include "nes_lib/include/InputDevice.h"
#include "nes_apu/include/Sound_Queue.h"

class LTimer
{
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

int main(int argc, char *argv[]) {

    const int SCREEN_FPS = 60;
    const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
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
    Sound_Queue* sound_queue;
    sound_queue = new Sound_Queue;
    sound_queue->init(96000);
    SDL_Window * sdl_window;
    SDL_Renderer * sdl_renderer;
    SDL_CreateWindowAndRenderer(SCREEN_X, SCREEN_Y, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE, &sdl_window, &sdl_renderer); //TODO: Sizes Need Changed to actual
    if (!sdl_window || !sdl_renderer) {
        SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "System Error",
                "Could not create window or renderer",
                nullptr);
        return -1;
    }

    SDL_SetWindowTitle(sdl_window, "Ethan And Kyle NESEmu v0.1");

    SDL_RenderSetLogicalSize(sdl_renderer, SCREEN_X, SCREEN_Y); //TODO: Sizes need to be changed to actual

    SDL_Texture * sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_X, SCREEN_Y); //TODO: Update Sizes

    InputDevice * joypad1 = nullptr;
    InputDevice * joypad2 = nullptr;

    int numJoysticks = SDL_NumJoysticks();
    std::cout << "Num Joysticks: " << numJoysticks << std::endl;

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
    APU apu = APU();

    NesCPUMemory memory(&ppu, &apu, &rom_gamepak, joypad1, joypad2);
    NesCpu cpu(&memory);

    ppu.assign_cpu(&cpu);
    apu.assign_memory(&memory);

    cpu.power_up();
    ppu.power_up();
    apu.power_up();


    SDL_Event e;
    bool quit = false;
    LTimer capTimer;



    nes_cpu_clock_t cpuclock = nes_cpu_clock_t(7);
    nes_ppu_clock_t ppuclock = nes_ppu_clock_t(0);
    nes_cpu_clock_t end_frame = nes_cpu_clock_t(0);
    bool imageReady;

    while (SDL_PollEvent(&e) != 0) {
    }


    capTimer.start();
    while(true) {
        cpuclock += cpu.step();
        apu.setTime(cpuclock-end_frame);
        while (ppuclock.count() < cpuclock.count()*3) {
            imageReady = ppu.step();
            if (imageReady) {
                size_t count = apu.step((cpuclock-end_frame).count());
                if (count > 0) {
                    sound_queue->write(apu.getSamples(), count);
                }
                SDL_UpdateTexture(sdl_texture, nullptr, ppu.get_framebuffer(), SCREEN_X * sizeof(uint32_t));
                SDL_RenderClear(sdl_renderer);
                SDL_RenderCopy(sdl_renderer, sdl_texture, nullptr, nullptr);
                SDL_RenderPresent(sdl_renderer);
                int frameticks = capTimer.getTicks();
                if (frameticks < SCREEN_TICK_PER_FRAME) {
                    SDL_Delay(SCREEN_TICK_PER_FRAME - frameticks);
                }
                capTimer.start();
                end_frame = cpuclock;
            }
            ppuclock++;
        }
    }

    delete joypad1;
    delete joypad2;
    delete sound_queue;
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyTexture(sdl_texture);
    SDL_DestroyWindow(sdl_window);

    SDL_Quit();

    return 0;
}