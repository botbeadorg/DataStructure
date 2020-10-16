#ifndef DS_C_SORT_H
#define DS_C_SORT_H

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int pow10Int[10] = {
	1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

#define QUICK_POWER10_INT(n) pow10Int[(n)]

// seq: asc---1, desc---0
void bubbleSort(unsigned *ary, int n, int seq) {
	int j;
	unsigned t;
	--n;
	/*
	 outer loop range:
	 [0, n-1]
	 [0, n-2]
	 [0, n-3]
	 ...
	 ...
	 [0, 3]
	 [0, 2]
	 [0, 1]
	 */
	if (seq)
		while (n > 0) {
			// j---outer loop lower bound, n---outer loop upper bound
			j = 0;
			// j---------->n
			// when j == n-1, j+1 is not beyond the range's upper bound
			while (j < n) {
				if (ary[j] > ary[j + 1]) {
					t = ary[j];
					ary[j] = ary[j + 1];
					ary[j + 1] = t;
				}
				++j;
			}
			--n;
		}
	else
		while (n > 0) {
			j = 0;
			while (j < n) {
				if (ary[j] < ary[j + 1]) {
					t = ary[j];
					ary[j] = ary[j + 1];
					ary[j + 1] = t;
				}
				++j;
			}
			--n;
		}

}

// seq: asc---1, desc---0
void selectionSort(unsigned *ary, int n, int seq) {
	int i, max_i;
	unsigned t;
	/*
	 selection range:
	 [0,n-1]
	 [0,n-2]
	 [0,n-3]
	 ...
	 ...
	 [0,3]
	 [0,2]
	 [0,1]
	 */
	if (seq)
		while (n > 0) {
			// i---outer loop lower bound, n---outer loop upper bound
			i = 1;
			// max_i---assume the index of the initial max value is 0
			max_i = 0;
			// max_i vs. [1, n-1]
			while (i < n) {
				if (ary[max_i] < ary[i])
					max_i = i;
				++i;
			}
			--n;
			if (max_i != n) {
				t = ary[n];
				ary[n] = ary[max_i];
				ary[max_i] = t;
			}
		}
	else
		while (n > 0) {
			i = 1;
			max_i = 0;
			while (i < n) {
				if (ary[max_i] > ary[i])
					max_i = i;
				++i;
			}
			--n;
			if (max_i != n) {
				t = ary[n];
				ary[n] = ary[max_i];
				ary[max_i] = t;
			}
		}
}

// seq: asc---1, desc---0
void merge2way(unsigned *r, unsigned *ary1, unsigned *ary2, int n1, int n2,
	int seq) {
	int i1, i2, i;
	i1 = i2 = i = 0;
	if (seq)
		while ((i1 < n1) && (i2 < n2)) {
			if (ary1[i1] > ary2[i2]) {
				r[i] = ary2[i2];
				++i2;
			}
			else {
				r[i] = ary1[i1];
				++i1;
			}
			++i;
		}
	else
		while ((i1 < n1) && (i2 < n2)) {
			if (ary1[i1] > ary2[i2]) {
				r[i] = ary1[i1];
				++i1;
			}
			else {
				r[i] = ary2[i2];
				++i2;
			}
			++i;
		}
	while (i1 < n1) {
		r[i] = ary1[i1];
		++i1;
		++i;
	}
	while (i2 < n2) {
		r[i] = ary2[i2];
		++i2;
		++i;
	}
}

/*
 e.g.
 [45] [53] [19] [36] [77] [30] [48] [96] [15] [36]
 [45   53] [19   36] [30   77] [48   96] [15   36]
 [19   36   45   53] [30   48   77   96] [15   36]
 [19   30   36   45   48   53   77   96] [15   36]
 [15   19   30   36   36   45   48   53   77   96]
 */
// seq: asc---1, desc---0
void mergeSort2way(unsigned *ary, int n, int seq) {
	/*
	 turn:
	 0--the data stored in the original array,
	 the next action: orginal array to temp array;

	 1--the data stored in the temp array,
	 the next action: temp array to orginal array.
	 */
	int turn;
	int range_1way;
	int count;
	unsigned *temp;
	temp = malloc(sizeof(unsigned) * n);
	if (!temp)
		return;
	// the first action---orginal array to temp array
	turn = 0;
	// treat every element as a ordered array
	range_1way = 1;
	// if range_1way >= n, one way range includes all element in the array,
	// if range_1way < n, at least a merge2way could be executed
	while (range_1way < n) {
		if (turn) {
			count = 0;
			while (count < n) {
				// n - count < range_1way
				if (count + range_1way > n)
					merge2way(ary + count, temp + count, 0, n - count, 0, seq);
				// n - count < 2 * range_1way
				else if (count + 2 * range_1way > n)
					merge2way(ary + count, temp + count,
					temp + count + range_1way, range_1way,
					n - count - range_1way, seq);
				else
					merge2way(ary + count, temp + count,
					temp + count + range_1way, range_1way, range_1way, seq);
				count += range_1way * 2;
			}
			turn = 0;
		}
		else {
			count = 0;
			while (count < n) {
				if (count + range_1way > n)
					merge2way(temp + count, ary + count, 0, n - count, 0, seq);
				else if (count + 2 * range_1way > n)
					merge2way(temp + count, ary + count,
					ary + count + range_1way, range_1way,
					n - count - range_1way, seq);
				else
					merge2way(temp + count, ary + count,
					ary + count + range_1way, range_1way, range_1way, seq);
				count += range_1way * 2;
			}
			turn = 1;
		}
		range_1way <<= 1;
	}
	// if the data sorted in temp array
	if (turn)
		for (count = 0; count < n; ++count)
			ary[count] = temp[count];
	free(temp);
}

// seq: asc---1, desc---0
void quickSort(int *ary, int n, int seq) {
	int temp, i, j;
	// there is a element in the array, just return.
	if (n < 2)
		return;
	// there are two elements in the array, is the sequence right?
	// and finish the recursive invocation.
	if (n == 2) {
		if (seq) {
			if (ary[0] > ary[1]) {
				n = ary[0];
				ary[0] = ary[1];
				ary[1] = n;
			}
		}
		else {
			if (ary[0] < ary[1]) {
				n = ary[0];
				ary[0] = ary[1];
				ary[1] = n;
			}
		}
		return;
	}

	if (seq) {
		// when i >= j, partition is completed.
		for (i = 1, j = n - 1; i < j;) {
			while (ary[i] < ary[0])
				++i;
			while (ary[j] > ary[0])
				--j;
			// after the while loops above
			if (i < j) {
				temp = ary[i];
				ary[i] = ary[j];
				ary[j] = temp;
			}
		}
	}
	else {
		for (i = 1, j = n - 1; i < j;) {
			while (ary[i] > ary[0])
				++i;
			while (ary[j] < ary[0])
				--j;
			if (i < j) {
				temp = ary[i];
				ary[i] = ary[j];
				ary[j] = temp;
			}
		}
	}

	// exchange the pivot with the max index of the smaller part
	// when for loop is end, j <= i
	temp = ary[0];
	ary[0] = ary[j];
	ary[j] = temp;

	// [0, j-1]
	quickSort(&ary[0], j, seq);
	// n - j(length before j) - 1(j element self)
	quickSort(&ary[j + 1], n - j - 1, seq);
}

// seq: asc---1, desc---0
void insertSort(int *ary, int n, int seq) {
	int i;
	int b;
	int j;
	int temp;
	b = 1;
	if (seq) {
		// unsorted elements range: [1,n)
		while (b < n) {
			i = 0;
			do {
				if (ary[b] < ary[i])
					break;
				++i;
			}
			while (i < b);
			if (i != b) {
				j = b;
				temp = ary[j];
				--j;
				--i;
				do {
					ary[j + 1] = ary[j];
					j--;
				}
				while (j > i);
				ary[i + 1] = temp;
			}
			++b;
		}
	}
	else {
		while (b < n) {
			i = 0;
			do {
				if (ary[b] > ary[i])
					break;
				++i;
			}
			while (i < b);
			if (i != b) {
				j = b;
				temp = ary[j];
				--j;
				--i;
				do {
					ary[j + 1] = ary[j];
					j--;
				}
				while (j > i);
				ary[i + 1] = temp;
			}
			++b;
		}
	}
}

// seq: asc---1, desc---0
// m: max value in ary
void countSort(int *ary, int n, int m, int seq) {
	int *tA;
	int i;
	int j;
	++m;
	tA = malloc(sizeof(int) * m);
	memset(tA, 0, sizeof(int) * m);
	// the value of element as index, the occurrence count of element as value
	for (i = 0; i < n; ++i)
		++tA[ary[i]];
	if (seq)
		for (j = 0, i = 0; j < m; ++j)
			// if the occurrence number of element is not zero
			for (; tA[j]; --tA[j])
				ary[i++] = j;
	else
		for (j = m - 1, i = 0; j > -1; --j)
			for (; tA[j]; --tA[j])
				ary[i++] = j;
	free(tA);
}

#define RADIX_INDEX(a, i, w) (((a)[(i)] / QUICK_POWER10_INT((w))) % 10)

void radixSortAsc(int *ary, int n) {
	int i;
	// alt:
	// 0--original->alt,
	// 1--alt->original
	int alt;
	int weight;
	int *tA;
	int counts[10];
	weight = 0;
	alt = 0;
	tA = malloc(sizeof(int) * n);
	memset(tA, 0, sizeof(int) * n);
	while (1) {
		memset(counts, 0, sizeof(int)*10);
		if (alt)
			for (i = 0; i < n; ++i)
				++counts[RADIX_INDEX(tA, i, weight)];
		else
			for (i = 0; i < n; ++i)
				++counts[RADIX_INDEX(ary, i, weight)];
		// the most significant digit of all elements are zero
		if (counts[0] == n)
			break;
		// counts[ind] = how many elements before it + how many elements in it
		// counts[ind] is length
		for (i = 1; i < 10; ++i)
			counts[i] += counts[i - 1];
		if (alt)
			// traversing from back to front,
			// because the max index in counts[ind] can easily get by --,
			// and filling data from higher index to lower index.
			// the elements sorted are still sorted.
			// the sequence of higher significant digit may disorganize the sequence of lower significant digit,
			// but the sequence of elements without higher significant are still sorted.
			for (i = n - 1; i > -1; --i)
				ary[--counts[RADIX_INDEX(tA, i, weight)]] = tA[i];
		else
			for (i = n - 1; i > -1; --i)
				tA[--counts[RADIX_INDEX(ary, i, weight)]] = ary[i];
		alt = !alt;
		++weight;
	}
	if (alt)
		for (i = 0; i < n; ++i)
			ary[i] = tA[i];
	free(tA);
}

void radixSortDesc(int *ary, int n) {
	int i;
	// alt:
	// 0--original->alt,
	// 1--alt->original
	int alt;
	int weight;
	int *tA;
	int counts[10];
	weight = 0;
	alt = 0;
	tA = malloc(sizeof(int) * n);
	memset(tA, 0, sizeof(int) * n);
	while (1) {
		memset(counts, 0, sizeof(int)*10);
		if (alt)
			for (i = 0; i < n; ++i)
				++counts[9 - RADIX_INDEX(tA, i, weight)];
		else
			for (i = 0; i < n; ++i)
				++counts[9 - RADIX_INDEX(ary, i, weight)];
		// the most significant digit of all elements are zero
		if (counts[9] == n)
			break;
		// how many elements before it
		for (i = 1; i < 10; ++i)
			counts[i] += counts[i - 1];
		if (alt)
			for (i = n - 1; i > -1; --i)
				ary[--counts[9 - RADIX_INDEX(tA, i, weight)]] = tA[i];
		else
			for (i = n - 1; i > -1; --i)
				tA[--counts[9 - RADIX_INDEX(ary, i, weight)]] = ary[i];
		alt = !alt;
		++weight;
	}
	if (alt)
		for (i = 0; i < n; ++i)
			ary[i] = tA[i];
	free(tA);
}

void radixSort(int *ary, int n, int seq) {
	if (seq)
		radixSortAsc(ary, n);
	else
		radixSortDesc(ary, n);
}

#endif
