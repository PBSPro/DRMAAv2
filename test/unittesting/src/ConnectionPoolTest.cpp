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

#include "../inc/ConnectionPoolTest.h"

#include <cppunit/extensions/AutoRegisterSuite.h>
#include <cppunit/TestAssert.h>
#include <Connection.h>
#include <ConnectionPool.h>
#include <InternalException.h>
#include <pbs_ifl.h>
#include <PBSConnection.h>

using namespace drmaa2;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ConnectionPoolTest);

void ConnectionPoolTest::setUp() {
	PBSConnection pbtest = PBSConnection(pbs_default(), 0, 0);
	ConnectionPool *tmp = ConnectionPool::getInstance();
	// add 5 connection
	for (int i = 0; i < 5; i++) {
		CPPUNIT_ASSERT_NO_THROW(tmp->addConnection(pbtest));
	}
}

void ConnectionPoolTest::tearDown() {
	ConnectionPool::getInstance()->clearConnectionPool();
}

void ConnectionPoolTest::TestMaxConnection() {
	int remainingCapacity = MAX_CONNS - 5;
	PBSConnection pbtest = PBSConnection(pbs_default(), 0, 0);
	ConnectionPool *tmp = ConnectionPool::getInstance();
	// add remaining
	for (int i = 0; i < remainingCapacity; i++) {
		CPPUNIT_ASSERT_NO_THROW(tmp->addConnection(pbtest));
	}
	CPPUNIT_ASSERT_THROW(tmp->addConnection(pbtest), InternalException);
}

void ConnectionPoolTest::TestConnection() {
	try {
		const Connection &conn_ =
				ConnectionPool::getInstance()->getConnection();

		const PBSConnection *pbsConn =
				dynamic_cast<const PBSConnection*>(&conn_);

		CPPUNIT_ASSERT(pbsConn->getFd() > 0);
	} catch (const Drmaa2Exception &ex) {
		//Do nothing
	}
}

void ConnectionPoolTest::TestNoConnection() {
	ConnectionPool *tmp = ConnectionPool::getInstance();

	// make connections busy in ConnectionPool
	for (int i = 0; i < 5; i++) {
		CPPUNIT_ASSERT_NO_THROW(tmp->getConnection());
	}
	// assert the getConnection
	CPPUNIT_ASSERT_THROW(tmp->getConnection(), InternalException);

}

void ConnectionPoolTest::TestReturnConnection() {
	ConnectionPool *tmp = ConnectionPool::getInstance();
	try {
		const Connection &conn_ = tmp->getConnection();

		// make connections busy in ConnectionPool
		for (int i = 0; i < 4; i++) {
			tmp->getConnection();
		}
		CPPUNIT_ASSERT_THROW(tmp->getConnection(), InternalException);
		tmp->returnConnection(conn_);
		CPPUNIT_ASSERT_NO_THROW(tmp->getConnection());

	} catch (const Drmaa2Exception &ex) {
		// do nothing
	}
}

void ConnectionPoolTest::TestReconnectConnection() {
	PBSConnection pbtest = PBSConnection(pbs_default(), 0, 0);
	ConnectionPool *tmp = ConnectionPool::getInstance();
	CPPUNIT_ASSERT_NO_THROW(tmp->reconnectConnection(pbtest));
}

void ConnectionPoolTest::TestConnectionFailExceptions() {

	ConnectionPool *tmp = ConnectionPool::getInstance();
	PBSConnection pbtest = PBSConnection("unknown", 0, 0);
	ConnectionPool::getInstance()->clearConnectionPool();

	CPPUNIT_ASSERT_THROW(tmp->addConnection(pbtest), ImplementationSpecificException);
}
