# NESemu ## NES Emulator ### Ethan Williams and Kyle Miller

NESEmu is an open-source emulator project for the Nintendo Entertainment System,
Nintendo's first home console. The NES was a revolutionary system that sparked
the home gaming industry in the United States.

The NES was innovative in using a dedicated PPU (picture processing unit) to
display graphics, allowing it to be one of the first consoles to offer
pixel-level background scrolling. In addition, an ingenious tile and palette
system for displaying the background and sprites allowed for remarkably
efficient use of the PPU's limited onboard memory. The memory could also be
expanded using mapper circuits onboard the Gamepak cartridges, which could
dynamically remap CPU and PPU memory to multiple ROM or RAM banks on the
cartridge during gameplay to effectively multiplex the address space.

The NES has also gained great popularity in the retro gaming and hacking
communities, with many emulators in active development and a wide library of
homebrew games written for the console.

### Build

NESEmu builds with CMake and requires SDL2 to run. It has been tested on macOS
10.14 with SDL2.0.9, but should be runnable on any Unix-like operating system.

### Usage

`./NESEmu romfile`

NESEmu supports any ROM utilizing iNES Mapper 000. In addition, due to an SDL
bug on macOS, the window does not accept mouse input during the game, so the
program must be closed externally.

### Future Plans

The next goals for NESEmu are adding APU emulation to output audio, as well as
adding support for mapper 001 to support a much wider library of games.
