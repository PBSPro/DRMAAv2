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

#include "drmaa2.hpp"
#include "drmaa2.h"

#include <string>

#include "SessionManagerImpl.h"

using namespace std;

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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
}

/**
 *  @brief  frees the pre-allocated drmaa slotinfo
 *
 *  @param[in]	si- Pointer to drmaa slotinfo

 *  @return - None
 *
 */
void drmaa2_slotinfo_free(drmaa2_slotinfo * si) {
	//TODO Add Code here
}

/**
 *  @brief  frees the pre-allocated drmaa rinfo structure
 *
 *  @param[in]	ri- Pointer to drmaa reservation info

 *  @return - None
 *
 */
void drmaa2_rinfo_free(drmaa2_rinfo * ri) {
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
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
	//TODO Add Code here
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
	//TODO Add Code here
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
	//TODO Add Code here
}
/**
 *  @brief  Frees the memory of Reservation session pointed by rs
 *
 *  @param[in]		rs	-	pointer to drmaa2_rsession.
 *
 *  @return - None
 */
void drmaa2_rsession_free(drmaa2_rsession * rs) {
	//TODO Add Code here
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
	//TODO Add Code here
}

/**
 *  @brief  frees the drmaa2_job array structure pointed by ja
 *
 *  @param[in]	ja	-	pointer to DRMAA jo array
 *
 *  @return - None
 */
void drmaa2_jarray_free(drmaa2_jarray * ja) {
	//TODO Add Code here
}

/**
 *  @brief  frees the drmaa2_r structure pointed by r
 *
 *  @param[in]	r	-	pointer to DRMAA reservation
 *
 *  @return - None
 */
void drmaa2_r_free(drmaa2_r * r) {
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return DRMAA2_SUCCESS;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return DRMAA2_SUCCESS;
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

	//TODO Add Code here
	return DRMAA2_SUCCESS;

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
	//TODO Add Code here
	return DRMAA2_INVALID_ARGUMENT;
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
	//TODO Add Code here
	return DRMAA2_INVALID_STATE;
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
	//TODO Add Code here
	return DRMAA2_INVALID_ARGUMENT;
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
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return DRMAA2_SUCCESS;
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
	//TODO Add Code here
	return DRMAA2_SUCCESS;
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
	//TODO Add Code here
	return DRMAA2_SUCCESS;
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
	//TODO Add Code here
	return DRMAA2_SUCCESS;
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
	//TODO Add Code here
	return DRMAA2_SUCCESS;
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
	//TODO Add Code here
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
	//TODO Add Code here
	return DRMAA2_DONE;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
}

/**
 *  @brief  returns the DRM system name
 *
 *  @param[in]	-  None

 *  @return drmaa2_string
 */
drmaa2_string drmaa2_get_drms_name(void) {
	//TODO Add Code here
	return (strdup( drmaa2::Singleton<drmaa2::SessionManager, drmaa2::SessionManagerImpl>::getInstance()->getDrmaaName().c_str()));
}

/**
 *  @brief  returns the DRM system version
 *
 *  @param[in]	-  None

 *  @return drmaa2_version
 */
drmaa2_version drmaa2_get_drms_version(void) {
	//TODO Add Code here
	return NULL;
}

/**
 *  @brief  returns the DRM application name
 *
 *  @param[in]	-  None
 *
 *  @return drmaa2_string
 */
drmaa2_string drmaa2_get_drmaa_name(void) {
	//TODO Add Code here
	return NULL;
}

/**
 *  @brief  returns the DRM application version
 *
 *  @param[in]	-  None

 *  @return drmaa2_version
 */
drmaa2_version drmaa2_get_drmaa_version(void) {
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
	return NULL;
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
	//TODO Add Code here
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
	//TODO Add Code here
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
	
