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
 * $Id: SQLFetch.c,v 1.5 2004/11/17 03:02:59 dbox Exp $
 *
 * $Log: SQLFetch.c,v $
 * Revision 1.5  2004/11/17 03:02:59  dbox
 * changed some bind/fetch internals, better ood_log behavior
 *
 * Revision 1.4  2003/01/27 21:06:50  dbox
 * WTF is COPYING doing in here, its already EPL it cant be GPL too!
 *
 * Revision 1.3  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.2  2002/06/19 22:21:37  dbox
 * more tweaks to OCI calls to report what happens when DEBUG level is set
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.18  2000/07/21 10:07:23  tom
 * Re-organsised for SQLExtendedFetch/SQLFetchScroll and LOBS added
 *
 * Revision 1.17  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.16  2000/07/07 08:02:51  tom
 * numerous changes for block cursors
 *
 * Revision 1.15  2000/06/05 16:00:48  tom
 * Alterations due to initial implementation of bound params
 *
 * Revision 1.14  2000/05/23 16:04:00  tom
 * Added more debugging code
 *
 * Revision 1.13  2000/05/17 15:18:50  tom
 * Tracing improved
 *
 * Revision 1.12  2000/05/16 07:54:09  tom
 * Library version fixes
 *
 * Revision 1.10  2000/05/11 13:27:45  tom
 * Bugfixes and tweaks
 *
 * Revision 1.9  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.8  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.7  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.6  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.5  2000/04/26 15:29:57  tom
 * Now gets the data for bound columns
 *
 * Revision 1.4  2000/04/26 10:15:55  tom
 * Changes for late defining of data types etc
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:25:18  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLFetch.c,v $ $Revision: 1.5 $";

