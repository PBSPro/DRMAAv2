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

#include <JobTemplateAttrHelper.h>
#include <Message.h>
#include <PBSConnection.h>
#include <PBSIFLExtend.h>
#include <PBSJobImpl.h>
#include <PBSProSystem.h>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <list>

namespace drmaa2 {
pthread_mutex_t PBSProSystem::_posixMutex = PTHREAD_MUTEX_INITIALIZER;

PBSProSystem::PBSProSystem() {
	// TODO Auto-generated constructor stub

}

PBSProSystem::~PBSProSystem() {
	// TODO Auto-generated destructor stub
}

void PBSProSystem::connect(Connection & connection_)
		throw (ImplementationSpecificException) {
	PBSConnection *pbsCnHolder_ = dynamic_cast<PBSConnection*>(&connection_);
	int fd_ = pbs_connect((char *) pbsCnHolder_->getServerName().c_str());
	if (fd_ < 0) {
		throw ImplementationSpecificException(pbs_errno, DRMAA2_SOURCEINFO());
	} else {
		pbsCnHolder_->setFd(fd_);
	}
}

void PBSProSystem::disconnect(const Connection & connection_)
		throw (ImplementationSpecificException) {
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	int ret_ = pbs_disconnect(pbsCnHolder_->getFd());
	if (ret_ < 0) {
		throw ImplementationSpecificException(pbs_errno, DRMAA2_SOURCEINFO());
	}
}

Job* PBSProSystem::runJob(const Connection& connection_,
		const JobTemplate& jobTemplate_) throw (ImplementationSpecificException) {
	string destination_;
	char *jobIdFromDRMS_;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	JobTemplateAttrHelper _attrParse;

	if (!jobTemplate_.reservationId.empty())
		destination_.append(jobTemplate_.queueName);
	else if (!jobTemplate_.queueName.empty())
		destination_.append(jobTemplate_.queueName);

	ATTRL *attributeList = _attrParse.parseTemplate((void*) &jobTemplate_);
	jobIdFromDRMS_ = pbs_submit(pbsCnHolder_->getFd(),
			(struct attropl *) attributeList,
			(char*) jobTemplate_.remoteCommand.c_str(),
			(char*) destination_.c_str(), NULL);
	if (jobIdFromDRMS_) {
		string jobId_(jobIdFromDRMS_);
		return new PBSJobImpl(jobId_, jobTemplate_);
	} else {
		throw ImplementationSpecificException(pbs_errno,
		DRMAA2_SOURCEINFO());
	}
}
void PBSProSystem::checkForPBS_ErrorException() throw (InvalidStateException,
		ImplementationSpecificException, DeniedByDrmsException) {

	switch (pbs_errno) {
		case PBSE_BADSTATE:
		case PBSE_MODATRRUN:
		case PBSE_ALRDYEXIT:
		case PBSE_MUTUALEX:
			throw InvalidStateException(DRMAA2_SOURCEINFO());
		case PBSE_ATTRRO:
		case PBSE_BADHOST:
		case PBSE_PERM:
		case PBSE_QACESS:
		case PBSE_BADSCRIPT:
		case PBSE_BADCRED:
		case PBSE_BADUSER:
		case PBSE_EXPIRED:
			throw DeniedByDrmsException(DRMAA2_SOURCEINFO());
		case PBSE_NOATTR:
		default:
			throw ImplementationSpecificException(pbs_errno,
				DRMAA2_SOURCEINFO());
	}
}
void PBSProSystem::hold(const Connection& connection_, const Job& job_)
		throw (InvalidStateException, DeniedByDrmsException,
		ImplementationSpecificException) {
	int ret_;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	ret_ = pbs_holdjob(pbsCnHolder_->getFd(), (char*) job_.getJobId().c_str(),
	NULL, NULL);
	if (ret_ != 0) {
		checkForPBS_ErrorException();
	}
}

void PBSProSystem::suspend(const Connection& connection_, const Job& job_)
		throw (InvalidStateException, DeniedByDrmsException,
		ImplementationSpecificException) {
	int ret_;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	ret_ = pbs_sigjob(pbsCnHolder_->getFd(), (char *) job_.getJobId().c_str(),
			(char *) CMD_SUSPEND, NULL);
	if (ret_ != 0) {
		checkForPBS_ErrorException();
	}
}

void PBSProSystem::resume(const Connection& connection_, const Job& job_)
		throw (InvalidStateException, DeniedByDrmsException,
		ImplementationSpecificException) {
	int ret_;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	ret_ = pbs_sigjob(pbsCnHolder_->getFd(), (char *) job_.getJobId().c_str(),
			(char *) CMD_RESUME, NULL);
	if (ret_ != 0) {
		checkForPBS_ErrorException();
	}
}

void PBSProSystem::release(const Connection& connection_, const Job& job_)
		throw (InvalidStateException, DeniedByDrmsException,
		ImplementationSpecificException) {
	int ret_;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	ret_ = pbs_rlsjob(pbsCnHolder_->getFd(), (char*) job_.getJobId().c_str(),
	NULL, NULL);
	if (ret_ != 0) {
		checkForPBS_ErrorException();
	}
}

void PBSProSystem::terminate(const Connection& connection_, const Job& job_)
		throw (InvalidStateException, DeniedByDrmsException,
		ImplementationSpecificException) {
	int ret_;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	ret_ = pbs_deljob(pbsCnHolder_->getFd(), (char*) job_.getJobId().c_str(),
	NULL);
	if (ret_ != 0) {
		checkForPBS_ErrorException();
	}
}

void PBSProSystem::reap(const Connection& connection_, const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

JobState PBSProSystem::state(const Connection& connection_,
		const Job& job_) throw () {
	char *runCount_;
	JobTemplateAttrHelper attrParse_;
	JobState jobState_;
	struct batch_status *batchResponse_ = NULL;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	attrParse_.setAttribute((char *) ATTR_state, (char *) "");
	attrParse_.setAttribute((char *) ATTR_runcount, (char *) "");
	ATTRL *attributeList_ = attrParse_.getAttributeList();
	batchResponse_ = pbs_statjob(pbsCnHolder_->getFd(),
			(char *) job_.getJobId().c_str(), attributeList_, (char *) "x");
	if (batchResponse_) {
		switch (batchResponse_->attribs->value[0]) {
		case 'R':
			jobState_ = RUNNING;
			break;
		case 'Q':
			jobState_ = QUEUED;
			break;
		case 'S':
			jobState_ = SUSPENDED;
			break;
		case 'H':
			jobState_ = QUEUED_HELD;
			break;
		case 'F':
			jobState_ = DONE;
			break;
		default:
			jobState_ = UNDETERMINED;
			break;
		}
		if (jobState_ == QUEUED || jobState_ == QUEUED_HELD) {
			if (batchResponse_->attribs->next) {
				runCount_ = batchResponse_->attribs->next->value;
				if (runCount_) {
					if (atol(runCount_) > 0) {
						if (jobState_ == QUEUED)
							jobState_ = REQUEUED;
						else if (jobState_ == QUEUED_HELD)
							jobState_ = REQUEUED_HELD;
					}
				}
			}
		}
		pbs_statfree(batchResponse_);
	}
	return jobState_;
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
	JobList _jList;
	list<string> _allJobs;
	struct batch_status *batchRsp_ = (struct batch_status *) 0, *tmpBatchRsp_ =
			(struct batch_status *) 0;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	batchRsp_ = pbs_statjob(pbsCnHolder_->getFd(), NULL, NULL, (char *) "x");
	tmpBatchRsp_ = batchRsp_;
	while (tmpBatchRsp_) {
		if (tmpBatchRsp_->name != NULL) {
			_allJobs.push_back(string(tmpBatchRsp_->name));
		}
		tmpBatchRsp_ = tmpBatchRsp_->next;
	}
	for (list<string>::iterator iterator = _allJobs.begin();
			iterator != _allJobs.end(); ++iterator) {
		PBSJobImpl *_job = new PBSJobImpl(*iterator);
		JobInfo _jInfo = _job->getJobInfo();
		if (!filter_.jobId.empty() && _jInfo.jobId != filter_.jobId)
			continue;
		if (!filter_.annotation.empty()
				&& _jInfo.annotation != filter_.annotation)
			continue;
		if (_jInfo.exitStatus != filter_.exitStatus)
			continue;
		if (_jInfo.jobState != filter_.jobState)
			continue;
		if (!filter_.annotation.empty()
				&& _jInfo.submissionMachine != filter_.submissionMachine)
			continue;
		if (_jInfo.cpuTime != filter_.cpuTime)
			continue;
		if (difftime(_jInfo.wallclockTime, filter_.wallclockTime) > 0)
			continue;
		if (difftime(_jInfo.submissionTime, filter_.submissionTime) > 0)
			continue;
		if (difftime(_jInfo.dispatchTime, filter_.dispatchTime) > 0)
			continue;
		if (difftime(_jInfo.finishTime, filter_.finishTime) > 0)
			continue;
		_jList.push_back(_job);
	}
	if (batchRsp_)
		pbs_statfree(batchRsp_);
	return _jList;
}

MachineInfoList PBSProSystem::getAllMachines(
		const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

QueueInfoList PBSProSystem::getAllQueues(const Connection& connection_) throw () {
	struct batch_status *batchRsp_ = (struct batch_status *) 0, *tmpBatchRsp_ =
			(struct batch_status *) 0;
	const PBSConnection *pbsCnHolder_ =
			dynamic_cast<const PBSConnection*>(&connection_);
	QueueInfoList queueList_;

	batchRsp_ = pbs_statque(pbsCnHolder_->getFd(), NULL, NULL, NULL);
	tmpBatchRsp_ = batchRsp_;
	while (tmpBatchRsp_ != NULL) {
		if (tmpBatchRsp_->name != NULL) {
			struct QueueInfo queueInfo_;
			queueInfo_.name.assign(tmpBatchRsp_->name);
			queueList_.push_back(queueInfo_);
		}
		tmpBatchRsp_ = tmpBatchRsp_->next;
	}
	if (batchRsp_)
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
