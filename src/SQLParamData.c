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
 * $Id: SQLParamData.c,v 1.5 2004/06/10 16:28:40 dbox Exp $
 *
 * $Log: SQLParamData.c,v $
 * Revision 1.5  2004/06/10 16:28:40  dbox
 * fixed some annoying behavior in SQLConnect and SQLDriverConnect where it didnt always find the server name
 *
 * Revision 1.4  2003/10/20 23:37:13  dbox
 * various changes to handle blob i/o
 *
 * Revision 1.3  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.2  2002/02/20 03:09:05  dbox
 * changed error reporting for stubbed out functions.  Added function calls
 * to 'test' subdirectory programs
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.10  2000/07/07 08:09:23  tom
 * logging altered to alert the log reader
 *
 * Revision 1.9  2000/06/06 10:24:42  tom
 * *** empty log message ***
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
 * Revision 1.2  2000/04/19 15:28:12  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLParamData.c,v $ $Revision: 1.5 $";

SQLRETURN SQL_API SQLParamData( 
    SQLHSTMT        StatementHandle,
    SQLPOINTER        *ValuePtrPtr )
{
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    /*
    SQLRETURN status=SQL_SUCCESS;
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"s",
			"SQLParamData","FIXME This function is not yet implemented");
}
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
fprintf(stderr,"called stubbed function line %d file %s\n",__LINE__,__FILE__);
    */
    if(ValuePtrPtr && (char)*ValuePtrPtr!='S'){
      *ValuePtrPtr = ORAMALLOC(strlen("Special Instructions")+1);
      strcpy((const char*)*ValuePtrPtr,"Special Instructions");
      //printf("set ValuePtrPtr to %s, goddammit\n",(const char*)*ValuePtrPtr);
      return SQL_NEED_DATA;
    }else{
      return SQL_SUCCESS;
    }
}
