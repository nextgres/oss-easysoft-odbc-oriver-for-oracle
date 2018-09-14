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
 * $Id: SQLGetFunctions.c,v 1.4 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLGetFunctions.c,v $
 * Revision 1.4  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.3  2002/05/14 23:01:06  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.2  2002/03/08 22:07:19  dbox
 * added commit/rollback, more tests for SQLColAttribute
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.15  2000/07/21 10:26:15  tom
 * SQL_API_SQLFETCHSCROLL,SQL_API_SQLEXTENDEDFETCH uncommented
 *
 * Revision 1.14  2000/07/07 08:06:40  tom
 * uncomments for new functions
 *
 * Revision 1.13  2000/06/06 10:23:01  tom
 * Tidy up for possible release 0.0.3
 *
 * Revision 1.12  2000/06/05 16:04:28  tom
 * Boundparam and cancel uncommented
 *
 * Revision 1.11  2000/05/11 13:24:49  tom
 * Added SQLGetTypeInfo
 *
 * Revision 1.10  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.9  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.8  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.7  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.6  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.5  2000/04/27 13:45:11  tom
 * Various tweaks to the functions reported
 *
 * Revision 1.4  2000/04/26 10:17:10  tom
 * Various changes to reported function list
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:27:32  tom
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 *
 * Added files
 *
 *
 *******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetFunctions.c,v $ $Revision: 1.4 $";

int supported_functions[] = 
{
    SQL_API_SQLALLOCHANDLE,
    SQL_API_SQLALLOCHANDLESTD,
    /*SQL_API_SQLALLOCSTMT,*/
    SQL_API_SQLBINDCOL,
    SQL_API_SQLBINDPARAMETER,
    SQL_API_SQLBROWSECONNECT,
    SQL_API_SQLCANCEL,
    SQL_API_SQLCLOSECURSOR,
    SQL_API_SQLCOLATTRIBUTE,
    SQL_API_SQLCOLUMNPRIVILEGES,
    SQL_API_SQLCOLUMNS,
    SQL_API_SQLCONNECT,
    SQL_API_SQLDRIVERCONNECT,
    SQL_API_SQLDESCRIBECOL,
    SQL_API_SQLDESCRIBEPARAM,
    SQL_API_SQLDISCONNECT,
    SQL_API_SQLENDTRAN, 
    SQL_API_SQLEXECDIRECT,
    SQL_API_SQLEXECUTE,
    SQL_API_SQLEXTENDEDFETCH,
    SQL_API_SQLFETCH,
    SQL_API_SQLFETCHSCROLL,
    SQL_API_SQLFOREIGNKEYS,
    SQL_API_SQLFREEHANDLE,
    SQL_API_SQLFREESTMT, 
    SQL_API_SQLGETCONNECTATTR,
/*    SQL_API_SQLGETCURSORNAME,*/
    SQL_API_SQLGETDATA,
/*    SQL_API_SQLGETDESCFIELD,*/
/*    SQL_API_SQLGETDESCREC,*/
    SQL_API_SQLGETDIAGFIELD,
    SQL_API_SQLGETDIAGREC,
    SQL_API_SQLGETENVATTR,
    SQL_API_SQLGETFUNCTIONS,
    SQL_API_SQLGETINFO,
    SQL_API_SQLGETSTMTATTR,
    SQL_API_SQLGETTYPEINFO,
    /*SQL_API_SQLMORERESULTS,*/
/*    SQL_API_SQLNATIVESQL,*/
    SQL_API_SQLNUMPARAMS,
    SQL_API_SQLNUMRESULTCOLS,
/*    SQL_API_SQLPROCEDURES,*/
/*    SQL_API_SQLPROCEDURECOLUMNS,*/
    SQL_API_SQLPARAMDATA,
    SQL_API_SQLPUTDATA,
    SQL_API_SQLPREPARE,
    SQL_API_SQLPRIMARYKEYS,
    SQL_API_SQLROWCOUNT,
/*    SQL_API_SQLSETDESCFIELD,*/
/*    SQL_API_SQLSETDESCREC,*/
    SQL_API_SQLSETCONNECTATTR,
/*    SQL_API_SQLSETCURSORNAME,*/
    SQL_API_SQLSETENVATTR, 
    /*SQL_API_SQLSETPOS,*/
    SQL_API_SQLSETSTMTATTR, 
    SQL_API_SQLSPECIALCOLUMNS,
    SQL_API_SQLSTATISTICS,
    SQL_API_SQLTABLEPRIVILEGES,
    SQL_API_SQLTABLES,
    /*SQL_API_SQLALLOCCONNECT,*/
    /*SQL_API_SQLALLOCENV,*/
/*    SQL_API_SQLSETSTMTOPTION,*/
/*    SQL_API_SQLSETCONNECTOPTION,*/
    /*SQL_API_SQLGETSTMTOPTION,*/
/*    SQL_API_SQLERROR,*/
    /*SQL_API_SQLFREECONNECT,*/
    /*SQL_API_SQLFREEENV,*/
/*    SQL_API_SQLSETSCROLLOPTIONS,*/
/*    SQL_API_SQLPARAMOPTIONS,*/
/*    SQL_API_SQLGETCONNECTOPTION,*/
/*    SQL_API_SQLBINDPARAM,*/
/*    SQL_API_SQLSETPARAM*/
};


SQLRETURN SQL_API SQLGetFunctions(
    SQLHDBC            ConnectionHandle,
    SQLUSMALLINT    FunctionId,
    SQLUSMALLINT    *SupportedPtr )
{
    int i;
    hDbc_T* dbc=(hDbc_T*)ConnectionHandle;

    if(!ConnectionHandle || 
            ((hgeneric*)ConnectionHandle)->htype!=SQL_HANDLE_DBC)
    {
        return SQL_INVALID_HANDLE;
    }
    assert(IS_VALID(dbc));
if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)dbc,0,"Uh"
            ,"FunctionId",FunctionId,
            "SupportedPtr",SupportedPtr);
}
    ood_clear_diag((hgeneric*)dbc);

    if ( FunctionId == SQL_API_ODBC3_ALL_FUNCTIONS )
    {
        for ( i = 0; i < SQL_API_ODBC3_ALL_FUNCTIONS_SIZE; i ++ )
        {
            SupportedPtr[ i ] = 0x0000;
        }
        for ( i = 0; i < sizeof( supported_functions ) / sizeof( supported_functions[ 0 ] ); i ++ )
        {
        int id = supported_functions[ i ];
    
            SupportedPtr[ id >> 4 ] |= ( 1 << ( id & 0x000F ));
        }
    }
    else if ( FunctionId == SQL_API_ALL_FUNCTIONS )
    {
        for ( i = 0; i < 100; i ++ )
        {
            SupportedPtr[ i ] = SQL_FALSE;
        }
        for ( i = 0; i < sizeof( supported_functions ) / sizeof( supported_functions[ 0 ] ); i ++ )
        {
            if ( supported_functions[ i ] < 100 )
            {
                SupportedPtr[ supported_functions[ i ]] = SQL_TRUE;
            }
        }
    }
    else
    {
        *SupportedPtr = SQL_FALSE;
        for ( i = 0; i < sizeof( supported_functions ) / sizeof( supported_functions[ 0 ] ); i ++ )
        {
            if ( supported_functions[ i ] == FunctionId )
            {
                *SupportedPtr = SQL_TRUE;
                break;
            }
        }
    }

if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,SQL_SUCCESS,"U",
            "*SupportedPtr",*SupportedPtr);
}
    return SQL_SUCCESS;
}
