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
 *                 Dennis Box (Fermi Nat Accelerator Lab)
 *
 *******************************************************************************
 *
 * $Id: common_functions.c,v 1.8 2004/11/17 03:02:59 dbox Exp $
 *
 ******************************************************************************/

static char const rcsid[]= "$RCSfile: common_functions.c,v $ $Revision: 1.8 $";

/*
 * SQL type display size
 *
 * returns the diplay size of a column based on the SQL type and the data
 * length
 */
#include "common.h"
const char * oci_var_type(SQLSMALLINT sqltype)
{
  switch(sqltype){
  case SQLT_CHR: return "SQLT_CHR"; 
  case SQLT_NUM: return "SQLT_NUM"; 
  case SQLT_INT: return "SQLT_INT"; 
  case SQLT_FLT: return "SQLT_FLT"; 
  case SQLT_STR: return "SQLT_STR"; 
  case SQLT_VNU: return "SQLT_VNU"; 
  case SQLT_PDN: return "SQLT_PDN"; 
  case SQLT_LNG: return "SQLT_LNG"; 
  case SQLT_VCS: return "SQLT_VCS"; 
  case SQLT_NON: return "SQLT_NON"; 
  case SQLT_RID: return "SQLT_RID"; 
  case SQLT_DAT: return "SQLT_DAT"; 
  case SQLT_VBI: return "SQLT_VBI"; 
  case SQLT_BIN: return "SQLT_BIN"; 
  case SQLT_LBI: return "SQLT_LBI"; 
  case SQLT_UIN: return "SQLT_UIN"; 
  case SQLT_SLS: return "SQLT_SLS"; 
  case SQLT_LVC: return "SQLT_LVC"; 
  case SQLT_LVB: return "SQLT_LVB"; 
  case SQLT_AFC: return "SQLT_AFC"; 
  case SQLT_AVC: return "SQLT_AVC"; 
  case SQLT_CUR: return "SQLT_CUR"; 
  case SQLT_RDD: return "SQLT_RDD"; 
  case SQLT_LAB: return "SQLT_LAB"; 
  case SQLT_OSL: return "SQLT_OSL"; 
  case SQLT_NTY: return "SQLT_NTY"; 
  case SQLT_REF: return "SQLT_REF"; 
  case SQLT_CLOB: return "SQLT_CLOB"; 
  case SQLT_BLOB: return "SQLT_BLOB"; 
  case SQLT_BFILEE: return "SQLT_BFILEE"; 
  case SQLT_CFILEE: return "SQLT_CFILEE"; 
  case SQLT_RSET: return "SQLT_RSET"; 
  case SQLT_NCO: return "SQLT_NCO"; 
  case SQLT_VST: return "SQLT_VST"; 
  case SQLT_ODT: return "SQLT_ODT"; 
  default: return "UNKNOWN_OCI_TYPE";
  }
}

