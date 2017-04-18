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

#ifndef INC_PBSCONNECTION_H_
#define INC_PBSCONNECTION_H_

#include <Connection.h>
#include <string>

using namespace std;
namespace drmaa2 {

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
	 * @param[in] serverName_ - const reference to serverName
	 * @param[in] fd_ - const reference to file descriptor
	 * @param[in] port_ - const reference to port number
	 */
	PBSConnection(const string& serverName_, const int& fd_, const int& port_) :
			_serverName(serverName_), _fd(fd_), _port(port_) {
	}
	/**
	 * @brief
	 * 	PBSConnection(PBSConnection&) - Copy constructor.
	 *
	 * @param[in]   obj - const reference to PBSConnection object
	 */
	PBSConnection(const PBSConnection& obj) :
			_serverName(obj.getServerName()), _fd(obj.getFd()), _port(
					obj.getPort()) {
	}
	/**
	 * @brief
	 * 	~PBSConnection() - virtual destructor.
	 */
	virtual ~PBSConnection();
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
	virtual Connection* clone() const;
private:
	/**
	 * @brief
	 *	connect() - establish connection to PBSPro
	 *
	 * @throw refer drmaa2::DRMSystem::connect
	 *
	 * @return   void
	 *
	 */
	virtual void connect() throw (ImplementationSpecificException);

	/**
	 * @brief
	 *	disconnect() - disconnect from PBSPro
	 * @throw refer drmaa2::DRMSystem::disconnect
	 *
	 * @return   void
	 *
	 */
	virtual void disconnect() throw (ImplementationSpecificException);
};

} /* namespace drmaa2 */

#endif /* INC_PBSCONNECTION_H_ */
