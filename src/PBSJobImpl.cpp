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

#include <ConnectionPool.h>
#include <Drmaa2Exception.h>
#include <JobTemplateAttrHelper.h>
#include <PBSConnection.h>
#include <PBSIFLExtend.h>
#include <PBSJobImpl.h>
#include <PBSProSystem.h>
#include <stddef.h>
#include <cstdlib>
#include <ctime>
#include <vector>

namespace drmaa2 {

PBSJobImpl::~PBSJobImpl() {
	// TODO Auto-generated destructor stub
}

const string& PBSJobImpl::getJobId(void) const {
	return _jobId;
}

const JobInfo& PBSJobImpl::getJobInfo(void) const {
	populateJobInfo();
	return _jobInfo;
}

const void PBSJobImpl::populateJobInfo(void) const {
	char *attrVal_;
	_jobInfo.jobId = _jobId;
	PBSConnection pbsconn_(pbs_default(), 0, 0);
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&pbsConnPoolObj_);
	struct batch_status *batchResponse_ = NULL;
	batchResponse_ = pbs_statjob(pbsCnHolder_->getFd(), (char *)_jobId.c_str(), NULL, (char *)"x");
	if(batchResponse_) {
		if(batchResponse_->attribs) {
			JobTemplateAttrHelper attrObj(batchResponse_->attribs);
			attrVal_ = attrObj.getAttribute((char *)ATTR_comment, NULL);
			if(attrVal_) {
				_jobInfo.annotation = string(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_exit_status, NULL);
			if(attrVal_) {
				_jobInfo.exitStatus = atol(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_state, NULL);
			if(attrVal_) {
				switch(attrVal_[0]) {
					case 'R':
						_jobInfo.jobState = RUNNING;
						break;
					case 'Q':
						_jobInfo.jobState = QUEUED;
						break;
					case 'S':
						_jobInfo.jobState = SUSPENDED;
						break;
					case 'H':
						_jobInfo.jobState = QUEUED_HELD;
						break;
					case 'F':
						_jobInfo.jobState = DONE;
						break;
					default:
						_jobInfo.jobState = UNDETERMINED;
						break;
				}
				if(_jobInfo.jobState == QUEUED || _jobInfo.jobState == QUEUED_HELD) {
					attrVal_ = attrObj.getAttribute((char *)ATTR_runcount, NULL);
					if(attrVal_) {
						if(atol(attrVal_) > 0) {
							if(_jobInfo.jobState == QUEUED)
								_jobInfo.jobState = REQUEUED;
							else if(_jobInfo.jobState == QUEUED_HELD)
								_jobInfo.jobState = REQUEUED_HELD;
						}
					}
				}
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_owner, NULL);
			if(attrVal_) {
				_jobInfo.jobOwner = string(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_queue, NULL);
			if(attrVal_) {
				_jobInfo.queueName = string(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_qtime, NULL);
			if(attrVal_) {
				_jobInfo.submissionTime = (time_t)atol(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_substate, NULL);
			if(attrVal_) {
				_jobInfo.jobSubState = string(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_stime, NULL);
			if(attrVal_) {
				_jobInfo.dispatchTime = (time_t)atol(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_etime, NULL);
			if(attrVal_) {
				_jobInfo.finishTime = (time_t)atol(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_execvnode, NULL);
			if(attrVal_) {
				string str_(attrVal_), token_;
				size_t pos_ = 0;
				while ((pos_ = str_.find('+')) != std::string::npos) {
					token_ = str_.substr(0, pos_);
					_jobInfo.allocatedMachines.push_back(token_);
					str_.erase(0, pos_ + 1);
				}
				_jobInfo.slots = _jobInfo.allocatedMachines.size();
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_used, (char *)WALLTIME);
			if(attrVal_) {
				_jobInfo.wallclockTime = atol(attrVal_);
			}
			attrVal_ = attrObj.getAttribute((char *)ATTR_used, (char *)CPUTIME);
			if(attrVal_) {
				_jobInfo.wallclockTime = atol(attrVal_);
			}
		}
		pbs_statfree(batchResponse_);
	}
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
}

const JobState& PBSJobImpl::getState(string& subState) {
	try {
		const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
		_jobState = Singleton<DRMSystem, PBSProSystem>::getInstance()->state(
				pbsConnPoolObj_, *this);
		ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
	} catch (const Drmaa2Exception &ex) {
		subState = _jobInfo.jobSubState;
		_jobState  = UNDETERMINED;
	}
	subState = _jobInfo.jobSubState;
	return _jobState;
}

const JobTemplate& PBSJobImpl::getJobTemplate(void) const {
	return _jt;
}

void PBSJobImpl::suspend(void) const throw () {
	try {
		const Connection &conn_ =
				ConnectionPool::getInstance()->getConnection();
		Singleton<DRMSystem, PBSProSystem>::getInstance()->suspend(
				ConnectionPool::getInstance()->getConnection(), *this);
		ConnectionPool::getInstance()->returnConnection(conn_);
		_jobState = SUSPENDED;
	} catch (const Drmaa2Exception &ex) {
		_jobState = UNDETERMINED;
	}
}

void PBSJobImpl::resume(void) const throw () {
	try {
		const Connection &conn_ =
				ConnectionPool::getInstance()->getConnection();
		Singleton<DRMSystem, PBSProSystem>::getInstance()->resume(
				conn_, *this);
		ConnectionPool::getInstance()->returnConnection(conn_);
		_jobState = RUNNING;
	} catch (const Drmaa2Exception &ex) {
		_jobState = UNDETERMINED;
	}
}

void PBSJobImpl::hold(void) const throw () {
	try {
		const Connection &conn_ =
				ConnectionPool::getInstance()->getConnection();
		Singleton<DRMSystem, PBSProSystem>::getInstance()->hold(
				conn_, *this);
		ConnectionPool::getInstance()->returnConnection(conn_);
		_jobState = QUEUED_HELD;
	} catch (const Drmaa2Exception &ex) {
		_jobState = UNDETERMINED;
	}
}

void PBSJobImpl::release(void) const throw () {
	try {
		const Connection &conn_ =
				ConnectionPool::getInstance()->getConnection();
		Singleton<DRMSystem, PBSProSystem>::getInstance()->release(
				conn_, *this);
		ConnectionPool::getInstance()->returnConnection(conn_);
		_jobState = RUNNING;
	} catch (const Drmaa2Exception &ex) {
		_jobState = UNDETERMINED;
	}
}

void PBSJobImpl::terminate(void) const throw () {
	try {
		const Connection &conn_ =
				ConnectionPool::getInstance()->getConnection();
		Singleton<DRMSystem, PBSProSystem>::getInstance()->terminate(
				conn_, *this);
		ConnectionPool::getInstance()->returnConnection(conn_);
		_jobState = DONE;
	} catch (const Drmaa2Exception &ex) {
		_jobState = UNDETERMINED;
	}
}

void PBSJobImpl::reap(void) const throw (){
	return;
}

void PBSJobImpl::waitStarted(const TimeAmount& timeout_) throw (){
}

void PBSJobImpl::waitTerminated(TimeAmount& timeout_) throw (){
}

} /* namespace drmaa2 */
