/*---------------------------------
Exercise SQLColumns() fixes.
Fixes and test supplied by Steve Reynolds, sreynolds@bradmark.com
Exercises: SQLColumns,SQLGetDiagRec
-----------------------------------*/
#include "test_defs.h"
#include <stdio.h>
#include <assert.h>


#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>


/* 
    
    mv_xxx is a module level variable and hence static.

*/
/*
static SQLHANDLE EnvHandle = NULL;
static SQLHDBC ConHandle = NULL;
*/

/*  functions */

void LogODBCError (SQLRETURN nResult, SQLSMALLINT fHandleType,
		   SQLHANDLE handle);
int db_connect ();
char *Get_DSNName() {return (char*) dsn;}
char *Get_UserName() {return (char*) userName;}
char *Get_Password() {return (char*) pswd ; }
int print_columns (char *TableName);

/* code begins */


/*----------------------------------------------------------------

     main


-----------------------------------------------------------------*/
int
main (int argc, char **argv)
{


  RETCODE retcode = -1;
  int rc;
  GET_LOGIN_VARS()
  retcode = SQLAllocHandle (SQL_HANDLE_ENV, SQL_NULL_HENV, &EnvHandle);

  if (retcode != SQL_SUCCESS) {
    printf ("Unable to allocate ODBC environment handle!\n");

    return -1;
  }


  retcode =
    SQLSetEnvAttr (EnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3,
		   SQL_IS_INTEGER);

  if (retcode != SQL_SUCCESS) {
    printf ("Unable to set version of ODBC environment handle!\n");
    LogODBCError (retcode, SQL_HANDLE_ENV, EnvHandle);
    return -2;
  }

  rc = db_connect ();
  if (rc >= 0) {
    rc = print_columns ("SOME_NUMERIC_TYPES");
  }
  return rc;
}

