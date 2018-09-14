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
 * $Id: oracle_functions.h,v 1.9 2005/11/21 23:19:17 dbox Exp $
 *
 * $Log: oracle_functions.h,v $
 * Revision 1.9  2005/11/21 23:19:17  dbox
 * typo in function decl
 *
 * Revision 1.8  2005/11/19 01:21:10  dbox
 * changes to support BINARY_FLOAT and BINARY_DOUBLE oracle 10 data types
 *
 * Revision 1.7  2005/03/17 02:37:32  dbox
 * added function ocistr_memcpy to handle binary output
 *
 * Revision 1.6  2004/08/27 19:52:55  dbox
 * tons of changes by Gary Houston, ghouston@intellecthr.com.  Fixes error handling and handle re-use
 *
 * Revision 1.5  2004/08/06 20:46:10  dbox
 * lots of re-working of behavior for filling ir and ar descriptors when
 * a statement handle is reused.  Fixes a crash when number of columns changes
 * in a re-used statement handle.
 *
 * Revision 1.4  2004/06/10 16:28:40  dbox
 * fixed some annoying behavior in SQLConnect and SQLDriverConnect where it didnt always find the server name
 *
 * Revision 1.3  2002/06/19 22:21:37  dbox
 * more tweaks to OCI calls to report what happens when DEBUG level is set
 *
 * Revision 1.2  2002/03/08 22:07:19  dbox
 * added commit/rollback, more tests for SQLColAttribute
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.12  2000/07/21 10:19:03  tom
 * New LOB functions added
 *
 * Revision 1.11  2000/07/07 07:54:37  tom
 * prototype changes for block cursors
 *
 * Revision 1.10  2000/06/05 15:55:06  tom
 * New fn headers for bound params
 *
 * Revision 1.9  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.8  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.7  2000/04/28 09:01:27  tom
 * drv_type_to_sql_type added
 *
 * Revision 1.6  2000/04/26 15:26:12  tom
 * New conversion functions
 *
 * Revision 1.5  2000/04/26 11:11:32  tom
 * Changes to accomodate new defines structure
 *
 * Revision 1.4  2000/04/26 10:12:25  tom
 * move to ood_driver_ names
 * functions for SQLBindCol added
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:30:21  tom
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 15:48:23  tom
 * Added to cvs
 *
 *
 ******************************************************************************/

#ifndef _ORACLE_FUNCTIONS_H
#define _ORACLE_FUNCTIONS_H

SQLRETURN ood_driver_error(void *hH, sword ret,char*,int);
SQLRETURN ood_driver_connect(hDbc_T *dbc);
SQLRETURN ood_driver_disconnect(hDbc_T *dbc);
SQLRETURN ood_driver_prepare(hStmt_T* stmt,SQLCHAR *sql);
SQLRETURN ood_driver_execute(hStmt_T* stmt);
SQLRETURN ood_driver_transaction(hDbc_T *dbc, SQLSMALLINT action);
SQLRETURN ood_driver_execute_describe(hStmt_T* stmt);
SQLRETURN ood_driver_prefetch(hStmt_T* stmt);
SQLRETURN ood_driver_num_result_cols(hStmt_T* stmt,SQLSMALLINT* cols);
SQLRETURN ood_driver_setup_fetch_env(ir_T *ir, ar_T* ar);
void ood_ir_array_reset (ir_T *ir, size_t num_entries, hDesc_T *desc);
SQLRETURN ood_driver_define_col(ir_T* ir);
SQLRETURN ood_ocitype_to_sqltype(ub2 data_type);
SQLRETURN ood_ocitype_to_sqltype_imp(hStmt_T* stmt, int colNum);
SQLRETURN ood_alloc_col_desc(hStmt_T *, ub4, hDesc_T*, hDesc_T*);

SQLRETURN ood_assign_ir(ir_T *ir,ub2 data_type, ub2 data_size, sb2 indicator,
    SQLRETURN (*default_copy)(), SQLRETURN (*to_string)());
SQLRETURN (*ood_fn_default_copy(ub2 drvtype, SQLSMALLINT sqltype))
    (int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

SQLRETURN (*drv_type_to_string(ub2 drvtype, SQLSMALLINT sqltype))
    (int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

SQLRETURN ood_alloc_param_desc(hStmt_T *stmt,ub4 param_num,
		hDesc_T *imp,hDesc_T* app);



/*
 * SQLGetData type conversions functions.
 */
/* CHR */
SQLRETURN ocistr_memcpy(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocistr_sqlnts(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocistr_sqlchr(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

/* VNU */
SQLRETURN ocivnu_sqlnts(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlslong(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlulong(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqldouble(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlfloat(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlsshort(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlushort(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlsbigint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlubigint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlutinyint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlstinyint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocivnu_sqlnumeric(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

/* INT */
SQLRETURN ociint_sqlnts(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlslong(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlulong(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqldouble(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlfloat(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlsshort(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlushort(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlsbigint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlubigint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlutinyint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociint_sqlstinyint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

/* FLT */
SQLRETURN ociflt_sqlnts(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociflt_sqlfloat(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociflt_sqldouble(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ociflt_sqlnumeric(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

#ifdef IEEE_754_FLT
SQLRETURN ocibflt_sqlflt(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocibflt_sqldouble(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocibdbl_sqlfloat(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocibdbl_sqldouble(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
#endif

/* DAT */
SQLRETURN ocidat_sqlnts(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocidat_sqltimestamp(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocidat_sqltime(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocidat_sqldate(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

/* LOB */
SQLRETURN ocilob_sqllvc(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocilob_sqllvb(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

/* SPECIAL */
SQLRETURN ocidty_sqlint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocidty_sqlnts(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocinul_sqlint(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);
SQLRETURN ocinul_sqlnts(int,ir_T*,SQLPOINTER,SQLINTEGER,SQLINTEGER*);


#endif

SQLRETURN ood_xlate_status(sword stat);
sword ood_driver_bind_param(hStmt_T*,ub4);
char * oci_hdtype_name(ub4 hdtype);
char * oci_stmt_type_name(int stmt_type);
char * oci_status_name(sword status);
