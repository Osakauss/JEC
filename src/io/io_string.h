#ifndef IO_STRING_H
#define IO_STRING_H 1

typedef struct io_string_t {
    const char *s;
} io_string_t;

io_string_t *
io_string(const char *s);

void
io_string_free(io_string_t *io);

char
io_string_next(io_string_t *io);

char
io_string_peek(io_string_t *io);

#endif
