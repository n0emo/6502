#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <raylib.h>

#include "cpu.h"
#include "instructions.h"
#include "mem.h"

#define WITH_DEBUG_OVERLAY

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

    Font font = LoadFontEx("./assets/Courier Prime Code.ttf", 20, 0, 0);
    // SetTargetFPS(6000);

    RenderTexture screen = LoadRenderTexture(width, height);

#ifdef WITH_DEBUG_OVERLAY
    bool debug_overlay = true;
    bool debug_continue = false;
#endif

    while (!WindowShouldClose())
    {
        bool do_execute = true;
#ifdef WITH_DEBUG_OVERLAY
        if (IsKeyPressed(KEY_F5))
        {
            debug_overlay = !debug_overlay;
        }

        if (IsKeyPressed(KEY_F6))
        {
            debug_continue = !debug_continue;
        }

        do_execute = IsKeyPressed(KEY_F7) || debug_continue;
#endif

        BeginDrawing();
        if (cpu.B && do_execute)
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

        ClearBackground(BLACK);
        DrawTextureEx(screen.texture, (Vector2){0, 0}, 0, 20, WHITE);

#ifdef WITH_DEBUG_OVERLAY
        if (debug_overlay)
        {
            const int font_size = 20;
            Color overlay_color = GetColor(0xFFFFFFAA);

            DrawTextEx(font, "Debug overlay", (Vector2) { 10, 10 }, font_size, 0, overlay_color);

            const int cpu_width = 280;
            const int cpu_height = 130;
            Rectangle cpu_bounds = {
                .x = GetScreenWidth() - cpu_width - 20,
                .y = GetScreenHeight() - cpu_height - 20,
                .width = cpu_width,
                .height = cpu_height,
            };

            DrawRectangleRec(cpu_bounds, GetColor(0xFFFFFF22));
            DrawRectangleLinesEx(cpu_bounds, 1.0f, overlay_color);

            Instruction inst = get_instruction_by_opcode(mem_read(cpu.mem, cpu.PC));
            char buf[1024];
            snprintf(
                buf,
                sizeof(buf),
                "Instruction: %s\n"
                "PC=0x%.4x, SP=0x%.2x\n"
                "AC=0x%.2x, X=0x%.2x, Y=0x%.2x\n"
                "NV-BDIZC\n"
                "%d%d%d%d%d%d%d%d\n\n",
                cpu_inst_name(inst.type),
                cpu.PC, cpu.SP,
                cpu.A, cpu.X, cpu.Y,
                cpu.N, cpu.V, cpu.U, cpu.B, cpu.D, cpu.I, cpu.Z, cpu.C);

            Vector2 pos = { cpu_bounds.x + 10, cpu_bounds.y + 10 };
            DrawTextEx(font, buf, pos, font_size, 0, overlay_color);

            DrawTextEx(
                font,
                "F5 - Overlay\n"
                "F6 - Continue\n"
                "F7 - Step\n",
                (Vector2) { GetScreenWidth() - 160, 10 },
                font_size,
                0,
                overlay_color
            );
        }
#endif

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
