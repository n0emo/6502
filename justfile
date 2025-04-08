configure:
    mkdir -p build
    cd build
    cmake -S . -B build -G Ninja

build:
    cmake --build build

run: build
    ./build/mos6502
