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
 * $Id: SQLEndTran.c,v 1.3 2002/03/08 22:07:19 dbox Exp $
 *
 * $Log: SQLEndTran.c,v $
 * Revision 1.3  2002/03/08 22:07:19  dbox
 * added commit/rollback, more tests for SQLColAttribute
 *
 * Revision 1.2  2002/02/20 03:09:05  dbox
 * changed error reporting for stubbed out functions.  Added function calls
 * to 'test' subdirectory programs
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.5  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.4  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:24:45  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLEndTran.c,v $ $Revision: 1.3 $";

SQLRETURN _SQLEndTran(
    SQLSMALLINT            HandleType,
    SQLHANDLE            Handle,
    SQLSMALLINT            CompletionType )
{
  return ood_driver_transaction(Handle,CompletionType);
}

SQLRETURN SQL_API SQLEndTran(
    SQLSMALLINT            HandleType,
    SQLHANDLE            Handle,
    SQLSMALLINT            CompletionType )
{
    return _SQLEndTran(HandleType,Handle,CompletionType);
}

