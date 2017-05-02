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

#ifndef INC_DRMSYSTEM_H_
#define INC_DRMSYSTEM_H_

#include <drmaa2.hpp>
#include <DeniedByDrmsException.h>
#include <ImplementationSpecificException.h>
#include <InvalidStateException.h>
#include <string>
#include <Connection.h>

namespace drmaa2 {

/**
 * @class DRMSystem
 * @brief An interface to DRMS system. Defines DRMS functionality
 * 		All DRMS calls goes through this interface
 */
class DRMSystem {

public:
	/**
	 * @brief Constructor
	 */
	DRMSystem();
	/**
	 * Destructor
	 */
	virtual ~DRMSystem();

	/**
	 * @brief Makes connection with DRMS
	 *
	 * @param[in] connection_ - connection object
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 										connecting to DRMS
	 * @throw DeniedByDrmsException - Connection refused by DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - None
	 *
	 */
	virtual void connect(Connection& connection_)
			throw (ImplementationSpecificException) = 0;

	/**
	 * @brief Disconnects from DRMS
	 *
	 * @param[in] connection_ - connection object
	 *
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - None
	 *
	 */
	virtual void disconnect(const Connection& connection_)
			throw (ImplementationSpecificException) = 0;

	/**
	 * @brief runs a job
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobTemplate_ - JobTemplate
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @warning Application has to handle Job memory deallocation
	 *
	 * @return Job - Job object
	 *
	 */
	virtual Job* runJob(const Connection & connection_,
			const JobTemplate& jobTemplate_)
					throw (ImplementationSpecificException) = 0;

	/**
	 * @brief Triggers a transition from QUEUED to QUEUED_HELD, or from
	 * 			REQUEUED to REQUEUED_HELD state.
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] job_ - Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw DeniedByDrmsException - Denied by DRMS
	 *
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void hold(const Connection & connection_, const Job& job_)
			throw (InvalidStateException, DeniedByDrmsException,
			ImplementationSpecificException) = 0;

	/**
	 * @brief Triggers a transition from RUNNING to SUSPENDED state.
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] job_ - Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw DeniedByDrmsException - Denied by DRMS
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void suspend(const Connection & connection_, const Job& job_)
			throw (InvalidStateException, DeniedByDrmsException,
			ImplementationSpecificException) = 0;

	/**
	 * @brief Triggers a transition from SUSPENDED to RUNNING state
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] job_ - Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw DeniedByDrmsException - Denied by DRMS
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void resume(const Connection & connection_, const Job& job_)
			throw (InvalidStateException, DeniedByDrmsException,
			ImplementationSpecificException) = 0;

	/**
	 * @brief Triggers a transition from QUEUED_HELD to QUEUED, or from
	 * 			REQUEUED_HELD to REQUEUED state.
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] job_ - Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw DeniedByDrmsException - Denied by DRMS
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void release(const Connection & connection_, const Job& job_)
			throw (InvalidStateException, DeniedByDrmsException,
			ImplementationSpecificException) = 0;

	/**
	 * @brief Deletes a job from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] job_ - Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw DeniedByDrmsException - Denied by DRMS
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void terminate(const Connection & connection_, const Job& job_)
			throw (InvalidStateException, DeniedByDrmsException,
			ImplementationSpecificException) = 0;

	/**
	 * @brief Job clean up in DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] job_ - Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - None
	 *
	 */
	virtual void reap(const Connection & connection_, const Job& job_) throw () = 0;

	/**
	 * @brief get Job state from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] job_ - Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - JobState
	 *
	 */
	virtual JobState state(const Connection & connection_,
			const Job& job_) throw () = 0;

	/**
	 * @brief get Job from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobId_ - Unique id to identify Job
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - Job
	 *
	 */

	virtual Job* getJob(const Connection & connection_,
			const string& jobId_) throw () = 0;

	/**
	 * @brief runs a JobArray
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobTemplate_ - JobTemplate
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @warning application has to handle Job Array memory 
	 *
	 * @return JobArray - JobArray object
	 *
	 */
	virtual JobArray* runJobArray(const Connection & connection_,
			const JobTemplate& jobTemplate_, const long beginIndex_,
			const long endIndex_, const long step_,
			const long maxParallel_) throw () = 0;

