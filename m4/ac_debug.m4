
#
# TBD
#

#############################################################
#The macro helps to enable/disable debugging. By default 
# the debugging is disabled. Macro defines a new macro
# DRMAA2_DEBUG, which can be used in  source
############################################################# 
AC_DEFUN([AC_DEBUG],
[
  AC_MSG_CHECKING([whether debugging is enabled])
  AC_ARG_ENABLE([debug],
    AC_HELP_STRING([--enable-debug], [enable debugging information]),
    [ case "$enableval" in
        yes) enable_debug=yes ;;
        no)  enable_debug=no ;;
        *)   AC_MSG_RESULT([none])
             AC_MSG_ERROR([bad value "$enableval" for --enable-debug]) ;;
      esac
    ]
  )

  if test "$enable_debug" = yes; then
	CFLAGS="$CFLAGS -g -O0 -Wall"
	CXXFLAGS="$CXXFLAGS -g -O0 -Wall"
	AC_DEFINE(DRMAA2_DEBUG, 1, [define to enable debugging])
  else 
	AC_DEFINE(DRMAA2_DEBUG, 0, [define to disable debugging]) 
  fi

  AC_MSG_RESULT([${enable_debug=no}])
])
