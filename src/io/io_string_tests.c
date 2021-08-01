#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "io_string.h"

static io_string_t *io;

// ===== EMPTY STRINGS =====

static void
empty_setup(void)
{
    io = io_string("");
    ck_assert(io != NULL);
}

static void
empty_teardown(void)
{
    ck_assert(io != NULL);
    io_string_free(io);
}

START_TEST(empty_next)
{
    ck_assert(io_string_next(io) == EOF);
    ck_assert(io_string_next(io) == EOF);
    ck_assert(io_string_next(io) == EOF);
}
END_TEST

START_TEST(empty_peek)
{
    ck_assert(io_string_peek(io) == EOF);
    ck_assert(io_string_peek(io) == EOF);
    ck_assert(io_string_peek(io) == EOF);
}
END_TEST

START_TEST(empty_peek_next)
{
    ck_assert(io_string_peek(io) == EOF);
    ck_assert(io_string_next(io) == EOF);
    ck_assert(io_string_peek(io) == EOF);
    ck_assert(io_string_next(io) == EOF);
}
END_TEST

// ===== BASIC STRING ======

static void
basic_setup(void)
{
    io = io_string("abc");
    ck_assert(io != NULL);
}

static void
basic_teardown(void)
{
    ck_assert(io != NULL);
    io_string_free(io);
}

START_TEST(basic_next)
{
    ck_assert_int_eq(io_string_next(io), 'a');
    ck_assert_int_eq(io_string_next(io), 'b');
    ck_assert_int_eq(io_string_next(io), 'c');
    ck_assert_int_eq(io_string_next(io), EOF);
    ck_assert_int_eq(io_string_next(io), EOF);
}
END_TEST

START_TEST(basic_peek)
{
    ck_assert_int_eq(io_string_peek(io), 'a');
    ck_assert_int_eq(io_string_next(io), 'a');
    ck_assert_int_eq(io_string_peek(io), 'b');
    ck_assert_int_eq(io_string_next(io), 'b');
    ck_assert_int_eq(io_string_peek(io), 'c');
    ck_assert_int_eq(io_string_next(io), 'c');
    ck_assert_int_eq(io_string_peek(io), EOF);
    ck_assert_int_eq(io_string_next(io), EOF);
}
END_TEST

extern Suite *
jec_suite_io_string(void)
{
    Suite *s;
    TCase *empty;
    TCase *basic;

    s = suite_create("io_string");

    empty = tcase_create("empty");
    tcase_add_checked_fixture(empty, empty_setup, empty_teardown);
    tcase_add_test(empty, empty_next);
    tcase_add_test(empty, empty_peek);
    suite_add_tcase(s, empty);

    basic = tcase_create("basic");
    tcase_add_checked_fixture(basic, basic_setup, basic_teardown);
    tcase_add_test(basic, basic_next);
    tcase_add_test(basic, basic_peek);
    suite_add_tcase(s, basic);

    return s;
}
