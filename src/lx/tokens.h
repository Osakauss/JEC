#ifndef JEC_TOKEN_H
#define JEC_TOKEN_H 1
#include "../symtab/symtab.h"

#define TOKEN_KIND_ERROR        0
#define TOKEN_KIND_WS           1
#define TOKEN_KIND_ID           2
#define TOKEN_KIND_SEMI         3
#define TOKEN_KIND_COMMA        4
#define TOKEN_KIND_DOT          5
#define TOKEN_KIND_OPRN         6
#define TOKEN_KIND_CPRN         7
#define TOKEN_KIND_OBRC         8
#define TOKEN_KIND_CBRC         9
#define TOKEN_KIND_OBRKT        10
#define TOKEN_KIND_CBRKT        11
// add other tokens
#define TOKEN_KIND_EOF          250

typedef struct lexer_api_t lexer_api_t;

/**
 *  note:
 * tokens must be cheap to create and move around, thus we will
 * _not_ use the standard `token_t *` and instead use actual
 * values, thus we will make sure to take care of what we do with
 * this type, e.g., strings are to be handled through a 'third party'
 * symbol manager, (this means no 'free' function !).
 */
typedef struct token_t {
    unsigned kind;
    union {
        sym_t id;
    } value;
} token_t;

/**
 * the `error` param is _not_ optional
 */
token_t
token_lex(lexer_api_t *api, symtab_t *tab, int *error);

/**
 * get the string representation of an ident
 */
const char *
token_id_str(token_t tk, symtab_t *tab);

#endif
