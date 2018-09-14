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
 * $Id: common.h,v 1.12 2005/11/01 20:48:56 dbox Exp $
 *
 *
 * Revision 1.10  2004/08/27 19:50:47  dbox
 * re worked handle init/reset functions
 *
 * Revision 1.9  2004/08/06 20:44:33  dbox
 * move and change some variable types
 *
 * Revision 1.8  2003/02/11 21:37:55  dbox
 * fixed a problem with array inserts, SQLSetStmtAttr and SQLGetStmtAttr
 * for parameters SQL_ATTR_PARAMSET_SIZE and SQL_ATTR_PARAM_BIND_TYPE
 * are now functional
 *
 * Revision 1.7  2003/01/30 19:16:01  dbox
 * more tweaking of SQLColAttribute.c and friends
 *
 * Revision 1.6  2003/01/17 23:27:15  dbox
 * more solaris tidy-ups
 *
 * Revision 1.5  2003/01/17 22:34:51  dbox
 * changing greivous mutex lock/unlock typo
 *
 * Revision 1.4  2002/06/19 22:21:37  dbox
 * more tweaks to OCI calls to report what happens when DEBUG level is set
 *
 * Revision 1.2  2002/05/14 23:01:06  dbox
 * added a bunch of error checking and some 'constructors' for the
 * environment handles
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.18  2000/07/21 10:16:08  tom
 * Changes to descriptors to accomodate LOBs
 * Prototypes for new ExtendedFetch/FetchScroll/Fetch internal functions
 *
 * Revision 1.17  2000/07/07 07:50:21  tom
 * Prototypes added for alt_fetch functions
 * New statement and IRD attrs for block cursors
 *
 * Revision 1.16  2000/06/05 15:50:39  tom
 * Various fixes
 * Initial Bound Params implementation
 *
 * Revision 1.15  2000/05/23 16:05:54  tom
 * autocommit added to dbc
 *
 * Revision 1.14  2000/05/17 15:20:21  tom
 * OCIEnv moved to dbc
 *
 * Revision 1.13  2000/05/11 13:17:32  tom
 * *** empty log message ***
 *
 * Revision 1.12  2000/05/10 12:42:44  tom
 * Various updates
 *
 * Revision 1.11  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.10  2000/05/04 14:56:32  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.9  2000/05/03 16:02:01  tom
 * Tracing added
 *
 * Revision 1.8  2000/05/02 16:24:58  tom
 * resolved deadlock and a few tidy-ups
 *
 * Revision 1.7  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.6  2000/04/26 15:24:42  tom
 * Handles made more robust
 *
 * Revision 1.5  2000/04/26 10:11:03  tom
 * Removed (currently) unnecessary fn stub
 *
 * Revision 1.4  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.3  2000/04/19 16:39:01  nick
 * Bring in autoconf
 *
 * Revision 1.2  2000/04/19 15:30:00  tom
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

/*
 * The symbol UNIX_DEBUG turns on some heavy (and pretty random) debugging,
 * useful for debugging the driver and possibly may help with debugging apps.
 * It may be more useful to define it at the top of the relevent source files
 * rather than here.
 */
/*#undef UNIX_DEBUG*/

#define DPRINT fprintf

#if defined(HAVE_CONFIG_H)
#include "config.h"

#endif

#include "ocitrace.h"
/*
 * Some constants for Oracle.
 */
#define ORACLE_MAX_SCHEMA_LEN 32
#define ORACLE_MAX_TABLE_LEN 32
#define ORACLE_MAX_COLUMN_LEN 32

/*
 * Tracing constants
 */
#define TRACE_FUNCTION_ENTRY 1
#define TRACE_FUNCTION_EXIT 2

/*
 * Product name etc
 */
#define ERROR_VENDOR "[Easysoft]"
#define ERROR_PRODUCT "[Oracle]"

 /*
  * Tracefile default
  */
#if !defined(WIN32)
#define TRACEFILE_DEFAULT "/tmp/oracle.log"
#else
#define TRACEFILE_DEFAULT "C:\\oracle.log"
#endif

/*
 * 64 bit ints defined here for Solaris
 */
#if defined HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if defined(__sun)
typedef uint64_t u_int64_t;
#endif

/*
 * Windows specific includes
 */

#if defined (WIN32)
#include <windows.h>
#include <odbcinst.h>
#define u_int64_t __int64
#define int64_t __int64
#endif

/*
 * general includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#include <math.h>
#if defined(HAVE_LIBPTHREAD)
#include <pthread.h>
#endif


/*
 * A lot of apps try to handle segmentation fault and exit in
 * a controlled way, giving no indication of where the actual
 * failure was. For this reason there are a few signal resets
 * in the debugging code.
 */
