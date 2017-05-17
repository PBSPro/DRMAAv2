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

#ifndef SRC_RESERVATIONIMPL_H_
#define SRC_RESERVATIONIMPL_H_

#include <string>

#include "drmaa2.hpp"

using namespace std;

namespace drmaa2 {
/**
 * @class ReservationImpl
 * @brief Concrete class of Reservation
 *
 */
class ReservationImpl : public Reservation {
	/**
	 * Constructor
	 */
	ReservationImpl() {};
	/**
	 * Copy constructor
	 */
	ReservationImpl(const ReservationImpl &reservationImpl_) {};
public:
	const string _reservationId;
	ReservationTemplate _rTemplate;
	mutable ReservationInfo _rInfo;
	/**
	 * Parameterized constructor
	 */
	ReservationImpl(const string& reservationId_):_reservationId(reservationId_) {
	}
	/**
	 * Parameterized constructor
	 */
	ReservationImpl(const string& reservationId_, const ReservationTemplate& rTemplate_):_reservationId(reservationId_), _rTemplate(rTemplate_) {
	};
	/**
	 * Destructor
	 */
	virtual ~ReservationImpl(void);

	/**
	 * @brief Returns reservation Id given by DRMS
	 *
	 * @param - None
	 *
	 * @return valid reservation Id
	 */
	virtual const string& getReservationId(void) const {
		return _reservationId;
	}

	/**
	 * @brief Populates Job information
	 *
	 * @param - None
	 *
	 * @return None
	 */
	const void populateReservationInfo(void) const;

	/**
	 * @brief Returns ReservationTemplate from which Reservation is submitted
	 *
	 * @param - None
	 *
	 * @return  ReservationTemlpate
	 */
	virtual const ReservationTemplate& getReservationTemplate(void) const {
		return _rTemplate;
	}

	/**
	 * @brief Returns detailed information of Reservation
	 *
	 * @param - None
	 *
	 * @return ReservationInfo
	 */
	virtual const ReservationInfo& getInfo(void) const;

	/**
	 * @brief Terminate the Reservation
	 *
	 * @param - None
	 *
	 * @return None
	 */
	virtual void terminate(void) const;
};

} /* namespace drmaa2 */

#endif /* SRC_RESERVATIONIMPL_H_ */
