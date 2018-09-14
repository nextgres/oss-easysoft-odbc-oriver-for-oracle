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
 *
 *******************************************************************************
 *
 * $Id: SQLExtendedFetch.c,v 1.2 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLExtendedFetch.c,v $
 * Revision 1.2  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.9  2000/07/21 10:06:45  tom
 * Initial implementation
 *
 * Revision 1.8  2000/06/06 10:23:57  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.6  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.5  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.4  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:24:58  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLExtendedFetch.c,v $ $Revision: 1.2 $";

SQLRETURN SQL_API SQLExtendedFetch(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       fFetchType,
    SQLINTEGER         irow,
    SQLUINTEGER           *pcrow,
    SQLUSMALLINT          *rgfRowStatus)
{
	SQLRETURN status;
	hStmt_T *stmt=(hStmt_T*)hstmt;
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,"SQLExtendedFetch->SQLFetchScroll",__LINE__,
			TRACE_FUNCTION_ENTRY, (SQLHANDLE)hstmt,0,"");
}
	ood_mutex_lock_stmt(stmt);

    if(rgfRowStatus)
	{
	    void *status_array_safe;
		status_array_safe=stmt->row_status_ptr;
		stmt->row_status_ptr=rgfRowStatus;
	    status=ood_SQLFetchScroll(stmt,fFetchType,irow);
		stmt->row_status_ptr=status_array_safe;
	}
	else
	{
	    status=ood_SQLFetchScroll(stmt,fFetchType,irow);
	}
	if(pcrow)
		*pcrow=stmt->num_fetched_rows;
    
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,"SQLExtendedFetch<-SQLFetchScroll",__LINE__,
			TRACE_FUNCTION_EXIT, (SQLHANDLE)NULL,status,"");
}
	ood_mutex_unlock_stmt(stmt);
	return status;
}
