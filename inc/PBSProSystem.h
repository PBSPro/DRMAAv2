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

#ifndef INC_PBSPROSYSTEM_H_
#define INC_PBSPROSYSTEM_H_

#include "DRMSystem.h"

namespace drmaa2 {

/**
 * @class PBSProSystem
 * @brief Concrete class of DRMSystem
 *
 */
class PBSProSystem: public DRMSystem {
	/**
	 * @brief Singleton is template for singleton design pattern
	 * 		friend template Singleton helps to access private constructor
	 * 		of PBSProSystem.
	 *
	 * 		Usage :
	 * 			drmaa2::DRMSystem singleTon = drmaa2::Singleton<drmaa2::DRMSystem, drmaa2::PBSProSystem>::getInstance();
	 *			singleTon.connect(...)
	 *
	 */
	friend Singleton<DRMSystem, PBSProSystem> ;
private:
	static pthread_mutex_t _posixMutex;

	/**
	 * @brief Default Constructor
	 */
	PBSProSystem();

	/**
	 * @brief Copy constructor is private so that
	 * 			instance cannot be created outside class
	 * 	@param obj_ - PBSProSystem object to be copied
	 *
	 */
	PBSProSystem(PBSProSystem const& obj_);
	/**
	 * @brief Assignment operator is overloaded and is private
	 * 			so that instance cannot be created outside class
	 * 	@param obj_ - PBSProSystem object to be copied
	 *
	 */
	PBSProSystem& operator=(PBSProSystem const& obj_);
public:
	/**
	 * @brief Default Destructor
	 */
	virtual ~PBSProSystem();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void connect(Connection& connection_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void disconnect(const Connection& connection_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual Job* runJob(const Connection & connection_,
			const JobTemplate& jobTemplate_) throw ();

	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void hold(const Connection & connection_, const Job& job_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void suspend(const Connection & connection_,
			const Job& job_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void resume(const Connection & connection_,
			const Job& job_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void release(const Connection & connection_,
			const Job& job_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void terminate(const Connection & connection_,
			const Job& job_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void reap(const Connection & connection_, const Job& job_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual JobState state(const Connection & connection_,
			const Job& job_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual Job* getJob(const Connection & connection_,
			const string& jobId_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual JobArray* runJobArray(const Connection & connection_,
			const JobTemplate& jobTemplate_) throw ();

	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void hold(const Connection & connection_,
			const JobArray& jobArray_) throw ();

	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void suspend(const Connection & connection_,
			const JobArray& jobArray_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void resume(const Connection & connection_,
			const JobArray& jobArray_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void release(const Connection & connection_,
			const JobArray& jobArray_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void terminate(const Connection & connection_,
			const JobArray& jobArray_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void reap(const Connection & connection_,
			const JobArray& jobArray_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual JobArray* getJobArray(const Connection & connection_,
			const string& jobArrayId_) throw ();

	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual Reservation* submit(const Connection & connection_,
			const ReservationTemplate& reservationTemplate_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual void remove(const Connection & connection_,
			const Reservation& reservation_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual ReservationList getAllReservations(
			const Connection & connection_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual Reservation* getReservation(const Connection & connection_,
				const string& reservationId_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual JobList getJobs(const Connection & connection_,
			const JobInfo& filter_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual MachineInfoList getAllMachines(
			const Connection & connection_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual QueueInfoList getAllQueues(
			const Connection & connection_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual string getDRMSName(const Connection & connection_) throw ();
	/**
	 * @brief overridden method from DRMSystem
	 */
	virtual Version getDRMSVersion(const Connection & connection_) throw ();

};

} /* namespace drmaa2 */

#endif /* INC_PBSPROSYSTEM_H_ */
