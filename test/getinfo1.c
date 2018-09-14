/* test all possible inputs to SQLGetInfo
 * author: Dennis Box, dbox@fnal.gov
 * $Id: getinfo1.c,v 1.7 2003/08/05 19:40:43 dbox Exp $
 */


/*    also  exercises following functions                          */
/*              SQLAllocHandle()                                   */
/*              SQLConnect()                                       */      
/*              SQLDisconnect()                                    */
/*              SQLFreeHandle()                                    */
/*              SQLSetEnvAttr()                                    */


#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>
#include "SQLGetInfoIn.h"
#include "test_defs.h"
#include <mem_functions.h>
#include <common.h>

typedef union {
  int   i[25];
  char s[100];
} out_p;
 

int main(int argc, char ** argv)
{ 
    out_p result;
    SQLSMALLINT some_val;
    int i;
    int iStart;
    int iEnd;

    GET_LOGIN_VARS();

    if(argc > 1 ){
     iStart = atoi(argv[1]);
     printf("starting sequence is %d \n",iStart);
    }else
      iStart=0;
   
    if(argc > 2){
     iEnd = atoi(argv[2]);
     printf("ending sequence is %d \n",iEnd);
    }else
      iEnd=getInfoOptsSIZE;

    /*printf ("start=%d end=%d argc=%d \n",iStart,iEnd, argc);*/
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
    assert(rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO);

    VERBOSE("connected to  database %s\n",twoTask);

    
    for(i=iStart; i<iEnd; i++){
      VERBOSE("checking index=%d infotype=%d name=%s\n",i,getInfoOpts[i],
				sql_get_info_type(getInfoOpts[i]));
      rc = SQLGetInfo(ConHandle,getInfoOpts[i],(void*)&result,
		      sizeof(result),&some_val);
      
      if(rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO){
	VERBOSE("test %s  returned status %d\n",
	       sql_get_info_type(getInfoOpts[i]),rc);
      }
    }

     
    result.i[0] = 0;
    rc = SQLGetInfo(ConHandle,SQL_TXN_CAPABLE,&result,
		    sizeof(result),&some_val);
    assert(rc==SQL_SUCCESS);

    assert(result.i[0]!=0); /*test transactions*/

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
