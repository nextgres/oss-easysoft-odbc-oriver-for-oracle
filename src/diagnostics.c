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
 * $Id: diagnostics.c,v 1.5 2005/03/17 01:57:06 dbox Exp $
 *
 * $Log: diagnostics.c,v 
 * Revision 1.4  2004/08/27 19:51:29  dbo
 * added a new error conditio
 *
 * Revision 1.3  2003/12/16 01:22:06  dbox
 * changes contributed by Steven Reynolds sreynolds@bradmark.com
 * SQLFreeHandle.c: Change order of frees in _SQLFreeHandle(). Free oci_stmt
 * handle first before other oci handles. Oracle OCI code was touching freed memory.
 *
 * SQLGetConnectAttr.c: SQLGetConnectAttr() was setting the commit mode.
 *
 * SQLGetDiagRec.c:  Remove call to ood_clear_diag in SQLGetDiagRec().
 * Otherwise client code was unable to get log messges.
 *
 * diagnostics.c: ood_post_diag() is allocates new error nodes, but code didn't set all
 * fields. Change malloc to calloc.
 *
 * oracle_functions.c: ood_driver_prepare() allocated a new oci statement with out
 * freeing the existing one.
 *
 * oracle_functions.c: ocivnu_sqlslong() passed to OCI code sizeof(long), but buflen
 * was 4. This failed on Tru64 where sizeof(long) is 8. Change to pass min of these
 * two values. Same in ocivnu_sqlulong().
 *
 * Revision 1.2  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.14  2000/07/21 10:23:06  tom
 * DiagRec output nulled for return=SQL_NO_DATA
 *
 * Revision 1.13  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.12  2000/07/07 07:41:20  tom
 * Added NULL pointer checks
 * Added row count (code from Steve Gilbert)
 *
 * Revision 1.11  2000/06/05 15:52:38  tom
 * Fixed broken cast
 *
 * Revision 1.10  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.9  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.8  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.7  2000/05/03 16:03:53  tom
 * Thread safety stuff
 *
 * Revision 1.6  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.5  2000/04/26 15:25:09  tom
 * general code clean up
 *
 * Revision 1.4  2000/04/26 10:11:15  tom
 * Various fixes
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:30:38  tom
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ********************************************************************************/

#include "common.h"
#include <sqlext.h>

/*
 * Centralised errors config
 */

char *error_messages[]=
{
    "String data, right truncated",                                 /* 01004 */
    "Option value changed",                                         /* 01S02 */
    "Fractional truncation",                                        /* 01S07 */
    "Invalid keyword",                                              /* 01S09 */
    "Count field incorrect",                                        /* 07002 */
    "Prepared statement not a cursor-specification",                /* 07005 */
    "Restricted data type attribute violation",                     /* 07006 */
    "Invalid descriptor index",                                     /* 07009 */
    "Client Unable to Establish Connection",                        /* 08001 */
    "Connection name in use",                                       /* 08002 */
    "Connection does not exist",                                    /* 08003 */
    "Server Rejected Connection",                                   /* 08004 */
    "Communications Failure",                                       /* 08S01 */
    "Degree of derived table does not match column list",           /* 21S02 */
    "String data, right truncated",                                 /* 22001 */
    "Indicator variable required but not supplied",                 /* 22002 */
    "Numeric value out of range",                                   /* 22003 */
    "Invalid datetime format",                                      /* 22007 */
    "Data value is not a valid datetime",                           /* 22008 */
    "Division by zero",                                             /* 22012 */
    "Interval field overflow",                                      /* 22015 */
    "Integrity Constraint Violation",                               /* 23000 */
    "Invalid cursor state",                                         /* 24000 */
    "Invalid transaction state",                                    /* 25000 */
    "Transaction state unknown",                                    /* 25S01 */
    "Invalid cursor name",                                          /* 34000 */
    "Duplicate cursor name",                                        /* 3C000 */
    "Syntax error or access violation",                             /* 42000 */
    "Serialization failure",                                        /* 40001 */
    "Base table or view not found",                                 /* 42S02 */
    "Stored Procedure nor found",                                   /* 42S02a */
    "Column not found in table ",                                   /* 42S22 */
    "General Error",                                                /* HY000 */
    "Memory Allocation Error",                                      /* HY001 */
    "Invalid Application Buffer Type",                              /* HY003 */
    "Invalid SQL Data Type",                                        /* HY004 */
    "Associated statement is not prepared",                         /* HY007 */
    "Operation Canceled",                                           /* HY008 */
    "Invalid use of null pointer",                                  /* HY009 */
    "Function sequence error",                                      /* HY010 */
    "Attribute cannot be set now",                                  /* HY011 */
    "Invalid attribute/option identifier",                          /* HY012 */
    "Invalid use of a automatically allocated descriptor handle",   /* HY017 */
    "Non-character and non-binary data sent in pieces",             /* HY019 */
    "Attempt to concatenate a null value",                          /* HY020 */
    "Invalid attribute value",                                      /* HY024 */
    "Invalid string or buffer length",                              /* HY090 */
    "Invalid descriptor field identifier",                          /* HY091 */
    "Option type out of range",                                     /* HY092 */
    "Information type out of range",                                /* HY096 */
    "Fetch type out of range",                                      /* HY106 */
    "Invalid cursor position",                                      /* HY109 */
    "Invalid bookmark value",                                       /* HY111 */
    "Optional feature not implemented",                             /* HYC00 */
    "Timeout Expired",                                              /* HYT00 */
    "Driver does not support this function",                        /* IM001 */
    "Data source name not found and no default driver specified",   /* IM002 */
    "Row value out of range",                                       /* S1107 */
    "Concurrency option out of range",                              /* S1108 */
    "Driver not capable",                                           /* S1C00 */
};

