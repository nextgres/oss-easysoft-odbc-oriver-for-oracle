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
 * $Id: SQLSetStmtAttr.c,v 1.7 2004/12/01 01:00:25 dbox Exp $
 *
 * $Log: SQLSetStmtAttr.c,v $
 * Revision 1.7  2004/12/01 01:00:25  dbox
 * reset bookmark flag when stmt is reset to have retrieved 0 rows
 *
 * Revision 1.6  2004/11/17 03:02:59  dbox
 * changed some bind/fetch internals, better ood_log behavior
 *
 * Revision 1.5  2003/02/11 21:37:55  dbox
 * fixed a problem with array inserts, SQLSetStmtAttr and SQLGetStmtAttr
 * for parameters SQL_ATTR_PARAMSET_SIZE and SQL_ATTR_PARAM_BIND_TYPE
 * are now functional
 *
 * Revision 1.4  2002/11/14 22:28:36  dbox
 * %$@*&%$??!
 *
 * Revision 1.3  2002/08/13 22:41:46  dbox
 * fixed the array insert test
 *
 * Revision 1.2  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.12  2000/07/21 10:14:09  tom
 * Lots of logging added for debug, SQL_ATTR_QUERY_TIMEOUT added
 *
 * Revision 1.11  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.10  2000/07/07 08:11:05  tom
 * inital implementation
 *
 * Revision 1.9  2000/06/06 10:23:41  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.7  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.6  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.5  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.4  2000/04/26 10:18:45  tom
 * removed (currently) unneccessary function stub
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:29:17  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLSetStmtAttr.c,v $ $Revision: 1.7 $";


SQLRETURN SQL_API SQLSetStmtAttr(
    SQLHSTMT              StatementHandle,
    SQLINTEGER            Attribute,
    SQLPOINTER            ValuePtr,
    SQLINTEGER            StringLength )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status=SQL_SUCCESS;
     if(ENABLE_TRACE){
      ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		      (SQLHANDLE)stmt,0,"shi",
		      "Attribute",odbc_sql_attr_type(Attribute),
		      "ValuePtr",ValuePtr,
		      "StringLength",StringLength);
    }
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);
    
    if(ENABLE_TRACE){
      ood_log_message(stmt->dbc,__FILE__,__LINE__,0,
		      (SQLHANDLE)stmt,0,"sh",
		      "Attribute",odbc_sql_attr_type(Attribute),
		      "ValuePtr",ValuePtr);
    }

    switch(Attribute)
      {
      case SQL_ATTR_METADATA_ID:
        THREAD_MUTEX_LOCK(stmt->dbc);
        stmt->dbc->metadata_id=(SQLUINTEGER)ValuePtr;
        THREAD_MUTEX_UNLOCK(stmt->dbc);
	break;
	
      case SQL_ATTR_APP_ROW_DESC:
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
	break;
	
      case SQL_ATTR_APP_PARAM_DESC:
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
	break;
	
      case SQL_ATTR_IMP_PARAM_DESC:
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
	break;
	
      case SQL_ATTR_IMP_ROW_DESC:
        stmt->current_ip=*((SQLPOINTER*)ValuePtr);
	break;
	
      case SQL_ATTR_QUERY_TIMEOUT:
	stmt->query_timeout=(SQLUINTEGER)ValuePtr;
	break;
	
      case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
        stmt->param_bind_offset_ptr=*((SQLPOINTER*)ValuePtr);
	break;
	
      case SQL_ATTR_ROW_BIND_OFFSET_PTR:
        stmt->row_bind_offset_ptr=*((SQLPOINTER*)ValuePtr);
	break;
	
      case SQL_ATTR_ROW_ARRAY_SIZE:
        stmt->row_array_size=(SQLINTEGER)ValuePtr;
	break;
	
      case SQL_ATTR_ROW_BIND_TYPE:
	if((SQLINTEGER)ValuePtr==SQL_BIND_BY_COLUMN)
	  stmt->row_bind_type=(SQLINTEGER)ValuePtr;
	else
	  {
	    ood_post_diag((hgeneric*)stmt->dbc,ERROR_ORIGIN_IM001,0,"",
			  ERROR_MESSAGE_IM001,
			  __LINE__,0,"",ERROR_STATE_IM001,
			  __FILE__,__LINE__);
	    status=SQL_SUCCESS_WITH_INFO;
	  }
	break;
	
      case SQL_ATTR_ROW_STATUS_PTR:
        stmt->row_status_ptr=(SQLUSMALLINT*)ValuePtr;
        break;
	
      case SQL_ATTR_ROWS_FETCHED_PTR:
        stmt->rows_fetched_ptr=(SQLUINTEGER*)ValuePtr;
        if(stmt->rows_fetched_ptr==0)
		stmt->bookmark=0;
	break;
      case SQL_ATTR_PARAM_BIND_TYPE:
        stmt->param_bind_type=(SQLINTEGER)ValuePtr;
	break;
      case SQL_ATTR_PARAMSET_SIZE:
        stmt->paramset_size=(SQLINTEGER)ValuePtr;
	break;

      /* these guys are unimplemented and return SQL_ERROR */
      case SQL_ATTR_PARAM_OPERATION_PTR:
      case SQL_ATTR_PARAMS_PROCESSED_PTR:
      case SQL_ATTR_ROW_OPERATION_PTR:
      case SQL_ATTR_PARAM_STATUS_PTR:
      case SQL_ATTR_ASYNC_ENABLE:
      case SQL_ATTR_CONCURRENCY:
      case SQL_ATTR_CURSOR_SCROLLABLE:
      case SQL_ATTR_CURSOR_SENSITIVITY:
      case SQL_ATTR_CURSOR_TYPE:
      case SQL_ATTR_ENABLE_AUTO_IPD:
      case SQL_ATTR_FETCH_BOOKMARK_PTR:
      case SQL_ATTR_KEYSET_SIZE:
      case SQL_ATTR_MAX_LENGTH:
      case SQL_ATTR_MAX_ROWS:
      case SQL_ATTR_NOSCAN:
      case SQL_ATTR_SIMULATE_CURSOR:
      case SQL_ATTR_USE_BOOKMARKS:
      default:
	ood_post_diag((hgeneric*)stmt->dbc,ERROR_ORIGIN_IM001,0,"",
		      ERROR_MESSAGE_IM001,
		      __LINE__,0,"",ERROR_STATE_IM001,
		      __FILE__,__LINE__);
        status=SQL_ERROR;
      }
    
    ood_mutex_unlock_stmt(stmt);
    if(ENABLE_TRACE){
      ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		      (SQLHANDLE)NULL,status,"");
    }
    return status;
}
