#include <stdlib.h>
#include "lexer_api.h"
#include "../io/io.h"

lexer_api_t *
lexer_api(io_t *io)
{
    lexer_api_t *api;

    api = malloc(sizeof (*api));
    if (!api)
        return NULL;

    api->io = io;
    return api;
}

void
lexer_api_free(lexer_api_t *api)
{
    io_free(api->io);
    free(api);
}

char
lexer_api_next(lexer_api_t *api)
{
    return io_next(api->io);
}

char
lexer_api_peek(lexer_api_t *api)
{
    return io_peek(api->io);
}
