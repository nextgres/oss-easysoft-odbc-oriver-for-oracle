
#include "test_defs.h"
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif

#define BUFSIZE 200
#define CHUNKSIZE 20

SQLRETURN SendLongData(void)
{
    // Declare The Local Memory Variables
   
    SQLPOINTER  Value ;
    SQLCHAR     Buffer[20]; 
    SQLCHAR     InputParam[] = "Special Instructions";
    size_t      DataSize = 0;
    unsigned char in_data[200];
    int i;
    for(i=0;i<BUFSIZE;i++)in_data[i]=(unsigned char)(BUFSIZE-1-i);
    // Start The Data-At-Execution Sequence By Calling 
    // SQLParamData()
    rc = SQLParamData(StmtHandle, (SQLPOINTER *) &Value);
    VERBOSE("returned a value of  %s\n",(const char*)Value);
    // Examine The Contents Of Value (Returned By SQLParamData())
    // To Determine Which Data-At-Execution Parameter Currently
    // Needs Data
    if (strcmp((const char *) Value, (const char *) InputParam) == 0 
        && rc ==  SQL_NEED_DATA)
    {
        // As Long As Data Is Available For The Parameter, Retrieve
        // Part Of It From The External Data File And Send It To The
        // Data Source
      // while (InFile.get(Buffer, sizeof(Buffer)))
      //{
      while(DataSize < BUFSIZE)
	{
	  for (i=DataSize;i<DataSize+CHUNKSIZE;i++){
	    Buffer[i-DataSize]=in_data[i];
	    VERBOSE("Buffer[%d]=in_data[%d]=%d\n",i-DataSize,i,in_data[i]);
	  }
	  
	  rc = SQLPutData(StmtHandle, (SQLPOINTER) Buffer, 
			  CHUNKSIZE);
	  assert(rc==SQL_SUCCESS||rc==SQL_SUCCESS_WITH_INFO);
	  DataSize = DataSize + CHUNKSIZE;
	  
	  // If The Amount Of Data Retrieved Exceeds The Size Of 
	  // The Column (Which Is 200 Bytes), Call The SQLCancel() 
	  // Function To Terminate The Data-At-Execution 
	  // Sequence And Exit
	  if (DataSize > BUFSIZE) 
            {
	      rc = SQLCancel(StmtHandle);
	      return(SQL_ERROR);
            }
	} 
	  
      // Call SQLParamData() Again To Terminate The 
      // Data-At-Execution Sequence
      rc = SQLParamData(StmtHandle, (SQLPOINTER *) &Value);
      
      // Display A Message Telling How Many Bytes Of Data Were Sent
      if (rc == SQL_SUCCESS || rc == SQL_SUCCESS_WITH_INFO)
	{
	  VERBOSE("Successfully inserted %d\n",DataSize); 
	  VERBOSE(" bytes of data into the database.\n");
	}
      
    }
    // Close The External Data File
    //InFile.close();

    // Return The ODBC API Return Code To The Calling Function 
    return(rc);
}

/*-----------------------------------------------------------------*/
/* The Main Function                                               */
/*-----------------------------------------------------------------*/
int main()
{
    SQLCHAR      InputParam[25] = "Special Instructions";
    SQLINTEGER   Indicator = SQL_DATA_AT_EXEC;

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
  sprintf(SQLStmt,"drop table blob_test");
  rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);
  
  sprintf(SQLStmt,"create table blob_test (an_int integer primary key,  a_blob");

  if(strstr(dsn,"DSN=psql"))
     strcat(SQLStmt," bytea  ) ");
  else if(strstr(dsn,"DSN=mysql"))
     strcat(SQLStmt," blob  ) ");
  else
     strcat(SQLStmt," long raw  ) ");

  rc = SQLExecDirect(StmtHandle, SQLStmt, SQL_NTS);
  
  //if(rc == SQL_SUCCESS)
  //	VERBOSE("success: executed statement: %s \n", SQLStmt);
  

 

            // Define An INSERT SQL Statement That Contains A 
            // Data-At-Execution Parameter 
            strcpy((char *) SQLStmt, "INSERT INTO blob_test ");
            strcat((char *) SQLStmt, "(an_int, a_blob");
            strcat((char *) SQLStmt, " ) VALUES ");
            strcat((char *) SQLStmt, "(1, ?)");

            // Prepare The SQL Statement
            rc = SQLPrepare(StmtHandle, SQLStmt, SQL_NTS);

            // Bind The Parameter Marker In The SQL Statement To A
            // Local Application Variable - This Parameter Will Use
            // SQLParamData() And SQLPutData() To Send Data In Pieces
            // (Note: The Column Size Is 200 Characters)
            rc = SQLBindParameter(StmtHandle, 1, 
                     SQL_PARAM_INPUT, SQL_C_BINARY, SQL_CHAR, 200,
                     0, (SQLPOINTER) InputParam, CHUNKSIZE, &Indicator);

            // Execute The SQL Statement
            rc = SQLExecute(StmtHandle);

            // The Return Code SQL_NEED_DATA Should Be Returned,
            // Indicating That SQLParamData() And SQLPutData() Need
            // To Be Called
            assert(rc == SQL_NEED_DATA);
	    rc = SendLongData();
	    assert(rc==SQL_SUCCESS);
            // Free The SQL Statement Handle
            if (StmtHandle != NULL)
	      SQLFreeHandle(SQL_HANDLE_STMT, StmtHandle);
	
        // Disconnect From The Northwind Sample Database
        rc = SQLDisconnect(ConHandle);
	return rc;
}
 
