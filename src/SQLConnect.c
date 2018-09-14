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
 *
 *******************************************************************************
 *
 * $Id: SQLConnect.c,v 1.4 2004/08/02 21:21:16 dbox Exp $
 *
 * $Log: SQLConnect.c,v $
 * Revision 1.4  2004/08/02 21:21:16  dbox
 * tinkered w reading odbc.ini procedure
 *
 * Revision 1.3  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.2  2002/05/14 23:01:05  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.14  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.13  2000/07/07 07:59:55  tom
 * Heavier tracing
 *
 * Revision 1.12  2000/06/06 10:24:35  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.10  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
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
 * Revision 1.6  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.5  2000/04/26 15:28:13  tom
 * Changed to account for new handle definitions
 *
 * Revision 1.4  2000/04/26 10:14:50  tom
 * Changes due to libclntsh no longer being dlopened
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:24:19  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLConnect.c,v $ $Revision: 1.4 $";

SQLRETURN SQL_API SQLConnect(
    SQLHDBC         ConnectionHandle,
    SQLCHAR         *ServerName, 
    SQLSMALLINT     NameLength1,
    SQLCHAR         *UserName, 
    SQLSMALLINT     NameLength2,
    SQLCHAR         *Authentication, 
    SQLSMALLINT     NameLength3 )
{
    hDbc_T *dbc=(hDbc_T*)ConnectionHandle;
    SQLRETURN status=SQL_SUCCESS;
    SQLCHAR trace_opt[4];
    SQLSMALLINT ret;
    assert(IS_VALID(dbc));

    
    if(!dbc || HANDLE_TYPE(dbc)!=SQL_HANDLE_DBC)
    {
        return SQL_INVALID_HANDLE;
    }
    ood_clear_diag((hgeneric*)dbc);


    if(!ServerName || NameLength1==0)
    {
        ood_post_diag((hgeneric*)dbc,ERROR_ORIGIN_HY000,0,"",
                "A DSN is required",
                __LINE__,0,"",ERROR_STATE_HY000,
                __FILE__,__LINE__);
    }
    
    THREAD_MUTEX_LOCK(dbc);
    if(NameLength1>0)
    {
        memcpy(dbc->DSN,ServerName,NameLength1 );
        dbc->DSN[NameLength1]='\0';
    }
    else
    {
        strcpy(dbc->DSN,(const char*)ServerName);
    }

    /*
     * Now see if we can get the DB
     */
    ret=SQLGetPrivateProfileString(dbc->DSN,"DB",
            "",dbc->DB,128,"ODBC.INI");
    if(!ret)
	    ret=SQLGetPrivateProfileString(dbc->DSN,"Database",
            "",dbc->DB,128,"ODBC.INI"); 

    /*
     * For UID and PWD the defaults in the odbc.ini are overridden 
     * by the connection string.
     */
    if(NameLength2>0)
    {
        memcpy(dbc->UID,UserName,NameLength2 );
        dbc->UID[NameLength2]='\0';
    }
    else if (NameLength2==SQL_NTS && UserName && *UserName)
    {
        strcpy(dbc->UID,(const char*)UserName);
    }
    else
    {
        ret=SQLGetPrivateProfileString(dbc->DSN,"USER",
                "",dbc->UID,32,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"USERNAME",
		      "",dbc->UID,32,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"UID",
	               "",dbc->UID,32,"ODBC.INI");

    }
    
    if(NameLength3>0)
    {
        memcpy(dbc->PWD,Authentication,NameLength3 );
        dbc->PWD[NameLength3]='\0';
    }
    else if (NameLength3==SQL_NTS &&Authentication && *Authentication)
    {
        strcpy(dbc->PWD,(const char*)Authentication);
    }
    else
    {
        ret=SQLGetPrivateProfileString(dbc->DSN,"PASSWORD",
                "",dbc->PWD,64,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"PWD",
                  "",dbc->PWD,64,"ODBC.INI");
	if(!ret)
	  ret=SQLGetPrivateProfileString(dbc->DSN,"PASSWD",
                  "",dbc->PWD,64,"ODBC.INI");
    }

if(ENABLE_TRACE){
    /*
     * Get the tracing options 
     */
     SQLGetPrivateProfileString(dbc->DSN,"Trace",
             "No",trace_opt,4,"ODBC.INI");
     if(*trace_opt=='Y'||*trace_opt=='y')
     {
         dbc->trace=SQL_OPT_TRACE_ON;
     }
     else if(*trace_opt=='N'||*trace_opt=='n')
     {
         dbc->trace=SQL_OPT_TRACE_OFF;
     }

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
    return(status);
}
