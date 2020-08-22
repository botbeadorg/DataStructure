#ifndef DSCSORTH
#define DSCSORTH

void bubbleSortASC(unsigned *ary, int n) {
	int j;
	unsigned t;
	if (!ary)
		return;
	--n;
	/*
	 outer loop range:
	 [0, n-1]
	 [0, n-2]
	 [0, n-3]
	 ...
	 ...
	 ...
	 [0, 3]
	 [0, 2]
	 [0, 1]
	 */
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
}

void bubbleSortDESC(unsigned *ary, int n) {
	int i, j;
	unsigned t;
	if (!ary)
		return;
	j = 0;
	/*
	 outer loop range:
	 [0,n-1]
	 [1,n-1]
	 [2,n-1]
	 ...
	 ...
	 [n-4,n-1]
	 [n-3,n-1]
	 [n-2,n-1]
	 */
	--n;
	while (j < n) {
		// j---outer loop lower bound, i---outer loop upper bound
		i = n;
		// j<----------i
		// when i == j+1, i-1 is not beyond the range's lower bound
		while (i > j) {
			if (ary[i] > ary[i - 1]) {
				t = ary[i - 1];
				ary[i - 1] = ary[i];
				ary[i] = t;
			}
			--i;
		}
		++j;
	}
}

#endif
