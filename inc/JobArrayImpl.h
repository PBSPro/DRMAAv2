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

#ifndef INC_JOBARRAYIMPL_H_
#define INC_JOBARRAYIMPL_H_

#include <string>

#include "drmaa2.hpp"

using namespace std;

namespace drmaa2 {
/**
 * @class JobArrayImpl
 * @brief Concrete class for JobArray
 *
 */
class JobArrayImpl : public JobArray {
	const string _jobId;
	JobTemplate _jt;
	JobList _jobList;
	/**
	 * Constructor
	 */
	JobArrayImpl() {};
	/**
	 * Copy constructor
	 */
	JobArrayImpl(const JobArrayImpl &jobImpl_) {};
public:
	/**
	 * Parameterized constructor
	 */
	JobArrayImpl(const string& jobId_):_jobId(jobId_) {
	}
	/**
	 * Parameterized constructor
	 */
	JobArrayImpl(const string& jobId_, const JobTemplate& jt_):_jobId(jobId_), _jt(jt_) {
	};
	/**
	 * Destructor
	 */
	virtual ~JobArrayImpl(void);

	/**
	 * @brief Returns Job array ID
	 *
	 * @param - None
	 *
	 * @return valid job array ID
	 */
	virtual const string& getJobArrayId(void) const;

	/**
	 * @brief Returns List of jobs in JobArray
	 *
	 * @param - None
	 *
	 * @return List of Job
	 */
	virtual JobList& getJobs(void);

	/**
	 * @brief Returns JobTemplate from which JobArray is submitted
	 *
	 * @param - None
	 *
	 * @return JobTemlpate
	 */
	virtual const JobTemplate& getJobTemplate(void) const;

	/**
	 * @brief Suspends a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void suspend(void) const;

	/**
	 * @brief resumes a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void resume(void) const;

	/**
	 * @brief Holds a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void hold(void) const;

	/**
	 * @brief Releases a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void release(void) const;

	/**
	 * @brief Terminates a JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidStateException - If the JobArray is in an inappropriate state
	 *
	 * @return None
	 */
	virtual void terminate(void) const;

	/**
	 * @brief Clean up any data about this JobArray
	 *
	 * @param - None
	 *
	 * @throw InvalidArgumentException - Reaped JobArray in any subsequent activity
	 * 									throws an InvalidArgumentException for
	 * 									the JobArray parameter
	 *
	 * @return None
	 */
	virtual void reap(void) const;
};

} /* namespace drmaa2 */

#endif /* INC_JOBARRAYIMPL_H_ */
