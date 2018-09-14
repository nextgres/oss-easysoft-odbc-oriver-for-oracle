
/* test of ORAMALLOC and ORAFREE
 * author: Dennis Box, dbox@fnal.gov
 * $Id: malloctest.c,v 1.4 2003/01/17 23:10:42 dbox Exp $
 */

#include "test_defs.h"
#include <mem_functions.h>
#include <stdio.h>

int main()
{
	long long  i = 10;
	long long j = 0;
	void **ptr;
	while(i< 200){
		ptr=ORAMALLOC(i*sizeof(i));
		VERBOSE("alloced %Ld bytes, ptr=%x\n",i,ptr);
		for(j=0; j<i; j++){
		  void *p2 = ORAMALLOC(i*sizeof(i));
		  ptr[j]=p2;
		}
		VERBOSE("alloced %Ld blocks of %Ld  bytes\n",i,i);
		for(j=0; j<i; j++){
		  ORAFREE(ptr[j]);
		  assert(ptr[j]==NULL); 
		}
		VERBOSE("freed %Ld blocks\n",i);
		ORAFREE(ptr);	
		assert(ptr==NULL );
		VERBOSE("freed %Ld bytes, ptr=%x\n",i,ptr);
		i=i*10;
	}
	return 0;
}
