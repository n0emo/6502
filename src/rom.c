#include "rom.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (f) fclose(f);

    if (buf && len)
    {
        *len = m;
    }

    return buf;
}

uint8_t *load_rom(const char *path)
{
    uintptr_t len;
    unsigned char *program = read_entire_file_binary(path, &len);
    if (program == NULL)
    {
        fprintf(stderr, "Error reading file: %s", strerror(errno));
        exit(1);
    }

    if (len != 32 * 1024)
    {
        fprintf(stderr, "Incorrect ROM size: expected 32768, actual %zu:", len);
        exit(1);
    }

    return program;
}
