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
 * $Id: SQLFreeHandle.c,v 1.10 2005/11/01 20:48:56 dbox Exp $
 *
 * $Log: SQLFreeHandle.c,v 
 * Revision 1.7  2004/08/27 19:42:40  dbo
 * correct some bad behavior in ar/ir handles wrt number of records in re-used handle
 *
 * Revision 1.6  2003/12/16 01:22:06  dbox
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
 * Revision 1.5  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.4  2002/06/19 22:21:37  dbox
 * more tweaks to OCI calls to report what happens when DEBUG level is set
 *
 * Revision 1.3  2002/05/14 23:01:05  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.2  2002/05/14 12:03:19  dbox
 * fixed some malloc/free syntax
 *
 * Revision 1.1.1.1  2002/02/11 19:48:06  dbox
 * second try, importing code into directories
 *
 * Revision 1.18  2000/07/21 10:08:13  tom
 * LOB functionality added
 *
 * Revision 1.17  2000/07/07 08:03:59  tom
 * block cursors changes to reflect new IRD structure
 *
 * Revision 1.16  2000/06/05 16:01:59  tom
 * Initial implememtation of bound params
 * A bugfix or two :)
 *
 * Revision 1.15  2000/05/17 15:19:22  tom
 * OCIEnv moved to dbc
 *
 * Revision 1.14  2000/05/11 13:17:32  tom
 * *** empty log message ***
 *
 * Revision 1.13  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.12  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.11  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.10  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.9  2000/05/02 16:24:58  tom
 * resolved deadlock and a few tidy-ups
 *
 * Revision 1.8  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.7  2000/04/28 08:39:45  tom
 * Tidy up
 *
 * Revision 1.6  2000/04/26 15:30:13  tom
 * Changes to account for new handle definitions
 *
 * Revision 1.5  2000/04/26 10:27:44  tom
 * Fixed a recursion error with free_xxx_list
 *
 * Revision 1.4  2000/04/26 10:06:28  tom
 * Connection handle ORAFREE implemented fully
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:26:47  tom
 * First Function Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLFreeHandle.c,v $ $Revision: 1.10 $";

void ood_ap_free(ap_T *ap)
{
    if(ap)
        ORAFREE(ap);
}

void ood_ar_free(ar_T *ar, int num_recs)
{
    int i;
    if(ar)
    {
        for(i=0;i<=num_recs;i++)
        {
            if(ar[i].bind_indicator)
            {
                if(ar[i].bind_indicator_malloced==1)
                    ORAFREE(ar[i].bind_indicator);
            }
        }
        ORAFREE(ar);
    }
}

void ood_ip_free(ip_T *ip, int num_recs)
{
    int i;
    sword ret;
    if(ip)
    {
        for(i=0;i<=num_recs;i++)
        {
            if(ip[i].data_ptr)
            {
                ORAFREE(ip[i].data_ptr);
		if(ip[i].locator)
		  {
		    unsigned j;
		    for(j=0;j<ip->desc->stmt->row_array_size;j++)
		      {
			OCIDescriptorFree_log(ip[i].locator[j],OCI_DTYPE_LOB);
		      }
                    ORAFREE(ip[i].locator);
		  }
            }
	    if(ip[i].ind_arr)
	      ORAFREE(ip[i].ind_arr);
	    if(ip[i].length_arr)
	      ORAFREE(ip[i].length_arr);
	    if(ip[i].rcode_arr)
	      ORAFREE(ip[i].rcode_arr);
        }
        ORAFREE(ip);
    }
}

void ood_ir_free(ir_T *ir, int num_recs)
{
  int i;

  if (ir)
    {
      for(i=0;i<=num_recs;i++)
        {
	  ood_ir_free_contents (ir + i);
        }
      ORAFREE(ir);
    }
}

/*
 * Descriptor Free
 *
 * Frees the descriptor's resources AND knocks it out of the connection
 * handle's descriptor list
 */
