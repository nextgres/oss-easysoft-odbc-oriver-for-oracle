/******************************************************************************
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
 * $Id: SQLAllocHandle.c,v 1.6 2004/08/27 19:35:46 dbox Exp $
 *
 * $Log: SQLAllocHandle.c,v $
 * Revision 1.6  2004/08/27 19:35:46  dbox
 * change autocommit behavior to conform w standards
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
 * Revision 1.17  2000/07/21 10:01:29  tom
 * Fixed descriptor flag init bug
 *
 * Revision 1.16  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.15  2000/07/07 07:55:36  tom
 * changes for block cursors (stmt handle attributes init)
 *
 * Revision 1.14  2000/06/05 15:55:49  tom
 * Initial bound param implementation
 * Couple of bugfixes
 *
 * Revision 1.13  2000/05/23 16:05:28  tom
 * Now sets autcommit correctly
 *
 * Revision 1.12  2000/05/17 15:16:26  tom
 * OCIEnv moved to dbc handle
 *
 * Revision 1.11  2000/05/11 13:17:32  tom
 * *** empty log message ***
 *
 * Revision 1.10  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.9  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.8  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.7  2000/05/02 16:24:58  tom
 * resolved deadlock and a few tidy-ups
 *
 * Revision 1.6  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.5  2000/04/26 15:26:47  tom
 * Changes to account for changes to handle definitions
 *
 * Revision 1.4  2000/04/26 10:12:42  tom
 * Various initialistion changes
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:23:14  tom
 * First functional checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 * Revision 1.1  2000/04/04 14:53:44  tom
 * Initial revision
 *
 ******************************************************************************/

static char const rcsid[]= "$RCSfile: SQLAllocHandle.c,v $ $Revision: 1.6 $";

#include "common.h"

static void descriptor_init(hDesc_T *this,hDesc_T* next,hDesc_T *prev,
        int type,hStmt_T* stmt)
{
  assert(IS_VALID(this));
  this->type=type;


    this->next=next;
    this->prev=prev;
    this->stmt=stmt;
    this->dbc=stmt->dbc;
	
    this->bound_col_flag=0;
    this->lob_col_flag=0;
    this->num_recs=0;
    ood_mutex_init((hgeneric*)this);
    ood_init_hgeneric(this,SQL_HANDLE_DESC);
}

