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

#include "PBSProSystem.h"

namespace drmaa2 {

PBSProSystem::PBSProSystem() {
	// TODO Auto-generated constructor stub

}

PBSProSystem::~PBSProSystem() {
	// TODO Auto-generated destructor stub
}

void PBSProSystem::connect(Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::disconnect(const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

Job* PBSProSystem::runJob(const Connection& connection_,
		const JobTemplate& jobTemplate_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::hold(const Connection& connection_, const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::suspend(const Connection& connection_,
		const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::resume(const Connection& connection_,
		const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::release(const Connection& connection_,
		const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::terminate(const Connection& connection_,
		const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

void PBSProSystem::reap(const Connection& connection_, const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
}

JobState PBSProSystem::state(const Connection& connection_,
		const Job& job_) throw () {
	//TODO Add Code here
	throw std::exception();
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
	//TODO Add Code here
	throw std::exception();
}

MachineInfoList PBSProSystem::getAllMachines(
		const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
}

QueueInfoList PBSProSystem::getAllQueues(
		const Connection& connection_) throw () {
	//TODO Add Code here
	throw std::exception();
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