static void descriptor_free(hDesc_T *desc)
{
    /*
     * Free the records
     */
  assert(IS_VALID(desc));
    switch(desc->type)
    {
        case DESC_AP:
            ood_ap_free(desc->recs.ap);
        break;

        case DESC_AR:
            ood_ar_free(desc->recs.ar,desc->num_recs);
        break;

        case DESC_IP:
		    if(desc->num_recs)
                ood_ip_free(desc->recs.ip,desc->num_recs);
        break;

        case DESC_IR:
		    if(desc->num_recs)
                ood_ir_free(desc->recs.ir,desc->num_recs);
        break;
    }

    THREAD_MUTEX_LOCK(desc->dbc);

    /*
     * Knock it out of the descriptor list
     */
    if(desc->prev)
        desc->prev->next=desc->next;
    else /* first in the descriptor list */
        desc->dbc->desc_list=desc->next;

    if(desc->next)
        desc->next->prev=desc->prev;
    else /*last in the descriptor list */
        desc->dbc->desc_list=NULL;
    THREAD_MUTEX_UNLOCK(desc->dbc);

    /*
     * Free the error list
     */
    ood_free_diag((hgeneric*)desc);
    ood_mutex_destroy((hgeneric*)desc);
    ORAFREE(desc);

}

static void free_descriptor_list(hDesc_T *list)
{
    if(list && IS_VALID(list))
    {
        free_descriptor_list(list->next);
        descriptor_free(list);
    }
    return;
}

static void free_statement_list(hStmt_T *list)
{
    if(list && IS_VALID(list))
    {
        free_statement_list(list->next);
        _SQLFreeHandle(SQL_HANDLE_STMT,(SQLHANDLE)list);
    }
    return;
}

/*
 * Local implemtation of SQLFreeHandle.
 *
 * NOTE
 * Statement handles and descriptor handles are removed from their respective
 * lists on the connection handle. This can result in the connection handle
 * itself being modified.
 */
