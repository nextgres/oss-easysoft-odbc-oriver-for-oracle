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
 * $Id: SQLTables.c,v 1.4 2004/08/27 19:49:34 dbox Exp $
 *
 * $Log: SQLTables.c,v $
 * Revision 1.4  2004/08/27 19:49:34  dbox
 * correct some bad behavior in ar/ir handles wrt number of records in re-used handles
 *
 * Revision 1.3  2003/01/27 21:06:50  dbox
 * WTF is COPYING doing in here, its already EPL it cant be GPL too!
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
 * Revision 1.16  2000/07/21 10:04:17  tom
 * Fixed var init bug
 *


 * Revision 1.15  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.14  2000/07/07 07:47:37  tom
 * Altered user catalog support
 * Chnages for block cursors
 *
 * Revision 1.13  2000/06/08 10:10:27  tom
 * Fixed concise type not being set
 *
 * Revision 1.12  2000/06/01 16:22:31  tom
 * Fixed escape chars
 *
 * Revision 1.11  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.10  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
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
 * Revision 1.6  2000/04/26 15:33:37  tom
 * Brought in line with new bound columns and IR population inplementations
 *
 * Revision 1.4  2000/04/26 10:19:03  tom
 * Changes to account for bound columns
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:29:37  tom
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *****************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLTables.c,v $ $Revision: 1.4 $";

