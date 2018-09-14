/********************************************************************************
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
 ********************************************************************************
 *
 * $Id: string_functions.h,v 1.2 2003/02/10 15:43:54 dbox Exp $
 *
 * $Log: string_functions.h,v $
 * Revision 1.2  2003/02/10 15:43:54  dbox
 * added unit test for SQLColumns, uppercased parameters to calls
 * to catalog functions
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.7  2000/07/21 10:19:37  tom
 * return_to_space prototype added
 *
 * Revision 1.6  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.5  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.4  2000/04/27 13:46:30  tom
 * Added con_strtok
 *
 * Revision 1.3  2000/04/26 15:33:58  tom
 *
 * added bound_strcpy
 *
 * Revision 1.2  2000/04/20 10:50:31  nick
 *
 * Add to CVS and tidy up
 *
 * Revision 1.1  2000/04/19 15:31:39  tom
 *
 * New files; first functional checkin
 *
 *
 ********************************************************************************/

#ifndef _STRING_FUNCTIONS_H
#define _STRING_FUNCTIONS_H
#include <string.h>
char* ood_xtoSQLNTS(SQLCHAR* str,SQLINTEGER str_len);
char* ood_xtoSQLNTS_orig(SQLCHAR* str,SQLINTEGER str_len);
char* ood_fast_strcat(char *dest,char *src,char *endptr);
int ood_bounded_strcpy(char *dest,char* src, int limit);
char *ood_con_strtok(char* str);
void return_to_space(char *s);
#endif
