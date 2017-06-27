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
#include <JobApiTest.h>
#include "drmaa2.h"
#include "drmaa2.hpp"
#include <string.h>
#include <unistd.h>


using namespace drmaa2;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(JobApiTest);

void JobApiTest::TestJobApi() {
	drmaa2_jsession js1 = drmaa2_create_jsession("SessionApi", "Contact");
	drmaa2_open_jsession("SessionApi");
	drmaa2_string jsC = drmaa2_jsession_get_contact(js1);
	drmaa2_string_free(&jsC);
	drmaa2_jtemplate jt = drmaa2_jtemplate_create();
	jt->remoteCommand = strdup("/bin/sleep");
	jt->jobName = strdup("JobApiTest");
	drmaa2_list_add(jt->args, (void*)strdup("100"));
	drmaa2_j j = drmaa2_jsession_run_job(js1, jt);
	drmaa2_j_hold(j);
	drmaa2_j_terminate(j);
	drmaa2_jinfo jinfo = drmaa2_j_get_info(j);
	drmaa2_j_free(&j);
	drmaa2_jarray ja = drmaa2_jsession_run_bulk_jobs(js1, jt, 1, 4, 1, 2);
	drmaa2_list jL = drmaa2_jarray_get_jobs(ja);
	drmaa2_list_free(&jL);
	drmaa2_jtemplate jtA = drmaa2_jarray_get_jtemplate(ja);
	sleep(2);
	drmaa2_jarray_suspend(ja);
	drmaa2_jarray_terminate(ja);
	drmaa2_string jaId = drmaa2_jarray_get_id(ja);
	drmaa2_string_free(&jaId);
	drmaa2_jarray_free(&ja);
	drmaa2_jtemplate_free(&jt);
	drmaa2_jtemplate_free(&jtA);
	drmaa2_jinfo_free(&jinfo);
	drmaa2_jsession_free(&js1);
}
