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
#include <ReservationApiTest.h>
#include "drmaa2.h"
#include "drmaa2.hpp"
#include <string.h>
#include <unistd.h>


using namespace drmaa2;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ReservationApiTest);

void ReservationApiTest::TestReservationApi() {
        drmaa2_rsession rs1 = drmaa2_create_rsession(strdup("SessionApi"), strdup("Contact"));
	drmaa2_open_rsession("SessionApi");
	drmaa2_rtemplate rt = drmaa2_rtemplate_create();
        rt->reservationName = strdup("DRMAA2RESERVATION");
        rt->startTime = time(NULL) + 30;
        rt->duration = 1000;
        rt->endTime = 0;
        rt->minSlots = 1;
	rt->minPhysMemory = 0;
	drmaa2_r r = drmaa2_rsession_request_reservation( rs1, rt);
	drmaa2_rinfo ri = drmaa2_r_get_info(r);
	drmaa2_rinfo_free(&ri);
	drmaa2_rtemplate_free(&rt);
	drmaa2_rsession_free(&rs1);
}
