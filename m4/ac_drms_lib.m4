
#
# TBD
#

AC_DEFUN([AC_DRMS_LIB_PATH],
[

AC_CHECK_LIB([pthread], [pthread_mutex_init], [], [echo "pthreads not found.  Please install pthread library before proceeding"; exit -1])
AC_CHECK_LIB([crypto], [main], [], [echo "openssl crypto library not found. Please install openssl library before proceeding"; exit -1])

AC_ARG_WITH(
    drms_lib_dir,
    AS_HELP_STRING(
        [--with-drms_lib_dir=DIR],
        [Specify the directory where DRMS libraries are installed.]
    ),
    [drms_lib_dir="$withval"
    ],
    [
     drms_lib_dir="/opt/pbs/lib"
     AC_MSG_NOTICE([taking DRMS path to $drms_lib_dir])
    ]
)

AC_SUBST(drms_lib_dir)

LDFLAGS="$LDFLAGS -L$drms_lib_dir -lpbs"

AC_CHECK_LIB([pbs],[get_attr],
                   [AC_MSG_NOTICE(libpbs check passed)
                  ],
                  [AC_MSG_FAILURE(libpbs check failed)],
)

])
