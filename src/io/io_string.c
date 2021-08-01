#include <stdlib.h>
#include <stdio.h> // for EOF
#include "io_string.h"

io_string_t *
io_string(const char *s)
{
    io_string_t *io;

    io = malloc(sizeof (*io));
    if (!io)
        return NULL;

    io->s = s;
    return io;
}

void
io_string_free(io_string_t *io)
{
    free(io);
}

char
io_string_next(io_string_t *io)
{
    char c;

    c = io->s[0];

    if (c != '\0') {
        io->s++;
        return c;
    } else {
        return EOF;
    }
}

char
io_string_peek(io_string_t *io)
{
    char c;

    c = io->s[0];

    if (c != '\0') {
        return c;
    } else {
        return EOF;
    }
}
