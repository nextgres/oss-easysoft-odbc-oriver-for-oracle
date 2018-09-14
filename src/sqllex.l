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
 * $Id: sqllex.l,v 1.3 2003/01/30 19:16:01 dbox Exp $
 *
 
 *
 ******************************************************************************/
%{
static char const rcsid[]= "$RCSfile: sqllex.l,v $ $Revision: 1.3 $";
#undef YY_DECL
#define YY_DECL static int ood_yylex(char *sql,char *buf, char *sql_end,int* num_params)
#ifndef YY_USE_PROTOS 
#define YY_USE_PROTOS 1
#endif
#ifndef YY_NO_UNPUT
#define YY_NO_UNPUT 1
#endif
#include "common.h"

#if defined(HAVE_LIBPTHREAD)
    pthread_mutex_t ood_global_mutex;
#elif defined(WIN32)
    HANDLE ood_global_mutex;
#endif
    int ood_thread_init=0;

int yywrap(void);

%}
DIGIT [0-9]
DATESEP -
TIMESEP :
DATE {DIGIT}{4}{DATESEP}{DIGIT}{2}{DATESEP}{DIGIT}{2}
TIME {DIGIT}{2}{TIMESEP}{DIGIT}{2}{TIMESEP}{DIGIT}{2}
%%
	/* escape anything */
\\	yymore();

	/* quote quoted strings literally */
\'[^\']*\' 				sql_end=ood_fast_strcat(buf,yytext,sql_end);

\{d" "+\'{DATE}\'\} {
                        char *quot;
						quot=strtok(yytext,"'");
						quot=strtok(NULL,"'");
						sql_end=ood_fast_strcat(buf,"TO_DATE('",sql_end);
						sql_end=ood_fast_strcat(buf,quot,sql_end);
						sql_end=ood_fast_strcat(buf,"','YYYY-MM-DD')",sql_end);
					}

\{t" "+\'{TIME}\'\} {
                        char *quot;
						quot=strtok(yytext,"'");
						quot=strtok(NULL,"'");
						sql_end=ood_fast_strcat(buf,"TO_DATE('",sql_end);
						sql_end=ood_fast_strcat(buf,quot,sql_end);
						sql_end=ood_fast_strcat(buf,"','HH24:MI:SS')",sql_end);
					}

\{ts" "+\'{DATE}" "{TIME}\'\} {
                        char *quot;
						quot=strtok(yytext,"'");
						quot=strtok(NULL,"'");
						sql_end=ood_fast_strcat(buf,"TO_DATE('",sql_end);
						sql_end=ood_fast_strcat(buf,quot,sql_end);
						sql_end=ood_fast_strcat(buf,"','YYYY-MM-DD HH24:MI:SS')",sql_end);
					}


	/* transpose ? to : */
\?	{
        char placeholder[8];
		sprintf(placeholder,":P%d",(*num_params)++);
		sql_end=ood_fast_strcat(buf,placeholder,sql_end);
	}

	/*semicolon on the end*/
\;" "*$	return 1;

	/*default */
.|\n	sql_end=ood_fast_strcat(buf,yytext,sql_end);
%%
char* ood_lex_parse(char *stringin, int len, int *num_params)
{
    char *sql_end=NULL,*sql_in=NULL,*sql_out=NULL;
	int limit;
    YY_BUFFER_STATE buf;
    if(!ood_thread_init)
	{
#if defined(HAVE_LIBPTHREAD)
	    pthread_mutex_init(&ood_global_mutex,NULL);
#elif defined(WIN32)
        ood_global_mutex=CreateMutex(NULL,FALSE,NULL);
#endif
		ood_thread_init++;
	}
	sql_in=ood_xtoSQLNTS((unsigned char*)stringin,len);
	sql_out=malloc(limit=(strlen(sql_in)*2));
	if(sql_out==NULL)
	    return NULL;
	*sql_out='\0';
    GLOBAL_MUTEX_LOCK(ood_global_mutex);
	buf=yy_scan_string(sql_in);
	ood_yylex(sql_in,sql_out,sql_end,num_params);
	yy_delete_buffer(buf);
    GLOBAL_MUTEX_UNLOCK(ood_global_mutex);
	if(sql_in!=stringin)
	    free(sql_in);
	return sql_out;
}

int yywrap(void){return 1;}


#ifdef TEST_PARSER
int main()
{
    char bufin[BUFSIZ],bufout[BUFSIZ];
	printf("YYIN :");
	fgets(bufin,BUFSIZ,stdin);
	yy_scan_string(bufin,&bufout[BUFSIZ-1]);
	yylex(bufout);
	printf("YYOUT: [%s]\n",bufout); 
}
#endif


