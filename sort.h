#ifndef DS_C_SORT_H
#define DS_C_SORT_H

// order: asc---1, desc---0
void bubbleSort(unsigned *ary, int n, int order) {
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
	if (order)
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

// order: asc---1, desc---0
void selectionSort(unsigned *ary, int n, int order) {
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
	if (order)
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

// order: asc---1, desc---0
void merge2way(unsigned *r_ary, unsigned *ary1, unsigned *ary2, int len_ary1,
	int len_ary2, int order) {
	int i_ary1, i_ary2, i_r;
	i_ary1 = i_ary2 = i_r = 0;
	if (order)
		while ((i_ary1 < len_ary1) && (i_ary2 < len_ary2)) {
			if (ary1[i_ary1] > ary2[i_ary2]) {
				r_ary[i_r] = ary2[i_ary2];
				++i_ary2;
			}
			else {
				r_ary[i_r] = ary1[i_ary1];
				++i_ary1;
			}
			++i_r;
		}
	else
		while ((i_ary1 < len_ary1) && (i_ary2 < len_ary2)) {
			if (ary1[i_ary1] > ary2[i_ary2]) {
				r_ary[i_r] = ary1[i_ary1];
				++i_ary1;
			}
			else {
				r_ary[i_r] = ary2[i_ary2];
				++i_ary2;
			}
			++i_r;
		}
	while (i_ary1 < len_ary1) {
		r_ary[i_r] = ary1[i_ary1];
		++i_ary1;
		++i_r;
	}
	while (i_ary2 < len_ary2) {
		r_ary[i_r] = ary2[i_ary2];
		++i_ary2;
		++i_r;
	}
}

#endif
