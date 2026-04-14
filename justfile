samples := "samples"

[private]
@default:
    just --list

# Create CMake cache
configure:
    mkdir -p build
    cmake -S . -B build -G Ninja

# Build emulator
build:
    cmake --build build

# Run emulator with `args`
run *args: build
    ./build/mos6502 {{ args }}

# Build `samples/$name.rom` from assembly `samples/$name.s`
build-sample name:
    ca65 \
        --listing {{ samples / name + ".lst" }} \
        -o {{ samples / name + ".o" }} \
        {{ samples / name + ".s" }}
    ld65 \
        --config memory.cfg \
        -o {{ samples / name + ".rom" }} \
        {{ samples / name + ".o" }}

# Build and run sample `samples/$name.rom` using emulator
run-sample name: (build-sample name) (run "run " + (samples / name + ".rom"))
