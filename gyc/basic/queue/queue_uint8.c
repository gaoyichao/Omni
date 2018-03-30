#include <queue_uint8.h>

#define T uint8
#define TEMPLATE_TYPE(a) a ##_uint8
#define TEMPLATE_FUNCTION(a,b) a ##_uint8_## b
#include <queue_t_c.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

