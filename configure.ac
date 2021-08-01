AC_PREREQ([2.69])
AC_INIT([jec], [0.0.0], [https://github.com/Osakuass/JEC], [])

### some testing ###
AC_CONFIG_SRCDIR([src/jec/defs.h])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/io/Makefile
    src/lx/Makefile
    src/symtab/Makefile
])


AM_INIT_AUTOMAKE([-Wall -Werror foreign 1.16.1])

### check for programs ###
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB

### check for libraries ###
PKG_CHECK_MODULES([CHECK], [check >= 0.10.0])

### misc ###
# directory for test files
AC_DEFINE_UNQUOTED([TESTDIR], ["${ac_abs_confdir}/src/tests/"], [Location for test-related files])
AC_CONFIG_HEADERS([config.h])

AC_OUTPUT