char *error_states[]={ 
 "01004",
 "01S02",
 "01S07",
 "01S09",
 "07002",
 "07005",
 "07006",
 "07009",
 "08001",
 "08002",
 "08003",
 "08004",
 "08S01",
 "21S02",
 "22001",
 "22002",
 "22003",
 "22007",
 "22008",
 "22012",
 "22015",
 "23000",
 "24000",
 "25000",
 "25S01",
 "34000",
 "3C000",
 "40001",
 "42000",
 "42S02",
 "42S02",
 "42S22",
 "HY000",
 "HY001",
 "HY003",
 "HY004",
 "HY007",
 "HY008",
 "HY009",
 "HY010",
 "HY011",
 "HY012",
 "HY017",
 "HY019",
 "HY020",
 "HY024",
 "HY090",
 "HY091",
 "HY092",
 "HY096",
 "HY106",
 "HY109",
 "HY111",
 "HYC00",
 "HYT00",
 "IM001",
 "IM002",
 "S1107",
 "S1108",
 "S1C00"
};

char *error_origins[]={
    "ISO 9075",
    "ODBC 3.0",
    "ODBC 2.0"
};

void ood_init_hgeneric(void *hH, SQLINTEGER htype)
{
    hgeneric* hGH=(hgeneric*)hH;

    ERROR_LIST(hGH)=NULL;
    DIAG_FIELDS(hGH).diag_number=0;
    HANDLE_TYPE(hGH)=htype;
}


