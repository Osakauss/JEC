#include <stdlib.h>
#include <stdio.h> // for EOF
#include "tokens.h"
#include "lexer_api.h"
#include "../jec/defs.h"

// ===== DEFINES =====

// max size for a single identifier string
#define MAX_IDENT_SIZE          512

// ===== TYPES OF CHARS AS UNDERSTOOD PER THE LEXER =====

static inline int
char_is_ws(char c)
{
    return
        (c == ' ') ||
        (c == '\t') ||
        (c == '\n') ||
        (c == '\r') ||
        (c == '\b');
}

static inline int
char_is_id_start(char c)
{
    return
        (c == '_')             ||
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z');
}

static inline int
char_is_id_cont(char c)
{
    return
        char_is_id_start(c) ||
        (c >= '0' && c <= '9');
}

// ===== STATIC METHODS =====

// ONLY FOR USE IN `token_lex_symbol`
#define lex_symbol_case(c, k) \
    case c: \
        tk.kind = TOKEN_KIND_ ## k; \
        goto success;

static token_t
token_lex_symbol(lexer_api_t *api)
{
    char peek;
    token_t tk;

    peek = lexer_api_peek(api);
    switch (peek) {
        lex_symbol_case(';', SEMI);
        lex_symbol_case(',', COMMA);
        lex_symbol_case('.', DOT);
        lex_symbol_case('(', OPRN);
        lex_symbol_case(')', CPRN);
        lex_symbol_case('{', OBRC);
        lex_symbol_case('}', CBRC);
        lex_symbol_case('[', OBRKT);
        lex_symbol_case(']', CBRKT);
        lex_symbol_case(EOF, EOF);
    }

    tk.kind = TOKEN_KIND_ERROR;
    return tk;

success:
    lexer_api_next(api);
    return tk;
}

#undef lex_symbol_case

static token_t
token_lex_ident(lexer_api_t *api, symtab_t *tab, int *error)
{
    char peek;
    char *id = NULL;
    size_t size = 0;
    token_t tk;

    peek = lexer_api_peek(api);
    if (!char_is_id_start(peek))
        goto failure;

    id = malloc(MAX_IDENT_SIZE + 1);
    if (!id) {
        *error = 1;
        goto failure;
    }

    // consume first char
    id[size++] = peek;
    lexer_api_next(api);

    for (;;) {
        peek = lexer_api_peek(api);
        if (!char_is_id_cont(peek)) {
            break;
        }

        if (size >= MAX_IDENT_SIZE) {
            *error = 1;
            goto failure;
        }

        id[size++] = peek;
        lexer_api_next(api);
    }

    id[size] = '\0';
    // TODO: is it safe to ignore ??
    void *_ignore = realloc(id, size + 1);
    if (symtab_insert(tab, &tk.value.id, id, 1) == SUCCESS) {
        tk.kind = TOKEN_KIND_ID;
        return tk;
    } else {
        *error = 1;
        goto failure;
    }
    
failure:
    if (id)
        free(id);
    tk.kind = TOKEN_KIND_ERROR;
    return tk;
}

// ===== EXPORTS =====

token_t
token_lex(lexer_api_t *api, symtab_t *tab, int *error)
{
    token_t tk;

    *error = 0;
    
    tk = token_lex_symbol(api);
    if (tk.kind != TOKEN_KIND_ERROR)
        return tk;
    
    tk = token_lex_ident(api, tab, error);
    if (error)
        goto failure;

    if (tk.kind != TOKEN_KIND_ERROR)
        return tk;
    
failure:
    tk.kind = TOKEN_KIND_ERROR;
    return tk;
}

const char *
token_id_str(token_t tk, symtab_t *tab)
{
    if (tk.kind != TOKEN_KIND_ID)
        return NULL;

    return symtab_query(tab, tk.value.id);
}
