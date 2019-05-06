cmake_minimum_required(VERSION 3.12)
project(NESemu)

set(CMAKE_CXX_STANDARD 17)


FIND_PACKAGE(sdl2 REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} nes_lib/include)

add_library(NESlib SHARED nes_lib/src/cpu.cpp nes_lib/src/memory.cpp nes_lib/src/Gamepak.cpp nes_lib/src/PPU.cpp nes_lib/src/PPUmemory.cpp nes_lib/src/InputDevice.cpp)

TARGET_LINK_LIBRARIES(NESlib ${SDL2_LIBRARIES})

add_executable(${PROJECT_NAME} main.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} NESlib)