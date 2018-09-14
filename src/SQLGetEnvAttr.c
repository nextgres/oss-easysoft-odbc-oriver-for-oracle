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
 * $Id: SQLGetEnvAttr.c,v 1.1.1.1 2002/02/11 19:48:07 dbox Exp $
 *
 * $Log: SQLGetEnvAttr.c,v $
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.8  2000/05/10 12:42:44  tom
 * Various updates
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
 * Revision 1.4  2000/04/26 10:16:50  tom
 * ODBCVER now returned correctly
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:27:27  tom
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetEnvAttr.c,v $ $Revision: 1.1.1.1 $";

SQLRETURN SQL_API SQLGetEnvAttr(
    SQLHENV                EnviromentHandle,
    SQLINTEGER            Attribute,
    SQLPOINTER            ValuePtr,
    SQLINTEGER            Bufferlength,
    SQLINTEGER            *StringLengthPtr )
{
    hEnv_T* env=(hEnv_T*)EnviromentHandle;
    SQLRETURN status=SQL_SUCCESS;

    ood_clear_diag((hgeneric*)env);
    THREAD_MUTEX_LOCK(env);

    switch(Attribute)
    {
        case SQL_ATTR_ODBC_VERSION:
            *((SQLINTEGER*)ValuePtr)=env->odbc_ver;
        break;

        case SQL_ATTR_OUTPUT_NTS:
            *((SQLINTEGER*)ValuePtr)=SQL_TRUE;
        break;
    }
    THREAD_MUTEX_UNLOCK(env);
    return status;
}
