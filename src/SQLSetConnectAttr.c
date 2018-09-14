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
 * $Id: SQLSetConnectAttr.c,v 1.5 2003/02/10 15:43:54 dbox Exp $
 *
 * $Log: SQLSetConnectAttr.c,v $
 * Revision 1.5  2003/02/10 15:43:54  dbox
 * added unit test for SQLColumns, uppercased parameters to calls
 * to catalog functions
 *
 * Revision 1.4  2002/11/14 22:28:36  dbox
 * %$@*&%$??!
 *
 * Revision 1.3  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.2  2002/05/14 23:01:06  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.10  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.9  2000/05/23 16:05:28  tom
 * Now sets autcommit correctly
 *
 * Revision 1.8  2000/05/11 13:27:45  tom
 * Bugfixes and tweaks
 *
 * Revision 1.7  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.6  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.5  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.4  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:28:47  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLSetConnectAttr.c,v $ $Revision: 1.5 $";
SQLRETURN SQL_API SQLSetConnectAttr(
    SQLHDBC            ConnectionHandle,
    SQLINTEGER        Attribute,
    SQLPOINTER        ValuePtr,
    SQLINTEGER        StringLength )
{
    hDbc_T* dbc=(hDbc_T*)ConnectionHandle;
    SQLRETURN status=SQL_SUCCESS;
    assert(IS_VALID(dbc));
if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)dbc,0,"ipi",
			"Attribute",Attribute,"ValuePtr",ValuePtr,
			"StringLength",StringLength);
}
    ood_clear_diag((hgeneric*)dbc);

    switch(Attribute)
    {
        case SQL_ATTR_METADATA_ID:
            THREAD_MUTEX_LOCK(dbc);
            dbc->metadata_id=*((SQLUINTEGER*)ValuePtr);
            THREAD_MUTEX_UNLOCK(dbc);
            break;

        case SQL_ATTR_ASYNC_ENABLE:
        case SQL_ATTR_ODBC_CURSORS:
        ood_post_diag((hgeneric*)dbc,ERROR_ORIGIN_HYC00,0,"",ERROR_MESSAGE_HYC00,
                __LINE__,0,"",ERROR_STATE_HYC00,
                __FILE__,__LINE__);
        status=SQL_ERROR;
        break;

        case SQL_ATTR_TRACE:
        THREAD_MUTEX_LOCK(dbc);
        dbc->trace=((SQLUINTEGER)ValuePtr);
        THREAD_MUTEX_UNLOCK(dbc);
        break;

        case SQL_ATTR_TRACEFILE:
        {
            SQLCHAR *tracefile;
            tracefile=(unsigned char*)ood_xtoSQLNTS_orig(ValuePtr,StringLength);
            THREAD_MUTEX_LOCK(dbc);
            if(!ood_bounded_strcpy((char*)dbc->tracefile,
						(char*)tracefile,FILENAME_MAX))
            {
                THREAD_MUTEX_UNLOCK(dbc);
                ood_post_diag((hgeneric*)dbc,ERROR_ORIGIN_01004,0,"",
                        ERROR_MESSAGE_01004,
                        __LINE__,0,"",ERROR_STATE_01004,
                        __FILE__,__LINE__);
                status=SQL_SUCCESS_WITH_INFO;
            }
			else
                THREAD_MUTEX_UNLOCK(dbc);
            if(tracefile!=ValuePtr)
                ORAFREE(tracefile);
        }
        break;

		case SQL_ATTR_AUTOCOMMIT:
        THREAD_MUTEX_LOCK(dbc);
		if( ((SQLUINTEGER)ValuePtr) == SQL_AUTOCOMMIT_ON )
		{
			dbc->autocommit=OCI_COMMIT_ON_SUCCESS;
		}
		else
		{
			dbc->autocommit=OCI_DEFAULT;
		}
        THREAD_MUTEX_UNLOCK(dbc);
		status=SQL_SUCCESS;
		break;

        default:
        ood_post_diag((hgeneric*)dbc,ERROR_ORIGIN_IM001,0,"",
                ERROR_MESSAGE_IM001,
                __LINE__,0,"",ERROR_STATE_IM001,
                __FILE__,__LINE__);
        status=SQL_SUCCESS_WITH_INFO;
    }
if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}

