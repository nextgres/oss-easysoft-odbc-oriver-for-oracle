/*******************************************************************************
 *
 * Copyright (c) 2000 Easysoft Ltd
 *
 * The contents of this file are subject to the Easysoft Public License 
 * Version 1.0 (the "License"); you may not use this file except in compliance 
 * with the License. 
 *
 * You may obtain a copy of the License at http://www.easysoft.org/EPL.html 
 *
 * Software distributed under the License is distributed on an "AS IS" basis, 
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for 
 * the specific language governing rights and limitations under the License. 
 *
 * The Original Code was created by Easysoft Limited and its successors. 
 *
 * Contributor(s): Tom Fosdick (Easysoft) 
 *                 Dennis Box (Fermi Nat Accelerator Lab)
 *
 *******************************************************************************
 *
 * $Id: common_functions.h,v 1.5 2004/11/17 03:02:59 dbox Exp $
 ******************************************************************************/
#ifndef _ORACOMMON_FUNCTIONS_H
#define _ORACOMMON_FUNCTIONS_H
#include <stdlib.h>

#define ORAFREE(aptr) do { if(aptr) { free (aptr); aptr=NULL; } } while(0)
#define ORAMALLOC(size)  ora_malloc(size)
#define ORAREALLOC(ptr, size) ora_realloc(ptr,size)

void* ora_malloc(size_t num);
void* ora_realloc(void *ptr, size_t size);
const char * odbc_var_type(SQLSMALLINT sqltype);
const char * oci_var_type(SQLSMALLINT sqltype);
const char * sql_get_info_type(SQLSMALLINT sqltype);
const char * odbc_sql_attr_type( SQLINTEGER  attr);
const char * odbc_desc_type(SQLSMALLINT sqltype);

SQLINTEGER sqltype_display_size(SQLSMALLINT a, int b);
void ood_mutex_lock_stmt(hStmt_T *stmt);
void ood_mutex_unlock_stmt(hStmt_T *stmt);
int ood_mutex_init(hgeneric* handle);
int ood_mutex_destroy(hgeneric* handle);
sword ood_alt_fetch_no_data(hStmt_T* stmt);
#ifdef ENABLE_TRACE
void ood_log_message(hDbc_T* dbc,char *file,int line, int mask, 
		SQLHANDLE handle, SQLRETURN ret, char *fmt, ... );
#endif
#endif
