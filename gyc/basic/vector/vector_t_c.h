/*
 * vector_t_h.h - 线性表模板,在头文件中include
 *
 * #define T int
 * #define TEMPLATE_TYPE(a) a ##_int
 * #define TEMPLATE_FUNCTION(a,b) a ##_int_## b
 * #include "vector_t_c.h"
 * #undef TEMPLATE_FUNCTION
 * #undef TEMPLATE_TYPE
 * #undef T
 */

#include <assert.h>
#include <stdarg.h>


#ifndef T
#error "必须声明线性表内容变量类型"
#endif

#ifndef TEMPLATE_TYPE
#error "必须声明宏TEMPLATE_TYPE(a)"
#endif

#ifndef TEMPLATE_FUNCTION
#error "必须声明宏TEMPLATE_FUNCTION(a,b)"
#endif

#include <mm.h>

/*
 * vector_T_init - 创建一个T类型的向量,成功创建则返回0.
 *
 * e.g. vector_int_init(&a,3);
 */
int TEMPLATE_FUNCTION(vector,init)(TEMPLATE_TYPE(vector) *v, int size)
{
    int alloc_size = size > 0 ? size :1;

	v->stor_begin = Calloc(alloc_size, T);
	assert(0 != v->stor_begin);

	v->stor_end = v->stor_begin + alloc_size;
	v->end = v->stor_begin + size;
	return 0;
}
/*
 * vector_T_init_copy - 创建一个T类型的向量, 并将@data中@length个元素拷贝到新向量中.
 */
int TEMPLATE_FUNCTION(vector, init_copy)(TEMPLATE_TYPE(vector) *v, T *data, int length)
{
    TEMPLATE_FUNCTION(vector,init)(v, length);
	memcpy(v->stor_begin, data, (size_t)length * sizeof(T));

	return 0;
}

/*
 * vector_T_copy_to - 将@v中的内容拷贝到缓存@to中
 *
 * 调用前需要保证@to已经分配了足够的内存
 */
void TEMPLATE_FUNCTION(vector, copy_to)(const TEMPLATE_TYPE(vector) *v, T *to)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	if (v->end != v->stor_begin)
		memcpy(to, v->stor_begin, sizeof(T) * (size_t)(v->end - v->stor_begin));
}

/*
 * vector_T_copy - 拷贝@from到@to
 *
 * 调用之前需要保证向量@to没有初始化
 */
int TEMPLATE_FUNCTION(vector, copy)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from)
{
	assert(NULL != from);
	assert(NULL != from->stor_begin);
	TEMPLATE_FUNCTION(vector, init)(to, TEMPLATE_FUNCTION(vector, size)(from));
	assert(0 != to->stor_begin);

	memcpy(to->stor_begin, from->stor_begin, (size_t)TEMPLATE_FUNCTION(vector, size)(from) * sizeof(T));
	return 0;
}

/*
 * vector_T_update - 更新@from到@to
 *
 * 若@to内存不够,将分配更多内存
 */
int TEMPLATE_FUNCTION(vector,update)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from)
{
	int n = TEMPLATE_FUNCTION(vector,size)(from);
	TEMPLATE_FUNCTION(vector,resize)(to, n);
	memcpy(to->stor_begin, from->stor_begin, sizeof(T)*n);
	return 0;
}

/*
 * vector_T_get_interval - 从@v中获取从@from到@to之间的数据内容,保存到缓存@res中
 */
int TEMPLATE_FUNCTION(vector,get_interval)(const TEMPLATE_TYPE(vector) *v,TEMPLATE_TYPE(vector) *res, int from, int to)
{
	TEMPLATE_FUNCTION(vector, resize)(res, to-from);
	memcpy(res->stor_begin, v->stor_begin+from, (size_t) (to-from) * sizeof(T));
	return 0;
}

/*
 * vector_T_destroy - 回收@v内存
 */
void TEMPLATE_FUNCTION(vector, destroy)(TEMPLATE_TYPE(vector) *v)
{
	assert(v != 0);
	if (v->stor_begin != 0) {
		Free(v->stor_begin);
		v->stor_begin = NULL;
		v->stor_end = NULL;
		v->end = NULL;
	}
}

/*
 * vector_T_contains_op - 查找元素@e
 *
 * @op: 元素比较器
 * 匹配则返回1,否则返回0
 */
