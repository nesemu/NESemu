cmake_minimum_required(VERSION 3.12)
project(NESemu)

set(CMAKE_CXX_STANDARD 17)


FIND_PACKAGE(sdl2 REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} nes_lib/include nes_apu/include)

add_library(NESapu SHARED nes_apu/src/apu_snapshot.cpp nes_apu/src/Blip_Buffer.cpp nes_apu/src/Multi_Buffer.cpp
        nes_apu/src/Nes_Apu.cpp nes_apu/src/Nes_Namco.cpp nes_apu/src/Nes_Oscs.cpp nes_apu/src/Nes_Vrc6.cpp nes_apu/src/Nonlinear_Buffer.cpp
        nes_apu/src/Sound_Queue.cpp)

TARGET_LINK_LIBRARIES(NESapu ${SDL2_LIBRARIES})

add_library(NESlib SHARED nes_lib/src/cpu.cpp nes_lib/src/memory.cpp nes_lib/src/Gamepak.cpp nes_lib/src/PPU.cpp
        nes_lib/src/PPUmemory.cpp nes_lib/src/InputDevice.cpp nes_lib/src/APU.cpp)

TARGET_LINK_LIBRARIES(NESapu ${SDL2_LIBRARIES})
TARGET_LINK_LIBRARIES(NESlib ${SDL2_LIBRARIES} NESapu)

add_executable(${PROJECT_NAME} main.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} NESapu NESlib)