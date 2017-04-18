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

#include "../inc/drmaa2listTest.h"

#include <cppunit/extensions/AutoRegisterSuite.h>
#include <cppunit/TestAssert.h>
#include <cstring>

#include "../../../api/c-binding/drmaa2.h"

CPPUNIT_TEST_SUITE_REGISTRATION(drmaa2::drmaa2_list_Test);

namespace drmaa2 {

drmaa2_list_Test::drmaa2_list_Test() {
	l = NULL;
}

drmaa2_list_Test::~drmaa2_list_Test() {
}

void drmaa2_list_Test::setUp() {
	l = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	drmaa2_list_add(l, "drmaa2_1");
	drmaa2_list_add(l, "drmaa2_2");
	drmaa2_list_add(l, "drmaa2_3");
}

void drmaa2_list_Test::tearDown() {
	drmaa2_list_free(&l);
}

void drmaa2_list_Test::testListCreate() {
	CPPUNIT_ASSERT(l != NULL);
}

void drmaa2_list_Test::testListFree() {
	drmaa2_list_free(&l);
	CPPUNIT_ASSERT(l == NULL);
}

void drmaa2_list_Test::testListDel() {
	long size = drmaa2_list_size(l);

	CPPUNIT_ASSERT_EQUAL(DRMAA2_INVALID_ARGUMENT, drmaa2_list_del(NULL, 0));

	CPPUNIT_ASSERT_EQUAL(DRMAA2_INVALID_ARGUMENT,
			drmaa2_list_del(l, size + 10));

	drmaa2_list_add(l, "drmaa2_4");

	CPPUNIT_ASSERT_EQUAL(DRMAA2_SUCCESS,
			drmaa2_list_del(l, (drmaa2_list_size(l) - 1)));

	CPPUNIT_ASSERT_EQUAL(size, drmaa2_list_size(l));
}

void drmaa2_list_Test::testListGet() {
	const char *value4 = "drmaa2_4";
	const char *value5 = "drmaa2_5";
	const char *value6 = "drmaa2_6";
	const char *value7 = "drmaa2_7";
	drmaa2_list_add(l, value4);
	drmaa2_list_add(l, value5);
	drmaa2_list_add(l, value6);
	drmaa2_list_add(l, value7);

	CPPUNIT_ASSERT_EQUAL((const char * )drmaa2_list_get(l, 3), value4);
	CPPUNIT_ASSERT_EQUAL((const char * )drmaa2_list_get(l, 2), value5);
	CPPUNIT_ASSERT_EQUAL((const char * )drmaa2_list_get(l, 1), value6);
	CPPUNIT_ASSERT_EQUAL((const char * )drmaa2_list_get(l, 0), value7);
}

void drmaa2_list_Test::testListAdd() {
	long size = drmaa2_list_size(l);
	const char *value = "drmaa2_4";
	CPPUNIT_ASSERT_EQUAL(DRMAA2_INVALID_ARGUMENT, drmaa2_list_add(NULL, value));
	CPPUNIT_ASSERT_EQUAL(DRMAA2_SUCCESS, drmaa2_list_add(l, value));
	CPPUNIT_ASSERT_EQUAL((const char * )drmaa2_list_get(l, 0), value);
	CPPUNIT_ASSERT_EQUAL((size + 1), drmaa2_list_size(l));
}

void drmaa2_list_Test::testListSize() {
	CPPUNIT_ASSERT_EQUAL((long)-1, drmaa2_list_size(NULL));
	long size = drmaa2_list_size(l);
	drmaa2_list_add(l, "drmaa2");
	CPPUNIT_ASSERT_EQUAL((size + 1), drmaa2_list_size(l));
}

void drmaa2_list_Test::testFreeNullList() {
	drmaa2_list ll = NULL;
	drmaa2_list_free(&ll);
	CPPUNIT_ASSERT(ll == NULL);
}
} /* namespace drmaa2 */
