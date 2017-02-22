
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
