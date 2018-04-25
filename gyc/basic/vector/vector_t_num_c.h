/*
 * vector_t_num_c.h - 线性表模板,在源文件中include
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

#include <assert.h>
#include <stdarg.h>


/*
 * vector_T_init_value - 以参数列表初始化一个Vector
 *
 * @v: 目标Vector
 * @no: 参数数量
 * @...: 参数列表
 *
 * e.g. vector_int_init_value(&b, 3, 1,2,3);
 */
int TEMPLATE_FUNCTION(vector,init_value)(TEMPLATE_TYPE(vector) *v, int no, ...)
{
    va_list ap;
    TEMPLATE_FUNCTION(vector,init)(v, no);

    va_start(ap, no);
    for (int i = 0; i < no; i++)
        VECTOR(*v)[i] = (T)va_arg(ap, T);
    va_end(ap);

    return 0;
}
/*
 * vector_T_init_value_end - 以参数列表初始化一个Vector.
 *
 * @v: 目标Vector
 * @endmark: 结束标志
 * @...: 参数列表
 *
 * 该函数与vector_T_init_value()类似,只是结束方式不同
 */
int TEMPLATE_FUNCTION(vector, init_value_end)(TEMPLATE_TYPE(vector) *v, T endmark, ...)
{
    int i = 0, n = 0;
    va_list ap;

    va_start(ap, endmark);
    while(1) {
        T num = (T) va_arg(ap, T);
        if (num == endmark)
            break;
        n++;
    }
    va_end(ap);

    TEMPLATE_FUNCTION(vector, init)(v, n);

    va_start(ap, endmark);
    for (i = 0; i < n; i++)
        VECTOR(*v)[i] = (T)va_arg(ap, T);
    va_end(ap);

    return 0;
}

/*
 * vector_T_contains - 判断Vector中是否包含元素@e
 *
 * @v: 目标Vector
 * @e: 期望元素
 * return: 0-不包含, 1-包含
 */
int TEMPLATE_FUNCTION(vector, contains)(const TEMPLATE_TYPE(vector) *v, T e)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    T *p = v->stor_begin;
    while (p < v->end) {
        if (*p == e)
            return 1;
        p++;
    }
    return 0;
}

/*
 * vector_T_isininterval - 判定Vector中的所有元素是否都在给定的区间中.
 *
 * @v: 目标Vector
 * @low: 区间下界(包含)
 * @high: 区间上界(包含)
 * return: 0-不在, 1-在
 */
int TEMPLATE_FUNCTION(vector, isininterval)(const TEMPLATE_TYPE(vector) *v, T low, T high)
{
    assert(NULL != v);
    assert(NULL != v->stor_begin);

    for(T* ptr = v->stor_begin; ptr < v->end; ptr++) {
        if (*ptr < low || *ptr > high)
            return 0;
    }
    return 1;
}

/*
 * vector_T_max - 找到Vector中的最大的元素
 *
 * @v: 目标Vector
 */
T TEMPLATE_FUNCTION(vector, max)(const TEMPLATE_TYPE(vector) *v)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    int n = TEMPLATE_FUNCTION(vector, size)(v);
    T max = VECTOR(*v)[0];
    for (int i = 1; i < n; i++) {
        if (VECTOR(*v)[i] > max)
            max = VECTOR(*v)[i];
    }
    return max;
}

/*
 * vector_T_which_max - 找到Vector中最大元素的索引
 */
int TEMPLATE_FUNCTION(vector, which_max)(const TEMPLATE_TYPE(vector) *v)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    int n = TEMPLATE_FUNCTION(vector, size)(v);
    int which = 0;
    for (int i = 1; i < n; i++) {
        T tmp = VECTOR(*v)[i];
        if (tmp > VECTOR(*v)[which])
            which = i;
    }
    return which;
}

/*
 * vector_T_min - 找到Vector中的最小元素
 */
T TEMPLATE_FUNCTION(vector, min)(const TEMPLATE_TYPE(vector) *v)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    int n = TEMPLATE_FUNCTION(vector, size)(v);
    T min = VECTOR(*v)[0];
    for (int i = 1; i < n; i++) {
        if (VECTOR(*v)[i] < min)
            min = VECTOR(*v)[i];
    }
    return min;
}

/*
 * vector_T_which_min - 找到Vector中最小元素的索引
 */
int TEMPLATE_FUNCTION(vector, which_min)(const TEMPLATE_TYPE(vector) *v)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    int n = TEMPLATE_FUNCTION(vector, size)(v);
    int which = 0;
    for (int i = 1; i < n; i++) {
        T tmp = VECTOR(*v)[i];
        if (tmp < VECTOR(*v)[which])
            which = i;
    }
    return which;
}
/*
 * vector_T_minmax - Vector中的最大值和最小值
 */
int TEMPLATE_FUNCTION(vector, minmax)(const TEMPLATE_TYPE(vector) *v, T *min, T *max)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    int n = TEMPLATE_FUNCTION(vector, size)(v);
    *min = *max = VECTOR(*v)[0];
    for (int i = 1; i < n; i++) {
        T tmp = VECTOR(*v)[i];
        if (tmp > *max)
            *max = tmp;
        else if (tmp < *min)
            *min = tmp;
    }
    return 0;
}
/*
 * vector_T_which_minmax - Vector中的最大值和最小值的索引
 */
int TEMPLATE_FUNCTION(vector, which_minmax)(const TEMPLATE_TYPE(vector) *v, int *which_min, int *which_max)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    int n = TEMPLATE_FUNCTION(vector, size)(v);
    *which_min = *which_max = 0;
    for (int i = 1; i < n; i++) {
        T tmp = VECTOR(*v)[i];
        if (tmp > VECTOR(*v)[*which_max])
            *which_max = i;
        else if (tmp < VECTOR(*v)[*which_min])
            *which_min = i;
    }
    return 0;
}
/*
 * vector_T_whereis - 获取元素@e的索引
 */
int TEMPLATE_FUNCTION(vector, whereis)(const TEMPLATE_TYPE(vector) *v, T e)
{
    assert(v != NULL);
    assert(v->stor_begin != NULL);

    T *p = v->stor_begin;
    while (p < v->end) {
        if (*p == e)
            return (p - v->stor_begin);
        p++;
    }

    return -1;
}



