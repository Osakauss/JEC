#include <stdlib.h>
#include "io.h"

io_t *
io_from_string(const char *s)
{
    io_t *io;
    io_string_t *impl;

    io = malloc(sizeof(*io));
    if (!io)
        return NULL;

    impl = io_string(s);
    if (!impl) {
        free(io);
        return NULL;
    }

    io->kind = IO_KIND_STRING;
    io->impl.string = impl;
    return io;
}

io_t *
io_from_file(const char *filename)
{
    io_t *io;
    io_file_t *impl;

    io = malloc(sizeof (*io));
    if (!io)
        return NULL;
    
    impl = io_file(filename);
    if (!impl) {
        free(io);
        return NULL;
    }

    io->kind = IO_KIND_FILE;
    io->impl.file = impl;
    return io;
}

void
io_free(io_t *io)
{
    switch (io->kind) {
    case IO_KIND_STRING:
        io_string_free(io->impl.string);
        return free(io);
    case IO_KIND_FILE:
        io_file_free(io->impl.file);
        return free(io);
    }
}

char
io_next(io_t *io)
{
    switch (io->kind) {
    case IO_KIND_STRING:
        return io_string_next(io->impl.string);
    case IO_KIND_FILE:
        return io_file_next(io->impl.file);
    }
}

char
io_peek(io_t *io)
{
    switch (io->kind) {
    case IO_KIND_STRING:
        return io_string_peek(io->impl.string);
    case IO_KIND_FILE:
        return io_file_peek(io->impl.file);
    }
}
