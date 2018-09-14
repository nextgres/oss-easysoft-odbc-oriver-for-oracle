#ifndef _ORA_MEM_FUNCTIONS_H
#define _ORA_MEM_FUNCTIONS_H
#include <stdlib.h>

#define ORAFREE(aptr) do { if(aptr) { free (aptr); aptr=NULL; } } while(0)
#define ORAMALLOC(size)  ora_malloc(size)
#define ORAREALLOC(ptr, size) ora_realloc(ptr,size)
void* ora_malloc(size_t num);
void* ora_realloc(void *ptr, size_t size);
#endif
