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
 * $Id: SQLGetInfo.c,v 1.9 2004/08/27 19:44:25 dbox Exp $
 *
 * $Log: SQLGetInfo.c,v $
 * Revision 1.9  2004/08/27 19:44:25  dbox
 * correct a rollback/commit state
 *
 * Revision 1.8  2003/02/18 19:25:23  dbox
 * fixed a bunch of return type errors
 *
 * Revision 1.7  2003/01/08 22:30:30  dbox
 * fixed some functionality for SQLGetInfo
 *
 * Revision 1.6  2002/11/14 22:28:36  dbox
 * %$@*&%$??!
 *
 * Revision 1.5  2002/08/14 04:02:25  dbox
 * cleaned up SQLGetInfo.c and its unit test
 *
 * Revision 1.4  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.3  2002/05/14 23:01:06  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.2  2002/04/03 01:21:44  dbox
 * reports that it supports transactions
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.10  2000/07/21 10:10:51  tom
 * Forward only cursor attributes added
 *
 * Revision 1.9  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.8  2000/07/07 08:07:26  tom
 * lots more debugging
 * A few more info sections implemented
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
 * Revision 1.2  2000/04/19 15:27:37  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetInfo.c,v $ $Revision: 1.9 $";

SQLRETURN SQL_API SQLGetInfo(
			     SQLHDBC                ConnectionHandle,
			     SQLUSMALLINT        InfoType,
			     SQLPOINTER            InfoValuePtr,
			     SQLSMALLINT            BufferLength,
			     SQLSMALLINT            *StringLengthPtr )
{
  hDbc_T* dbc=(hDbc_T*)ConnectionHandle;
  SQLRETURN status=SQL_SUCCESS;
  assert(IS_VALID(dbc));
  if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		    (SQLHANDLE)dbc,0,"s","InfoType",sql_get_info_type(InfoType));
  }
  ood_clear_diag((hgeneric*)dbc);

  switch(InfoType)
    {
    case SQL_ACCESSIBLE_PROCEDURES:
    case SQL_ACCESSIBLE_TABLES:
      strcpy(InfoValuePtr,"N");
      break;
        
    case SQL_ACTIVE_ENVIRONMENTS:
      *(SQLUSMALLINT*)InfoValuePtr=0;
      break;

    case SQL_AGGREGATE_FUNCTIONS:
      *(SQLUINTEGER*)InfoValuePtr=SQL_AF_ALL|
	SQL_AF_AVG|SQL_AF_COUNT|SQL_AF_DISTINCT|
	SQL_AF_MAX|SQL_AF_MIN|SQL_AF_SUM;
      break;

    case SQL_ALTER_DOMAIN: /* ALTER DOMAIN not supported*/
     *(SQLUINTEGER*)InfoValuePtr=SQL_AM_NONE;
      break;


    case SQL_ALTER_TABLE:
      *(SQLUINTEGER*)InfoValuePtr=SQL_AT_ADD_COLUMN_DEFAULT |
	SQL_AT_ADD_CONSTRAINT | SQL_AT_CONSTRAINT_NAME_DEFINITION |
	SQL_AT_DROP_COLUMN_CASCADE | SQL_AT_DROP_TABLE_CONSTRAINT_CASCADE |
	SQL_AT_SET_COLUMN_DEFAULT | SQL_AT_CONSTRAINT_INITIALLY_IMMEDIATE |
	SQL_AT_CONSTRAINT_DEFERRABLE;
      break;

    case SQL_ASYNC_MODE:
      *(SQLUINTEGER*)InfoValuePtr=SQL_AM_NONE;
      break;

    case SQL_BATCH_ROW_COUNT: /* TODO */
    case SQL_BATCH_SUPPORT:
       *(SQLUINTEGER*)InfoValuePtr=0;
       status=SQL_SUCCESS_WITH_INFO;

      ood_post_diag(dbc,ERROR_ORIGIN_IM001,0,"",
		    ERROR_MESSAGE_IM001,
		    __LINE__,0,"",ERROR_STATE_IM001,
		    __FILE__,__LINE__);

     break;

    case SQL_BOOKMARK_PERSISTENCE: /* TODO :- check this */
      *(SQLUINTEGER*)InfoValuePtr=SQL_BP_OTHER_HSTMT
	|SQL_BP_UPDATE;
      break;

    case SQL_CATALOG_LOCATION:
    case SQL_CATALOG_USAGE:
      *(SQLUINTEGER*)InfoValuePtr=0; /* No catatlogs */
      break;

    case SQL_CATALOG_NAME:
      strcpy(InfoValuePtr,"N");
      break;

    case SQL_CATALOG_NAME_SEPARATOR:
    case SQL_CATALOG_TERM:
      *(SQLCHAR*)InfoValuePtr='\0';
      break;

    case SQL_COLLATION_SEQ: /* TODO :- find out what this is */
      *(SQLCHAR*)InfoValuePtr='\0';
      break;

    case SQL_COLUMN_ALIAS:
      strcpy(InfoValuePtr,"Y");
      break;

    case SQL_CONCAT_NULL_BEHAVIOR:
      *(SQLUSMALLINT*)InfoValuePtr=SQL_CB_NON_NULL;
      break;


      /*
       * Conversions. 
       */
    case SQL_CONVERT_BIGINT:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_BIGINT|
	SQL_CVT_NUMERIC;
      break;

    case SQL_CONVERT_BINARY:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_BINARY;
      break;

    case SQL_CONVERT_BIT:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_BIT;
      break;

    case SQL_CONVERT_CHAR:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR;
      break;

    case SQL_CONVERT_DATE:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_DATE|
	SQL_CVT_TIMESTAMP|
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR;
      break;

    case SQL_CONVERT_DECIMAL: /* TODO :- don't do these yet */
      *(SQLUINTEGER*)InfoValuePtr=
	0;
      break;

    case SQL_CONVERT_DOUBLE:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_DOUBLE|
	SQL_CVT_NUMERIC|
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR;
      break;

    case SQL_CONVERT_FLOAT:
    case SQL_CONVERT_REAL:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_REAL|
	SQL_CVT_FLOAT|
	SQL_CVT_DOUBLE|
	SQL_CVT_NUMERIC|
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR;
      break;

    case SQL_CONVERT_INTEGER:
    case SQL_CONVERT_SMALLINT:
    case SQL_CONVERT_TINYINT:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR|
	SQL_CVT_INTEGER|
	SQL_CVT_BIGINT|
	SQL_CVT_SMALLINT|
	SQL_CVT_NUMERIC;
      break;

    case SQL_CONVERT_INTERVAL_YEAR_MONTH:
      *(SQLUINTEGER*)InfoValuePtr=
	0;
      break;
    case SQL_CONVERT_INTERVAL_DAY_TIME:
      *(SQLUINTEGER*)InfoValuePtr=
	0;
      break;

    case SQL_CONVERT_LONGVARBINARY:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_LONGVARBINARY;
      break;

    case SQL_CONVERT_LONGVARCHAR:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_LONGVARCHAR;
      break;

    case SQL_CONVERT_NUMERIC:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR|
	SQL_CVT_NUMERIC;
      break;


    case SQL_CONVERT_TIME:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_TIME|
	SQL_CVT_TIMESTAMP|
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR;
      break;

    case SQL_CONVERT_TIMESTAMP:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_TIMESTAMP|
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR;
      break;

      break;
    case SQL_CONVERT_VARBINARY:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_VARBINARY;
      break;

    case SQL_CONVERT_VARCHAR:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CVT_CHAR|
	SQL_CVT_VARCHAR;
      break;

    case SQL_CONVERT_FUNCTIONS: /* TODO :- find this out */
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_CORRELATION_NAME: /* TODO :- make certain of this */
      *(SQLUSMALLINT*)InfoValuePtr=SQL_CN_ANY;
      break;

    case SQL_CREATE_ASSERTION: /* TODO  :- make certain of this */
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_CREATE_CHARACTER_SET: 
      *(SQLUINTEGER*)InfoValuePtr=0; 
      break;

    case SQL_CREATE_COLLATION:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_CREATE_DOMAIN:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_CREATE_SCHEMA: /* CREATE USER != CREATE SCHEMA */
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_CREATE_TABLE:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CT_CREATE_TABLE|
	SQL_CT_TABLE_CONSTRAINT|
	SQL_CT_CONSTRAINT_NAME_DEFINITION|
	SQL_CT_COMMIT_DELETE|
	SQL_CT_GLOBAL_TEMPORARY|
	SQL_CT_COLUMN_CONSTRAINT|
	SQL_CT_COLUMN_DEFAULT|
	SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE| /* guess */
	SQL_CT_CONSTRAINT_NON_DEFERRABLE; /* guess */
      break;

    case SQL_CREATE_VIEW:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CV_CREATE_VIEW|
	SQL_CV_CHECK_OPTION|
	SQL_CV_CASCADED; /*guess*/
      break;

    case SQL_CURSOR_COMMIT_BEHAVIOR:
    case SQL_CURSOR_ROLLBACK_BEHAVIOR:
      /* Commit or rollback on one cursor has no affect on other cursors. */
      *(SQLUSMALLINT*)InfoValuePtr=SQL_CB_PRESERVE;
      break;

    case SQL_CURSOR_SENSITIVITY:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_UNSPECIFIED;
      break;

    case SQL_DATA_SOURCE_NAME: /*TODO this is not always strcitly correct */
      THREAD_MUTEX_LOCK(dbc);
      ood_bounded_strcpy(InfoValuePtr,(char*)dbc->DSN,BufferLength);
      if(StringLengthPtr)
	*StringLengthPtr=strlen(dbc->DSN);
      THREAD_MUTEX_UNLOCK(dbc);
      break;

    case SQL_DATA_SOURCE_READ_ONLY:
      strcpy(InfoValuePtr,"N");
      break;

    case SQL_DATABASE_NAME:
      THREAD_MUTEX_LOCK(dbc);
      ood_bounded_strcpy(InfoValuePtr,(char*)dbc->DB,BufferLength);
      if(StringLengthPtr)
	*StringLengthPtr=strlen(dbc->DB);
      THREAD_MUTEX_UNLOCK(dbc);
      break;

    case SQL_DATETIME_LITERALS: 
      /*
       * TODO At the moment Oracle supports a superset of the types 
       * supported by this driver 
       */
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_DL_SQL92_DATE|
	SQL_DL_SQL92_TIME|
	SQL_DL_SQL92_TIMESTAMP;
      break;

    case SQL_DBMS_VER: /* TODO :- actually implement this */
      *(SQLCHAR*)InfoValuePtr='\0';
      if(StringLengthPtr)
	*StringLengthPtr=0;
      break;

    case SQL_DBMS_NAME:
      ood_bounded_strcpy(InfoValuePtr,
			 ERROR_PRODUCT,BufferLength);
      if(StringLengthPtr)
	*StringLengthPtr=strlen(ERROR_PRODUCT);
      break;

    case SQL_DDL_INDEX:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_DI_CREATE_INDEX|
	SQL_DI_DROP_INDEX;
      break;

    case SQL_DEFAULT_TXN_ISOLATION:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_TXN_SERIALIZABLE;
      break;

    case SQL_DESCRIBE_PARAMETER: /* When we actually implement it! */
      strcpy(InfoValuePtr,"Y");
      break;

      /* SQL_DM_VER, SQL_DRIVER_HDBC, SQL_DRIVER_HENV,
       * SQL_DRIVER_HDESC, SQL_DRIVER_HLIB, SQL_DRIVER_HSTMT are DM fns */

    case SQL_DRIVER_NAME:
      /* TODO this is a hack! */
#ifdef WIN32
      strcpy(InfoValuePtr,"oracle.dll");
#else
      strcpy(InfoValuePtr,"liboraodbc.so");
#endif
      if(StringLengthPtr)
	*StringLengthPtr=strlen((const char*)InfoValuePtr);
      break;

    case SQL_DRIVER_ODBC_VER:
    case SQL_ODBC_VER:
      ood_bounded_strcpy(InfoValuePtr,"03.51",BufferLength);
      if(StringLengthPtr)
	*StringLengthPtr=strlen((const char*)InfoValuePtr);
      break;

    case SQL_DRIVER_VER:
      {
	/*
	 * This is annoying as the library version string is
	 * not easily convertable into the correct format 
	 */
	char tmp[128];
#ifdef LIB_VERSION
	sprintf(tmp,"00.00.0000 %s",LIB_VERSION);
#else
	sprintf(tmp,"00.00.0000");
#endif
	ood_bounded_strcpy(InfoValuePtr,tmp,BufferLength);
	if(StringLengthPtr)
	  *StringLengthPtr=strlen(InfoValuePtr);
      }
      break;

    case SQL_DROP_ASSERTION:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_DROP_CHARACTER_SET:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_DROP_COLLATION:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_DROP_DOMAIN:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_DROP_SCHEMA: /* drop user != drop schema */
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_DROP_TABLE:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_DT_DROP_TABLE|
	SQL_DT_CASCADE|
	SQL_DT_RESTRICT;
      break;

    case SQL_DROP_VIEW:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_DV_DROP_VIEW;
      break;

      /* ODBC Ref Vol2 Page 777 */
    case SQL_DYNAMIC_CURSOR_ATTRIBUTES1:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_DYNAMIC_CURSOR_ATTRIBUTES2:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_EXPRESSIONS_IN_ORDERBY:
      strcpy(InfoValuePtr,"Y");
      break;

    case SQL_FILE_USAGE:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_FILE_NOT_SUPPORTED;
      break;

    case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1: /*TODO*/
      *(SQLUINTEGER*)InfoValuePtr=SQL_CA1_NEXT;
      break;

    case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_CA2_READ_ONLY_CONCURRENCY|
	SQL_CA2_CRC_APPROXIMATE;
      break;

    case SQL_GETDATA_EXTENSIONS:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_GD_ANY_COLUMN|
	SQL_GD_ANY_ORDER|
	/*SQL_GD_BLOCK|*/ /*when implemented! */
	SQL_GD_BOUND;
      break;

    case SQL_GROUP_BY:
      *(SQLUSMALLINT*)InfoValuePtr=
	SQL_GB_GROUP_BY_EQUALS_SELECT;
      break;

    case SQL_IDENTIFIER_CASE: /* TODO check */
      *(SQLUSMALLINT*)InfoValuePtr=SQL_IC_UPPER;
      break;

    case SQL_IDENTIFIER_QUOTE_CHAR:
      strcpy(InfoValuePtr,"\"");
      if(StringLengthPtr)
	*StringLengthPtr=1;
      break;

    case SQL_INDEX_KEYWORDS:
      *(SQLUINTEGER*)InfoValuePtr=SQL_IK_ALL;
      break;

    case SQL_INFO_SCHEMA_VIEWS:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_INSERT_STATEMENT:
      *(SQLUINTEGER*)InfoValuePtr=
	SQL_IS_INSERT_LITERALS|
	SQL_IS_INSERT_SEARCHED;
      break;

    case SQL_INTEGRITY:
      strcpy(InfoValuePtr,"Y");
      break;

    case SQL_KEYSET_CURSOR_ATTRIBUTES1: /* TODO :- implement this! */
    case SQL_KEYSET_CURSOR_ATTRIBUTES2:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_KEYWORDS: /* TODO :- implement this! */
      *(SQLCHAR*)InfoValuePtr='\0';
      break;

    case SQL_LIKE_ESCAPE_CLAUSE:
      strcpy(InfoValuePtr,"Y");
      break;

    case SQL_MAX_ASYNC_CONCURRENT_STATEMENTS:
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_MAX_BINARY_LITERAL_LEN: /* TODO check */
      *(SQLUINTEGER*)InfoValuePtr=0;
      break;

    case SQL_MAX_CATALOG_NAME_LEN:
      *(SQLUSMALLINT*)InfoValuePtr=0;
      break;

    case SQL_MAX_CHAR_LITERAL_LEN: /* TODO guess */

      *(SQLUINTEGER*)InfoValuePtr=30;
      break;

    case SQL_MAX_COLUMN_NAME_LEN:/* TODO guess */
    case SQL_MAX_SCHEMA_NAME_LEN:
    case SQL_MAX_USER_NAME_LEN:
    case SQL_MAX_TABLE_NAME_LEN:    
    case SQL_MAX_IDENTIFIER_LEN:
    case SQL_MAX_CURSOR_NAME_LEN:
    case SQL_MAX_PROCEDURE_NAME_LEN:


      *(SQLUSMALLINT*)InfoValuePtr=30;
      break;

    case SQL_MAX_COLUMNS_IN_GROUP_BY:
    case SQL_MAX_COLUMNS_IN_ORDER_BY:
    case SQL_MAX_COLUMNS_IN_SELECT:
    case SQL_MAX_COLUMNS_IN_TABLE:
    case SQL_MAX_TABLES_IN_SELECT:
      *(SQLUSMALLINT*)InfoValuePtr=1000;
      break;

    case SQL_TXN_CAPABLE: 
      *(SQLUSMALLINT*)InfoValuePtr=SQL_TXN_CAPABLE;
      break;

    case SQL_SCHEMA_TERM:      
      strcpy(InfoValuePtr,"owner");
      break;

    case SQL_TABLE_TERM:
      strcpy(InfoValuePtr,"table");
      break;


    case SQL_SCHEMA_USAGE:
      *(SQLUINTEGER*)InfoValuePtr=SQL_SU_DML_STATEMENTS |
	SQL_SU_PROCEDURE_INVOCATION | SQL_SU_PRIVILEGE_DEFINITION;
     break;

   case SQL_UNION:
     *(SQLUSMALLINT*)InfoValuePtr=SQL_U_UNION | SQL_U_UNION_ALL;
     break;

    case SQL_MAX_ROW_SIZE:
     *(SQLUINTEGER*)InfoValuePtr=0;
     break;


    case SQL_MAX_ROW_SIZE_INCLUDES_LONG:
      strcpy(InfoValuePtr,"N");
      break;

    case SQL_NUMERIC_FUNCTIONS:
    *(SQLUSMALLINT*)InfoValuePtr=SQL_FN_NUM_ABS |
      SQL_FN_NUM_ACOS | 
      SQL_FN_NUM_ATAN |
      SQL_FN_NUM_ATAN2 |
      SQL_FN_NUM_CEILING |
      SQL_FN_NUM_COS |
      SQL_FN_NUM_EXP |
      SQL_FN_NUM_FLOOR |
      SQL_FN_NUM_LOG |
      SQL_FN_NUM_MOD |
      SQL_FN_NUM_POWER |
      SQL_FN_NUM_ROUND |
      SQL_FN_NUM_SIGN |
      SQL_FN_NUM_SIN |
      SQL_FN_NUM_SQRT |
      SQL_FN_NUM_TAN |
      SQL_FN_NUM_TRUNCATE ;
     break;

    case SQL_SUBQUERIES:
    *(SQLUINTEGER*)InfoValuePtr=SQL_SQ_CORRELATED_SUBQUERIES |
      SQL_SQ_COMPARISON | 
      SQL_SQ_EXISTS | 
      SQL_SQ_IN | 
      SQL_SQ_QUANTIFIED ;
    break;
    
    case SQL_CREATE_TRANSLATION:
    case SQL_MAX_INDEX_SIZE:
    case SQL_MAX_STATEMENT_LEN:
    case SQL_PARAM_ARRAY_ROW_COUNTS:
    case SQL_PARAM_ARRAY_SELECTS:
    case SQL_OJ_CAPABILITIES:
    case SQL_SCROLL_OPTIONS:
    case SQL_SQL_CONFORMANCE:
    case SQL_SQL92_DATETIME_FUNCTIONS:
    case SQL_SQL92_FOREIGN_KEY_DELETE_RULE:
    case SQL_SQL92_GRANT:
    case SQL_SQL92_NUMERIC_VALUE_FUNCTIONS:
    case SQL_SQL92_PREDICATES:
    case SQL_SQL92_RELATIONAL_JOIN_OPERATORS:
    case SQL_SQL92_REVOKE:
    case SQL_SQL92_ROW_VALUE_CONSTRUCTOR:
    case SQL_SQL92_STRING_FUNCTIONS:
    case SQL_SQL92_VALUE_EXPRESSIONS:
    case SQL_STANDARD_CLI_CONFORMANCE:
    case SQL_STATIC_CURSOR_ATTRIBUTES1:
    case SQL_STATIC_CURSOR_ATTRIBUTES2:
    case SQL_STRING_FUNCTIONS:
    case SQL_TIMEDATE_ADD_INTERVALS:
    case SQL_TIMEDATE_DIFF_INTERVALS:
    case SQL_TIMEDATE_FUNCTIONS:
    case SQL_TXN_ISOLATION_OPTION:

      *(SQLUINTEGER*)InfoValuePtr=0;
      status=SQL_SUCCESS_WITH_INFO;

      ood_post_diag(dbc,ERROR_ORIGIN_IM001,0,"",
		    ERROR_MESSAGE_IM001,
		    __LINE__,0,"",ERROR_STATE_IM001,
		    __FILE__,__LINE__);

      break;


      /* TODO :- the rest of these! - see the ODBC 3 Book Vol2 page 791 */
    case SQL_MAX_CONCURRENT_ACTIVITIES:
    case SQL_MAX_DRIVER_CONNECTIONS:
    case SQL_MULT_RESULT_SETS:
    case SQL_MULTIPLE_ACTIVE_TXN:
    case SQL_NEED_LONG_DATA_LEN:
    case SQL_NON_NULLABLE_COLUMNS:
    case SQL_NULL_COLLATION:
    case SQL_ODBC_INTERFACE_CONFORMANCE:
    case SQL_ODBC_API_CONFORMANCE:
    case SQL_ORDER_BY_COLUMNS_IN_SELECT:
    case SQL_PROCEDURE_TERM:
    case SQL_PROCEDURES:
    case SQL_QUOTED_IDENTIFIER_CASE:
    case SQL_ROW_UPDATES:
    case SQL_SEARCH_PATTERN_ESCAPE:
    case SQL_SERVER_NAME:
    case SQL_SPECIAL_CHARACTERS:
    case SQL_SYSTEM_FUNCTIONS:
    case SQL_USER_NAME:
    case SQL_XOPEN_CLI_YEAR:
    default:
      *(SQLUSMALLINT*)InfoValuePtr=0;
      status=SQL_SUCCESS_WITH_INFO;

      ood_post_diag(dbc,ERROR_ORIGIN_IM001,0,"",
		    ERROR_MESSAGE_IM001,
		    __LINE__,0,"",ERROR_STATE_IM001,
		    __FILE__,__LINE__);

      break;
    }

  if(ENABLE_TRACE){
    /*sql_get_info_type(InfoType)*/
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		    (SQLHANDLE)NULL,status,"i","*InfoValuePtr",
		    *(SQLUINTEGER*)InfoValuePtr);
  }
  return status;
}
