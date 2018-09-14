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
 * $Id: SQLExecute.c,v 1.2 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLExecute.c,v $
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
 * Revision 1.10  2000/07/07 08:01:58  tom
 * ood_driver_prefetch for block cursors
 *
 * Revision 1.9  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.8  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.7  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.6  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.5  2000/04/26 15:29:29  tom
 * Changes to account for new handle defintions
 *
 * Revision 1.4  2000/04/26 10:15:33  tom
 * move to ood_driver_ function names
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:24:54  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLExecute.c,v $ $Revision: 1.2 $";

SQLRETURN SQL_API SQLExecute(
    SQLHSTMT        StatementHandle )
{
    hStmt_T *stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status;

    if(!stmt||HANDLE_TYPE(stmt)!=SQL_HANDLE_STMT)
        return SQL_INVALID_HANDLE;

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
}
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

    status=ood_driver_execute(stmt);
    if(status)
    {
        ood_mutex_unlock_stmt(stmt);
        return status;
    }
    status=ood_driver_execute_describe(stmt);
	if(stmt->stmt_type==OCI_STMT_SELECT)
		stmt->fetch_status=ood_driver_prefetch(stmt);
    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}
