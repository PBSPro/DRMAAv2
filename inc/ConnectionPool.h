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
#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <Connection.h>
#include <pthread.h>
#include <list>
#include <InternalException.h>

using namespace std;

#define MAX_CONNS 20
#define CON_NOT_AVAILABLE  "No Connection available"
#define ADD_CON_FAILED  "Add connection failed"
#define MAX_CON_REACHED "Max connection reached"

namespace drmaa2 {

/**
 *  @brief Class that maintains pool of connections to DRMS.
 */
class ConnectionPool {
private:
	static pthread_mutex_t _instMutex;
	static ConnectionPool* _instance;
	static pthread_mutex_t _connMutex;
	/**
	 * @brief
	 *      ConnectionPool() - constructor for ConnectionPool
	 *
	 */
	ConnectionPool() {
	}
	/**
	 * @brief
	 *      ConnectionPool() - copy constructor for ConnectionPool
	 *
	 */
	ConnectionPool(ConnectionPool& _conPool) {
	}
	list<Connection*> _usedConnections;
	list<Connection*> _freeConnections;
public:
	/**
	 * @brief
	 *	getInstance() - returns singleton Instance of ConnectionPool
	 *
	 * @return    pointer to ConnectionPool object
	 *
	 */
	static ConnectionPool* getInstance() {
		pthread_mutex_lock(&_instMutex);
		if (_instance == 0) {
			_instance = new ConnectionPool;
		}
		pthread_mutex_unlock(&_instMutex);
		return _instance;
	}
	/**
	 * @brief
	 *      getConnection() - returns the available connections in pool.
	 *
	 * @param - None
	 *
	 * @throw InternalException - If connections are not available
	 *
	 * @return	Connection
	 */
	const Connection& getConnection() throw (InternalException);

	/**
	 * @brief
	 *      addConnection() - adds the connection passed and establishes connection.
	 *
	 * @param[in]   object - Connection
	 *
	 * @throw InternalException - If Max connection reached
	 *
	 * @throw refer drmaa2::Connection::connect
	 *
	 * @return	None
	 */
	void addConnection(const Connection& object)
			throw (ImplementationSpecificException, InternalException);
	/**
	 * @brief
	 *      returnConnection() - returns unused connection to pool.
	 *
	 * @param[in]   object - const reference to PBSConnection
	 *
	 * @return	void
	 *
	 */
	void returnConnection(const Connection& object);
	/**
	 * @brief
	 *      reconnectConnection() - re-establishes connection to PBS.
	 *
	 * @param[in]   object - Connection
	 *
	 * @throw refer drmaa2::Connection::connect
	 *
	 * @return	void
	 *
	 */
	void reconnectConnection(const Connection& object)
			throw (ImplementationSpecificException, InternalException);

	/**
	 * @brief
	 *      clearConnectionPool() - closes existing connections and frees up the pool.
	 *
	 * @return	void
	 *
	 */
	void clearConnectionPool();
};
}
#endif
