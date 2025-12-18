#include "gui.h"
#include "instructions.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CLOCK_FREQUENCY 1.5e4f
#define CELLS 32
#define KEY_ADDR 0x00ff
#define RAND_ADDR 0x00fe

typedef enum Theme
{
    THEME_DARK,
    THEME_LIGHT,
} Theme;

typedef struct Colors
{
    Color palette[16];
    Color background;
    Color foreground;
} Colors;

typedef struct App
{
    RenderTexture target;
    Colors colors;
    Font font;
    Cpu *cpu;
    Memory *mem;
    bool close;
    bool debug_continue;
    bool do_execute;
} App;

static App app_create(Cpu *cpu, Memory *mem);
static void app_frame(App *app);
static void app_handle_input(App *app);
static void app_execute_cpu(App *app);
static void app_draw(App *app);
static void app_draw_screen(App *app, Rectangle bounds);
static void app_draw_debug_info(App *app, Rectangle bounds);
static Colors load_colors_for_theme(Theme theme);

void run_gui(Cpu *cpu, Memory *mem)
{
    srand(time(NULL));
    InitWindow(920, 640, "6502");
    SetTargetFPS(60);

    App app = app_create(cpu, mem);

    while (!app.close)
    {
        app_frame(&app);
    }

    CloseWindow();
}

static App app_create(Cpu *cpu, Memory *mem)
{
    App app;
    memset(&app, 0, sizeof(app));

    app.target = LoadRenderTexture(CELLS, CELLS);
    app.colors = load_colors_for_theme(THEME_LIGHT);
    app.cpu = cpu;
    app.mem = mem;
    app.font = LoadFontEx("./assets/Courier Prime Code.ttf", 20, 0, 0);

    return app;
}

static void app_frame(App *app)
{
    app->close = WindowShouldClose();
    if (app->close)
    {
        return;
    }

    app_handle_input(app);
    app_execute_cpu(app);
    app_draw(app);
}

static void app_handle_input(App *app)
{
    if (IsKeyPressed(KEY_F6))
    {
        app->debug_continue = !app->debug_continue;
    }

    app->do_execute = (IsKeyPressed(KEY_F7) || IsKeyPressedRepeat(KEY_F7) || app->debug_continue) && app->cpu->B;

    if (IsKeyPressed(KEY_F10))
    {
        cpu_print(app->cpu);
    }

    if (IsKeyPressed(KEY_F11))
    {
        for (uintptr_t i = 0; i <= 0x00ff; i += 16)
        {
            printf("0x%04zx:", i);
            for (uintptr_t j = 0; j < 16; j++)
            {
                printf(" %02hhx", mem_read(app->mem, i + j));
            }
            printf("\n");
        }
        printf("\n");
    }

    int c = GetCharPressed();
    if (c != 0)
    {
        mem_write(app->mem, KEY_ADDR, (uint8_t)c);
    }
}

static void app_execute_cpu(App *app)
{
    uintptr_t clocks = 0;
    if (app->do_execute)
    {
        if (!app->debug_continue)
        {
            clocks = 1;
        }
        else
        {
            float dt = GetFrameTime();
            clocks = dt * CLOCK_FREQUENCY;
        }
    }

    uintptr_t current_clocks = 0;
    while (current_clocks < clocks && app->cpu->B)
    {
        mem_write(app->mem, RAND_ADDR, rand() % 256);
        cpu_execute(app->cpu);
        Instruction inst = get_instruction_by_opcode(mem_read(app->mem, app->cpu->PC));
        current_clocks += inst.cycles;
    }
}

static void app_draw(App *app)
{
    BeginDrawing();
    ClearBackground(app->colors.background);

    const float padding = 5.0f;

    const float screen_size = GetScreenHeight() - padding * 2;
    const Rectangle screen_bounds = {padding, padding, screen_size, screen_size};
    app_draw_screen(app, screen_bounds);

    const float debug_width = GetScreenWidth() - screen_size - padding * 3;
    const float debug_height = GetScreenHeight() - padding * 2;
    const float debug_x = GetScreenWidth() - debug_width - padding;
    const float debug_y = padding;
    const Rectangle debug_bounds = {debug_x, debug_y, debug_width, debug_height};
    app_draw_debug_info(app, debug_bounds);

    EndDrawing();
}

