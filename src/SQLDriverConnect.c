/********************************************************************************
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
 * $Id: SQLDriverConnect.c,v 1.6 2004/06/10 16:28:40 dbox Exp $
 *
 * $Log: SQLDriverConnect.c,v $
 * Revision 1.6  2004/06/10 16:28:40  dbox
 * fixed some annoying behavior in SQLConnect and SQLDriverConnect where it didnt always find the server name
 *
 * Revision 1.5  2003/08/05 19:40:43  dbox
 * changed tests to make less oracle specific
 *
 * Revision 1.4  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.3  2002/05/14 23:01:05  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.2  2002/05/02 15:39:48  dbox
 * fixed unused var warnings found by insure++
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.14  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.13  2000/07/07 08:00:59  tom
 * _SQLDriverConnect -> ood_SQLDriverConnect
 * heavier tracing
 *
 * Revision 1.12  2000/06/06 10:22:34  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.9  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.8  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.7  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.6  2000/04/27 13:44:38  tom
 * Fixed "quick and dirty" connection string parsing
 *
 * Revision 1.5  2000/04/26 15:29:08  tom
 * Redundant code removed.
 * Dodgy strncpy replaced
 *
 * Revision 1.4  2000/04/26 10:15:14  tom
 * Changes due to libclntsh no longer being dlopened
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:24:40  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLDriverConnect.c,v $ $Revision: 1.6 $";

SQLRETURN ood_SQLDriverConnect(
    SQLHDBC                ConnectionHandle,
    SQLHWND                WindowHandle,
    SQLCHAR                *InConnectionString,
    SQLSMALLINT            StringLength1,
    SQLCHAR                *OutConnectionString,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *StringLength2Ptr,
    SQLUSMALLINT        DriverCompletion )
{
    hDbc_T *dbc=(hDbc_T*)ConnectionHandle;
    char *local_str,         /* Local copy of connection str */
         *this_pair,         /* AAA=BBB to deal with now */
         *next_pair;         /* the next AAA=BBB to deal with */
    int len_constr;          /* real length of connection string */
    SQLRETURN status=SQL_SUCCESS;
    SQLSMALLINT ret;
    SQLCHAR trace_opt[4];
    assert(IS_VALID(dbc));

    if(!dbc||HANDLE_TYPE(dbc)!=SQL_HANDLE_DBC)
    {
        return SQL_INVALID_HANDLE;
    }
    ood_clear_diag((hgeneric*)dbc);

    if(StringLength1!=SQL_NTS)
        len_constr=StringLength1;
    else
        len_constr=strlen((const char*)InConnectionString);

    local_str=ORAMALLOC(len_constr+1);
    memcpy(local_str,InConnectionString,len_constr);
    local_str[len_constr] = '\0';
    
    THREAD_MUTEX_LOCK(dbc);
    
    *dbc->UID='\0';
    *dbc->PWD='\0';
    *dbc->DB='\0';
    *dbc->DSN='\0';

    this_pair=local_str;
    do
    {
        next_pair=(char*)ood_con_strtok(this_pair);
#if defined(WIN32)

        if(!strnicmp(this_pair,"DSN=",4))
            strcpy(dbc->DSN,&this_pair[4]);
        
        else if(!strnicmp(this_pair,"UID=",4))
            strcpy(dbc->UID,&this_pair[4]);

        else if(!strnicmp(this_pair,"PWD=",4))
            strcpy(dbc->PWD,&this_pair[4]);
        
        else if(!strnicmp(this_pair,"DB=",3))
            strcpy(dbc->DB,&this_pair[3]);

#else
        if(!strncasecmp(this_pair,"DSN=",4)){
            strcpy(dbc->DSN,&this_pair[4]);
	}
        else if(!strncasecmp(this_pair,"UID=",4)){
            strcpy(dbc->UID,&this_pair[4]);
	}
        else if(!strncasecmp(this_pair,"PWD=",4)){
            strcpy(dbc->PWD,&this_pair[4]);
	}
        else if(!strncasecmp(this_pair,"DB=",3)){
            strcpy(dbc->DB,&this_pair[3]);
	}
#endif

        this_pair=next_pair;
    }while(this_pair);
    ORAFREE(local_str);

    if(!*dbc->DB){
        ret=SQLGetPrivateProfileString(dbc->DSN,"DB",
                "",dbc->DB,128,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"Database",
                "",dbc->DB,128,"ODBC.INI");

    }
    if(!*dbc->UID){
        ret=SQLGetPrivateProfileString(dbc->DSN,"USER",
                "",dbc->UID,32,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"USERNAME",
                "",dbc->UID,32,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"UID",
                "",dbc->UID,32,"ODBC.INI");
    }
    if(!*dbc->PWD){
        ret=SQLGetPrivateProfileString(dbc->DSN,"PASSWORD",
                "",dbc->PWD,64,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"PWD",
                "",dbc->PWD,64,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"PASSWD",
                "",dbc->PWD,64,"ODBC.INI");
    }
    if(OutConnectionString&&BufferLength)
    {
        char OutTmp[512+FILENAME_MAX]; /* should be long enough */ 
        /*SQLSMALLINT strlentmp;*/

        sprintf(OutTmp,"DSN=%s;DB=%s;UID=%s;PWD=%s;",
                dbc->DSN,dbc->DB,dbc->UID,dbc->PWD);
        ood_bounded_strcpy((char*)OutConnectionString,OutTmp,BufferLength);
        /*strlentmp=(SQLSMALLINT)strlen(OutTmp);*/
        /**StringLength2Ptr=strlentmp>BufferLength?BufferLength:strlentmp;*/
    }

if(ENABLE_TRACE){
    /*
     * Get the tracing options 
     */
     SQLGetPrivateProfileString(dbc->DSN,"Trace",
             "No",trace_opt,4,"ODBC.INI");
     if(*trace_opt=='Y'||*trace_opt=='y')
         dbc->trace=SQL_OPT_TRACE_ON;
     else if(*trace_opt=='N'||*trace_opt=='n')
         dbc->trace=SQL_OPT_TRACE_OFF;

     SQLGetPrivateProfileString(dbc->DSN,"TraceFile",
             TRACEFILE_DEFAULT,dbc->tracefile,FILENAME_MAX,"ODBC.INI");

}


    /*
     * We now have all the information we need to be able to 
     * do a connect :- so go for it...
     */
    status=ood_driver_connect(dbc);
    THREAD_MUTEX_UNLOCK(dbc);
if(ENABLE_TRACE){
     ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
             (SQLHANDLE)dbc,0,"ssss",
			 NULL,"New Connection",
			 "DSN",dbc->DSN,
			 "DB",dbc->DB,
			 "USER",dbc->UID);
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}

SQLRETURN SQL_API SQLDriverConnect(
    SQLHDBC                ConnectionHandle,
    SQLHWND                WindowHandle,
    SQLCHAR                *InConnectionString,
    SQLSMALLINT            StringLength1,
    SQLCHAR                *OutConnectionString,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *StringLength2Ptr,
    SQLUSMALLINT        DriverCompletion )
{
    return ood_SQLDriverConnect(ConnectionHandle,WindowHandle,
			InConnectionString,
            StringLength1,OutConnectionString,BufferLength,StringLength2Ptr,
            DriverCompletion);
}
