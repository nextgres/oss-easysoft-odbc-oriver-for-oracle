
/* test of low_level 'constructors' for   ar_T * , ir_T * and  hEnv_T * */

 /* author: Dennis Box, dbox@fnal.gov
 * $Id: lowlevel.c,v 1.4 2004/08/27 19:53:49 dbox Exp $
 */

#include <mem_functions.h>
#include <common.h>
#include <stdio.h>
#include "test_defs.h"
#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>

int main()
{

  SQLRETURN ret;
  ar_T * art;
  ir_T * irt;
  hEnv_T * het;

  hDbc_T * hdt;
  hDesc_T *ar, *ap, *ir, *ip;
  hStmt_T * hst;

  art = ORAMALLOC(sizeof(ar_T));
  ood_ar_init (art);
  assert(IS_VALID(art));
  if(getenv("VERBOSE"))
    dump_ar_T(art);

  irt =  ORAMALLOC(sizeof(ir_T));
  ood_ir_init (irt,1,het);
  assert(irt->valid_flag==VALID_FLAG_DEFAULT);
 if(getenv("VERBOSE"))
    dump_ir_T(irt);

  ret = irt->to_string();
  assert(ret==SQL_ERROR);

  het = make_hEnv_T();
  assert(het->valid_flag==VALID_FLAG_DEFAULT);
  assert(het->base_handle.valid_flag==VALID_FLAG_DEFAULT);

  hdt = make_hDbc_T();
  assert(hdt->valid_flag==VALID_FLAG_DEFAULT);
  assert(hdt->base_handle.valid_flag==VALID_FLAG_DEFAULT);

  hst = make_hStmt_T();
  assert(hst->valid_flag==VALID_FLAG_DEFAULT);
  assert(hst->base_handle.valid_flag==VALID_FLAG_DEFAULT);

  ap = make_hDesc_T();
  assert(ap->valid_flag==VALID_FLAG_DEFAULT);
  assert(ap->base_handle.valid_flag==VALID_FLAG_DEFAULT);
  ar = make_hDesc_T();
  assert(ar->valid_flag==VALID_FLAG_DEFAULT);
  assert(ar->base_handle.valid_flag==VALID_FLAG_DEFAULT);
  ip = make_hDesc_T();
  assert(ip->valid_flag==VALID_FLAG_DEFAULT);
  assert(ip->base_handle.valid_flag==VALID_FLAG_DEFAULT);
  ir = make_hDesc_T();
  assert(ir->valid_flag==VALID_FLAG_DEFAULT);
  assert(ir->base_handle.valid_flag==VALID_FLAG_DEFAULT);
  
  hst->implicit_ap=ap;
  hst->current_ap=hst->implicit_ap;
  hst->implicit_ar=ar;
  hst->current_ar=hst->implicit_ar;
  hst->implicit_ip=ip;
  hst->current_ip=hst->implicit_ip;
  hst->implicit_ir=ir;
  hst->current_ir=hst->implicit_ir;


  assert(IS_VALID(hst->implicit_ap));
  assert(IS_VALID(hst->implicit_ar));
  assert(IS_VALID(hst->implicit_ip));
  assert(IS_VALID(hst->implicit_ir));
  assert(IS_VALID(hst->current_ap));
  assert(IS_VALID(hst->current_ar));
  assert(IS_VALID(hst->current_ip));
  assert(IS_VALID(hst->current_ir));
  assert(IS_VALID(hst));

  ORAFREE(ap);
  ORAFREE(ar);
  ORAFREE(ir);
  ORAFREE(ip);
  ORAFREE(hdt);
  ORAFREE(het);
  ORAFREE(irt);
  ORAFREE(art);

  return 0;
}
