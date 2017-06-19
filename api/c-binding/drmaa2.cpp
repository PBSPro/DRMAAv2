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

#include <drmaa2.hpp>
#include <drmaa2.h>

#include <string>
#include <list>
#include <vector>

#include "SessionManagerImpl.h"
#include <InvalidArgumentException.h>
#include <InvalidStateException.h>

using namespace std;
using namespace drmaa2;

#ifdef __cplusplus
extern "C" {
#endif
#include <string.h>

#include <stdlib.h>
#include <stdio.h>

const char *last_error_text[DRMAA2_LASTERROR+1] = {"Success",
	"Denied by DRMS",
	"Failed to communicating with DRMS",
	"Try later",
	"Session Management",
	"Operation Timed out",
	"Internal Error",
	"Invalid argument",
	"Invalid state",
	"Invalid session",
	"Out of resource",
	"Unsupported attribute",
	"Unsupported operation",
	"Implementation specific error",
	"Last"};

/**
 *  @brief __last_error is thread-local and Thead-safe. Setting it in one
 *  thread  does not affect its value in any other thread.
 */
__thread drmaa2_error __last_error = DRMAA2_SUCCESS;
extern __thread drmaa2_error __last_error;


/**
 *  @brief  frees the previously allocated drmaa2_string
 *
 *  @param[in]	str - pointer to pre allocated drmaa2_string

 *  @return - None
 *
 */
void drmaa2_string_free(drmaa2_string * str) {
	free(*str);
	*str = NULL;
}

/**
 *  @brief  returns the last occurred error
 *  @return drmaa2_error
 *
 */
drmaa2_error drmaa2_lasterror(void) {
	return lasterror;
}

/**
 *  @brief  returns the last occurred error in string
 *  @return
 *  	drmaa2_string - newly allocated string of last error
 *  	NULL - If no error text exists
 *
 */
drmaa2_string drmaa2_lasterror_text(void) {
	if (lasterror <= DRMAA2_UNSET_ERROR && lasterror >= DRMAA2_LASTERROR) {
		return NULL;
	}
	return (char*)last_error_text[lasterror];
}

/**
 *  @brief  Sets string list default callback function
 *
 *  @param value - Call back function for string
 *
 *  @return - none
 *
 */
void drmaa2_string_list_default_callback(void **value) {
	drmaa2_string_free((drmaa2_string *)value);
}

/**
 *  @brief  Sets job list default callback function
 *
 *  @param value - Call back function for job
 *
 *  @return - none
 *
 */
void drmaa2_j_list_default_callback(void **value) {
	drmaa2_j_free((drmaa2_j *)value);
}

/**
 *  @brief  Sets queueinfo list default callback function
 *
 *  @param value - Call back function for queueinfo
 *
 *  @return - none
 *
 */
void drmaa2_queueinfo_list_default_callback(void **value) {
	drmaa2_queueinfo_free((drmaa2_queueinfo *)value);
}

/**
 *  @brief  Sets machineinfo list default callback function
 *
 *  @param value - Call back function for machineinfo
 *
 *  @return - none
 *
 */
void drmaa2_machineinfo_list_default_callback(void **value) {
	drmaa2_machineinfo_free((drmaa2_machineinfo *)value);
}

/**
 *  @brief  Sets slotinfo list default callback function
 *
 *  @param value - Call back function for slotinfo
 *
 *  @return - none
 *
 */
void drmaa2_slotinfo_list_default_callback(void **value) {
	drmaa2_slotinfo_free((drmaa2_slotinfo *)value);
}

/**
 *  @brief  Sets reservation list default callback function
 *
 *  @param value - Call back function for reservation
 *
 *  @return - none
 *
 */
void drmaa2_r_list_default_callback(void **value) {
	drmaa2_r_free((drmaa2_r *)value);
}

/**
 *  @brief  creates a drmaa list
 *
 *  @param[in]	t - Type of the list which needs to be created
 *  @param[in]	callback - Call back function to call when free is called.

 *  @return drmaa2_list
 *
 */
drmaa2_list drmaa2_list_create(const drmaa2_listtype t,
		const drmaa2_list_entryfree callback) {
	drmaa2_list l = NULL;

	l = (drmaa2_list) malloc(sizeof(drmaa2_list_s));
	if (l != NULL) {
		l->free_callback = callback;
		l->type = t;
		l->size = 0;
		l->head = NULL;
	}
	return l;
}

/**
 *  @brief  frees a drmaa list
 *
 *  @param[in]	l - Pointer to drmaa list

 *  @return - None
 *
 */
void drmaa2_list_free(drmaa2_list * l) {
	if (*l == NULL)
		return;
	drmaa2_item head = (drmaa2_item) (*l)->head;
	drmaa2_item item;
	while (head != NULL) {
		item = head;
		head = item->next;
		if ((*l)->free_callback != NULL)
			(*l)->free_callback((void **) &(item->data.value));
		free(item);
	}
	free(*l);
	*l = NULL;
}

/**
 *  @brief  gets the element form a particular position of the list
 *
 *  @param[in]	l - Pointer to drmaa list
 *  @param[in]	pos - position from which the data has to be taken

 *  @return
 *  	void* - pointer to the element returned as void*
 *  	NULL  - If the position is not valid
 *
 */
const void *drmaa2_list_get(const drmaa2_list l, const long pos) {
	if (l == NULL)
		return NULL;

	if (pos < 0 || pos >= l->size)
		return NULL;

	drmaa2_item current = (drmaa2_item) l->head;
	long i;
	for (i = 0; i < pos; i++)
		current = current->next;
	return current->data.value;
}

/**
 *  @brief  adds an element to the list
 *
 *  @param[in]	l - Pointer to drmaa list
 *  @param[in] 	value - pointer to the element which needs to be inserted

 *  @return
 *  	drmaa2_error - DRMAA2_INVALID_ARGUMENT
 *  				 - DRMAA2_INTERNAL
 *				 - DRMAA2_SUCCESS
 *
 */
drmaa2_error drmaa2_list_add(drmaa2_list l, const void *value) {
	if (l == NULL)
		return DRMAA2_INVALID_ARGUMENT;

	drmaa2_item item;
	if ((item = (drmaa2_item) malloc(sizeof(drmaa2_item))) == NULL) {
		return DRMAA2_INTERNAL;
	}
	item->data.value = value;
	item->next = (drmaa2_item)l->head;
	l->head = item;
	l->size++;

	return DRMAA2_SUCCESS;

}

/**
 *  @brief  deletes an element from the list
 *
 *  @param[in]	l - Pointer to drmaa list
 *  @param[in]	pos - position of the element which needs to be deleted

 *  @return drmaa2_error - DRMAA2_INVALID_ARGUMENT
 *						 - DRMAA2_SUCCESS
 *
 */
drmaa2_error drmaa2_list_del(drmaa2_list l, const long pos) {
	if (l == NULL)
		return DRMAA2_INVALID_ARGUMENT;

	if (pos < 0 || pos >= l->size)
		return DRMAA2_INVALID_ARGUMENT;

	drmaa2_item current = (drmaa2_item) l->head;
	if (pos == 0) {
		drmaa2_item prev = current;
		l->head = prev->next;
		l->size--;
		if (l->free_callback != NULL)
			l->free_callback((void **) prev->data.value);
		free(prev);

		return DRMAA2_SUCCESS;
	}

	long i;
	for (i = 0; i < (pos - 1); i++)
		current = current->next;

	drmaa2_item prev = current->next;
	current->next = prev->next;
	l->size--;
	if (l->free_callback != NULL)
		l->free_callback((void **) prev->data.value);
	free(prev);

	return DRMAA2_SUCCESS;
}

/**
 *  @brief  returns the size of the list
 *
 *  @param[in]	l - Pointer to drmaa list

 *  @return
 *  		long - size of list
 *
 *
 */
long drmaa2_list_size(const drmaa2_list l) {
	if (l == NULL)
		return -1;
	return l->size;
}

/**
 *  @brief  Assigned default callback function for dictionary
 *
 *  @param[in]	key - key in dictionary
 *  @param[in]	value - value for key
 *
 *  @return
 *  		long - size of list
 *
 *
 */
void drmaa2_dict_default_callback(char **key, char **value) {
	if(*key) {
		free(*key);
		*key = NULL;
	}
	if(*value) {
		free(*value);
		*value = NULL;
	}
}

/**
 *  @brief  creates a drmaa dictionary
 *
 *  @param[in]	callback - Call back function to call when free is called.

 *  @return
 *  		drmaa2_dict
 *
 */
drmaa2_dict drmaa2_dict_create(const drmaa2_dict_entryfree callback) {
	drmaa2_dict d = (drmaa2_dict) malloc(sizeof(drmaa2_dict_s));
	if(callback) {
		d->free_entry = callback;
	} else {
		d->free_entry = drmaa2_dict_default_callback;
	}
	d->head = NULL;
	return d;
}

/**
 *  @brief  frees a drmaa dict
 *
 *  @param[in]	d - Pointer to drmaa dict

 *  @return - none
 *
 */
void drmaa2_dict_free(drmaa2_dict * d) {
	if ((*d) == NULL)
		return;

	drmaa2_item head = (*d)->head;
	drmaa2_item tmp;
	while (head != NULL) {
		tmp = head;
		head = head->next;
		if ((*d)->free_entry != NULL)
			(*d)->free_entry((char **) &(tmp->data.kv_pair.key),
					(char **) &(tmp->data.kv_pair.value));
		free(tmp);
	}
	free(*d);
	*d = NULL;
}

/**
 *  @brief  returns a drmaa list of keys in the dictionary
 *
 *  @param[in]	d - Pointer to drmaa dict

 *  @return drmaa2_string_list
 *
 */
drmaa2_string_list drmaa2_dict_list(const drmaa2_dict d) {
	if (d == NULL)
		return NULL;

	drmaa2_string_list l = drmaa2_list_create(DRMAA2_STRINGLIST,
			(drmaa2_list_entryfree) drmaa2_string_free);
	drmaa2_item item = d->head;
	while (item != NULL) {
		drmaa2_list_add(l, item->data.kv_pair.key);
		item = item->next;
	}
	return l;
}

/**
 *  @brief  checks for the presence of key in the dictionary
 *
 *  @param[in]	d - Pointer to drmaa dict
 *  @param[in]	key - pointer to key string

 *  @return drmaa2_bool
 *
 */
drmaa2_bool drmaa2_dict_has(const drmaa2_dict d, const char *key) {
	if (d == NULL && key == NULL)
		return DRMAA2_FALSE;

	drmaa2_item item = d->head;
	while (item != NULL) {
		if (!strcmp(item->data.kv_pair.key, key))
			return DRMAA2_TRUE;
		item = item->next;
	}
	return DRMAA2_FALSE;
}

/**
 *  @brief  gets the key's value from the dictionary
 *
 *  @param[in]	d - Pointer to drmaa dict
 *  @param[in]	key - pointer to key string

 *  @return char *
 *
 */
const char *drmaa2_dict_get(const drmaa2_dict d, const char *key) {
	if (d == NULL && key == NULL)
		return NULL;

	drmaa2_item item = d->head;
	while (item != NULL) {
		if (!strcmp(item->data.kv_pair.key, key))
			return item->data.kv_pair.value;
		item = item->next;
	}
	return NULL;
}

/**
 *  @brief  deletes the entry with specified key from the dictionary
 *
 *  @param[in]	d - Pointer to drmaa dict
 *  @param[in]	key - pointer to key string

 *  @return
 *  	drmaa2_error -  DRMAA2_SUCCESS
 *  				 - 	DRMAA2_INVALID_ARGUMENT
 */
drmaa2_error drmaa2_dict_del(drmaa2_dict d, const char *key) {
	if (d == NULL && key == NULL)
		return DRMAA2_INVALID_ARGUMENT;

	drmaa2_item item = d->head;
	drmaa2_item prev = NULL;

	while (item != NULL) {
		if (!strcmp(item->data.kv_pair.key, key)) {
			if (prev)
				prev->next = item->next;
			else
				d->head = item->next;

			if (d->free_entry != NULL)
				d->free_entry((char **) &(item->data.kv_pair.key),
						(char **) &(item->data.kv_pair.value));
			free(item);
			return DRMAA2_SUCCESS;
		}
		prev = item;
		item = item->next;
	}
	return DRMAA2_INVALID_ARGUMENT;
}

/**
 *  @brief  sets the key value pair to the dictionary
 *
 *  @param[in]	d - Pointer to drmaa dict
 *  @param[in]	key - pointer to key string
 *  @param[in]	val - pointer to value string

 *  @return
 *  	drmaa2_error -  DRMAA2_SUCCESS
 *  				 - 	DRMAA2_INVALID_ARGUMENT
 */
drmaa2_error drmaa2_dict_set(drmaa2_dict d, const char *key, const char *val) {
	if (d == NULL && key == NULL)
		return DRMAA2_INVALID_ARGUMENT;
	drmaa2_item item = d->head;
	while (item != NULL) {
		if (!strcmp(item->data.kv_pair.key, key)) {
			if (d->free_entry != NULL)
				d->free_entry(NULL, (char **) &(item->data.kv_pair.value));
			item->data.kv_pair.value = val;
			return DRMAA2_SUCCESS;
		}
		item = item->next;
	}
	item = (drmaa2_item) malloc(sizeof(drmaa2_item_s));
	item->data.kv_pair.key = key;
	item->data.kv_pair.value = val;
	item->next = NULL;
	d->head == NULL ? (d->head = item) : (d->head->next = item);
	return DRMAA2_SUCCESS;
}

/**
 * @brief creates a drmaa2_job info structure and returns its pointer
 *
 * @param - none
 *
 * @return
 * 		drmaa2_jinfo - pointer to newly allocated drmaa2_jinfo structure
 *		NULL - if allocation fails and DRMAA2_OUT_OF_RESOURCE error is set
 *
 */

drmaa2_jinfo drmaa2_jinfo_create(void) {
	drmaa2_jinfo jinfo;
	try{
		jinfo = new drmaa2_jinfo_s();
	} catch(bad_alloc &ex) {
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		return NULL;
	}
	jinfo->allocatedMachines = drmaa2_list_create(DRMAA2_SLOTINFOLIST, NULL);;
	if(jinfo->allocatedMachines == NULL) {
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		drmaa2_jinfo_free(&jinfo);
		return NULL;
	}
	return jinfo;
}

/**
 * @brief creates a drmaa2_rinfo structure and returns its pointer
 *
 * @param - none
 *
 * @return
 * 		drmaa2_rinfo - pointer to newly allocated drmaa2_jinfo structure
 *		NULL - if allocation fails and DRMAA2_OUT_OF_RESOURCE error is set
 *
 */

drmaa2_rinfo drmaa2_rinfo_create(void) {
	drmaa2_rinfo ri;
	try{
		ri = new drmaa2_rinfo_s;
	}catch(bad_alloc& ex){
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		return NULL;
	}
	ri->usersACL = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	ri->reservedMachines = drmaa2_list_create(DRMAA2_SLOTINFOLIST, NULL);
	if(ri->usersACL == NULL || ri->reservedMachines == NULL) {
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		drmaa2_rinfo_free(&ri);
		return NULL;
	}
	return ri;
}

/**
 * @brief frees the drmaa2_job info structure and its inner data types
 *
 * @param[in]  ji -	pointer to drmaa2_job info structure.
 *
 * @param - none
 *
 * @return  void
 *
 */
void drmaa2_jinfo_free(drmaa2_jinfo * ji) {
	int size = 0, i = 0;
	char *tmp = NULL;
	if(*ji) {
		size = drmaa2_list_size((*ji)->allocatedMachines);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				tmp = (char *)drmaa2_list_get((*ji)->allocatedMachines, i);
				free(tmp);
			}
		}
		drmaa2_list_free(&(*ji)->allocatedMachines);
		if((*ji)->jobId)
			free((char *)(*ji)->jobId);
		if((*ji)->terminatingSignal)
			free((char *)(*ji)->terminatingSignal);
		if((*ji)->annotation)
			free((char *)(*ji)->annotation);
		if((*ji)->jobSubState)
			free((char *)(*ji)->jobSubState);
		if((*ji)->submissionMachine)
			free((char *)(*ji)->submissionMachine);
		if((*ji)->jobOwner)
			free((char *)(*ji)->jobOwner);
		if((*ji)->queueName)
			free((char *)(*ji)->queueName);
		free(*ji);
		*ji = NULL;
	}
}

