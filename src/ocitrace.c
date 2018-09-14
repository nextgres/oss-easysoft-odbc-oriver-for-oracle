#include "common.h"
#include "ocitrace.h"
#include <sqlext.h>
#include <oci.h>

char* typep(ub1 ivar){
  switch(ivar){ 
  case OCI_HTYPE_BIND :
    return "OCI_HTYPE_BIND ";
  case OCI_HTYPE_DEFINE :
    return "OCI_HTYPE_DEFINE ";
  default:
    return "ERROR";
  }
}


char* in_outp(ub1 ivar){
  switch(ivar){ 
  case OCI_PARAM_IN:
    return "OCI_PARAM_IN";
  case OCI_PARAM_OUT:
    return "OCI_PARAM_OUT";
  default:
    return "ERROR";
  }
}

char* piecep(ub1 ivar){
  switch(ivar){ 
  case OCI_ONE_PIECE:
    return "OCI_ONE_PIECE";
  case   OCI_FIRST_PIECE: 
    return "OCI_FIRST_PIECE";
  case   OCI_NEXT_PIECE:
    return "OCI_NEXT_PIECE";
  case OCI_LAST_PIECE:
    return "OCI_LAST_PIECE";
 
  default:
    return "ERROR";
  }
}
