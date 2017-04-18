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

#include <Drmaa2Exception.h>
#include <sstream>

namespace drmaa2 {

Drmaa2Exception::~Drmaa2Exception() throw () {
	// TODO Auto-generated destructor stub
}

Drmaa2Exception::Drmaa2Exception(const Drmaa2Exception& other_) {
	*this = other_;
}

Drmaa2Exception::Drmaa2Exception(const Message& message_,
		const SourceInfo& sourceInfo_) :
		_message(message_), _sourceInfo(sourceInfo_) {
}

Drmaa2Exception& Drmaa2Exception::operator =(const Drmaa2Exception& other_) {
	if (this != &other_) {
		_message = other_._message;
		_sourceInfo = other_._sourceInfo;
	}
	return *this;
}

const char* Drmaa2Exception::what() const throw () {
	// Default implementation
	// Msg will be in the format of _FILE_:_LINE_ "short" \n "Detail"
	std::stringstream sStreamer;
	if (_sourceInfo.getLineNumber() < -1) {
		sStreamer << _sourceInfo.getFileName()
				<< ":"
				<< _sourceInfo.getLineNumber()
				<< " ";
	}
	sStreamer << _message.getShortMsg()
			<< "\n"
			<< _message.getDetailMsg();
	sStreamer << _whatInfo;
	return _whatInfo.c_str();
}

} /* namespace drmaa2 */
