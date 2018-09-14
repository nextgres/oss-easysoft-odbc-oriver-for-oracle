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
 * $Id: SQLColumns.c,v 1.8 2005/03/17 01:57:06 dbox Exp $
 *
 * $Log: SQLColumns.c,v 
 * Revision 1.7  2004/08/27 19:39:33  dbo
 * correct some bad behavior in ar/ir handles wrt number of records in re-used handle
 *
 * Revision 1.6  2004/06/11 20:10:43  dbox
 * fix to SQLColumns and friends
 *
 * Revision 1.5  2003/02/10 15:43:54  dbox
 * added unit test for SQLColumns, uppercased parameters to calls
 * to catalog functions
 *
 * Revision 1.4  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.3  2002/06/19 22:21:37  dbox
 * more tweaks to OCI calls to report what happens when DEBUG level is set
 *
 * Revision 1.2  2002/05/14 12:03:19  dbox
 * fixed some malloc/free syntax
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.18  2000/07/21 10:04:17  tom
 * Fixed var init bug
 *
 * Revision 1.17  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.16  2000/07/07 07:59:36  tom
 * changes for block cursors
 *
 * Revision 1.15  2000/06/08 10:10:27  tom
 * Fixed concise type not being set
 *
 * Revision 1.14  2000/06/01 16:22:58  tom
 * Fixed escape chars
 *
 * Revision 1.13  2000/05/11 13:20:36  tom
 * Couple of bugfixes
 *
 * Revision 1.12  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.11  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.10  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.9  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.8  2000/05/02 16:24:58  tom
 * resolved deadlock and a few tidy-ups
 *
 * Revision 1.7  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.6  2000/04/26 15:27:54  tom
 * More changes to account for SQLBindCol
 *
 * Revision 1.5  2000/04/26 11:11:32  tom
 * Changes to accomodate new defines structure
 *
 * Revision 1.4  2000/04/26 10:14:04  tom
 * Altered to account for bound columns
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:24:15  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLColumns.c,v $ $Revision: 1.8 $";

/*
 * Due to the fact that Oracle returns the data type as a varchar and
 * we need it as an integer, we need to preprocess the reult set.
 */

sword ood_fetch_sqlcolumns(struct hStmt_TAG* stmt)
{
	sword ret=OCI_SUCCESS;
	sword tmp_ret=OCI_SUCCESS;
	unsigned int row;
	SQLINTEGER dtype,*indie;
	SQLRETURN status=stmt->fetch_status;
	char *five,*fourteen;

	if(stmt->fetch_status==SQL_NO_DATA&&stmt->bookmark)
	{
		OCIAttrGet_log_stat(stmt->oci_stmt,OCI_HTYPE_STMT,
			    &stmt->num_result_rows,0,OCI_ATTR_ROW_COUNT,
			    stmt->dbc->oci_err,ret);
		stmt->num_fetched_rows=0;

		return(OCI_NO_DATA);
	}
	if(status!=SQL_NO_DATA&&stmt->bookmark) 
	{
	    /*
	     * stmt->bookmark would be nonzero if this wasn't the first call
	     * to SQLFetch. As OCIStmtExec==SQLexecute,SQLFetch we don't want
	     * to call OCIStmtFetch for the first call of SQLFetch.
	     * if status==SQL_NO_DATA we're trying to go past the end of the
	     * result set. If we were to call OCIStmtFetch now we'd get 
	     * an error.
	     */
         ret=OCIStmtFetch_log_stat(stmt->oci_stmt,stmt->dbc->oci_err,
	            stmt->row_array_size,OCI_FETCH_NEXT,
                OCI_DEFAULT,ret);

		 OCIAttrGet_log_stat(stmt->oci_stmt,OCI_HTYPE_STMT,
			    &stmt->num_result_rows,0,OCI_ATTR_ROW_COUNT,
			    stmt->dbc->oci_err,tmp_ret);

		 if(ret==OCI_NO_DATA)
		 {

			 stmt->num_fetched_rows=stmt->num_result_rows%stmt->row_array_size;
	         stmt->fetch_status=SQL_NO_DATA;
			 ret=OCI_SUCCESS;
		 }
    }
    else if(status==SQL_NO_DATA&&!stmt->bookmark) 
    {
	    /* 
	     * This means the OCIStmtExecute returned OCI_NO_DATA, ie the 
		 * result set<=array size. This is equivalent here to 
		 * OCIStmtFetch returning OCI_NO_DATA at the end of a larger 
		 * result set. 
	     */
		 OCIAttrGet_log_stat(stmt->oci_stmt,OCI_HTYPE_STMT,
			    &stmt->num_result_rows,0,OCI_ATTR_ROW_COUNT,
			    stmt->dbc->oci_err,ret);

		stmt->num_fetched_rows=stmt->num_result_rows;
	    stmt->fetch_status=SQL_NO_DATA;
	    ret=OCI_SUCCESS;
    }

	indie=ORAMALLOC(sizeof(int)*stmt->row_array_size);
	five=stmt->current_ir->recs.ir[5].data_ptr;
	fourteen=stmt->current_ir->recs.ir[14].data_ptr;

	for(row=0;row<stmt->row_array_size;row++)
	{

	    /*
	     * Columns 5 and 14 are primarily the ones we're interested in 
	     */
	    status=ocidty_sqlint(row,
				&stmt->current_ir->recs.ir[5],&dtype,
                sizeof(SQLINTEGER),
                indie);
	    
	    *((SQLINTEGER*)fourteen)=dtype;
		fourteen+=stmt->current_ir->recs.ir[5].data_size;

	    *((SQLINTEGER*)five)=dtype;
		five+=stmt->current_ir->recs.ir[5].data_size;
    
	}

	ORAFREE(indie);
	
	return ret;
}