	/**
	 * @brief Triggers a transition from QUEUED to QUEUED_HELD, or from
	 * 			REQUEUED to REQUEUED_HELD state.
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobArray_ - JobArray
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void hold(const Connection & connection_,
			const JobArray& jobArray_) throw () = 0;

	/**
	 * @brief Triggers a transition from RUNNING to SUSPENDED state.
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobArray_ - JobArray
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void suspend(const Connection & connection_,
			const JobArray& jobArray_) throw () = 0;

	/**
	 * @brief Triggers a transition from SUSPENDED to RUNNING state
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobArray_ - JobArray
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw k - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void resume(const Connection & connection_,
			const JobArray& jobArray_) throw () = 0;

	/**
	 * @brief Triggers a transition from QUEUED_HELD to QUEUED, or from
	 * 			REQUEUED_HELD to REQUEUED state.
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobArray_ - JobArray
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 * @throw InvalidStateException - State is invalid to perform operation
	 *
	 * @return - None
	 *
	 */
	virtual void release(const Connection & connection_,
			const JobArray& jobArray_) throw () = 0;

	/**
	 * @brief Deletes a JobArray from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobArray_ - JobArray
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - None
	 *
	 */
	virtual void terminate(const Connection & connection_,
			const JobArray& jobArray_) throw () = 0;

	/**
	 * @brief JobArray clean up in DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobArray_ - JobArray
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - None
	 *
	 */
	virtual void reap(const Connection & connection_,
			const JobArray& jobArray_) throw () = 0;

	/**
	 * @brief get JobArray from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] jobArrayId_ - Unique id to identify JobArray
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - JobArray
	 *
	 */
	virtual JobArray* getJobArray(const Connection & connection_,
			const string& jobArrayId_) throw () = 0;

	/**
	 * @brief Submit reservation to DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] reservationTemplate_ - ReservationTemplate
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - Reservation
	 *
	 */
	virtual Reservation* submit(const Connection & connection_,
			const ReservationTemplate& reservationTemplate_) throw () = 0;

	/**
	 * @brief deletes reservation from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] reservation_ - Reservation
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - None
	 *
	 */
	virtual void remove(const Connection & connection_,
			const Reservation& reservation_) throw () = 0;

	/**
	 * @brief Gets all reservation from DRMS
	 *
	 * @param[in] connection_ - connection object
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - ReservationList
	 *
	 */
	virtual ReservationList getAllReservations(
			const Connection & connection_) throw () = 0;

	/**
	 * @brief Get reservation from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] reservationId_ - Unique id to identify Reservation
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - Reservation
	 *
	 */
	virtual Reservation* getReservation(const Connection & connection_,
			const string& reservationId_) throw () = 0;

	/**
	 * @brief Gets all Jobs from DRMS
	 *
	 * @param[in] connection_ - connection object
	 * @param[in] filter_ - JobInfo
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - JobList
	 *
	 */
	virtual JobList getJobs(const Connection & connection_,
			const JobInfo& filter_) throw () = 0;

	/**
	 * @brief Gets all machine info from DRMS
	 *
	 * @param[in] connection_ - connection object
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - MachineInfoList
	 *
	 */
	virtual MachineInfoList getAllMachines(
			const Connection & connection_) throw () = 0;

	/**
	 * @brief Gets all queue info from DRMS
	 *
	 * @param[in] connection_ - connection object
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - QueueInfoList
	 *
	 */
	virtual QueueInfoList getAllQueues(const Connection & connection_) throw () = 0;

	/**
	 * @brief Gets DRMS Name
	 *
	 * @param[in] connection_ - connection object
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - DRMS Name
	 *
	 */
	virtual string getDRMSName(const Connection & connection_) throw () = 0;

	/**
	 * @brief Gets DRMS Version
	 *
	 * @param[in] connection_ - connection object
	 *
	 * @throw DrmCommunicationException - Communication errors while
	 * 									sending/receiving data from/to DRMS
	 * @throw ImplementationSpecificException - Any implementation specific
	 * 											errors
	 *
	 * @return - DRMS Version
	 *
	 */
	virtual Version getDRMSVersion(const Connection & connection_) throw () = 0;

};

} /* namespace drmaa2 */

#endif /* INC_DRMSYSTEM_H_ */