#ifdef UNIX_DEBUG
#include <signal.h>
#endif
#include <assert.h>


#include <oci.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

struct hgeneric_TAG;

#define IS_VALID(x) (x !=NULL && x->valid_flag==VALID_FLAG_DEFAULT)
#include "diagnostics.h"
#include "mem_functions.h"

/*
 * Base class for all handles.
 */
typedef struct hgeneric_TAG
{
#if defined(HAVE_LIBPTHREAD)
    pthread_mutex_t mutex;
#elif defined(WIN32)
    HANDLE mutex;
#endif
  error_node *error_list;
  error_header diag_fields;
  int htype;
  SQLSMALLINT valid_flag;
}hgeneric;

/*
 * Accessor macros for handle base class members.
 */
#define ERROR_LIST(h) (((hgeneric*)h)->error_list)
#define HANDLE_TYPE(h) (((hgeneric*)h)->htype)
#define DIAG_FIELDS(h) (((hgeneric*)h)->diag_fields)
#define HANDLE_MUTEX(h) (((hgeneric*)h)->mutex)
#define HANDLE_MUTEX_PTR(h) &(((hgeneric*)h)->mutex)

/*
 * Special case macros for mutex's
 */
#if defined(HAVE_LIBPTHREAD)
#define THREAD_MUTEX_LOCK(h) pthread_mutex_lock(&(((hgeneric*)h)->mutex))
#define THREAD_MUTEX_UNLOCK(h) pthread_mutex_unlock(&(((hgeneric*)h)->mutex))
#define GLOBAL_MUTEX_LOCK(h) pthread_mutex_lock(&(h))
#define GLOBAL_MUTEX_UNLOCK(h) pthread_mutex_unlock(&(h))
/*#elif defined(WIN32)
#define THREAD_MUTEX_LOCK(h) WaitForSingleObject(&(((hgeneric*)h)->mutex),INFINITE );
#define THREAD_MUTEX_UNLOCK(h) ReleaseMutex(&(((hgeneric*)h)->mutex))
#define GLOBAL_MUTEX_LOCK(h) WaitForSingleObject(&(h),INFINITE);
#define GLOBAL_MUTEX_UNLOCK(h) ReleaseMutex(&(h));*/
#else
#define THREAD_MUTEX_LOCK(h)
#define THREAD_MUTEX_UNLOCK(h)
#define GLOBAL_MUTEX_LOCK(h)
#define GLOBAL_MUTEX_UNLOCK(h)
#endif

/*
 * We need some global mutex's, mainly for the lexer.
 */
#if defined(HAVE_LIBPTHREAD)
    extern pthread_mutex_t ood_global_mutex;
#elif defined(WIN32)
    extern HANDLE ood_global_mutex;
#endif
    extern int ood_thread_init;

/*
 * Product name etc
 */
#define ERROR_VENDOR "[Easysoft]"
#define ERROR_PRODUCT "[Oracle]"


/*
 * Identifiers for possible types of descriptors
 */
#define DESC_AP 0
#define DESC_IP 1
#define DESC_AR 2
#define DESC_IR 3

SQLRETURN not_implemented();
SQLRETURN init_hgeneric(hgeneric *t);
#define SQLSMALLINT_DEFAULT (SQLSMALLINT)0
#define SQLCHAR_DEFAULT (SQLCHAR)NULL
#define SQLPOINTER_DEFAULT (SQLPOINTER)NULL
#define SQLINTEGER_DEFAULT (SQLINTEGER)0
#define VALID_FLAG_DEFAULT 100
#define ub2_DEFAULT (ub2)0
#define ub4_DEFAULT (ub4)0
#define sb2_DEFAULT (sb2)0
#define sb4_DEFAULT (sb4)0
#define int_DEFAULT (int)0
#define FUNCTION_DEFAULT not_implemented
#define struct_DEFAULT (void*)NULL;
#define OCILobLocator_DEFAULT (OCILobLocator*)NULL

/*
 * ODBC Handle strunctures :- see later for the definitions.
 */
struct hEnv_TAG;
struct hDbc_TAG;
struct hStmt_TAG;
struct hDesc_TAG;

OCIEnv * gOCIEnv_p;

/*
 * Application Parameter Descriptor
 * Actually the same as the Application Row descriptor
 */
#define ap_TAG ar_TAG

/*
 * Implementation Parameter Descriptor
 * Actually the same as the Implementation Row descriptor
 */
#define ip_TAG ir_TAG

/*
 * Application Row Descriptor
 */