static void ood_sqltables_construct_sql(hStmt_T* stmt,char* sql, char* schema, 
		char *table, char *type, char* sql_end, int has_where_clause)
{
#ifdef ENABLE_USER_CATALOG
	const char sql_base[]="SELECT NULL, '', TABLE_NAME, TABLE_TYPE, ' ' FROM USER_CATALOG ";
#else
	const char sql_base[]="SELECT NULL, OWNER, TABLE_NAME, TABLE_TYPE, ' ' FROM ALL_CATALOG";
#endif
	sql_end=ood_fast_strcat(sql,(char*)sql_base,sql_end);
#ifndef ENABLE_USER_CATALOG
    if(schema&&*schema)
    {
        sql_end=ood_fast_strcat(sql," WHERE OWNER",sql_end);

        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ",sql_end);
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
#endif
    if(table&&*table)
    {
        if(has_where_clause)
            sql_end=ood_fast_strcat(sql," AND TABLE_NAME",sql_end);
        else
            sql_end=ood_fast_strcat(sql," WHERE TABLE_NAME",
					sql_end);
        
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
    if(type&&*type)
    {
        char *cptr=type;
        if(has_where_clause)
            sql_end=ood_fast_strcat(sql," AND TABLE_TYPE = '",
					sql_end);
        else
            sql_end=ood_fast_strcat(sql," WHERE TABLE_TYPE = '",
					sql_end);
        do
        {
            if(*cptr==',')
	            break;
            else
            {
                if(*cptr!='\'')
                    *sql_end++=*cptr;
            }
            cptr++;
        }while(*cptr);
        *sql_end++='\'';
        *sql_end='\0';
		if(*cptr) 
		{
			cptr++;
			sql_end=ood_fast_strcat(sql," UNION ",sql_end);
			ood_sqltables_construct_sql(stmt,sql,schema,table,cptr,
					sql_end,has_where_clause);
		}
    }
}

SQLRETURN SQL_API SQLTables( 
    SQLHSTMT            StatementHandle,
    SQLCHAR                *CatalogName,
    SQLSMALLINT            NameLength1,
    SQLCHAR                *SchemaName,
    SQLSMALLINT            NameLength2,
    SQLCHAR                *TableName,
    SQLSMALLINT            NameLength3,
    SQLCHAR                *TableType,
    SQLSMALLINT            NameLength4 )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    char *schema=NULL,*table=NULL,*type=NULL,*sql_end=NULL;
    char sql[1024]="";
    SQLRETURN status;
    ir_T *ir;
    ar_T *ar;

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
}
    ood_clear_diag((hgeneric*)stmt);

    if(SchemaName&&!*SchemaName
            &&TableName&&!*TableName
            &&CatalogName&&!strcmp((const char*)CatalogName,SQL_ALL_CATALOGS))
    {
        /*
         * This can never return any inormation, but there needs to
         * be a query, so use one which will never return anything
         */
        strcpy(sql,"SELECT NULL,NULL,NULL,NULL,NULL FROM DUAL WHERE ROWNUM<0");
    }
    else if(TableName&&!*TableName
            &&SchemaName&&!strcmp((const char*)SchemaName,SQL_ALL_SCHEMAS))
    {
        /*
         * All schemas
         */
        strcpy(sql,
              "SELECT DISTINCT NULL,OWNER,NULL,NULL,NULL FROM ALL_CATALOG");
    }
    else if(SchemaName&&!*SchemaName
            &&TableName&&!*TableName
            &&TableType&&!strcmp((const char*)TableType,SQL_ALL_TABLE_TYPES))
    {
        /*
         * All table types 
         */
        strcpy(sql,
          "SELECT DISTINCT NULL,NULL,NULL,TABLE_TYPE,NULL FROM ALL_CATALOG");
    }
    else
	{
        schema=ood_xtoSQLNTS(SchemaName,NameLength2);
        table=ood_xtoSQLNTS(TableName,NameLength3);
        type=ood_xtoSQLNTS(TableType,NameLength4);
    
	ood_sqltables_construct_sql(stmt,sql,schema,table,type,sql_end,0);
        
	if(debugLevel()> 2){
	  fprintf(stderr,
		  "SQLTables schema [%s][%d], table [%s][%d], type [%s][%d] SQL [%s]\n",
		  schema,NameLength2,table,NameLength3,type,NameLength4,sql);
	  fprintf(stderr,"schema [0x%.8lx] SchemaName [0x%.8lx] NameLength2 [%d]\n",
		  (long)schema,(long)SchemaName,NameLength2);
	}
        if(schema&&(schema!=(char*)SchemaName))/*malloc'd*/
            ORAFREE(schema);
        if(table&&(table!=(char*)TableName))/*malloc'd*/
            ORAFREE(table);
        if(type&&(type!=(char*)TableType))/*malloc'd*/
            ORAFREE(type);
	}

    ood_mutex_lock_stmt(stmt);
    status=ood_driver_prepare(stmt,(unsigned char*)sql);
    status|=ood_driver_execute(stmt);

    /* Clear old data out of stmt->current_ir so it can be rebound.
       The data in stmt->current_ar must not be touched, since it
       may contain already bound ODBC columns. */

    ood_ir_array_reset (stmt->current_ir->recs.ir, stmt->current_ir->num_recs,
			stmt->current_ir);
	
    /*
     * Now we have to set up the columns for retrieval
     */
    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,5,stmt->current_ir,
				stmt->current_ar))
    {
        ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,SQL_ERROR,"");
}
        return SQL_ERROR;
    }

    ir=stmt->current_ir->recs.ir;
    ar=stmt->current_ar->recs.ar;

    /* stmt->current_ir->num_recs is equal to the allocated size of the
       ir and ar arrays. Shouldn't expect it to record the number of
       bound parameters.  
       stmt->current_ir->num_recs=5; */

    /*
     * Col 0 is bookmark, not implemented yet at all
     */
    ir++,ar++;

    /*
     * Col 1 is TABLE_CAT, varchar, always NULL
     */
    status|=ood_assign_ir(ir,SQLT_STR,2,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=1;
    ar->octet_length=ar->length=30;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"TABLE_CAT");
    status|=ood_driver_define_col(ir);
    ir++,ar++;

    /* 
     * Col 2 is TABLE_SHEM, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_SCHEMA_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_SCHEMA_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_SCHEMA_LEN+1;
	ar->nullable=SQL_NO_NULLS;
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
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"TABLE_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 4 is TABLE_TYPE
     */
    status|=ood_assign_ir(ir,SQLT_STR,64,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=63;
    ar->octet_length=ar->length=64;
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"TABLE_TYPE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /* 
     * Col 5 is REMARKS.
     * This is expensive to bring back and not often used so we don't
     * bother.
     */
    status|=ood_assign_ir(ir,SQLT_STR,32,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=32;
    ar->octet_length=ar->length=33;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"REMARKS");
    status|=ood_driver_define_col(ir);

	stmt->fetch_status=ood_driver_prefetch(stmt);

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}
