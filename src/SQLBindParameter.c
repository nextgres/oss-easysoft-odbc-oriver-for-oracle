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
 * $Id: SQLBindParameter.c,v 1.6 2004/08/27 19:37:11 dbox Exp $
 *
 * $Log: SQLBindParameter.c,v $
 * Revision 1.6  2004/08/27 19:37:11  dbox
 * change logging behavior
 *
 * Revision 1.5  2004/08/06 20:42:29  dbox
 * 1. got rid of useless function _SQLBindParameter 2. fixed behavior of Column size for various data types to conform to ODBC standard
 *
 * Revision 1.4  2003/10/20 23:37:13  dbox
 * various changes to handle blob i/o
 *
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
 * Revision 1.10  2000/07/21 10:02:08  tom
 * Added LOB flag
 *
 * Revision 1.9  2000/06/06 10:24:22  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.8  2000/06/05 15:57:01  tom
 * Initial implementation
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
 * Revision 1.2  2000/04/19 15:23:28  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLBindParameter.c,v $ $Revision: 1.6 $";


SQLRETURN SQL_API SQLBindParameter(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        ParameterNumber,
    SQLSMALLINT            InputOutputType,
    SQLSMALLINT            ValueType,
    SQLSMALLINT            ParameterType,
    SQLUINTEGER            ColumnSize,
    SQLSMALLINT            DecimalDigits,
    SQLPOINTER            ParameterValuePtr,
    SQLINTEGER            BufferLength,
    SQLINTEGER            *StrLen_or_IndPtr )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status=SQL_SUCCESS;

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"iiiiiipip",
			"ParameterNumber",ParameterNumber,
			"InputOutputType",InputOutputType,
			"ValueType",ValueType,
			"ParameterType",ParameterType,
		    "ColumnSize",ColumnSize,
			"DecimalDigits",DecimalDigits,
			"ParameterValuePtr",ParameterValuePtr,
		    "BufferLength",BufferLength,
			"StrLen_or_IndPtr",StrLen_or_IndPtr);
}
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

	switch(InputOutputType)
	{
		case SQL_PARAM_INPUT:
        /*
	     * We may not have allocated any param descriptors yet, 
	     */
	    if(SQL_SUCCESS!=ood_alloc_param_desc(stmt,ParameterNumber,
					stmt->current_ip,stmt->current_ap))
        {
            status=SQL_ERROR;
			break;
        }
        else
        {
			stmt->current_ap->recs.ap[ParameterNumber].concise_type=
				stmt->current_ap->recs.ap[ParameterNumber].data_type=ValueType;
            stmt->current_ap->recs.ap[ParameterNumber].bind_indicator=
				StrLen_or_IndPtr;
            stmt->current_ap->recs.ap[ParameterNumber].buffer_length=	BufferLength;

	    /* Column size is ignored for various data types.
	       Reference: ODBC Programmer's Reference, Appendix D,
	       Column Size
	       http://msdn.microsoft.com/library/default.asp?url=/library/en-us/odbc/htm/odbccolumn_size.asp.
	    */
	    
	    stmt->current_ap->recs.ap[ParameterNumber].octet_length = BufferLength;

	    switch (ParameterType)
	      {
	      case SQL_BIT:
	      case SQL_TINYINT:
	      case SQL_SMALLINT:
	      case SQL_INTEGER:
	      case SQL_BIGINT:
	      case SQL_REAL:
	      case SQL_FLOAT:
	      case SQL_DOUBLE:
	      case SQL_TYPE_DATE:
	      case SQL_TYPE_TIME:
		break;

	      default:
		if(ColumnSize >= BufferLength)
		  stmt->current_ap->recs.ap[ParameterNumber].octet_length
		    = ColumnSize;
	      }

            stmt->current_ap->recs.ap[ParameterNumber].data_ptr=
				ParameterValuePtr;
			stmt->current_ap->recs.ap[ParameterNumber].bind_target_type=
				ParameterType;
			stmt->current_ap->recs.ap[ParameterNumber].precision=
				DecimalDigits;
            /*stmt->current_ap->recs.ar[ParameterNumber].*/
            /*stmt->current_ap->recs.ar[ParameterNumber].*/
            /*stmt->current_ap->recs.ar[ParameterNumber].*/
            /*stmt->current_ap->recs.ar[ParameterNumber].*/

			stmt->current_ap->bound_col_flag++;
			stmt->current_ap->lob_col_flag++;
	    }
		break;

		default:
			ood_post_diag((hgeneric*)stmt->dbc,ERROR_ORIGIN_HYC00,0,"",
					ERROR_MESSAGE_HYC00,
                    __LINE__,0,"",ERROR_STATE_HYC00,
                    __FILE__,__LINE__);
            status=SQL_ERROR;
	}


    ood_mutex_unlock_stmt(stmt);
/*
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
*/
    return status;
}
