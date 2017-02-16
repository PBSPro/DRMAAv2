
#
# TBD
#

AC_DEFUN([AC_ENABLE_DOXYGEN],
[
AC_ARG_ENABLE(	doxygen,
		[--enable-doxygen
		enable documentation generation with doxygen (auto)],
		[enable_doxygen=yes],
		[enable_doxygen=no])
AC_ARG_ENABLE(	dot,
		[--enable-dot
		use 'dot' to generate graphs in doxygen (auto)],
		[enable_dot=yes],
		[enable_dot=no])
AC_ARG_ENABLE(	html-docs, 	
		[--enable-html-docs
		enable HTML generati0on with doxygen (yes)],
		[enable_html_docs=yes],
		[ enable_html_docs=no])
AC_ARG_ENABLE(	latex-docs,
		[--enable-latex-docs
		enable LaTeX documentation generation with doxygen (no)],
		[enable_latex_docs=yes],
		[enable_latex_docs=no])

if test "x$enable_doxygen" = xno; then
        enable_doc=no
	AC_MSG_NOTICE([enabled not doxygen ... will not generate docs])
else 
        AC_PATH_PROG(DOXYGEN, doxygen, , $PATH)
        if test "x$DOXYGEN" = x; then
                if test "x$enable_doxygen" = xyes; then
                        AC_MSG_ERROR([could not find doxygen])
                fi
                enable_doc=no
        else
                enable_doc=yes
                AC_PATH_PROG(DOT, dot, , $PATH)
        fi
fi


if test -n "$DOXYGEN" -a "$DOXYGEN" != "no"; then
	m4_ifvaln([$1], [
	AC_MSG_CHECKING($DOXYGEN version >= $1)
	changequote(<<, >>)
	rx=`echo $1 | sed -e 's/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\1/'`
	ry=`echo $1 | sed -e 's/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\2/'`
	rz=`echo $1 | sed -e 's/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\3/'`

	version=`"$DOXYGEN" --version | tr -d '\015'`
	dx=`echo $version | sed -e 's/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\1/'`
	dy=`echo $version | sed -e 's/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\2/'`
	dz=`echo $version | sed -e 's/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*/\3/'`

	changequote([, ])
	if test $dx -gt $rx \
	|| (test $dx -eq $rx -a $dy -gt $ry) \
	|| (test $dx -eq $rx -a $dy -eq $ry -a $dz -ge $rz); then
		AC_MSG_RESULT(yes (found $dx.$dy.$dz))
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([Need DOXYGEN version $1, but only found version $dx.$dy.$dz.])
	fi
	])
fi


AM_CONDITIONAL(DOC, test x$enable_doc = xyes)
AM_CONDITIONAL(LATEX_DOC, test x$enable_latex_docs = xyes)
AM_CONDITIONAL(HTML_DOC, test x$enable_html_docs = xyes)
AM_CONDITIONAL(DOT, test x$enable_dot = xyes)

if test x$DOT = x; then
        if test "x$enable_dot" = xyes; then
                AC_MSG_ERROR([could not find dot])
        fi
        enable_dot=no
else
        enable_dot=yes
fi
AC_SUBST(enable_doc)
AC_SUBST(enable_dot)
AC_SUBST(enable_html_docs)
AC_SUBST(enable_latex_docs)
])

