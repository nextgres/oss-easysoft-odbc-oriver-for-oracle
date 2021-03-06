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
 * $Id: SQLPrepare.c,v 1.4 2004/08/06 20:43:51 dbox Exp $
 *
 * $Log: SQLPrepare.c,v $
 * Revision 1.4  2004/08/06 20:43:51  dbox
 * change variable type from int to ub4
 *
 * Revision 1.3  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.2  2002/05/31 19:54:59  dbox
 * added  reasonable comments to the tests, fixed SQLPrepare so that it
 * allocates space in the statement for the column ap and ir descriptors
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.11  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.10  2000/06/05 16:08:31  tom
 * SQL parser added
 *
 * Revision 1.9  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.8  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.7  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.6  2000/04/26 15:32:06  tom
 * Changed to account for new handle definitions
 *
 * Revision 1.5  2000/04/26 10:18:00  tom
 * move to ood_driver_ functions
 *
 * Revision 1.4  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.3  2000/04/19 15:28:17  tom
 * First Function Checkin
 *
 * Revision 1.2  2000/04/13 15:54:18  tom
 * Put in Oracle function call
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLPrepare.c,v $ $Revision: 1.4 $";

SQLRETURN SQL_API SQLPrepare(
    SQLHSTMT        StatementHandle,
    SQLCHAR            *StatementText,
    SQLINTEGER        TextLength )
{
  ub4 i;
    hStmt_T *stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status=SQL_SUCCESS;

    if(!stmt||HANDLE_TYPE(stmt)!=SQL_HANDLE_STMT)
    {
        return SQL_INVALID_HANDLE;
    }
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
}
    ood_clear_diag((hgeneric*)stmt);
    
	stmt->sql=ood_lex_parse((char*)StatementText,TextLength,
			(int*)&stmt->current_ap->num_recs);

    ood_mutex_lock_stmt(stmt);

    status=ood_driver_prepare(stmt,(unsigned char*)stmt->sql);
    for(i=1;i<=stmt->current_ap->num_recs; i++){
      status |= ood_alloc_param_desc(stmt,i,
				     stmt->current_ip,stmt->current_ap);
    }
    ood_mutex_unlock_stmt(stmt);

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}
