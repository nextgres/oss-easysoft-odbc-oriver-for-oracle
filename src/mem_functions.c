#include "mem_functions.h"
#include <stdio.h>
/* $Id: mem_functions.c,v 1.2 2002/11/14 22:28:36 dbox Exp $*/
void* ora_malloc(size_t size)
{
  void * ptr = malloc(size);
  if(ptr==NULL){
    fprintf(stderr,"Out Of Memory Error line %d file %s\n",__LINE__,__FILE__);
    exit(-1);
  }
  return ptr;
}

void* ora_realloc(void *ptr, size_t size)
{  
  void * ptr2 = realloc(ptr,size);
  if(ptr==NULL){
    fprintf(stderr,"Out Of Memory Error line %d file %s\n",__LINE__,__FILE__);
    exit(-1);
  }
  return ptr2;
}
