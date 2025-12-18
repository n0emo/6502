cmake_minimum_required(VERSION 3.1...3.31)

project(mos6502)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(_USE_MATH_DEFINES)

include(FetchContent)
set(FETCHCONTENT_QUIET NO)

find_package(raylib)

if(!raylib_FOUND)
    set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "Build raylib games" FORCE)
    set(SUPPORT_MODULE_RAUDIO OFF CACHE BOOL "Enable raudio module" FORCE)
    set(CUSTOMIZE_BUILD ON CACHE BOOL "Custom build options" FORCE)

    set(RAYLIB_VERSION 5.5)

    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )

    FetchContent_MakeAvailable(raylib)
endif()

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
