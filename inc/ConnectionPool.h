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
#include <iostream>
#include <list>
#include <string>

extern "C" {
#include <pbs_ifl.h>
#include <syscall.h>
#include <pthread.h>
#include <unistd.h>
}

using namespace std;

#define MAX_CONNS 20

/**
  *  @brief Abstract class for holding DRMS.  
  */  
class Connection {
	friend class ConnectionPool;
	public:
		/**
		 * @brief
		 * 	~Connection() - virtual destructor.
		 */
		virtual ~Connection() {}
		/**
		 * @brief
		 *	clone() - Pure virtual function to clone the object.
		 */
		virtual Connection* clone() const = 0;
	private:
		/**
		 * @brief
		 *	connect() - Pure virtual function to connect to DRMS.
		 */
		virtual void connect() throw() = 0;
		/**
		 * @brief
		 *	connect() - Pure virtual function to disconnect from DRMS.
		 */
		virtual void disconnect() throw() = 0;
};

/**
  *  @brief Class that holds PBS related Information. 
  */
class PBSConnection: public Connection {
		const string _serverName;
		int _fd;
		const int _port;
		/**
		 * @brief
		 * 	operator==() - Compare operator for checking address of two objects
		 *
		 * @param[in]   obj - const reference to PBSConnection object
		 *
		 * @return	bool - True is address are same.
		 */
		bool operator==(const PBSConnection& obj) {
			if (this == &obj)
				return true;
			else
				return false;
		}
	public:
		/**
		 * @brief
		 * 	PBSConnection() - Parameterized constructor.
		 * 
		 * @param[in]   serverName_ - const reference to serverName
		 *  		fd_ - const reference to file descriptor 
		 *  		port_ - const reference to port number
		 */
		PBSConnection(const string& serverName_, const int& fd_, const int& port_)
			:_serverName(serverName_), _fd(fd_), _port(port_) {}
		/**
		 * @brief
		 * 	PBSConnection(PBSConnection&) - Copy constructor.
		 *
		 * @param[in]   obj - const reference to PBSConnection object
		 */
		PBSConnection(const PBSConnection& obj)
		: _serverName(obj.getServerName()), _fd(obj.getFd()), _port(obj.getPort()) {}
		/**
		 * @brief
		 * 	~PBSConnection() - virtual destructor.
		 */
		virtual ~PBSConnection() {};
		/**
		 * @brief
		 *	getFd() - returns connection fd
		 *
		 * @return    int - fd associated with the class
		 *
		 */
		int getFd() const {
			return _fd;
		}
		/**
		 * @brief
		 *	setFd() - sets connection fd with value passed
		 *
		 * @param[in]   fd - file descriptor 
		 *
		 * @return    void
		 *
		 */
		void setFd(int fd) {
			_fd = fd;
		}
		/**
		 * @brief
		 *	getPort() - gets port used for connection
		 *
		 * @return   int - port associated with the connection
		 *
		 */
		int getPort() const {
			return _port;
		}
		/**
		 * @brief
		 *	getServerName() - gets ServerName used for connection
		 *
		 * @return   string - ServerName associated with the connection
		 *
		 */
		const string& getServerName() const {
			return _serverName;
		}

		/**
		 * @brief
		 *	clone() - creates a clone of the current object
		 *
		 * @return   Connection* - pointer to PBSConnection class
		 *
		 */
		Connection* clone() const {
			PBSConnection *_clonePBSConnection = new PBSConnection(*this);
			if(this->getFd() > 0)
				_clonePBSConnection->setFd(dup(this->getFd()));
			return _clonePBSConnection;
		}
	private:
		/**
		 * @brief
		 *	connect() - establish connection to PBSPro
		 *
		 * @return   void
		 *
		 */
		void connect() throw() {
			_fd = pbs_connect((char *)_serverName.c_str()); //TODO: Will replace with wrapped DRMS System apis
			if(_fd < 0) {
				//TODO throw exception
			}
		}
		/**
		 * @brief
		 *	disconnect() - disconnect from PBSPro
		 *
		 * @return   void
		 *
		 */
		void disconnect() throw() {
			pbs_disconnect(_fd); //TODO: Will replace with wrapped DRMS System apis
			//TODO throw exception
		}
};

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
		ConnectionPool() {}
		/**
		 * @brief
		 *      ConnectionPool() - copy constructor for ConnectionPool
		 *
		 */
		ConnectionPool(ConnectionPool& _conPool) {}
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
			if (_instance == 0)
			{
				_instance = new ConnectionPool;
			}
			pthread_mutex_unlock(&_instMutex);
			return _instance;
		}
		/**
		 * @brief
		 *      getConnection() - returns the available connections in pool.
		 *
		 * @param[in]   void
		 *
		 * @return	Connection& - reference to PBSConnection
		 *
		 *
		 */
		const Connection& getConnection();
		/**
		 * @brief
		 *      addConnection() - adds the connection passed and establishes connection.
		 *
		 * @param[in]   object - const reference to PBSConnection
		 *
		 * @return	Connection& - const reference to PBSConnection
		 *
		 *
		 */
		const Connection& addConnection(const Connection& object);
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
		 * @param[in]   object - const reference to PBSConnection
		 *
		 * @return	void
		 *
		 */
		void reconnectConnection(const Connection& object);
		/**
		 * @brief
		 *      clearConnectionPool() - closes existing connections and frees up the pool.
		 *
		 * @return	void
		 *
		 */
		void clearConnectionPool();
};

#endif
