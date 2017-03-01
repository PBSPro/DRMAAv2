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

#include "ConnectionPool.h"

ConnectionPool* ConnectionPool::_instance = 0;
pthread_mutex_t ConnectionPool::_instMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ConnectionPool::_connMutex = PTHREAD_MUTEX_INITIALIZER;


const Connection& ConnectionPool::getConnection() {
	pthread_mutex_lock(&ConnectionPool::_connMutex);
	if(_freeConnections.size() > 0) {
		Connection *cnHold_ = _freeConnections.front();
		_usedConnections.push_back(cnHold_);
		_freeConnections.pop_front();
		pthread_mutex_unlock(&ConnectionPool::_connMutex);
		return *cnHold_;
	}
	//TODO: Throw exception
	pthread_mutex_unlock(&ConnectionPool::_connMutex);
}

const Connection& ConnectionPool::addConnection(const Connection& object) {
	pthread_mutex_lock(&ConnectionPool::_connMutex);
	if ((_usedConnections.size() + _freeConnections.size()) < MAX_CONNS) {
		Connection *addObj_ = object.clone();
		addObj_->connect();
		_usedConnections.push_back(addObj_);
		pthread_mutex_unlock(&ConnectionPool::_connMutex);
		return *addObj_;
	} else {
		//TODO: Throw exception
	}
	pthread_mutex_unlock(&ConnectionPool::_connMutex);
}

void ConnectionPool::returnConnection(const Connection& object) {
	pthread_mutex_lock(&ConnectionPool::_connMutex);
	if(_usedConnections.size() > 0) {
		for(list<Connection*>::iterator it = _usedConnections.begin(); it != _usedConnections.end(); ++it) {
			if(&object == *it) {
				PBSConnection *_pbsCnHolder = static_cast<PBSConnection*>(*it);
				_freeConnections.push_back(_pbsCnHolder);
				_usedConnections.erase(it);
				break;
			}
		}
	}
	//TODO: Throw exception
	pthread_mutex_unlock(&ConnectionPool::_connMutex);
}

void ConnectionPool::reconnectConnection(const Connection& object) {
	pthread_mutex_lock(&ConnectionPool::_connMutex);
	for(list<Connection*>::const_iterator it = _usedConnections.begin(); it != _usedConnections.end(); ++it) {
		if(&object == *it) {
			(*it)->connect();
		}
	}
	pthread_mutex_unlock(&ConnectionPool::_connMutex);
	//TODO: Throw exception
}

void ConnectionPool::clearConnectionPool() {
	pthread_mutex_lock(&ConnectionPool::_connMutex);
	if(_freeConnections.size() > 0) {
		list<Connection*>::iterator it = _freeConnections.begin();
		while(it != _freeConnections.end()) {
			(*it)->disconnect();
			delete (*it);
			_freeConnections.erase(it++);
		}
	}
	if(_usedConnections.size() > 0) {
		list<Connection*>::iterator it = _usedConnections.begin();
		while(it != _usedConnections.end()) {
			(*it)->disconnect();
			delete (*it);
			_usedConnections.erase(it++);
		}
	}
	pthread_mutex_unlock(&ConnectionPool::_connMutex);
}
