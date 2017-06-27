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

#ifndef INC_MONITORINGSESSIONIMPL_H_
#define INC_MONITORINGSESSIONIMPL_H_

#include <list>
#include <string>

#include "drmaa2.hpp"

using namespace std;

namespace drmaa2 {

class MonitoringSessionImpl : public MonitoringSession {
public:
	mutable MachineInfoList _mInfo;
	mutable ReservationList _rInfo;
	mutable JobList _jInfo;
	mutable QueueInfoList _qInfo;
	/**
	 * Destructor
	 */
	virtual ~MonitoringSessionImpl(void) {
	};

	/**
	 * @brief Returns list of machines available in the DRM system as
	 * 			execution host.
	 *
	 * @param[in] machines_ - Filter criteria for machines
	 *
	 * @return MachineInfoList
	 */
	virtual const MachineInfoList& getAllMachines(
			const list<string> machines_) const;

	/**
	 * @brief Returns list reservations visible for the user running
	 * 			the DRMAA-based application
	 *
	 * @param - None
	 *
	 * @return ReservationList
	 */
	virtual const ReservationList& getAllReservations(void) const;

	/**
	 * @brief Returns list Jobs visible for the user running
	 * 			the DRMAA-based application
	 *
	 * @param[in] filter_ - Filter criteria
	 *
	 * @return JobList
	 */
	virtual const JobList& getAllJobs(JobInfo& filter_) const;

	/**
	 * @brief Returns list of Queues available in the DRM system.
	 *
	 * @param[in] queues_ - Filter criteria for queues
	 *
	 * @return QueueInfoList
	 */
	virtual const QueueInfoList& getAllQueues(const list<string> queues_) const;
};

}

#endif /* INC_MONITORINGSESSIONIMPL_H_ */
