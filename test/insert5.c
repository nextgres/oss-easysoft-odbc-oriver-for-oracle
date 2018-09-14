
/* test inserts of ints, using bound parameters*/
/* tests that a bug was fixed in driver where calling SQLPrepareStmt followed
 * by SQLDescribeParam resulted in a core dump.  This is fixed but 
 * SQLDescribeParam is returning incorrect info despite reporting SQL_SUCCESS
 * author: Dennis Box, dbox@fnal.gov
 * $Id: insert5.c,v 1.3 2003/08/05 19:40:43 dbox Exp $
 */


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
    SQLINTEGER   anInt,cbInt,cbFloat;
    SQLFLOAT   aFloat;
    SQLCHAR   aCharArray[MAX_CHAR_LEN];
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

    sprintf(SQLStmt, "drop table int_table");
    rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);

    sprintf(SQLStmt, "create table int_table (an_int integer)");
    rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);
    assert(rc==SQL_SUCCESS);

    sprintf(SQLStmt,"insert into int_table values( ");
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
			  SQL_C_LONG, SQL_INTEGER, 0, 0, &anInt, 0,
			  &cbInt);
    assert(rc == SQL_SUCCESS);
    
    VERBOSE("executing....\n");

    for(i=0;i<ARRAY_LEN;i++){
      anInt=i;
       rc = SQLExecute(StmtHandle);
      assert(rc == SQL_SUCCESS);
      VERBOSE("success: executed statement values %d\n",anInt);
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
