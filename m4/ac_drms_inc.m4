
#
# TBD
#

AC_DEFUN([AC_DRMS_INC_PATH],
[

AC_ARG_WITH(
    drms_inc_dir,
    AS_HELP_STRING(
        [--with-drms_inc_dir=DIR],
        [Specify the directory where DRMS header files are installed.]
    ),
    [drms_inc_dir="$withval"
    ],
    [
     drms_inc_dir="/opt/pbs/include"
     AC_MSG_NOTICE([taking DRMS path to $drms_inc_dir])
    ]
)

AC_SUBST(drms_inc_dir)

])