const char * odbc_desc_type(SQLSMALLINT sqltype)
{
	switch(sqltype){
	case SQL_DESC_COUNT: return "SQL_DESC_COUNT";
	case SQL_DESC_TYPE: return "SQL_DESC_TYPE";
	case SQL_DESC_LENGTH: return "SQL_DESC_LENGTH";
	case SQL_DESC_OCTET_LENGTH_PTR: return "SQL_DESC_OCTET_LENGTH_PTR";
	case SQL_DESC_PRECISION: return "SQL_DESC_PRECISION";
	case SQL_DESC_SCALE: return "SQL_DESC_SCALE";
	case SQL_DESC_DATETIME_INTERVAL_CODE: return "SQL_DESC_DATETIME_INTERVAL_CODE";
	case SQL_DESC_NULLABLE: return "SQL_DESC_NULLABLE";
	case SQL_DESC_INDICATOR_PTR: return "SQL_DESC_INDICATOR_PTR";
	case SQL_DESC_DATA_PTR: return "SQL_DESC_DATA_PTR";
	case SQL_DESC_NAME: return "SQL_DESC_NAME";
	case SQL_DESC_UNNAMED: return "SQL_DESC_UNNAMED";
	case SQL_DESC_OCTET_LENGTH: return "SQL_DESC_OCTET_LENGTH";
	case SQL_DESC_ALLOC_TYPE: return "SQL_DESC_ALLOC_TYPE";
	case SQL_DESC_ALLOC_AUTO: return "SQL_DESC_ALLOC_AUTO";
	case SQL_DESCRIBE_PARAMETER: return "SQL_DESCRIBE_PARAMETER";
	case SQL_DESC_ARRAY_SIZE: return "SQL_DESC_ARRAY_SIZE";
	case SQL_DESC_ARRAY_STATUS_PTR: return "SQL_DESC_ARRAY_STATUS_PTR";
	case SQL_DESC_AUTO_UNIQUE_VALUE: return "SQL_DESC_AUTO_UNIQUE_VALUE";
	case SQL_DESC_BASE_COLUMN_NAME: return "SQL_DESC_BASE_COLUMN_NAME";
	case SQL_DESC_BASE_TABLE_NAME: return "SQL_DESC_BASE_TABLE_NAME";
	case SQL_DESC_BIND_OFFSET_PTR: return "SQL_DESC_BIND_OFFSET_PTR";
	case SQL_DESC_BIND_TYPE: return "SQL_DESC_BIND_TYPE";
	case SQL_DESC_CASE_SENSITIVE: return "SQL_DESC_CASE_SENSITIVE";
	case SQL_DESC_CATALOG_NAME: return "SQL_DESC_CATALOG_NAME";
	case SQL_DESC_CONCISE_TYPE: return "SQL_DESC_CONCISE_TYPE";
	case SQL_DESC_DATETIME_INTERVAL_PRECISION: return "SQL_DESC_DATETIME_INTERVAL_PRECISION";
	case SQL_DESC_DISPLAY_SIZE: return "SQL_DESC_DISPLAY_SIZE";
	case SQL_DESC_FIXED_PREC_SCALE: return "SQL_DESC_FIXED_PREC_SCALE";
	case SQL_DESC_LABEL: return "SQL_DESC_LABEL";
	case SQL_DESC_LITERAL_PREFIX: return "SQL_DESC_LITERAL_PREFIX";
	case SQL_DESC_LITERAL_SUFFIX: return "SQL_DESC_LITERAL_SUFFIX";
	case SQL_DESC_LOCAL_TYPE_NAME: return "SQL_DESC_LOCAL_TYPE_NAME";
	case SQL_DESC_MAXIMUM_SCALE: return "SQL_DESC_MAXIMUM_SCALE";
	case SQL_DESC_MINIMUM_SCALE: return "SQL_DESC_MINIMUM_SCALE";
	case SQL_DESC_NUM_PREC_RADIX: return "SQL_DESC_NUM_PREC_RADIX";
	case SQL_DESC_PARAMETER_TYPE: return "SQL_DESC_PARAMETER_TYPE";
	case SQL_DESC_ROWS_PROCESSED_PTR: return "SQL_DESC_ROWS_PROCESSED_PTR";
	case SQL_DESC_ROWVER: return "SQL_DESC_ROWVER";
	case SQL_DESC_SCHEMA_NAME: return "SQL_DESC_SCHEMA_NAME";
	case SQL_DESC_SEARCHABLE: return "SQL_DESC_SEARCHABLE";
	case SQL_DESC_TYPE_NAME: return "SQL_DESC_TYPE_NAME";
	case SQL_DESC_TABLE_NAME: return "SQL_DESC_TABLE_NAME";
	case SQL_DESC_UNSIGNED: return "SQL_DESC_UNSIGNED";
	case SQL_DESC_UPDATABLE: return "SQL_DESC_UPDATABLE";
	default: return "UNKNOWN";
	}
}

const char * odbc_var_type(SQLSMALLINT sqltype)
{
  switch(sqltype){
  case SQL_UNKNOWN_TYPE: return "SQL_UNKNOWN_TYPE";        
  case SQL_CHAR    : return "SQL_CHAR";         
  case SQL_NUMERIC  : return "SQL_NUMERIC";        
  case SQL_DECIMAL  : return "SQL_DECIMAL";        
  case SQL_INTEGER   : return "SQL_INTEGER";       
  case SQL_SMALLINT  : return "SQL_SMALLINT";       
  case SQL_FLOAT    : return "SQL_FLOAT";        
  case SQL_REAL     : return "SQL_REAL";        
  case SQL_DOUBLE    : return "SQL_DOUBLE";       
  case SQL_DATETIME   : return "SQL_DATETIME";      
  case SQL_VARCHAR     : return "SQL_VARCHAR";
  default: return "OUT_OF_RANGE_NO_IDEA"; 
  }
}

