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

#ifndef INC_JOBSESSIONIMPL_H_
#define INC_JOBSESSIONIMPL_H_

#include <list>
#include <string>

#include <drmaa2.hpp>
#include <ConnectionPool.h>
#include <PBSIFLExtend.h>

using namespace std;

namespace drmaa2 {

class JobSessionImpl : public JobSession {
	list<string> _sessionJobs;
	JobList _jobList;
public:
	/**
	 * @brief Parameterized Constructor
	 *
	 * @param contact_ - DRMS contact name
	 * @param sessionName_ - Session Name
	 * @param jobCategories_ - Job Categories
	 *
	 */
	JobSessionImpl(const string& sessionName_,
			const StringList& jobCategories_, const string& contact_ = string(pbs_default())) :
				JobSession(sessionName_, jobCategories_, contact_) {

	}

	JobSessionImpl& operator=(const JobSessionImpl& obj_) {
		return *this;
	}
	/**
	 * Destructor
	 */
	virtual ~JobSessionImpl(void) {
	}

	/**
	 * @brief Returns associated jobs
	 *
	 * @param[in] filter_ - Filter criteria
	 *
	 * @return JobList
	 */
	virtual const JobList& getJobs(const JobInfo& filter_);

	/**
	 * @brief Returns JobArray
	 *
	 * @param[in] jobArrayId_ - JobArray to be searched in the list
	 *
	 * @throw InvalidArgumentException - If the ID is not found
	 *
	 * @return JobArray
	 */
	virtual const JobArray& getJobArray(const string& jobArrayId_);

	/**
	 * @brief Submit Job to DRMS
	 *
	 * @param[in] jobTemplate_ - Detailed job information
	 *
	 * @throw InvalidArgumentException - If DRMS fails to accept JobTemplate
	 *
	 * @return valid job id returned from the DRMS
	 */
	virtual Job& runJob(const JobTemplate& jobTemplate_) const;

	/**
	 * @brief Submit JobArray to DRMS
	 *
	 * @param[in] jobTemplate_ - Detailed job information
	 * @param[in] beginIndex_ - start index of JobArray
	 * @param[in] endIndex_ - end index of JobArray
	 * @param[in] step_ - Incremental count for index
	 * @param[in] maxParallel_ - Number of jobs to run in parallel
	 *
	 * @throw InvalidArgumentException - If DRMS fails to accept JobTemplate
	 *
	 * @return JobArray
	 */
	virtual JobArray& runBulkJobs(const JobTemplate& jobTemplate_,
			const long beginIndex_, const long endIndex_, const long step_,
			const long maxParallel_) const;

	/**
	 * @brief In a list of specified job ids waits until
	 * 			any of the job is started
	 *
	 * @param[in] jobs_ - List of Jobs
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw InvalidArgumentException - Invalid list of jobs
	 * @throw TimeoutException - Fails to start in a given duration
	 *
	 * @return Job
	 */
	virtual const Job& waitAnyStarted(const JobList& jobs_,
			const TimeAmount timeout_);

	/**
	 * @brief In a list of specified job ids waits until
	 * 			any of the job is terminated
	 *
	 * @param[in] jobs_ - List of Jobs
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw InvalidArgumentException - Invalid list of jobs
	 * @throw TimeoutException - Fails to terminate in a given duration
	 *
	 * @return Job
	 */
	virtual const Job& waitAnyTerminated(const JobList& jobs_,
			const TimeAmount timeout_);
};

}

#endif /* INC_JOBSESSIONIMPL_H_ */