SQLRETURN SQL_API SQLColumns(
    SQLHSTMT            StatementHandle,
    SQLCHAR                *CatalogName,
    SQLSMALLINT            NameLength1,
    SQLCHAR                *SchemaName,
    SQLSMALLINT            NameLength2,
    SQLCHAR                *TableName,
    SQLSMALLINT            NameLength3,
    SQLCHAR                *ColumnName,
    SQLSMALLINT            NameLength4 )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    char *schema=NULL,*table=NULL,*column=NULL,*sql_end=NULL;
    unsigned char has_where_clause=0;
    SQLRETURN status;
    ir_T *ir;
    ar_T *ar;

    /*
     * SQLColumns to select list is as follows
     * 1  CATALOG            -
     * 2  SCHEMA             OWNER
     * 3  TABLE_NAME         TABLE_NAME
     * 4  COL_NAME           COLUMN_NAME
     * 5  DATA_TYPE          DATA_TYPE 
     * 6  TYPE_NAME          DATA_TYPE
     * 7  COLUMN_SIZE        NVL(DATA_PRECISION,DATA_LENGTH)
     * 8  BUFFER_LENGTH      DATA_LENGTH
     * 9  DECIMAL_DIGITS     DATA_SCALE
     * 10 NUM_PREC_RADIX     10
     * 11 NULLABLE           REPLACE(...NULLABLE
     * 12 REMARKS            NULL (join with ALL_TAB_COMMENTS expensive)
     * 13 COLUMN_DEF         NULL (it's a LONG, we don't do them yet)
     * 14 SQL_DATA_TYPE      DATA_TYPE
     * 15 SQL_DATETIME_SUB   NULL (not implemented yet)
     * 16 CHAR_OCTET_LENGTH  DATA_LENGTH
     * 17 ORDINAL_POSITION   COLUMN_ID
     * 18 IS_NULLABLE        NULLABLE
     */
    
#ifdef ENABLE_USER_CATALOG
    char sql[512]="SELECT NULL, '', TABLE_NAME, COLUMN_NAME, DATA_TYPE, DATA_TYPE, NVL(DATA_PRECISION,DATA_LENGTH), DATA_LENGTH,DATA_SCALE, 10, REPLACE(REPLACE(NULLABLE,'Y',1),'N',0), ' ', NULL, DATA_TYPE, NULL, CHAR_COL_DECL_LENGTH, COLUMN_ID, REPLACE(REPLACE(NULLABLE,'N','NO'),'Y','YES') FROM USER_TAB_COLUMNS";
#else
    char sql[512]="SELECT NULL, OWNER, TABLE_NAME, COLUMN_NAME, DATA_TYPE, DATA_TYPE, NVL(DATA_PRECISION,DATA_LENGTH), DATA_LENGTH,DATA_SCALE, 10, REPLACE(REPLACE(NULLABLE,'Y',1),'N',0), ' ', NULL, DATA_TYPE, NULL, CHAR_COL_DECL_LENGTH, COLUMN_ID, REPLACE(REPLACE(NULLABLE,'N','NO'),'Y','YES') FROM ALL_TAB_COLUMNS";
