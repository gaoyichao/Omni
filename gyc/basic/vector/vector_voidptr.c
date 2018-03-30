#include <vector_voidptr.h>
#include <stdlib.h>
#include <assert.h>

#define VECTOR_NUMBER
#define T voidptr
#define TEMPLATE_TYPE(a) a ##_voidptr
#define TEMPLATE_FUNCTION(a,b) a ##_voidptr_## b
#include <vector/vector_t_c.h>
#include <vector/vector_t_num_c.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER
