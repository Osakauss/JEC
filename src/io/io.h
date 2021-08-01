#ifndef JEC_IO_H
#define JEC_IO_H 1
#include "io_string.h"
#include "io_file.h"

#define IO_KIND_ERROR       0
#define IO_KIND_STRING      1
#define IO_KIND_FILE        2

typedef struct io_t {
    int kind;
    union {
        io_string_t *string;
        io_file_t *file;
    } impl;
} io_t;

io_t *
io_from_string(const char *s);

io_t *
io_from_file(const char *filename);

void
io_free(io_t *io);

char
io_next(io_t *io);

char
io_peek(io_t *io);

#endif
