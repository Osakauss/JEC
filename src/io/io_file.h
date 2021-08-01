#ifndef IO_FILE_H
#define IO_FILE_H 1

typedef struct io_file_t {
    // avoid stdio.h (reasonable ?)
    void *file;
} io_file_t;

io_file_t *
io_file(const char *filename);

void
io_file_free(io_file_t *io);

char
io_file_next(io_file_t *io);

char
io_file_peek(io_file_t *io);

#endif
