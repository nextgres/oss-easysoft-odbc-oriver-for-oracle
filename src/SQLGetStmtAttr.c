/*******************************************************************************
 *
 * Copyright (c) 2000 Easysoft Ltd
 * * The contents of this file are subject to the Easysoft Public License 
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
 * $Id: SQLGetStmtAttr.c,v 1.4 2003/02/11 21:37:55 dbox Exp $
 *
 * $Log: SQLGetStmtAttr.c,v $
 * Revision 1.4  2003/02/11 21:37:55  dbox
 * fixed a problem with array inserts, SQLSetStmtAttr and SQLGetStmtAttr
 * for parameters SQL_ATTR_PARAMSET_SIZE and SQL_ATTR_PARAM_BIND_TYPE
 * are now functional
 *
 * Revision 1.3  2002/11/14 22:28:36  dbox
 * %$@*&%$??!
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
 * Revision 1.11  2000/07/21 10:11:38  tom
 * SQL_ATTR_QUERY_TIMEOUT added
 *
 * Revision 1.10  2000/07/07 08:07:52  tom
 * initial implementation
 *
 * Revision 1.9  2000/06/06 10:23:06  tom
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
 * Revision 1.4  2000/04/26 10:17:34  tom
 * removed (currently) unnecessary prototype
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:27:42  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetStmtAttr.c,v $ $Revision: 1.4 $";

SQLRETURN SQL_API SQLGetStmtAttr(
				 SQLHSTMT            StatementHandle,
				 SQLINTEGER            Attribute,
				 SQLPOINTER            ValuePtr,
				 SQLINTEGER            BufferLength,
				 SQLINTEGER            *StringLengthPtr )
{
  hStmt_T* stmt=(hStmt_T*)StatementHandle;
  SQLRETURN status=SQL_SUCCESS;
  if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		    (SQLHANDLE)stmt,0,"i",
		    "Attribute",Attribute);
  }
  ood_clear_diag((hgeneric*)stmt);
  ood_mutex_lock_stmt(stmt);

  switch(Attribute)
    {
    case SQL_ATTR_METADATA_ID:
      THREAD_MUTEX_LOCK(stmt->dbc);
      *((SQLUINTEGER*)ValuePtr)=stmt->dbc->metadata_id;
      THREAD_MUTEX_UNLOCK(stmt->dbc);
      break;

    case SQL_ATTR_APP_ROW_DESC:
      *((SQLPOINTER*)ValuePtr)=(SQLPOINTER)stmt->current_ar;
      break;
    case SQL_ATTR_APP_PARAM_DESC:
      *((SQLPOINTER*)ValuePtr)=(SQLPOINTER)stmt->current_ap;
      break;

    case SQL_ATTR_IMP_PARAM_DESC:
      *((SQLPOINTER*)ValuePtr)=(SQLPOINTER)stmt->current_ir;
      break;

    case SQL_ATTR_IMP_ROW_DESC:
      *((SQLPOINTER*)ValuePtr)=(SQLPOINTER)stmt->current_ip;
      break;

    case SQL_ATTR_QUERY_TIMEOUT:
      *((SQLUINTEGER*)ValuePtr)=stmt->query_timeout;
      break;

    case SQL_ATTR_PARAM_BIND_OFFSET_PTR:
      *((SQLPOINTER*)ValuePtr)=(SQLPOINTER)stmt->param_bind_offset_ptr;
      break;

    case SQL_ATTR_ROW_BIND_OFFSET_PTR:
      *((SQLPOINTER*)ValuePtr)=(SQLPOINTER)stmt->row_bind_offset_ptr;
      break;

    case SQL_ATTR_ROW_ARRAY_SIZE:
    case SQL_ROWSET_SIZE:
      *((SQLUINTEGER*)ValuePtr)=stmt->row_array_size?
	stmt->row_array_size:1;
      break;

    case SQL_ATTR_PARAM_BIND_TYPE:
      *((SQLUINTEGER*)ValuePtr)=stmt->param_bind_type?
	stmt->param_bind_type:SQL_PARAM_BIND_BY_COLUMN;
      break;
      
   case SQL_ATTR_PARAMSET_SIZE:
      *((SQLUINTEGER*)ValuePtr)=stmt->paramset_size;
      break;
      

    case SQL_ATTR_PARAM_OPERATION_PTR:
      /*break;*/

    case SQL_ATTR_PARAM_STATUS_PTR:
      /*break;*/

    case SQL_ATTR_PARAMS_PROCESSED_PTR:
      /*break;*/

 
    case SQL_ATTR_ROW_BIND_TYPE:
      /*break;*/

    case SQL_ATTR_ROW_OPERATION_PTR:
      /*break;*/

    case SQL_ATTR_ROW_STATUS_PTR:
      /*break;*/

    case SQL_ATTR_ROWS_FETCHED_PTR:
      /*break;*/

    case SQL_ATTR_ASYNC_ENABLE:
      /*break;*/

    case SQL_ATTR_CONCURRENCY:
      /*break;*/

    case SQL_ATTR_CURSOR_SCROLLABLE:
      /*break;*/

    case SQL_ATTR_CURSOR_SENSITIVITY:
      /*break;*/

    case SQL_ATTR_CURSOR_TYPE:
      /*break;*/

    case SQL_ATTR_ENABLE_AUTO_IPD:
      /*break;*/

    case SQL_ATTR_FETCH_BOOKMARK_PTR:
      /*break;*/

    case SQL_ATTR_KEYSET_SIZE:
      /*break;*/

    case SQL_ATTR_MAX_LENGTH:
      /*break;*/

    case SQL_ATTR_MAX_ROWS:
      /*break;*/

    case SQL_ATTR_NOSCAN:
      /*break;*/

    case SQL_ATTR_SIMULATE_CURSOR:
      /*break;*/

    case SQL_ATTR_USE_BOOKMARKS:
      /*break;*/

    default:
      ood_post_diag((hgeneric*)stmt->dbc,ERROR_ORIGIN_IM001,0,"",
		    ERROR_MESSAGE_IM001,
		    __LINE__,0,"",ERROR_STATE_IM001,
		    __FILE__,__LINE__);
      status=SQL_SUCCESS_WITH_INFO;
    }

  ood_mutex_unlock_stmt(stmt);
  if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		    (SQLHANDLE)NULL,status,"");
  }
  return status;
}
