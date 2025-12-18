#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "addressing.c"
#include "cpu.c"
#include "gui.c"
#include "instructions.c"
#include "int.c"
#include "mem.c"
#include "rom.c"

#define SHIFT(xs, xs_sz) (assert((xs_sz) > 0), (xs_sz)--, *(xs)++)

typedef enum Subcommand
{
    SUBCOMMAND_HELP,
    SUBCOMMAND_RUN,
    SUBCOMMAND_TEST,
} Subcommand;

typedef struct Args
{
    const char *executable;
    Subcommand subcommand;
    const char *run_program;
    const char *test_rom;
} Args;

const char *usage = "Usage: %s <subcommand> [options]\n"
                    "Subcommands:\n"
                    "   help         print this message\n"
                    "   run <path>   run binary program\n"
                    "   test <path>  test cpu with rom\n";

bool parse_args(int argc, const char **argv, Args *args)
{
    *args = (Args){0};
    args->executable = SHIFT(argv, argc);
    if (argc == 0) return false;

    const char *subcommand = SHIFT(argv, argc);

    if (strcmp(subcommand, "help") == 0)
    {
        args->subcommand = SUBCOMMAND_HELP;
        return true;
    }

    if (strcmp(subcommand, "run") == 0)
    {
        if (argc == 0) return false;

        args->subcommand = SUBCOMMAND_RUN;
        args->run_program = SHIFT(argv, argc);
        return true;
    }

    if (strcmp(subcommand, "test") == 0)
    {
        if (argc == 0) return false;

        args->subcommand = SUBCOMMAND_TEST;
        args->test_rom = SHIFT(argv, argc);
        return true;
    }

    return false;
}

void run_program(const char *path)
{
    uint8_t *program = load_rom(path);

    Cpu cpu = {0};
    Memory mem = {0};

    mem_init(&mem, program);

    cpu_init(&cpu, &mem);
    cpu_reset(&cpu);
    cpu.B = 1;

    run_gui(&cpu, &mem);
}

void run_test(const char *path)
{
    unsigned char *test = load_rom(path);

    Cpu cpu = {0};
    Memory mem = {0};
    mem_append(
        &mem,
        (Device){
            .begin_address = 0,
            .end_address = 65535,
            .data = test,
            .readonly = false,
        }
    );

    cpu_init(&cpu, &mem);
    cpu_reset(&cpu);
    cpu.B = 1;

    run_gui(&cpu, &mem);

    // TODO: cleanup
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
    case SUBCOMMAND_TEST:
        run_test(args.test_rom);
        break;
    }

    return 0;
}
