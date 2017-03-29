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

#include "PBSProSystem.h"
#include "drmaa2.hpp"
#include "Message.h"
#include "Drmaa2Exception.h"
#include "InvalidArgumentException.h"
#include "JobTemplateAttrHelper.h"
#include "DeniedByDrmsException.h"
#include "InvalidArgumentException.h"
#include "InvalidStateException.h"
#include "ImplementationSpecificException.h"
#include "SourceInfo.h"
#include "pbs_ifl.h"
#include "pbs_error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace drmaa2 {
pthread_mutex_t PBSProSystem::_posixMutex = PTHREAD_MUTEX_INITIALIZER;

PBSProSystem::PBSProSystem() {
	// TODO Auto-generated constructor stub

}

PBSProSystem::~PBSProSystem() {
	// TODO Auto-generated destructor stub
}

void PBSProSystem::connect(Connection & connection_) throw () {
	PBSConnection *pbsCnHolder_ = static_cast<PBSConnection*>(&connection_);
	int fd_ = pbs_connect((char *)pbsCnHolder_->getServerName().c_str());
	if(fd_ < 0) {
		if(pbs_errno == PBSE_NOSERVER)
			throw ImplementationSpecificException(PBSE_NOSERVER,
				SourceInfo(__func__,__LINE__));
		else if(pbs_errno == PBSE_NOCONNECTS)
			throw Drmaa2Exception(Message(DRMS_NO_CONNECTS),
					SourceInfo(__func__,__LINE__));
		else if((pbs_errno == PBSE_SYSTEM) || (pbs_errno == PBSE_PERM))
			throw Drmaa2Exception(Message(DRMS_CONN_AUTHFAIL),
					SourceInfo(__func__,__LINE__));
		else
			throw DeniedByDrmsException(SourceInfo(__func__,__LINE__));
	} else {
		pbsCnHolder_->setFd(fd_);
	}
}

void PBSProSystem::disconnect(const Connection & connection_) throw () {
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&connection_);
	int ret_ = pbs_disconnect(pbsCnHolder_->getFd());
	if(ret_ < 0) {
		throw DeniedByDrmsException(SourceInfo(__func__,__LINE__),
				Message(DENIED_BY_DRMS_LONG));
	}
}

Job* PBSProSystem::runJob(const Connection& connection_,
		const JobTemplate& jobTemplate_) throw () {
	string destination_;
	char *jobIdFromDRMS_;
	const PBSConnection *pbsCnHolder_ =
			static_cast<const PBSConnection*>(&connection_);
	JobTemplateAttrHelper _attrParse;

	if (!jobTemplate_.reservationId.empty())
		destination_.append(jobTemplate_.queueName);
	else if (!jobTemplate_.queueName.empty())
		destination_.append(jobTemplate_.queueName);

	ATTRL *attributeList = _attrParse.parseTemplate((void*)&jobTemplate_);
	jobIdFromDRMS_ = pbs_submit(pbsCnHolder_->getFd(), (struct attropl *) attributeList,
			(char*) jobTemplate_.remoteCommand.c_str(), (char*) destination_.c_str(), NULL);
	if(jobIdFromDRMS_) {
		return (Job*)jobIdFromDRMS_; //TODO: Needs to be changed to Job object
	} else {
		throw ImplementationSpecificException(pbs_errno,SourceInfo(__func__,__LINE__));
	}
}

void PBSProSystem::hold(const Connection& connection_, const Job& job_) throw () {
	int ret_;
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&connection_);
	string jobId_;
	jobId_.assign((char*) &job_); //TODO: Needs to be changed after Job object implemented
	if (jobId_.empty())
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_SHORT));
	ret_ = pbs_holdjob(pbsCnHolder_->getFd(), (char*)jobId_.c_str(), NULL, NULL);
	if(ret_ != 0)
		throw InvalidStateException(SourceInfo(__func__,__LINE__),
				Message(INVALID_STATE_LONG));
}

void PBSProSystem::suspend(const Connection& connection_,
		const Job& job_) throw () {
	int ret_;
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&connection_);
	string jobId_;
	jobId_.assign((char*) &job_); //TODO: Needs to be changed after Job object implemented
	if (jobId_.empty())
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_SHORT));

	ret_ = pbs_sigjob(pbsCnHolder_->getFd(), (char *)jobId_.c_str(), (char *)CMD_SUSPEND, NULL);
	if(ret_ != 0) {
		throw InvalidStateException(SourceInfo(__func__,__LINE__),
				Message(INVALID_STATE_LONG));
	}
}

