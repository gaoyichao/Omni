#ifndef VECTOR_INT_H
#define VECTOR_INT_H

#include <stdio.h>

#define VECTOR_NUMBER
#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include <vector/vector_t_h.h>
#include <vector/vector_t_num_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER

void print_vector_int(const vector_int *v, FILE *f);
void print_vector_int3(const vector_int *v, char c, FILE *f);
int vector_int_scan_tie(vector_int *res, vector_int *el, vector_int *il, int max);
int vector_int_order_inc(const vector_int *v, vector_int *ret, int max);
int vector_int_order_dec(const vector_int *v, vector_int *ret, int max);
int vector_int_order_inc2(vector_int *v1, vector_int *v2, vector_int *ret, int max);
int vector_int_ele_freq_min_max(vector_int *dis, const vector_int *v, int *min, int *max);
int vector_int_rank(vector_int *v);

#endif // VECTOR_INT_H
