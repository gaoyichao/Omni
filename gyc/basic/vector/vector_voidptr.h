#ifndef VECTOR_VOIDPTR_H
#define VECTOR_VOIDPTR_H

typedef void* voidptr;

#define VECTOR_NUMBER
#define T voidptr
#define TEMPLATE_TYPE(a) a ##_voidptr
#define TEMPLATE_FUNCTION(a,b) a ##_voidptr_## b
#include <vector/vector_t_h.h>
#include <vector/vector_t_num_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER

#endif // VECTOR_VOIDPTR_H
