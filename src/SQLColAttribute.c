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
 * $Id: SQLColAttribute.c,v 1.10 2004/11/23 23:03:22 dbox Exp $
 *

 *
 ******************************************************************************/

#include "common.h"
#include <sqlext.h>

static char const rcsid[]= "$RCSfile: SQLColAttribute.c,v $";


const char * _sql_desc_type_name( struct ar_TAG * ar)
{ 
 
  if(ar->scale==-127){
    return "FLOAT";
  }

  if(ar->concise_type>=SQL_NUMERIC && ar->concise_type<=SQL_DOUBLE  ){
    if(ar->scale==0)
      return "INTEGER";
    return "NUMBER";
  }

  if(ar->concise_type==SQL_VARCHAR)
    return "VARCHAR";

  if(ar->concise_type==SQL_CHAR)
    return "CHAR";

  if(ar->concise_type==SQL_TYPE_TIMESTAMP )
    return "DATE";

  if(ar->concise_type==SQL_LONGVARBINARY  )
    return "BLOB";

  if(ar->concise_type==SQL_LONGVARCHAR  )
    return "CLOB";



  return "UNKNOWN TYPE";
}



SQLRETURN SQL_API SQLColAttribute(
    SQLHSTMT            StatementHandle,
    SQLUSMALLINT        ColumnNumber,
    SQLUSMALLINT        FieldIdentifier,
    SQLPOINTER            CharacterAttributePtr,
    SQLSMALLINT            BufferLength,
    SQLSMALLINT            *StringLengthPtr,
    SQLPOINTER            NumericAttributePtr )
{
    hStmt_T *stmt=(hStmt_T*)StatementHandle;
    struct ar_TAG *ar;
    struct ir_TAG *ir;
    SQLRETURN status=SQL_SUCCESS;
    assert(IS_VALID(stmt));

    if(!stmt||HANDLE_TYPE(stmt)!=SQL_HANDLE_STMT)
        return SQL_INVALID_HANDLE;

if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"is",
			"ColumnNumber",ColumnNumber,
			"FieldIdentifier",odbc_desc_type(FieldIdentifier));
}
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

    ar=&stmt->current_ar->recs.ar[ColumnNumber];
    /*assert(IS_VALID(ar));*/
    ir=&stmt->current_ir->recs.ir[ColumnNumber];
    /*assert(IS_VALID(ir));*/
   switch(FieldIdentifier)
    {
        case SQL_DESC_AUTO_UNIQUE_VALUE:
            *((SQLINTEGER*)NumericAttributePtr)=ar->auto_unique;
//#define UNIX_DEBUG
#ifdef UNIX_DEBUG
fprintf(stderr,"SQL_DESC_AUTO_UNIQUE_VALUE=%d %s %d\n",*((SQLINTEGER*)NumericAttributePtr),__FILE__,__LINE__);
#endif
 break;
    
    case SQL_DESC_LABEL:
    case SQL_DESC_BASE_COLUMN_NAME:
    case SQL_DESC_NAME:
      //case SQL_COLUMN_LABEL: //this is same as SQL_DESC_LABEL
    case SQL_COLUMN_NAME:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_COLUMN_NAME %s %d\n",__FILE__,__LINE__);
#endif
      if(!ood_bounded_strcpy(CharacterAttributePtr,
			     (char*)ar->column_name,BufferLength))
	{
	  ood_post_diag((hgeneric*)stmt,ERROR_ORIGIN_01004,ColumnNumber,"",
                        ERROR_MESSAGE_01004,
			__LINE__,0,"",ERROR_STATE_01004,
			__FILE__,__LINE__);
	  status=SQL_SUCCESS_WITH_INFO;
	  if(StringLengthPtr)
	    *StringLengthPtr=BufferLength;
	}
      else
	if(StringLengthPtr)
	  *StringLengthPtr=strlen((const char*)ar->column_name);
#ifdef UNIX_DEBUG
      fprintf(stderr,"%s %d %d name [%s]\n",
	      __FILE__,__LINE__,ColumnNumber,
	      ((SQLCHAR*)CharacterAttributePtr));
#endif
      break;

    case SQL_DESC_TABLE_NAME:     
    case SQL_DESC_BASE_TABLE_NAME:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_BASE_TABLE_NAME %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLCHAR*)CharacterAttributePtr)='\0';
      if(StringLengthPtr)
	*StringLengthPtr=0;
      break;
      
    case SQL_DESC_CASE_SENSITIVE:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_CASE_SENSITIVE %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->case_sensitive;
      break;
      
    case SQL_DESC_CATALOG_NAME:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_CATALOG_NAME %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLCHAR*)CharacterAttributePtr)='\0';
      if(StringLengthPtr)
	*StringLengthPtr=0;
      break;
      
    case SQL_DESC_CONCISE_TYPE:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_CONCISE_TYPE %s %d\n",__FILE__,__LINE__);