SQLRETURN _SQLGetDiagField(
    SQLSMALLINT            HandleType,
    SQLHANDLE            Handle,
    SQLSMALLINT            RecNumber,
    SQLSMALLINT            DiagIdentifier,
    SQLPOINTER            DiagInfoPtr,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *StringLengthPtr )
{
    hgeneric *hGH=(hgeneric*)Handle;
    error_node *ptr;
	hDbc_T* dbc;
    SQLRETURN status=SQL_NO_DATA;

	switch(HandleType)
	{
		case SQL_HANDLE_DBC:
			dbc=(hDbc_T*)Handle;
			break;

		case SQL_HANDLE_STMT:
			dbc=((hStmt_T*)Handle)->dbc;
			break;

		case SQL_HANDLE_DESC:
			dbc=((hDesc_T*)Handle)->stmt->dbc;
			break;

		default:
			dbc=NULL;
	}

    THREAD_MUTEX_LOCK(hGH);

    switch(DiagIdentifier)
    {
        case SQL_DIAG_CURSOR_ROW_COUNT:
        break;

        case SQL_DIAG_DYNAMIC_FUNCTION:
        break;

        case SQL_DIAG_DYNAMIC_FUNCTION_CODE:
        break;

        case SQL_DIAG_NUMBER:
        DiagInfoPtr=(SQLPOINTER)hGH->diag_fields.diag_number;
        status=SQL_SUCCESS;
        break;

        case SQL_DIAG_RETURNCODE:
        break;

        case SQL_DIAG_ROW_COUNT:
		if (HandleType == SQL_HANDLE_STMT)
        {
           *(int *)DiagInfoPtr = ((hStmt_T *)Handle)->num_result_rows;
           status = SQL_SUCCESS;
        }

        break;

        default:
        {
            for(ptr=hGH->error_list;ptr;ptr=ptr->next)
            {
                if(!--RecNumber)
                {
                    break;
                }
            }
            if(!ptr)
            {
                THREAD_MUTEX_UNLOCK(hGH);
                return SQL_NO_DATA;
            }
            /* find record */
            switch(DiagIdentifier)
            {
                case SQL_DIAG_CLASS_ORIGIN:
                    if(!ood_bounded_strcpy((char*)DiagInfoPtr,
								(char*)ptr->origin,BufferLength))
                    {
                        ood_post_diag(hGH,ERROR_ORIGIN_01004,-1,"",
                                ERROR_MESSAGE_01004,
                        __LINE__,-1,"",ERROR_STATE_01004,
                        __FILE__,__LINE__);
                        status=SQL_SUCCESS_WITH_INFO;
                    }
                    *StringLengthPtr=strlen(DiagInfoPtr);
                    status=SQL_SUCCESS;
                break;
        
                case SQL_DIAG_COLUMN_NUMBER:
                    DiagInfoPtr=(SQLPOINTER)ptr->column_number;
                    status=SQL_SUCCESS;
                break;
        
                case SQL_DIAG_CONNECTION_NAME:
                    if(!ood_bounded_strcpy(DiagInfoPtr,"Easysoft Oracle",
                                BufferLength))
                    {
                        ood_post_diag(hGH,ERROR_ORIGIN_01004,-1,"",
                                ERROR_MESSAGE_01004,
                        __LINE__,-1,"",ERROR_STATE_01004,
                        __FILE__,__LINE__);
                        status=SQL_SUCCESS_WITH_INFO;
                    }
                    *StringLengthPtr=strlen(DiagInfoPtr);
                    status=SQL_SUCCESS;
                break;
    
                case SQL_DIAG_MESSAGE_TEXT:
                    if(!ood_bounded_strcpy((char*)DiagInfoPtr,
								(char*)ptr->message,BufferLength))
                    {
                        ood_post_diag(hGH,ERROR_ORIGIN_01004,-1,"",
                                ERROR_MESSAGE_01004,
                        __LINE__,-1,"",ERROR_STATE_01004,
                        __FILE__,__LINE__);
                        status=SQL_SUCCESS_WITH_INFO;
                    }
                    *StringLengthPtr=strlen(DiagInfoPtr);
                    status=SQL_SUCCESS;
                break;
        
                case SQL_DIAG_NATIVE:
                    DiagInfoPtr=(SQLPOINTER)ptr->native;
                    status=SQL_SUCCESS;
                break;
    
                case SQL_DIAG_ROW_NUMBER:
                    DiagInfoPtr=(SQLPOINTER)ptr->row_number;
                    status=SQL_SUCCESS;
                break;

                case SQL_DIAG_SERVER_NAME:
				    if(dbc)
					{
                        if(!ood_bounded_strcpy(DiagInfoPtr,dbc->DSN,
                                    BufferLength))
                        {
                            ood_post_diag(hGH,ERROR_ORIGIN_01004,-1,"",
                                    ERROR_MESSAGE_01004,
                            __LINE__,-1,"",ERROR_STATE_01004,
                            __FILE__,__LINE__);
                            status=SQL_SUCCESS_WITH_INFO;
                        }
                        *StringLengthPtr=strlen(DiagInfoPtr);
                        status=SQL_SUCCESS;
					}
					else
					{
                        *StringLengthPtr=0;
					}
                break;

                case SQL_DIAG_SQLSTATE:
                    if(!ood_bounded_strcpy((char*)DiagInfoPtr,
								(char*)ptr->sqlstate,BufferLength))
                    {
                        ood_post_diag(hGH,ERROR_ORIGIN_01004,-1,"",
                                ERROR_MESSAGE_01004,
                        __LINE__,-1,"",ERROR_STATE_01004,
                        __FILE__,__LINE__);
                        status=SQL_SUCCESS_WITH_INFO;
                    }
                    *StringLengthPtr=strlen(DiagInfoPtr);
                    status=SQL_SUCCESS;
                break;

                case SQL_DIAG_SUBCLASS_ORIGIN:
                    if(!ood_bounded_strcpy((char*)DiagInfoPtr,
								"ODBC 3.0",BufferLength))
                    {
                        ood_post_diag(hGH,ERROR_ORIGIN_01004,-1,"",
                                ERROR_MESSAGE_01004,
                        __LINE__,-1,"",ERROR_STATE_01004,
                        __FILE__,__LINE__);
                        status=SQL_SUCCESS_WITH_INFO;
                    }
                    *StringLengthPtr=strlen(DiagInfoPtr);
                    status=SQL_SUCCESS;
                break;

                default:
                break;
            }
        }
    }
    THREAD_MUTEX_UNLOCK(hGH);
    return status;
}

