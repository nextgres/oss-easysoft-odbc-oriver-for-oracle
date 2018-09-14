/*test insertion of dates*/

#include "test_defs.h"

#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>



int main()
{
    // Declare The Local Memory Variables
    #define MAX_CHAR_LEN 255
    #define ARRAY_LEN 3
    SQLINTEGER   cbDate;
    SQL_DATE_STRUCT   aDate;
    SQLCHAR      dateStr[56];
    int i;

     SQLSMALLINT      DataType;
    SQLUINTEGER      ParameterSize;
    SQLSMALLINT      DecimalDigits;
    SQLSMALLINT      Nullable;


    GET_LOGIN_VARS();
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
		    (SQLCHAR *)userName , SQL_NTS, (SQLCHAR *) pswd, SQL_NTS);
    assert(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO );

    VERBOSE("connected to  database %s\n",twoTask);

    

    rc = SQLAllocStmt(ConHandle, &StmtHandle);
    assert(rc == SQL_SUCCESS);

    sprintf(SQLStmt, "drop table date_table");
    rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);

    sprintf(SQLStmt, "create table date_table (a_date date)");
    rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);
    assert(rc==SQL_SUCCESS);

    sprintf(SQLStmt,"insert into date_table values( ");
    strcat(SQLStmt," ? ) ");

    VERBOSE("preparing statement %s\n", SQLStmt);


    rc = SQLPrepare(StmtHandle, SQLStmt, SQL_NTS);
    assert(rc == SQL_SUCCESS);


    rc = SQLDescribeParam(StmtHandle,1,&DataType,&ParameterSize,
			  &DecimalDigits, &Nullable);
    VERBOSE("1:DataType=%d ParameterSize=%d DecimalDigits=%d Nullable=%d\n",
	    DataType,ParameterSize,DecimalDigits, Nullable);
    assert(rc == SQL_SUCCESS);

    VERBOSE("binding....\n");

    rc = SQLBindParameter(StmtHandle, 1, SQL_PARAM_INPUT, 
			  SQL_C_CHAR, SQL_TYPE_DATE, 0, 0, dateStr, 
			  sizeof(dateStr),
			  NULL);
    assert(rc == SQL_SUCCESS);
    
    VERBOSE("executing....\n");

    for(i=0;i<ARRAY_LEN;i++){
      sprintf(dateStr, "2%d-Sep-2002",i+1);
      VERBOSE("inserting %s\n",dateStr);
      rc = SQLExecute(StmtHandle);
      assert(rc == SQL_SUCCESS);
      VERBOSE("success: inserted %s \n",dateStr);
    }


    VERBOSE("calling SQLFreeStmt\n");
    if (StmtHandle != NULL)
      rc=SQLFreeHandle(SQL_HANDLE_STMT,StmtHandle);
    assert(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);


    rc = SQLDisconnect(ConHandle);
    assert(rc == SQL_SUCCESS);
    VERBOSE("disconnected from  database\n");
     

    VERBOSE("calling SQLFreeHandle(ConHandle) \n");

    assert (ConHandle != (SQLHANDLE)NULL);
    rc = SQLFreeHandle(SQL_HANDLE_DBC, ConHandle);
    assert(rc == SQL_SUCCESS);
   
    VERBOSE("calling SQLFreeHandle(EnvHandle) \n");

    assert (EnvHandle != (SQLHANDLE)NULL);
    rc = SQLFreeHandle(SQL_HANDLE_ENV, EnvHandle);
    assert(rc == SQL_SUCCESS);
   

    return(rc);
}
