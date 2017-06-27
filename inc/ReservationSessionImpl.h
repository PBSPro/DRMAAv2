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

#ifndef INC_RESERVATIONSESSIONIMPL_H_
#define INC_RESERVATIONSESSIONIMPL_H_

#include <list>
#include <string>

#include "drmaa2.hpp"
#include <PBSIFLExtend.h>
#include <ConnectionPool.h>
#include <ReservationImpl.h>

using namespace std;

namespace drmaa2 {
class ReservationSessionImpl : public ReservationSession {
private:
	const string contact; /*!< DRMS Contact name */
	const string sessionName; /*!< Session name */
	mutable ReservationList _reservationList;
public:

	/**
	 * @brief Constructor
	 *
	 * @param contact_ - DRMS contact name
	 * @param sessionName_ - Session Name
	 *
	 */
	ReservationSessionImpl(const string sessionName_,
			const string& contact_ = string(pbs_default())) :
			ReservationSession(contact_, sessionName_) {

	}

	/**
	 * Destructor
	 */
	virtual ~ReservationSessionImpl(void) {
		for (list<Reservation*>::iterator it = _reservationList.begin(); it != _reservationList.end(); ++it) {
			ReservationImpl *_resvImpl = dynamic_cast<ReservationImpl*>(*it);
			delete _resvImpl;
		}
	}

	/**
	 * @brief Returns Reservation of a given ID
	 *
	 * @param[in] reservationId_ - reservationId_ to be searched in the list
	 *
	 * @return Reservation
	 */
	virtual const Reservation& getReservation(const string& reservationId_);

	/**
	 * @brief Request DRMS to create a reservation
	 *
	 * @param[in] reservationTemplate_ - reservation information
	 *
	 * @return Reservation
	 */
	virtual const Reservation& requestReservation(
			const ReservationTemplate& reservationTemplate_) const;

	/**
	 * @brief Returns associated list of reservations
	 *
	 * @param - None
	 *
	 * @return ReservationList
	 */

	virtual const ReservationList& getReservations(void);
};
}

#endif /* INC_RESERVATIONSESSIONIMPL_H_ */
