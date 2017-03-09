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

#ifndef SRC_SOURCEINFO_H_
#define SRC_SOURCEINFO_H_

#include <string>

/*!< Macro will fetch the line number and filename*/
#define DRMAA2_SOURCEINFO() drmaa2::SourceInfo( __FILE__, __LINE__ )
using namespace std;

namespace drmaa2 {
/**
 * @class SourceInfo
 * @brief The class contains source code location information.
 *
 */
class SourceInfo {
public:
	/**
	 * @brief Default constructor
	 *
	 */
	SourceInfo();

	/**
	 * @brief destructor
	 *
	 */
	virtual ~SourceInfo();

	/**
	 * @brief copy constructor
	 *
	 * @param other_ - Object to be copied
	 *
	 */
	SourceInfo(const SourceInfo &other_);

	/**
	 * @brief Argument constructor to create a SourceInfo object
	 *
	 * @param fileName_ - Name of the file source code
	 * @param lineNumber_ - Line number of the source code
	 *
	 */
	SourceInfo(const string &fileName_, int lineNumber_);

	/**
	 * @brief Operator '='overloading
	 *
	 * @param other_ - SourceInfo object
	 *
	 * @return - SourceInfo object
	 */
	SourceInfo &operator =(const SourceInfo &other_);

	/**
	 * @brief Operator '==' overloading
	 *
	 * @param other_ - SourceInfo object
	 *
	 * @return - true if objects are equal
	 * 			false otherwise
	 *
	 */
	bool operator ==(const SourceInfo &other_) const;

	/**
	 * @brief Operator '!=' overloading
	 *
	 * @param other_ - SourceInfo object
	 *
	 * @return - true if objects not equal
	 * 			false otherwise
	 *
	 */
	bool operator !=(const SourceInfo &other_) const;

	/**
	 * @brief Getter method for file name
	 *
	 * @return - file name
	 */
	const string& getFileName() const {
		return _fileName;
	}

	/**
	 * @brief Getter method for line number
	 *
	 * @return - line number
	 */
	const int getLineNumber() const {
		return _lineNumber;
	}

private:
	string _fileName; /*!< File name*/
	int _lineNumber; /*!< Line Number*/
};
} /* namespace drmaa2 */

#endif /* SRC_SOURCEINFO_H_ */
