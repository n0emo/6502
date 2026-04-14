cmake_minimum_required(VERSION 3.1...3.31)

project(mos6502)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(_USE_MATH_DEFINES)

include(FetchContent)
set(FETCHCONTENT_QUIET NO)

set(RAYLIB_VERSION 5.5)

find_package(raylib ${RAYLIB_VERSION} QUIET)

if(NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )

    FetchContent_MakeAvailable(raylib)
endif()

add_executable(mos6502 ./src/main.c)

target_include_directories(mos6502
    PRIVATE ./src
)

target_link_libraries(mos6502
    PRIVATE raylib
)