const char * sql_get_info_type(SQLSMALLINT sqltype)
{
  switch(sqltype){
  case SQL_ACCESSIBLE_TABLES: 
	return "SQL_ACCESSIBLE_TABLES";
  case SQL_ACCESSIBLE_PROCEDURES: 
	return "SQL_ACCESSIBLE_PROCEDURES";
  case SQL_ACTIVE_ENVIRONMENTS: 
	return "SQL_ACTIVE_ENVIRONMENTS";
  case SQL_AGGREGATE_FUNCTIONS: 
	return "SQL_AGGREGATE_FUNCTIONS";
  case SQL_ALTER_DOMAIN: 
	return "SQL_ALTER_DOMAIN";
  case SQL_ALTER_TABLE: 
	return "SQL_ALTER_TABLE";
  case SQL_ASYNC_MODE: 
	return "SQL_ASYNC_MODE";
  case SQL_BATCH_ROW_COUNT: 
	return "SQL_BATCH_ROW_COUNT";
  case SQL_BATCH_SUPPORT: 
	return "SQL_BATCH_SUPPORT";
  case SQL_BOOKMARK_PERSISTENCE: 
	return "SQL_BOOKMARK_PERSISTENCE";
  case SQL_CATALOG_LOCATION: 
	return "SQL_CATALOG_LOCATION";
  case SQL_CATALOG_NAME: 
	return "SQL_CATALOG_NAME";
  case SQL_CATALOG_NAME_SEPARATOR: 
	return "SQL_CATALOG_NAME_SEPARATOR";
  case SQL_CATALOG_TERM: 
	return "SQL_CATALOG_TERM";
  case SQL_CATALOG_USAGE: 
	return "SQL_CATALOG_USAGE";
  case SQL_COLLATION_SEQ: 
	return "SQL_COLLATION_SEQ";
  case SQL_COLUMN_ALIAS: 
	return "SQL_COLUMN_ALIAS";
  case SQL_CONCAT_NULL_BEHAVIOR: 
	return "SQL_CONCAT_NULL_BEHAVIOR";
  case SQL_CONVERT_BIGINT: 
	return "SQL_CONVERT_BIGINT";
  case SQL_CONVERT_BINARY: 
	return "SQL_CONVERT_BINARY";
  case SQL_CONVERT_BIT: 
	return "SQL_CONVERT_BIT";
  case SQL_CONVERT_CHAR: 
	return "SQL_CONVERT_CHAR";
  case SQL_CONVERT_DATE: 
	return "SQL_CONVERT_DATE";
  case SQL_CONVERT_DECIMAL: 
	return "SQL_CONVERT_DECIMAL";
  case SQL_CONVERT_DOUBLE: 
	return "SQL_CONVERT_DOUBLE";
  case SQL_CONVERT_FLOAT: 
	return "SQL_CONVERT_FLOAT";
  case SQL_CONVERT_FUNCTIONS: 
	return "SQL_CONVERT_FUNCTIONS";
  case SQL_CONVERT_INTEGER: 
	return "SQL_CONVERT_INTEGER";
  case SQL_CONVERT_INTERVAL_DAY_TIME: 
	return "SQL_CONVERT_INTERVAL_DAY_TIME";
  case SQL_CONVERT_INTERVAL_YEAR_MONTH: 
	return "SQL_CONVERT_INTERVAL_YEAR_MONTH";
  case SQL_CONVERT_LONGVARBINARY: 
	return "SQL_CONVERT_LONGVARBINARY";
  case SQL_CONVERT_LONGVARCHAR: 
	return "SQL_CONVERT_LONGVARCHAR";
  case SQL_CONVERT_NUMERIC: 
	return "SQL_CONVERT_NUMERIC";
  case SQL_CONVERT_REAL: 
	return "SQL_CONVERT_REAL";
  case SQL_CONVERT_SMALLINT: 
	return "SQL_CONVERT_SMALLINT";
  case SQL_CONVERT_TIME: 
	return "SQL_CONVERT_TIME";
  case SQL_CONVERT_TIMESTAMP: 
	return "SQL_CONVERT_TIMESTAMP";
  case SQL_CONVERT_TINYINT: 
	return "SQL_CONVERT_TINYINT";
  case SQL_CONVERT_VARBINARY: 
	return "SQL_CONVERT_VARBINARY";
  case SQL_CONVERT_VARCHAR: 
	return "SQL_CONVERT_VARCHAR";
  case SQL_CONVERT_WCHAR: 
	return "SQL_CONVERT_WCHAR";
  case SQL_CONVERT_WLONGVARCHAR: 
	return "SQL_CONVERT_WLONGVARCHAR";
  case SQL_CONVERT_WVARCHAR: 
	return "SQL_CONVERT_WVARCHAR";
  case SQL_CORRELATION_NAME: 
	return "SQL_CORRELATION_NAME";
  case SQL_CREATE_ASSERTION: 
	return "SQL_CREATE_ASSERTION";
  case SQL_CREATE_CHARACTER_SET: 
	return "SQL_CREATE_CHARACTER_SET";
  case SQL_CREATE_COLLATION: 
	return "SQL_CREATE_COLLATION";
  case SQL_CREATE_DOMAIN: 
	return "SQL_CREATE_DOMAIN";
  case SQL_CREATE_SCHEMA: 
	return "SQL_CREATE_SCHEMA";
  case SQL_CREATE_TABLE: 
	return "SQL_CREATE_TABLE";
  case SQL_CREATE_TRANSLATION: 
	return "SQL_CREATE_TRANSLATION";
  case SQL_CREATE_VIEW: 
	return "SQL_CREATE_VIEW";
  case SQL_CURSOR_COMMIT_BEHAVIOR: 
	return "SQL_CURSOR_COMMIT_BEHAVIOR";
  case SQL_CURSOR_ROLLBACK_BEHAVIOR: 
	return "SQL_CURSOR_ROLLBACK_BEHAVIOR";
  case SQL_CURSOR_SENSITIVITY: 
	return "SQL_CURSOR_SENSITIVITY";
  case SQL_DATA_SOURCE_NAME: 
	return "SQL_DATA_SOURCE_NAME";
  case SQL_DATA_SOURCE_READ_ONLY: 
	return "SQL_DATA_SOURCE_READ_ONLY";
  case SQL_DATABASE_NAME: 
	return "SQL_DATABASE_NAME";
  case SQL_DATETIME_LITERALS: 
	return "SQL_DATETIME_LITERALS";
  case SQL_DBMS_NAME: 
	return "SQL_DBMS_NAME";
  case SQL_DBMS_VER: 
	return "SQL_DBMS_VER";
  case SQL_DDL_INDEX: 
	return "SQL_DDL_INDEX";
  case SQL_DESCRIBE_PARAMETER: 
	return "SQL_DESCRIBE_PARAMETER";
  case SQL_DM_VER: 
	return "SQL_DM_VER";
  case SQL_DRIVER_HDBC: 
	return "SQL_DRIVER_HDBC";
  case SQL_DRIVER_HDESC: 
	return "SQL_DRIVER_HDESC";
  case SQL_DRIVER_HENV: 
	return "SQL_DRIVER_HENV";
  case SQL_DRIVER_HLIB: 
	return "SQL_DRIVER_HLIB";
  case SQL_DRIVER_HSTMT: 
	return "SQL_DRIVER_HSTMT";
  case SQL_DRIVER_NAME: 
	return "SQL_DRIVER_NAME";
  case SQL_DRIVER_ODBC_VER: 
	return "SQL_DRIVER_ODBC_VER";
  case SQL_DRIVER_VER: 
	return "SQL_DRIVER_VER";
  case SQL_DROP_ASSERTION: 
	return "SQL_DROP_ASSERTION";
  case SQL_DROP_CHARACTER_SET: 
	return "SQL_DROP_CHARACTER_SET";
  case SQL_DROP_COLLATION: 
	return "SQL_DROP_COLLATION";
  case SQL_DROP_DOMAIN: 
	return "SQL_DROP_DOMAIN";
  case SQL_DROP_SCHEMA: 
	return "SQL_DROP_SCHEMA";
  case SQL_DROP_TABLE: 
	return "SQL_DROP_TABLE";
  case SQL_DROP_TRANSLATION: 
	return "SQL_DROP_TRANSLATION";
  case SQL_DROP_VIEW: 
	return "SQL_DROP_VIEW";
  case SQL_DYNAMIC_CURSOR_ATTRIBUTES1: 
	return "SQL_DYNAMIC_CURSOR_ATTRIBUTES1";
  case SQL_DYNAMIC_CURSOR_ATTRIBUTES2: 
	return "SQL_DYNAMIC_CURSOR_ATTRIBUTES2";
  case SQL_EXPRESSIONS_IN_ORDERBY: 
	return "SQL_EXPRESSIONS_IN_ORDERBY";
case SQL_FILE_USAGE: 
	return "SQL_FILE_USAGE";
  case SQL_FETCH_DIRECTION: 
	return "SQL_FETCH_DIRECTION";
  case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1: 
	return "SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1";
  case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2: 
	return "SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2";
  case SQL_GETDATA_EXTENSIONS: 
	return "SQL_GETDATA_EXTENSIONS";
  case SQL_GROUP_BY: 
	return "SQL_GROUP_BY";
  case SQL_IDENTIFIER_CASE: 
	return "SQL_IDENTIFIER_CASE";
  case SQL_IDENTIFIER_QUOTE_CHAR: 
	return "SQL_IDENTIFIER_QUOTE_CHAR";
  case SQL_INDEX_KEYWORDS: 
	return "SQL_INDEX_KEYWORDS";
  case SQL_INFO_SCHEMA_VIEWS: 
	return "SQL_INFO_SCHEMA_VIEWS";
  case SQL_INSERT_STATEMENT: 
	return "SQL_INSERT_STATEMENT";
  case SQL_INTEGRITY: 
	return "SQL_INTEGRITY";
  case SQL_KEYSET_CURSOR_ATTRIBUTES1: 
	return "SQL_KEYSET_CURSOR_ATTRIBUTES1";
  case SQL_KEYSET_CURSOR_ATTRIBUTES2: 
	return "SQL_KEYSET_CURSOR_ATTRIBUTES2";
  case SQL_KEYWORDS: 
	return "SQL_KEYWORDS";
  case SQL_LIKE_ESCAPE_CLAUSE: 
	return "SQL_LIKE_ESCAPE_CLAUSE";
  case SQL_LOCK_TYPES: 
	return "SQL_LOCK_TYPES";
  case SQL_MAX_ASYNC_CONCURRENT_STATEMENTS: 
	return "SQL_MAX_ASYNC_CONCURRENT_STATEMENTS";
  case SQL_MAX_BINARY_LITERAL_LEN: 
	return "SQL_MAX_BINARY_LITERAL_LEN";
  case SQL_MAX_CATALOG_NAME_LEN: 
	return "SQL_MAX_CATALOG_NAME_LEN";
  case SQL_MAX_CHAR_LITERAL_LEN: 
	return "SQL_MAX_CHAR_LITERAL_LEN";
  case SQL_MAX_COLUMN_NAME_LEN: 
	return "SQL_MAX_COLUMN_NAME_LEN";
  case SQL_MAX_COLUMNS_IN_GROUP_BY: 
	return "SQL_MAX_COLUMNS_IN_GROUP_BY";
  case SQL_MAX_COLUMNS_IN_INDEX: 
	return "SQL_MAX_COLUMNS_IN_INDEX";
  case SQL_MAX_COLUMNS_IN_ORDER_BY: 
	return "SQL_MAX_COLUMNS_IN_ORDER_BY";
  case SQL_MAX_COLUMNS_IN_SELECT: 
	return "SQL_MAX_COLUMNS_IN_SELECT";
  case SQL_MAX_COLUMNS_IN_TABLE: 
	return "SQL_MAX_COLUMNS_IN_TABLE";
  case SQL_MAX_CONCURRENT_ACTIVITIES: 
	return "SQL_MAX_CONCURRENT_ACTIVITIES";
  case SQL_MAX_CURSOR_NAME_LEN: 
	return "SQL_MAX_CURSOR_NAME_LEN";
  case SQL_MAX_DRIVER_CONNECTIONS: 
	return "SQL_MAX_DRIVER_CONNECTIONS";
  case SQL_MAX_IDENTIFIER_LEN: 
	return "SQL_MAX_IDENTIFIER_LEN";
  case SQL_MAX_INDEX_SIZE: 
	return "SQL_MAX_INDEX_SIZE";
    /*case SQL_MAX_LENGTH: 
	return "SQL_MAX_LENGTH";*/
  case SQL_MAX_PROCEDURE_NAME_LEN: 
	return "SQL_MAX_PROCEDURE_NAME_LEN";
  case SQL_MAX_ROW_SIZE: 
	return "SQL_MAX_ROW_SIZE";
  case SQL_MAX_ROW_SIZE_INCLUDES_LONG: 
	return "SQL_MAX_ROW_SIZE_INCLUDES_LONG";
  case SQL_MAX_SCHEMA_NAME_LEN: return "SQL_MAX_SCHEMA_NAME_LEN";
  case SQL_MAX_STATEMENT_LEN: return "SQL_MAX_STATEMENT_LEN";
  case SQL_MAX_TABLE_NAME_LEN: return "SQL_MAX_TABLE_NAME_LEN";
  case SQL_MAX_TABLES_IN_SELECT: return "SQL_MAX_TABLES_IN_SELECT";
  case SQL_MAX_USER_NAME_LEN: return "SQL_MAX_USER_NAME_LEN";
  case SQL_MULT_RESULT_SETS: return "SQL_MULT_RESULT_SETS";
  case SQL_MULTIPLE_ACTIVE_TXN: return "SQL_MULTIPLE_ACTIVE_TXN";
  case SQL_NEED_LONG_DATA_LEN: return "SQL_NEED_LONG_DATA_LEN";
  case SQL_NON_NULLABLE_COLUMNS: return "SQL_NON_NULLABLE_COLUMNS";
  case SQL_NULL_COLLATION: return "SQL_NULL_COLLATION";
  case SQL_NUMERIC_FUNCTIONS: return "SQL_NUMERIC_FUNCTIONS";
  case SQL_ODBC_API_CONFORMANCE: return "SQL_ODBC_API_CONFORMANCE";
  case SQL_ODBC_INTERFACE_CONFORMANCE: 
	return "SQL_ODBC_INTERFACE_CONFORMANCE";
  case SQL_ODBC_SQL_CONFORMANCE: return "SQL_ODBC_SQL_CONFORMANCE";
  case SQL_OJ_CAPABILITIES: return "SQL_OJ_CAPABILITIES";
  case SQL_ORDER_BY_COLUMNS_IN_SELECT: 
	return "SQL_ORDER_BY_COLUMNS_IN_SELECT";
  case SQL_PARAM_ARRAY_ROW_COUNTS: 
	return "SQL_PARAM_ARRAY_ROW_COUNTS";
  case SQL_PARAM_ARRAY_SELECTS: 
	return "SQL_PARAM_ARRAY_SELECTS";
  case SQL_POS_OPERATIONS: return "SQL_POS_OPERATIONS";
  case SQL_POSITIONED_STATEMENTS: return "SQL_POSITIONED_STATEMENTS";
  case SQL_PROCEDURES: return "SQL_PROCEDURES";
  case SQL_PROCEDURE_TERM: return "SQL_PROCEDURE_TERM";
  case SQL_QUOTED_IDENTIFIER_CASE: return "SQL_QUOTED_IDENTIFIER_CASE";
  case SQL_ROW_UPDATES: return "SQL_ROW_UPDATES";
  case SQL_SCHEMA_TERM: return "SQL_SCHEMA_TERM";
  case SQL_SCHEMA_USAGE: return "SQL_SCHEMA_USAGE";
  case SQL_SCROLL_CONCURRENCY: return "SQL_SCROLL_CONCURRENCY";
  case SQL_SCROLL_OPTIONS: return "SQL_SCROLL_OPTIONS";
  case SQL_SEARCH_PATTERN_ESCAPE: return "SQL_SEARCH_PATTERN_ESCAPE";
  case SQL_SERVER_NAME: return "SQL_SERVER_NAME";
  case SQL_SPECIAL_CHARACTERS: return "SQL_SPECIAL_CHARACTERS";
  case SQL_SQL_CONFORMANCE: return "SQL_SQL_CONFORMANCE";
  case SQL_SQL92_DATETIME_FUNCTIONS: return "SQL_SQL92_DATETIME_FUNCTIONS";
  case SQL_SQL92_FOREIGN_KEY_DELETE_RULE: 
	return "SQL_SQL92_FOREIGN_KEY_DELETE_RULE";
  case SQL_SQL92_FOREIGN_KEY_UPDATE_RULE: 
	return "SQL_SQL92_FOREIGN_KEY_UPDATE_RULE";
  case SQL_SQL92_GRANT: return "SQL_SQL92_GRANT";
  case SQL_SQL92_NUMERIC_VALUE_FUNCTIONS: 
	return "SQL_SQL92_NUMERIC_VALUE_FUNCTIONS";
  case SQL_SQL92_PREDICATES: return "SQL_SQL92_PREDICATES";
  case SQL_SQL92_RELATIONAL_JOIN_OPERATORS: 
	return "SQL_SQL92_RELATIONAL_JOIN_OPERATORS";
  case SQL_SQL92_REVOKE: return "SQL_SQL92_REVOKE";
  case SQL_SQL92_ROW_VALUE_CONSTRUCTOR: 
	return "SQL_SQL92_ROW_VALUE_CONSTRUCTOR";
  case SQL_SQL92_STRING_FUNCTIONS: return "SQL_SQL92_STRING_FUNCTIONS";
  case SQL_SQL92_VALUE_EXPRESSIONS: return "SQL_SQL92_VALUE_EXPRESSIONS";
  case SQL_STANDARD_CLI_CONFORMANCE: return "SQL_STANDARD_CLI_CONFORMANCE";
  case SQL_STATIC_CURSOR_ATTRIBUTES1: return "SQL_STATIC_CURSOR_ATTRIBUTES1";
  case SQL_STATIC_CURSOR_ATTRIBUTES2: return "SQL_STATIC_CURSOR_ATTRIBUTES2";
  case SQL_STATIC_SENSITIVITY: return "SQL_STATIC_SENSITIVITY";
  case SQL_STRING_FUNCTIONS: return "SQL_STRING_FUNCTIONS";
  case SQL_SUBQUERIES: return "SQL_SUBQUERIES";
  case SQL_SYSTEM_FUNCTIONS: return "SQL_SYSTEM_FUNCTIONS";
  case SQL_TABLE_TERM: return "SQL_TABLE_TERM";
  case SQL_TIMEDATE_ADD_INTERVALS: return "SQL_TIMEDATE_ADD_INTERVALS";
  case SQL_TIMEDATE_DIFF_INTERVALS: return "SQL_TIMEDATE_DIFF_INTERVALS";
  case SQL_TIMEDATE_FUNCTIONS: return "SQL_TIMEDATE_FUNCTIONS";
  case SQL_TXN_CAPABLE: return "SQL_TXN_CAPABLE";
  case SQL_TXN_ISOLATION_OPTION: return "SQL_TXN_ISOLATION_OPTION";
  case SQL_UNION: return "SQL_UNION";
  case SQL_USER_NAME: return "SQL_USER_NAME";
  case SQL_XOPEN_CLI_YEAR: return "SQL_XOPEN_CLI_YEAR";
  default:
    return "NO_IDEA";
  } 
}




