#include <check.h>
#include "../io/io.h"
#include "../symtab/symtab.h"
#include "lexer_api.h"
#include "tokens.h"

lexer_api_t *api;
symtab_t *tab;

// ===== FIXTURE FOR SOME SYMBOLS =====

static void
setup_some_symbols(void)
{
    io_t *io;
    io = io_from_string(";,.{");
    api = lexer_api(io);
    tab = symtab();
    ck_assert(api != NULL);
    ck_assert(tab != NULL);
}

static void
teardown_some_symbols(void)
{
    lexer_api_free(api);
    symtab_free(tab);
}

START_TEST(test_some_symbols)
{
    int error;
    ck_assert_int_eq(token_lex(api, tab, &error).kind, TOKEN_KIND_SEMI);
    ck_assert_int_eq(token_lex(api, tab, &error).kind, TOKEN_KIND_COMMA);
    ck_assert_int_eq(token_lex(api, tab, &error).kind, TOKEN_KIND_DOT);
    ck_assert_int_eq(token_lex(api, tab, &error).kind, TOKEN_KIND_OBRC);
}
END_TEST

// ===== FIXTURE FOR SOME IDENTS =====

static void
setup_idents(void)
{
    api = lexer_api(io_from_string("foo"));
    ck_assert(api != NULL);
    tab = symtab();
}

static void
teardown_idents(void)
{
    lexer_api_free(api);
    symtab_free(tab);
}

START_TEST(lex_idents)
{

}
END_TEST

extern Suite *
jec_suite_tokens(void)
{
    Suite *s;
    TCase *tc0, *tc1;

    s = suite_create("tokens");

    tc0 = tcase_create("some-symbols");
    tcase_add_checked_fixture(tc0, setup_some_symbols, teardown_some_symbols);
    tcase_add_test(tc0, test_some_symbols);

    tc1 = tcase_create("idents");
    tcase_add_test(tc1, lex_idents);

    suite_add_tcase(s, tc0);
    suite_add_tcase(s, tc1);
    return s;
}
