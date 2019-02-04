cmake_minimum_required(VERSION 3.12)
project(NESemu)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(sdl2 REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

add_executable(NESemu main.cpp nes_lib/cpu.cpp nes_lib/memory.cpp nes_lib/Gamepak.cpp nes_lib/PPU.cpp nes_lib/PPUmemory.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})