static void app_draw_screen(App *app, Rectangle bounds)
{
    BeginTextureMode(app->target);
    for (size_t row = 0; row < CELLS; row++)
    {
        for (size_t col = 0; col < CELLS; col++)
        {
            size_t index = CELLS * (32 - col - 1) + row;
            uint8_t color_value = mem_read(app->mem, 0x0200 + index) % 16;
            Color color = app->colors.palette[color_value];
            DrawPixel(row, col, color);
        }
    }
    EndTextureMode();

    DrawTexturePro(
        app->target.texture,
        (Rectangle){0, 0, app->target.texture.width, app->target.texture.height},
        bounds,
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
}

static void app_draw_debug_info(App *app, Rectangle bounds)
{
    const int font_size = 20;

    Instruction inst = get_instruction_by_opcode(mem_read(app->cpu->mem, app->cpu->PC));
    char buf[1024];
    snprintf(
        buf,
        sizeof(buf),
        "CPU state\n\n"
        "Instruction: %s\n"
        "PC=0x%.4x, SP=0x%.2x\n"
        "AC=0x%.2x, X=0x%.2x, Y=0x%.2x\n"
        "NV-BDIZC\n"
        "%d%d%d%d%d%d%d%d\n"
        "\n\n"
        "Device state\n\n"
        "Key: '0x%.2x', rand: 0x%.2x\n"
        "\n\n"
        "Shortcuts\n\n"
        "ESC - close\n"
        "F6  - continue\n"
        "F7  - step\n"
        "F10 - log CPU\n"
        "F11 - log zero-page\n",
        cpu_inst_name(inst.type),
        app->cpu->PC,
        app->cpu->SP,
        app->cpu->A,
        app->cpu->X,
        app->cpu->Y,
        app->cpu->N,
        app->cpu->V,
        app->cpu->U,
        app->cpu->B,
        app->cpu->D,
        app->cpu->I,
        app->cpu->Z,
        app->cpu->C,
        mem_read(app->mem, KEY_ADDR),
        mem_read(app->mem, RAND_ADDR)
    );

    Vector2 pos = {bounds.x + 10, bounds.y + 10};
    DrawTextEx(app->font, buf, pos, font_size, 0, app->colors.foreground);
}

static Colors load_colors_for_theme(Theme theme)
{
    switch (theme)
    {
    case THEME_DARK:
        return (Colors){
            .palette =
                {
                    GetColor(0x000000ff),
                    GetColor(0xffffffff),
                    GetColor(0x880000ff),
                    GetColor(0xaaffeeff),
                    GetColor(0xcc44ccff),
                    GetColor(0x00cc55ff),
                    GetColor(0x0000aaff),
                    GetColor(0xeeee77ff),
                    GetColor(0xdd8855ff),
                    GetColor(0x664400ff),
                    GetColor(0xff7777ff),
                    GetColor(0x333333ff),
                    GetColor(0x777777ff),
                    GetColor(0xaaff66ff),
                    GetColor(0x0088ffff),
                    GetColor(0xbbbbbbff),
                },
            .background = GetColor(0x181818ff),
            .foreground = GetColor(0xeeeeeeff),
        };
    case THEME_LIGHT:
        return (Colors){
            .palette =
                {
                    GetColor(0xffffffff),
                    GetColor(0x000000ff),
                    GetColor(0x880000ff),
                    GetColor(0xaaffeeff),
                    GetColor(0xcc44ccff),
                    GetColor(0x00cc55ff),
                    GetColor(0x0000aaff),
                    GetColor(0xeeee77ff),
                    GetColor(0xdd8855ff),
                    GetColor(0x664400ff),
                    GetColor(0xff7777ff),
                    GetColor(0x333333ff),
                    GetColor(0x777777ff),
                    GetColor(0xaaff66ff),
                    GetColor(0x0088ffff),
                    GetColor(0xbbbbbbff),
                },
            .background = GetColor(0xddddddff),
            .foreground = GetColor(0x181818ff),
        };
    }
}
