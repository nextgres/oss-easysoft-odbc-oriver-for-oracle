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
 * $Id: string_functions.c,v 1.2 2003/02/10 15:43:54 dbox Exp $
 *
 * $Log: string_functions.c,v $
 * Revision 1.2  2003/02/10 15:43:54  dbox
 * added unit test for SQLColumns, uppercased parameters to calls
 * to catalog functions
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.9  2000/07/21 10:20:41  tom
 * return_to_space added
 *
 * Revision 1.8  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.7  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.6  2000/05/03 16:13:25  tom
 * *** empty log message ***
 *
 * Revision 1.5  2000/04/27 14:34:30  tom
 * Forgot to increment str in con_strtok
 *
 * Revision 1.4  2000/04/27 13:46:06  tom
 * Added con_strtok
 *
 * Revision 1.3  2000/04/26 15:33:48  tom
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

#include "common.h"
#include <ctype.h>

static char const rcsid[]= "$RCSfile: string_functions.c,v $ $Revision: 1.2 $";

/*
 * xtoSQLNTS
 * 
 * Given an array of chars and a SQL length, will reliably return a null
 * terminated string. Useful in catalog functions etc.
 *
 * Beware, if the return != input string it has been malloc'd and needs
 * to be freed
 */

char* ood_xtoSQLNTS(SQLCHAR* str,SQLINTEGER str_len)
{
    char *local_str=NULL;
    int i;
    if(str_len==SQL_NTS){ 
        if(str&&str[0])
		for(i=0; i<strlen(str); i++)
			str[i]=toupper(str[i]);
        return((char*)str);
   }
    if(str_len>=0)
    {
        local_str=ORAMALLOC(str_len+1);
        memcpy(local_str,str,str_len);
        local_str[str_len] = '\0';
    }
    for(i=0; i<str_len; i++)local_str[i]=toupper(local_str[i]);
    return(local_str);
}


char* ood_xtoSQLNTS_orig(SQLCHAR* str,SQLINTEGER str_len)
{
    char *local_str=NULL;
    if(str_len==SQL_NTS) /* Nothing to do */
        return((char*)str);

    if(str_len>=0)
    {
        local_str=ORAMALLOC(str_len+1);
        memcpy(local_str,str,str_len);
        local_str[str_len] = '\0';
    }
    return(local_str);
}
/*
 * Fast strcat
 *
 * just like the normal strcat, but takes an extra parameter for a pointer to
 * the end of the first string and returns a pointer to the end of the 
 * resulting string.
 * Pass the extra parameter (endptr) as NULL to initialise;
 */
char* ood_fast_strcat(char *dest,char *src,char *endptr)
{
    if(!dest || !src)
    {
        endptr=NULL;
        return NULL;
    }
    if(!endptr) /* uninitialised... */
    {
        endptr=(char*)dest;
        while(*endptr)
            endptr++;
    }
    while(*src)
    {
        *endptr=*src;
        endptr++;
        src++;
    }
    *endptr='\0';
    return(endptr);
}

/*
 * bounded_strcpy
 * String copy with bounds check. Like strncpy but
 * 1) Will alway NULL terminate
 * 2) returns true if all OK, false if a bounds checking or other error occurs.
 */ 
int ood_bounded_strcpy(char *dest,char* src, int limit)
{
    int i;

    limit--;
    for(i=0;i<limit&&*src;i++)
    {
        *dest++=*src++;
    }
    *dest='\0';
    if(*src)
        return 0;
    return i;
}

/*
 * con_strtok
 * 
 * Sets the end of the first pair of tokens in the connect string to \0 and
 * returns a pinter to the next char... ie
 *
 * DRIVER={AAA;BBB;CCC};DSN=oracle; will become
 * DRIVER={AAA;BBB;CCC}\0 and a pointer to the D of DSN will be returned.
 * A NULL is returned on end of string.
 */
char *ood_con_strtok(char* str)
{
    int inbraces=0;
    while(*str)
    {
        if(*str=='{')
            inbraces++;
        if(*str=='}')
            inbraces--;
        if(!inbraces&&*str==';')
        {
            *str='\0';
            str++;
            if(*str)
                return(str);
            else 
                return NULL;
        }
        str++;
    }
    return NULL;
}

void return_to_space(char *s)
{
	while(*s&&!isspace(*s))
		s++;
	*s='\0';
}
