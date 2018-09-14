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
 * $Id: SQLGetData.c,v 1.2 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLGetData.c,v $
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
 * Revision 1.12  2000/07/21 10:09:38  tom
 * Logging tidied and return mechanism tweaked for LOBS
 *
 * Revision 1.11  2000/07/07 08:06:06  tom
 * numerous updates for block cursors
 *
 * Revision 1.10  2000/05/15 08:58:37  tom
 * Release 0.0.2
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
 * Revision 1.6  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.5  2000/04/26 15:31:13  tom
 * The new method of assignments to the IR means we no longer need the big 
 * switch
 *
 * Revision 1.4  2000/04/26 10:16:30  tom
 * switch filled out a bit more
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:27:09  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetData.c,v $ $Revision: 1.2 $";

SQLRETURN _SQLGetData(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        ColumnNumber,
    SQLSMALLINT            TargetType,
    SQLPOINTER            TargetValuePtr,
    SQLINTEGER            BufferLength,
    SQLINTEGER            *StrLen_or_indPtr )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    hDesc_T *ir;
    hDesc_T *ar;
    ar_T *ar_rec;
    ir_T *ir_rec;
    SQLRETURN status=SQL_SUCCESS;

/*    ood_clear_diag((hgeneric*)stmt); *//* This is a bit too expensive here */
#ifdef UNIX_DEBUG
	fprintf(stderr,"SQLGetData row %d col %d buffer 0x%.8lx\n",
			stmt->current_row-1,ColumnNumber,
			(long)TargetValuePtr);
#endif
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"iiii",
			"ColumnNumber",ColumnNumber,
			"RowNumber",stmt->current_row-1,
			"TargetType",TargetType,
			"BufferLength",BufferLength);
}

    ood_mutex_lock_stmt(stmt);
    ir=stmt->current_ir;
    ar=stmt->current_ar;
    ar_rec=&ar->recs.ar[ColumnNumber];
    ir_rec=&ir->recs.ir[ColumnNumber];

    /*
     * NULLs are no problem
     */
    if(ir_rec->ind_arr[stmt->current_row-1])
    {
        *StrLen_or_indPtr=SQL_NULL_DATA;
        ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,status,"");
}
        return SQL_SUCCESS;
    }

    /*
     * Easy of it's requested as the same type as we reported
     */
    if(TargetType==ar_rec->data_type
            ||TargetType==SQL_C_DEFAULT
            ||TargetType==SQL_ARD_TYPE)
    {
        status=ir_rec->default_copy(stmt->current_row-1,ir_rec,TargetValuePtr,
				BufferLength, StrLen_or_indPtr) ;
        ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,status,"sii",
				"SQL_C_DEFAULT|SQL_ARD_TYPE","",
				"indicator",StrLen_or_indPtr?*StrLen_or_indPtr:0,
				"*Target",*((SQLCHAR*)TargetValuePtr));
}
        return status;
    }

    /*
     * Also easy if requested as SQL_C_CHAR
     */
    if(TargetType==SQL_C_CHAR)
    {
        status=ir_rec->to_string(stmt->current_row-1,ir_rec,TargetValuePtr,
				BufferLength,StrLen_or_indPtr);
        ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,status,"s","SQL_C_CHAR","");
}
        return status;
    }

    /* 
     * Not so easy. This effectively is a lookup table of what we can and 
     * can't do.
     * Obviously the more common the conversions the earlier they want to be
     * in these switches.
     */
    status=(ood_fn_default_copy(ir_rec->data_type,TargetType))
            (stmt->current_row-1,ir_rec,TargetValuePtr,BufferLength,
			 StrLen_or_indPtr);
    ood_mutex_unlock_stmt(stmt);

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"s","data converted at SQLGetData","");
}
    return status;








#ifdef _DELETED_CODE
    switch(ir_rec->data_type)
    {
        /* 
         * The Oracle 8.x.5 number bug means we get SQLT_VNU's everywhere
         */
        case SQLT_VNU:
        switch(TargetType)
        {
            case SQL_C_SLONG:
                status=ocivnu_sqlslong(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_ULONG:
                status=ocivnu_sqlulong(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_DOUBLE:
                status=ocivnu_sqldouble(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_FLOAT:
                status=ocivnu_sqlfloat(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_SSHORT:
                status=ocivnu_sqlsshort(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_USHORT:
                status=ocivnu_sqlushort(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_SBIGINT:
                status=ocivnu_sqlsbigint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_UBIGINT:
                status=ocivnu_sqlubigint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_UTINYINT:
                status=ocivnu_sqlutinyint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_STINYINT:
                status=ocivnu_sqlstinyint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_NUMERIC: /* This is the default, so shouldn't happen */
                status=ocivnu_sqlnumeric(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            default:
                ood_post_diag(stmt,ERROR_ORIGIN_07006,ColumnNumber,"",
                        ERROR_MESSAGE_07006,
                    __LINE__,0,"",ERROR_STATE_07006,
                    __FILE__,__LINE__);
            status=SQL_ERROR;
        }
        break;

        case SQLT_INT:
        switch(TargetType)
        {
            case SQL_C_SLONG:
                status=ociint_sqlslong(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_ULONG:
                status=ociint_sqlulong(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_DOUBLE:
                status=ociint_sqldouble(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_FLOAT:
                status=ociint_sqlfloat(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_SSHORT:
                status=ociint_sqlsshort(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_USHORT:
                status=ociint_sqlushort(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_SBIGINT:
                status=ociint_sqlsbigint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_UBIGINT:
                status=ociint_sqlubigint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_UTINYINT:
                status=ociint_sqlutinyint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            case SQL_C_STINYINT:
                status=ociint_sqlstinyint(ir_rec,TargetValuePtr,BufferLength,
                    StrLen_or_indPtr);

            default:
                ood_post_diag(stmt,ERROR_ORIGIN_07006,ColumnNumber,"",
                        ERROR_MESSAGE_07006,
                    __LINE__,0,"",ERROR_STATE_07006,
                    __FILE__,__LINE__);
            status=SQL_ERROR;
        }
    }
    return status;
#endif
}

SQLRETURN SQL_API SQLGetData(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        ColumnNumber,
    SQLSMALLINT            TargetType,
    SQLPOINTER            TargetValuePtr,
    SQLINTEGER            BufferLength,
    SQLINTEGER            *StrLen_or_indPtr )
{
    return _SQLGetData(StatementHandle,ColumnNumber,TargetType,TargetValuePtr,
                BufferLength,StrLen_or_indPtr);
}
