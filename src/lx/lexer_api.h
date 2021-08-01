#ifndef LEXER_API_H
#define LEXER_API_H 1

typedef struct io_t io_t;

typedef struct lexer_api_t {
    io_t *io;
} lexer_api_t;

lexer_api_t *
lexer_api(io_t *io);

void
lexer_api_free(lexer_api_t *api);

char
lexer_api_next(lexer_api_t *api);

char
lexer_api_peek(lexer_api_t *api);

#endif