SQLRETURN ood_SQLFetch( 
    hStmt_T* stmt )
{
  sword ret=OCI_SUCCESS;
  sword tmp=OCI_SUCCESS;
  int offset;
  unsigned int i,row;

  SQLRETURN status=stmt->fetch_status;
 
  

  stmt->num_fetched_rows=-1;

#ifdef UNIX_DEBUG
  signal(SIGSEGV,SIG_DFL);
#endif

  /* 
   * We have a mapping problem between ODBC and OCI here. 
   * OCIStmtExecute is equivalent to SQLExecute,SQLFetch. 
   * OCIStmtFetch returns OCI_NO_DATA when it hits the end of the result
   * set whereas SQLFetch returns SQL_NO_DATA when you try to go past
   * the end of the result set. Of of this mesna there is a lot of twiddling
   * to be done in here.
   *
   * NOTE: alt_fetch functions simply return OCI_SUCCESS if there is data
   * to be processed, OCI_NO_DATA if not.
   */
  if(stmt->alt_fetch)
    {
      ret=stmt->alt_fetch(stmt);
      if(ret==OCI_NO_DATA)
	{
	  stmt->fetch_status=SQL_NO_DATA;
	  if(stmt->rows_fetched_ptr)
	    *stmt->rows_fetched_ptr=stmt->num_fetched_rows;
	  return(SQL_NO_DATA);
	}
      status=stmt->fetch_status;
    }
  else
    {
      if(status!=SQL_NO_DATA&&stmt->bookmark) 
	{
	  /*
	   * stmt->bookmark would be nonzero if this wasn't the first call
	   * to SQLFetch. As OCIStmtExec==SQLexecute,SQLFetch we don't want
	   * to call OCIStmtFetch for the first call of SQLFetch.
	   * if status==SQL_NO_DATA we're trying to go past the end of the
	   * result set. If we were to call OCIStmtFetch now we'd get 
	   * an error.
	   */
	  ret=OCIStmtFetch_log_stat(stmt->oci_stmt,stmt->dbc->oci_err,
				    stmt->row_array_size,OCI_FETCH_NEXT,
				    OCI_DEFAULT,ret);
#ifdef UNIX_DEBUG
	  errcheck(__FILE__,__LINE__,ret,stmt->dbc->oci_err);
#endif

	  OCIAttrGet_log_stat(stmt->oci_stmt,OCI_HTYPE_STMT,
			      &stmt->num_result_rows,0,OCI_ATTR_ROW_COUNT,
			      stmt->dbc->oci_err,tmp);
	}
      else if(status==SQL_NO_DATA&&!stmt->bookmark) 
	{
	  /* 
	   * This means the OCIStmtExecute returned OCI_NO_DATA, ie the 
	   * result set<=array size. This is equivalent here to 
	   * OCIStmtFetch returning OCI_NO_DATA at the end of a larger 
	   * result set. 
	   */
	  status=SQL_SUCCESS;
	  ret=OCI_NO_DATA;
	}
    }
	
  if(ret&&ret!=OCI_NO_DATA) 
    {
      /*
       * OCIStmtFetch has returned an error.
       */
      if(ret==OCI_ERROR||ret==OCI_SUCCESS_WITH_INFO)
        {
	  ood_driver_error(stmt,ret,__FILE__,__LINE__);
	  status=SQL_ERROR;
        }
    }
  else
    {
      /*
       * Either OCIStmtFetch returned OCI_NO_DATA or OCIStmtExecute
       * returned OCI_NO_DATA or OCIStmtExecute and OCIStmtFetch (if
       * called) returned OCI_SUCCESS
       */
      if(ret==OCI_NO_DATA)
        {
	  stmt->num_fetched_rows=stmt->num_result_rows
	    %stmt->row_array_size;
	  stmt->fetch_status=SQL_NO_DATA;
	  if(!stmt->num_fetched_rows) /* landed on a boundary */
	    status=SQL_NO_DATA;
        }
      else
	{
	  if(stmt->num_fetched_rows==-1)
	    {
	      if(status==SQL_NO_DATA) /* Nothing fetched, obviously */
		stmt->num_fetched_rows=0;
	      else 
		stmt->num_fetched_rows=stmt->row_array_size;
	    }
	}

      stmt->current_row=0;
      if(stmt->row_bind_offset_ptr)
	offset=*((SQLUINTEGER*)stmt->row_bind_offset_ptr);
      else
	offset=0;

      for(row=0;row<stmt->row_array_size;row++)
	{
	  stmt->current_row++;
	  stmt->bookmark++;

#ifdef UNIX_DEBUG
	  fprintf(stderr,"Fetching row %d of num_fetched %d\n",
		  row,stmt->num_fetched_rows);
#endif

	  if(stmt->row_status_ptr&&(signed)row<stmt->num_fetched_rows)
	    {
	      stmt->row_status_ptr[row]=(SQLSMALLINT)SQL_ROW_SUCCESS;
	    }
	  else if((signed)row>=stmt->num_fetched_rows)
	    {
	      if(stmt->row_status_ptr)
		{
		  stmt->row_status_ptr[row]=SQL_NO_DATA;
		}
	      continue;
	    }
	  if(stmt->current_ir->lob_col_flag) 
	    {
	      for(i=1;i<=stmt->current_ir->num_recs;i++)
		{
		  stmt->current_ir->recs.ir[i].posn=1;
		}
	    }

	  if(stmt->current_ar->bound_col_flag) 
	    /* There are bound columns... */
            {

	      for(i=1;i<=stmt->current_ar->num_recs;i++)
                {
		  if(stmt->current_ar->recs.ar[i].data_ptr)
                    {
		      if(!stmt->current_ir->recs.ir[i].ind_arr[row])
                        {
			  /*
			  *stmt->current_ar->recs.ar[i].bind_indicator=
			    stmt->current_ir->recs.ir[i].data_size;
			  */
			  if(ENABLE_TRACE){
                            ood_log_message(stmt->dbc,__FILE__,__LINE__,
					    TRACE_FUNCTION_EXIT,
					    (SQLHANDLE)NULL,status,"iihi",
					    "BoundColNo",i,
					    "BufferLength",
					    stmt->current_ar->recs.ar[i].buffer_length,
					    "Data Pointer",
					    (long)stmt->current_ar->recs.ar[i].data_ptr,
					    "Bind Indicator",
					     *stmt->current_ar->recs.ar[i].bind_indicator);
			  }
			  /*
			   * And finally call the conversion function!
			   *
			   * ...yes, we're going for the "most stupidly 
			   * complicated function call in history" award
			   */
                          status|=stmt->current_ir->recs.ir[i].default_copy(
					         row,
						 &stmt->current_ir->recs.ir[i],
						 ((unsigned char*)
						  stmt->current_ar->recs.ar[i].data_ptr)
						 +(row
						   *stmt->current_ar->recs.ar[i].buffer_length)
						 +offset,
						 stmt->current_ar->recs.ar[i].buffer_length
						 +offset,
						 stmt->current_ar->recs.ar[i].bind_indicator
						 +offset
						 );
                        }
		      else /* if null */
                        {
			  
			  if(ENABLE_TRACE){
			    ood_log_message(stmt->dbc,__FILE__,__LINE__,
					    TRACE_FUNCTION_EXIT,
					    (SQLHANDLE)NULL,status,"iihs",
					    "BoundColNo",i,
					    "BufferLength",
					    stmt->current_ar->recs.ar[i].buffer_length,
					    "Data Pointer",
					    (long)stmt->current_ar->recs.ar[i].data_ptr,
					    "Null Field Status","Column is NULL");
			  }
			  *stmt->current_ar->recs.ar[i].bind_indicator
			    =SQL_NULL_DATA;
                        }
                    } /* if ar->data_ptr */
		} /* for num_recs */
	    } /* if bound columns */
        }/* for rows */
    } /* if results */

  if(stmt->rows_fetched_ptr)
    *stmt->rows_fetched_ptr=stmt->num_fetched_rows;

  return status;
}

SQLRETURN SQL_API SQLFetch( 
    SQLHSTMT StatementHandle )
{
  hStmt_T* stmt=(hStmt_T*)StatementHandle;
  SQLRETURN status;
  if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
		    (SQLHANDLE)stmt,0,"ii",
		    "Status",stmt->fetch_status,
		    "num_result_rows",stmt->num_result_rows);
  }
  ood_clear_diag((hgeneric*)stmt);
  ood_mutex_lock_stmt(stmt);
	
  status=ood_SQLFetch(stmt);
	
  ood_mutex_unlock_stmt(stmt);

  if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
		    (SQLHANDLE)NULL,status,"");
  }
  return status;
}
