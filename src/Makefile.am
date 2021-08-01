SUBDIRS = io lx symtab .

# files used for testing
dist_pkgdata_DATA = \
    tests/empty.c \
    tests/abc.c

TESTS = jec_tests
check_PROGRAMS = jec_tests

jec_tests_SOURCES = test.c
jec_tests_CFLAGS = @CHECK_CFLAGS@
jec_tests_LDADD = \
    @CHECK_LIBS@ \
    io/libjec_io_tests.a \
    io/libjec_io.a \
    lx/libjec_lx_tests.a \
    lx/libjec_lx.a \
    symtab/libjec_symtab_tests.a \
    symtab/libjec_symtab.a 