const char * odbc_sql_attr_type( SQLINTEGER  attr)
{
  switch(attr){
      case SQL_ATTR_METADATA_ID: 
	return "SQL_ATTR_METADATA_ID";
      case SQL_ATTR_APP_ROW_DESC: 
	return "SQL_ATTR_APP_ROW_DESC";
      case SQL_ATTR_APP_PARAM_DESC: 
	return "SQL_ATTR_APP_PARAM_DESC";
      case SQL_ATTR_IMP_PARAM_DESC: 
	return "SQL_ATTR_IMP_PARAM_DESC";
      case SQL_ATTR_IMP_ROW_DESC: 
	return "SQL_ATTR_IMP_ROW_DESC";
      case SQL_ATTR_QUERY_TIMEOUT: 
	return "SQL_ATTR_QUERY_TIMEOUT";
      case SQL_ATTR_PARAM_BIND_OFFSET_PTR: 
	return "SQL_ATTR_PARAM_BIND_OFFSET_PTR";
      case SQL_ATTR_ROW_BIND_OFFSET_PTR: 
	return "SQL_ATTR_ROW_BIND_OFFSET_PTR";
      case SQL_ATTR_ROW_ARRAY_SIZE: 
	return "SQL_ATTR_ROW_ARRAY_SIZE";
      case SQL_ATTR_ROW_BIND_TYPE: 
	return "SQL_ATTR_ROW_BIND_TYPE";
      case SQL_ATTR_ROW_STATUS_PTR: 
	return "SQL_ATTR_ROW_STATUS_PTR";
      case SQL_ATTR_ROWS_FETCHED_PTR: 
	return "SQL_ATTR_ROWS_FETCHED_PTR";
      case SQL_ATTR_PARAM_BIND_TYPE: 
	return "SQL_ATTR_PARAM_BIND_TYPE";
      case SQL_ATTR_PARAM_OPERATION_PTR: 
	return "SQL_ATTR_PARAM_OPERATION_PTR";
      case SQL_ATTR_PARAMS_PROCESSED_PTR: 
	return "SQL_ATTR_PARAMS_PROCESSED_PTR";
      case SQL_ATTR_PARAMSET_SIZE: 
	return "SQL_ATTR_PARAMSET_SIZE";
      case SQL_ATTR_ROW_OPERATION_PTR: 
	return "SQL_ATTR_ROW_OPERATION_PTR";
      case SQL_ATTR_PARAM_STATUS_PTR: 
	return "SQL_ATTR_PARAM_STATUS_PTR";
      case SQL_ATTR_ASYNC_ENABLE: 
	return "SQL_ATTR_ASYNC_ENABLE";
      case SQL_ATTR_CONCURRENCY: 
	return "SQL_ATTR_CONCURRENCY";
      case SQL_ATTR_CURSOR_SCROLLABLE: 
	return "SQL_ATTR_CURSOR_SCROLLABLE";
      case SQL_ATTR_CURSOR_SENSITIVITY: 
	return "SQL_ATTR_CURSOR_SENSITIVITY";
      case SQL_ATTR_CURSOR_TYPE: 
	return "SQL_ATTR_CURSOR_TYPE";
      case SQL_ATTR_ENABLE_AUTO_IPD: 
	return "SQL_ATTR_ENABLE_AUTO_IPD";
      case SQL_ATTR_FETCH_BOOKMARK_PTR: 
	return "SQL_ATTR_FETCH_BOOKMARK_PTR";
      case SQL_ATTR_KEYSET_SIZE: 
	return "SQL_ATTR_KEYSET_SIZE";
      case SQL_ATTR_MAX_LENGTH: 
	return "SQL_ATTR_MAX_LENGTH";
      case SQL_ATTR_MAX_ROWS: 
	return "SQL_ATTR_MAX_ROWS";
      case SQL_ATTR_NOSCAN: 
	return "SQL_ATTR_NOSCAN";
      case SQL_ATTR_SIMULATE_CURSOR: 
	return "SQL_ATTR_SIMULATE_CURSOR";
      case SQL_ATTR_USE_BOOKMARKS: 
	return "SQL_ATTR_USE_BOOKMARKS";
      default:
	return "NO IDEA";
   }
}




