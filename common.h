#ifndef DSCCOMMONH
#define DSCCOMMONH

#include <stdlib.h>
#include <time.h>

#define SAMPLE(ary, n) \
do{ \
int i=0; \
srand(time(0)); \
(*(ary)) = (unsigned *)malloc(sizeof(unsigned) * (n)); \
if(*(ary))for(; i < n; ++i)*(*(ary) + i) = rand(); \
}while(0)

#define _SAMPLE(ary) \
do{ \
if((ary))free((ary)); \
}while(0)

#define TRAVERSING(ary, n) \
do{ \
int i=0; \
for(; i < n;++i)printf("%d : %u\n", i, ary[i]); \
}while(0)

#endif