int TEMPLATE_FUNCTION(vector, contains_op)(const TEMPLATE_TYPE(vector) *v, T e, int (*op)(const void*, const void*))
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	T *p = v->stor_begin;
	while (p < v->end) {
		if (0 == op(p,&e))
			return 1;
		p++;
	}
	return 0;
}

/*
 * vector_T_empty - 判定vector是否为空.
 *
 * 为空返回1,否则返回0.
 */
int TEMPLATE_FUNCTION(vector, empty)(const TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	return v->stor_begin == v->end;
}

/*
 * vector_T_size - 获取vector中元素数量
 */
int TEMPLATE_FUNCTION(vector, size)(const TEMPLATE_TYPE(vector)* v)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return v->end - v->stor_begin;
}

/*
 * vector_T_capacity - 获取vector的容量,分配内存大小
 */
int TEMPLATE_FUNCTION(vector, capacity)(const TEMPLATE_TYPE(vector) *v)
{
	return v->stor_end - v->stor_begin;
}

/*
 * vector_T_e - 获取@v中指定位置@pos的元素
 */
T TEMPLATE_FUNCTION(vector, e)(TEMPLATE_TYPE(vector) *v, int pos)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return *(v->stor_begin + pos);
}

/*
 * vector_T_tail - 获取@v中最后的元素
 */
T TEMPLATE_FUNCTION(vector, tail)(TEMPLATE_TYPE(vector) *v)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return *((v->end) - 1);
}

/*
 * vector_T_e_ptr - 获取@v中指定位置@pos的地址
 */
T* TEMPLATE_FUNCTION(vector, e_ptr)(TEMPLATE_TYPE(vector) *v, int pos)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return v->stor_begin + pos;
}

/*
 * vector_T_set - 为@v中指定位置@pos的元素赋值@value
 */
void TEMPLATE_FUNCTION(vector, set)(TEMPLATE_TYPE(vector) *v, int pos, T value)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	*(v->stor_begin + pos) = value;
}
/*
 * vector_T_null - 设定@v中所有元素全为0
 */
void TEMPLATE_FUNCTION(vector, null)(TEMPLATE_TYPE(vector) *v)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	if (TEMPLATE_FUNCTION(vector, size)(v) > 0)
		memset(v->stor_begin, 0, sizeof(T)*(size_t)TEMPLATE_FUNCTION(vector, size)(v));
}

/*
 * vector_T_fill - 用@e填充@v中所有元素
 */
void TEMPLATE_FUNCTION(vector, fill)(TEMPLATE_TYPE(vector) *v, T e)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	for (T* ptr = v->stor_begin; ptr < v->stor_end; ptr++)
		*ptr = e;
}

/*
 * vector_T_reserve - 分配更多的内存
 *
 * @v: 目标Vector
 * @size: 至少拥有内存数量
 */
int TEMPLATE_FUNCTION(vector, reserve)(TEMPLATE_TYPE(vector) *v, int size)
{
	int actual_size = TEMPLATE_FUNCTION(vector, size)(v);
	T *tmp;
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	if (size <= actual_size) return 0;

	tmp = Realloc(v->stor_begin, (size_t)size, T);
	assert(0 != v->stor_begin);
	v->stor_begin = tmp;
	v->stor_end = v->stor_begin + size;
	v->end = v->stor_begin + actual_size;

	return 0;
}

/*
 * vector_T_clear - 清空vector
 */
void TEMPLATE_FUNCTION(vector, clear)(TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	v->end = v->stor_begin;
}

/*
 * vector_T_resize - 重新分配内存
 */
void TEMPLATE_FUNCTION(vector, resize)(TEMPLATE_TYPE(vector) *v, int newsize)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	TEMPLATE_FUNCTION(vector, reserve)(v, newsize);
	v->end = v->stor_begin + newsize;
}
/*
 * vector_T_adjust - 调整@v的元素数量
 */
void TEMPLATE_FUNCTION(vector, adjust)(TEMPLATE_TYPE(vector) *v, int newsize)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	if (newsize > TEMPLATE_FUNCTION(vector,capacity)(v))
		TEMPLATE_FUNCTION(vector, reserve)(v, newsize * 2);

	v->end = v->stor_begin + newsize;
}

/*
 * vector_T_resize_min - 释放掉@v中没有使用的内存
 */
void TEMPLATE_FUNCTION(vector, resize_min)(TEMPLATE_TYPE(vector) *v)
{
	if (v->stor_end == v->end) return;

	size_t size = (size_t)(v->end - v->stor_begin);
	T *tmp = Realloc(v->stor_begin, size, T);
	assert(0 != tmp);

	v->stor_begin = tmp;
	v->stor_end = v->end = v->stor_begin + size;
}


