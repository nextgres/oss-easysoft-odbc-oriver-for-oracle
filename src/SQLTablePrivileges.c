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
 * $Id: SQLTablePrivileges.c,v 1.3 2004/08/27 19:48:50 dbox Exp $
 *
 * $Log: SQLTablePrivileges.c,v $
 * Revision 1.3  2004/08/27 19:48:50  dbox
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
 * Revision 1.12  2000/07/21 10:04:17  tom
 * Fixed var init bug
 *
 * Revision 1.11  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.10  2000/07/07 08:12:38  tom
 * initial implementation
 *
 * Revision 1.9  2000/06/06 10:22:53  tom
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
 * Revision 1.2  2000/04/19 15:29:32  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLTablePrivileges.c,v $ $Revision: 1.3 $";

SQLRETURN SQL_API SQLTablePrivileges( 
    SQLHSTMT            StatementHandle,
    SQLCHAR                *CatalogName,
    SQLSMALLINT            NameLength1,
    SQLCHAR                *SchemaName,
    SQLSMALLINT            NameLength2,
    SQLCHAR                *TableName,
    SQLSMALLINT            NameLength3 )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
#ifdef ENABLE_USER_CATALOG
    char sql[512]="SELECT NULL, '', TABLE_NAME, GRANTOR, GRANTEE, PRIVILEGE, GRANTABLE FROM USER_TAB_PRIVS";
#else
    char sql[512]="SELECT NULL, TABLE_SCHEMA, TABLE_NAME, GRANTOR, GRANTEE, PRIVILEGE, GRANTABLE FROM ALL_TAB_PRIVS";
#endif
	char *schema=NULL,*table=NULL,*sql_end=NULL;
    SQLRETURN status=SQL_SUCCESS;
	int has_where_clause=0;
	ir_T *ir;
    ar_T *ar;

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
}
    ood_clear_diag((hgeneric*)stmt);

	schema=ood_xtoSQLNTS(SchemaName,NameLength2);
    table=ood_xtoSQLNTS(TableName,NameLength3);

#ifdef UNIX_DEBUG    
fprintf(stderr,"SQLTablePrivileges schema [%s], table [%s]\n",schema,table);
#endif

    if(schema&&*schema)
    {
        sql_end=ood_fast_strcat(sql," WHERE TABLE_SCHEMA",sql_end);

        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ,",sql_end);
        else
            sql_end=ood_fast_strcat(sql," LIKE ",sql_end);

        if(*schema!='\'')
        {
            sql_end=ood_fast_strcat(sql,"'",sql_end);
            sql_end=ood_fast_strcat(sql,schema,sql_end);
            sql_end=ood_fast_strcat(sql,"'",sql_end);
        }
        else
            sql_end=ood_fast_strcat(sql,schema,sql_end);

        if(!stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," ESCAPE \'\\\'",sql_end);

        has_where_clause=1;
    }
    if(table&&*table)
    {
        if(has_where_clause)
            sql_end=ood_fast_strcat(sql," AND TABLE_NAME",sql_end);
        else
            sql_end=ood_fast_strcat(sql," WHERE TABLE_NAME",sql_end);
        
        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ",sql_end);
        else
            sql_end=ood_fast_strcat(sql," LIKE ",sql_end);

        if(*table!='\'')
        {
            sql_end=ood_fast_strcat(sql,"'",sql_end);
            sql_end=ood_fast_strcat(sql,table,sql_end);
            sql_end=ood_fast_strcat(sql,"'",sql_end);
        }
        else
            sql_end=ood_fast_strcat(sql,table,sql_end);

        if(!stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," ESCAPE \'\\\'",sql_end);
        has_where_clause=1;
    }

#if defined(ENABLE_TRACE) && defined(UNIX_DEBUG)
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"s","SQL",sql);
#endif

	if(schema&&schema!=(char*)SchemaName)
        ORAFREE(schema);
    if(table&&table!=(char*)TableName)
        ORAFREE(table);

    ood_mutex_lock_stmt(stmt);
	status=ood_driver_prepare(stmt,(unsigned char*)sql);
	if(!status)
        status=ood_driver_execute(stmt);

    if(status)
    {
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,status,"");
}
        ood_mutex_unlock_stmt(stmt);
        return status;
    }
    
    /* Clear old data out of stmt->current_ir so it can be rebound.
       The data in stmt->current_ar must not be touched, since it
       may contain already bound ODBC columns. */

    ood_ir_array_reset (stmt->current_ir->recs.ir, stmt->current_ir->num_recs,
			stmt->current_ir);
	
    /*
     * Now we have to set up the columns for retrieval
     */
    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,7,stmt->current_ir,
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
       stmt->current_ir->num_recs=7; */

    /*
     * Col 0 is bookmark, not implemented yet
     */
    ir++,ar++;
    /*
     * Col 1 is TABLE_CAT, varchar, always NULL
     */
    status|=ood_assign_ir(ir,SQLT_STR,1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    status|=ood_driver_define_col(ir);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=0;
    ar->octet_length=ar->length=1;
    strcpy((char*)ar->column_name,"TABLE_CAT");

    ir++,ar++;

    /* 
     * Col 2 is TABLE_SHEM, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_SCHEMA_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_SCHEMA_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_SCHEMA_LEN+1;
    strcpy((char*)ar->column_name,"TABLE_SCHEM");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /* 
     * Col 3 is TABLE_NAME, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_TABLE_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_TABLE_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_TABLE_LEN+1;
    strcpy((char*)ar->column_name,"TABLE_NAME");
    status|=ood_driver_define_col(ir);


    ir++,ar++;

	/*
	 * Col 4 is grantor, varchar
	 */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_TABLE_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_TABLE_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_TABLE_LEN+1;
    strcpy((char*)ar->column_name,"GRANTOR");
    status|=ood_driver_define_col(ir);


    ir++,ar++;

	/*
	 * Col 5 is grantee, varchar
	 */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_TABLE_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_TABLE_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_TABLE_LEN+1;
    strcpy((char*)ar->column_name,"GRANTEE");
    status|=ood_driver_define_col(ir);



    ir++,ar++;

	/*
	 * Col 6 is privilege, varchar
	 */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_TABLE_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_TABLE_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_TABLE_LEN+1;
    strcpy((char*)ar->column_name,"PRIVILEGE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 7 is is_grantable (YES/NO) varchar
	 */
    status|=ood_assign_ir(ir,SQLT_STR,4,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=3;
    ar->octet_length=ar->length=4;
    strcpy((char*)ar->column_name,"IS_GRANTABLE");
    status|=ood_driver_define_col(ir);

	stmt->fetch_status=ood_driver_prefetch(stmt);

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return SQL_SUCCESS;
}
