#include "common.h"
/* $Id: common.c,v 1.9 2005/03/17 01:45:41 dbox Exp $*/

static  int g_Debug_Oracle_ODBC;


SQLRETURN not_implemented()
{
  fprintf(stderr,
	  "you have followed a pointer to an unimplemented function!\n");
  return SQL_ERROR;
}


SQLRETURN init_hgeneric(hgeneric *t)
{
  t->error_list = (void*)NULL;
  t->htype = int_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;
  return SQL_SUCCESS;
}

void ood_ar_init (ar_T *t)
{
  t->auto_unique = SQLSMALLINT_DEFAULT;
  t->base_column_name = SQLCHAR_DEFAULT;
  t->base_table_name = SQLCHAR_DEFAULT;
  t->case_sensitive = SQL_FALSE;
  t->catalog_name = SQLCHAR_DEFAULT;
  t->concise_type = SQL_C_DEFAULT;
  t->data_ptr = SQLPOINTER_DEFAULT;
  t->display_size = SQLINTEGER_DEFAULT;
  t->fixed_prec_scale = SQLSMALLINT_DEFAULT;
  t->bind_indicator = SQLINTEGER_DEFAULT;
  t->bind_indicator_malloced = 0;
  t->length = SQLINTEGER_DEFAULT;
  t->literal_prefix = SQLCHAR_DEFAULT;
  t->literal_suffix = SQLCHAR_DEFAULT;
  t->local_type_name = SQLCHAR_DEFAULT;
  t->column_name[0] = SQLCHAR_DEFAULT;
  t->nullable = SQL_TRUE;
  t->num_prec_radix = 10;
  t->octet_length = SQLINTEGER_DEFAULT;
  t->precision = SQLSMALLINT_DEFAULT;
  t->scale = SQLSMALLINT_DEFAULT;
  t->schema_name = SQLCHAR_DEFAULT;
  t->searchable = SQL_TRUE;
  t->table_name = SQLCHAR_DEFAULT;
  t->data_type = SQLSMALLINT_DEFAULT;
  t->type_name = SQLCHAR_DEFAULT;
  t->un_signed = SQL_TRUE;
  t->updateable = SQL_TRUE;
  t->buffer_length = SQLINTEGER_DEFAULT;
  t->bind_target_type = SQLSMALLINT_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;
}

void dump_ar_T(ar_T * t)

{

  printf("this is function dump_ar_T(%x)\n",t);
  printf("auto_unique=%d ",t->auto_unique);
  printf("base_column_name='%s'\n",t->base_column_name);
  printf("base_table_name='%s' ",t->base_table_name);
  printf("case_sensitive=%d\n",t->case_sensitive);
  printf("catalog_name='%s' ",t->catalog_name);
  printf("concise_type=%d\n",t->concise_type);
  printf("data_ptr=%x ",t->data_ptr);
  printf("display_size=%d\n",t->display_size);
  printf("fixed_prec_scale=%d ",t->fixed_prec_scale);
  printf("bind_indicator=%d\n",t->bind_indicator);
  printf("length=%d\n",t->length);
  printf("literal_prefix='%s' ",t->literal_prefix);
  printf("literal_suffix='%s'\n",t->literal_suffix);
  printf("local_type_name=%d ",t->local_type_name);
  printf("column_name='%s'\n",t->column_name);
  printf("nullable=%d ",t->nullable);
  printf("num_prec_radix=%d\n",t->num_prec_radix);
  printf("octet_length=%d ",t->octet_length);
  printf("precision=%d\n",t->precision);
  printf("scale=%d ",t->scale);
  printf("schema_name='%s'\n",t->schema_name);
  printf("searchable=%d ",t->searchable);
  printf("table_name='%s'\n",t->table_name);
  printf("data_type=%d ",t->data_type);
  printf("type_name='%s'\n",t->type_name);
  printf("un_signed=%d ",t->un_signed);
  printf("updateable=%d\n",t->updateable);
  printf("buffer_length=%d ",t->buffer_length);
  printf("bind_target_type=%d\n",t->bind_target_type);
  printf("valid_flag=%d\n",t->valid_flag);
}

void ood_ir_init (ir_T *t, ub4 col_num, hDesc_T *desc)
{
  t->data_type = ub2_DEFAULT;
  t->orig_type = ub2_DEFAULT;
  t->data_size = ub4_DEFAULT;
  t->col_num = col_num;
  t->default_copy = FUNCTION_DEFAULT;
  t->to_string =  FUNCTION_DEFAULT;
  t->to_oracle = FUNCTION_DEFAULT;
  t->desc = desc;
  t->data_ptr =  struct_DEFAULT;
  t->ind_arr = sb2_DEFAULT;
  t->length_arr = ub2_DEFAULT;
  t->rcode_arr = ub2_DEFAULT;
  t->locator = (void*)NULL ;
  t->posn = ub4_DEFAULT;
  t->lobsiz = ub4_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;
}

void ood_ir_free_contents (ir_T *t)
{
  if (t->data_ptr)
    {
      ORAFREE (t->data_ptr);
    }
  if (t->ind_arr)
    ORAFREE (t->ind_arr);
  if (t->length_arr)
    ORAFREE (t->length_arr);
  if (t->rcode_arr)
    ORAFREE (t->rcode_arr);
  if (t->locator)
    {
      SQLUINTEGER i;

      for (i = 0; i < t->desc->stmt->row_array_size; i++)
	{
	  OCIDescriptorFree_log (t->locator[i], OCI_DTYPE_LOB);
	}
      ORAFREE (t->locator);
    }
}

