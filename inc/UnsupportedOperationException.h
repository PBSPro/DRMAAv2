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

#ifndef INC_UNSUPPORTEDOPERATIONEXCEPTION_H_
#define INC_UNSUPPORTEDOPERATIONEXCEPTION_H_

#include <Drmaa2Exception.h>
#include <Message.h>
#include <SourceInfo.h>

namespace drmaa2 {

/**
 * @class UnsupportedOperationException
 * @brief Exception is generated for unsupported operation
 */
class UnsupportedOperationException: public Drmaa2Exception {
public:

	/**
	 * @brief Constructs the exception with the specified message and source location.
	 * @param message - Message associated to the exception.
	 * @param sourceInfo - Source location related to the exception.
	 */
	UnsupportedOperationException(const SourceInfo &sourceInfo = SourceInfo(),
			const Message &message = Message(UN_SUPPORTED_OP_SHORT,
			UN_SUPPORTED_OP_LONG));

	/**
	 * @brief default destructor
	 *
	 */
	virtual ~UnsupportedOperationException() throw();
};

} /* namespace drmaa2 */

#endif /* INC_UNSUPPORTEDOPERATIONEXCEPTION_H_ */