SQLRETURN _SQLAllocHandle( 
    SQLSMALLINT     HandleType,
    SQLHANDLE         InputHandle,
    SQLHANDLE        *OutputHandlePtr )
{
    
    hgeneric *parent=(hgeneric*)InputHandle;
    
    if(parent){
      assert(IS_VALID(parent));
      ood_clear_diag((hgeneric*)parent);
    }

    switch(HandleType)
    {
        case SQL_HANDLE_ENV:
        {
            /*
             * Environment Initialisation etc.
             */
            hEnv_T *env;

            if ( !OutputHandlePtr )
            {
                *OutputHandlePtr = SQL_NULL_HENV;
                return SQL_ERROR;
            }
    
    
            
	    env = make_hEnv_T();
            if ( !env )
            {
                *OutputHandlePtr = SQL_NULL_HENV;
                return SQL_ERROR;
            }

            env->parent=InputHandle;
            HANDLE_TYPE(env)=SQL_HANDLE_ENV;
            ood_init_hgeneric(env,SQL_HANDLE_ENV);
            *OutputHandlePtr=(SQLHANDLE)env;

            ood_mutex_init((hgeneric*)env);
            return(SQL_SUCCESS);
        }
        break;

        case SQL_HANDLE_DBC:
        {
            hDbc_T *dbc;

            if (!OutputHandlePtr)
            {
                *OutputHandlePtr=SQL_NULL_HDBC;
                return SQL_ERROR;
            }

            if(!parent || parent->htype!=SQL_HANDLE_ENV)
            {
                *OutputHandlePtr = SQL_NULL_HDBC;
                return SQL_INVALID_HANDLE;
            }

            
	    dbc=make_hDbc_T();
            if(!dbc)
            {
                *OutputHandlePtr = SQL_NULL_HDBC;
                ood_post_diag(parent,ERROR_ORIGIN_HY001,0,"",
                        ERROR_MESSAGE_HY001,
                        __LINE__,0,"",ERROR_STATE_HY001,
                        __FILE__,__LINE__);
                return(SQL_ERROR);
            }

            dbc->env=(hEnv_T*)parent;
            dbc->trace=SQL_OPT_TRACE_OFF;
            strcpy((char*)dbc->tracefile,(char*)TRACEFILE_DEFAULT);

            /*
             * Explicitly set the OCI handles to NULL
             */
            dbc->oci_err=NULL;
            dbc->oci_srv=NULL;
            dbc->oci_svc=NULL;
            dbc->oci_ses=NULL;
            dbc->oci_env=NULL;

	    /* autocommit ON is the normal default for ODBC. It can be
	       changed through SQLSetConnectAttr.  */
	    dbc->autocommit=OCI_COMMIT_ON_SUCCESS;

	    dbc->metadata_id=0;

            /*
             * init the lists
             */
            dbc->stmt_list=NULL;
            dbc->desc_list=NULL;

            ood_init_hgeneric(dbc,SQL_HANDLE_DBC);
            *OutputHandlePtr=(SQLHANDLE)dbc;

            ood_mutex_init((hgeneric*)dbc);
            return(SQL_SUCCESS);
        }
        break;

        case SQL_HANDLE_STMT:
        {
            hStmt_T *stmt;
            hDbc_T *dbc=(hDbc_T*)InputHandle;
	    hDesc_T *ar, *ap, *ir, *ip;
	    assert(IS_VALID(dbc));

            if(!dbc || HANDLE_TYPE(dbc)!=SQL_HANDLE_DBC)
                return SQL_INVALID_HANDLE;
if(ENABLE_TRACE){
            ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
                    (SQLHANDLE)dbc,SQL_SUCCESS,
                    "i","Handle Type",SQL_HANDLE_STMT);
}

            if(!OutputHandlePtr)
            {
                ood_post_diag(parent,ERROR_ORIGIN_HY009,0,"",
                        ERROR_MESSAGE_HY009,
                        __LINE__,0,"",ERROR_STATE_HY009,
                        __FILE__,__LINE__);
if(ENABLE_TRACE){
            ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                    (SQLHANDLE)dbc,SQL_ERROR,
                    "s","OutputHandlePtr","NULL");
}
                return SQL_ERROR;
            }

            
	    stmt = make_hStmt_T();
            stmt->oci_stmt=NULL;
#ifdef UNIX_DEBUG
			fprintf(stderr,"%s %d stmt [0x%.8lx] \n",
					__FILE__,__LINE__,(long)stmt);
#endif
			
            stmt->dbc=dbc;
            ood_init_hgeneric(stmt,SQL_HANDLE_STMT);
            ood_mutex_init((hgeneric*)stmt);

            /*
             * Set up our four implicits
             */
            
	    ap=make_hDesc_T();
	    stmt->implicit_ap=ap;
            stmt->current_ap=ap;

            
            ar=make_hDesc_T();
	    stmt->implicit_ar=ar;
	    stmt->current_ar=ar;

            
            ip=make_hDesc_T();
            stmt->implicit_ip=ip;
            stmt->current_ip=ip;

           
            ir=make_hDesc_T();
            stmt->implicit_ir=ir;
            stmt->current_ir=ir;

            descriptor_init(stmt->implicit_ap,stmt->implicit_ar,
                    NULL,DESC_AP,stmt);
            descriptor_init(stmt->implicit_ar,stmt->implicit_ip,
                    stmt->implicit_ap,DESC_AR,stmt);

            descriptor_init(stmt->implicit_ip,stmt->implicit_ir,
                    stmt->implicit_ar,DESC_IP,stmt);
            descriptor_init(stmt->implicit_ir,NULL,
                    stmt->implicit_ar,DESC_IR,stmt);

            /*
             * initialise to defaults
             */
	    stmt->num_result_rows=0;
	    stmt->alt_fetch=NULL;
	    stmt->alt_fetch_data=NULL;
	    stmt->sql=NULL;
	    stmt->row_bind_offset_ptr=NULL;
	    stmt->row_array_size=1;
	    stmt->param_bind_offset_ptr=NULL;
	    stmt->rows_fetched_ptr=NULL;
	    stmt->num_fetched_rows=0;
	    stmt->fetch_status=SQL_SUCCESS;
	    stmt->current_row=0;
	    stmt->bookmark=0;
	    stmt->row_status_ptr=NULL;

            /*
             * Drop them into the descriptor list
             */
            THREAD_MUTEX_LOCK(dbc);
            stmt->current_ap->prev=NULL;
            if(dbc->desc_list)
                dbc->desc_list->prev=stmt->current_ir;
            stmt->current_ir->next=dbc->desc_list;
            dbc->desc_list=stmt->current_ap;

            /*
             * Finally drop the Statement into the statement list
             */
            stmt->prev=NULL;
            stmt->next=dbc->stmt_list;
            if(dbc->stmt_list)
                dbc->stmt_list->prev=stmt;
            dbc->stmt_list=stmt;
            THREAD_MUTEX_UNLOCK(dbc);

            *OutputHandlePtr=(SQLHANDLE)stmt;
if(ENABLE_TRACE){
            ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                    (SQLHANDLE)dbc,SQL_SUCCESS,
                    "h","Output Handle",
                    stmt);
}
        }
        break;

        case SQL_HANDLE_DESC:
        {
            hDbc_T *dbc=(hDbc_T*)InputHandle;
            hDesc_T *desc;
       	    assert(IS_VALID(dbc));
         
            if(!dbc || HANDLE_TYPE(dbc)!=SQL_HANDLE_DBC)
                return SQL_INVALID_HANDLE;
if(ENABLE_TRACE){
            ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
                    (SQLHANDLE)dbc,SQL_SUCCESS,
                    "i","Handle Type",
                    SQL_HANDLE_DESC);
}


            if(!OutputHandlePtr)
            {
                ood_post_diag(parent,ERROR_ORIGIN_HY009,0,"",
                        ERROR_MESSAGE_HY009,
                        __LINE__,0,"",ERROR_STATE_HY009,
                        __FILE__,__LINE__);
if(ENABLE_TRACE){
            ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                    (SQLHANDLE)dbc,SQL_ERROR,
                    "p","OutputHandlePtr","NULL");
}
                return SQL_ERROR;
            }

            
	    desc=make_hDesc_T();
            if(!desc)
            {
                *OutputHandlePtr = SQL_NULL_HDBC;
                ood_post_diag(parent,ERROR_ORIGIN_HY001,0,"",
                        ERROR_MESSAGE_HY001,
                        __LINE__,0,"",ERROR_STATE_HY001,
                        __FILE__,__LINE__);
if(ENABLE_TRACE){
            ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                    (SQLHANDLE)dbc,SQL_ERROR,
                    "");
}
                return(SQL_ERROR);
            }
            ood_mutex_init((hgeneric*)desc);

            desc->prev=NULL;
            desc->stmt=NULL;
            desc->dbc=dbc;
            ood_init_hgeneric(desc,SQL_HANDLE_DESC);
            /* 
             * Drop the descriptor into the descriptor list
             */
            THREAD_MUTEX_LOCK(dbc);
            desc->next=dbc->desc_list;
            dbc->desc_list=desc;
            THREAD_MUTEX_UNLOCK(dbc);

            *OutputHandlePtr = (SQLHANDLE)desc;
if(ENABLE_TRACE){
            ood_log_message(dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                    (SQLHANDLE)dbc,SQL_SUCCESS,
                    "p","Output Handle",
                    desc);
}

            return SQL_SUCCESS;
        }
        break;
        
        default:
            return SQL_ERROR;
    }
   
    return SQL_SUCCESS;
}

SQLRETURN SQL_API SQLAllocHandle( 
    SQLSMALLINT     HandleType,
    SQLHANDLE         InputHandle,
    SQLHANDLE        *OutputHandlePtr )
{
    setDebugLevel(0);
    if(getenv("DEBUG"))	
	setDebugLevel(atoi(getenv("DEBUG")));
    return _SQLAllocHandle( HandleType, InputHandle, OutputHandlePtr );
}
