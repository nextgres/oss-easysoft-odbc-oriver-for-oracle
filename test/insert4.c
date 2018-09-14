


/* test inserts, using bound parameters  - works 
 * author: Dennis Box, dbox@fnal.gov
 * $Id: insert4.c,v 1.8 2004/08/27 19:53:49 dbox Exp $
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


    sprintf(SQLStmt,"insert into some_types values( ");
    strcat(SQLStmt," ?, ?, ? ) ");

    VERBOSE("preparing statement %s\n", SQLStmt);


    rc = SQLPrepare(StmtHandle, SQLStmt, SQL_NTS);
    assert(rc == SQL_SUCCESS);
 
    VERBOSE("binding....\n");

    rc = SQLBindParameter(StmtHandle, 1, SQL_PARAM_INPUT, 
			  SQL_INTEGER, SQL_INTEGER, 0, 0, &anInt, 0,
			  NULL);
    assert(rc == SQL_SUCCESS);

    rc = SQLBindParameter(StmtHandle, 2, SQL_PARAM_INPUT, 
			  SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &aFloat, 0,
			  NULL);
    assert(rc == SQL_SUCCESS);

    rc = SQLBindParameter(StmtHandle, 3, SQL_PARAM_INPUT, 
			  SQL_C_CHAR, SQL_CHAR, MAX_CHAR_LEN, 
			  0, aCharArray, MAX_CHAR_LEN,
			  NULL);
    assert(rc == SQL_SUCCESS);

    rc = SQLDescribeParam(StmtHandle,1,&DataType,&ParameterSize,
			  &DecimalDigits, &Nullable);
    VERBOSE("1:DataType=%d ParameterSize=%d DecimalDigits=%d Nullable=%d\n",
	    DataType,ParameterSize,DecimalDigits, Nullable);
    assert(rc == SQL_SUCCESS);
    
    rc = SQLDescribeParam(StmtHandle,2,&DataType,&ParameterSize,
			  &DecimalDigits,&Nullable);
    VERBOSE("2:DataType=%d ParameterSize=%d DecimalDigits=%d Nullable=%d\n",
	    DataType,ParameterSize,DecimalDigits, Nullable);
    assert(rc == SQL_SUCCESS);
   
    rc = SQLDescribeParam(StmtHandle,3,&DataType,
			  &ParameterSize,&DecimalDigits,&Nullable);
    VERBOSE("3:DataType=%d ParameterSize=%d DecimalDigits=%d Nullable=%d\n",
	    DataType,ParameterSize,DecimalDigits, Nullable);
    assert(rc == SQL_SUCCESS);

    VERBOSE("executing....\n");

    for(i=10;i<ARRAY_LEN+10;i++){
      anInt=i+1;
      aFloat=i+0.5;
      sprintf(aCharArray,"int=%d flt=%f",anInt,aFloat);
      VERBOSE("insert anInt=%d aFloat=%f charArray='%s'\n",
	      anInt,aFloat,aCharArray);
      rc = SQLExecute(StmtHandle);
      assert(rc == SQL_SUCCESS);
      VERBOSE("success: executed statement values %s\n",aCharArray);
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