/**
 *  @brief  frees the pre-allocated drmaa slotinfo
 *
 *  @param[in]	si- Pointer to drmaa slotinfo

 *  @return - None
 *
 */
void drmaa2_slotinfo_free(drmaa2_slotinfo * si) {
	if (*si != NULL) {
		drmaa2_string_free(&((*si)->machineName));
		free(*si);
		*si = NULL;
	}
}

/**
 *  @brief  frees the pre-allocated drmaa rinfo structure
 *
 *  @param[in]	ri- Pointer to drmaa reservation info

 *  @return - None
 *
 */
void drmaa2_rinfo_free(drmaa2_rinfo * ri) {
	int size = 0, i = 0;
	char *tmp = NULL;
	if (*ri != NULL) {
		drmaa2_string_free(&((*ri)->reservationId));
		drmaa2_string_free(&((*ri)->reservationName));
		size = drmaa2_list_size((*ri)->usersACL);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				tmp = (char *)drmaa2_list_get((*ri)->usersACL, i);
				free(tmp);
			}
		}
		drmaa2_list_free  (&((*ri)->usersACL));
		size = drmaa2_list_size((*ri)->reservedMachines);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				tmp = (char *)drmaa2_list_get((*ri)->reservedMachines, i);
				free(tmp);
			}
		}
		drmaa2_list_free  (&((*ri)->reservedMachines));
		free((*ri));
		*ri = NULL;
	}
	return;
}

/**
 * @brief creates a drmaa2_job template structure and returns its pointer
 *
 * @param - none
 *
 * @return
 * 		drmaa2_jtemplate - pointer to drmaa2_jtemplate if allocated
 *		NULL - if allocation fails and DRMAA2_OUT_OF_RESOURCE error is set
 */
drmaa2_jtemplate drmaa2_jtemplate_create(void) {
	drmaa2_jtemplate jt;
	try{
		jt = new drmaa2_jtemplate_s();
	}catch(bad_alloc &ex){
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		return NULL;
	}
	jt->jobEnvironment = drmaa2_dict_create(NULL);
	jt->stageInFiles = drmaa2_dict_create(NULL);
	jt->stageOutFiles = drmaa2_dict_create(NULL);
	jt->resourceLimits = drmaa2_dict_create(NULL);
	jt->candidateMachines = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	jt->email = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	jt->args = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	if(jt->jobEnvironment == NULL || jt->stageInFiles == NULL || jt->stageOutFiles == NULL
			|| jt->resourceLimits == NULL || jt->email == NULL || jt->args == NULL) {
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		drmaa2_jtemplate_free(&jt);
		return NULL;
	}
	return jt;
}

/**
 * @brief  frees a drmaa2_job template structure and its inner data types
 *
 * @param[in]   jt	-	pointer to drmaa2_job template structure.
 *
 * @return    void
 *
 */
void drmaa2_jtemplate_free(drmaa2_jtemplate * jt) {
	int size = 0, i = 0;
	char *tmp = NULL;
	if(*jt) {
		drmaa2_dict_free(&(*jt)->jobEnvironment);
		drmaa2_dict_free(&(*jt)->stageInFiles);
		drmaa2_dict_free(&(*jt)->stageOutFiles);
		drmaa2_dict_free(&(*jt)->resourceLimits);
		size = drmaa2_list_size((*jt)->candidateMachines);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				tmp = (char *)drmaa2_list_get((*jt)->candidateMachines, i);
				free(tmp);
			}
		}
		drmaa2_list_free(&(*jt)->candidateMachines);
		size = drmaa2_list_size((*jt)->email);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				tmp = (char *)drmaa2_list_get((*jt)->email, i);
				free(tmp);
			}
		}
		drmaa2_list_free(&(*jt)->email);
		size = drmaa2_list_size((*jt)->args);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				tmp = (char *)drmaa2_list_get((*jt)->args, i);
				free(tmp);
			}
		}
		drmaa2_list_free(&(*jt)->args);
		if((*jt)->remoteCommand)
			free((char *)(*jt)->remoteCommand);
		if((*jt)->workingDirectory)
			free((char *)(*jt)->workingDirectory);
		if((*jt)->jobCategory)
			free((char *)(*jt)->jobCategory);
		if((*jt)->jobName)
			free((char *)(*jt)->jobName);
		if((*jt)->inputPath)
			free((char *)(*jt)->inputPath);
		if((*jt)->outputPath)
			free((char *)(*jt)->outputPath);
		if((*jt)->errorPath)
			free((char *)(*jt)->errorPath);
		if((*jt)->reservationId)
			free((char *)(*jt)->reservationId);
		if((*jt)->queueName)
			free((char *)(*jt)->queueName);
		if((*jt)->accountingId)
			free((char *)(*jt)->accountingId);
		free((*jt));
		*jt = NULL;
	}
	return;
}

/**
 * @brief - creates a drmaa2_rtemplate and returns its pointer
 *
 * @param - none
 *
 * @return
 * 		drmaa2_rtemplate - pointer drmaa2_rtemplate if allocated
 *		NULL - if allocation fails and DRMAA2_OUT_OF_RESOURCE error is set
 */

drmaa2_rtemplate drmaa2_rtemplate_create(void) {
	drmaa2_rtemplate rt;
	try{
		rt = new drmaa2_rtemplate_s();
	}catch(bad_alloc &ex){
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		return NULL;
	}
	rt->usersACL = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	rt->candidateMachines = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	if (rt->usersACL == NULL || rt->candidateMachines == NULL) {
		drmaa2_rtemplate_free(&rt);
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		return NULL;
	}
	return rt;

}

/**
 *  @brief  frees the pre-allocated drmaa rtemplate structure
 *
 *  @param[in]	rt - Pointer to drmaa rtemplate
 *
 *  @return - None
 *
 */
void drmaa2_rtemplate_free(drmaa2_rtemplate * rt) {
	int size = 0, i = 0;
	char *nodeVal;
	if(*rt) {
		if((*rt)->reservationName)
			drmaa2_string_free(&(*rt)->reservationName);
		if((*rt)->jobCategory)
			drmaa2_string_free(&(*rt)->jobCategory);
		size = drmaa2_list_size((*rt)->usersACL);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				nodeVal = (char *)drmaa2_list_get((*rt)->usersACL, i);
				free(nodeVal);
			}
		}
		drmaa2_list_free(&(*rt)->usersACL);
		size = drmaa2_list_size((*rt)->candidateMachines);
		if(size > 0) {
			for(i = 0; i < size; i++) {
				nodeVal = (char *)drmaa2_list_get((*rt)->candidateMachines, i);
				free(nodeVal);
			}
		}
		drmaa2_list_free(&(*rt)->candidateMachines);
		free(*rt);
		*rt = NULL;
	}
	return;
}

/**
 *  @brief  frees the pre-allocated drmaa notification structure
 *
 *  @param[in]	n - Pointer to drmaa notification
 *
 *  @return - None
 *
 */
void drmaa2_notification_free(drmaa2_notification * n) {
	//TODO Add Code here
}

/**
 *  @brief  frees the pre-allocated drmaa queueinfo structure
 *
 *  @param[in]	qi- Pointer to drmaa queueinfo
 *
 *  @return - None
 *
 */
void drmaa2_queueinfo_free(drmaa2_queueinfo * qi) {
	if (*qi != NULL) {
		drmaa2_string_free(&((*qi)->name));
		free(*qi);
		*qi = NULL;
	}
}

/**
 *  @brief  frees the pre-allocated drmaa version structure
 *
 *  @param[in]	v - Pointer to drmaa version
 *
 *  @return - None
 *
 */
void drmaa2_version_free(drmaa2_version * v) {
	if(*v != NULL) {
		drmaa2_string_free(&(*v)->major);
		drmaa2_string_free(&(*v)->minor);
		free (*v);
		*v = NULL;
	}
}

/**
 *  @brief  frees the pre-allocated drmaa machine info structure
 *
 *  @param[in]	mi - Pointer to drmaa machine info
 *
 *  @return - None
 *
 */
void drmaa2_machineinfo_free(drmaa2_machineinfo * mi) {
	if(*mi != NULL) {
		drmaa2_version_free(&(*mi)->machineOSVersion);
		drmaa2_string_free(&(*mi)->name);
		free (*mi);
		*mi = NULL;
	}
}

