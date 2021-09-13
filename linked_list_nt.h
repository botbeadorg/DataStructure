#ifndef SNGL_DBL_LINKED_LIST_WITH_HEAD_NT_H
#define SNGL_DBL_LINKED_LIST_WITH_HEAD_NT_H

#include <stdint.h>
#include <stdbool.h>

#pragma pack(1)

typedef struct value__t value__t;
struct value__t {
	union {
		union {
			bool b;
			char c;
			unsigned char uc;
			signed short s;
			unsigned short us;
			signed int i;
		} si;
		unsigned int ui;
		signed long sl;
		unsigned long ul;
		signed long long sll;
		unsigned long long ull;
		float f;
		double d;
		long double ld;
		float _Complex fc;
		double _Complex dc;
		long double _Complex ldc;
		char *ex;
	}v;
	size_t total_bytes_ex, unit_bytes_ex;
};

typedef struct linked_node__t linked_node__t;
struct linked_node__t {
	size_t count;
	linked_node__t *next, *last;
	value__t v;
};
#pragma pack()

enum link_node_member {
	n_next, n_load, n_last, node_other_member
};

typedef uintptr_t dbl_node[node_other_member];
typedef uintptr_t sngl_node[n_last];

typedef dbl_node dbl_link;
typedef dbl_node sngl_link;

/*
 * the initialization of dbl_link
 * Input:
 *      doubll : struct linked_node__t *
 */
#define reset_d_link(doubll)\
do {\
	(doubll)->v = (value__t){.v.ldc = 0, .total_bytes_ex = 0, .unit_bytes_ex = 0};\
	(doubll)->count = 0;\
	(doubll)->last = (doubll)->next = (doubll);\
} while (0)

/*
 * do a loop in the doubll according to the index of the nodes
 * Input:
 *      doubll : struct linked_node__t *, the head of the double linked list
 * Output:
 *      r : struct value__t *, the value part in the current dbl_node
 *      i : size_t, the index of the current dbl_node
 */
