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
 * $Id: SQLFreeStmt.c,v 1.2 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLFreeStmt.c,v $
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
 * Revision 1.11  2000/07/21 10:08:58  tom
 * Logging tidied and fixed use of wrong descriptor
 *
 * Revision 1.10  2000/07/07 08:04:17  tom
 * resets for new stmt handle attrs
 *
 * Revision 1.9  2000/06/05 16:02:53  tom
 * Double ORAFREE avoided
 * Initial bound params implementation
 *
 * Revision 1.8  2000/05/17 15:19:35  tom
 * OCIEnv moved to dbc
 *
 * Revision 1.7  2000/05/11 13:17:32  tom
 * *** empty log message ***
 *
 * Revision 1.6  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.5  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.4  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:26:54  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ********************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLFreeStmt.c,v $ $Revision: 1.2 $";

/*
 * SQLFreeStmt can be called from SQLCloseCursor
 */

SQLRETURN _SQLFreeStmt(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        Option )
{
#define STUB_OUT_FOR_3_4
#ifdef STUB_OUT_FOR_3_4
return SQL_SUCCESS;
#else

    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    SQLRETURN status=SQL_SUCCESS;
    sword ret;

    ood_clear_diag((hgeneric*)stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,status,"s",
			"Option",
			Option==SQL_CLOSE?"SQL_CLOSE":
			Option==SQL_DROP?"SQL_DROP":
			Option==SQL_UNBIND?"SQL_UNBIND":
			Option==SQL_RESET_PARAMS?"SQL_RESET_PARAMS":"unkown");
}
    

    switch(Option)
    {
        case SQL_CLOSE:
#ifdef UNIX_DEBUG
            fprintf(stderr,"%s %d stmt[0x%.8lx];\n",__FILE__,__LINE__,
					(long)StatementHandle);
#endif
			/*
			 * Belt and braces approch to resetting Oracle...
			 *
			 * Cancel.
			 */
/*            ret=OCIBreak(stmt->dbc->oci_svc,stmt->dbc->oci_err);*/
			/* Cancel cursor */
			if(stmt->oci_stmt)
			{
			    ret|=OCIHandleFree(stmt->oci_stmt,OCI_HTYPE_STMT);
			    stmt->oci_stmt=NULL;
			}
			/* Hopefully it will behave now :) */

			stmt->current_ap->lob_col_flag=0;
			stmt->current_ip->lob_col_flag=0;
			stmt->current_ap->bound_col_flag=0;
			stmt->current_ip->bound_col_flag=0;
			stmt->num_result_rows=0;
			stmt->bookmark=0;
			stmt->num_fetched_rows=-1;
			stmt->alt_fetch=NULL;
			if(stmt->alt_fetch_data)
			{
				free(stmt->alt_fetch_data);
				stmt->alt_fetch_data=NULL;
			}
			if(stmt->sql)
			{
				free(stmt->sql);
			    stmt->sql=NULL;
			}

            ood_ap_free(stmt->current_ap->recs.ap);
            ood_ar_free(stmt->current_ar->recs.ar);
            ood_ir_free(stmt->current_ir->recs.ir,stmt->current_ir->num_recs);
            ood_ip_free(stmt->current_ip->recs.ip,stmt->current_ip->num_recs);
            stmt->current_ap->recs.ap=NULL;
            stmt->current_ar->recs.ar=NULL;
            stmt->current_ip->recs.ip=NULL;
            stmt->current_ir->recs.ir=NULL;
            stmt->current_ir->num_recs=0;
            stmt->current_ip->num_recs=0;
			stmt->row_bind_offset_ptr=NULL;
			stmt->param_bind_offset_ptr=NULL;
            break;

        case SQL_DROP:
        status=_SQLFreeHandle(SQL_HANDLE_STMT,(SQLHANDLE)StatementHandle);
        break;

        case SQL_UNBIND:
            break;

        case SQL_RESET_PARAMS:
            break;
    }

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)stmt,status,"");
}
    return status;
#endif
}

SQLRETURN SQL_API SQLFreeStmt(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        Option )
{
    return _SQLFreeStmt(StatementHandle,Option);
}