#endif
      /*
      *((SQLINTEGER*)NumericAttributePtr)=ood_ocitype_to_sqltype_imp(stmt,ColumnNumber);
      */

      *((SQLINTEGER*)NumericAttributePtr)=ar->data_type;
      break;
      
    case SQL_DESC_COUNT:
    case SQL_COLUMN_COUNT:
      *((SQLINTEGER*)NumericAttributePtr)=
	(SQLINTEGER)stmt->current_ir->num_recs;
#ifdef UNIX_DEBUG
      fprintf(stderr,"%s %d %d SQL_DESC_COUNT %d\n",
	      __FILE__,__LINE__,ColumnNumber,
	      *((SQLINTEGER*)NumericAttributePtr));
#endif
      break;
      
    case SQL_DESC_DISPLAY_SIZE:
      *((SQLINTEGER*)NumericAttributePtr)=ar->display_size;
#ifdef UNIX_DEBUG
      fprintf(stderr,"%s %d %d display size %d\n",
	      __FILE__,__LINE__,ColumnNumber,
	      *((SQLINTEGER*)NumericAttributePtr));
#endif
      break;
      
    case SQL_DESC_OCTET_LENGTH:
    case SQL_DESC_LENGTH:
    case SQL_COLUMN_LENGTH:
      *((SQLINTEGER*)NumericAttributePtr)=ar->octet_length;
#ifdef UNIX_DEBUG
      fprintf(stderr,"%s %d %d octet length %d\n",
	      __FILE__,__LINE__,ColumnNumber,
	      *((SQLINTEGER*)NumericAttributePtr));
#endif
      break;
      
    case SQL_DESC_FIXED_PREC_SCALE:
      *((SQLINTEGER*)NumericAttributePtr)=ar->fixed_prec_scale;
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_FIXED_PREC_SCALE=%d %s %d\n",*((SQLINTEGER*)NumericAttributePtr),__FILE__,__LINE__);
#endif
      break;
      
    case SQL_DESC_LITERAL_PREFIX:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_LITERAL_PREFIX %s %d\n",__FILE__,__LINE__);
#endif
      ood_bounded_strcpy(CharacterAttributePtr,
			 (char*)ar->literal_prefix,BufferLength);
      if(StringLengthPtr)
	*StringLengthPtr=strlen((const char*)ar->literal_prefix);
      break;
      
    case SQL_DESC_LITERAL_SUFFIX:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_LITERAL_SUFFIX %s %d\n",__FILE__,__LINE__);
#endif
      ood_bounded_strcpy(CharacterAttributePtr,
			 (char*)ar->literal_suffix,BufferLength);
      if(StringLengthPtr)
	*StringLengthPtr=strlen((const char*)ar->literal_suffix);
      break;
      
    case SQL_DESC_LOCAL_TYPE_NAME:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_LOCAL_TYPE_NAME %s %d\n",__FILE__,__LINE__);
#endif
      if(ar->local_type_name)
	{
	  ood_bounded_strcpy(CharacterAttributePtr,
			     (char*)ar->local_type_name,BufferLength);
	  if(StringLengthPtr)
	    *StringLengthPtr=strlen((const char*)ar->local_type_name);
	}
      else
	{
	  strcpy(CharacterAttributePtr,"local_type_name");
	  if(StringLengthPtr)
	    *StringLengthPtr=0;
	}
      break;
      
    case SQL_DESC_NULLABLE:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_NULLABLE %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->nullable;
      break;
      
    case SQL_DESC_NUM_PREC_RADIX:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_NUM_PREC_RADIX %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->num_prec_radix;
      break;
      
    case SQL_DESC_PRECISION:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_PRECISION %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->precision;