SQLINTEGER sqltype_display_size(SQLSMALLINT sqltype,int length)
{
    switch(sqltype)
    {
        case SQL_C_CHAR:
            return length-1;

        case SQL_BIT:
            return 1;

        case SQL_TINYINT:
            return 4;

        case SQL_SMALLINT:
        case SQL_C_SSHORT:
            return 6;
        case SQL_C_USHORT:
            return 5;

        case SQL_INTEGER:
        case SQL_C_SLONG:
            return 11;
        case SQL_C_ULONG:
            return 10;

        case SQL_BIGINT:
            return 20;

        case SQL_REAL:
            return 13;

        case SQL_FLOAT:
        case SQL_DOUBLE:
            return 23;

        case SQL_TYPE_DATE:
        case SQL_C_DATE:
            return 10;

        case SQL_TYPE_TIME:
        case SQL_C_TIME:
            return 8;

        case SQL_TYPE_TIMESTAMP:
        case SQL_C_TIMESTAMP:
            return 21;

        default :
            return length;
    }
}

/*
 * Pthread lock statement
 *
 * Locks the statement handle and the four current descriptors
 */
void ood_mutex_lock_stmt(hStmt_T *stmt)
{
    THREAD_MUTEX_LOCK(stmt);
    THREAD_MUTEX_LOCK(stmt->current_ap);
    THREAD_MUTEX_LOCK(stmt->current_ip);
    THREAD_MUTEX_LOCK(stmt->current_ar);
    THREAD_MUTEX_LOCK(stmt->current_ir);
}

