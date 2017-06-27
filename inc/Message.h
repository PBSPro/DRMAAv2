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

#ifndef INC_MESSAGE_H_
#define INC_MESSAGE_H_

#include <string>

#define DENIED_BY_DRMS_SHORT  "DeniedByDrmsException"
#define DENIED_BY_DRMS_LONG "Connection refused by DRMS"
#define DRMS_SERVER_NULL "DRMS Server name not valid"
#define DRMS_NO_CONNECTS "DRMS Connection full"
#define DRMS_JOB_SUBMIT_ERROR "Error submitting job"
#define DRMS_CONN_AUTHFAIL "DRMS Connection Authentication Failed"
#define DRM_COMM_SHORT  "DrmCommunicationException"
#define DRM_COMM_LONG "Communication error with DRMS"
#define IMPL_SPEC_SHORT  "ImplementationSpecificException"
#define IMPL_SPEC_LONG "Implementation Error"
#define INTERNAL_SHORT  "InternalException"
#define INTERNAL_LONG "System internal error"
#define INVALID_ARG_SHORT  "InvalidArgumentException"
#define INVALID_ARG_LONG "Argument specified is invalid"
#define INVALID_STATE_SHORT  "InvalidStateException"
#define INVALID_STATE_LONG "Invalid job state"
#define INVALID_SESSION_SHORT  "InvalidSessionException"
#define INVALID_SESSION_LONG "Invalid Session"
#define OUT_OF_RESOURCE_SHORT  "OutOfResourceException"
#define OUT_OF_RESOURCE_LONG "Shortage of resource(s)"
#define TIMEOUT_SHORT  "TimeoutException"
#define TIMEOUT_LONG "Timeout during operation"
#define TRY_LATER_SHORT  "TryLaterException"
#define TRY_LATER_LONG "Operation Failed try after sometime"
#define UN_SUPPORTED_ATTR_SHORT  "UnsupportedAttributeException"
#define UN_SUPPORTED_ATTR_LONG "Attribute is not supported"
#define UN_SUPPORTED_OP_SHORT  "UnsupportedOperationException"
#define UN_SUPPORTED_OP_LONG "Operation is not supported"
#define ATTR_NOT_FOUND "Attribute not found"

using namespace std;
namespace drmaa2 {

/**
 * @class Message
 * @brief Class contains two attributes to hold short and detail description
 * 			of a any message. This can be used for logging and exception
 */
class Message {
public:
	/**
	 * @brief Default constructor
	 *
	 */
	Message();

	/**
	 * @brief destructor
	 *
	 */
	virtual ~Message();

	/**
	 * @brief copy constructor
	 *
	 * @param other_ - Object to be copied
	 *
	 */
	Message(const Message &other_);


	/**
	 * @brief Argument constructor to create a Message object
	 * 			with short message
	 *
	 * @param shortMsg_ - Short message
	 *
	 */
	explicit Message(const string &shortMsg_);

	/**
	 * @brief Argument constructor to create a Message object
	 *
	 * @param shortMsg_ - Short message
	 * @param detailMsg_ - Detailed message
	 *
	 */
	explicit Message(const string &shortMsg_, const string &detailMsg_);


	/**
	 * @brief Operator '='overloading
	 *
	 * @param other_ - Message object
	 *
	 * @return - Message object
	 */
	Message &operator =(const Message &other_);

	/**
	 * @brief Operator '==' overloading
	 *
	 * @param other_ - Message object
	 *
	 * @return - true if objects are equal
	 * 			false otherwise
	 *
	 */
	bool operator ==(const Message &other_) const;

	/**
	 * @brief Operator '!=' overloading
	 *
	 * @param other_ - Message object
	 *
	 * @return - true if objects not equal
	 * 			false otherwise
	 *
	 */
	bool operator !=(const Message &other_) const;


	/**
	 * @brief getter method for detail message
	 *
	 * @return - detail message string reference
	 *
	 */
	const string& getDetailMsg() const {
		return _detailMsg;
	}

	/**
	 * @brief setter method for detailed message
	 *
	 * @param detailMsg - detail message
	 *
	 */
	void setDetailMsg(const string& detailMsg) {
		_detailMsg = detailMsg;
	}

	/**
	 * @brief getter method for short message
	 *
	 * @return - string message string reference
	 *
	 */
	const string& getShortMsg() const {
		return _shortMsg;
	}

	/**
	 * @brief setter method for short message
	 *
	 * @param shortMsg - short message
	 *
	 */
	void setShortMsg(const string& shortMsg) {
		_shortMsg = shortMsg;
	}

private:
	string _shortMsg; /*!< Short message */
	string _detailMsg; /*!< Detail message */
};

} /* namespace drmaa2 */

#endif /* INC_MESSAGE_H_ */
