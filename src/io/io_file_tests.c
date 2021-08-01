#include <config.h>
#include <stdio.h>
#include <check.h>
#include "io_file.h"

static io_file_t *io;

// ===== EMPTY FILE =====

static void
empty_setup(void)
{
    io = io_file(TESTDIR "/empty.c");
    ck_assert(io != NULL);
}

// ===== ABC FILE =====

static void
abc_setup(void)
{
    io = io_file(TESTDIR "/abc.c");
    ck_assert(io != NULL);
}

static void
teardown(void)
{
    io_file_free(io);
}

// ===== TESTS =====

START_TEST(test_empty_next)
{
    ck_assert_int_eq(io_file_next(io), EOF);
    ck_assert_int_eq(io_file_next(io), EOF);
    ck_assert_int_eq(io_file_next(io), EOF);
}
END_TEST

START_TEST(test_empty_peek)
{
    ck_assert_int_eq(io_file_peek(io), EOF);
    ck_assert_int_eq(io_file_peek(io), EOF);
    ck_assert_int_eq(io_file_peek(io), EOF);
}
END_TEST

START_TEST(test_abc_next)
{
    ck_assert_int_eq(io_file_next(io), 'a');
    ck_assert_int_eq(io_file_next(io), 'b');
    ck_assert_int_eq(io_file_next(io), 'c');
    ck_assert_int_eq(io_file_next(io), EOF);
    ck_assert_int_eq(io_file_next(io), EOF);
}
END_TEST

START_TEST(test_abc_peek)
{
    ck_assert_int_eq(io_file_peek(io), 'a');
    ck_assert_int_eq(io_file_next(io), 'a');
    ck_assert_int_eq(io_file_peek(io), 'b');
    ck_assert_int_eq(io_file_next(io), 'b');
    ck_assert_int_eq(io_file_peek(io), 'c');
    ck_assert_int_eq(io_file_next(io), 'c');
    ck_assert_int_eq(io_file_peek(io), EOF);
    ck_assert_int_eq(io_file_next(io), EOF);
}
END_TEST

extern Suite *
jec_suite_io_file(void)
{
    Suite *s;
    TCase *empty;
    TCase *abc;

    s = suite_create("io_file");

    empty = tcase_create("empty");
    tcase_add_checked_fixture(empty, empty_setup, teardown);
    tcase_add_test(empty, test_empty_next);
    tcase_add_test(empty, test_empty_peek);

    abc = tcase_create("abc");
    tcase_add_checked_fixture(abc, abc_setup, teardown);
    tcase_add_test(abc, test_abc_next);
    tcase_add_test(abc, test_abc_peek);

    suite_add_tcase(s, empty);
    suite_add_tcase(s, abc);

    return s;
}
