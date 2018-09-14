
#include "test_defs.h"
#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>



int main(int argc, char ** argv)
{

    SQLCHAR TableName[MAX_LEN];

    SQLCHAR ColumnName[MAX_LEN];
    SQLCHAR ColumnTypeName[MAX_LEN];
    SQLCHAR isNullable[MAX_LEN];
    SQLSMALLINT ColumnType;


    SQLINTEGER sz_i;
    SQLSMALLINT sz_si;
    SQLSMALLINT nullable_si;

    

    GET_LOGIN_VARS();
    

    if(argc > 1)
      sprintf(TableName,"%s",argv[1]);
    else
      sprintf(TableName,"%s","SOME_NUMERIC_TYPES");
      
    VERBOSE("calling SQLAllocHandle(EnvHandle) \n");

    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &EnvHandle);
    assert(rc == SQL_SUCCESS);
    assert(EnvHandle != (SQLHANDLE)NULL);


   
    rc = SQLSetEnvAttr(EnvHandle, SQL_ATTR_ODBC_VERSION, 
		       (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);

    assert(rc == SQL_SUCCESS);
        
    VERBOSE("calling SQLAllocHandle(ConHandle) \n");

    rc = SQLAllocHandle(SQL_HANDLE_DBC, EnvHandle, &ConHandle);
    assert(ConHandle != (SQLHANDLE)NULL);
    assert(rc == SQL_SUCCESS);
    if(dsn[0])
      rc = SQLDriverConnect(ConHandle, NULL, dsn,
			    SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    else
      rc = SQLConnect(ConHandle, twoTask, SQL_NTS, 
		      (SQLCHAR *)userName , 
		      SQL_NTS, (SQLCHAR *) pswd, SQL_NTS);
    assert(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO );

    VERBOSE("connected to  database %s\n",twoTask);
    VERBOSE("allocing handle\n");

    rc = SQLAllocStmt(ConHandle, &StmtHandle);
    assert(rc == SQL_SUCCESS);


    rc=SQLColumns(StmtHandle, NULL, 0,NULL,0,(SQLCHAR*)TableName,
		  SQL_NTS,NULL,0);
      
    assert(rc == SQL_SUCCESS);

    rc = SQLBindCol(StmtHandle, 4, SQL_C_CHAR, (SQLPOINTER)
		    &ColumnName, sizeof(ColumnName), NULL);
    assert(rc == SQL_SUCCESS);
    rc = SQLBindCol(StmtHandle, 5, SQL_C_SLONG, (SQLPOINTER)
		    &ColumnType, sizeof(ColumnType), NULL);
    assert(rc == SQL_SUCCESS);
    rc = SQLBindCol(StmtHandle, 6, SQL_C_CHAR, (SQLPOINTER)
		    &ColumnTypeName, sizeof(ColumnTypeName), NULL);
    assert(rc == SQL_SUCCESS);
    rc = SQLBindCol(StmtHandle, 7, SQL_C_SLONG, (SQLPOINTER)
		    &sz_i, sizeof(sz_i), NULL);
    assert(rc == SQL_SUCCESS);
    rc = SQLBindCol(StmtHandle, 18, SQL_C_CHAR, (SQLPOINTER)
		    &isNullable, sizeof(isNullable), NULL);
    assert(rc == SQL_SUCCESS);

    while (rc != SQL_NO_DATA)
      {
	rc = SQLFetch(StmtHandle);
	assert(rc==SQL_SUCCESS || rc==SQL_NO_DATA);
	if (rc != SQL_NO_DATA)
	  VERBOSE("ColumnName=%s type=%s num_type=%d  size=%d nullable=%s\n",
		  ColumnName,
		  ColumnTypeName,
		  ColumnType,
		  sz_i,
		  isNullable);
      }

    return 0;
}