#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_PRECISION = %d\n",
	      *((SQLINTEGER*)NumericAttributePtr));
#endif

      break;


    case SQL_DESC_SCALE:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_SCALE %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->scale;

#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_SCALE = %d\n",
	      *((SQLINTEGER*)NumericAttributePtr));
#endif

      break;
      
    case SQL_DESC_SCHEMA_NAME:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_SCHEMA_NAME %s %d\n",__FILE__,__LINE__);
#endif
      ood_bounded_strcpy(CharacterAttributePtr,
			 NULL,BufferLength);
      if(StringLengthPtr)
	*StringLengthPtr=0;
      break;
      
    case SQL_DESC_SEARCHABLE:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_SEARCHABLE %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->searchable;
      break;
      
    case SQL_DESC_TYPE:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_TYPE %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->data_type;
      break;
      
    case SQL_DESC_TYPE_NAME:


      if(getenv("DEBUG") && atoi(getenv("DEBUG"))>3){
	fprintf(stderr,"SQL_DESC_TYPE_NAME %s %d\n",__FILE__,__LINE__);
	/*dump_ar_T(ar);
	dump_ir_T(ir); */
      }
      if(!ar->type_name){
	const char *nm = _sql_desc_type_name(ar);
	int len = strlen(nm);
	char *type_name = (char*) ORAMALLOC(len);
	assert(len <= 31); /*max identifier length in oracle*/
	ood_bounded_strcpy(type_name, nm,31);
	ar->type_name=type_name;
	
	ood_bounded_strcpy(CharacterAttributePtr,
			   (char*)ar->type_name,BufferLength);
	if(StringLengthPtr)
	  *StringLengthPtr=strlen((const char*)ar->type_name);
      }

#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_TYPE_NAME %s %d\n",__FILE__,__LINE__);
#endif
      if(ar->type_name)
	{
	  ood_bounded_strcpy(CharacterAttributePtr,
			     (char*)ar->type_name,BufferLength);
	  if(StringLengthPtr)
	    *StringLengthPtr=strlen((const char*)ar->type_name);
	}
      else
	{
	  strcpy(CharacterAttributePtr,"type_name");
	  if(StringLengthPtr)
	    *StringLengthPtr=0;
	}

      break;
      
    case SQL_DESC_UNNAMED:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_UNNAMED %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=SQL_NAMED;
      break;
      
    case SQL_DESC_UNSIGNED:
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_UNSIGNED %s %d\n",__FILE__,__LINE__);
#endif
      *((SQLINTEGER*)NumericAttributePtr)=ar->un_signed;
      break;
      
    case SQL_DESC_UPDATABLE:
      *((SQLINTEGER*)NumericAttributePtr)=ar->updateable;
#ifdef UNIX_DEBUG
      fprintf(stderr,"SQL_DESC_UPDATABLE=%d %s %d\n",*((SQLINTEGER*)NumericAttributePtr),__FILE__,__LINE__);
#endif
      break;
      
    default:
      status=SQL_ERROR;
    }
    ood_mutex_unlock_stmt(stmt);
#if defined(UNIX_DEBUG) && defined (FRED)
    for(ColumnNumber=1;ColumnNumber<=stmt->current_ir->num_recs;ColumnNumber++)
    {
        fprintf(stderr,"%s %d col num [%d] fn[%.8lx]\n",
                __FILE__,__LINE__,
                stmt->current_ir->recs.ir[ColumnNumber].col_num,
                (long)stmt->current_ir->recs.ir[ColumnNumber].default_copy);
    }
#endif

if(ENABLE_TRACE){
	if(CharacterAttributePtr)
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"sis",
			"FieldIdentifier",odbc_desc_type(FieldIdentifier),
			"ColumnNnumber",ColumnNumber,
			"CharacterAttributePtr",CharacterAttributePtr);
	else
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"sii",
			"FieldIdentifier",odbc_desc_type(FieldIdentifier),
			"ColumnNnumber",ColumnNumber,
			"*NumericAttributePtr",*(SQLINTEGER*)NumericAttributePtr);
//#undef  UNIX_DEBUG
}
    return status;
}
