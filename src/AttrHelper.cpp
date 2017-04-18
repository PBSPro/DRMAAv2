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

#include <AttrHelper.h>
#include <OutOfResourceException.h>
#include <SourceInfo.h>
#include <cstring>
#include <new>

namespace drmaa2 {

ATTRL* AttrHelper::createAttribute() {
	try
	{
		ATTRL *_attr = new ATTRL;
		_attr->next = NULL;
		_attr->name = NULL;
		_attr->resource = NULL;
		_attr->value = NULL;
		return _attr;
	} catch (std::bad_alloc &e) {
		throw OutOfResourceException(SourceInfo(__func__, __LINE__));
	}
}

void AttrHelper::deleteAttributeList() {
	ATTRL *curAttr_, *holdAttr_;
	if(_attrList == NULL )
		return;

	for(curAttr_ = _attrList; curAttr_ != NULL; curAttr_ = holdAttr_) {
		holdAttr_ = curAttr_->next;
		delete curAttr_;
	}
}

void AttrHelper::setAttribute(char* attrName_, char* attrVal_, OPERATION op_) {
	ATTRL *attr_;

	attr_ = createAttribute();

	attr_->name = attrName_;
	attr_->value = attrVal_;
	attr_->op = op_;
	if (_attrList == NULL) {
		_attrList = attr_;
	} else {
		ADD_NODE(_attrList,attr_);
	}

	return;

}

char* AttrHelper::getAttribute(char* attrName_, char* attrVal_) {
	ATTRL *attrTmp_;

	attrTmp_ = _attrList;

	while (attrTmp_) {
		if (strcmp(attrName_, attrTmp_->name) == 0) {
			if (attrVal_) {
				if (strcmp(attrVal_, attrTmp_->resource) == 0) {
					return (attrTmp_->value);
				}
			} else {
				return (attrTmp_->value);
			}
		}
		attrTmp_ = attrTmp_->next;
	}
	return NULL;
}

void AttrHelper::setResource(char* resName_, char* resVal_) {
	ATTRL *attr_;

	attr_ = createAttribute();

	attr_->name = (char *)ATTR_l;
	attr_->resource = resName_;
	attr_->value = (char *)resVal_;
	if (_attrList == NULL) {
		_attrList = attr_;
	} else {
		ADD_NODE(_attrList,attr_);
	}

	return;
}

}


