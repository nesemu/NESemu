cmake_minimum_required(VERSION 3.12)
project(NESemu)

set(CMAKE_CXX_STANDARD 17)


FIND_PACKAGE(sdl2 REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

add_executable(NESemu main.cpp nes_lib/cpu.cpp nes_lib/memory.cpp nes_lib/Gamepak.cpp nes_lib/PPU.cpp nes_lib/PPUmemory.cpp nes_lib/InputDevice.cpp nes_lib/APU.cpp
        nes_apu/apu_snapshot.cpp nes_apu/Blip_Buffer.cpp nes_apu/Multi_Buffer.cpp nes_apu/Nes_Apu.cpp nes_apu/Nes_Namco.cpp nes_apu/Nes_Oscs.cpp
        nes_apu/Nes_Vrc6.cpp nes_apu/Nonlinear_Buffer.cpp nes_apu/Sound_Queue.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})