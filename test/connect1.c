/*      test following functions:                                  */
/*              SQLAllocHandle()                                   */
/*              SQLConnect()                                       */      
/*              SQLDisconnect()                                    */
/*              SQLFreeHandle()                                    */
/*              SQLSetEnvAttr()                                    */
/*
 * author: Dennis Box, dbox@fnal.gov
 * $Id: connect1.c,v 1.8 2005/11/01 20:48:56 dbox Exp $
 */


#include "test_defs.h"
#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>



int main(int argc, char**argv)
{
    SQLUSMALLINT  result;
    SQLSMALLINT some_val;
    int i;
    int num_cons;
    num_cons=1; 
    if(argc>1){
	num_cons=atoi(argv[1]);
	printf("connecting/disconnecting %d times\n",num_cons);
    }
    GET_LOGIN_VARS();
        
    for(i=0;i<num_cons;i++){   
    VERBOSE("calling SQLAllocHandle(EnvHandle) \n");
   
    rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &EnvHandle);
    assert(rc == SQL_SUCCESS);
    assert(EnvHandle != (SQLHANDLE)NULL);
    rc = SQLSetEnvAttr(EnvHandle, SQL_ATTR_ODBC_VERSION, 
		       (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);

    assert(rc == SQL_SUCCESS);


        
    	VERBOSE("calling SQLAllocHandle(ConHandle) \n");
	printf("connect %d\n",i);
    	rc = SQLAllocHandle(SQL_HANDLE_DBC, EnvHandle, &ConHandle);
    	assert(ConHandle != (SQLHANDLE)NULL);
    	assert(rc == SQL_SUCCESS);
    	if(dsn[0])
      		rc = SQLDriverConnect(ConHandle, NULL, dsn,
			    SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    	else
       		rc = SQLConnect(ConHandle, twoTask, SQL_NTS, 
		    (SQLCHAR *)userName , SQL_NTS, (SQLCHAR *) pswd, SQL_NTS);
    	assert(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);

    	VERBOSE("connected to  database %s\n",twoTask);

    	result = 0;
    	rc = SQLGetInfo(ConHandle,SQL_TXN_CAPABLE,&result,
		    sizeof(result),&some_val);
    	assert(result!=0); /* report transactions correctly? */

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
    } 

    return(rc);
}
