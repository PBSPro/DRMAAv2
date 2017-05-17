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

#include <cppunit/extensions/AutoRegisterSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <ReservationSessionTest.h>
#include <SessionManagerImpl.h>
#include <PBSProSystem.h>
#include "drmaa2.hpp"
#include <string>
#include <unistd.h>


using namespace drmaa2;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ReservationSessionTest);

void ReservationSessionTest::TestReservationSession() {
	string session_("ReservationSession"), contact_(pbs_default());
	SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
	ReservationTemplate rt_;
	const ReservationSession &resSessionObj_ = sessionManagerObj_->createReservationSession(session_, contact_);
	rt_.reservationName.assign("DRMAA2RESERVATION");
	rt_.startTime = time(NULL) + 30;
	rt_.duration = 1000;
	rt_.endTime = 0;
	rt_.minSlots = 1;
	rt_.minPhysMemory = 30;
	rt_.usersACL.push_back("root@rampranesh");
	const ReservationTemplate &rTemplate_ = rt_;
	const Reservation &res = resSessionObj_.requestReservation(rTemplate_);
	sleep(5);
	ReservationInfo _rInfo = res.getInfo();
	res.terminate();
	sessionManagerObj_->destroyReservationSession(session_);
	delete sessionManagerObj_;
}
