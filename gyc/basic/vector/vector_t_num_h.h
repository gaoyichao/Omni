/*
 * vector_t_num_h.h - 线性表模板,在源文件中include
 *
 * #define T int
 * #define TEMPLATE_TYPE(a) a ##_int
 * #define TEMPLATE_FUNCTION(a,b) a ##_int_## b
 * #include "vector_t_h.h"
 * #undef TEMPLATE_FUNCTION
 * #undef TEMPLATE_TYPE
 * #undef T
 */

#ifndef VECTOR_NUMBER
#error "宏VECTOR_NUMBER未定义."
#endif

/*
 * 初始化分配内存
 */
int TEMPLATE_FUNCTION(vector, init_value)(TEMPLATE_TYPE(vector) *v, int no, ...);
int TEMPLATE_FUNCTION(vector, init_value_end)(TEMPLATE_TYPE(vector) *v, T endmark, ...);

/*
 * 包含关系
 */
int TEMPLATE_FUNCTION(vector, contains)(const TEMPLATE_TYPE(vector) *v, T e);
int TEMPLATE_FUNCTION(vector, isininterval)(const TEMPLATE_TYPE(vector) *v, T low, T high);

/*
 * 寻找最大最小值
 */
T TEMPLATE_FUNCTION(vector, max)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, which_max)(const TEMPLATE_TYPE(vector) *v);
T TEMPLATE_FUNCTION(vector, min)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, which_min)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, minmax)(const TEMPLATE_TYPE(vector) *v, T *min, T *max);
int TEMPLATE_FUNCTION(vector, which_minmax)(const TEMPLATE_TYPE(vector) *v, int *which_min, int *which_max);
int TEMPLATE_FUNCTION(vector, whereis)(const TEMPLATE_TYPE(vector) *v, T e);



