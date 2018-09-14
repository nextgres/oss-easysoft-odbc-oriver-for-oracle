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
 * $Id: SQLSetEnvAttr.c,v 1.1.1.1 2002/02/11 19:48:07 dbox Exp $
 *
 * $Log: SQLSetEnvAttr.c,v $
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.9  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.8  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.7  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.6  2000/04/27 13:45:42  tom
 * Reoved extraneous include
 *
 * Revision 1.5  2000/04/26 15:32:52  tom
 * Changed to account for new handle definitions
 *
 * Revision 1.4  2000/04/26 10:18:22  tom
 * Now sets ODBC_VER correctly
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:29:07  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLSetEnvAttr.c,v $ $Revision: 1.1.1.1 $";

SQLRETURN SQL_API SQLSetEnvAttr(
    SQLHENV            EnviromentHandle,
    SQLINTEGER        Attribute,
    SQLPOINTER        ValuePtr,
    SQLINTEGER        StringLength )
{
    hEnv_T *env=(hEnv_T*)EnviromentHandle;
    SQLRETURN status=SQL_SUCCESS;

    if(!env||HANDLE_TYPE(env)!=SQL_HANDLE_ENV)
    {
        status=SQL_INVALID_HANDLE;
    }
    ood_clear_diag((hgeneric*)env);
    THREAD_MUTEX_LOCK(env);

    switch(Attribute)
    {
        case SQL_ATTR_CONNECTION_POOLING:
            ood_post_diag((hgeneric*)env,ERROR_ORIGIN_HYC00,0,"",ERROR_MESSAGE_HYC00,
                    __LINE__,0,"",ERROR_STATE_HYC00,
                    __FILE__,__LINE__);
            status=SQL_ERROR;
            break;

        case SQL_ATTR_CP_MATCH:
            ood_post_diag((hgeneric*)env,ERROR_ORIGIN_HYC00,0,"",ERROR_MESSAGE_HYC00,
                    __LINE__,0,"",ERROR_STATE_HYC00,
                    __FILE__,__LINE__);
            status=SQL_ERROR;
            break;

        case SQL_ATTR_ODBC_VERSION:
            env->odbc_ver=(SQLINTEGER)ValuePtr;
            status=SQL_SUCCESS;
            break;

        case SQL_ATTR_OUTPUT_NTS:
            ood_post_diag((hgeneric*)env,ERROR_ORIGIN_HYC00,-1,"",ERROR_MESSAGE_HYC00,
                    __LINE__,-1,"",ERROR_STATE_HYC00,
                    __FILE__,__LINE__);
            status=SQL_ERROR;
            break;

        default:
            ood_post_diag((hgeneric*)env,ERROR_ORIGIN_HYC00,0,"",ERROR_MESSAGE_HYC00,
                    __LINE__,0,"",ERROR_STATE_HYC00,
                    __FILE__,__LINE__);
            status=SQL_ERROR;
    }
    THREAD_MUTEX_UNLOCK(env);
    return status;
}
