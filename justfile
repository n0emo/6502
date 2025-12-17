samples := "samples"

configure:
    mkdir -p build
    cmake -S . -B build -G Ninja

build:
    cmake --build build

run *args: build
    ./build/mos6502 {{args}}

build-sample name:
    ca65 \
        --listing {{ samples / name + ".lst" }} \
        -o {{ samples / name + ".o" }} \
        {{ samples / name + ".s" }}
    ld65 \
        --config memory.cfg \
        -o {{ samples / name + ".rom" }} \
        {{ samples / name + ".o" }}

run-sample name: \
    (build-sample name) \
    (run "run " + (samples / name + ".rom"))