/*-------------------------------------------------------------------------

     db_connect

     Form connection to the database.

---------------------------------------------------------------------------*/
int
db_connect ()
{

  SQLRETURN nResult = -1;
  SQLRETURN rc = -1;
  GET_LOGIN_VARS();

  nResult = SQLAllocHandle (SQL_HANDLE_DBC, EnvHandle, (SQLHDBC *) &ConHandle);


  if (nResult != SQL_SUCCESS) {
    printf ("ODBC Unable to allocate handle for connection!\n");
    LogODBCError (nResult, SQL_HANDLE_ENV, EnvHandle);
    return (-3);
  }

    if(dsn[0])
      nResult = SQLDriverConnect(ConHandle, NULL, dsn,
			    SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    else
      nResult = SQLConnect(ConHandle, twoTask, SQL_NTS, 
		      (SQLCHAR *)userName , 
		      SQL_NTS, (SQLCHAR *) pswd, SQL_NTS);

  /* if failed to connect, free the allocated hdbc before return */
  if (nResult != SQL_SUCCESS && nResult != SQL_SUCCESS_WITH_INFO) {
    printf ("ODBC Unable to connect using dsn %s with user %s!\n",
	    Get_DSNName (), Get_UserName ());

    LogODBCError (nResult, SQL_HANDLE_DBC, ConHandle);

    return (-4);
  }

  return 0;
}









/*-------------------------------------------------------------------------

    print_columns

    Print the column names from the given table.

---------------------------------------------------------------------------*/

int
print_columns (char *param_TableName)
{
  char *TableType = NULL;
  char schema[256] = { '\0', '\0', '\0' };
  char *schema_ptr = NULL;
  char *catalog_ptr = NULL;
  char TableName[129];
  char ColumnName[129];
  SQLSMALLINT ColumnType;
  char TableSchema[256];
  char TableCatalog[256];
  SQLSMALLINT schema_len, catalog_len, tname_len;
  SQLINTEGER tname_size, schema_size, catalog_size, cname_size, ctype_size;


  SQLHANDLE StmtHandle = NULL;
  SQLRETURN rc;
  int ret_len;
  int numret = 0;

  int i;
  int l, RemoteFilter;


  if (!param_TableName) {
    return -1;
  }

  /* Allocate An SQL Statement Handle */
  rc = SQLAllocHandle (SQL_HANDLE_STMT, ConHandle, &StmtHandle);
  if (rc != SQL_SUCCESS) {
    printf ("SQLAllocHandle call failed in print_columns on %s\n",
	    param_TableName);
    LogODBCError (rc, SQL_HANDLE_DBC, ConHandle);
    return -1;
  }

  catalog_ptr = NULL;
  strcpy (schema, Get_UserName ());
  schema_len = strlen (schema);
  for (i = 0; i < schema_len; i++) {
    schema[i] = (char) toupper (schema[i]);
  }
  schema_ptr = schema;


  tname_len = (SQLSMALLINT) strlen (param_TableName);


  RemoteFilter = 1;
  schema_len = (SQLSMALLINT) (schema_ptr ? strlen (schema_ptr) : 0);
  catalog_len = (SQLSMALLINT) (catalog_ptr ? strlen (catalog_ptr) : 0);



  rc = SQLColumns (StmtHandle,
		   (RemoteFilter ? catalog_ptr : NULL), catalog_len,
		   (RemoteFilter ? schema_ptr : NULL), schema_len,
		   param_TableName, tname_len, NULL, 0);

  if (rc != SQL_SUCCESS) {
    printf ("SQLColumns call failed in print_columns on %s.%s.%s\n",
	    (catalog_ptr ? catalog_ptr : "%"),
	    (schema_ptr ? schema_ptr : "%"), param_TableName);
    LogODBCError (rc, SQL_HANDLE_STMT, StmtHandle);
    return -5;
  }


  rc = SQLBindCol (StmtHandle, 5, SQL_SMALLINT, (SQLPOINTER)
		   & ColumnType, sizeof (ColumnType), &ctype_size);
  if (rc != SQL_SUCCESS) {
    printf
      ("SQLBindCol (column type) call failed in print_columns on %s.%s.%s\n",
       (catalog_ptr ? catalog_ptr : "%"), (schema_ptr ? schema_ptr : "%"),
       param_TableName);

    LogODBCError (rc, SQL_HANDLE_STMT, StmtHandle);
    return -6;
  }

  rc = SQLBindCol (StmtHandle, 4, SQL_C_CHAR, (SQLPOINTER)
		   & ColumnName, sizeof (ColumnName), &cname_size);
  if (rc != SQL_SUCCESS) {
    printf
      ("SQLBindCol (column name) call failed in print_columns on %s.%s.%s\n",
       (catalog_ptr ? catalog_ptr : "%"), (schema_ptr ? schema_ptr : "%"),
       param_TableName);

    LogODBCError (rc, SQL_HANDLE_STMT, StmtHandle);
    return -7;
  }

  rc = SQLBindCol (StmtHandle, 3, SQL_C_CHAR, (SQLPOINTER)
		   & TableName, sizeof (TableName), &tname_size);
  if (rc != SQL_SUCCESS) {
    printf
      ("SQLBindCol (table name) call failed in print_columns on %s.%s.%s\n",
       (catalog_ptr ? catalog_ptr : "%"), (schema_ptr ? schema_ptr : "%"),
       param_TableName);

    LogODBCError (rc, SQL_HANDLE_STMT, StmtHandle);
    return -8;
  }

  rc = SQLBindCol (StmtHandle, 2, SQL_C_CHAR, (SQLPOINTER)
		   & TableSchema, sizeof (TableSchema), &schema_size);
  if (rc != SQL_SUCCESS) {
    printf
      ("SQLBindCol (schema) call failed in print_columns on %s.%s.%s\n",
       (catalog_ptr ? catalog_ptr : "%"), (schema_ptr ? schema_ptr : "%"),
       param_TableName);
    LogODBCError (rc, SQL_HANDLE_STMT, StmtHandle);
    return -9;
  }

  rc = SQLBindCol (StmtHandle, 1, SQL_C_CHAR, (SQLPOINTER)
		   & TableCatalog, sizeof (TableCatalog), &catalog_size);
  if (rc != SQL_SUCCESS) {
    printf
      ("SQLBindCol (catalog) call failed in print_columns on %s.%s.%s\n",
       (catalog_ptr ? catalog_ptr : "%"), (schema_ptr ? schema_ptr : "%"),
       param_TableName);
    LogODBCError (rc, SQL_HANDLE_STMT, StmtHandle);
    return -10;
  }

  /* loop over the result set */

  ret_len = 0;
  rc = SQL_SUCCESS;
  l = 0;

  while (rc == SQL_SUCCESS) {
    ColumnName[0] = '\0';
    TableName[0] = '\0';
    TableSchema[0] = '\0';
    TableCatalog[0] = '\0';
    l++;
    rc = SQLFetch (StmtHandle);
    if (rc != SQL_SUCCESS && rc != SQL_NO_DATA) {
      printf
	("SQLFetch call number %d failed (%d) in print_columns on %s.%s.%s\n",
	 l, rc, (catalog_ptr ? catalog_ptr : "%"),
	 (schema_ptr ? schema_ptr : "%"), param_TableName);
      LogODBCError (rc, SQL_HANDLE_STMT, StmtHandle);
      return -11;
    }
    /* only permit exact matches! */
    if (rc != SQL_NO_DATA) {
      if ((strcmp (param_TableName, TableName) == 0) &&
	  ((catalog_ptr == NULL)
	   || (strcmp (catalog_ptr, TableCatalog) == 0))
	  && ((schema_ptr == NULL)
	      || (strcmp (schema_ptr, TableSchema) == 0))) {
	numret++;
	VERBOSE("SQLColumns returned a match %s.%s.%s. Column name is %s\n",
	   TableCatalog, TableSchema, TableName, ColumnName);
      }
      else {
	/*printf("SQLColumns returned a non-match %s.%s.%s\n",TableCatalog,TableSchema,TableName); */

      }
    }
  }

  VERBOSE("print_columns found %d matching columns:\n", numret);

  SQLFreeHandle (SQL_HANDLE_STMT, StmtHandle);

  if(rc==SQL_NO_DATA) return SQL_SUCCESS;
  return -1;

}


/*--------------------------------------------------------

     LogODBCError

     Discover what we can about an ODBC error and print it

-----------------------------------------------------------*/

#define MAXBUFLEN 4096
#define MAXLOGSIZE 16364


void
LogODBCError (SQLRETURN nResult, SQLSMALLINT fHandleType, SQLHANDLE handle)
{
  SQLTCHAR szErrState[SQL_SQLSTATE_SIZE + 1];	/* SQL Error State string */
  SQLTCHAR szErrText[SQL_MAX_MESSAGE_LENGTH + 1];	/* SQL Error Text string */
  char szBuffer[SQL_SQLSTATE_SIZE + SQL_MAX_MESSAGE_LENGTH + MAXBUFLEN + 1];
  /* formatted Error text Buffer */
  SQLSMALLINT wErrMsgLen = 0;	/* Error message length */
  SQLINTEGER dwErrCode;		/* Native Error code */

  SQLRETURN nErrResult = -1;	/* Return Code from SQLGetDiagRec */
  SQLSMALLINT sMsgNum = 1;
  int fFirstRun = 1;


  /* continue to bring WriteLogFile till all errors are displayed. 
     more than one call may be reqd. as err text has fixed
     string size. */

  /* call SQLGetDiagRec function with proper ODBC handles, repeatedly until
     function returns SQL_NO_DATA. Concatenate all error strings
     in the display buffer and display all results. */


  nErrResult = SQL_SUCCESS;
  while (SQL_SUCCEEDED (nErrResult)) {

    szBuffer[0] = '\0';
    dwErrCode = 0;
    szErrText[0] = '\0';
    szErrState[0] = '\0';
    wErrMsgLen = 0;

    nErrResult = SQLGetDiagRec (fHandleType, handle, sMsgNum++, szErrState,
				&dwErrCode, szErrText,
				SQL_MAX_MESSAGE_LENGTH - 1, &wErrMsgLen);

    if (SQL_SUCCEEDED (nErrResult)) {
      printf
	("SQL Error State:%s, Native Error Code: %lX, ODBC Error: %s\n",
	 (LPSTR) szErrState, dwErrCode, (LPSTR) szErrText);
    }
    else {
      if (!fFirstRun) {
	return;
      }
      printf ("Unable to retrieve ODBC error information, code %d\n",
	      nErrResult);
    }

    fFirstRun = 0;
  }




}