SQLRETURN _SQLFreeHandle(
			 SQLSMALLINT            HandleType,
			 SQLHANDLE            Handle )
{
  sword ret;
    hDbc_T* dbc;
    hStmt_T* stmt;
    if(!Handle)
      return SQL_INVALID_HANDLE;
    
    ood_clear_diag((hgeneric*)(hgeneric*)Handle);
    
    switch(HandleType)
      {
      case SQL_HANDLE_ENV:
        {
	  /*return SQL_SUCCESS;*/
	  hEnv_T *env=(hEnv_T*)Handle;
	  assert(IS_VALID(env));
	  if(HANDLE_TYPE(env)!=SQL_HANDLE_ENV)
	    {
	      abort();
	    }
#if 0
#ifdef LIBCLNTSH8
	  OCITerminate(OCI_DEFAULT);
#endif
#endif
	  ood_mutex_destroy((hgeneric*)env);
	  ood_free_diag((hgeneric*)env);
	  ORAFREE(env);
        }
        break;
	
      case SQL_HANDLE_DBC:
        {
	  
	  /*return SQL_SUCCESS;*/
	  
	  hDbc_T *dbc=(hDbc_T*)Handle;
	  if(dbc && IS_VALID(dbc))
	    {
	      if(ENABLE_TRACE){
		ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
				(SQLHANDLE)dbc,0,"s",NULL,"(No Exit Trace)");
	      }
	      /* free the oci_stmt before oci environment handle,
		 otherwise oracle oci touches free-ed memory and we crash */
	      stmt = dbc->stmt_list;
	      while (stmt && IS_VALID(stmt)) {
		if (stmt->oci_stmt) {
		  OCIHandleFree_log_stat(stmt->oci_stmt,
					 OCI_HTYPE_STMT,ret);
		  stmt->oci_stmt = (OCIStmt *)0;
		}
		stmt = stmt->next;
	      }
	      ood_free_diag((hgeneric*)dbc);
	      THREAD_MUTEX_LOCK(dbc);
	      
	      if(dbc->oci_err){
		OCIHandleFree_log_stat(dbc->oci_err,OCI_HTYPE_ERROR,ret);
		dbc->oci_err=NULL;
	      }
	      if(dbc->oci_srv){
		OCIHandleFree_log_stat(dbc->oci_srv,OCI_HTYPE_SERVER,ret);
		dbc->oci_srv=NULL;
	      }
	      if(dbc->oci_svc){
		OCIHandleFree_log_stat(dbc->oci_svc,OCI_HTYPE_SVCCTX,ret);
		dbc->oci_svc=NULL;
	      }
	      if(dbc->oci_ses){
		OCIHandleFree_log_stat(dbc->oci_ses,OCI_HTYPE_SESSION,ret);
		dbc->oci_ses=NULL;
	      }
	      if(dbc->oci_env)
		{
		/*Turns out ORAFREE() followed by repeated OCIEnvCreate 
		* leaks memory.  Code has been 
 		* modified so OCIEnvCreate is called only once, a
		* global var gOCIEnv_p initialized  and then used 
		* everywhere dbc->oci_env and all its leak-alikes
		* were used previously.  The ORAFREE below should
		* never be called but if it does its harmless
		*/
		  /*
		   * I can't find any documentation on what should be done
		   * with the Oracle environment handle at 
		   * the end of a session.
		   * Using OCIHandleFree on it seems to yield unpredicatable
		   * results. ORAFREE() seems OK on UNIX
		   */

		  ORAFREE(dbc->oci_env);


		}
	      THREAD_MUTEX_UNLOCK(dbc);
	      free_statement_list(dbc->stmt_list);
	      free_descriptor_list(dbc->desc_list);
	      ood_mutex_destroy((hgeneric*)dbc);
	      ORAFREE(dbc);
	    }
        }
        break;
	
      case SQL_HANDLE_STMT:
        {
	  /*return SQL_SUCCESS;*/
	  hStmt_T *stmt=(hStmt_T*)Handle;
	  if(!IS_VALID(stmt)) return SQL_ERROR;
	  if(ENABLE_TRACE){
	    dbc=stmt->dbc;
	    if(!IS_VALID(dbc)) return SQL_ERROR;
	    
	    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			    (SQLHANDLE)stmt,0,"");
	  }
	  ood_free_diag((hgeneric*)stmt);
	  if(stmt->oci_stmt){
	    OCIHandleFree_log_stat(stmt->oci_stmt,OCI_HTYPE_STMT,ret);
	    stmt->oci_stmt=NULL;
	  }
	  /*
	   * Free our Descriptors
	   */
	  descriptor_free(stmt->implicit_ap);
	  descriptor_free(stmt->implicit_ip);
	  descriptor_free(stmt->implicit_ar);
	  descriptor_free(stmt->implicit_ir);
	  
	  /*
	   * Free any data associated with an alternative fetch fn
	   */
	  ORAFREE(stmt->alt_fetch_data);
	  /*
	   * And the sql
	   */
	  ORAFREE(stmt->sql);
	  
	  /*
	   * Knock the statement out of the statement list
	   */
	  THREAD_MUTEX_LOCK(stmt->dbc);
	  if(stmt->prev)
	    stmt->prev->next=stmt->next;
	  else /* first in the statement list */
	    stmt->dbc->stmt_list=stmt->next;
	  if(stmt->next)
	    stmt->next->prev=stmt->prev;
	  else /* this is the last in the list */
	    stmt->dbc->stmt_list=NULL;
	  THREAD_MUTEX_UNLOCK(stmt->dbc);
	  ood_mutex_destroy((hgeneric*)stmt);
	  ORAFREE(stmt);
	  if(ENABLE_TRACE){
	    ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
			    (SQLHANDLE)NULL,SQL_SUCCESS,"");
	  }
        }
        break;
	
    case SQL_HANDLE_DESC:
      {
	/*return SQL_SUCCESS;*/
            hDesc_T *desc=(hDesc_T*)Handle;
	    if(ENABLE_TRACE){
	      hDbc_T* dbc=desc->dbc;
	      if(!IS_VALID(desc)) return SQL_ERROR;
	      if(!IS_VALID(dbc)) return SQL_ERROR;
	      
	      ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
			      (SQLHANDLE)desc,0,"");
	    }
            if(!desc||HANDLE_TYPE(desc)!=SQL_HANDLE_DESC)
	      return SQL_INVALID_HANDLE;
	    
            if(desc->stmt)
	      {
                /*
                 * only implicit descriptors have statement refs
                 */
                ood_post_diag((hgeneric*)desc,ERROR_ORIGIN_HY017,0,"",
			      ERROR_MESSAGE_HY017,
			      __LINE__,0,desc->stmt->dbc->DSN,ERROR_STATE_HY017,
			      __FILE__,__LINE__);
	      }
            descriptor_free(desc);
	    if(ENABLE_TRACE){
	      ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
			      (SQLHANDLE)NULL,SQL_SUCCESS,"");
	    }
      }
      break;
      
    default:
      return SQL_ERROR;
    }
    return SQL_SUCCESS;
}

SQLRETURN SQL_API SQLFreeHandle(
				SQLSMALLINT            HandleType,
				SQLHANDLE            Handle )
{
  return _SQLFreeHandle( HandleType, Handle );
}
