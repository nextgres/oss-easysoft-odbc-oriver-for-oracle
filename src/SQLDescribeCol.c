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
 ******************************************************************************
 *
 * $Id: SQLDescribeCol.c,v 1.3 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLDescribeCol.c,v $
 * Revision 1.3  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.2  2002/05/02 15:39:48  dbox
 * fixed unused var warnings found by insure++
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.11  2000/07/21 10:05:06  tom
 * Casting made explicit
 *
 * Revision 1.10  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.9  2000/06/06 10:24:27  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.8  2000/05/12 10:59:50  tom
 * Checking for release candidate
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
 * Revision 1.2  2000/04/19 15:24:27  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLDescribeCol.c,v $ $Revision: 1.3 $";

SQLRETURN SQL_API SQLDescribeCol(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        ColumnNumber,
    SQLCHAR                *ColumnName,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *NameLengthPtr,
    SQLSMALLINT            *DataTypePtr,
    SQLUINTEGER            *ColumnSizePtr,
    SQLSMALLINT            *DecimalDigitsPtr,
    SQLSMALLINT            *NullablePtr )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    ar_T* ar;
    SQLRETURN status=SQL_SUCCESS;

    if(!StatementHandle || HANDLE_TYPE(StatementHandle)!=SQL_HANDLE_STMT)
    {
        return SQL_INVALID_HANDLE;
    }
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"i",
			"ColumnNumber",ColumnNumber);
}
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

    if(ColumnNumber>stmt->current_ir->num_recs)
    {
        ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_07009,ColumnNumber,
                "",ERROR_MESSAGE_07009,
                __LINE__,0,"",ERROR_STATE_07009,__FILE__,__LINE__);
        ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,SQL_ERROR,"");
}
        return SQL_ERROR;
    }
    ar=&stmt->current_ar->recs.ar[ColumnNumber];

    if(!ood_bounded_strcpy((char*)ColumnName,
				(char*)ar->column_name,BufferLength))
    {
        ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_01004,ColumnNumber,
                "",ERROR_MESSAGE_01004,
                __LINE__,0,"",ERROR_STATE_01004,__FILE__,__LINE__);
        status=SQL_SUCCESS_WITH_INFO;
    }
    if(NameLengthPtr)
        *NameLengthPtr=(SQLSMALLINT)strlen((const char*)ar->column_name);

    if(DataTypePtr)
        *DataTypePtr=(SQLSMALLINT)ar->concise_type;
    
    if(ColumnSizePtr)
        *ColumnSizePtr=(SQLUINTEGER)ar->length;

    if(DecimalDigitsPtr)
        *DecimalDigitsPtr=(SQLSMALLINT)ar->precision;

    if(NullablePtr)
        *NullablePtr=(SQLSMALLINT)ar->nullable;

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,SQL_SUCCESS,"");
}
    return status;
}
