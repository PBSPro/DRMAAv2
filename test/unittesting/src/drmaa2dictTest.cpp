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

#include "drmaa2dictTest.h"

namespace drmaa2 {
CPPUNIT_TEST_SUITE_REGISTRATION(drmaa2_dict_Test);

drmaa2_dict_Test::drmaa2_dict_Test() {
	d = NULL;
}
static void callback(char **key, char **value) {
	// Do nothing
}

void drmaa2_dict_Test::setUp() {
	d = drmaa2_dict_create(callback);
	drmaa2_dict_set(d, "key_1", "value_1");
	drmaa2_dict_set(d, "key_2", "value_2");
	drmaa2_dict_set(d, "key_3", "value_3");
}

void drmaa2_dict_Test::tearDown() {
	drmaa2_dict_free(&d);
}

void drmaa2_dict_Test::testDictCreate() {
	CPPUNIT_ASSERT(d != NULL);
}

void drmaa2_dict_Test::testDictFree() {
	drmaa2_dict_free(&d);
	CPPUNIT_ASSERT(d == NULL);
}

void drmaa2_dict_Test::testDictDel() {
	CPPUNIT_ASSERT_EQUAL(DRMAA2_SUCCESS, drmaa2_dict_del(d, "key_1"));
	CPPUNIT_ASSERT_EQUAL(DRMAA2_INVALID_ARGUMENT, drmaa2_dict_del(d, "key_1"));
}

void drmaa2_dict_Test::testDictSetGet() {
	const char *key4 = "key_4";
	const char *value4 = "value_4";
	drmaa2_dict_set(d, key4, value4);
	CPPUNIT_ASSERT_EQUAL(value4, drmaa2_dict_get(d, "key_4"));
}

void drmaa2_dict_Test::testDictGetList() {
	drmaa2_list l = drmaa2_dict_list(d);
	CPPUNIT_ASSERT(l != NULL);
	drmaa2_list_free(&l);
}

void drmaa2_dict_Test::testFreeNullDict() {
	drmaa2_dict dd = NULL;
	drmaa2_dict_free(&dd);
	CPPUNIT_ASSERT(dd == NULL);

}

drmaa2_dict_Test::~drmaa2_dict_Test() {
	// TODO Auto-generated destructor stub
}

} /* namespace drmaa2 */
