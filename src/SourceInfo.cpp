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

#include "SourceInfo.h"

namespace drmaa2 {

SourceInfo::SourceInfo() :
		_fileName(""), _lineNumber(-1) {

}

SourceInfo::~SourceInfo() {
	// TODO Auto-generated destructor stub
}

SourceInfo::SourceInfo(const SourceInfo& other_) :
		_fileName(other_._fileName), _lineNumber(other_._lineNumber) {
}

SourceInfo::SourceInfo(const string& fileName_, int lineNumber_) :
		_fileName(fileName_), _lineNumber(lineNumber_) {

}

SourceInfo& SourceInfo::operator =(const SourceInfo& other_) {
	if (this != &other_) {
		this->_fileName = other_._fileName;
		this->_lineNumber = other_._lineNumber;
	}
	return *this;
}

bool SourceInfo::operator ==(const SourceInfo& other_) const {
	return _fileName == other_._fileName && _lineNumber == other_._lineNumber;
}

bool SourceInfo::operator !=(const SourceInfo& other_) const {
	return !(*this == other_);
}

} /* namespace drmaa2 */
