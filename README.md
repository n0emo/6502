# MOS 6502 Emulator

A MOS 6502 CPU emulator with a graphical debugger and display, built with Raylib.

## Features

- Complete MOS 6502 instruction set (including illegal opcodes)
- Real-time graphical display (32x32 color cells)
- Interactive debugger with keyboard controls
- Memory-mapped I/O (keyboard input, random number generator)

## Controls (during execution)

- `F6` - Continue/run
- `F7` - Step one instruction
- `F9` - Log zero-page memory
- `F10` - Log CPU state
- `ESC` - Exit

## Sample Programs

Sample assembly programs are included in the `samples/` directory.

## Quick Start

### Requirements

- CMake 3.1+
- [Raylib](https://www.raylib.com/) (auto-downloaded if not found)
- [Just](https://just.systems)
- [cc65](https://cc65.github.io) (for building samples)

### Building and running

```bash
# Configure CMake
$ just configure

# Build emulator
$ just build

# Other tasks
$ just
Available recipes:
    build             # Build emulator
    build-sample name # Build `samples/$name.rom` from assembly `samples/$name.s`
    configure         # Create CMake cache
    run *args         # Run emulator with `args`
    run-sample name   # Build and run sample `samples/$name.rom` using emulator
```
