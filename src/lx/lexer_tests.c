#include <check.h>

extern Suite *
jec_suite_lexer(void)
{
    Suite *s;

    s = suite_create("lexer");

    return s;
}