/**
 * @brief drmaa2_jtemplate_impl_spec - Lists the drmaa spec implementations
 * 							supported for job template (e.g. drmaa, drmaa2)
 *
 * @return
 * 		drmaa2_string_list - list of drmaa spec implementations supported
 * 		NULL - If this feature is not supported,
 * 				set drmaa2_lasterror_v to DRMAA2_UNSUPPORTED_OPERATION
 */
drmaa2_string_list drmaa2_jtemplate_impl_spec(void) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief drmaa2_jinfo_impl_spec - Lists the drmaa spec implementations
 * 								supported for job info (e.g. drmaa, drmaa2)
 *
 * @return
 * 		drmaa2_string_list - list of drmaa spec implementations supported
 * 		NULL - If this feature is not supported,
 * 				set drmaa2_lasterror_v to DRMAA2_UNSUPPORTED_OPERATION
 */
drmaa2_string_list drmaa2_jinfo_impl_spec(void) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief Lists the drmaa spec implementations supported for
 * 			reservation template (e.g. drmaa, drmaa2)
 *
 * @return
 * 		drmaa2_string_list - list of drmaa spec implementations supported
 * 		NULL - If this feature is not supported,
 * 				set drmaa2_lasterror_v to DRMAA2_UNSUPPORTED_OPERATION
 */
drmaa2_string_list drmaa2_rtemplate_impl_spec(void) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief Lists the drmaa spec implementations supported for
 * 		reservation info (e.g. drmaa, drmaa2)
 *
 *
 * @return
 * 		drmaa2_string_list - list of drmaa spec implementations supported
 * 		NULL - If this feature is not supported,
 * 				set drmaa2_lasterror_v to DRMAA2_UNSUPPORTED_OPERATION
 */
drmaa2_string_list drmaa2_rinfo_impl_spec(void) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief drmaa2_queueinfo_impl_spec - Lists the queue info
 * 			spec implementations supported
 *
 *
 * @return
 * 		drmaa2_string_list -list of queueinfo spec implementations supported
 * 		NULL - If this feature is not supported,
 * 				set drmaa2_lasterror_v to DRMAA2_UNSUPPORTED_OPERATION
 */
drmaa2_string_list drmaa2_queueinfo_impl_spec(void) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief drmaa2_machineinfo_impl_spec - Lists the machine info
 * 			 spec implementations supported
 *
 *
 * @return
 * 		drmaa2_string_list -list of machineinfo spec implementations supported
 * 		NULL - If this feature is not supported,
 * 				set drmaa2_lasterror_v to DRMAA2_UNSUPPORTED_OPERATION
 */
drmaa2_string_list drmaa2_machineinfo_impl_spec(void) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief drmaa2_notification_impl_spec - Lists the notification
 * 			 spec implementations supported
 *
 *
 * @return
 * 		drmaa2_string_list - list of notification spec implementations supported
 * 		NULL - If this feature is not supported,
 * 				set drmaa2_lasterror_v to DRMAA2_UNSUPPORTED_OPERATION
 */
drmaa2_string_list drmaa2_notification_impl_spec(void) {
	//TODO Add Code here
	return NULL;
}

drmaa2_string drmaa2_get_instance_value(const void *instance,
		const char *name) {
	//TODO Add Code here
	return NULL;
}

drmaa2_string drmaa2_describe_attribute(const void *instance,
		const char *name) {
	//TODO Add Code here
	return NULL;
}

drmaa2_error drmaa2_set_instance_value(void *instance, const char *name,
		const char *value) {
	//TODO Add Code here
	return DRMAA2_SUCCESS;
}

/**
 *  @brief  Frees the memory of Job session pointed by js
 *
 *
 *  @param[in]		js	-	pointer to drmaa2_jsession.
 *
 *  @return - None
 */
void drmaa2_jsession_free(drmaa2_jsession * js) {
	if(*js) {
		SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
		JobSession *obj = reinterpret_cast<JobSession *>(*js);
		try {
			sessionManagerObj_->destroyJobSession(obj->getSessionName());
		} catch(InvalidArgumentException& ex) {
			return;
		}
	}
}
/**
 *  @brief  Frees the memory of Reservation session pointed by rs
 *
 *  @param[in]		rs	-	pointer to drmaa2_rsession.
 *
 *  @return - None
 */
void drmaa2_rsession_free(drmaa2_rsession * rs) {
	if(*rs) {
		SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
		ReservationSession *obj = reinterpret_cast<ReservationSession *>(*rs);
		try {
			sessionManagerObj_->destroyReservationSession(obj->getSessionName());
		} catch(InvalidArgumentException& ex) {
			return;
		}
	}
}

/**
 *  @brief  Frees the memory of Monitoring session pointed by ms
 *
 *
 *  @param[in]		ms	-	pointer to drmaa2_msession.
 *
 *  @return - None
 */
void drmaa2_msession_free(drmaa2_msession * ms) {
	//TODO Add Code here
}

/**
 *  @brief  frees the drmaa2_job structure pointed by j
 *
 *  @param[in]		j -	pointer to drmaa2_job
 *
 *  @return - None
 */
void drmaa2_j_free(drmaa2_j * j) {
	if (*j)
		delete (Job *)reinterpret_cast<Job *>(*j);
	*j = NULL;
}

/**
 *  @brief  frees the drmaa2_job array structure pointed by ja
 *
 *  @param[in]	ja	-	pointer to DRMAA jo array
 *
 *  @return - None
 */
void drmaa2_jarray_free(drmaa2_jarray * ja) {
	if (*ja)
		delete (JobArray *)reinterpret_cast<JobArray *>(*ja);
	*ja = NULL;
}

/**
 *  @brief  frees the drmaa2_r structure pointed by r
 *
 *  @param[in]	r	-	pointer to DRMAA reservation
 *
 *  @return - None
 */
void drmaa2_r_free(drmaa2_r * r) {
	if(*r == NULL)
		return;
	Reservation *obj = reinterpret_cast<Reservation *>(*r);
	delete obj;
}

/**
 * @brief Get the contact of a given drmaa2 reservation session.
 *
 * @param[in]  rs - given reservation session
 *
 *
 * @return
 * 		drmaa2_string - contact name associated with reservation session
 * 		NULL - if reservation session is invalid, drmaa2_lasterror_v
 * 				set to DRMAA2_INVALID_SESSION
 */
