#include <stdlib.h>
#include <check.h>

extern Suite *jec_suite_io_string(void);
extern Suite *jec_suite_io_file(void);
extern Suite *jec_suite_io(void);
extern Suite *jec_suite_tokens(void);
extern Suite *jec_suite_symtab(void);

extern int
main(int argc, char **argv)
{
    int fails;
    SRunner *sr;
    Suite *s;

    s = suite_create("jec");

    sr = srunner_create(s);
    srunner_add_suite(sr, jec_suite_io_string());
    srunner_add_suite(sr, jec_suite_io_file());
    srunner_add_suite(sr, jec_suite_io());
    srunner_add_suite(sr, jec_suite_tokens());
    srunner_add_suite(sr, jec_suite_symtab());

    srunner_run_all(sr, CK_VERBOSE);
    fails = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (fails != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
