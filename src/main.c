#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <raylib.h>

#include "cpu.h"
#include "instructions.h"
#include "mem.h"

#define SHIFT(xs, xs_sz) (assert((xs_sz) > 0), (xs_sz)--, *(xs)++)

typedef enum Subcommand
{
    SUBCOMMAND_HELP,
    SUBCOMMAND_RUN,
} Subcommand;

typedef struct Args
{
    const char *executable;
    Subcommand subcommand;
    const char *run_program;
} Args;

const float clock_frequency = 1.5e4f;

const char *usage =
    "Usage: %s <subcommand> [options]\n"
    "Subcommands:\n"
    "   help:        print this message\n"
    "   run <path>:  run binary program\n";

bool parse_args(int argc, const char **argv, Args *args)
{
    *args = (Args){0};
    args->executable = SHIFT(argv, argc);
    if (argc == 0)
        return false;

    const char *subcommand = SHIFT(argv, argc);

    if (strcmp(subcommand, "help") == 0)
    {
        args->subcommand = SUBCOMMAND_HELP;
        return true;
    }

    if (strcmp(subcommand, "run") == 0)
    {
        if (argc == 0)
            return false;

        args->subcommand = SUBCOMMAND_RUN;
        args->run_program = SHIFT(argv, argc);
        return true;
    }

    return false;
}

unsigned char *read_entire_file_binary(const char *path, uintptr_t *len)
{
    unsigned char *buf = NULL;

    FILE *f = fopen(path, "rb");
    if (f == NULL)
    {
        goto cleanup;
    }

    if (fseek(f, 0, SEEK_END) < 0)
    {
        goto cleanup;
    }

#ifndef _WIN32
    long m = ftell(f);
#else
    long long m = _ftelli64(f);
#endif

    if (m < 0)
    {
        goto cleanup;
    }

    if (fseek(f, 0, SEEK_SET) < 0)
    {
        goto cleanup;
    }

    buf = malloc(m);
    fread(buf, m, 1, f);
    if (ferror(f))
    {
        free(buf);
        buf = NULL;
        goto cleanup;
    }

cleanup:
    if (f)
        fclose(f);

    if (buf && len)
    {
        *len = m;
    }

    return buf;
}

void run_program(const char *path)
{
    uintptr_t len;
    const unsigned char *program = read_entire_file_binary(path, &len);
    if (program == NULL)
    {
        fprintf(stderr, "Error reading file: %s", strerror(errno));
        exit(1);
    }

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
    for (size_t i = 0; i < len; i++)
    {
        mem_write_force(&mem, 0x0600 + i, program[i]);
    }

    cpu_init(&cpu, &mem);

    cpu_reset(&cpu);
    cpu.B = 1;

    Device main_dev = mem.devices[0];
    size_t width = 32;
    size_t height = 32;

    srand(time(NULL));

    InitWindow(640, 640, "6502");

    Font font = LoadFontEx("./assets/Courier Prime Code.ttf", 20, 0, 0);
    SetTargetFPS(60);

    RenderTexture screen = LoadRenderTexture(width, height);

    bool debug_overlay = true;
    bool debug_continue = false;

    while (!WindowShouldClose())
    {
        bool do_execute = true;
        if (IsKeyPressed(KEY_F5))
        {
            debug_overlay = !debug_overlay;
        }

        if (IsKeyPressed(KEY_F6))
        {
            debug_continue = !debug_continue;
        }

        do_execute = IsKeyPressed(KEY_F7) || IsKeyPressedRepeat(KEY_F7) || debug_continue;

        if (IsKeyPressed(KEY_F10))
        {
            for (uintptr_t i = 0; i <= 0x00ff; i += 16)
            {
                printf("0x%04zx:", i);
                for (uintptr_t j = 0; j < 16; j++)
                {
                    printf(" %02hhx", mem_read(&mem, i + j));
                }
                printf("\n");
            }
            printf("\n");
        }

        mem_write(&mem, 0x00fe, rand() % 256);

        if (IsKeyPressed(KEY_W))
        {
            mem_write(&mem, 0x00ff, 'w');
        }
        if (IsKeyPressed(KEY_S))
        {
            mem_write(&mem, 0x00ff, 's');
        }
        if (IsKeyPressed(KEY_A))
        {
            mem_write(&mem, 0x00ff, 'a');
        }
        if (IsKeyPressed(KEY_D))
        {
            mem_write(&mem, 0x00ff, 'd');
        }

        uintptr_t clocks = 0;
        if (cpu.B && do_execute)
        {
            if (!debug_continue)
            {
                clocks = 1;
            }
            else
            {
                float dt = GetFrameTime();
                clocks = dt * clock_frequency;
            }
        }

        uintptr_t current_clocks = 0;
        while (current_clocks < clocks)
        {
            cpu_execute(&cpu);
            Instruction inst = get_instruction_by_opcode(mem_read(cpu.mem, cpu.PC));
            current_clocks += inst.cycles;
        }

#if 0
        printf("0x%02x 0x%02x 0x%02x 0x%02x\n",
               mem_read(cpu.mem, 0x0000),
               mem_read(cpu.mem, 0x0001),
               mem_read(cpu.mem, 0x00fe),
               mem_read(cpu.mem, 0x00ff));
#endif

        BeginDrawing();
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

        ClearBackground(BLACK);
        DrawTextureEx(screen.texture, (Vector2){0, 0}, 0, 20, WHITE);

        if (debug_overlay)
        {
            const int font_size = 20;
            Color overlay_color = GetColor(0xFFFFFFAA);

            DrawTextEx(font, "Debug overlay", (Vector2){10, 10}, font_size, 0, overlay_color);

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

            Vector2 pos = {cpu_bounds.x + 10, cpu_bounds.y + 10};
            DrawTextEx(font, buf, pos, font_size, 0, overlay_color);

            DrawTextEx(
                font,
                "F5 - Overlay\n"
                "F6 - Continue\n"
                "F7 - Step\n",
                (Vector2){GetScreenWidth() - 160, 10},
                font_size,
                0,
                overlay_color);
        }

        EndDrawing();
    }

    CloseWindow();
}

int main(int argc, const char **argv)
{
    Args args;
    if (!parse_args(argc, argv, &args))
    {
        fprintf(stderr, usage, argv[0]);
        return 1;
    }

    switch (args.subcommand)
    {
    case SUBCOMMAND_HELP:
        printf(usage, args.executable);
        break;
    case SUBCOMMAND_RUN:
        run_program(args.run_program);
        break;
    }

    return 0;
}
