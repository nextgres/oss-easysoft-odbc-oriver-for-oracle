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
 * $Id: SQLStatistics.c,v 1.3 2004/08/27 19:48:14 dbox Exp $
 *
 * $Log: SQLStatistics.c,v $
 * Revision 1.3  2004/08/27 19:48:14  dbox
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
 * Revision 1.15  2000/07/21 10:04:17  tom
 * Fixed var init bug
 *
 * Revision 1.14  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.13  2000/07/07 08:11:45  tom
 * block cursor updates
 *
 * Revision 1.12  2000/06/08 10:48:20  tom
 * SLONGs changed to LONGs
 *
 * Revision 1.11  2000/06/08 10:10:27  tom
 * Fixed concise type not being set
 *
 * Revision 1.10  2000/06/06 12:20:32  tom
 * More escape char antics
 *
 * Revision 1.9  2000/06/01 16:22:58  tom
 * Fixed escape chars
 *
 * Revision 1.8  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.7  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
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
 * Revision 1.2  2000/04/19 15:29:27  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLStatistics.c,v $ $Revision: 1.3 $";

/*
 * NOTE, this completely ignores the Reserved param ATM
 */


SQLRETURN SQL_API SQLStatistics(
    SQLHSTMT            StatementHandle,
    SQLCHAR                *CatalogName,
    SQLSMALLINT            NameLength1,
    SQLCHAR                *SchemaName,
    SQLSMALLINT            NameLength2,
    SQLCHAR                *TableName,
    SQLSMALLINT            NameLength3,
    SQLUSMALLINT        Unique,
    SQLUSMALLINT        Reserved )
{
    SQLRETURN status=SQL_SUCCESS;
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    char *schema=NULL,*table=NULL,*sql_end=NULL;
    ir_T *ir;
    ar_T *ar;

    /*
     * SQLStatistics to query
     *
     * CATALOG                  NULL
     * SCHEMA                   TABLE_OWNER
     * TABLE_NAME               TABLE_NAME
     * NON_UNIQUE               UNIQUENESS
     * INDEX_QUALIFIER          OWNER
     * INDEX_NAME               INDEX_NAME
     * TYPE                     SQL_INDEX_OTHER
     * ORDINAL_POSITION         COLUMN_POSITION
     * COLUMN_NAME              COLUMN_NAME
     * ASC_OR_DESC              DESCEND
     * CARDINALITY              NULL (put this in if you want, 
     *                                            it's ALL_INDEXES.NUM_ROWS)
     * PAGES                    NULL 
     * FILTER_CONDITION         NULL
     */
    char sql[1024];

    ood_clear_diag((hgeneric*)stmt);
    
#ifdef ENABLE_USER_CATALOG
    sprintf(sql,"SELECT NULL,'',USER_INDEXES.TABLE_NAME,REPLACE(REPLACE(USER_INDEXES.UNIQUENESS,'NONUNIQUE',%d),'UNIQUE',%d),NULL,USER_INDEXES.INDEX_NAME,%d,USER_IND_COLUMNS.COLUMN_POSITION,USER_IND_COLUMNS.COLUMN_NAME,REPLACE(REPLACE(USER_IND_COLUMNS.DESCEND,'DESC','D'),'ASC','A'),NULL,NULL,NULL FROM USER_INDEXES, USER_IND_COLUMNS WHERE USER_INDEXES.TABLE_NAME=USER_IND_COLUMNS.TABLE_NAME AND USER_INDEXES.INDEX_NAME=USER_IND_COLUMNS.INDEX_NAME",
            SQL_FALSE,SQL_TRUE,SQL_INDEX_OTHER);
#else
    sprintf(sql,"SELECT NULL,ALL_INDEXES.TABLE_OWNER,ALL_INDEXES.TABLE_NAME,REPLACE(REPLACE(ALL_INDEXES.UNIQUENESS,'NONUNIQUE',%d),'UNIQUE',%d),NULL,ALL_INDEXES.INDEX_NAME,%d,ALL_IND_COLUMNS.COLUMN_POSITION,ALL_IND_COLUMNS.COLUMN_NAME,REPLACE(REPLACE(ALL_IND_COLUMNS.DESCEND,'DESC','D'),'ASC','A'),NULL,NULL,NULL FROM ALL_INDEXES, ALL_IND_COLUMNS WHERE ALL_INDEXES.TABLE_OWNER=ALL_IND_COLUMNS.TABLE_OWNER AND ALL_INDEXES.TABLE_NAME=ALL_IND_COLUMNS.TABLE_NAME AND ALL_INDEXES.OWNER=ALL_IND_COLUMNS.INDEX_OWNER AND ALL_INDEXES.INDEX_NAME=ALL_IND_COLUMNS.INDEX_NAME",
            SQL_FALSE,SQL_TRUE,SQL_INDEX_OTHER);
#endif

    schema=ood_xtoSQLNTS(SchemaName,NameLength2);
    table=ood_xtoSQLNTS(TableName,NameLength3);

    if(schema&&*schema)
    {
#ifdef ENABLE_USER_CATALOG
        sql_end=ood_fast_strcat(sql," AND USER_INDEXES.TABLE_OWNER",sql_end);
#else
        sql_end=ood_fast_strcat(sql," AND ALL_INDEXES.TABLE_OWNER",sql_end);
#endif

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
    }
    if(table&&*table)
    {
#ifdef ENABLE_USER_CATALOG
        sql_end=ood_fast_strcat(sql," AND USER_INDEXES.TABLE_NAME",sql_end);
#else
        sql_end=ood_fast_strcat(sql," AND ALL_INDEXES.TABLE_NAME",sql_end);
#endif
        
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
    }
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"s","SQL",sql);
}

    if(schema&&schema!=(char*)SchemaName)
        ORAFREE(schema);
    if(table&&table!=(char*)TableName)
        ORAFREE(table);

    ood_mutex_lock_stmt(stmt);

    status=ood_driver_prepare(stmt,(unsigned char*)sql);
    status|=ood_driver_execute(stmt);
    
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
    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,13,stmt->current_ir,
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
       stmt->current_ir->num_recs=13; */

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
    ar->octet_length=ar->length=2;
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
	 * Col 4 is NON_UNIQUE, smallint
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(SQLSMALLINT);
    strcpy((char*)ar->column_name,"NON_UNIQUE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 5 is INDEX_QUALIFIER, varchar;
	 */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_TABLE_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_TABLE_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_TABLE_LEN+1;
    strcpy((char*)ar->column_name,"INDEX_QUALIFIER");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 6 is INDEX_NAME, another varchar
	 */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_COLUMN_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_COLUMN_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_COLUMN_LEN+1;
    strcpy((char*)ar->column_name,"INDEX_QUALIFIER");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 7 is TYPE, SMALLINT
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(SQLSMALLINT);
    strcpy((char*)ar->column_name,"TYPE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/* 
	 * Col 8 is ORDINAL_POSITION, another SMALLINT
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(SQLSMALLINT);
    strcpy((char*)ar->column_name,"TYPE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 9 is COLUMN_NAME, varchar
	 */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_COLUMN_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_COLUMN_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_COLUMN_LEN+1;
    strcpy((char*)ar->column_name,"COLUMN_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 10 is ASC_OR_DESC, char(1)
	 */
    status|=ood_assign_ir(ir,SQLT_STR,4,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=1;
    ar->octet_length=ar->length=2;
    strcpy((char*)ar->column_name,"ASC_OR_DESC");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 11 is CARDINALITY, Integer
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=sqltype_display_size(SQL_C_LONG,0);
    ar->octet_length=ar->length=sizeof(SQLINTEGER);
    strcpy((char*)ar->column_name,"CARDINALITY");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 12 is PAGES, integer
	 */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=sqltype_display_size(SQL_C_LONG,0);
    ar->octet_length=ar->length=sizeof(SQLINTEGER);
    strcpy((char*)ar->column_name,"PAGES");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 13 is FILTER_CONDITION, varchar (always NULL)
	 */
    status|=ood_assign_ir(ir,SQLT_STR,2,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=1;
    ar->octet_length=ar->length=2;
    strcpy((char*)ar->column_name,"FILTER_CONDITION");
    status|=ood_driver_define_col(ir);

	stmt->fetch_status=ood_driver_prefetch(stmt);

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}
