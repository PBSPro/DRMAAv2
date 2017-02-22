
#
#  Copyright (C) 1994-2017 Altair Engineering, Inc.
#  For more information, contact Altair at www.altair.com.
#   
#  This file is part of the PBS Professional ("PBS Pro") software.
#  
#  Open Source License Information:
#   
#  PBS Pro is free software. You can redistribute it and/or modify it under the
#  terms of the GNU Affero General Public License as published by the Free 
#  Software Foundation, either version 3 of the License, or (at your option) any 
#  later version.
#   
#  PBS Pro is distributed in the hope that it will be useful, but WITHOUT ANY 
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
#  PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
#   
#  You should have received a copy of the GNU Affero General Public License along 
#  with this program.  If not, see <http://www.gnu.org/licenses/>.
#   
#  Commercial License Information: 
#  
#  The PBS Pro software is licensed under the terms of the GNU Affero General 
#  Public License agreement ("AGPL"), except where a separate commercial license 
#  agreement for PBS Pro version 14 or later has been executed in writing with Altair.
#   
#  Altair’s dual-license business model allows companies, individuals, and 
#  organizations to create proprietary derivative works of PBS Pro and distribute 
#  them - whether embedded or bundled with other software - under a commercial 
#  license agreement.
#  
#  Use of Altair’s trademarks, including but not limited to "PBS™", 
#  "PBS Professional®", and "PBS Pro™" and Altair’s logos is subject to Altair's 
#  trademark licensing policies.
#

AC_DEFUN([AC_ENABLE_DOXYGEN],
[
AC_ARG_ENABLE(	doxygen,
		[--enable-doxygen
		enable documentation generation with doxygen (no)],
		[enable_doxygen=yes],
		[enable_doxygen=no])
AC_ARG_ENABLE(	dot,
		[--enable-dot
		use 'dot' to generate graphs in doxygen (no)],
		[enable_dot=yes],
		[enable_dot=no])
AC_ARG_ENABLE(	html-docs, 	
		[--enable-html-docs
		enable HTML generation with doxygen (yes)],
		[enable_html_docs=yes],
		[ enable_html_docs=no])
AC_ARG_ENABLE(	latex-docs,
		[--enable-latex-docs
		enable LaTeX documentation generation with doxygen (no)],
		[enable_latex_docs=yes],
		[enable_latex_docs=no])

if test "x$enable_doxygen" = xno; then
        enable_doc=no
	AC_MSG_NOTICE([Will not generate docs])
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

