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

#include <drmaa2.hpp>
#include <JobTemplateAttrHelper.h>
#include <algorithm>
#include <ctime>
#include <iterator>
#include <list>
#include <map>
#include <sstream>
#include <vector>

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
			_emailNotify.append(EMAIL_B);
		}
		if(jobTemplate_.emailOnTerminated) {
			_emailNotify.append(EMAIL_E);
		}
		setAttribute((char *) ATTR_m, (char*) _emailNotify.c_str());
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
		_emailList.assign(stream_.str());
		_emailList.erase(_emailList.size() - 1, _emailList.size());
		stream_.str("");
		stream_.clear();
		setAttribute((char*) ATTR_M, (char*)_emailList.c_str());
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
		stream_ >> _startTime;
		stream_.str("");
		stream_.clear();
		setAttribute((char *)ATTR_a, (char *)_startTime.c_str());
	}
	if(jobTemplate_.priority != 0) {
		stream_ << jobTemplate_.priority;
		stream_ >> _priority;
		stream_.str("");
		stream_.clear();
		setAttribute((char *)ATTR_p, (char *)_priority.c_str());
	}
	if(jobTemplate_.jobEnvironment.size() > 0) {
		for(map<string,string>::iterator it = jobTemplate_.jobEnvironment.begin(); it != jobTemplate_.jobEnvironment.end(); ++it) {
			_envList.append(it->first);
			_envList.append("=");
			_envList.append(it->second);
			_envList.append(",");
		}
		_envList.erase(_envList.size() - 1, _envList.size());
		setAttribute((char*) ATTR_v, (char*)_envList.c_str());
	}
	if (jobTemplate_.minPhysMemory) {
		stream_ << jobTemplate_.minPhysMemory << "KB";
		_resourceMemory.append(stream_.str());
		stream_.str("");
		stream_.clear();
		setResource((char *)MEM, (char *)_resourceMemory.c_str());
	}
	if (jobTemplate_.minSlots) {
		stream_ << jobTemplate_.minSlots;
		_resourceSlot.append(stream_.str());
		stream_.str("");
		stream_.clear();
		setResource((char *)NCPUS, (char *)_resourceSlot.c_str());
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
			_stageinFiles.append(it->second);
			_stageinFiles.append("@");
			_stageinFiles.append(it->first);
			_stageinFiles.append(",");
		}
		_stageinFiles.erase(_stageinFiles.size() - 1, _stageinFiles.size());
		setAttribute((char*) ATTR_stagein, (char*)_stageinFiles.c_str());
	}
	if(jobTemplate_.stageOutFiles.size() > 0) {
		for(map<string,string>::iterator it = jobTemplate_.stageOutFiles.begin(); it != jobTemplate_.stageOutFiles.end(); ++it) {
			_stageoutFiles.append(it->first);
			_stageoutFiles.append("@");
			_stageoutFiles.append(it->second);
			_stageoutFiles.append(",");
		}
		_stageoutFiles.erase(_stageoutFiles.size() - 1, _stageoutFiles.size());
		setAttribute((char*) ATTR_stagein, (char*)_stageoutFiles.c_str());
	}
	if (!jobTemplate_.remoteCommand.empty()) {
		setAttribute((char *) ATTR_executable, (char*) jobTemplate_.remoteCommand.c_str());

		copy(jobTemplate_.args.begin(), jobTemplate_.args.end(), ostream_iterator<string>(stream_,(char *)" "));
		_argList.assign(stream_.str());
		if(!_argList.empty()) {
			_argList.erase(_argList.size() - 1, _argList.size());
			stream_.str("");
			stream_.clear();
			_submitArguments.assign(ARGUMENT_XMLSTART);
			_submitArguments.append(_argList);
			_submitArguments.append(ARGUMENT_XMLEND);
			setAttribute((char *) ATTR_Arglist, (char*) _submitArguments.c_str());
		}
	}

	return _attrList;
}
}