#define loop_d_link_i(r, i, doubll)\
do {\
	size_t ind = 0;\
	linked_node__t *pnt = (doubll)->next;\
	for (; ind < (doubll)->count; (pnt = pnt->next), (++ind)){\
		(r) = &(pnt->v);\
		(i) = ind;

/*
 * do a loop backward in the doubll according to the index of the nodes
 * Input:
 *      doubll : struct linked_node__t *, the head of the double linked list
 * Output:
 *      r : struct value__t *, the value part in the current dbl_node
 *      i : size_t, the index of the current dbl_node
 */
#define loop_d_link_i_backward(r, i, doubll)\
do {\
	size_t ind = 0;\
	linked_node__t *pnt = (doubll)->last;\
	for (; ind < (doubll)->count; (pnt = pnt->last), (++ind)){\
		(r) = &(pnt->v);\
		(i) = ind;

/*
 * do a loop in the doubll according to the addresses saved in the doubll
 * Input:
 *      doubll : struct linked_node__t *, the head of the double linked list
 * Output:
 *      r : struct value__t *, the value part in the current dbl_node
 *      i : size_t, the index of the current dbl_node
 */
#define loop_d_link_p(r, i, doubll)\
do {\
	size_t ind = 0;\
	linked_node__t *pnt = 0;\
	for (pnt = (doubll); pnt->next != (doubll); (pnt = pnt->next), (++ind)){\
		(r) = &(pnt->next->v);\
		(i) = ind;

/*
 * do a loop backward in the doubll according to the addresses saved in the doubll
 * Input:
 *      doubll : struct linked_node__t *, the head of the double linked list
 * Output:
 *      r : struct value__t *, the value part in the current dbl_node
 *      i : size_t, the index of the current dbl_node
 */
#define loop_d_link_p_backward(r, i, doubll)\
do {\
	size_t ind = 0;\
	linked_node__t *pnt = 0;\
	for (pnt = (doubll); pnt->last != (doubll); (pnt = pnt->last), (++ind)){\
		(r) = &(pnt->last->v);\
		(i) = ind;

/*
 *  the last four macros should end with the macro
 */
#define end_loop_d_link() }}while(0)

/*
 * add a new node at the end of the doubll
 * Input:
 *      doubll : struct linked_node__t *, the head of the double linked list
 *      node : struct linked_node__t *, the address of the new node
 *      load : struct value__t * , something be saved in the node
 */
#define add_d_node_end(node, load, doubll)\
do {\
	(node)->next = (doubll);\
	(node)->last = (doubll)->last;\
	(node)->v = *(load);\
	(doubll)->last = (doubll)->last->next = (node);\
	++((doubll)->count);\
} while (0)

/*
 * add a new node at the beginning of the doubll
 * Input:
 *      doubll : struct linked_node__t *, the head of the double linked list
 *      node : struct linked_node__t *, the address of the new node
 *      load : struct value__t *, something be saved in the node
 */
#define add_d_node_beginning(node, load, doubll)\
do {\
	(node)->last = (doubll);\
	(node)->next = (doubll)->next;\
	(node)->v = *(load);\
	(doubll)->next = (doubll)->next->last = (node);\
	++((doubll)->count);\
} while (0)

/*
 * find a node among the doubll's range
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, index
 * Output:
 *      node : struct linked_node__t *, the address of the node founded
 */
#define find_d_node1(node, doubll, i)\
do {\
	if ((!(doubll)->count) || ((i) > ((doubll)->count - 1))){\
		(node) = (doubll);\
	}else{\
		(node) = (doubll)->next;\
		for (size_t j = 0; j < (i); ((node) = (node)->next), (++j));\
	}\
} while (0)

/*
 * find a node backward among the doubll's range
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, index
 * Output:
 *      node : struct linked_node__t *, the address of the node founded
 */
#define find_d_node1_backward(node, doubll, i)\
do {\
	if ((!(doubll)->count) || ((i) > ((doubll)->count - 1))){\
		(node) = (doubll);\
	}else{\
		(node) = (doubll)->last;\
		for (size_t j = 0; j < (i); ((node) = (node)->last), (++j));\
	}\
} while (0)

/*
 * is doubll empty?
 * Input:
 *      doubll : struct linked_node__t *
 */
#define d_link_empty(doubll)\
	((doubll)->next == (doubll)) && ((doubll)->last == (doubll)) ? 1 : 0

/*
 * add a node among the doubll's range
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, index
 *      node : struct linked_node__t *, the address of the new node
 *      load : struct value__t *, something be saved in the node
 */
#define add_d_node1(node, load, doubll, i)\
do {\
	linked_node__t *p = 0;\
	find_d_node1(p, (doubll), (i));\
	if (p != (doubll)){\
		(node)->next = p;\
		(node)->last = p->last;\
		(node)->v = *(load);\
		p->last = p->last->next = (node);\
		++((doubll)->count);\
	}else{\
		add_d_node_end((node), (load), (doubll));\
	}\
} while (0)

/*
 * add a node backward among the doubll's range
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, index
 *      node : struct linked_node__t *, the address of the new node
 *      load : struct value__t *, something be saved in the node
 */
#define add_d_node1_backward(node, load, doubll, i)\
do {\
	linked_node__t *p = 0;\
	find_d_node1_backward(p, (doubll), (i));\
	if (p != (doubll)){\
		(node)->last = p;\
		(node)->next = p->next;\
		(node)->v = *(load);\
		p->next = p->next->last = (node);\
		++((doubll)->count);\
	}else{\
		add_d_node_beginning((node), (load), (doubll));\
	}\
} while (0)

/*
 * remove a node at the beginning of the doubll
 * Input:
 *      doubll : struct linked_node__t *
 * Output:
 *      node : struct linked_node__t *, the address of the node be removed
 */
#define sub_d_node_beginning(node, doubll)\
do {\
	size_t c = (doubll)->count;\
	if (c < 1){\
		(node) = (doubll);\
	}else if (c > 1){\
		(node) = (doubll)->next;\
		(node)->next->last = (doubll);\
		(doubll)->next = (node)->next;\
		--((doubll)->count);\
	}else{\
		(node) = (doubll)->next;\
		reset_d_link((doubll));\
	}\
} while (0)

/*
 * remove a node at the end of the doubll
 * Input:
 *      doubll : struct linked_node__t *
 * Output:
 *      node : struct linked_node__t *, the address of the node be removed
 */
#define sub_d_node_end(node, doubll)\
do {\
	size_t c = (doubll)->count;\
	if (c < 1){\
		(node) = (doubll);\
	}else if (c > 1){\
		(node) = (doubll)->last;\
		(node)->last->next = (doubll);\
		(doubll)->last = (node)->last;\
		--((doubll)->count);\
	}else{\
		(node) = (doubll)->last;\
		reset_d_link((doubll));\
	}\
} while (0)

/*
 * remove a node at the specified index
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, index
 * Output:
 *      node : struct linked_node__t *, the address of the node be removed
 */
#define sub_d_node1(node, doubll, i)\
do {\
	find_d_node1((node), (doubll), (i));\
	if ((node) != (doubll)){\
		(node)->next->last = (node)->last;\
		(node)->last->next = (node)->next;\
		--((doubll)->count);\
	}\
} while (0)

/*
 * remove a node backward at the specified index
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, index
 * Output:
 *      node : struct linked_node__t *, the address of the node be removed
 */
#define sub_d_node1_backward(node, doubll, i)\
do {\
	find_d_node1_backward((node), (doubll), (i));\
	if ((node) != (doubll)){\
		(node)->next->last = (node)->last;\
		(node)->last->next = (node)->next;\
		--((doubll)->count);\
	}\
} while (0)

/*
 * do a loop in the doubll from a node at the specified index
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, the specified index, (0, max index]
 * Output:
 *      r : struct value__t *, the value part in the current dbl_node
 *      j : size_t, the index of the current dbl_node
 */
#define loop_d_link_from(r, j, i, doubll)\
do {\
	linked_node__t *p = 0;\
	linked_node__t *t_p = 0;\
	size_t i_f = (i);\
	find_d_node1(p, (doubll), i_f);\
	if (p != (doubll)) t_p = p;\
	else break;\
	do {\
		if ((doubll) != t_p){\
			(j) = i_f;\
			(r) = &(t_p->v);\

/*
 * the last macro should end with the macro
 * Input:
 *      doubll : struct linked_node__t *
 */
#define end_loop_d_link_from(doubll) \
            i_f = (i_f + 1) % (doubll)->count; \
            t_p = t_p->next; \
        }else \
            t_p = t_p->next; \
    }while(t_p != p); \
}while(0)

/*
 * do a loop backward in the doubll from a node at the specified index
 * Input:
 *      doubll : struct linked_node__t *
 *      i : size_t, the specified index, (0, max index]
 * Output:
 *      r : struct value__t *, the value part in the current dbl_node
 *      j : size_t, the index of the current dbl_node
 */
#define loop_d_link_from_backward(r, j, i, doubll)\
do {\
	linked_node__t *p = 0;\
	linked_node__t *t_p = 0;\
	size_t i_f = (i);\
	find_d_node1_backward(p, (doubll), i_f);\
	if (p != (doubll)) t_p = p;\
	else break;\
	do {\
		if ((doubll) != t_p){\
			(j) = i_f;\
			(r) = &(t_p->v);\

/*
 * the last macro should end with the macro
 * Input:
 *      doubll : struct linked_node__t *
 */
#define end_loop_d_link_from_backward(doubll) \
            i_f = (i_f + 1) % (doubll)->count; \
            t_p = t_p->last; \
        }else \
            t_p = t_p->last; \
    }while(t_p != p); \
}while(0)

/*
 * the initialization of sngl_link
 * Input:
 *      snglll : struct linked_node__t *
 */
#define reset_s_link(snglll)\
do {\
	(snglll)->last = (snglll)->next = 0;\
	(snglll)->count = 0;\
} while (0)

/*
 * is snglll empty?
 * Input:
 *      snglll : struct linked_node__t *
 */
#define s_link_empty(snglll) (snglll)->count ? 0 : 1

/*
 * add a new node at the end of the sngl_link
 * Input:
 *      snglll : struct linked_node__t *
 *      node : struct linked_node__t *
 *      load : struct value__t * , something be saved in the node
 */
#define add_s_node_end(node, load, snglll)\
do {\
	(node)->next = 0;\
	(node)->v = *(load);\
	if ((snglll)->count)\
		(snglll)->last = (snglll)->last->next = (node);\
	else\
		(snglll)->last = (snglll)->next = (node);\
	++((snglll)->count);\
} while (0)

/*
 * add a new node at the beginning of the sngl_link
 * Input:
 *      snglll : struct linked_node__t *
 *      node : struct linked_node__t *
 *      load : struct value__t * , something be saved in the node
 */
#define add_s_node_beginning(node, load, snglll)\
do {\
	(node)->v = *(load);\
	(node)->next = (snglll)->next;\
	(snglll)->next = (node);\
	if ((snglll)->count);else\
		(snglll)->last = (node);\
	++((snglll)->count);\
} while (0)

/*
 * find a node among the snglll's range
 * Input:
 *      snglll : struct linked_node__t *
 *      i : size_t, index
 * Output:
 *      node : struct linked_node__t *, the address of the node founded
 */
#define find_s_node1(node, snglll, i)\
do {\
	linked_node__t *t_p = (snglll);\
	for (size_t j = 0; (j < (i)) && (t_p->next); (t_p = t_p->next), (++j));\
	(node) = t_p;\
} while (0)

/*
 * do a loop in the snglll according to the pointers in a node
 * Input:
 *      snglll : struct linked_node__t *
 * Output:
 *      r : struct value__t *, v part in the current linked_node__t
 *      i : size_t, the index of the current linked_node__t
 */
#define loop_s_link(r, i, snglll);\
do {\
	linked_node__t * pnt;\
	size_t ind = 0;\
	for (pnt = (snglll); pnt->next; (pnt = pnt->next), (++ind)){\
		(r) = &(pnt->next->v);\
		(i) = ind;

/*
 *  end the loop in a sngl_link
 */
#define end_loop_s_link() }}while(0)

/*
 * add a node among the snglll's range
 * Input:
 *      snglll : struct linked_node__t *
 *      i : size_t, index
 *      node : struct linked_node__t *
 *      load : struct value__t *, something be saved in the node
 */
#define add_s_node1(node, load, snglll, i)\
do {\
	linked_node__t *p = 0;\
	find_s_node1(p, (snglll), (i));\
	(node)->next = p->next;\
	(node)->v = *(load);\
	p->next = (node);\
	++((snglll)->count);\
} while (0)

/*
 * remove a node at the end of the sngl_link
 * Input:
 *      snglll : struct linked_node__t *
 * Output:
 *      node : struct linked_node__t *, the address of the node be removed
 */
#define sub_s_node_end(node, snglll)\
do {\
	size_t c = (snglll)->count;\
	if (c < 1){\
		(node) = 0;\
	}else if (c > 1){\
		linked_node__t *p = 0;\
		(node) = (snglll)->last;\
		find_s_node1(p, (snglll), (c - 1));\
		p->next = 0;\
		(snglll)->last = p;\
		--((snglll)->count);\
	}else{\
		(node) = (snglll)->last;\
		reset_s_link((snglll));\
	}\
} while (0)

/*
 * remove a node at the beginning of the snglll
 * Input:
 *      snglll : struct linked_node__t *
 * Output:
 *      node : struct linked_node__t *, the address of the node be removed
 */
#define sub_s_node_beginning(node, snglll)\
do {\
	size_t c = (snglll)->count;\
	if (c < 1){\
		(node) = 0;\
	}else if (c > 1){\
		(node) = snglll->next;\
		(snglll)->next = (node)->next;\
		--((snglll)->count);\
	}else{\
		(node) = snglll->next;\
		reset_s_link((snglll));\
	}\
} while (0)

/*
 * remove a node at the specified index
 * Input:
 *      snglll : struct linked_node__t *
 *      i : size_t, index
 * Output:
 *      node : struct linked_node__t *, the address of the node be removed
 */

#define sub_s_node1(node, snglll, i)\
do {\
	linked_node__t *p = 0;\
	if ((i) > ((snglll)->count - 1));\
	else{\
		find_s_node1(p, (snglll), (i));\
		(node) = p->next;\
		p->next = (node)->n_next;\
		--((snglll)->count);\
	}\
} while (0)

#endif
