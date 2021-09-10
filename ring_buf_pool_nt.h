#ifndef RING_ARRAY_NT_H
#define RING_ARRAY_NT_H

#include <string.h>

#ifndef LEN_ARY
#define LEN_ARY 4096
#endif
typedef struct ring_ary ring_ary;
struct ring_ary
{
	size_t b, e; /*[b, e)  */
	unsigned char buf[LEN_ARY];
};

/*
 * reset the ring_ary
 * Input:
 *      ary : ring_ary
 */
#define RESET_RA(ary)\
do {\
	(ary).b = (ary).e = 0;\
} while (0)

#define IS_EMPTY_RA(r, ary) r = (ary).b != (ary).e ? 0 : 1

/*
 * retrieve the remaining capacity of the ary,
 * and the length of segment1 ( if any ) and segment2 ( if any )
 * Input:
 *      ary : struct ring_ary
 * Output:
 *      capacity : size_t , the remaining total bytes
 *      seg1 : size_t
 *      seg2 : size_t
 */
#define CAPACITY_RA(capacity, seg1, seg2, ary)\
do {\
	if ((ary).e > (ary).b){\
		switch ((ary).b){\
			case 0:\
				(seg2) = 0;\
				(seg1) = LEN_ARY - (ary).e - 1;\
				break;\
			case 1:\
				(seg2) = 0;\
				(seg1) = LEN_ARY - (ary).e;\
				break;\
			default:\
				(seg2) = (ary).b - 1;\
				(seg1) = LEN_ARY - (ary).e;\
		}\
		(capacity) = (seg1) + (seg2);\
	}else if ((ary).e < (ary).b){\
		(capacity) = (seg1) = (ary).b - (ary).e - 1;\
		(seg2) = 0;\
	}else{\
		(capacity) = (seg1) = LEN_ARY - 1;\
		(seg2) = 0;\
	}\
} while (0)

/*
 * get information about the numbers of the bytes be occupied
 * Input:
 *      ra : struct ring_ary
 * Output:
 *      rl : size_t, the totol length of the bytes occupied
 *      l_s1 : size_t, the length of the 1th segment
 *      l_s2 : size_t, the length of the 2th segment(if any)
 */
#define SOLID_RA(rl, l_s1, l_s2, ra)\
do {\
	if ((ra).b > (ra).e){\
		switch ((ra).e){\
			case 0:\
				(rl) = (l_s1) = LEN_ARY - (ra).b;\
				(l_s2) = 0;\
				break;\
			case 1:\
				(l_s2) = 1;\
				(l_s1) = LEN_ARY - (ra).b;\
				(rl) = (l_s1) + (l_s2);\
				break;\
			default:\
				(l_s1) = LEN_ARY - (ra).b;\
				(l_s2) = (ra).e;\
				(rl) = (l_s1) + (l_s2);\
		}\
	}else{\
		(l_s2) = 0;\
		(rl) = (l_s1) = (ra).e - (ra).b;\
	}\
} while (0)

/*
 * load data ！！save data in the ring buffer
 * Input:
 *      ary : ring_ary
 *      base : void *
 *      len : size_t, the length of the data to be copied in bytes
 * Output:
 *      rl : size_t, the length of The remaining data
 */
#define LOAD_RA(rl, ary, base, len)\
do {\
	size_t l, s1l, s2l;\
	unsigned char *tb = (unsigned char *)(base);\
	CAPACITY_RA(l, s1l, s2l, (ary));\
	if ((len) < l){\
		(rl) = 0;\
		if ((len) < s1l){\
			memcpy(&((ary).buf[(ary).e]), tb, len);\
			(ary).e += (len);\
		}else if ((len) > s1l){\
			memcpy(&((ary).buf[(ary).e]), tb, s1l);\
			tb += s1l;\
			memcpy(&((ary).buf[0]), tb, ((len) - s1l));\
			(ary).e = ((len) - s1l);\
		}else{\
			memcpy(&(ary.buf[(ary).e]), tb, s1l);\
			(ary).e = 0;\
		}\
	}else{\
		(rl) = ((len) > l) ? ((len) - l) : 0;\
		if (s2l){\
			memcpy(&((ary).buf[(ary).e]), tb, s1l);\
			tb += s1l;\
			memcpy(&((ary).buf[0]), tb, s2l);\
			(ary).e = s2l;\
		}else{\
			memcpy(&((ary).buf[(ary).e]), tb, l);\
			(ary).e += s1l;\
		}\
	}\
	(ary).buf[(ary).e] = 0;\
} while (0)

/*
 * unload data ！！from the ring buffer, and change the b,e
 * Input:
 *      ary : ring_ary
 *      len : size_t, the length of the data to be unload (in bytes)
 * Output:
 *      seg1 : size_t, the 1th index of the first segment
 *      seg2 : size_t, the 2th index of the second segment
 *      len_seg1 : size_t, the length of the first segment
 *      len_seg2 : size_t, the length of the second segment
 */
#define UNLOAD_RA(seg1, seg2, len_seg1, len_seg2, ary, len)\
do {\
	size_t l, s1, s2;\
	SOLID_RA(l, s1, s2, (ary));\
	if ((ary).e < (ary).b){\
		if (l > (len)){\
			if (s1 > (len)){\
				(seg1) = (ary).b;\
				(len_seg1) = (len);\
				(len_seg2) = 0;\
				(seg2) = (ary).e;\
				(ary).b += (len);\
			}else if (s1 < (len)){\
				(seg1) = (ary).b;\
				(len_seg1) = s1;\
				(seg2) = 0;\
				(len_seg2) = (len) - s1;\
				(ary).b = (len_seg2);\
			}else{\
				(seg1) = (ary).b;\
				(len_seg1) = s1;\
				(seg2) = (ary).e;\
				(len_seg2) = 0;\
				(ary).b = 0;\
			}\
		}else{\
			(seg1) = (ary).b;\
			(len_seg1) = s1;\
			(seg2) = 0;\
			(len_seg2) = s2;\
			RESET_RA(ary);\
		}\
	}else{\
		if (l < (len)){\
			(seg2) = 0;\
			(len_seg2) = 0;\
			(seg1) = (ary).b;\
			(len_seg1) = (len);\
			RESET_RA(ary);\
		}else{\
			(len_seg2) = 0;\
			(seg2) = (ary).e;\
			(seg1) = (ary).b;\
			(len_seg1) = (len);\
			(ary).b += (len);\
		}\
	}\
} while (0)

#endif
