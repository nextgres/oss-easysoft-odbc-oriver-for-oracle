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
 * $Id: SQLForeignKeys.c,v 1.4 2004/08/27 19:41:21 dbox Exp $
 *
 * $Log: SQLForeignKeys.c,v $
 * Revision 1.4  2004/08/27 19:41:21  dbox
 * correct some bad behavior in ar/ir handles wrt number of records in re-used handles
 *
 * Revision 1.3  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.2  2002/03/05 22:55:50  dbox
 * added functionality to oracle_functions.c
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.12  2000/07/21 10:04:17  tom
 * Fixed var init bug
 *
 * Revision 1.11  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.10  2000/07/07 08:03:12  tom
 * initial implementation
 *
 * Revision 1.9  2000/06/06 10:23:38  tom
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
 * Revision 1.2  2000/04/19 15:26:23  tom
 * First Functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLForeignKeys.c,v $ $Revision: 1.4 $";

SQLRETURN SQL_API SQLForeignKeys (
    SQLHSTMT            StatementHandle,
    SQLCHAR                *PKCatalogName,
    SQLSMALLINT            NameLength1,
    SQLCHAR                *PKSchemaName,
    SQLSMALLINT            NameLength2,
    SQLCHAR                *PKTableName,
    SQLSMALLINT            NameLength3,
    SQLCHAR                *FKCatalogName,
    SQLSMALLINT            NameLength4,
    SQLCHAR                *FKSchemaName,
    SQLSMALLINT            NameLength5,
    SQLCHAR                *FKTableName,
    SQLSMALLINT            NameLength6 )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    char *pkschema=NULL,*pktable=NULL,*fkschema=NULL,
	    *fktable=NULL,*sql_end=NULL;
    SQLRETURN status;
    ir_T *ir;
    ar_T *ar;

/*
 * Enable User Catalog configure option really comes into its own here, it can
 * cut a ForeignKeys call down from severalminutes to under a second in 
 * some cases
 */
#ifdef ENABLE_USER_CATALOG
	char sql[512]="SELECT NULL,A.R_OWNER,B.TABLE_NAME,C.COLUMN_NAME,NULL,A.OWNER,A.TABLE_NAME,D.COLUMN_NAME,D.POSITION,NULL,NULL,A.CONSTRAINT_NAME,A.R_CONSTRAINT_NAME,NULL FROM USER_CONSTRAINTS A,USER_CONSTRAINTS B,USER_CONS_COLUMNS C,USER_CONS_COLUMNS D  WHERE A.CONSTRAINT_TYPE = 'r' AND  A.R_CONSTRAINT_NAME = B.CONSTRAINT_NAME AND C.CONSTRAINT_NAME = B.CONSTRAINT_NAME AND D.CONSTRAINT_NAME = A.CONSTRAINT_NAME AND C.POSITION = D.POSITION ";
#else
	char sql[512]="SELECT NULL,A.R_OWNER,B.TABLE_NAME,C.COLUMN_NAME,NULL,A.OWNER,A.TABLE_NAME,D.COLUMN_NAME,D.POSITION,NULL,NULL,A.CONSTRAINT_NAME,A.R_CONSTRAINT_NAME,NULL FROM ALL_CONSTRAINTS A,ALL_CONSTRAINTS B,ALL_CONS_COLUMNS C,ALL_CONS_COLUMNS D  WHERE A.CONSTRAINT_TYPE = 'r' AND  A.R_CONSTRAINT_NAME = B.CONSTRAINT_NAME  AND C.CONSTRAINT_NAME = B.CONSTRAINT_NAME AND D.CONSTRAINT_NAME = A.CONSTRAINT_NAME AND C.POSITION = D.POSITION ";
#endif
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"");
}

    ood_clear_diag((hgeneric*)stmt);

	pkschema=ood_xtoSQLNTS(PKSchemaName,NameLength2);
    pktable=ood_xtoSQLNTS(PKTableName,NameLength3);
	fkschema=ood_xtoSQLNTS(FKSchemaName,NameLength5);
    fktable=ood_xtoSQLNTS(FKTableName,NameLength6);
	
