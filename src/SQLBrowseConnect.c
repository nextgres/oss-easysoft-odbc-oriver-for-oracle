/******************************************************************************
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
 ******************************************************************************
 *
 * $Id: SQLBrowseConnect.c,v 1.3 2002/06/26 21:02:23 dbox Exp $
 *
 * $Log: SQLBrowseConnect.c,v $
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
 * Revision 1.8  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.7  2000/07/07 07:57:32  tom
 * _SQLDriverConnect -> ood_SQLDriverConnect
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
 * Revision 1.2  2000/04/19 15:23:34  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLBrowseConnect.c,v $ $Revision: 1.3 $";

SQLRETURN SQL_API SQLBrowseConnect(
    SQLHDBC            ConnectionHandle,
    SQLCHAR            *InConnectionString,
    SQLSMALLINT        StringLength1,
    SQLCHAR            *OutConnectionString,
    SQLSMALLINT        BufferLength,
    SQLSMALLINT        *StringLength2Ptr )
{
    /*
     * What we need to *guarantee* a connection is DB,UID and PWD
     */
    char *local_str,         /* Local copy of connection str */
         *this_pair,         /* AAA=BBB to deal with now */
         *next_pair;         /* the next AAA=BBB to deal with */
    int len_constr;          /* real length of connection string */
    int have_db=0,have_uid=0,have_pwd=0;
    char *out_end=NULL;
    hDbc_T* dbc=(hDbc_T*)ConnectionHandle;
    SQLRETURN status=SQL_SUCCESS;
    assert(IS_VALID(dbc));
if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)dbc,0,"");
}
    ood_clear_diag((hgeneric*)dbc);

    if(StringLength1!=SQL_NTS)
        len_constr=StringLength1;
    else
        len_constr=strlen((const char*)InConnectionString);

    local_str=ORAMALLOC(len_constr+1);
    memcpy(local_str,InConnectionString,len_constr);
    local_str[len_constr] = '\0';

    this_pair=local_str;
    do
    {
        next_pair=(char*)ood_con_strtok(this_pair);
#if defined(WIN32)

        if(!strnicmp(this_pair,"UID=",4))
            have_uid++;

        else if(!strnicmp(this_pair,"PWD=",4))
            have_pwd++;
        
        else if(!strnicmp(this_pair,"DB=",4))
            have_db++;

#else

        if(!strncasecmp(this_pair,"UID=",4))
            have_uid++;

        else if(!strncasecmp(this_pair,"PWD=",4))
            have_pwd++;
        
        else if(!strncasecmp(this_pair,"DB=",4))
            have_db++;

#endif

        this_pair=next_pair;
    }while(this_pair);
    ORAFREE(local_str);

    if(have_uid&&have_pwd&&have_db)
        status= ood_SQLDriverConnect(ConnectionHandle,NULL,
                InConnectionString,StringLength1,OutConnectionString,
                BufferLength,StringLength2Ptr,SQL_DRIVER_COMPLETE);
    else 
    {
        status=SQL_NEED_DATA;
        if(!have_uid)
        {
            out_end=ood_fast_strcat((char*)OutConnectionString,"UID=?;",out_end);
        }
        if(!have_pwd)
        {
            out_end=ood_fast_strcat((char*)OutConnectionString,"PWD=?;",out_end);
        }
        if(!have_db)
        {
            out_end=ood_fast_strcat((char*)OutConnectionString,"DB=?;",out_end);
        }
    }
if(ENABLE_TRACE){
    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return status;
}
