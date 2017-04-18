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

#ifndef SRC_PBSJOBIMPL_H_
#define SRC_PBSJOBIMPL_H_

#include <drmaa2.hpp>
#include <string>

using namespace std;

namespace drmaa2 {
/**
 * @class PBSJobImpl
 * @brief Concrete class of Job
 *
 */
class PBSJobImpl: public Job {
private:
	const string _jobId;
	JobTemplate _jt;
	JobState _jobState;
	mutable JobInfo _jobInfo;
	/**
	 * Constructor
	 */
	PBSJobImpl() {
		_jobState = UNDETERMINED;
	};
	/**
	 * Copy constructor
	 */
	PBSJobImpl(const PBSJobImpl &jobImpl_) {
		_jobState = UNDETERMINED;
	};
public:
	/**
	 * Parameterized constructor
	 */
	PBSJobImpl(const string& jobId_):_jobId(jobId_) {
		_jobState = UNDETERMINED;
	}
	/**
	 * Parameterized constructor
	 */
	PBSJobImpl(const string& jobId_, const JobTemplate& jt_):_jobId(jobId_), _jt(jt_) {
		_jobState = UNDETERMINED;
	};
	/**
	 * Destructor
	 */
	virtual ~PBSJobImpl(void);

	/**
	 * @brief Returns Job ID
	 *
	 * @param - None
	 *
	 * @return valid job ID
	 */
	virtual const string& getJobId(void) const;

	/**
	 * @brief Returns detailed Job information
	 *
	 * @param - None
	 *
	 * @return JobInfo
	 */
	virtual const JobInfo& getJobInfo(void) const;

	/**
	 * @brief Populates Job information
	 *
	 * @param - None
	 *
	 * @return None
	 */
	const void populateJobInfo(void) const;

	/**
	 * @brief Returns Job State
	 *
	 * @param[out] subState - Sub state of job is set
	 *
	 * @return JobState
	 */
	virtual const JobState& getState(string& subState);

	/**
	 * @brief Returns JobTemplate from which Job is submitted
	 *
	 * @param - None
	 *
	 * @return JobTemlpate
	 */
	virtual const JobTemplate& getJobTemplate(void) const;

	/**
	 * @brief Suspends a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void suspend(void) throw();

	/**
	 * @brief resumes a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void resume(void) throw();

	/**
	 * @brief Holds a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void hold(void) throw();

	/**
	 * @brief Releases a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return
	 */
	virtual void release(void) throw();

	/**
	 * @brief Terminates a Job
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the Job is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void terminate(void) throw();

	/**
	 * @brief Clean up any data about this job
	 *
	 * @param - None
	 *
	 * @throw InvalidArgumentException - Reaped job in any subsequent activity
	 * 									throws an InvalidArgumentException for
	 * 									the job parameter
	 *
	 * @return None
	 */
	virtual void reap(void) throw();

	/**
	 * @brief Blocking call to wait until job starts
	 *
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw TimeoutException - Fails to start in a given duration
	 *
	 * @return None
	 */
	virtual void waitStarted(const TimeAmount& timeout_) throw();

	/**
	 * @brief Blocking call to wait until job ends
	 *
	 * @param[in] timeout_ - Indicates call blocking duration
	 *
	 * @throw TimeoutException - Fails to terminate in a given duration
	 *
	 * @return None
	 */
	virtual void waitTerminated(TimeAmount& timeout_) throw();
};

} /* namespace drmaa2 */

#endif /* SRC_PBSJOBIMPL_H_ */