#ifdef UNIX_DEBUG    
fprintf(stderr,"SQLForeignKeys pkschema [%s], pktable [%s] \
fkschema [%s], fktable [%s]\n",pkschema,pktable,fkschema,fktable);
#endif

   if(pkschema&&*pkschema)
   {
        sql_end=ood_fast_strcat(sql," AND B.OWNER",sql_end);

        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ,",sql_end);
        else
            sql_end=ood_fast_strcat(sql," LIKE ",sql_end);

        if(*pkschema!='\'')
        {
            sql_end=ood_fast_strcat(sql,"'",sql_end);
            sql_end=ood_fast_strcat(sql,pkschema,sql_end);
            sql_end=ood_fast_strcat(sql,"'",sql_end);
        }
        else
            sql_end=ood_fast_strcat(sql,pkschema,sql_end);

        if(!stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," ESCAPE \'\\\'",sql_end);

    }
    if(fkschema&&*fkschema)
    {
        sql_end=ood_fast_strcat(sql," AND A.OWNER",sql_end);

        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ,",sql_end);
        else
            sql_end=ood_fast_strcat(sql," LIKE ",sql_end);

        if(*fkschema!='\'')
        {
            sql_end=ood_fast_strcat(sql,"'",sql_end);
            sql_end=ood_fast_strcat(sql,fkschema,sql_end);
            sql_end=ood_fast_strcat(sql,"'",sql_end);
        }
        else
            sql_end=ood_fast_strcat(sql,fkschema,sql_end);

        if(!stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," ESCAPE \'\\\'",sql_end);

    } 
    if(pktable&&*pktable)
    {
        sql_end=ood_fast_strcat(sql," AND B.TABLE_NAME",
					sql_end);
        
        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ",sql_end);
        else
            sql_end=ood_fast_strcat(sql," LIKE ",sql_end);

        if(*pktable!='\'')
        {
            sql_end=ood_fast_strcat(sql,"'",sql_end);
            sql_end=ood_fast_strcat(sql,pktable,sql_end);
            sql_end=ood_fast_strcat(sql,"'",sql_end);
        }
        else
            sql_end=ood_fast_strcat(sql,pktable,sql_end);

        if(!stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," ESCAPE \'\\\'",sql_end);
    }
	if(fktable&&*fktable)
    {
        sql_end=ood_fast_strcat(sql," AND A.TABLE_NAME",
					sql_end);
        
        if(stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," = ",sql_end);
        else
            sql_end=ood_fast_strcat(sql," LIKE ",sql_end);

        if(*fktable!='\'')
        {
            sql_end=ood_fast_strcat(sql,"'",sql_end);
            sql_end=ood_fast_strcat(sql,fktable,sql_end);
            sql_end=ood_fast_strcat(sql,"'",sql_end);
        }
        else
            sql_end=ood_fast_strcat(sql,fktable,sql_end);

        if(!stmt->dbc->metadata_id)
            sql_end=ood_fast_strcat(sql," ESCAPE \'\\\'",sql_end);
    }
