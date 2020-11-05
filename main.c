#pragma hdrstop
#pragma argsused

#include <stdio.h>
#include "common.h"
#include "sort.h"

#ifdef _WIN32
#include <tchar.h>
#else
typedef char _TCHAR;
#define _tmain main
#endif

#define ARYLEN 100

int _tmain(int argc, _TCHAR* argv[]) {
	char c;
	unsigned *p;
	// SAMPLE(&p, ARYLEN);
	SAMPLE_RANGE(&p, ARYLEN, 100000);
	TRAVERSING(p, ARYLEN);

	MARKING("---------------------");

	// bubbleSort(p, ARYLEN, 0);
	// quickSort(p, ARYLEN, 0);
	// insertSort(p, ARYLEN, 0);
	// countSort(p, ARYLEN, 100000, 1);
	insertSort(p, ARYLEN, 0);
	// combSort11(p, ARYLEN, 0);

	TRAVERSING(p, ARYLEN);

	_SAMPLE(p);
	scanf("%c", &c);
	return 0;
}
