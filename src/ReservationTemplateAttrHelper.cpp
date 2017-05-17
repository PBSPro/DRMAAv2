/*
 * Copyright (C) 1994-2017 Altair Engineering, Inc.
 * For more information, contact Altair at www.altair.com.
 *
 * This file is part of the PBS Professional ("PBS Pro") software.
 *
 * Open Source License Information:
 *
 * PBS Pro is free software. You can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * PBS Pro is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Commercial License Information:
 *
 * The PBS Pro software is licensed under the terms of the GNU Affero General
 * Public License agreement ("AGPL"), except where a separate commercial license
 * agreement for PBS Pro version 14 or later has been executed in writing with Altair.
 *
 * Altair’s dual-license business model allows companies, individuals, and
 * organizations to create proprietary derivative works of PBS Pro and distribute
 * them - whether embedded or bundled with other software - under a commercial
 * license agreement.
 *
 * Use of Altair’s trademarks, including but not limited to "PBS™",
 * "PBS Professional®", and "PBS Pro™" and Altair’s logos is subject to Altair's
 * trademark licensing policies.
 *
 */
#include "drmaa2.hpp"
#include "DRMSystem.h"
#include "AttrHelper.h"
#include "ReservationTemplateAttrHelper.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <string.h>
#include <functional>
#include <numeric>
#include <string.h>


namespace drmaa2 {

ATTRL* ReservationTemplateAttrHelper::parseTemplate(void* template_) {
	if(template_ == NULL)
		return _attrList;
	stringstream stream_;
	ReservationTemplate *_rT = static_cast<ReservationTemplate *>(template_);
	ReservationTemplate &reservationTemplate_ = *_rT;
	if(!reservationTemplate_.reservationName.empty()) {
		setAttribute((char *)ATTR_resv_name, (char *)reservationTemplate_.reservationName.c_str());
	}
	if((long)reservationTemplate_.startTime > 0) {
		struct tm * timeinfo;
		timeinfo = localtime ( &reservationTemplate_.startTime );
		stream_ << mktime(timeinfo);
		stream_ >> startTime_;
		stream_.str("");
		stream_.clear();
		setAttribute((char *)ATTR_resv_start, (char *)startTime_.c_str());
	}
	if((long)reservationTemplate_.endTime > 0) {
		struct tm * timeinfo;
		timeinfo = localtime ( &reservationTemplate_.endTime );
		stream_ << mktime(timeinfo);
		stream_ >> endTime_;
		stream_.str("");
		stream_.clear();
		setAttribute((char *)ATTR_resv_end, (char *)endTime_.c_str());
	}
	if(reservationTemplate_.duration > 0) {
		stream_ << reservationTemplate_.duration;
		stream_ >> durationTime_;
		stream_.str("");
		stream_.clear();
		setResource((char *)WALLTIME, (char *)durationTime_.c_str());
	}
	if(reservationTemplate_.minSlots) {
		stream_ << reservationTemplate_.minSlots;
		resourceSlot_.append(stream_.str());
		stream_.str("");
		stream_.clear();
		setResource((char *)NCPUS, (char *)resourceSlot_.c_str());
	}
	if (reservationTemplate_.minPhysMemory) {
		stream_ << reservationTemplate_.minPhysMemory << "KB";
		resourceMemory_.append(stream_.str());
		stream_.str("");
		stream_.clear();
		setResource((char *)MEM, (char *)resourceMemory_.c_str());
	}
	if(reservationTemplate_.candidateMachines.size() > 0) {
		copy(reservationTemplate_.candidateMachines.begin(), reservationTemplate_.candidateMachines.end(), ostream_iterator<string>(stream_,(char *)","));
		candidateMachines_.assign(stream_.str());
		candidateMachines_.erase(candidateMachines_.size() - 1, candidateMachines_.size());
		stream_.str("");
		stream_.clear();
		setAttribute((char*) ATTR_auth_h, (char*)candidateMachines_.c_str());
	}
	if(reservationTemplate_.machineOS > 0) {
		switch(reservationTemplate_.machineOS) {
		case LINUX:
			setResource((char *)ARCH, (char *)OS_LINUX);
		default:
			break;
		}
	}
	if(reservationTemplate_.usersACL.size() > 0) {
		copy(reservationTemplate_.usersACL.begin(), reservationTemplate_.usersACL.end(), ostream_iterator<string>(stream_,(char *)","));
		aclUsers_.assign(stream_.str());
		aclUsers_.erase(aclUsers_.size() - 1, aclUsers_.size());
		stream_.str("");
		stream_.clear();
		setAttribute((char*) ATTR_auth_u, (char*)aclUsers_.c_str());
	}
	return _attrList;
}

}