/*
 * vector_T_remove_section - 从@v中移除@from到@to的内容,不包含@to
 */
void TEMPLATE_FUNCTION(vector, remove_section)(TEMPLATE_TYPE(vector) *v, int from, int to)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	assert(to < TEMPLATE_FUNCTION(vector, size)(v) + 1);
	assert(from <= to);
	memmove(v->stor_begin + from, v->stor_begin + to, sizeof(T) * (size_t)(v->end - v->stor_begin - to));
	v->end -= (to - from);
}

/*
 * vector_T_append - 将@from添加到@to之后
 */
int TEMPLATE_FUNCTION(vector, append)(const TEMPLATE_TYPE(vector) *from, TEMPLATE_TYPE(vector) *to)
{
	int fromsize = TEMPLATE_FUNCTION(vector, size)(from);
	int tosize = TEMPLATE_FUNCTION(vector, size)(to);

	TEMPLATE_FUNCTION(vector, resize)(to, tosize + fromsize);
	memcpy(to->stor_begin + tosize, from->stor_begin, sizeof(T) * (size_t)fromsize);
	to->end = to->stor_begin+tosize+fromsize;
	return 0;
}
/*
 * vector_T_move_interval - 把@v中从@begin到@end的内容移到@to开始的一段内存下
 */
int TEMPLATE_FUNCTION(vector,move_interval)(TEMPLATE_TYPE(vector) *v, int begin, int end, int to)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	memcpy(v->stor_begin+to, v->stor_begin+begin, sizeof(T) * (size_t) (end-begin));
	return 0;
}

/*
 * vector_T_new_item - 新建一个T对象
 */
T* TEMPLATE_FUNCTION(vector, new_item)(TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	/* full, allocate more storage */
	if (v->stor_end == v->end) {
		int new_size = TEMPLATE_FUNCTION(vector, size)(v) * 2;
		if (new_size == 0) new_size = 1;
		TEMPLATE_FUNCTION(vector, reserve)(v, new_size);
	}

	v->end += 1;
	return (v->end - 1);
}

/*
 * vector_T_insert - 在@pos位置插入一个T对象@value
 */
int TEMPLATE_FUNCTION(vector, insert)(TEMPLATE_TYPE(vector) *v, unsigned int pos, T value)
{
	size_t size = (size_t)TEMPLATE_FUNCTION(vector, size)(v);
	TEMPLATE_FUNCTION(vector, resize)(v, (long)size+1);
    if (pos < size)
		memmove(v->stor_begin+pos+1, v->stor_begin+pos, sizeof(T)*(size - (size_t)pos));
	v->stor_begin[pos] = value;
	return 0;
}

/*
 * vector_T_swap_elements - 交换@i和@j两个元素
 */
int TEMPLATE_FUNCTION(vector, swap_elements)(TEMPLATE_TYPE(vector) *v, int i, int j)
{
	T tmp = VECTOR(*v)[i];
	VECTOR(*v)[i] = VECTOR(*v)[j];
	VECTOR(*v)[j] = tmp;
	return 0;
}

/*
 * vector_T_push_back - 将T对象@e添加到@v末尾
 */
int TEMPLATE_FUNCTION(vector, push_back)(TEMPLATE_TYPE(vector) *v, T e)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	/* full, allocate more storage */
	if (v->stor_end == v->end) {
		int new_size = TEMPLATE_FUNCTION(vector, size)(v) * 2;
		if (new_size == 0) new_size = 1;
		TEMPLATE_FUNCTION(vector, reserve)(v, new_size);
	}

	*(v->end) = e;
	v->end += 1;

	return 0;
}

/*
 * vector_T_pop_back - 返回并移除@v中的最后一个元素
 */
T TEMPLATE_FUNCTION(vector, pop_back)(TEMPLATE_TYPE(vector) *v)
{
	T tmp;
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	assert(v->end != v->stor_begin);
	tmp = TEMPLATE_FUNCTION(vector, tail)(v);
	v->end -= 1;
	return tmp;
}

/*
 * vector_T_remove - 移除第i个元素
 */
void TEMPLATE_FUNCTION(vector, remove)(TEMPLATE_TYPE(vector) *v, int i)
{
    TEMPLATE_FUNCTION(vector, remove_section)(v, i, i + 1);
}
