struct ar_TAG
{
  SQLSMALLINT auto_unique;
  SQLCHAR*    base_column_name;
  SQLCHAR*    base_table_name;
  SQLSMALLINT case_sensitive;
  SQLCHAR*    catalog_name;
  SQLSMALLINT concise_type;
  SQLPOINTER  data_ptr;
  SQLINTEGER  display_size;
  SQLSMALLINT fixed_prec_scale;
  SQLINTEGER *bind_indicator; /* for bound columns */
  SQLINTEGER  bind_indicator_malloced;
  SQLINTEGER  length;
  SQLCHAR*    literal_prefix;
  SQLCHAR*    literal_suffix;
  SQLCHAR*    local_type_name;
  SQLCHAR     column_name[ORACLE_MAX_COLUMN_LEN+1];
  SQLSMALLINT nullable;
  SQLINTEGER  num_prec_radix;
  SQLINTEGER  octet_length;
  SQLSMALLINT precision;
  SQLSMALLINT scale;
  SQLCHAR*    schema_name;
  SQLSMALLINT searchable;
  SQLCHAR*    table_name;
  SQLSMALLINT data_type;
  SQLCHAR*    type_name;
  SQLSMALLINT un_signed;
  SQLSMALLINT updateable;

  SQLINTEGER buffer_length;
  SQLSMALLINT bind_target_type;
  SQLSMALLINT valid_flag;
};

/*
 * Implementation Row Descriptor
 */
struct ir_TAG
{
  ub2 data_type;
  ub2 orig_type;
  ub4 data_size;
  ub4 col_num;
  SQLRETURN (*default_copy)();
  SQLRETURN (*to_string)();
  SQLRETURN (*to_oracle)();
  struct hDesc_TAG *desc;
  void *data_ptr;
  sb2 *ind_arr;
  ub2 *length_arr;
  ub2 *rcode_arr;
  /*
   * For locator based types
   */
  OCILobLocator **locator;       /* The oracle locator */
  ub4 posn;  /* the current posistion in the data */
  ub4 lobsiz; /* the size of the lob */
  SQLSMALLINT valid_flag;
};

/*
 * Environment Handle
 */
struct hEnv_TAG
{
  hgeneric base_handle;

  SQLHANDLE parent;
  SQLINTEGER odbc_ver;
  SQLSMALLINT valid_flag;
};

/*
 * Connection Handle
 */
struct hDbc_TAG
{
  hgeneric base_handle;

  char UID[32];
  char PWD[64];
  char DB[128];
  char DSN[64];

  OCIEnv *oci_env;
  OCIError *oci_err;
  OCIServer *oci_srv;
  OCISvcCtx *oci_svc;
  OCISession *oci_ses;

  SQLUINTEGER metadata_id;
  SQLUINTEGER trace;
  SQLCHAR tracefile[FILENAME_MAX];

  ub4 autocommit;

  struct hStmt_TAG *stmt_list;
  struct hDesc_TAG *desc_list;

  struct hEnv_TAG *env;
  SQLSMALLINT valid_flag;

};

/*
 * Describe Handle
 */
struct hDesc_TAG
{
    hgeneric base_handle;

    ub4 num_recs;
    int type;
    int bound_col_flag;
	int lob_col_flag;
    union recs_TAG
    {
        struct ap_TAG *ap;
        struct ip_TAG *ip;
        struct ar_TAG *ar;
        struct ir_TAG *ir;
    }recs;

    struct hDesc_TAG *next;
    struct hDesc_TAG *prev;

    struct hStmt_TAG *stmt;
    struct hDbc_TAG *dbc;
  SQLSMALLINT valid_flag;
};

/*
 * Statement Handle
 */
struct hStmt_TAG
{
  hgeneric base_handle;

  struct hDesc_TAG *implicit_ap;
  struct hDesc_TAG *implicit_ip;
  struct hDesc_TAG *implicit_ar;
  struct hDesc_TAG *implicit_ir;
  struct hDesc_TAG *current_ap;
  struct hDesc_TAG *current_ip;
  struct hDesc_TAG *current_ar;
  struct hDesc_TAG *current_ir;

  OCIStmt *oci_stmt;
  struct hStmt_TAG *next;
  struct hStmt_TAG *prev;

  ub2 stmt_type; /* The Oracle stmt type ie select, update*/
  int num_result_rows; /* The number of the result rows */
  int num_fetched_rows; /* we need this data anyway */
  int bookmark;
  int current_row;
  sword (*alt_fetch)(struct hStmt_TAG*); /*alternative fetch func */
  void *alt_fetch_data; /* Extra things alt_fetch needs,
			   is ORAFREEd so must  be malloc'd */
  SQLRETURN fetch_status; /* to return next */
  char *sql;
  struct hDbc_TAG *dbc;

