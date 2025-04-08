cmake_minimum_required(VERSION 3.1)

project(mos6502)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(./lib/raylib)

add_executable(mos6502
    ./src/addressing.c
    ./src/cpu.c
    ./src/instructions.c
    ./src/int.c
    ./src/main.c
    ./src/mem.c
)

target_include_directories(mos6502
    PRIVATE ./src
)

target_link_libraries(mos6502
    PRIVATE raylib
)
