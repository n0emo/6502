configure:
    mkdir -p build
    cd build
    cmake -S . -B build -G Ninja

build:
    cmake --build build

run *args: build
    ./build/mos6502 {{args}}
