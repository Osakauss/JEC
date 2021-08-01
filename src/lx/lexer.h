#ifndef JEC_LEXER_H
#define JEC_LEXER_H

struct lexer_api_t;

typedef struct lexer_t {
    struct lexer_api_t *api;
} lexer_t;

typedef struct token_t {

} token_t;

lexer_t *
lexer(io_t *io);

void
lexer_free(lexer_t *lx);

token_t
lexer_next(lexer_t *lx);

token_t
lexer_peek(lexer_t *lx);

#endif
