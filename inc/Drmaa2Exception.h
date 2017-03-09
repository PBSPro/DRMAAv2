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

#ifndef SRC_DRMAA2EXCEPTION_H_
#define SRC_DRMAA2EXCEPTION_H_

#include <exception>
#include <string>

#include "Message.h"
#include "SourceInfo.h"

using namespace std;
namespace drmaa2 {
/**
 * @class Drmaa2Exception
 * @brief Basic Drmaa2 exception
 */

class Drmaa2Exception: public std::exception {
public:
	/**
	 * @brief Default denstructor
	 */
	virtual ~Drmaa2Exception() throw();

	/**
	 * @brief copy constructor
	 *
	 * @param other_ - Object to be copied
	 */
	Drmaa2Exception(const Drmaa2Exception &other_);

	/**
	 * @brief Argument constructor
	 *
	 * @param message_ - Message
	 *
	 * @param sourceInfo_ - source info
	 */
	Drmaa2Exception(const Message &message_, const SourceInfo &sourceInfo_);

	/**
	 * @brief Operator '='overloading
	 *
	 * @param other_ - Drmaa2Exception object
	 *
	 * @return - Drmaa2Exception object
	 */
	Drmaa2Exception &operator =(const Drmaa2Exception &other_);

	/**
	 * @brief Returns exception message set during throw
	 *
	 * @param - None
	 *
	 * @return - message
	 */
	virtual const char *what() const throw ();

	/**
	 * @brief Getter method for message
	 *
	 * @return - message
	 */
	const Message& getMessage() const {
		return _message;
	}

	/**
	 * @brief Setter method for message
	 *
	 * @param message_ - message
	 *
	 * @return - None
	 */

	void setMessage(const Message& message_) {
		_message = message_;
	}

private:
	Message _message;
	SourceInfo _sourceInfo;
	string _whatInfo; /*<! This is to avoid complexity involved in
							dynamic memory handling during what() function */

};

} /* namespace drmaa2 */

#endif /* SRC_DRMAA2EXCEPTION_H_ */
