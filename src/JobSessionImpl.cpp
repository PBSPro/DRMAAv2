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

#include <JobSessionImpl.h>
#include <ConnectionPool.h>
#include <PBSProSystem.h>
#include <PBSConnection.h>
#include <PBSJobArrayImpl.h>

namespace drmaa2 {

const JobList& JobSessionImpl::getJobs(const JobInfo& filter_) {
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	_jobList = drms->getJobs(pbsConnPoolObj_, filter_);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
	return _jobList;
}

const JobArray& JobSessionImpl::getJobArray(const string& jobArrayId_) {
	PBSJobArrayImpl *jobArrayImpl_ = new PBSJobArrayImpl(jobArrayId_);
	JobArray& jobArray_ = static_cast<JobArray&>(*jobArrayImpl_);
	return jobArray_;
}

Job& JobSessionImpl::runJob(const JobTemplate& jobTemplate_) const {
	Job *job_;
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	job_ = (Job *)drms->runJob(pbsConnPoolObj_, jobTemplate_);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
	return *job_;
}

JobArray& JobSessionImpl::runBulkJobs(const JobTemplate& jobTemplate_,
		const long beginIndex_, const long endIndex_, const long step_,
		const long maxParallel_) const {
	JobArray *jobArray_;
	PBSConnection pbsconn_(pbs_default(), 0, 0);
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	jobArray_ = (JobArray *)drms->runJobArray(pbsConnPoolObj_, jobTemplate_,
			beginIndex_, endIndex_, step_, maxParallel_);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
	return *jobArray_;
}

const Job& JobSessionImpl::waitAnyStarted(const JobList& jobs_,
		const TimeAmount timeout_) {
}

const Job& JobSessionImpl::waitAnyTerminated(const JobList& jobs_,
		const TimeAmount timeout_) {
}

}
