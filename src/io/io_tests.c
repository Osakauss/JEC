#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include "io.h"

static io_t *io;

// ===== STRING IO =====

static void
setup_string(void)
{
    io = io_from_string("abc");
    ck_assert(io != NULL);
}

// ===== FILE IO =====

static void
setup_file(void)
{
    io = io_from_file(TESTDIR "/abc.c");
    ck_assert(io != NULL);
}

static void
teardown(void)
{
    ck_assert(io != NULL);
    io_free(io);
}

// ===== BASIC TESTS =====

START_TEST(test_io_next)
{
    ck_assert_int_eq(io_next(io), 'a');
    ck_assert_int_eq(io_next(io), 'b');
    ck_assert_int_eq(io_next(io), 'c');
    ck_assert_int_eq(io_next(io), EOF);
    ck_assert_int_eq(io_next(io), EOF);
}
END_TEST

START_TEST(test_io_peek)
{
    ck_assert_int_eq(io_peek(io), 'a');
    ck_assert_int_eq(io_next(io), 'a');
    ck_assert_int_eq(io_peek(io), 'b');
    ck_assert_int_eq(io_next(io), 'b');
    ck_assert_int_eq(io_peek(io), 'c');
    ck_assert_int_eq(io_next(io), 'c');
    ck_assert_int_eq(io_peek(io), EOF);
    ck_assert_int_eq(io_next(io), EOF);
}
END_TEST

START_TEST(test_kind_string)
{
    ck_assert_int_eq(io->kind, IO_KIND_STRING);
}
END_TEST

START_TEST(test_kind_file)
{
    ck_assert_int_eq(io->kind, IO_KIND_FILE);
}
END_TEST

extern Suite *
jec_suite_io(void)
{
    Suite *s;
    TCase *string;
    TCase *file;

    s = suite_create("io");

    string = tcase_create("string");
    tcase_add_checked_fixture(string, setup_string, teardown);
    tcase_add_test(string, test_io_next);
    tcase_add_test(string, test_io_peek);
    tcase_add_test(string, test_kind_string);
    suite_add_tcase(s, string);

    file = tcase_create("file");
    tcase_add_checked_fixture(file, setup_file, teardown);
    tcase_add_test(file, test_io_next);
    tcase_add_test(file, test_io_peek);
    tcase_add_test(file, test_kind_file);
    suite_add_tcase(s, file);

    return s;
}
