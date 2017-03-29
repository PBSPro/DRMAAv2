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
#include "JobTemplateAttrHelper.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <string.h>
#include <functional>
#include <numeric>
#include <string.h>

namespace drmaa2 {

ATTRL* JobTemplateAttrHelper::parseTemplate(void* template_) {
	stringstream stream_;
	string searchStr_;
	map<string,string>::iterator it;
	if(template_ == NULL)
		return _attrList;
	JobTemplate *_jT = static_cast<JobTemplate *>(template_);
	JobTemplate &jobTemplate_ = *_jT;
	if(jobTemplate_.emailOnStarted || jobTemplate_.emailOnTerminated) {
		if (jobTemplate_.emailOnStarted) {
			emailNotify_.append(EMAIL_B);
		}
		if(jobTemplate_.emailOnTerminated) {
			emailNotify_.append(EMAIL_E);
		}
		setAttribute((char *) ATTR_m, (char*) emailNotify_.c_str());
	}
	if(jobTemplate_.submitAsHold) {
		setAttribute((char *)ATTR_h, (char*)USER_HOLD);
	}
	if (!jobTemplate_.rerunnable) {
		setAttribute((char *)ATTR_r, (char*)NO);
	} else {
		setAttribute((char *)ATTR_r, (char*)YES);
	}
	if (jobTemplate_.email.size() > 0) {
		copy(jobTemplate_.email.begin(), jobTemplate_.email.end(), ostream_iterator<string>(stream_,(char *)","));
		emailList_.assign(stream_.str());
		emailList_.erase(emailList_.size() - 1, emailList_.size());
		stream_.str("");
		stream_.clear();
		setAttribute((char*) ATTR_M, (char*)emailList_.c_str());
	}
	if (!jobTemplate_.jobName.empty()) {
		setAttribute((char *) ATTR_N, (char*)jobTemplate_.jobName.c_str());
	}
	if (!jobTemplate_.workingDirectory.empty()) {
		setAttribute((char *)ATTR_sandbox, (char *)jobTemplate_.workingDirectory.c_str());
	}
	if (!jobTemplate_.outputPath.empty()) {
		setAttribute((char *) ATTR_o, (char*) jobTemplate_.outputPath.c_str());
	}
	if(!jobTemplate_.errorPath.empty()) {
		setAttribute((char *) ATTR_e, (char*) jobTemplate_.errorPath.c_str());
	}
	if (jobTemplate_.joinFiles) {
		setAttribute((char *) ATTR_j, (char*) OUTERR);
	}
	if(!jobTemplate_.accountingId.empty()) {
		setAttribute((char *) ATTR_A, (char*) jobTemplate_.accountingId.c_str());
	}
	if((long)jobTemplate_.startTime > 0) {
		struct tm * timeinfo;
		timeinfo = localtime ( &jobTemplate_.startTime );
		stream_ << mktime(timeinfo);
		stream_ >> startTime_;
		stream_.str("");
		stream_.clear();
		setAttribute((char *)ATTR_a, (char *)startTime_.c_str());
	}
	if(jobTemplate_.priority != 0) {
		stream_ << jobTemplate_.priority;
		stream_ >> priority_;
		stream_.str("");
		stream_.clear();
		setAttribute((char *)ATTR_p, (char *)priority_.c_str());
	}
	if(jobTemplate_.jobEnvironment.size() > 0) {
		for(map<string,string>::iterator it = jobTemplate_.jobEnvironment.begin(); it != jobTemplate_.jobEnvironment.end(); ++it) {
			envList_.append(it->first);
			envList_.append("=");
			envList_.append(it->second);
			envList_.append(",");
		}
		envList_.erase(envList_.size() - 1, envList_.size());
		setAttribute((char*) ATTR_v, (char*)envList_.c_str());
	}
	if (jobTemplate_.minPhysMemory) {
		stream_ << jobTemplate_.minPhysMemory << "KB";
		resourceMemory_.append(stream_.str());
		stream_.str("");
		stream_.clear();
		setResource((char *)MEM, (char *)resourceMemory_.c_str());
	}
	if (jobTemplate_.minSlots) {
		stream_ << jobTemplate_.minSlots;
		resourceSlot_.append(stream_.str());
		stream_.str("");
		stream_.clear();
		setResource((char *)NCPUS, (char *)resourceSlot_.c_str());
	}
	searchStr_.assign(DRMAA2_WALLCLOCK_TIME);
	it = jobTemplate_.resourceLimits.find(searchStr_);
	if(it != jobTemplate_.resourceLimits.end()) {
		setResource((char *)WALLTIME, (char *)it->second.c_str());
	}
	searchStr_.assign(DRMAA2_CPU_TIME);
	it = jobTemplate_.resourceLimits.find(searchStr_);
	if(it != jobTemplate_.resourceLimits.end()) {
		setResource((char *)CPUTIME, (char *)it->second.c_str());
	}
	if(jobTemplate_.machineOS > 0) {
		switch(jobTemplate_.machineOS) {
		case LINUX:
			setResource((char *)ARCH, (char *)OS_LINUX);
		default:
			break;
		}
	}
	if(jobTemplate_.stageInFiles.size() > 0) {
		for(map<string,string>::iterator it = jobTemplate_.stageInFiles.begin(); it != jobTemplate_.stageInFiles.end(); ++it) {
			stageinFiles_.append(it->second);
			stageinFiles_.append("@");
			stageinFiles_.append(it->first);
			stageinFiles_.append(",");
		}
		stageinFiles_.erase(stageinFiles_.size() - 1, stageinFiles_.size());
		setAttribute((char*) ATTR_stagein, (char*)stageinFiles_.c_str());
	}
	if(jobTemplate_.stageOutFiles.size() > 0) {
		for(map<string,string>::iterator it = jobTemplate_.stageOutFiles.begin(); it != jobTemplate_.stageOutFiles.end(); ++it) {
			stageoutFiles_.append(it->first);
			stageoutFiles_.append("@");
			stageoutFiles_.append(it->second);
			stageoutFiles_.append(",");
		}
		stageoutFiles_.erase(stageoutFiles_.size() - 1, stageoutFiles_.size());
		setAttribute((char*) ATTR_stagein, (char*)stageoutFiles_.c_str());
	}
	if (!jobTemplate_.remoteCommand.empty()) {
		setAttribute((char *) ATTR_executable, (char*) jobTemplate_.remoteCommand.c_str());

		copy(jobTemplate_.args.begin(), jobTemplate_.args.end(), ostream_iterator<string>(stream_,(char *)" "));
		argList_.assign(stream_.str());
		if(!argList_.empty()) {
			argList_.erase(argList_.size() - 1, argList_.size());
			stream_.str("");
			stream_.clear();
			submitArguments_.assign(ARGUMENT_XMLSTART);
			submitArguments_.append(argList_);
			submitArguments_.append(ARGUMENT_XMLEND);
			setAttribute((char *) ATTR_Arglist, (char*) submitArguments_.c_str());
		}
	}

	return _attrList;
}
}


