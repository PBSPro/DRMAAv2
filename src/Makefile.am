
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

noinst_LTLIBRARIES=     libsrc.la

libsrc_la_SOURCES= Session.cpp  \
                   SessionManagerImpl.cpp \
                   Message.cpp \
                   SourceInfo.cpp \
                   DeniedByDrmsException.cpp \
                   ImplementationSpecificException.cpp \
                   InvalidSessionException.cpp \
                   TimeoutException.cpp \
                   UnsupportedOperationException.cpp \
                   Drmaa2Exception.cpp \
                   InternalException.cpp \
                   InvalidStateException.cpp \
                   TryLaterException.cpp \
                   DrmCommunicationException.cpp \
                   InvalidArgumentException.cpp \
                   OutOfResourceException.cpp \
                   UnsupportedAttributeException.cpp \
                   DRMSystem.cpp \
                   PBSProSystem.cpp \
                   ConnectionPool.cpp \
                   PBSConnection.cpp \
                   AttrHelper.cpp \
                   JobTemplateAttrHelper.cpp \
                   ReservationTemplateAttrHelper.cpp \
                   PBSJobImpl.cpp \
                   JobSessionImpl.cpp \
                   PBSJobArrayImpl.cpp

libsrc_la_CPPFLAGS =    -I$(top_srcdir)/inc -I$(top_srcdir)/api/cpp-binding -I$(top_srcdir)/inc -I$(drms_inc_dir)

libsrc_la_LDFLAGS  =    $(COVERAGE_LDFLAGS)

clean-local:
	rm -rf *.info *.gcno *.png *.html *.o *.lo

