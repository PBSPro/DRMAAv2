
#
# TBD
#

AC_DEFUN([AC_TDD_GCOV],
[

AC_ARG_ENABLE(  gcov,
                [--enable-gcov
                enable coverage report generation(auto)],
                [enable_gcov=yes],
                [enable_gcov=no])

AM_CONDITIONAL(HAVE_GCOV, test "x$enable_gcov" = "xyes")

  if test "x$enable_gcov" = "xyes"; then
	AC_MSG_NOTICE([gcov has been enabled])
	COVERAGE_CFLAGS="-fprofile-arcs -ftest-coverage"
	COVERAGE_CXXFLAGS="-fprofile-arcs -ftest-coverage"
	COVERAGE_LDFLAGS="-lgcov --coverage"

	AC_MSG_NOTICE([gcov flags are $COVERAGE_CFLAGS and $COVERAGE_LDFLAGS:])
  fi

  # Check if ccache is being used
  AC_CHECK_PROG(SHTOOL, shtool, shtool)
  if test "$SHTOOL"; then
    AS_CASE([`$SHTOOL path $CC`],
                [*ccache*], [gcc_ccache=yes],
                [gcc_ccache=no])
  fi

  if test "$gcc_ccache" = "yes" && (test -z "$CCACHE_DISABLE" || test "$CCACHE_DISABLE" != "1"); then
    AC_MSG_ERROR([ccache must be disabled when --enable-gcov option is used. You can disable ccache by setting environment variable CCACHE_DISABLE=1.])
  fi

]) # AC_TDD_GCOV