void dump_ir_T(ir_T * t)

{

  printf("this is function dump_ir_T(%x)\n",t);
  printf("t->data_type =%d ",t->data_type );
  printf("t->orig_type =%d\n",t->orig_type );
  printf("t->data_size =%d ",t->data_size );
  printf("t->col_num = %d\n",t->col_num );
  printf("t->default_copy =%d ",t->default_copy  );
  printf("t->to_string = %x\n", t->to_string  );
  printf("t->to_oracle = %x ",t->to_oracle );
  printf("t->desc = %d\n",t->desc );
  printf("t->data_ptr =  %x ",t->data_ptr );
  printf("t->ind_arr = %d\n",t->ind_arr );
  printf("t->length_arr = %d ",t->length_arr );
  printf("t->rcode_arr =%d\n", t->rcode_arr );
  printf("t->locator = %x ",t->locator );
  printf("t->posn = %d\n",t->posn );
  printf("t->lobsiz = %d ",t->lobsiz );
  printf("t->valid_flag =%d\n ", t->valid_flag );

}


hEnv_T * make_hEnv_T()
{

  hEnv_T * t = ORAMALLOC(sizeof(hEnv_T));
  init_hgeneric(&(t->base_handle));
  t->parent = (SQLHANDLE*)NULL /*SQLHANDLE_DEFAULT*/ ;
  t->odbc_ver = SQLINTEGER_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;
  return t;
}

hDbc_T * make_hDbc_T()
{
  hDbc_T * t = ORAMALLOC(sizeof(hDbc_T));
  init_hgeneric(&(t->base_handle));
  t->UID[0] = SQLCHAR_DEFAULT;
  t->PWD[0] = SQLCHAR_DEFAULT;
  t->DB[0] = SQLCHAR_DEFAULT;
  t->DSN[0] = SQLCHAR_DEFAULT;
  t->oci_env = (OCIEnv*) NULL;
  t->oci_err = (OCIError*)NULL;
  t->oci_srv = (OCIServer*)NULL;
  t->oci_svc = (OCISvcCtx*)NULL;
  t->oci_ses = (OCISession*)NULL;
  t->metadata_id = (SQLUINTEGER)0;
  t->trace = (SQLUINTEGER)0;
  t->tracefile[0] = SQLCHAR_DEFAULT;
  t->autocommit = ub4_DEFAULT;
  t->stmt_list = struct_DEFAULT;
  t->desc_list = struct_DEFAULT;
  t->env = struct_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;

  return t;
}


hDesc_T * make_hDesc_T()
{
  hDesc_T * t = ORAMALLOC(sizeof(hDesc_T));

  init_hgeneric(&(t->base_handle));
  t->num_recs = ub4_DEFAULT;
  t->type = int_DEFAULT;
  t->bound_col_flag = int_DEFAULT;
  t->lob_col_flag = int_DEFAULT;
  t->recs.ap = struct_DEFAULT;
  t->recs.ip = struct_DEFAULT;
  t->recs.ar = struct_DEFAULT;
  t->recs.ir = struct_DEFAULT;
  t->next = struct_DEFAULT;
  t->prev = struct_DEFAULT;
  t->stmt = struct_DEFAULT;
  t->dbc = struct_DEFAULT;
  t->valid_flag = VALID_FLAG_DEFAULT;

  return t;

}


hStmt_T * make_hStmt_T()
{
  hStmt_T * t = ORAMALLOC(sizeof(hStmt_T));
  init_hgeneric(&(t->base_handle));
  t->implicit_ap = struct_DEFAULT;
  t->implicit_ip = struct_DEFAULT;
  t->implicit_ar = struct_DEFAULT;
  t->implicit_ir = struct_DEFAULT;
  t->current_ap = struct_DEFAULT;
  t->current_ip = struct_DEFAULT;
  t->current_ar = struct_DEFAULT;
  t->current_ir = struct_DEFAULT;
  t->oci_stmt = (OCIStmt*)NULL;
  t->next = struct_DEFAULT;
  t->prev = struct_DEFAULT;
  t->stmt_type = ub2_DEFAULT;
  t->num_result_rows = int_DEFAULT;
  t->num_fetched_rows = int_DEFAULT;
  t->bookmark = int_DEFAULT;
  t->current_row = int_DEFAULT;
  t->alt_fetch_data = (void*)NULL;
  t->fetch_status = int_DEFAULT;
  t->sql = SQLCHAR_DEFAULT;
  t->dbc = struct_DEFAULT;
  t->row_bind_offset_ptr = SQLPOINTER_DEFAULT;
  t->param_bind_offset_ptr = SQLPOINTER_DEFAULT;
  t->row_array_size = (SQLUINTEGER)NULL;
  t->row_status_ptr = (SQLUSMALLINT)NULL;
  t->row_bind_type = (SQLUINTEGER)NULL;
  t->rows_fetched_ptr = (SQLUINTEGER)NULL;
  t->param_bind_type = (SQLUINTEGER)NULL;
  t->paramset_size = 1;
  t->query_timeout = (SQLUINTEGER)NULL;
  t->valid_flag = VALID_FLAG_DEFAULT;
  return t;
}



int debugLevel()
{
  return g_Debug_Oracle_ODBC;
}

int debugLevel2()
{
  return (g_Debug_Oracle_ODBC>1);
}

int debugLevel3()
{
  return (g_Debug_Oracle_ODBC>2);
}

void setDebugLevel(int i)
{
  g_Debug_Oracle_ODBC = i;
}
