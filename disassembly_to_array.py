#!/usr/bin/env python3

import sys

def convert_line(line):
    line = line.strip()
    if not line:
        return ""

    parts = line.split()

    address = parts[0];

    # Find where the instruction starts (first non-hex part after address)
    instruction_start = 1
    while instruction_start < len(parts) and len(parts[instruction_start]) not in range(3, 5):
        instruction_start += 1

    bytes_parts = parts[1:instruction_start]
    instruction = ' '.join(parts[instruction_start:])

    formatted_bytes = []
    for byte in bytes_parts:
        byte = byte.lstrip('$')
        formatted_bytes.append(f"0x{byte.lower()}")

    if not formatted_bytes:
        return f"    // {instruction}"

    return f"    /* {address} */ {', '.join(formatted_bytes)},  // {instruction}"

def main():
    converted = []
    for line in sys.stdin:
        converted.append(convert_line(line))

    for line in converted:
        print(line)

if __name__ == "__main__":
    main()
