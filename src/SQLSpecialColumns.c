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
 * $Id: SQLSpecialColumns.c,v 1.3 2004/08/27 19:46:54 dbox Exp $
 *
 * $Log: SQLSpecialColumns.c,v $
 * Revision 1.3  2004/08/27 19:46:54  dbox
 * correct some bad behavior in ar/ir handles wrt number of records in re-used handles
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
 * Revision 1.11  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.10  2000/07/07 08:11:25  tom
 * initial implementation
 *
 * Revision 1.9  2000/06/06 10:23:04  tom
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
 * Revision 1.2  2000/04/19 15:29:22  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLSpecialColumns.c,v $ $Revision: 1.3 $";

sword ood_fetch_sqlspecialcolumns(struct hStmt_TAG* stmt)
{
	if(stmt->num_result_rows)
	{
	    if(stmt->row_status_ptr)
		{
			stmt->row_status_ptr[0]=SQL_NO_DATA;
		}
		return OCI_NO_DATA;
	}
	if(stmt->row_status_ptr)
	{
		stmt->row_status_ptr[0]=SQL_SUCCESS;
	    for(stmt->current_row=1;(unsigned)stmt->current_row<stmt->row_array_size;
			stmt->current_row++)
			stmt->row_status_ptr[stmt->current_row]=SQL_NO_DATA;
	}
	stmt->num_result_rows++;

	((SQLSMALLINT*)stmt->current_ir->recs.ir[1].data_ptr)[0]=
		SQL_SCOPE_SESSION;

	strcpy(stmt->current_ir->recs.ir[2].data_ptr,"ROWID");

	((SQLSMALLINT*)stmt->current_ir->recs.ir[3].data_ptr)[0]=
		SQL_C_BINARY;

	strcpy(stmt->current_ir->recs.ir[4].data_ptr,"ROWID");

	((SQLSMALLINT*)stmt->current_ir->recs.ir[5].data_ptr)[0]=10;
	((SQLSMALLINT*)stmt->current_ir->recs.ir[6].data_ptr)[0]=10;
	((SQLSMALLINT*)stmt->current_ir->recs.ir[7].data_ptr)[0]=0;
	((SQLSMALLINT*)stmt->current_ir->recs.ir[8].data_ptr)[0]=
		SQL_PC_PSEUDO;

	stmt->current_ir->recs.ir[1].ind_arr[0]=0;
	stmt->current_ir->recs.ir[2].ind_arr[0]=0;
	stmt->current_ir->recs.ir[3].ind_arr[0]=0;
	stmt->current_ir->recs.ir[4].ind_arr[0]=0;
	stmt->current_ir->recs.ir[5].ind_arr[0]=0;
	stmt->current_ir->recs.ir[6].ind_arr[0]=0;
	stmt->current_ir->recs.ir[7].ind_arr[0]=SQL_NULL_DATA;
	stmt->current_ir->recs.ir[8].ind_arr[0]=0;

	return(OCI_SUCCESS);
}


SQLRETURN SQL_API SQLSpecialColumns(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        IdentifierType,
    SQLCHAR                *CatalogName,
    SQLSMALLINT            NameLength1,
    SQLCHAR                *SchemaName,
    SQLSMALLINT            NameLength2,
    SQLCHAR                *TableName,
    SQLSMALLINT            NameLength3,
    SQLUSMALLINT        Scope,
    SQLUSMALLINT        Nullable )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
	ir_T* ir;
	ar_T* ar;
    SQLRETURN status=SQL_SUCCESS;
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
}
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

    /* Clear old data out of stmt->current_ir so it can be rebound.
       The data in stmt->current_ar must not be touched, since it
       may contain already bound ODBC columns. */

    ood_ir_array_reset (stmt->current_ir->recs.ir, stmt->current_ir->num_recs,
			stmt->current_ir);
	
    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,8,stmt->current_ir,
				       stmt->current_ar))
    {
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,SQL_ERROR,"");
}
        ood_mutex_unlock_stmt(stmt);
        return SQL_ERROR;
    }

    ir=stmt->current_ir->recs.ir;
    ar=stmt->current_ar->recs.ar;

    /* stmt->current_ir->num_recs is equal to the allocated size of the
       ir and ar arrays. Shouldn't expect it to record the number of
       bound parameters.  
       stmt->current_ir->num_recs=8; */
	

	/*
     * Col 0 is bookmark, not implemented yet
     */
    ir++,ar++;
    /*
     * Col 1 is SCOPE, INTEGER
     */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(int),0,
            ociint_sqlslong,ociint_sqlslong);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->octet_length=ar->length=sizeof(long);
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"SCOPE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));


    ir++,ar++;

	/*
     * Col 2 is NAME, VARCHAR(6);
     */
    status|=ood_assign_ir(ir,SQLT_STR,6,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->octet_length=ar->length=6;
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"COLUMN_NAME");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));

    ir++,ar++;

	/*
	 * Col 3 is data_type,integer
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_SLONG;
    ar->octet_length=ar->length=sizeof(long);
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"DATA_TYPE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));

    ir++,ar++;

	/*
	 * Col 4 is TYPE_NAME, varchar(6)
	 */
    status|=ood_assign_ir(ir,SQLT_STR,6,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->octet_length=ar->length=6;
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"TYPE_NAME");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));

    ir++,ar++;

	/*
	 * Col 5 is COLUMN_SIZE,INTEGER
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->octet_length=ar->length=sizeof(long);
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"COLUMN_SIZE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));

    ir++,ar++;

	/*
	 * Col 6 is BUFFER_LENGTH, integer
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->octet_length=ar->length=sizeof(long);
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"BUFFER_LENGTH");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));

    ir++,ar++;


	/*
	 * Col 7 is the DECIMAL_DIGITS, INTEGER
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->octet_length=ar->length=sizeof(long);
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"DECIMAL_DIGITS");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));

    ir++,ar++;


	/*
	 * Col 8 is PSEUDOCOL, INTEGER
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->octet_length=ar->length=sizeof(long);
    ar->display_size=sqltype_display_size(ar->data_type,ar->length);
    strcpy((char*)ar->column_name,"PSEUDO_COLUMN");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size);
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	
	if(IdentifierType!=SQL_BEST_ROWID)
		stmt->alt_fetch=ood_alt_fetch_no_data;
	else
	    stmt->alt_fetch=ood_fetch_sqlspecialcolumns;

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return SQL_SUCCESS;
}
