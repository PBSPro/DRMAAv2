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

#ifndef SRC_SESSIONMANAGERIMPL_H_
#define SRC_SESSIONMANAGERIMPL_H_

#include <drmaa2.hpp>
#include <pthread.h>
#include <string>
#include <JobSessionImpl.h>
#include <ReservationSessionImpl.h>
#include <MonitoringSessionImpl.h>

#include "drmaa2.hpp"

using namespace std;

namespace drmaa2 {
/**
 * @class SessionManagerImpl
 * @brief Concrete class of SessionManager
 *
 */
class SessionManagerImpl: public SessionManager {
	/**
	 * @brief Singleton is template for singleton design pattern
	 * 		friend template Singleton helps to access private constructor
	 * 		of SessionManagerImpl.
	 *
	 */
	friend Singleton<SessionManager, SessionManagerImpl> ;
private:
	map<string, JobSessionImpl> _jobSessionMap;
	map<string, ReservationSessionImpl> _reservationSessionMap;
	MonitoringSessionImpl _mSession;
	static pthread_mutex_t _posixMutex;

	/**
	 * @brief default constructor
	 */
	SessionManagerImpl() {
	}

	/**
	 * @brief Copy constructor is private so that
	 * 			instance cannot be created outside class
	 * 	@param obj_ - SessionManagerImpl object to be copied
	 *
	 */
	SessionManagerImpl(SessionManagerImpl const& obj_);
	/**
	 * @brief Assignment operator is overloaded and is private
	 * 			so that instance cannot be created outside class
	 * 	@param obj_ - SessionManagerImpl object to be copied
	 *
	 */
	SessionManagerImpl& operator=(SessionManagerImpl const& obj_);
public:
	/**
	 * @brief default destructor
	 */
	virtual ~SessionManagerImpl();
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual bool supports(const DrmaaCapability& capability_);

	/**
	 * @brief overridden method from SessionManager
	 */
	virtual const JobSession& createJobSession(const string& sessionName_,
			const string& contact_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual const JobSession& openJobSession(const string& sessionName_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual void closeJobSession(JobSession& session_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual void destroyJobSession(const string& sessionName_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual const ReservationSession& createReservationSession(
			const string& sessionName_, const string& contact_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual const ReservationSession& openReservationSession(
			const string& sessionName_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual void closeReservationSession(ReservationSession& session_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual void destroyReservationSession(const string& sessionName_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual const MonitoringSession& openMonitoringSession(
			const string& contact_);

	/**
	 * @brief overridden method from SessionManager
	 */
	virtual void closeMonitoringSession(const MonitoringSession& session_);
	/**
	 * @brief overridden method from SessionManager
	 */
	virtual const StringList& getReservationSessionNames(void);

	/**
	 * @brief overridden method from SessionManager
	 */
	virtual const StringList& getJobSessionNames(void);

	/**
	 * @brief overridden method from SessionManager
	 */
	virtual void registerEventNotification(const DrmaaCallback& callback);

	/**
	 * @brief overridden method from SessionManager
	 *
	 */
	virtual void initialize();

};

} /* namespace drmaa2 */

#endif /* SRC_SESSIONMANAGERIMPL_H_ */
