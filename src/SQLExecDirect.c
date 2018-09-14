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
 * $Id: SQLExecDirect.c,v 1.6 2004/08/27 19:40:40 dbox Exp $
 *
 * $Log: SQLExecDirect.c,v $
 * Revision 1.6  2004/08/27 19:40:40  dbox
 * change error handling
 *
 * Revision 1.5  2004/08/06 20:42:56  dbox
 * changes to work more correctly with ood_lex_parse
 *
 * Revision 1.4  2004/05/05 18:08:39  dbox
 * change return status of where 1=0 from SQL_NO_DATA to SQL_SUCCESS. The NO_DATA return value leads to table not found errors in Dbi.  The correct thing is to find the problem in Dbi, but minos needs this to work now so consider this a band-aid
 *
 * Revision 1.3  2004/05/04 22:41:51  dbox
 * fixed error return code for bad select statement
 *
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
 * Revision 1.10  2000/07/21 10:06:10  tom
 * Fixed prefetch assignment
 *
 * Revision 1.9  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.8  2000/07/07 08:01:32  tom
 * heavier tracing and debugging
 *
 * Revision 1.7  2000/06/05 16:00:19  tom
 * Added SQL parse stage
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
 * Revision 1.2  2000/04/19 15:24:50  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLExecDirect.c,v $ $Revision: 1.6 $";

SQLRETURN SQL_API SQLExecDirect(
    SQLHSTMT        StatementHandle,
    SQLCHAR            *StatementText,
    SQLINTEGER        TextLength )
{
    hStmt_T *stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status;
    int param_no = 1; /* used in ood_lex_parse to assign oracle parameter
			 names. value returned is one greater than the final
			 parameter number. */

    if(!stmt||HANDLE_TYPE(stmt)!=SQL_HANDLE_STMT)
        return SQL_INVALID_HANDLE;

    if(ENABLE_TRACE){
      ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		      (SQLHANDLE)stmt,0,"si",
		      "SQL",StatementText,
		      "Length",TextLength);
    }

    ood_clear_diag((hgeneric*)stmt);

    /* This replaces the '?' parameter placeholders with OCI-style
       :Pn, where n is the parameter number and Pn becomes the oracle
       parmeter name.  */
    stmt->sql=ood_lex_parse((char*)StatementText,
			    TextLength,&param_no);
    if (param_no - 1 > stmt->current_ap->num_recs)
      {
	/* this check is inadequate, since maybe not all the allocated
	   parameters in current_ap are bound.  */
	ood_post_diag((hgeneric*)stmt->dbc,ERROR_ORIGIN_07002,0,"",
		      ERROR_MESSAGE_07002,
		      __LINE__,0,"",ERROR_STATE_07002,
		      __FILE__,__LINE__);
	return SQL_ERROR;
      }

    ood_mutex_lock_stmt(stmt);

    status=ood_driver_prepare(stmt,(unsigned char*)stmt->sql);


#if defined(ENABLE_TRACE) && defined (UNIX_DEBUG)
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)stmt,status,"s",
			"Status Report",stmt->sql);
#endif

    if (status == SQL_SUCCESS || status == SQL_SUCCESS_WITH_INFO)
      {
	SQLRETURN result = ood_driver_execute(stmt);

#if defined(ENABLE_TRACE) && defined (UNIX_DEBUG)
	ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
			(SQLHANDLE)stmt,status,"",
			"Status Report",stmt->sql);
#endif
	if (result != SQL_SUCCESS)
	  status = result;
      }

    if (status == SQL_SUCCESS || status == SQL_SUCCESS_WITH_INFO)
      {
	SQLRETURN result = ood_driver_execute_describe(stmt);

	if (result != SQL_SUCCESS)
	  status = result;
      }
    if (stmt->stmt_type==OCI_STMT_SELECT
	&& (status == SQL_SUCCESS || status == SQL_SUCCESS_WITH_INFO))
      {
	stmt->fetch_status=ood_driver_prefetch(stmt);

	if (stmt->fetch_status != SQL_SUCCESS
	    && stmt->fetch_status != SQL_SUCCESS_WITH_INFO
	    && stmt->fetch_status != SQL_NO_DATA)
	  {
	    status = stmt->fetch_status;
	  }
      }
    ood_mutex_unlock_stmt(stmt);

    if(ENABLE_TRACE)
      {
	ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
			(SQLHANDLE)NULL,status,"");
      }
    return status;
}
