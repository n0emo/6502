#include "cpu.h"
#include "mem.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    Color colors[] = {
        GetColor(0x000000FF),
        GetColor(0xffffffFF),
        GetColor(0x880000FF),
        GetColor(0xaaffeeFF),
        GetColor(0xcc44ccFF),
        GetColor(0x00cc55FF),
        GetColor(0x0000aaFF),
        GetColor(0xeeee77FF),
        GetColor(0xdd8855FF),
        GetColor(0x664400FF),
        GetColor(0xff7777FF),
        GetColor(0x333333FF),
        GetColor(0x777777FF),
        GetColor(0xaaff66FF),
        GetColor(0x0088ffFF),
        GetColor(0xbbbbbbFF),
    };

    Cpu cpu = {0};
    Memory mem = {0};

    mem_init(&mem);
    cpu_init(&cpu, &mem);

    cpu_reset(&cpu);
    cpu.B = 1;

    Device main_dev = mem.devices[0];
    size_t width = 32;
    size_t height = 32;

    srand(time(NULL));

    InitWindow(640, 640, "6502");
    // SetTargetFPS(6000);

    RenderTexture screen = LoadRenderTexture(width, height);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        if (cpu.B)
        {
            mem_write(&mem, 0x00ff, GetKeyPressed() % 256);
            mem_write(&mem, 0x00fe, rand() % 256);
            cpu_execute(&cpu);
            for (size_t row = 0; row < height; row++)
            {
                for (size_t col = 0; col < width; col++)
                {
                    size_t index = height * (width - col - 1) + row;
                    uint8_t color_value = mem_read(&mem, 0x0200 + index) % 16;
                    Color color = colors[color_value];
                    BeginTextureMode(screen);
                    DrawPixel(row, col, color);
                    EndTextureMode();
                }
            }
        }
        else
        {
            printf("End\n");
        }
        ClearBackground(BLACK);
        DrawTextureEx(screen.texture, (Vector2){0, 0}, 0, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
