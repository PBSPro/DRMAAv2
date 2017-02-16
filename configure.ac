
#
# TBD
#

# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([DRMAAV2],[0.0], [pbssupport@altair.com])

#Versioning
DRMAA2_MAJOR_VERSION=0
DRMAA2_MINOR_VERSION=0
DRMAA2_MICRO_VERSION=0
DRMAA2_INTERFACE_AGE=0
DRMAA2_BINARY_AGE=0
AC_SET_VERSION


AC_CANONICAL_TARGET
AC_CONFIG_MACRO_DIR([m4])

# Initialize automake
AM_INIT_AUTOMAKE
# check for programs 

AC_PROG_CXX
AC_LANG(C++)

AC_CHECK_FUNCS([strchr])

# check for macros
AC_DEBUG
AC_ENABLE_DOXYGEN([1.8.5])
AC_DRMS_INC_PATH
AC_DRMS_LIB_PATH

AC_TDD_GCOV
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_CXXFLAGS)
AC_SUBST(COVERAGE_LDFLAGS)


# Initialize Libtool
LT_INIT

AC_CONFIG_FILES(Makefile
		src/Makefile
		inc/Makefile
		api/Makefile
		doc/Makefile
		test/Makefile
		test/unittesting/Makefile
		test/unittesting/src/Makefile
		test/unittesting/inc/Makefile
)
AC_OUTPUT
