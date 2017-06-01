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

# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([DRMAAV2],[0.0], [pbssupport@altair.com]) 
# versioning 
DRMAA_NAME="DRMAAV2"
DRMAA_MAJOR_VERSION=2
DRMAA_MINOR_VERSION=2
DRMAA_MICRO_VERSION=0
#TODO Should be from the installed DRMS
DRMS_NAME="PBSPro"
DRMS_MAJOR_VERSION=14
DRMS_MINOR_VERSION=2
DRMS_MICRO_VERSION=0

AC_SET_VERSION

AC_DEBUG

AC_CANONICAL_TARGET

AC_CONFIG_MACRO_DIR([m4])

# Initialize automake
AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_LANG(C++)

# check for macros 
AC_DEBUG
AC_CHECK_FUNCS([strchr])
# Doxygen support
DX_INIT_DOXYGEN($DRMAA_NAME, doc/Doxyfile.in)

AC_TDD_GCOV
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_CXXFLAGS)
AC_SUBST(COVERAGE_LDFLAGS)

AC_DRMS_INC_PATH 
AC_DRMS_LIB_PATH 
AC_CONFIG_HEADER(config.h)

# Initialize Libtool
LT_INIT

AC_CONFIG_FILES(Makefile
		src/Makefile
		inc/Makefile
		api/Makefile
		test/Makefile
		test/unittesting/Makefile
		test/unittesting/src/Makefile
		test/unittesting/inc/Makefile
)
AC_OUTPUT
