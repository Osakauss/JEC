#include <check.h>
#include "symtab.h"
#include "../jec/defs.h"

// ===== FIXTURE FOR EMPTY SYMTAB =====

static symtab_t *tab;

static void
setup(void)
{
    tab = symtab();
    ck_assert(tab != NULL);
}

static void
teardown(void)
{
    symtab_free(tab);
}

// ===== TESTS =====

START_TEST(bad_query_returns_null)
{
    sym_t sym;
    const char *query;

    sym = 0xDEADBEEF;
    query = symtab_query(tab, sym);
    ck_assert_ptr_eq(query, NULL);
}
END_TEST

START_TEST(insert_works)
{
    char *s;
    const char *query;
    const char *expected = "hello";
    sym_t sym;
    
    s = strdup("hello");
    ck_assert(s != NULL);

    symtab_insert(tab, &sym, s, 0);
    query = symtab_query(tab, sym);
    ck_assert_str_eq(query, expected);
    ck_assert_ptr_ne(query, s);
}
END_TEST

START_TEST(insert_owned_works)
{
    char *s;
    const char *query;
    sym_t sym;
    
    s = strdup("hello");
    ck_assert(s != NULL);

    symtab_insert(tab, &sym, s, 1);
    query = symtab_query(tab, sym);
    ck_assert_ptr_eq(query, s);
}
END_TEST

START_TEST(query_works)
{
    const char *expected = "hello";
    const char *query;
    sym_t sym;

    ck_assert(symtab_insert(tab, &sym, "hello", 0) == SUCCESS);
    ck_assert(symtab_insert(tab, NULL, "abc", 0) == SUCCESS);
    ck_assert(symtab_insert(tab, NULL, "wow !", 0) == SUCCESS);
    query = symtab_query(tab, sym);

    ck_assert(query != NULL);
    ck_assert_str_eq(query, expected);
}
END_TEST

START_TEST(same_string_yields_same_sym)
{
    const char *s;
    sym_t sym, sym_;

    s = "hello";
    ck_assert(symtab_insert(tab, &sym, s, 0) == SUCCESS);

    ck_assert(symtab_insert(tab, &sym_, s, 0) == SUCCESS);
    ck_assert_int_eq(sym, sym_);
    ck_assert(symtab_insert(tab, &sym_, s, 0) == SUCCESS);
    ck_assert_int_eq(sym, sym_);
    ck_assert(symtab_insert(tab, &sym_, s, 0) == SUCCESS);
    ck_assert_int_eq(sym, sym_);
}
END_TEST

extern Suite *
jec_suite_symtab(void)
{
    Suite *s;
    TCase *tc0, *tc1;


    s = suite_create("symtab");

    tc0 = tcase_create("limits");
    tcase_add_checked_fixture(tc0, setup, teardown);
    tcase_add_test(tc0, bad_query_returns_null);

    tc1 = tcase_create("insert-query");
    tcase_add_checked_fixture(tc1, setup, teardown);
    tcase_add_test(tc1, query_works);
    tcase_add_test(tc1, insert_works);
    tcase_add_test(tc1, insert_owned_works);
    tcase_add_test(tc1, same_string_yields_same_sym);

    suite_add_tcase(s, tc0);
    suite_add_tcase(s, tc1);
    return s;
}