#if defined(ENABLE_TRACE) && defined(UNIX_DEBUG)
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"s","SQL",sql);
#endif
	
    if(pkschema&&pkschema!=(char*)PKSchemaName)
        ORAFREE(pkschema);
    if(pktable&&pktable!=(char*)PKTableName)
        ORAFREE(pktable);
    if(fkschema&&fkschema!=(char*)FKSchemaName)
        ORAFREE(fkschema);
    if(fktable&&fktable!=(char*)FKTableName)
        ORAFREE(fktable);

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
    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,14,stmt->current_ir,
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
       stmt->current_ir->num_recs=14; */

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
    strcpy((char*)ar->column_name,"PKTABLE_CAT");

    ir++,ar++;

    /* 
     * Col 2 is TABLE_SHEM, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,SQL_MAX_SCHEMA_NAME_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=SQL_MAX_SCHEMA_NAME_LEN;
    ar->octet_length=ar->length=SQL_MAX_SCHEMA_NAME_LEN+1;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"PKTABLE_SCHEM");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /* 
     * Col 3 is TABLE_NAME, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,SQL_MAX_TABLE_NAME_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=SQL_MAX_TABLE_NAME_LEN;
    ar->octet_length=ar->length=SQL_MAX_TABLE_NAME_LEN+1;
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"PKTABLE_NAME");
    status|=ood_driver_define_col(ir);


    ir++,ar++;

    /*
     * Col 4 is COLUMN_NAME, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,SQL_MAX_COLUMN_NAME_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=SQL_MAX_COLUMN_NAME_LEN;
    ar->octet_length=ar->length=SQL_MAX_COLUMN_NAME_LEN+1;
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"PKCOLUMN_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;


    /*
     * Col 5 is TABLE_CAT, varchar, always NULL
     */
    status|=ood_assign_ir(ir,SQLT_STR,1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    status|=ood_driver_define_col(ir);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=0;
    ar->octet_length=ar->length=1;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"FKTABLE_CAT");

    ir++,ar++;

    /* 
     * Col 6 is TABLE_SHEM, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,SQL_MAX_SCHEMA_NAME_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=SQL_MAX_SCHEMA_NAME_LEN;
    ar->octet_length=ar->length=SQL_MAX_SCHEMA_NAME_LEN+1;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"FKTABLE_SCHEM");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /* 
     * Col 7 is TABLE_NAME, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,SQL_MAX_TABLE_NAME_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=SQL_MAX_TABLE_NAME_LEN;
    ar->octet_length=ar->length=SQL_MAX_TABLE_NAME_LEN+1;
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"FKTABLE_NAME");
    status|=ood_driver_define_col(ir);


    ir++,ar++;

    /*
     * Col 8 is COLUMN_NAME, varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,SQL_MAX_COLUMN_NAME_LEN+1,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=SQL_MAX_COLUMN_NAME_LEN;
    ar->octet_length=ar->length=SQL_MAX_COLUMN_NAME_LEN+1;
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"FKCOLUMN_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 9 is  KEY_SEQ smallint.
     */
    status|=ood_assign_ir(ir,SQLT_VNU,31,0,
            ocivnu_sqlslong,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NO_NULLS;
    strcpy((char*)ar->column_name,"KEY_SEQ");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 10 is update rule, smallint.
	 */
    status|=ood_assign_ir(ir,SQLT_VNU,31,0,
            ocivnu_sqlslong,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"UPDATE_RULE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

	/*
	 * Col 11 is delete rule, smallint.
	 */
    status|=ood_assign_ir(ir,SQLT_VNU,31,0,
            ocivnu_sqlslong,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"DELETE_RULE");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

    /*
     * Col 12 is FK_NAME , varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,31,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=32;
    ar->octet_length=ar->length=15;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"FK_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;
     
	/*
     * Col 13 is PK_NAME , varchar
     */
    status|=ood_assign_ir(ir,SQLT_STR,31,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_CHAR;
    ar->display_size=32;
    ar->octet_length=ar->length=15;
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"PK_NAME");
    status|=ood_driver_define_col(ir);

    ir++,ar++;

     /*
	 * Col 14 is delete rule, smallint.
	 */
    status|=ood_assign_ir(ir,SQLT_VNU,31,0,
            ocivnu_sqlslong,ocivnu_sqlnts);
    ar->data_type=ar->concise_type=SQL_C_LONG;
    ar->display_size=14;
    ar->octet_length=ar->length=sizeof(int);
	ar->nullable=SQL_NULLABLE;
    strcpy((char*)ar->column_name,"DELETE_RULE");
    status|=ood_driver_define_col(ir);

	stmt->fetch_status=ood_driver_prefetch(stmt);

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return SQL_SUCCESS;
}