  SQLPOINTER row_bind_offset_ptr;
  SQLPOINTER param_bind_offset_ptr;
  SQLUINTEGER row_array_size;
  SQLUSMALLINT *row_status_ptr;
  SQLUINTEGER row_bind_type;
  SQLUINTEGER *rows_fetched_ptr;
  SQLUINTEGER param_bind_type;
  SQLUINTEGER paramset_size;
  SQLUINTEGER query_timeout; /* NOTE: does nothing (TODO)*/
  SQLSMALLINT valid_flag;

};

/*
 * typedefs for handles and structures
 */
typedef struct hEnv_TAG hEnv_T;
typedef struct hDbc_TAG hDbc_T;
typedef struct hStmt_TAG hStmt_T;
typedef struct hDesc_TAG hDesc_T;
#define ap_T ar_T
typedef struct ar_TAG ar_T;
#define ip_T ir_T
typedef struct ir_TAG ir_T;

void ood_ar_init (ar_T *t);
void dump_ar_T(ar_T * t);
void ood_ir_init (ir_T *t, ub4 col_num, hDesc_T *desc);
void ood_ir_free_contents (ir_T *t);
void dump_ir_T(ir_T * t);

hEnv_T * make_hEnv_T();
hDbc_T * make_hDbc_T();
hDesc_T * make_hDesc_T();
hStmt_T * make_hStmt_T();
int debugLevel();
int debugLevel2();
int debugLevel3();
void setDebugLevel(int i);

/*
 * FUDGE dm functions
 */
#if !defined(WIN32)
extern SQLRETURN SQLGetPrivateProfileString();
#endif

/*
 * Local versions of ODBC API functions and other strangitudes
 */
/*diagnostics.c*/
SQLRETURN _SQLGetDiagField(SQLSMALLINT,SQLHANDLE,SQLSMALLINT,SQLSMALLINT,
        SQLPOINTER,SQLSMALLINT,SQLSMALLINT*);
SQLRETURN _SQLGetDiagRec(SQLSMALLINT,SQLHANDLE,SQLSMALLINT,SQLCHAR*,
        SQLINTEGER*,SQLCHAR*,SQLSMALLINT,SQLSMALLINT*);

/*SQLFreeHandle.c*/
void ood_ap_free(ap_T *ap);
void ood_ar_free(ar_T *ar, int num_recs);
void ood_ip_free(ip_T *ip, int num_recs);
void ood_ir_free(ir_T *ir, int num_recs);
SQLRETURN _SQLFreeHandle(SQLSMALLINT,SQLHANDLE);

/*SQLGetData.c*/
SQLRETURN  _SQLGetData(SQLHSTMT,SQLUSMALLINT,SQLSMALLINT,
        SQLPOINTER,SQLINTEGER,SQLINTEGER*);

/*SQLAllocHandle.c*/
SQLRETURN _SQLAllocHandle(SQLSMALLINT,SQLHANDLE,SQLHANDLE*);

/*SQLBindParameter.c*/
SQLRETURN _SQLBindParameter(SQLHSTMT,SQLUSMALLINT,SQLSMALLINT,SQLSMALLINT,
        SQLSMALLINT,SQLUINTEGER,SQLSMALLINT,SQLPOINTER,SQLINTEGER,SQLINTEGER*);

/*SQLEndTran.c*/
SQLRETURN _SQLEndTran(SQLSMALLINT,SQLHANDLE,SQLSMALLINT);

/*SQLDriverConnect.c*/
SQLRETURN ood_SQLDriverConnect(SQLHDBC,SQLHWND,SQLCHAR*,SQLSMALLINT,SQLCHAR*,\
        SQLSMALLINT,SQLSMALLINT*,SQLUSMALLINT);

/*SQLFetch.c*/
SQLRETURN ood_SQLFetch(hStmt_T*);
/*SQLFetchScroll.c*/
SQLRETURN ood_SQLFetchScroll(hStmt_T*,SQLSMALLINT,SQLINTEGER);


/*SQLFreeStmt.c*/
SQLRETURN _SQLFreeStmt(SQLHSTMT,SQLUSMALLINT);

/*SQLGetTypeInfo.c :- alternate fetch function */
sword ood_fetch_sqlgettypeinfo(struct hStmt_TAG* stmt);

/*SQLColumns.c*/
sword ood_fetch_sqlcolumns(struct hStmt_TAG* stmt);

/*SQLSpecialColumns.c*/
sword ood_fetch_sqlspecialcolumns(struct hStmt_TAG* stmt);

/* Not worth having an h file for this fn alone... from sqllex.l */
char* ood_lex_parse(char *, int, int *);

/*
 * Include the prototypes from other source files
 */
#include "string_functions.h"
#include "oracle_functions.h"
#include "common_functions.h"

#endif
