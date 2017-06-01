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
#include <InternalException.h>
#include <Message.h>
#include <pbs_ifl.h>
#include <pthread.h>
#include <PBSConnection.h>
#include <SessionManagerImpl.h>
#include <SourceInfo.h>
#include <exception>
#include <sstream>
#include <Message.h>
#include <SourceInfo.h>
#include <InvalidArgumentException.h>

namespace drmaa2 {
pthread_mutex_t SessionManagerImpl::_posixMutex = PTHREAD_MUTEX_INITIALIZER;

SessionManagerImpl::~SessionManagerImpl() {
	// TODO Auto-generated destructor stub
}

bool SessionManagerImpl::supports(const DrmaaCapability& capability_) {
	//TODO Add Code here
	return true;
}

const JobSession& SessionManagerImpl::createJobSession(
		const string& sessionName_, const string& contact_) {
	if (sessionName_.empty()) {
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_LONG));
	}

	if (_jobSessionMap.find(sessionName_) == _jobSessionMap.end()) {
		StringList jobCategories_;
		string jobSessionContact_;
		if (contact_.empty()) {
			char *tmp = NULL;
			tmp = pbs_default();
			if(tmp != NULL) {
				jobSessionContact_.assign(tmp);
			}
		} else {
			jobSessionContact_.assign(contact_);
		}
		JobSessionImpl jobSession_(sessionName_, jobCategories_, contact_);
		_jobSessionMap.insert(std::pair<string,JobSessionImpl>(sessionName_,jobSession_));
	}
	JobSessionImpl &jobSessionImplObj_ = _jobSessionMap.find(sessionName_)->second;
	JobSession& newJobSession_ = static_cast<JobSession&>(jobSessionImplObj_);
	return newJobSession_;
}

const JobSession& SessionManagerImpl::openJobSession(
		const string& sessionName_) {
	//TODO Add Code here
	throw std::exception();
}

void SessionManagerImpl::closeJobSession(JobSession& session_) {
}

void SessionManagerImpl::destroyJobSession(const string& sessionName_) {
	if (sessionName_.empty()) {
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_LONG));
	}
	if (_jobSessionMap.erase(sessionName_) == 0) {
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_LONG));
	}
}

const ReservationSession& SessionManagerImpl::createReservationSession(
		const string& sessionName_, const string& contact_) {
	if (sessionName_.empty()) {
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_LONG));
	}

	if (_reservationSessionMap.find(sessionName_) == _reservationSessionMap.end()) {
		string reservationSessionContact_;
		ReservationSessionImpl reservationSession_(sessionName_, contact_);
		_reservationSessionMap.insert(std::pair<string, ReservationSessionImpl>(sessionName_,reservationSession_));
	}
	ReservationSessionImpl &reservationSessionImplObj_ = _reservationSessionMap.find(sessionName_)->second;
	ReservationSession& newReservationSession_ = static_cast<ReservationSession&>(reservationSessionImplObj_);
	return newReservationSession_;
}

const ReservationSession& SessionManagerImpl::openReservationSession(
		const string& sessionName_) {
	//TODO Add Code here
	throw std::exception();
}

void SessionManagerImpl::closeReservationSession(ReservationSession& session_) {
	//TODO Add Code here
}

void SessionManagerImpl::destroyReservationSession(const string& sessionName_) {
	if (sessionName_.empty()) {
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_LONG));
	}
	if (_reservationSessionMap.erase(sessionName_) == 0) {
		throw InvalidArgumentException(SourceInfo(__func__,__LINE__),
				Message(INVALID_ARG_LONG));
	}
}

const MonitoringSession& SessionManagerImpl::openMonitoringSession(
		const string& contact_) {
	return _mSession;
}

void SessionManagerImpl::closeMonitoringSession(const MonitoringSession& session_) {
}

const StringList& SessionManagerImpl::getReservationSessionNames(void) {
	//TODO Add Code here
	throw std::exception();
}

const StringList& SessionManagerImpl::getJobSessionNames(void) {
	//TODO Add Code here
	throw std::exception();
}

void SessionManagerImpl::registerEventNotification(
		const DrmaaCallback& callback) {
	//TODO Add Code here
}

void SessionManagerImpl::initialize() {
	// For now contact will be same for all Session
	// So initialize the ConnectionPool with set of PBS connection
	char *pbsDefault = pbs_default();
	if (pbsDefault) {
		PBSConnection pbsconn_(pbsDefault, 0, 0);
		for (int i = 1; i < (MAX_CONNS + 1); i++) {
			try {
				ConnectionPool::getInstance()->addConnection(pbsconn_);
			} catch (const Drmaa2Exception &ex) {
				// Caller should catch the exception.
				std::stringstream ss;
				ss << ex.what();
				ss << " Created only ";
				ss << (i);
				ss << " Connections";
				throw InternalException(SourceInfo(__func__, __LINE__),
						Message(ss.str()));
			}
		}
	} else {
		throw InternalException(SourceInfo(__func__, __LINE__));
	}
}

} /* namespace drmaa2 */

