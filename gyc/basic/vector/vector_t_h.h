/*
 * vector_t_h.h - 线性表模板,在头文件中include
 *
 * #define T int
 * #define TEMPLATE_TYPE(a) a ##_int
 * #define TEMPLATE_FUNCTION(a,b) a ##_int_## b
 * #include "vector_t_h.h"
 * #undef TEMPLATE_FUNCTION
 * #undef TEMPLATE_TYPE
 * #undef T
 */

#ifndef T
#error "必须声明线性表内容变量类型"
#endif

#ifndef TEMPLATE_TYPE
#error "必须声明宏TEMPLATE_TYPE(a)"
#endif

#ifndef TEMPLATE_FUNCTION
#error "必须声明宏TEMPLATE_FUNCTION(a,b)"
#endif

typedef struct TEMPLATE_TYPE(vector)
{
    T* stor_begin;
    T* stor_end;
    T* end;
} TEMPLATE_TYPE(vector);

/*
 * 申请内存 vector_T_init
 */
int TEMPLATE_FUNCTION(vector, init)(TEMPLATE_TYPE(vector) *v, int size);
int TEMPLATE_FUNCTION(vector, init_copy)(TEMPLATE_TYPE(vector) *v, T *data, int length);
void TEMPLATE_FUNCTION(vector, destroy)(TEMPLATE_TYPE(vector) *v);
/*
 * 填充内存
 */
void TEMPLATE_FUNCTION(vector, null)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, fill)(TEMPLATE_TYPE(vector) *v, T e);
/*
 * 深度拷贝
 */
void TEMPLATE_FUNCTION(vector, copy_to)(const TEMPLATE_TYPE(vector) *v, T *to);
int TEMPLATE_FUNCTION(vector, copy)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from);
int TEMPLATE_FUNCTION(vector,update)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from);
int TEMPLATE_FUNCTION(vector,get_interval)(const TEMPLATE_TYPE(vector) *v,TEMPLATE_TYPE(vector) *res, int from, int to);
/*
 * Vector属性
 */
int TEMPLATE_FUNCTION(vector, contains_op)(const TEMPLATE_TYPE(vector) *v, T e, int (*op)(const void*, const void*));
int TEMPLATE_FUNCTION(vector, empty)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, full)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, size)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, capacity)(const TEMPLATE_TYPE(vector) *v);

/*
 * 访问Vector元素
 */
#ifndef VECTOR
#define VECTOR(a) ((a).stor_begin)
#endif
T TEMPLATE_FUNCTION(vector, e)(TEMPLATE_TYPE(vector) *v, int pos);
T TEMPLATE_FUNCTION(vector, tail)(TEMPLATE_TYPE(vector) *v);
T* TEMPLATE_FUNCTION(vector, e_ptr)(TEMPLATE_TYPE(vector) *v, int pos);
void TEMPLATE_FUNCTION(vector, set)(TEMPLATE_TYPE(vector) *v, int pos, T value);
int TEMPLATE_FUNCTION(vector, offset)(TEMPLATE_TYPE(vector) *v, T *a);
/*
 * 各种resize
 */
int TEMPLATE_FUNCTION(vector, reserve)(TEMPLATE_TYPE(vector) *v, int size);
void TEMPLATE_FUNCTION(vector, resize)(TEMPLATE_TYPE(vector) *v, int newsize);
void TEMPLATE_FUNCTION(vector, resize_min)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, adjust)(TEMPLATE_TYPE(vector) *v, int newsize);
/*
 * 添加和删除元素
 */
void TEMPLATE_FUNCTION(vector, remove_section)(TEMPLATE_TYPE(vector) *v, int from, int to);
int TEMPLATE_FUNCTION(vector, append)(const TEMPLATE_TYPE(vector) *from, TEMPLATE_TYPE(vector) *to);
int TEMPLATE_FUNCTION(vector,move_interval)(TEMPLATE_TYPE(vector) *v, int begin, int end, int to);

T* TEMPLATE_FUNCTION(vector, new_item)(TEMPLATE_TYPE(vector) *v);
T* TEMPLATE_FUNCTION(vector, new_items)(TEMPLATE_TYPE(vector) *v, int n);
int TEMPLATE_FUNCTION(vector, insert)(TEMPLATE_TYPE(vector) *v, unsigned int pos, T value);
int TEMPLATE_FUNCTION(vector, swap_elements)(TEMPLATE_TYPE(vector) *v, int i, int j);
int TEMPLATE_FUNCTION(vector, push_back)(TEMPLATE_TYPE(vector) *v, T e);
T TEMPLATE_FUNCTION(vector, pop_back)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, remove)(TEMPLATE_TYPE(vector) *v, int i);

void TEMPLATE_FUNCTION(vector, clear)(TEMPLATE_TYPE(vector) *v);







