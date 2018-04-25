#ifndef VECTOR_UINT8_H
#define VECTOR_UINT8_H

#include <types.h>

#define T uint8
#define TEMPLATE_TYPE(a) a ##_uint8
#define TEMPLATE_FUNCTION(a,b) a ##_uint8_## b
#include <vector_t_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T


#endif // VECTOR_UINT8_H
