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

#ifndef INC_ATTRHELPER_H
#define INC_ATTRHELPER_H

#include "drmaa2.hpp"
extern "C" {
#include "pbs_ifl.h"
#include "pbs_error.h"
}

namespace drmaa2 {

/**
 * @class AttrHelper
 * @brief Helper class for parsing Templates to attributes 
 */
class AttrHelper {
public:
	ATTRL* _attrList;
	/**
	 * @brief default constructor
	 *
	 */
	AttrHelper() {
		_attrList = NULL;
	}
	/**
	 * @brief default destructor
	 *
	 */
	virtual ~AttrHelper() {
		if(_attrList)
			deleteAttributeList();
	}
	/**
	 * @brief Method to create new attribute list
	 *
	 * @return  attrl - pointer to attribute List
	 */
	virtual ATTRL* createAttribute();
	/**
	 * @brief Method to free attribute list
	 *
	 * @return  void
	 */
	virtual void deleteAttributeList();
	/**
	 * @brief Method to add entry to attribute list
	 *
	 * @param[in] attrName_ - attribute name
	 * 	      attrVal_  - attribute value
	 *	      op_ - operator needs to be set
	 *
	 * @return  void
	 */
	virtual void setAttribute(char *attrName_, char *attrVal_, OPERATION op_ = SET);
	/**
	 * @brief Method to get entry from attribute list
	 *
	 * @param[in] attrName_ - attribute name
	 * 	      attrVal_  - attribute value
	 *
	 * @throw Drmaa2_Exception - Attribute not found exception
	 *
	 * @return  char* - Pointer to value
	 */
	virtual char* getAttribute(char *attrName_, char *attrVal_) throw();
	/**
	 * @brief Method to get entry from attribute list
	 *
	 * @param[in] resName_  - Resource name
	 *	      resVal_   - Resource Value
	 */
	virtual void setResource(char *resName_, char *resVal_);
	/**
	 * @brief pure virtual method to override for different templates.
	 */
	virtual ATTRL* parseTemplate(void* template_) = 0;
};

}

#endif