void PBSProSystem::resume(const Connection& connection_,
		const Job& job_) throw () {
	int ret_;
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&connection_);
	string jobId_;
	jobId_.assign((char*) &job_); //TODO: Needs to be changed after Job object implemented
	if (jobId_.empty())
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_SHORT));

	ret_ = pbs_sigjob(pbsCnHolder_->getFd(), (char *)jobId_.c_str(), (char *)CMD_RESUME, NULL);
	if(ret_ != 0) {
		throw InvalidStateException(SourceInfo(__func__,__LINE__),
				Message(INVALID_STATE_LONG));
	}
}

void PBSProSystem::release(const Connection& connection_,
		const Job& job_) throw () {
	int ret_;
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&connection_);
	string jobId_;
	jobId_.assign((char*) &job_); //TODO: Needs to be changed after Job object implemented
	if (jobId_.empty())
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_SHORT));
	ret_ = pbs_rlsjob(pbsCnHolder_->getFd(), (char*)jobId_.c_str(), NULL, NULL);
	if(ret_ != 0) {
		throw InvalidStateException(SourceInfo(__func__,__LINE__),
				Message(INVALID_STATE_LONG));
	}
}

void PBSProSystem::terminate(const Connection& connection_,
		const Job& job_) throw () {
	int ret_;
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&connection_);
	string jobId_;
	jobId_.assign((char*) &job_); //TODO: Needs to be changed after Job object implemented
	if (jobId_.empty())
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_SHORT));
	ret_ = pbs_deljob(pbsCnHolder_->getFd(), (char*)jobId_.c_str(), NULL);
	if(ret_ != 0) {
		throw InvalidStateException(SourceInfo(__func__,__LINE__),
				Message(INVALID_STATE_LONG));
	}
}

void PBSProSystem::reap(const Connection& connection_, const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

JobState PBSProSystem::state(const Connection& connection_,
		const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

Job* PBSProSystem::getJob(const Connection& connection_,
		const string& jobId_) throw () {
	//TODO Add Code here
	throw std::exception();
}

JobArray* PBSProSystem::runJobArray(const Connection& connection_,
		const JobTemplate& jobTemplate_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::hold(const Connection& connection_,
		const JobArray& jobArray_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::suspend(const Connection& connection_,
		const JobArray& jobArray_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::resume(const Connection& connection_,
		const JobArray& jobArray_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::release(const Connection& connection_,
		const JobArray& jobArray_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::terminate(const Connection& connection_,
		const JobArray& jobArray_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::reap(const Connection& connection_,
		const JobArray& jobArray_) throw () {
	//TODO Add Code here
	throw std::exception();
}

JobArray* PBSProSystem::getJobArray(const Connection& connection_,
		const string& jobArrayId_) throw () {
	//TODO Add Code here
	throw std::exception();
}

Reservation* PBSProSystem::submit(const Connection& connection_,
		const ReservationTemplate& reservationTemplate_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::remove(const Connection& connection_,
		const Reservation& reservation_) throw () {
	//TODO Add Code here
	throw std::exception();
}

ReservationList PBSProSystem::getAllReservations(
		const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

JobList PBSProSystem::getJobs(const Connection& connection_,
		const JobInfo& filter_) throw () {
	//TODO Add Code here
	throw std::exception();
}

MachineInfoList PBSProSystem::getAllMachines(
		const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

QueueInfoList PBSProSystem::getAllQueues
(const Connection& connection_) throw () {
	struct batch_status *batchRsp_ = (struct batch_status *) 0,
			*tmpBatchRsp_ = (struct batch_status *) 0;
	const PBSConnection *pbsCnHolder_ = static_cast<const PBSConnection*>(&connection_);
	QueueInfoList queueList_;

	batchRsp_ = pbs_statque(pbsCnHolder_->getFd(), NULL, NULL, NULL);
	tmpBatchRsp_ =  batchRsp_;
	while (tmpBatchRsp_ != NULL) {
		if (tmpBatchRsp_->name != NULL) {
			struct QueueInfo queueInfo_;
			queueInfo_.name.assign(tmpBatchRsp_->name);
			queueList_.push_back(queueInfo_);
		}
		tmpBatchRsp_ = tmpBatchRsp_->next;
	}
	if(batchRsp_)
		pbs_statfree(batchRsp_);
	return queueList_;
}

string PBSProSystem::getDRMSName(const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

Reservation* PBSProSystem::getReservation(const Connection& connection_,
		const string& reservationId_) throw () {
	//TODO Add Code here
	throw std::exception();
}

Version PBSProSystem::getDRMSVersion(const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

} /* namespace drmaa2 */