/* thread unlock statement
 *
 * unlocks the statement and the four current descriptors
 */
void ood_mutex_unlock_stmt(hStmt_T *stmt)
{
    THREAD_MUTEX_UNLOCK(stmt->current_ap);
    THREAD_MUTEX_UNLOCK(stmt->current_ip);
    THREAD_MUTEX_UNLOCK(stmt->current_ar);
    THREAD_MUTEX_UNLOCK(stmt->current_ir);
    THREAD_MUTEX_UNLOCK(stmt);
}

/* thread mutex init
 *
 * Initialise thread
 */

int ood_mutex_init(hgeneric* handle)
{
#if defined(HAVE_LIBPTHREAD)
    return pthread_mutex_init(HANDLE_MUTEX_PTR(handle),NULL);
#elif defined(WIN32)
    return (int) (HANDLE_MUTEX(handle)=(int*)CreateMutex(NULL,FALSE,NULL));
#else
    return 1;
#endif
}

int ood_mutex_destroy(hgeneric* handle)
{
#if defined(HAVE_LIBPTHREAD)
    return pthread_mutex_destroy(HANDLE_MUTEX_PTR(handle));
#elif defined(WIN32)
    return ReleaseMutex(HANDLE_MUTEX_PTR(handle));
#else
    return 1;
#endif
}