#endif

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
}
    ood_clear_diag((hgeneric*)stmt);

    schema=ood_xtoSQLNTS(SchemaName,NameLength2);
    table=ood_xtoSQLNTS(TableName,NameLength3);
    column=ood_xtoSQLNTS(ColumnName,NameLength4);

    if(getenv("VERBOSE"))    
	fprintf(stderr,"SQLColumns schema [%s], table [%s], column [%s]\n",
		schema,table,column);

    if(schema&&*schema)
    {
        sql_end=ood_fast_strcat(sql," WHERE OWNER",sql_end);

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
    if(column&&*column)
    {
        if(has_where_clause)
            sql_end=ood_fast_strcat(sql," AND COLUMN_NAME",sql_end);
        else
            sql_end=ood_fast_strcat(sql," WHERE COLUMN_NAME",sql_end);
        
        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ",sql_end);
        else
            sql_end=ood_fast_strcat(sql," LIKE ",sql_end);

        if(*column!='\'')
        {
            sql_end=ood_fast_strcat(sql,"'",sql_end);
            sql_end=ood_fast_strcat(sql,column,sql_end);
            sql_end=ood_fast_strcat(sql,"'",sql_end);
        }
        else
           sql_end=ood_fast_strcat(sql,column,sql_end);

        if(!stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," ESCAPE \'\\\'",sql_end);
        has_where_clause=1;
    }
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"s","SQL",sql);
}
    if(schema&&schema!=(char*)SchemaName)
        ORAFREE(schema);
    if(table&&table!=(char*)TableName)
        ORAFREE(table);
    if(column&&column!=(char*)ColumnName)
        ORAFREE(column);

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

	/*
	 * Due to the fact that Oracle returns the data type as a string and
	 * we have to return as an integer, we need to preprocess the result
	 * set using an alternative fetc function.
	 */
	stmt->alt_fetch=ood_fetch_sqlcolumns;

    /* Clear old data out of stmt->current_ir so it can be rebound.
       The data in stmt->current_ar must not be touched, since it
       may contain already bound ODBC columns. */

    ood_ir_array_reset (stmt->current_ir->recs.ir, stmt->current_ir->num_recs,
			stmt->current_ir);
	
    /*
     * Now we have to set up the columns for retrieval
     */
    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,18,stmt->current_ir,
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
       stmt->current_ir->num_recs=18; */

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
	ar->nullable=SQL_NULLABLE;
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
     * Col 4 is COLUMN_NAME, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,ORACLE_MAX_COLUMN_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=ORACLE_MAX_COLUMN_LEN;
    ar->octet_length=ar->length=ORACLE_MAX_COLUMN_LEN+1;
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"COLUMN_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 5 is data type, SQL_SMALLINT
     */
    status|=ood_assign_ir(ir,SQLT_STR,31,0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"SQL_DATA_TYPE");
    status|=ood_driver_define_col(ir);
	
	/*
	 * If we left Col5 as is, and then tried to bind to char it
	 * would look at the ir type (SQLT_STR) and bind completely the
	 * wrong function! Now we've bound it as far as Oracle is
	 * concerned we can tweak it to something sensible.
	 */
	ir->data_type=SQLT_INT;

    ir++,ar++;

    /*
     * Col 6 is type name, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,31,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=14;
    ar->octet_length=ar->length=15;
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"TYPE_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /* 
     * Col 7 is COLUMN_SIZE or precision.
     */
    status|=ood_assign_ir(ir,SQLT_VNU,22,0,
            ocivnu_sqlslong,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(SQLINTEGER);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"COLUMN_SIZE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 8 is BUFFER_LENGTH, integer
     */
    status|=ood_assign_ir(ir,SQLT_VNU,22,0,
            ocivnu_sqlslong,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(SQLINTEGER);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"BUFFER_LENGTH");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 9 is DECIMAL_DIGITS, integer
     */
    status|=ood_assign_ir(ir,SQLT_VNU,22,0,
            ocivnu_sqlsshort,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(SQLINTEGER);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"DECIMAL_DIGITS");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 10 is NUM_PREC_RADIX
     */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(int),0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"NUM_PREC_RADIX");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 11 is the NULLABLE contingent
     */
    status|=ood_assign_ir(ir,SQLT_VNU,31,0,
            ocivnu_sqlsshort,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(SQLINTEGER);
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"NULLABLE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 12 is remarks :- this would involve a join with ALL_TAB_COMMENTS
     * which would be expensive. Implement it if you need it.
     */
    status|=ood_assign_ir(ir,SQLT_STR,33,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=32;
    ar->octet_length=ar->length=33;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"REMARKS");
    status|=ood_driver_define_col(ir);

    ir++,ar++;
    
    /*
     * Col 13 is column_def. Oracle stores this as a LONG and as LONGs 
     * haven't been implemented yet, this is a NULL
     */
    status|=ood_assign_ir(ir,SQLT_STR,31,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=1;
    ar->octet_length=ar->length=2;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"COLUMN_DEF");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 14 is SQL_DATA_TYPE, it's the same as col 5
     */
    status|=ood_assign_ir(ir,SQLT_STR,31,0,
            ociint_sqlsshort,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"SQL_DATA_TYPE");
    status|=ood_driver_define_col(ir);
	/*
	 * See comment after col5
	 */
	ir->data_type=SQLT_INT;

    ir++,ar++;

    /*
     * Col 15 is the DATE_TIME_SUB, integer.
     * We don't do this yet.
     */
    status|=ood_assign_ir(ir,SQLT_STR,sizeof(int),0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=0;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"DATE_TIME_SUB");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 16 is CHAR_OCTET_LENGTH, integer. 
     * This is the same as BUFFER_LENGTH (which is not strictly correct)
     */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(int),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"CHAR_OCTET_LENGTH");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /* 
     * Col 17 is ORDINAL_POSITION,integer
     */
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(int),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"ORDINAL_POSITION");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * And finally, col 18 is the nullable contingent again, this
     * time as a string
     */
    status|=ood_assign_ir(ir,SQLT_STR,31,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=3;
    ar->octet_length=ar->length=4;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"IS_NULLABLE");
    status|=ood_driver_define_col(ir);

	stmt->fetch_status=ood_driver_prefetch(stmt);

if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,status,"");
}
    ood_mutex_unlock_stmt(stmt);
    return status;
}
