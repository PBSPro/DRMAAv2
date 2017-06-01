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

#include <JobArrayImpl.h>
#include <ConnectionPool.h>
#include <PBSProSystem.h>
#include <JobTemplateAttrHelper.h>
#include <Drmaa2Exception.h>
#include <JobImpl.h>
#include <stdlib.h>
#include <pbs_ifl.h>

namespace drmaa2 {

JobArrayImpl::~JobArrayImpl() {
	// TODO Auto-generated destructor stub
}
const string& JobArrayImpl::getJobArrayId(void) const {
	return _jobId;
}

JobList& JobArrayImpl::getJobs(void) {
	JobInfo filter_;
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	_jobList = drms->getJobs(pbsConnPoolObj_, filter_);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
	return _jobList;
}

const JobTemplate& JobArrayImpl::getJobTemplate(void) const {
	return _jt;
}

void JobArrayImpl::suspend(void) const {
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	drms->suspend(pbsConnPoolObj_, *this);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
}

void JobArrayImpl::resume(void) const {
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	drms->resume(pbsConnPoolObj_, *this);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
}

void JobArrayImpl::hold(void) const {
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	drms->hold(pbsConnPoolObj_, *this);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
}

void JobArrayImpl::release(void) const {
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	drms->release(pbsConnPoolObj_, *this);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
}

void JobArrayImpl::terminate(void) const {
	const Connection &pbsConnPoolObj_ = ConnectionPool::getInstance()->getConnection();
	DRMSystem *drms = Singleton<DRMSystem, PBSProSystem>::getInstance();
	drms->terminate(pbsConnPoolObj_, *this);
	ConnectionPool::getInstance()->returnConnection(pbsConnPoolObj_);
}

void JobArrayImpl::reap(void) const {

}

}