SQLRETURN _SQLGetDiagRec(
    SQLSMALLINT            HandleType,
    SQLHANDLE            Handle,
    SQLSMALLINT            RecNumber,
    SQLCHAR                *SqlState,
    SQLINTEGER            *NativeErrorPtr,
    SQLCHAR                *MessageText,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *TextLengthPtr )
{
    hgeneric *hGH=(hgeneric*)Handle;
    error_node *ptr;


    if(!hGH)
        return SQL_INVALID_HANDLE;

    if(!hGH->error_list)
	{
		if(SqlState)
			*SqlState='\0';
		if(NativeErrorPtr)
		    *NativeErrorPtr=-1;
		if(MessageText)
			*MessageText='\0';
		if(TextLengthPtr)
			*TextLengthPtr=-1;
        return SQL_NO_DATA;
	}
    

    if(RecNumber<1)
    {
        RecNumber*=-1;
/*        return SQL_ERROR;*/
    }

    THREAD_MUTEX_LOCK(hGH);
    for(ptr=hGH->error_list;ptr;ptr=ptr->next)
    {
        if(!--RecNumber)
        {
            break;
        }
    }
    if(!ptr)
    {
        THREAD_MUTEX_UNLOCK(hGH);
        return SQL_NO_DATA;
    }
    
    if(ptr->sqlstate)
        strcpy((char*)SqlState,(const char*)ptr->sqlstate);
    if(NativeErrorPtr)
        *NativeErrorPtr=ptr->native;

    BufferLength-=strlen(ERROR_VENDOR);
    if(BufferLength>0)
        strcpy((char*)MessageText,ERROR_VENDOR);
    BufferLength-=strlen(ERROR_PRODUCT);

    if(BufferLength>0)
        strcat((char*)MessageText,ERROR_PRODUCT);

    BufferLength-=ptr->message?strlen((const char*)ptr->message):0;

    if(BufferLength>0)
        strcat((char*)MessageText,(const char*)ptr->message);

	if(TextLengthPtr)
        *TextLengthPtr=ptr->message?strlen((const char*)ptr->message):0;

    THREAD_MUTEX_UNLOCK(hGH);
    return SQL_SUCCESS;
}


/*
 * ood_post_diag puts another diag rec on the SQLHANDLE
 *
 * The handle is assumed to be locked.
 */
void ood_post_diag(struct hgeneric_TAG *hH,
        char* origin,
        SQLINTEGER column_number,
        char *connection_name,
        char *message,
        SQLINTEGER native,
        SQLINTEGER row_number,
        char *server_name,
        char *sqlstate,
        char *file,
        int line)
{
    error_node *new;
    error_node *ptr;

    if(!hH)
        return;

if(ENABLE_TRACE){
	switch(HANDLE_TYPE(hH))
	{
		case SQL_HANDLE_DBC:
	        ood_log_message((hDbc_T*)hH,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
			    (SQLHANDLE)NULL,0,"sis","FILE",file,"LINE",line,
				"message",message);
            break;
        case SQL_HANDLE_STMT:
	        ood_log_message(((hStmt_T*)hH)->dbc,__FILE__,__LINE__,
				TRACE_FUNCTION_EXIT,
			    (SQLHANDLE)NULL,0,"sis","FILE",file,"LINE",line,
				"message",message);
            
            break;
        case SQL_HANDLE_DESC:
	        ood_log_message(((hDesc_T*)hH)->dbc,__FILE__,__LINE__,
				TRACE_FUNCTION_EXIT,
			    (SQLHANDLE)NULL,0,"sis","FILE",file,"LINE",line,
				"message",message);
            break;
	}
}

    /* changed following call from malloc to calloc. otherwise, was
	   ending up pulling off an un-initialized error node! */
    new=(error_node*)calloc(1,sizeof(error_node));
    if(!new)
        return;

    new->next=NULL;
    if(!ERROR_LIST(hH)) /* first */
    {
        
        ERROR_LIST(hH)=new;
        ptr=ERROR_LIST(hH);
    }
    else
    {
        for(ptr=ERROR_LIST(hH);ptr->next;ptr=ptr->next);
        ptr->next=new;
    }
    ptr->origin=(unsigned char*)origin;
    ptr->column_number=column_number;
    ptr->connection_name=(unsigned char*)connection_name;
    strcpy((char*)ptr->message,(const char*)message);
    ptr->native=native;
    ptr->row_number=row_number;
    ptr->server_name=(unsigned char*)server_name;
    ptr->sqlstate=(unsigned char*)sqlstate;
    DIAG_FIELDS(hH).diag_number++;
}

/*
 * Free Errors ORAFREEs the error list
 *
 * the handle is assumed to be locked.
 */

void ood_free_diag(struct hgeneric_TAG *hH)
{
    error_node *this,*next;
    for(this=ERROR_LIST(hH);this;this=next)
    {
        next=this->next;
        ORAFREE(this);
    }
}

/*
 * Clear diag
 *
 * like free_errors but locks the handle
 */
void ood_clear_diag(struct hgeneric_TAG *hH)
{
    error_node *this,*next;
    THREAD_MUTEX_LOCK(hH);
    for(this=ERROR_LIST(hH);this;this=next)
    {
        next=this->next;
        ORAFREE(this);
    }
    ERROR_LIST(hH)=NULL;
    DIAG_FIELDS(hH).diag_number=0;
    THREAD_MUTEX_UNLOCK(hH);
}
