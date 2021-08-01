#include <stdlib.h>
#include <stdio.h>
#include "io_file.h"

/**
 * 
 * TODO:
 * improve how we handle files and buffer chars for
 * better performance, allow for parallel io perhaps ?
 */

io_file_t *
io_file(const char *filename)
{
    FILE *file;
    io_file_t *io;

    io = malloc(sizeof (*io));
    if (!io)
        return NULL;

    file = fopen(filename, "r");
    if (!file) {
        free(io);
        return NULL;
    }

    io->file = file;
    return io;
}

void
io_file_free(io_file_t *io)
{
    fclose(io->file);
    free(io);
}

char
io_file_next(io_file_t *io)
{
    char c;

    return fgetc(io->file);
}

char
io_file_peek(io_file_t *io)
{
    char c;

    long pos = ftell(io->file);
    c = fgetc(io->file);
    fseek(io->file, pos, SEEK_SET);
    return c;
}