/*
 * Altertate fetch :- fetch no data
 */
sword ood_alt_fetch_no_data(hStmt_T* stmt)
{
	return OCI_NO_DATA;
}

/*
 * _log_message
 *
 * The nitty gritty of log_message
 */
static void _log_message_imp(char *tracefilename,char *file,int line, 
        int mask, SQLHANDLE handle, SQLRETURN ret, char* fmt, va_list ap)
{
    FILE *tracefile=stderr;
    char *left; /*left hand side of each format pair */
    /*
    if(NULL==(tracefile=fopen(tracefilename,"a")))
    {
        return;
    }
    */
#if !defined(WIN32)
    fprintf(tracefile, "%s[%d]%s[%04d]\n",ERROR_PRODUCT,
            (int)getpid(),file,line);
#else
    fprintf(tracefile, "%s-%s[%04d]\n",ERROR_PRODUCT,
            file,line);
#endif

    if(mask==TRACE_FUNCTION_ENTRY)
    {
        fputs("                Entry:",tracefile);
        switch(HANDLE_TYPE(handle))
        {
            case SQL_HANDLE_STMT:
            fputs("[SQL_HANDLE_STMT]",tracefile);
            break;

            case SQL_HANDLE_DBC:
            fputs("[SQL_HANDLE_DBC]",tracefile);
            break;

            case SQL_HANDLE_DESC:
            fputs("[SQL_HANDLE_DESC]",tracefile);
            break;

            case SQL_HANDLE_ENV:
            fputs("[SQL_HANDLE_ENV]",tracefile);
            break;
            
            default:
            fprintf(tracefile,"[UNKOWN=%d]",HANDLE_TYPE(handle));
            break;
        }
        fprintf(tracefile,"[0x%.8lx]\n",(long)handle);
    }
    else if(mask==TRACE_FUNCTION_EXIT)
    {
        fprintf(tracefile,"                Exit: ");
        switch(ret)
        {
            case SQL_SUCCESS:
            fputs("[SQL_SUCCESS]\n",tracefile);
            break;

            case SQL_NO_DATA:
            fputs("[SQL_NO_DATA]\n",tracefile);
            break;

            case SQL_ERROR:
            fputs( "[SQL_ERROR]\n",tracefile);
            break;

            case SQL_INVALID_HANDLE:
            fputs("[SQL_INVALID_HANDLE]\n",tracefile);
            break;

            case SQL_STILL_EXECUTING:
            fputs("[SQL_STILL_EXECUTING]\n",tracefile);
            break;

            case SQL_NEED_DATA:
            fputs("[SQL_NEED_DATA]\n",tracefile);
            break;

            case SQL_SUCCESS_WITH_INFO:
            fputs("[SQL_SUCESS_WITH_INFO]\n",tracefile);
            break;

            default:
            fprintf(tracefile, "[UNKNOWN=%d]", ret );
            break;
        }
    }
    else
    {
        fputs("                Unknown:\n",tracefile);
    }

    if ( !fmt )
    {
        fputs("<NULL FORMAT>\n",tracefile);
        return;
    }

    while ( *fmt )
    {
        left=va_arg(ap,char *);
        if(left)
            fprintf(tracefile,"                %s = ",left);
        else
            fprintf(tracefile,"                     ");
        switch ( *fmt )
        {
              case 'h':            /* a handle */
              case 'p':            /* a pointer */
              {
                void *h = va_arg( ap, void * );
                fprintf(tracefile, "%p\n", h );
                break;
              }
    
              case 's':            /* C string */
              {
                char * ptr = va_arg( ap, char * );

                if ( ptr )
                {
                    fprintf(tracefile, "\"%s\"\n", ptr );
                }
                else
                {
                    fprintf(tracefile, "<NULLPTR>\n" );
                }
                break;
              }

              case 'I':            /* a short */
              {
              short i = (short) va_arg( ap, int );

                fprintf(tracefile, "%d\n", i );
                break;
              }

              case 'U':            /* a unsigned short */
              {
              unsigned short i = (unsigned short) va_arg( ap, int );

                fprintf(tracefile, "%u\n", i );
                break;
              }

              case 'i':            /* a int */
              {
                int i = va_arg( ap, int );

                fprintf(tracefile, "%d\n", i );
                break;
              }

              case 'u':            /* a unsigned int */
              {
                unsigned int i = va_arg( ap, unsigned int );

                fprintf(tracefile, "%u\n", i );
                break;
              }
        }/*switch(*fmt)*/

        if(*fmt)
            fmt++;
    }/*while(*fmt)*/
    fputc('\n',tracefile);
    /*fclose(tracefile);*/
}

