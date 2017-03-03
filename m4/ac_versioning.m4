
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
AC_DEFUN([AC_SET_VERSION],
[
	AC_DEFINE_UNQUOTED(DRMAA_NAME, "$DRMAA_NAME", [DRMAA Name])
	AC_DEFINE_UNQUOTED(DRMAA_MAJOR_VERSION, "$DRMAA_MAJOR_VERSION", [DRMAA Major version])
	AC_DEFINE_UNQUOTED(DRMAA_MINOR_VERSION, "$DRMAA_MINOR_VERSION", [DRMAA Minor version]) 
	AC_DEFINE_UNQUOTED(DRMAA_MICRO_VERSION, "$DRMAA_MICRO_VERSION", [DRMAA Macro version]) 
	
	AC_DEFINE_UNQUOTED(DRMS_NAME, "$DRMS_NAME", [DRMS Name])
	AC_DEFINE_UNQUOTED(DRMS_MAJOR_VERSION, "$DRMS_MAJOR_VERSION", [DRMS Major version])
	AC_DEFINE_UNQUOTED(DRMS_MINOR_VERSION, "$DRMS_MINOR_VERSION", [DRMS Minor version]) 
	AC_DEFINE_UNQUOTED(DRMS_MICRO_VERSION, "$DRMS_MICRO_VERSION", [DRMS Macro version]) 
])


