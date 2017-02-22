
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