/*
 * log_message
 *
 * Make a trace entry. This fn is deliberately small to try to keep performance
 * up when tracing is off.
 *
 * Operation is a little odd. There is a format string much like printf,
 * but each format specifier represents a pair of arguments, ie
 * ood_log_message(..."sh","SQL",stmt->sqltext,"Statement",stmt);
 * would produce
 *            SQL = SELECT * FROM ....
 *            Statement = 0x43565432
 *
 *  Format specifiers are...
 *
 * h            Print a handle.
 * p            Print a pointer.
 * i            Print a small integer.
 * u            Print a unsigned small integer.
 * I            Print a integer.
 * U            Print a unsigned integer.
 * s            A null terminated string.
 */
void ood_log_message(hDbc_T* dbc,char *file,int line, int mask, SQLHANDLE handle,
        SQLRETURN ret, char *fmt, ... )
{
    va_list ap;
	/*
	 * This is a common place for deadlocks...
	 */
#ifdef UNIX_DEBUG
	int i=0;
	while(EBUSY==pthread_mutex_trylock( &((hgeneric*)dbc)->mutex ))
	{
		fprintf(stderr,"ood_log_message -> dbc [0x%.8lx] on pid %d locked\n",
				(long)dbc,getpid());
		sleep(1);
		if(i++>10)
			abort();
	}
#else
    THREAD_MUTEX_LOCK(dbc);
#endif
    /*
    if(dbc->trace!=SQL_OPT_TRACE_ON)
    {
        THREAD_MUTEX_UNLOCK(dbc);
        return;
    }
    */
    va_start( ap, fmt );
    _log_message_imp((char*)dbc->tracefile,file,line,mask,handle,ret,fmt,ap);
    va_end( ap );
    THREAD_MUTEX_UNLOCK(dbc);
    return;
}

