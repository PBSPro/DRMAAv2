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

#include <SessionManagerImpl.h>

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
	//TODO Add Code here
	throw std::exception();
}

const JobSession& SessionManagerImpl::openJobSession(
		const string& sessionName_) {
	//TODO Add Code here
	throw std::exception();
}

void SessionManagerImpl::closeJobSession(JobSession& session_) {
	//TODO Add Code here

}

void SessionManagerImpl::destroyJobSession(const string& sessionName_) {
	//TODO Add Code here
}

const ReservationSession& SessionManagerImpl::createReservationSession(
		const string& sessionName_, const string& contact_) {
	//TODO Add Code here
	throw std::exception();
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
	//TODO Add Code here
}

const MonitoringSession& SessionManagerImpl::openMonitoringSession(
		const string& contact_) {
	//TODO Add Code here
	throw std::exception();
}

void SessionManagerImpl::closeMonitoringSession(MonitoringSession& session_) {
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
	// TODO Add code here
}

} /* namespace drmaa2 */
