#ifndef QUEUE_UINT8_H
#define QUEUE_UINT8_H

#include <types.h>

#define T uint8
#define TEMPLATE_TYPE(a) a ##_uint8
#define TEMPLATE_FUNCTION(a,b) a ##_uint8_## b
#include <queue_t_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#endif // QUEUE_UINT8_H
