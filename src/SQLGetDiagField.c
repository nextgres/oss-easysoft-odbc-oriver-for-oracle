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
 * $Id: SQLGetDiagField.c,v 1.3 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLGetDiagField.c,v $
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
 * Revision 1.6  2000/05/04 15:53:44  tom
 * fixed a cut-and-paste error
 *
 * Revision 1.5  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.4  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.3  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.2  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetDiagField.c,v $ $Revision: 1.3 $";

/*
 * Please note this in implemented in diagnostics.h
 */

SQLRETURN SQL_API SQLGetDiagField(
    SQLSMALLINT            HandleType,
    SQLHANDLE            Handle,
    SQLSMALLINT            RecNumber,
    SQLSMALLINT            DiagIdentifier,
    SQLPOINTER            DiagInfoPtr,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *StringLengthPtr )
{
    SQLRETURN status=SQL_SUCCESS;
    hDbc_T* dbc=NULL;
if(ENABLE_TRACE){
    switch(HANDLE_TYPE(Handle))
    {
        case SQL_HANDLE_DBC:
            dbc=(hDbc_T*)Handle;
            break;
        case SQL_HANDLE_STMT:
            dbc=((hStmt_T*)Handle)->dbc;
            break;
        case SQL_HANDLE_DESC:
            dbc=((hDesc_T*)Handle)->dbc;
            break;
    }
    if(dbc){
        ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)Handle,0,"");
	assert(IS_VALID(dbc));
    }
}
     status=_SQLGetDiagField( HandleType, Handle, RecNumber, DiagIdentifier, 
            DiagInfoPtr, BufferLength, StringLengthPtr );

if(ENABLE_TRACE){
     if(dbc){
         ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
             (SQLHANDLE)NULL,status,"");
	 assert(IS_VALID(dbc));
     }
}
     return status;
}
