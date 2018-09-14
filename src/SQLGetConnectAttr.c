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
 * $Id: SQLGetConnectAttr.c,v 1.6 2005/03/17 01:57:06 dbox Exp $
 *
 * $Log: SQLGetConnectAttr.c,v 
 * Revision 1.5  2003/12/16 01:22:06  dbo
 * changes contributed by Steven Reynolds sreynolds@bradmark.co
 * SQLFreeHandle.c: Change order of frees in _SQLFreeHandle(). Free oci_stm
 * handle first before other oci handles. Oracle OCI code was touching freed memory
 
 * SQLGetConnectAttr.c: SQLGetConnectAttr() was setting the commit mode
 
 * SQLGetDiagRec.c:  Remove call to ood_clear_diag in SQLGetDiagRec()
 * Otherwise client code was unable to get log messges
 
 * diagnostics.c: ood_post_diag() is allocates new error nodes, but code didn't set al
 * fields. Change malloc to calloc
 
 * oracle_functions.c: ood_driver_prepare() allocated a new oci statement with ou
 * freeing the existing one
 
 * oracle_functions.c: ocivnu_sqlslong() passed to OCI code sizeof(long), but bufle
 * was 4. This failed on Tru64 where sizeof(long) is 8. Change to pass min of thes
 * two values. Same in ocivnu_sqlulong()
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
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.9  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.8  2000/07/07 08:04:56  tom
 * SQL_ATTR_CURRENT_CATALOG
 * heavier tracing
 *
 * Revision 1.7  2000/06/05 16:03:30  tom
 * Deadlock avoidance
 *
 * Revision 1.6  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.5  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.4  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:27:00  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ********************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetConnectAttr.c,v $ $Revision: 1.6 $";
SQLRETURN SQL_API SQLGetConnectAttr(
    SQLHDBC                ConnectionHandle,
    SQLINTEGER            Attribute,
    SQLPOINTER            ValuePtr,
    SQLINTEGER            BufferLength,
    SQLINTEGER            *StringLengthPtr )
{
    hDbc_T* dbc=(hDbc_T*)ConnectionHandle;
    SQLRETURN status=SQL_SUCCESS;
    assert(IS_VALID(dbc));
if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)dbc,0,"i","Attribute",Attribute);
}
    ood_clear_diag((hgeneric*)dbc);

    switch(Attribute)
    {
        case SQL_ATTR_METADATA_ID:
        THREAD_MUTEX_LOCK(dbc);
        *((SQLUINTEGER*)ValuePtr)=dbc->metadata_id;
        THREAD_MUTEX_UNLOCK(dbc);
        break;

        case SQL_ATTR_TRACE:
        THREAD_MUTEX_LOCK(dbc);
        *((SQLUINTEGER*)ValuePtr)=dbc->trace;
        THREAD_MUTEX_UNLOCK(dbc);
        break;

        case SQL_ATTR_TRACEFILE:
        {
            SQLINTEGER i;
            THREAD_MUTEX_LOCK(dbc);
            i=ood_bounded_strcpy(ValuePtr,(char*)dbc->tracefile,BufferLength);
            THREAD_MUTEX_UNLOCK(dbc);
            if(!i)
            {
                ood_post_diag((hgeneric*)dbc,ERROR_ORIGIN_01004,0,"",
                        ERROR_MESSAGE_01004,
                        __LINE__,0,"",ERROR_STATE_01004,
                        __FILE__,__LINE__);
                status=SQL_SUCCESS_WITH_INFO;
            }
            else if(StringLengthPtr)
                *StringLengthPtr=i;
        }
        break;

		case SQL_ATTR_AUTOCOMMIT:
        THREAD_MUTEX_LOCK(dbc);
        *((SQLUINTEGER*)ValuePtr)=
			(dbc->autocommit==OCI_DEFAULT)?SQL_AUTOCOMMIT_OFF:SQL_AUTOCOMMIT_ON;
        THREAD_MUTEX_UNLOCK(dbc);
        break;

		case SQL_ATTR_CURRENT_CATALOG:
		*((SQLCHAR*)ValuePtr)='\0';
		if(StringLengthPtr)
			*StringLengthPtr=0;
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