drmaa2_string drmaa2_rsession_get_contact(const drmaa2_rsession rs) {
	if(rs == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	ReservationSession *obj = reinterpret_cast<ReservationSession *>(rs);
	return (drmaa2_string)strdup(obj->getContact().c_str());
}

/**
 * @brief Get the session name of a given drmaa2 reservation session.
 *
 * @param[in]  rs - given reservation session
 *
 *
 * @return
 * 		drmaa2_string - session name associated with reservation session
 * 		NULL - if reservation session is invalid, drmaa2_lasterror_v
 * 				set to DRMAA2_INVALID_SESSION
 */
drmaa2_string drmaa2_rsession_get_session_name(const drmaa2_rsession rs) {
	if(rs == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	ReservationSession *obj = reinterpret_cast<ReservationSession *>(rs);
	return (drmaa2_string)strdup(obj->getSessionName().c_str());
}

/**
 * @brief Get the reservation object from a specific reservation session
 * 			 with the given reservation id.
 *
 * @param[in]  rs	     - given reservation session
 * @param[in]  reservationId - given reservation id
 *
 *
 * @return
 * 		drmaa2_r- reservation
 * 		NULL - if reservation session is invalid, drmaa2_lasterror_v
 * 				set to DRMAA2_INVALID_SESSION
 *			 - if reservation id is invalid, drmaa2_lasterror_v
 *			 	set to DRMAA2_INVALID_ARGUMENT
 */
drmaa2_r drmaa2_rsession_get_reservation(const drmaa2_rsession rs,
		const drmaa2_string reservationId) {
	if(rs == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	ReservationSession *obj = reinterpret_cast<ReservationSession *>(rs);
	const Reservation &res = obj->getReservation(string(reservationId));
	if(&res != NULL) {
		void *r = const_cast<Reservation*>(&res);
		return (drmaa2_r)r;
	} else
		return NULL;
}

/**
 * @brief Creates a reservation object in a specific reservation session
 * 		with the given reservation template. It also evaluates reservation
 * 		template for the mandatory attributes and sends the request to DRMS.
 *
 * @param[in]  rs	     - given reservation session
 * @param[in]  rt	     - given reservation template
 *
 *
 * @return
 * 		drmaa2_r- reservation
 * 		NULL - if reservation session is invalid, drmaa2_lasterror_v
 * 				set to DRMAA2_INVALID_SESSION
 *			 - if reservation template evaluation fails, drmaa2_lasterror_v
 *			 	set to DRMAA2_UNSUPPORTED_ATTRIBUTE
 *		     - if DRMS rejects reservation request, drmaa2_lasterror_v
 *		     	set to DRMAA2_DENIED_BY_DRMS
 *
 */

drmaa2_r drmaa2_rsession_request_reservation(const drmaa2_rsession rs,
		const drmaa2_rtemplate rt) {
	if (rs == NULL || rt == NULL){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}

	SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
	ReservationSession *reservationSession = reinterpret_cast<ReservationSession *>(rs);
	int size = 0;

	ReservationTemplate reservationTemplate;
	size = drmaa2_list_size(rt->candidateMachines);
	if(size > 0) {
		for(int i = 0 ; i < size ; i++){
			reservationTemplate.candidateMachines.push_back(
					string((char*)drmaa2_list_get(rt->candidateMachines, i)));
		}
	}
	reservationTemplate.duration = rt->duration;
	reservationTemplate.endTime = rt->endTime;
	if(rt->reservationName!=NULL){
		reservationTemplate.reservationName.assign(rt->reservationName);
	}
	reservationTemplate.startTime = rt->startTime;
	reservationTemplate.machineArch = (CpuArchitecture)rt->machineArch;
	reservationTemplate.machineOS = (OperatingSystem)rt->machineOS;
	reservationTemplate.maxSlots = rt->maxSlots;
	reservationTemplate.minPhysMemory = rt->minPhysMemory;
	reservationTemplate.minSlots = rt->minSlots;
	size = drmaa2_list_size(rt->usersACL);
	if(size > 0) {
		for(int i = 0 ; i < size ; i++){
			reservationTemplate.usersACL.push_back(
					string((char*)drmaa2_list_get(rt->usersACL, i)));
		}
	}
	try{
		const Reservation &r = reservationSession->requestReservation(reservationTemplate);
		void *res = const_cast<Reservation *>(&r);
		return (drmaa2_r)res;
	}catch(ImplementationSpecificException& ex){
		lasterror = DRMAA2_DENIED_BY_DRMS;
		return NULL;
	}
}

/**
 * @brief Get all the reservation objects from a specific reservation session.
 *
 * @param[in]  rs	     - given reservation session
 *
 *
 * @return
 * 		drmaa2_r_list - reservation interface list
 * 		NULL - if reservation session is invalid, drmaa2_lasterror_v
 * 				set to DRMAA2_INVALID_SESSION
 *
 */
drmaa2_r_list drmaa2_rsession_get_reservations(const drmaa2_rsession rs) {
	if (rs == NULL ){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
	ReservationSession *reservationSession = reinterpret_cast<ReservationSession *>(rs);
	ReservationList rL = reservationSession->getReservations();
	if(rL.size() > 0) {
		drmaa2_r_list rl = drmaa2_list_create(DRMAA2_RESERVATIONLIST, NULL);
		for(list<Reservation*>::const_iterator it = rL.begin(); it != rL.end(); ++it) {
			drmaa2_list_add(rl, (void *)const_cast<Reservation*>(*it));
		}
		return rl;
	}
	lasterror = DRMAA2_INVALID_SESSION;
	return NULL;
}
/**
 * @brief Get the reservation id from a given reservation object
 *
 * @param[in]  r	     - given reservation object
 *
 *
 * @return drmaa2_string
 *
 * @retval - reservation id
 *
 */
drmaa2_string drmaa2_r_get_id(const drmaa2_r r) {
	if(r == NULL ){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	Reservation *obj = reinterpret_cast<Reservation *>(r);
	return (drmaa2_string)strdup(obj->getReservationId().c_str());
}
/**
 * @brief Get the reservation session name from a given reservation object
 *
 * @param[in]  r	     - given reservation object
 *
 *
 * @return
 * 		drmaa2_string - reservation session name
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 *
 */
drmaa2_string drmaa2_r_get_session_name(const drmaa2_r r) {
	lasterror = DRMAA2_INVALID_ARGUMENT;
	return NULL;
}

/**
 * @brief Get the reservation template from a given reservation object
 *
 * @param[in]  r	     - given reservation object
 *
 *
 * @return
 * 		drmaa2_rtemplate- - reservation template
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 *
 */

drmaa2_rtemplate drmaa2_r_get_reservation_template(const drmaa2_r r) {
	if(r == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	Reservation *obj = reinterpret_cast<Reservation *>(r);
	const ReservationTemplate &rT = obj->getReservationTemplate();
	drmaa2_rtemplate rt = drmaa2_rtemplate_create();
	if(rt == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	if(rT.candidateMachines.size() > 0) {
		for(vector<string>::const_iterator it = rT.candidateMachines.begin(); it != rT.candidateMachines.end(); ++it) {
			drmaa2_list_add(rt->candidateMachines,strdup((*it).c_str()));
		}
	}
	rt->duration = rT.duration;
	rt->endTime = rT.endTime;
	rt->machineArch = (drmaa2_cpu)rT.machineArch;
	rt->machineOS = (drmaa2_os)rT.machineOS;
	rt->maxSlots = rT.maxSlots;
	rt->minPhysMemory = rT.minPhysMemory;
	rt->minSlots = rT.minSlots;
	if(rT.reservationName.length() > 0)
		rt->reservationName = strdup(rT.reservationName.c_str());
	rt->startTime = rT.startTime;
	if(rT.usersACL.size() > 0) {
		for(vector<string>::const_iterator it = rT.usersACL.begin(); it != rT.usersACL.end(); ++it) {
			drmaa2_list_add(rt->usersACL,strdup((*it).c_str()));
		}
	}
	return rt;
}

/**
 *
 * @brief Get the reservation info from a given reservation object
 *
 * @param[in]  r	     - given reservation object
 *
 *
 * @return
 * 		drmaa2_rinfo - reservation info object
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */
drmaa2_rinfo drmaa2_r_get_info(const drmaa2_r r) {
	if(r == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	Reservation *obj = reinterpret_cast<Reservation *>(r);
	const ReservationInfo &rI = obj->getInfo();
	drmaa2_rinfo ri = drmaa2_rinfo_create();
	if(rI.reservationId.length() > 0)
		ri->reservationId = strdup(rI.reservationId.c_str());
	if(rI.reservationName.length() > 0)
		ri->reservationName = strdup(rI.reservationName.c_str());
	ri->reservedEndTime = rI.reservedEndTime;
	if(rI.reservedMachines.size() > 0) {
		//TODO: Need to add
	}
	ri->reservedSlots = rI.reservedSlots;
	ri->reservedStartTime = rI.reservedStartTime;
	if(rI.usersACL.size() > 0) {
		for(set<string>::const_iterator it = rI.usersACL.begin(); it != rI.usersACL.end(); ++it) {
			drmaa2_list_add(ri->reservedMachines,strdup((*it).c_str()));
		}
	}
	return ri;
}

/**
 * @brief Terminate a given reservation
 *
 * @param[in]  r	     - given reservation object
 *
 *
 * @return
 * 		drmaa2_error - on success DRMAA2_SUCCESS
 * 		DRMAA2_DENIED_BY_DRMS if failed
 *
 */
drmaa2_error drmaa2_r_terminate(drmaa2_r r) {
	Reservation *obj = reinterpret_cast<Reservation *>(r);
	try {
		obj->terminate();
		return DRMAA2_SUCCESS;
	} catch (ImplementationSpecificException &ex) {
			return DRMAA2_INVALID_STATE;
	}
}

/**
 *  @brief  Gets the id from drmaa2 drmaa2_job array.
 *
 *  @param[in]	ja	-	pointer to drmaa2_job array
 *
 *  @return
 *  	drmaa2_string - job id of drmaa2_job array
 *  	NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */
drmaa2_string drmaa2_jarray_get_id(const drmaa2_jarray ja) {
	if(ja == NULL ){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	return (drmaa2_string)strdup(obj->getJobArrayId().c_str());
}

/**
 *  @brief  Returns the list of jobs in the drmaa2_job array structure.
 *
 *  @param[in]	ja	-	Pointer to drmaa2_job array structure
 *
 *  @return
 *  	drmaa2_j_list - gets list of drmaa2_jobs from drmaa2_jarray
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 */
drmaa2_j_list drmaa2_jarray_get_jobs(const drmaa2_jarray ja) {
	if(ja == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	JobList jL = obj->getJobs();
	if(jL.size() > 0) {
		drmaa2_list jl = drmaa2_list_create(DRMAA2_JOBLIST, NULL);
		for(list<Job*>::const_iterator it = jL.begin(); it != jL.end(); ++it) {
			drmaa2_list_add(jl, (*it));
		}
		return jl;
	}
	return NULL;
}

/**
 *  @brief  gets the session name in which the drmaa2_job array is submitted
 *
 *  @param[in]	ja	-	pointer to drmaa2_job array
 *
 *  @return
 *  	drmaa2_string - returns session name from drmaa2_job array
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 */
drmaa2_string drmaa2_jarray_get_session_name(const drmaa2_jarray ja) {
	lasterror = DRMAA2_UNSUPPORTED_OPERATION;
	return NULL;
}

/**
 * @brief	 Returns pointer of drmaa2_job template structure from Job Array.
 *
 *
 * @param[in]     ja	-    pointer to drmaa2_job structure.
 *
 * @return
 * 		drmaa2_jtemplate - drmaa2_jtemplate
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */

drmaa2_jtemplate drmaa2_jarray_get_jtemplate(const drmaa2_jarray ja) {
	if(ja == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	drmaa2_jtemplate jt = drmaa2_jtemplate_create();
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	const JobTemplate& jT = obj->getJobTemplate();
	if(jT.accountingId.length() > 0)
		jt->accountingId = (drmaa2_string)strdup(jT.accountingId.c_str());
	if(jT.args.size() > 0) {
		for(vector<string>::const_iterator it = jT.args.begin(); it != jT.args.end(); ++it) {
			drmaa2_list_add(jt->args, strdup((*it).c_str()));
		}
	}
	if(jT.candidateMachines.size() > 0) {
		for(vector<string>::const_iterator it = jT.candidateMachines.begin(); it != jT.candidateMachines.end(); ++it) {
			drmaa2_list_add(jt->candidateMachines, strdup((*it).c_str()));
		}
	}
	jt->deadlineTime = jT.deadlineTime;
	if(jT.email.size() > 0) {
		for(list<string>::const_iterator it = jT.email.begin(); it != jT.email.end(); ++it) {
			drmaa2_list_add(jt->email, strdup((*it).c_str()));
		}
	}
	jt->emailOnStarted = jT.emailOnStarted ? DRMAA2_TRUE : DRMAA2_FALSE;
	jt->emailOnTerminated = jT.emailOnTerminated ? DRMAA2_TRUE : DRMAA2_FALSE;
	if(jT.errorPath.length() > 0)
		jt->errorPath = (drmaa2_string)strdup(jT.errorPath.c_str());
	if(jT.inputPath.length() > 0)
		jt->inputPath = (drmaa2_string)strdup(jT.inputPath.c_str());
	if(jT.jobCategory.length() > 0)
		jt->jobCategory = (drmaa2_string)strdup(jT.jobCategory.c_str());
	if(jT.jobEnvironment.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.jobEnvironment.begin(); it!=jT.jobEnvironment.end(); ++it) {
			drmaa2_dict_set(jt->jobEnvironment, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	if(jT.jobName.length() > 0) {
		jt->jobName = (drmaa2_string)strdup(jT.jobName.c_str());
	}
	jt->joinFiles = jT.joinFiles ? DRMAA2_TRUE : DRMAA2_FALSE;
	jt->machineArch = (drmaa2_cpu)jT.machineArch;
	jt->machineOS = (drmaa2_os)jT.machineOS;
	jt->maxSlots = jT.maxSlots;
	jt->minPhysMemory = jT.minPhysMemory;
	jt->minSlots = jT.minSlots;
	if(jT.outputPath.length() > 0)
		jt->outputPath = (drmaa2_string)strdup(jT.outputPath.c_str());
	jt->priority = jT.priority;
	if(jT.queueName.length() > 0)
		jt->queueName = (drmaa2_string)strdup(jT.queueName.c_str());
	if(jT.remoteCommand.length() > 0)
		jt->remoteCommand = (drmaa2_string)strdup(jT.remoteCommand.c_str());
	jt->rerunnable = jT.rerunnable ? DRMAA2_TRUE : DRMAA2_FALSE;
	if(jT.reservationId.length() > 0)
		jt->reservationId = (drmaa2_string)strdup(jT.reservationId.c_str());
	if(jT.resourceLimits.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.resourceLimits.begin(); it!=jT.resourceLimits.end(); ++it) {
			drmaa2_dict_set(jt->resourceLimits, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	if(jT.stageInFiles.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.stageInFiles.begin(); it!=jT.stageInFiles.end(); ++it) {
			drmaa2_dict_set(jt->stageInFiles, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	if(jT.stageOutFiles.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.stageOutFiles.begin(); it!=jT.stageOutFiles.end(); ++it) {
			drmaa2_dict_set(jt->stageOutFiles, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	jt->startTime = jT.startTime;
	jt->submitAsHold = jT.submitAsHold ? DRMAA2_TRUE : DRMAA2_FALSE;
	if(jT.workingDirectory.length() > 0)
		jt->workingDirectory = (drmaa2_string)strdup(jT.workingDirectory.c_str());
	return jt;
}

/**
 *  @brief  suspends the drmaa2_job array
 *
 *  @param[in]	ja	-	pointer to drmaa2_job array
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 */

drmaa2_error drmaa2_jarray_suspend(drmaa2_jarray ja) {
	string subState;
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	try {
		obj->suspend();
		return DRMAA2_SUCCESS;
	} catch (InvalidStateException &ex) {
		return DRMAA2_INVALID_STATE;
	}
}

/**
 *  @brief  resumes the drmaa2_job array
 *
 *  @param[in]	ja	-	pointer to drmaa2_job array
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 */

drmaa2_error drmaa2_jarray_resume(drmaa2_jarray ja) {
	string subState;
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	try {
		obj->resume();
		return DRMAA2_SUCCESS;
	} catch (InvalidStateException &ex) {
		return DRMAA2_INVALID_STATE;
	}
}

/**
 *  @brief  holds the drmaa2_job array
 *
 *  @param[in]	ja	-	pointer to drmaa2_job array
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 */

drmaa2_error drmaa2_jarray_hold(drmaa2_jarray ja) {
	string subState;
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	try {
		obj->hold();
		return DRMAA2_SUCCESS;
	} catch (InvalidStateException &ex) {
		return DRMAA2_INVALID_STATE;
	}
}

/**
 *  @brief  releases the drmaa2_job array
 *
 *  @param[in]	ja	-	pointer to drmaa2_job array
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 *
 */

drmaa2_error drmaa2_jarray_release(drmaa2_jarray ja) {
	string subState;
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	try {
		obj->release();
		return DRMAA2_SUCCESS;
	} catch (InvalidStateException &ex) {
		return DRMAA2_INVALID_STATE;
	}
}

/**
 *  @brief  terminates the drmaa2_job array
 *
 *  @param[in]	ja	-	pointer to drmaa2_job array
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 *
 */

drmaa2_error drmaa2_jarray_terminate(drmaa2_jarray ja) {
	string subState;
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	try {
		obj->terminate();
		return DRMAA2_SUCCESS;
	} catch (InvalidStateException &ex) {
		return DRMAA2_INVALID_STATE;
	}
}

/**
 *  @brief  Clean up any data about the job array
 *
 *  @param[in]	ja	-	pointer to job array
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 *
 */
drmaa2_error drmaa2_jarray_reap(drmaa2_jarray ja) {
	JobArray *obj = reinterpret_cast<JobArray *>(ja);
	obj->reap();
	return DRMAA2_SUCCESS;
}

/**
 *  @brief  Returns the DRMS name of the drmaa2_job session
 *
 *  @param[in]	js	-	pointer to drmaa2_job session.
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 *
 */

drmaa2_string drmaa2_jsession_get_contact(const drmaa2_jsession js) {
	if(js == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobSession *obj = reinterpret_cast<JobSession *>(js);
	return (drmaa2_string)strdup(obj->getContact().c_str());
}

/**
 *  @brief  returns the session name from drmaa2_job session.
 *
 *  @param[in]	js	-	pointer to drmaa2_job session
 *
 *  @return
 *  	drmaa2_string - returns session name from drmaa2_jsession
 *  	NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */

drmaa2_string drmaa2_jsession_get_session_name(const drmaa2_jsession js) {
	if(js == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobSession *obj = reinterpret_cast<JobSession *>(js);
	return (drmaa2_string)strdup(obj->getSessionName().c_str());
}

/**
 *  @brief  returns the string list of drmaa2_job categories
 *  		in the drmaa2_job session
 *
 *  @param[in]	js	-	pointer to drmaa2_job session
 *
 *  @return
 *  	drmaa2_string_list - returns list of job categories
 *  					from drmaa2_jsession
 *  	NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 *
 */
drmaa2_string_list drmaa2_jsession_get_job_categories(
		const drmaa2_jsession js) {
	if (js == NULL)
		return NULL;
	drmaa2_list jc = drmaa2_list_create(DRMAA2_STRINGLIST, NULL);
	JobSession *jobSession = reinterpret_cast<JobSession *>(js);
	StringList jC = jobSession->getJobCategories();
	if(jC.size() > 0) {
		for(StringList::const_iterator it = jC.begin(); it != jC.end(); ++it) {
			drmaa2_list_add(jc, strdup((*it).c_str()));
		}
	}
	return jc;
}

/**
 *  @brief  returns list of jobs in the drmaa2_job session matching
 *  		the jobinfo filter
 *
 *  @param[in]	js	-	pointer to drmaa2_job session js
 *  @param[in]	filter	-	filter for getting jobs with specific conditions
 *
 *  @return
 *  	drmaa2_j_list - returns list of jobs matching the drmaa2_job
 *  				filter provided
 *  	NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */
drmaa2_j_list drmaa2_jsession_get_jobs(const drmaa2_jsession js,
		const drmaa2_jinfo filter) {
	if(js == NULL || filter == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobSession *jobSession = reinterpret_cast<JobSession *>(js);
	if (js == NULL)
		return NULL;
	else {
		JobInfo jobInfo;
		vector<string> allocatedMachines;
		for(int i = 0 ; i < drmaa2_list_size(filter->allocatedMachines); i++ ){
			allocatedMachines.push_back(string((char*)drmaa2_list_get(filter->allocatedMachines, i)));
		}
		jobInfo.allocatedMachines = allocatedMachines;
		if(filter->annotation != NULL){
			jobInfo.annotation = string(filter->annotation);
		}
		jobInfo.cpuTime = filter->cpuTime;
		jobInfo.dispatchTime = filter->dispatchTime;
		jobInfo.exitStatus = filter->exitStatus;
		jobInfo.finishTime = filter->finishTime;
		if(filter->jobId != NULL){
			jobInfo.jobId = string(filter->jobId);
		}
		if(filter->jobOwner!=NULL){
			jobInfo.jobOwner = string(filter->jobOwner);
		}
		JobState jobState;
		switch(filter->jobState){
			case DRMAA2_RUNNING: jobState = RUNNING; break;
			case DRMAA2_QUEUED: jobState = QUEUED; break;
			case DRMAA2_QUEUED_HELD: jobState = QUEUED_HELD; break;
			case DRMAA2_REQUEUED: jobState = REQUEUED; break;
			case DRMAA2_REQUEUED_HELD: jobState = REQUEUED_HELD; break;
			case DRMAA2_SUSPENDED: jobState = SUSPENDED; break;
			case DRMAA2_DONE: jobState = DONE	; break;
			default: jobState = UNDETERMINED;
		}
		jobInfo.jobState = jobState;
		if(filter->jobSubState!=NULL){
			jobInfo.jobSubState = string(filter->jobSubState);
		}
		if(filter->queueName!=NULL){
			jobInfo.queueName = string(filter->queueName);
		}
		jobInfo.slots = filter->slots;
		if(filter->submissionMachine!=NULL){
			jobInfo.submissionMachine.assign(filter->submissionMachine);
		}
		jobInfo.submissionTime = filter->submissionTime;
		if(filter->terminatingSignal!=NULL){
			jobInfo.terminatingSignal.assign(filter->terminatingSignal);
		}
		jobInfo.wallclockTime = filter->wallclockTime;
		JobList jobs = jobSession->getJobs(jobInfo);
		drmaa2_j_list j_list = drmaa2_list_create(DRMAA2_JOBLIST, NULL);
		for(list<Job*>::const_iterator iterator = jobs.begin(), end = jobs.end() ; iterator != end ; ++iterator ){
			drmaa2_list_add(j_list, (void*)*iterator);
		}
		return j_list;
	}
}

/**
 *  @brief  returns the specified job array from the drmaa2_job session
 *
 *  @param[in]	js	-	pointer to drmaa2_job session.
 *  @param[in]	jobarrayId	-	Job array id which needs to be retrieved
 *
 *  @return
 *  	drmaa2_jarray - returns pointer to drmaa2_jarray with specified
 *  				jobarrayId in the job session
 *  	NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */
drmaa2_jarray drmaa2_jsession_get_job_array(const drmaa2_jsession js,
		const drmaa2_string jobarrayId) {
	if(js == NULL || jobarrayId == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobSession *jobSession = reinterpret_cast<JobSession *>(js);
	void *jarray;
	jarray = const_cast<JobArray*>(&jobSession->getJobArray(jobarrayId));
	return (drmaa2_jarray)jarray;
}

/**
 *  @brief  runs job in the job session with the drmaa2_job template specified
 *
 *  @param[in]	js	-	pointer to drmaa2_job session
 *  @param[in]	jt	-	Job template that needs to be run
 *
 *  @return
 *  		drmaa2_j - returns pointer to job which is newly started
 *  					in the job session
 *  		NULL and sets last error to DRMAA2_INVALID_ARGUMENT
 *  					if any argument is invalid
 *  		NULL and sets last error to DRMAA2_INVALID_SESSION
 *  					if session name is invalid
 */
drmaa2_j drmaa2_jsession_run_job(const drmaa2_jsession js,
		const drmaa2_jtemplate jt) {
	char *tmp = NULL;
	int size = 0, i = 0;
	if(js == NULL || jt == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobSession *jobSession = reinterpret_cast<JobSession *>(js);

	/*conversion from drmaa2_jtemplate to JobTemplate*/
	string key, value, intermediate;
	JobTemplate jobTemplate;

	if(jt->accountingId!=NULL)
		jobTemplate.accountingId.assign(jt->accountingId);

	vector<string> slArgs;
	size = drmaa2_list_size(jt->args);
	for(i = 0 ; i < size ; ++i ){
		intermediate.assign((char*)drmaa2_list_get(jt->args, i));
		slArgs.push_back(intermediate);
	}
	jobTemplate.args = slArgs;

	if(drmaa2_list_size(jt->candidateMachines) > 0) {
		size = drmaa2_list_size(jt->candidateMachines);
		for(i = 0; i < size; i++) {
			jobTemplate.candidateMachines.push_back(string((char*)drmaa2_list_get(jt->candidateMachines, i)));
		}
	}

	list<string> slEmail;
	for(i = 0 ; i < drmaa2_list_size(jt->email) ; ++i ){
		intermediate.assign((char*)drmaa2_list_get(jt->email, i));
		slEmail.push_back(intermediate);
	}
	jobTemplate.email = slEmail;

	jobTemplate.emailOnStarted = jt->emailOnStarted == DRMAA2_TRUE ? true : false;

	jobTemplate.emailOnTerminated = jt->emailOnTerminated == DRMAA2_TRUE ? true : false;

	if(jt->errorPath!=NULL)
		jobTemplate.errorPath.assign(jt->errorPath);

	if(jt->inputPath!=NULL)
		jobTemplate.inputPath.assign(jt->inputPath);

	if(jt->jobCategory!=NULL)
		jobTemplate.jobCategory.assign(jt->jobCategory);

	drmaa2_list lEnv;
	lEnv = drmaa2_dict_list(jt->jobEnvironment);
	map<string, string> dJobEnvironment;
	size = drmaa2_list_size(lEnv);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lEnv, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->jobEnvironment,(char*)key.c_str()));
		dJobEnvironment.insert(std::pair<string,string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lEnv);
	jobTemplate.jobEnvironment = dJobEnvironment;

	if(jt->jobName!=NULL)
		jobTemplate.jobName.assign(jt->jobName);

	jobTemplate.joinFiles = jt->joinFiles == DRMAA2_TRUE ? true : false;

	jobTemplate.machineArch = (drmaa2::CpuArchitecture)jt->machineArch;
	jobTemplate.machineOS = (drmaa2::OperatingSystem)jt->machineOS;
	jobTemplate.maxSlots = jt->maxSlots;
	jobTemplate.minPhysMemory = jt->minPhysMemory;
	jobTemplate.minSlots = jt->minSlots;

	if(jt->outputPath!=NULL)
		jobTemplate.outputPath.assign(jt->outputPath);

	jobTemplate.priority = jt->priority;

	if(jt->queueName!=NULL)
		jobTemplate.queueName.assign(jt->queueName);

	if(jt->remoteCommand!=NULL)
		jobTemplate.remoteCommand.assign(jt->remoteCommand);

	jobTemplate.rerunnable = jt->rerunnable == DRMAA2_TRUE ? true : false;

	if(jt->reservationId!=NULL)
		jobTemplate.reservationId.assign(jt->reservationId);

	map<string, string> dResourceLimits;
	drmaa2_list lResource;
	lResource = drmaa2_dict_list(jt->resourceLimits);
	size = drmaa2_list_size(lResource);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lResource, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->resourceLimits,(char*)key.c_str()));
		dResourceLimits.insert(std::pair<string, string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lResource);
	jobTemplate.resourceLimits = dResourceLimits;

	map<string, string> dStageInFiles;
	drmaa2_list lStageIn;
	lStageIn = drmaa2_dict_list(jt->stageInFiles);
	size = drmaa2_list_size(lStageIn);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lStageIn, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->stageInFiles,(char*)key.c_str()));
		dStageInFiles.insert(std::pair<string, string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lStageIn);
	jobTemplate.stageInFiles = dStageInFiles;

	map<string, string> dStageOutFiles;
	drmaa2_list lStageOut;
	lStageOut = drmaa2_dict_list(jt->stageOutFiles);
	size = drmaa2_list_size(lStageOut);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lStageOut, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->stageOutFiles,(char*)key.c_str()));
		dStageOutFiles.insert(std::pair<string, string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lStageOut);
	jobTemplate.stageOutFiles = dStageOutFiles;

	jobTemplate.startTime = jt->startTime;
	jobTemplate.submitAsHold = jt->submitAsHold == DRMAA2_TRUE ? true : false;

	if(jt->workingDirectory!=NULL)
		jobTemplate.workingDirectory = jt->workingDirectory;

	try{
		const Job &j = jobSession->runJob(jobTemplate);
		void *job = const_cast<Job*>(&j);
		return (drmaa2_j)job;
	}catch(ImplementationSpecificException &ex){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
}

/**
 *  @brief  Write description of function here.
 *
 *  @param[in]	js	-	pointer to drmaa2_job session
 *  @param[in]	jt 	-	Job template to be considered or used
 *  @param[in]	begin_index -	Starting index of drmaa2_job array
 *  @param[in] 	end_index -	Ending index of drmaa2_job array
 *  @param[in]	step 	-	Increment between each jobs
 *  @param[in]	max_parallel -	Maximum parallel jobs to run
 *
 *  @return
 *  		drmaa2_jarray - returns pointer to drmaa2_jarray if successful
 *  		NULL and sets last error to DRMAA2_INVALID_ARGUMENT
 *  				if any argument is invalid
 *  		NULL and sets last error to DRMAA2_INTERNAL
 *  				if any internal error occurs
 */
drmaa2_jarray drmaa2_jsession_run_bulk_jobs(const drmaa2_jsession js,
		const drmaa2_jtemplate jt, const long long begin_index,
		const long long end_index, const long long step,
		const long long max_parallel) {
	char *tmp = NULL;
	int size = 0, i = 0;
	if(js == NULL || jt == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	JobSession *jobSession = reinterpret_cast<JobSession *>(js);

	/*conversion from drmaa2_jtemplate to JobTemplate*/
	string key, value, intermediate;
	JobTemplate jobTemplate;

	if(jt->accountingId!=NULL)
		jobTemplate.accountingId.assign(jt->accountingId);

	vector<string> slArgs;
	size = drmaa2_list_size(jt->args);
	for(i = 0 ; i < size ; ++i ){
		intermediate.assign((char*)drmaa2_list_get(jt->args, i));
		slArgs.push_back(intermediate);
	}
	jobTemplate.args = slArgs;

	if(drmaa2_list_size(jt->candidateMachines) > 0) {
		size = drmaa2_list_size(jt->candidateMachines);
		for(i = 0; i < size; i++) {
			jobTemplate.candidateMachines.push_back(string((char*)drmaa2_list_get(jt->candidateMachines, i)));
		}
	}

	list<string> slEmail;
	for(i = 0 ; i < drmaa2_list_size(jt->email) ; ++i ){
		intermediate.assign((char*)drmaa2_list_get(jt->email, i));
		slEmail.push_back(intermediate);
	}
	jobTemplate.email = slEmail;

	jobTemplate.emailOnStarted = jt->emailOnStarted == DRMAA2_TRUE ? true : false;

	jobTemplate.emailOnTerminated = jt->emailOnTerminated == DRMAA2_TRUE ? true : false;

	if(jt->errorPath!=NULL)
		jobTemplate.errorPath.assign(jt->errorPath);

	if(jt->inputPath!=NULL)
		jobTemplate.inputPath.assign(jt->inputPath);

	if(jt->jobCategory!=NULL)
		jobTemplate.jobCategory.assign(jt->jobCategory);

	drmaa2_list lEnv;
	lEnv = drmaa2_dict_list(jt->jobEnvironment);
	map<string, string> dJobEnvironment;
	size = drmaa2_list_size(lEnv);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lEnv, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->jobEnvironment,(char*)key.c_str()));
		dJobEnvironment.insert(std::pair<string,string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lEnv);
	jobTemplate.jobEnvironment = dJobEnvironment;

	if(jt->jobName!=NULL)
		jobTemplate.jobName.assign(jt->jobName);

	jobTemplate.joinFiles = jt->joinFiles == DRMAA2_TRUE ? true : false;

	jobTemplate.machineArch = (drmaa2::CpuArchitecture)jt->machineArch;
	jobTemplate.machineOS = (drmaa2::OperatingSystem)jt->machineOS;
	jobTemplate.maxSlots = jt->maxSlots;
	jobTemplate.minPhysMemory = jt->minPhysMemory;
	jobTemplate.minSlots = jt->minSlots;

	if(jt->outputPath!=NULL)
		jobTemplate.outputPath.assign(jt->outputPath);

	jobTemplate.priority = jt->priority;

	if(jt->queueName!=NULL)
		jobTemplate.queueName.assign(jt->queueName);

	if(jt->remoteCommand!=NULL)
		jobTemplate.remoteCommand.assign(jt->remoteCommand);

	jobTemplate.rerunnable = jt->rerunnable == DRMAA2_TRUE ? true : false;

	if(jt->reservationId!=NULL)
		jobTemplate.reservationId.assign(jt->reservationId);

	map<string, string> dResourceLimits;
	drmaa2_list lResource;
	lResource = drmaa2_dict_list(jt->resourceLimits);
	size = drmaa2_list_size(lResource);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lResource, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->resourceLimits,(char*)key.c_str()));
		dResourceLimits.insert(std::pair<string, string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lResource);
	jobTemplate.resourceLimits = dResourceLimits;

	map<string, string> dStageInFiles;
	drmaa2_list lStageIn;
	lStageIn = drmaa2_dict_list(jt->stageInFiles);
	size = drmaa2_list_size(lStageIn);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lStageIn, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->stageInFiles,(char*)key.c_str()));
		dStageInFiles.insert(std::pair<string, string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lStageIn);
	jobTemplate.stageInFiles = dStageInFiles;

	map<string, string> dStageOutFiles;
	drmaa2_list lStageOut;
	lStageOut = drmaa2_dict_list(jt->stageOutFiles);
	size = drmaa2_list_size(lStageOut);
	for(i = 0 ; i < size ; i++){
		tmp = (char*)drmaa2_list_get(lStageOut, i);
		key.assign(tmp);
		value.assign(drmaa2_dict_get(jt->stageOutFiles,(char*)key.c_str()));
		dStageOutFiles.insert(std::pair<string, string>(key, value));
		free(tmp);
	}
	drmaa2_list_free(&lStageOut);
	jobTemplate.stageOutFiles = dStageOutFiles;

	jobTemplate.startTime = jt->startTime;
	jobTemplate.submitAsHold = jt->submitAsHold == DRMAA2_TRUE ? true : false;

	if(jt->workingDirectory!=NULL)
		jobTemplate.workingDirectory = jt->workingDirectory;

	try{
		const JobArray &ja = jobSession->runBulkJobs(jobTemplate, begin_index, end_index, step, max_parallel);
		void *jobArray = const_cast<JobArray*>(&ja);
		return (drmaa2_jarray)jobArray;
	}catch(ImplementationSpecificException &ex){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
}

/**
 *  @brief  blocks till the drmaa2_job in the drmaa2_job session started
 *  		or till the time out
 *
 *  @param[in]	js -	pointer to drmaa2_job session
 *  @param[in]	l	-	list of drmaa2_jobs
 *  @param[in]	timeout	-	timeout to wait if event has not occurred
 *
 *  @return
 *  		drmaa2_j - returns pointer to drmaa2_job which started
 *  		NULL and last error is set to DRMAA2_TIMEOUT if timeout happens
 */
drmaa2_j drmaa2_jsession_wait_any_started(const drmaa2_jsession js,
		const drmaa2_j_list l, const time_t timeout) {
	//TODO Add Code here
	return NULL;
}

/**
 *  @brief  blocks till the drmaa2_job in the drmaa2_job session
 *  		ended or till the time out
 *
 *  @param[in]	js -	pointer to drmaa2_job session
 *  @param[in]	l  - list of drmaa2_jobs
 *  @param[in]	timeout - timeout to wait if event has not occurs

 *  @return
 *  		drmaa2_j - returns pointer to drmaa2_job which terminated
 *  		NULL and last error is set to DRMAA2_TIMEOUT if timeout happens
 */
drmaa2_j drmaa2_jsession_wait_any_terminated(const drmaa2_jsession js,
		const drmaa2_j_list l, const time_t timeout) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief  Returns newly allocated drmaa2_job id string
 *
 * @param[in]	j	-	pointer to drmaa2_job structure.
 *
 * @return
 * 		drmaa2_string - Gets the job id from drmaa2_job
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 *
 */

drmaa2_string drmaa2_j_get_id(const drmaa2_j j) {
	if(j == NULL ){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	Job *obj = reinterpret_cast<Job *>(j);
	return (drmaa2_string)strdup(obj->getJobId().c_str());
}

/**
 * @brief  Returns newly allocated session name string
 *
 * @param[in]     j	-    pointer to drmaa2_job structure.
 *
 * @return
 * 		drmaa2_string - gets the session name from drmaa2_session
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */

drmaa2_string drmaa2_j_get_session_name(const drmaa2_j j) {
	lasterror = DRMAA2_UNSUPPORTED_OPERATION;
	return NULL;
}

/**
 * @brief  Returns pointer of drmaa2_job template structure from Job.
 *
 *
 * @param[in]     j	-    pointer to drmaa2_job structure.
 *
 * @return
 * 		drmaa2_string - gets the session name from drmaa2_session
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */
drmaa2_jtemplate drmaa2_j_get_jtemplate(const drmaa2_j j) {
	if(j == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	drmaa2_jtemplate jt = drmaa2_jtemplate_create();
	Job *obj = reinterpret_cast<Job *>(j);
	const JobTemplate& jT = obj->getJobTemplate();
	if(jT.accountingId.length() > 0)
		jt->accountingId = (drmaa2_string)strdup(jT.accountingId.c_str());
	if(jT.args.size() > 0) {
		for(vector<string>::const_iterator it = jT.args.begin(); it != jT.args.end(); ++it) {
			drmaa2_list_add(jt->args, strdup((*it).c_str()));
		}
	}
	if(jT.candidateMachines.size() > 0) {
		for(vector<string>::const_iterator it = jT.candidateMachines.begin(); it != jT.candidateMachines.end(); ++it) {
			drmaa2_list_add(jt->candidateMachines, strdup((*it).c_str()));
		}
	}
	jt->deadlineTime = jT.deadlineTime;
	if(jT.email.size() > 0) {
		for(list<string>::const_iterator it = jT.email.begin(); it != jT.email.end(); ++it) {
			drmaa2_list_add(jt->email, strdup((*it).c_str()));
		}
	}
	jt->emailOnStarted = jT.emailOnStarted ? DRMAA2_TRUE : DRMAA2_FALSE;
	jt->emailOnTerminated = jT.emailOnTerminated ? DRMAA2_TRUE : DRMAA2_FALSE;
	if(jT.errorPath.length() > 0)
		jt->errorPath = (drmaa2_string)strdup(jT.errorPath.c_str());
	if(jT.inputPath.length() > 0)
		jt->inputPath = (drmaa2_string)strdup(jT.inputPath.c_str());
	if(jT.jobCategory.length() > 0)
		jt->jobCategory = (drmaa2_string)strdup(jT.jobCategory.c_str());
	if(jT.jobEnvironment.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.jobEnvironment.begin(); it!=jT.jobEnvironment.end(); ++it) {
			drmaa2_dict_set(jt->jobEnvironment, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	if(jT.jobName.length() > 0) {
		jt->jobName = (drmaa2_string)strdup(jT.jobName.c_str());
	}
	jt->joinFiles = jT.joinFiles ? DRMAA2_TRUE : DRMAA2_FALSE;
	jt->machineArch = (drmaa2_cpu)jT.machineArch;
	jt->machineOS = (drmaa2_os)jT.machineOS;
	jt->maxSlots = jT.maxSlots;
	jt->minPhysMemory = jT.minPhysMemory;
	jt->minSlots = jT.minSlots;
	if(jT.outputPath.length() > 0)
		jt->outputPath = (drmaa2_string)strdup(jT.outputPath.c_str());
	jt->priority = jT.priority;
	if(jT.queueName.length() > 0)
		jt->queueName = (drmaa2_string)strdup(jT.queueName.c_str());
	if(jT.remoteCommand.length() > 0)
		jt->remoteCommand = (drmaa2_string)strdup(jT.remoteCommand.c_str());
	jt->rerunnable = jT.rerunnable ? DRMAA2_TRUE : DRMAA2_FALSE;
	if(jT.reservationId.length() > 0)
		jt->reservationId = (drmaa2_string)strdup(jT.reservationId.c_str());
	if(jT.resourceLimits.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.resourceLimits.begin(); it!=jT.resourceLimits.end(); ++it) {
			drmaa2_dict_set(jt->resourceLimits, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	if(jT.stageInFiles.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.stageInFiles.begin(); it!=jT.stageInFiles.end(); ++it) {
			drmaa2_dict_set(jt->stageInFiles, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	if(jT.stageOutFiles.size() > 0) {
		for (std::map<string,string>::const_iterator it=jT.stageOutFiles.begin(); it!=jT.stageOutFiles.end(); ++it) {
			drmaa2_dict_set(jt->stageOutFiles, strdup(it->first.c_str()), strdup(it->second.c_str()));
		}
	}
	jt->startTime = jT.startTime;
	jt->submitAsHold = jT.submitAsHold ? DRMAA2_TRUE : DRMAA2_FALSE;
	if(jT.workingDirectory.length() > 0)
		jt->workingDirectory = (drmaa2_string)strdup(jT.workingDirectory.c_str());
	return jt;
}

/**
 * @brief  Suspends the drmaa2_job in drmaa2_j structure.
 *
 *
 * @param[in]     j	-	pointer to drmaa2_job structure.
 *
 * @return
 * 		drmaa2_error - Returns DRMAA2_SUCCESS on success
 * 	   	DRMAA2_INVALID_STATE if job state is invalid for suspend
 */

drmaa2_error drmaa2_j_suspend(drmaa2_j j) {
	string subState;
	Job *obj = reinterpret_cast<Job *>(j);
	obj->suspend();
	const JobState &jState = obj->getState(subState);
	if(jState == SUSPENDED)
		return DRMAA2_SUCCESS;
	else
		return DRMAA2_INVALID_STATE;
}

/**
 * @brief  Resumes the drmaa2_job in drmaa2_j structure.
 *
 *
 * @param[in]     j	-	pointer to drmaa2_job structure.
 *
 *
 * @return
 * 		drmaa2_error - Returns DRMAA2_SUCCESS on success
 * 	   	DRMAA2_INVALID_STATE if job state is invalid for resume
 */

drmaa2_error drmaa2_j_resume(drmaa2_j j) {
	string subState;
	Job *obj = reinterpret_cast<Job *>(j);
	obj->resume();
	const JobState &jState = obj->getState(subState);
	if(jState == RUNNING)
		return DRMAA2_SUCCESS;
	else
		return DRMAA2_INVALID_STATE;
}

/**
 * @brief  Holds the drmaa2_job in drmaa2_j structure.
 *
 *
 * @param[in]    	j	-	pointer to drmaa2_job structure.
 *
 *
 * @return
 * 		drmaa2_error - Returns DRMAA2_SUCCESS on success
 * 		DRMAA2_INVALID_STATE if job state is invalid for hold
 */

drmaa2_error drmaa2_j_hold(drmaa2_j j) {
	string subState;
	Job *obj = reinterpret_cast<Job *>(j);
	obj->hold();
	const JobState &jState = obj->getState(subState);
	if(jState == QUEUED_HELD || jState == REQUEUED_HELD)
		return DRMAA2_SUCCESS;
	else
		return DRMAA2_INVALID_STATE;
}

/**
 * @brief  Releases the drmaa2_job in drmaa2_j structure.
 *
 *
 * @param[in]    	j	-	pointer to drmaa2_job structure.
 *
 *
 * @return
 * 		drmaa2_error - Returns DRMAA2_SUCCESS on success
 * 		DRMAA2_INVALID_STATE if job state is invalid for release
 */

drmaa2_error drmaa2_j_release(drmaa2_j j) {
	string subState;
	Job *obj = reinterpret_cast<Job *>(j);
	obj->release();
	const JobState &jState = obj->getState(subState);
	if(jState == RUNNING)
		return DRMAA2_SUCCESS;
	else
		return DRMAA2_INVALID_STATE;
}

/**
 * @brief  Terminates the drmaa2_job in drmaa2_j structure.
 *
 *
 * @param[in]    	j	-	pointer to drmaa2_job structure.
 *
 *
 * @return
 * 		drmaa2_error - Returns DRMAA2_SUCCESS on success
 * 		DRMAA2_INVALID_STATE if job state is invalid for terminate
 *
 */

drmaa2_error drmaa2_j_terminate(drmaa2_j j) {
	string subState;
	Job *obj = reinterpret_cast<Job *>(j);
	obj->terminate();
	const JobState &jState = obj->getState(subState);
	if(jState == DONE)
		return DRMAA2_SUCCESS;
	else
		return DRMAA2_INVALID_STATE;
}

/**
 *  @brief  Clean up any data about the job
 *
 *  @param[in]	j -	pointer to drmaa2_job
 *
 *  @return
 *  	DRMAA2_SUCCESS if successful
 *  	drmaa2_error if fails
 *
 */
drmaa2_error drmaa2_j_reap(drmaa2_j j) {
	Job *obj = reinterpret_cast<Job *>(j);
	obj->reap();
	return DRMAA2_SUCCESS;
}

/**
 * @brief  Gets drmaa2_job state from the drmaa2_job in drmaa2_j structure.
 *
 *
 * @param[in]     j	-	pointer to drmaa2_job structure.
 * @param[in]    substate	-	state of the drmaa2_job in drmaa2_j structure
 *
 *
 * @return
 * 		drmaa2_jstate - returns current drmaa2_job state
 */
drmaa2_jstate drmaa2_j_get_state(const drmaa2_j j, drmaa2_string * substate) {
	string subState;
	Job *obj = reinterpret_cast<Job *>(j);
	const JobState &jState = obj->getState(subState);
	*substate = strdup(subState.c_str());
	return (drmaa2_jstate)jState;
}

/**
 * @brief  Gets drmaa2_job info structure from the drmaa2_job
 * 			in drmaa2_j structure.
 *
 *
 * @param[in]	    j	-	pointer to drmaa2_job structure.
 *
 * @return
 * 		drmaa2_jinfo - returns drmaa2_jinfo if successful
 * 		NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */
drmaa2_jinfo drmaa2_j_get_info(const drmaa2_j j) {
	if(j == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	drmaa2_jinfo ji = drmaa2_jinfo_create();
	Job *obj = reinterpret_cast<Job *>(j);
	const JobInfo& jI = obj->getJobInfo();
	if(jI.allocatedMachines.size() > 0) {
		for(vector<string>::const_iterator it = jI.allocatedMachines.begin(); it != jI.allocatedMachines.end(); ++it) {
			drmaa2_list_add(ji->allocatedMachines, strdup((*it).c_str()));
		}
	}
	if(jI.annotation.length() > 0)
		ji->annotation = (drmaa2_string)strdup(jI.annotation.c_str());
	ji->cpuTime = jI.cpuTime;
	ji->dispatchTime = jI.dispatchTime;
	ji->exitStatus = jI.exitStatus;
	ji->finishTime = jI.finishTime;
	if(jI.jobId.length() > 0)
		ji->jobId = (drmaa2_string)strdup(jI.jobId.c_str());
	if(jI.jobOwner.length() > 0)
		ji->jobOwner = (drmaa2_string)strdup(jI.jobOwner.c_str());
	ji->jobState = (drmaa2_jstate)jI.jobState;
	if(jI.jobSubState.length() > 0)
		ji->jobSubState = (drmaa2_string)strdup(jI.jobSubState.c_str());
	if(jI.queueName.length() > 0)
		ji->queueName = (drmaa2_string)strdup(jI.queueName.c_str());
	ji->slots = jI.slots;
	if(jI.submissionMachine.length() > 0)
		ji->submissionMachine = (drmaa2_string)strdup(jI.submissionMachine.c_str());
	ji->submissionTime = jI.submissionTime;
	if(jI.terminatingSignal.length() > 0)
		ji->terminatingSignal = (drmaa2_string)strdup(jI.terminatingSignal.c_str());
	ji->wallclockTime = jI.wallclockTime;
	return ji;
}

/**
 * @brief  Blocks till the drmaa2_job is started or till timeout expires
 *
 *
 * @param[in]     j	-	pointer to drmaa2_job structure.
 * @param[in]     timeout	-	Timeout in time_t format.
 *
 * @return
 * 		drmaa2_error - returns DRMAA2_SUCCESS if successful
 * 		DRMAA2_TIMEOUT if timeout happened
 */
drmaa2_error drmaa2_j_wait_started(const drmaa2_j j, const time_t timeout) {
	//TODO Add Code here
	return DRMAA2_SUCCESS;
}

/**
 * @brief  Blocks till the drmaa2_job is terminated or till timeout expires
 *
 *
 * @param[in]     j	-	pointer to drmaa2_job structure.
 * @param[in]     timeout	-	Timeout in time_t format.
 *
 * @return
 * 		drmaa2_error - returns DRMAA2_SUCCESS if successful
 * 		DRMAA2_TIMEOUT if timeout happened
 */
drmaa2_error drmaa2_j_wait_terminated(const drmaa2_j j, const time_t timeout) {
	//TODO Add Code here
	return DRMAA2_SUCCESS;
}

/**
 *  @brief  Returns the list of all advance reservations visible for
 *  		the user running the DRMAA-based application. The list or
 *  		number of jobs returned should  adhere to DRMS policies.
 *          Ex: The list might be limited to contain only the reservations
 *          seen by the DRMAA2 user.
 *
 *  @param[in]	ms	     pointer to drmaa2_msession created earlier
 *
 *  @return
 *  	drmaa2_j_list if succeeds
 * 		NULL if fails
 */
drmaa2_r_list drmaa2_msession_get_all_reservations(const drmaa2_msession ms) {
	if(ms == NULL){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
	MonitoringSession *mS = reinterpret_cast<MonitoringSession*>(ms);
	const ReservationList rL = mS->getAllReservations();
	drmaa2_r_list r_list = drmaa2_list_create(DRMAA2_RESERVATIONLIST, NULL);
	if(rL.size() > 0) {
		for(ReservationList::const_iterator it = rL.begin(); it != rL.end() ; ++it ){
			drmaa2_list_add(r_list, (void*)(*it));
		}
		return r_list;
	} else {
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
}
/**
 *  @brief  Returns the list of all jobs i.e. DRMAA2 jobs and the jobs
 *  		submitted out of DRMAA2 library, Also, The list or number of
 *  		jobs returned should  adhere to DRMS policies.
 *          Ex: The list might be limited to contain only the jobs seen by
 *          the DRMAA2 user.
 *
 *  @param[in]	ms	-	     pointer to drmaa2_msession created earlier
 *  @param[in]	filter -	 of type drmaa2_jinfo (through which we can query
 *  						jobs per jobstate, job owner and per queue etc

 *  @return
 *  	drmaa2_j_list if succeeds
 *  	NULL if fails
 */
drmaa2_j_list drmaa2_msession_get_all_jobs(const drmaa2_msession ms,
		const drmaa2_jinfo filter) {
	if(ms == NULL){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
	int size;
	JobInfo jInfo;
	MonitoringSession *mS = reinterpret_cast<MonitoringSession*>(ms);
	size = filter->allocatedMachines->size;
	if(size > 0) {
		for(int i = 0; i < size; i++) {
			SlotInfo sInfo;
			drmaa2_slotinfo s = (drmaa2_slotinfo)drmaa2_list_get(filter->allocatedMachines, i);
			jInfo.allocatedMachines.push_back(string(s->machineName));
		}
	}
	if(filter->annotation)
		jInfo.annotation.assign(filter->annotation);
	jInfo.cpuTime = filter->cpuTime;
	jInfo.dispatchTime = filter->dispatchTime;
	jInfo.exitStatus = filter->exitStatus;
	jInfo.finishTime = filter->finishTime;
	if(filter->jobId)
		jInfo.jobId.assign(filter->jobId);
	if(filter->jobOwner)
		jInfo.jobOwner.assign(filter->jobOwner);
	jInfo.jobState = (JobState)filter->jobState;
	if(filter->jobSubState)
		jInfo.jobSubState.assign(filter->jobSubState);
	if(filter->queueName)
		jInfo.queueName.assign(filter->queueName);
	jInfo.slots = filter->slots;
	if(filter->submissionMachine)
		jInfo.submissionMachine.assign(filter->submissionMachine);
	jInfo.submissionTime = filter->submissionTime;
	if(filter->terminatingSignal)
		jInfo.terminatingSignal.assign(filter->terminatingSignal);
	jInfo.wallclockTime = filter->wallclockTime;
	const JobList jL = mS->getAllJobs(jInfo);
	drmaa2_r_list j_list = drmaa2_list_create(DRMAA2_JOBLIST, NULL);
	if(jL.size() > 0) {
		for(JobList::const_iterator it = jL.begin(); it != jL.end() ; ++it ){
			drmaa2_list_add(j_list, (void*)(*it));
		}
		return j_list;
	} else {
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
}

/**
 *  @brief  Returns list of queues available for submitting jobs in
 *  		the DRMS system.
 *
 *  @param[in]	ms	- pointer to drmaa2_msession created earlier
 *  @param[in]	names  -  a list of queue names acts as filter i.e.
 *  					 we fetch info of only these queues.

 *  @return
 *  	drmaa2_queueinfo_list if succeeds
 * 		NULL if fails.
 */
drmaa2_queueinfo_list drmaa2_msession_get_all_queues(const drmaa2_msession ms,
		const drmaa2_string_list names) {
	if(ms == NULL){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
	MonitoringSession *mS = reinterpret_cast<MonitoringSession*>(ms);
	list<string> gList;
	int size = drmaa2_list_size(names);
	if(size > 0) {
		for(int i = 0; i < size; i++)
		gList.push_back(string((char *)drmaa2_list_get(names, i)));
	}
	const QueueInfoList qL = mS->getAllQueues(gList);
	drmaa2_queueinfo_list q_list = drmaa2_list_create(DRMAA2_QUEUEINFOLIST, NULL);
	if(qL.size() > 0) {
		for(QueueInfoList::const_iterator it = qL.begin(); it != qL.end() ; ++it ){
			try {
				drmaa2_queueinfo qi = new drmaa2_queueinfo_s();
				if((*it).name.length() > 0) {
					qi->name = strdup((char *)(*it).name.c_str());
					drmaa2_list_add(q_list, qi);
				}
			} catch (bad_alloc &ex) {
				drmaa2_list_free(&q_list);
				lasterror = DRMAA2_OUT_OF_RESOURCE;
				return NULL;
			}
		}
		return q_list;
	}else {
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
}

/**
 *  @brief  Returns list of machines or hosts available for submitting jobs in
 *  		the DRMS system.
 *
 *  @param[in]	ms -	pointer to drmaa2_msession created earlier
 *  @param[in]	names -  a list of queue names acts as filter i.e. we fetch info
 *  					of only these queues.

 *  @return
 *  	drmaa2_machineinfo_list if succeeds
 * 		NULL if fails.
 */
drmaa2_machineinfo_list drmaa2_msession_get_all_machines(
		const drmaa2_msession ms, const drmaa2_string_list names) {
	if(ms == NULL){
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
	MonitoringSession *mS = reinterpret_cast<MonitoringSession*>(ms);
	list<string> mList;
	int size = drmaa2_list_size(names);
	if(size > 0) {
		for(int i = 0; i < size; i++)
		mList.push_back(string((char *)drmaa2_list_get(names, i)));
	}
	const MachineInfoList mL = mS->getAllMachines(mList);
	drmaa2_queueinfo_list m_list = drmaa2_list_create(DRMAA2_MACHINEINFOLIST, NULL);
	if(mL.size() > 0) {
		for(MachineInfoList::const_iterator it = mL.begin(); it != mL.end() ; ++it ){
			try {
				drmaa2_machineinfo mi = new drmaa2_machineinfo_s();
				mi->available = (*it).available ? DRMAA2_TRUE : DRMAA2_FALSE;
				mi->coresPerSocket = (*it).coresPerSocket;
				mi->load = (*it).load;
				mi->machineArch = (drmaa2_cpu)(*it).machineArch;
				mi->machineOS = (drmaa2_os)(*it).machineOS;
				if((*it).machineOSVersion.minor.length() > 0)
					mi->machineOSVersion->minor = strdup((char*)(*it).machineOSVersion.minor.c_str());
				if((*it).machineOSVersion.major.length() > 0)
					mi->machineOSVersion->major = strdup((char*)(*it).machineOSVersion.major.c_str());
				if((*it).name.length() > 0)
					mi->name = strdup((char*)(*it).name.c_str());
				mi->physMemory = (*it).physMemory;
				mi->sockets = (*it).sockets;
				mi->threadsPerCore = (*it).threadsPerCore;
				mi->virtMemory = (*it).virtMemory;
				drmaa2_list_add(m_list, mi);
			} catch (bad_alloc &ex) {
				drmaa2_list_free(&m_list);
				lasterror = DRMAA2_OUT_OF_RESOURCE;
				return NULL;
			}
		}
		return m_list;
	} else {
		lasterror = DRMAA2_INVALID_SESSION;
		return NULL;
	}
}

/**
 *  @brief  returns the DRM system name
 *
 *  @param[in]	-  None

 *  @return drmaa2_string
 */
drmaa2_string drmaa2_get_drms_name(void) {
	return (strdup( drmaa2::Singleton<drmaa2::SessionManager, drmaa2::SessionManagerImpl>::getInstance()->getDrmsName().c_str()));
}

/**
 *  @brief  returns the DRM system version
 *
 *  @param[in]	-  None

 *  @return drmaa2_version
 */
drmaa2_version drmaa2_get_drms_version(void) {
	const Version V = drmaa2::Singleton<drmaa2::SessionManager, drmaa2::SessionManagerImpl>::getInstance()->getDrmsVersion();
	try {
		drmaa2_version v = new drmaa2_version_s();
		v->minor = strdup(V.minor.c_str());
		v->major = strdup(V.major.c_str());
		return v;
	} catch (bad_alloc &ex) {
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		return NULL;
	}
}

/**
 *  @brief  returns the DRM application name
 *
 *  @param[in]	-  None
 *
 *  @return drmaa2_string
 */
drmaa2_string drmaa2_get_drmaa_name(void) {
	return (strdup( drmaa2::Singleton<drmaa2::SessionManager, drmaa2::SessionManagerImpl>::getInstance()->getDrmaaName().c_str()));
}

/**
 *  @brief  returns the DRM application version
 *
 *  @param[in]	-  None

 *  @return drmaa2_version
 */
drmaa2_version drmaa2_get_drmaa_version(void) {
	const Version V = drmaa2::Singleton<drmaa2::SessionManager, drmaa2::SessionManagerImpl>::getInstance()->getDrmaaVersion();
	try {
		drmaa2_version v = new drmaa2_version_s();
		v->minor = strdup(V.minor.c_str());
		v->major = strdup(V.major.c_str());
		return v;
	} catch (bad_alloc &ex) {
		lasterror = DRMAA2_OUT_OF_RESOURCE;
		return NULL;
	}
}

/**
 *  @brief  returns true if drmaa supports the provided drmaa2_capability
 *
 *  @param[in]	c	-	drmaa2_capability enum value

 *  @return drmaa2_bool
 */
drmaa2_bool drmaa2_supports(const drmaa2_capability c) {
	//TODO Add Code here
	return DRMAA2_TRUE;

}

/**
 *  @brief  Creates a drmaa2 job session for a given name and contact

 *
 *  @param[in]	session_name - Name of the session .
 *  @param[in]	contact - DRMS name.
 *  @return
 *  	drmaa2_jsession if succeeds
 *      NULL if fails and sets drmaa2_lasterror_v to DRMAA2_INVALID_ARGUMENT error.
 */
drmaa2_jsession drmaa2_create_jsession(const char *session_name,
		const char *contact) {
	if(session_name == NULL || contact == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
	try {
		const JobSession &jobSessionObj_ = sessionManagerObj_->createJobSession(string(session_name), string(contact));
		return (drmaa2_jsession)&jobSessionObj_;
	} catch(InvalidArgumentException& ex) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
}

/**
 *  @brief  Creates a drmaa2 reservation session for a given name and contact
 *
 *  @param[in]	session_name - Name of the session.
 *  @param[in]	contact - DRMS name.

 *  @return
 *  	drmaa2_jsession if succeeds
 *      NULL if fails and sets DRMAA2_INVALID_ARGUMENT error.
 */
drmaa2_rsession drmaa2_create_rsession(const char *session_name,
		const char *contact) {
	if(session_name == NULL || contact == NULL) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
	try {
		const ReservationSession &reservationSessionObj_ = sessionManagerObj_->createReservationSession(string(session_name), string(contact));
		return (drmaa2_rsession)&reservationSessionObj_;
	} catch(InvalidArgumentException& ex) {
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
}

/**
 *  @brief  Opens an already existing drmaa2_job session
 *
 *  @param[in]  session_name - Session name of the drmaa2_job session which
 *  							needs to be opened

 *  @return
 *  	drmaa2_jsession - returns pointer to drmaa2_jsession for a session
 *  						which is already present
 *  	NULL - if fails and DRMAA2_INVALID_ARGUMENT error is set
 *
 */

drmaa2_jsession drmaa2_open_jsession(const char *session_name) {
	if(session_name == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
	try{
		const JobSession &jobSessionObj_ = sessionManagerObj_->openJobSession(string(session_name));
		return (drmaa2_jsession)&jobSessionObj_;
	}catch(exception &ex){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
}

/**
 *  @brief  Opens the previously created session.
 *
 *  @param[in]	session_name - Name of the session.

 *  @return
 *  	drmaa2_rsession if a session exists
 *  	NULL if sets drmaa2_lasterror_v to DRMAA2_INVALID_ARGUMENT error.
 */
drmaa2_rsession drmaa2_open_rsession(const char *session_name) {
	if(session_name == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
	try{
		const ReservationSession &reservationSessionObj_ = sessionManagerObj_->openReservationSession(string(session_name));
		return (drmaa2_rsession)&reservationSessionObj_;
	}catch(exception &ex){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
}

/**
 *  @brief  Opens the monitoring session.
 *
 *  @param[in]	session_name - session name

 *  @return
 *  	drmaa2_msession if the session exists
 *  	NULL if the session does not exist and if session name is invalid
 *  			Also sets drmaa2_lasterror_v to DRMAA2_INVALID_ARGUMENT error
 */
drmaa2_msession drmaa2_open_msession(const char *session_name) {
	if(session_name == NULL){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
	SessionManager *sessionManagerObj_ = Singleton<SessionManager, SessionManagerImpl>::getInstance();
	try{
		const MonitoringSession &monitoringSessionObj_ = sessionManagerObj_->openMonitoringSession(string(session_name));
		return (drmaa2_msession)&monitoringSessionObj_;
	}catch(exception &ex){
		lasterror = DRMAA2_INVALID_ARGUMENT;
		return NULL;
	}
}

/**
 *  @brief  closes the drmaa2_job session pointed by js
 *
 *  @param[in]	js - pointer to drmaa2_job session which needs to be closed

 *  @return
 *  	drmaa2_error - returns DRMAA2_SUCCESS if successful
 *  	DRMAA2_INVALID_SESSION if session is invalid
 */
drmaa2_error drmaa2_close_jsession(drmaa2_jsession js) {
	if(js == NULL){
		return DRMAA2_INVALID_SESSION;
	}
	return DRMAA2_SUCCESS;
}

/**
 *  @brief  Disassociates reservation session from DRMS. Should be callable
 *          only once else it should return DRMAA2_INVALID_SESSION error.
 *
 *  @param[in]	rs - pointer to drmaa2 reservation session created earlier.

 *  @return
 *  	DRMAA2_SUCCESS if succeeds
 *  	NULL and also sets sets drmaa2_lasterror_v to DRMAA2_INVALID_SESSION
 *
 */
drmaa2_error drmaa2_close_rsession(drmaa2_rsession rs) {
	if(rs == NULL){
		return DRMAA2_INVALID_SESSION;
	}
	return DRMAA2_SUCCESS;
}

/**
 *  @brief  Dis-associates monitoring session from DRMS.Should be callable only
 *          once else it should return DRMAA2_INVALID_SESSION error.
 *
 *  @param[in]	ms - pointer to drmaa2 monitoring session created earlier.

 *  @return
 *  	DRMAA2_SUCCESS if succeeds
 *  	NULL and also sets sets drmaa2_lasterror_v to DRMAA2_INVALID_SESSION
 *  			error if fails
 */
drmaa2_error drmaa2_close_msession(drmaa2_msession ms) {
	//TODO Add Code here
	return DRMAA2_SUCCESS;
}

/**
 *  @brief destroys the drmaa2_job session pointed by js
 *
 *  @param[in] session_name - pointer to drmaa2_job session which needs to be closed

 * 	@return
 * 		DRMAA2_SUCCESS if successful
 *      DRMAA2_INVALID_ARGUMENT if session name is invalid
 */

drmaa2_error drmaa2_destroy_jsession(const char *session_name) {
	//TODO Add Code here
	return DRMAA2_SUCCESS;
}

/**
 *  @brief  Reaps the persisted or cached state information for
 *  		the given session name.
 *
 *  @param[in]	session_name - Name of the session.

 *  @return
 * 		DRMAA2_SUCCESS if successful
 *      DRMAA2_INVALID_ARGUMENT if session name is invalid
 *
 */
drmaa2_error drmaa2_destroy_rsession(const char *session_name) {
	//TODO Add Code here
	return DRMAA2_SUCCESS;
}

/**
 *  @brief  returns the list of drmaa2_job sessions

 *  @return
 *  	drmaa2_string_list - returns list of job session names available
 *  	NULL and last error is set
 */
drmaa2_string_list drmaa2_get_jsession_names(void) {
	//TODO Add Code here
	return NULL;
}

/**
 * @brief Get a list of all reservation session names
 *
 *
 * @return
 * 		drmaa2_string_list - list of reservation sessions
 * 		NULL - If there are no existing reservation sessions
 *			 - If this feature is not supported, set drmaa2_lasterror_v
 *			 	to DRMAA2_UNSUPPORTED_OPERATION
 *
 */
drmaa2_string_list drmaa2_get_rsession_names(void) {
	//TODO Add Code here
	return NULL;
}

/**
 *  @brief  Registers a drmaa2_callback with the DRMS library.
 *  	    For the first phase of implementation we do not support this.
 *
 *  @param[in]	callback - a structure of type drmaa2_callback containing the
 *  						details of callback function, event etc

 *  @return
 *   	DRMAA2_SUCCESS if succeeds
 *  	DRMAA2_UNSUPPORTED_OPERATION error if fails
 *
 */
drmaa2_error drmaa2_register_event_notification(
		const drmaa2_callback callback) {
	//TODO Add Code here
	return DRMAA2_SUCCESS;
}

#ifdef __cplusplus
}

;

#endif
	
