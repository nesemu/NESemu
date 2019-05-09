# NESemu
## NES Emulator
### Ethan Williams and Kyle Miller

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

NESEmu builds with XCode on macOS 10.14. Open NESemu.xcworkspace and build the `NESEmu` target

### Usage

Double-click on `NESEmu.app`, open a `.nes` rom file, and play! Below are the controls (currently only player one input supported):
- A Button -> K
- B Button -> J
- Start -> M
- Select -> N
- Up/Down/Left/Right -> WASD

### Future Plans

The next goals for NESEmu are adding APU emulation to output audio. In fact
a third party audio library has already been implemented in the APU branch but
we would very much like to write our own APU.
