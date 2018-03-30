/*
 * queue_t_h.h - 线性队列模板,在头文件中include
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
 * queue_T_init - 创建队列
 */
int TEMPLATE_FUNCTION(queue,init)(TEMPLATE_TYPE(queue) *q, int size)
{
    assert(0 != q);
    assert(size > 0);

    q->stor_begin = Calloc(size, T);
    assert(0 != q->stor_begin);

    q->stor_end = q->stor_begin + size;
    q->begin = q->stor_begin;
    q->end = NULL;

    return 0;
}

/*
 * queue_T_destroy - 回收队列内存
 */
void TEMPLATE_FUNCTION(queue,destroy)(TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->stor_begin);

    Free(q->stor_begin);
    q->stor_begin = 0;
    q->stor_end = 0;
    q->begin = 0;
    q->end = 0;
}

/*
 * queue_T_is_empty - 判定队列是否为空
 */
int TEMPLATE_FUNCTION(queue,is_empty)(const TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->stor_begin);
    return NULL == q->end;
}

/*
 * queue_T_is_full - 判定队列是否满
 */
int TEMPLATE_FUNCTION(queue,is_full)(const TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->stor_begin);
    return q->begin == q->end;
}

/*
 * queue_T_size - 获取队列长度
 */
int TEMPLATE_FUNCTION(queue,size)(const TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->stor_begin);

    if (NULL == q->end)
        return 0;
    else if (q->begin < q->end)
        return q->end - q->begin;
    else
        return (q->stor_end - q->begin) + (q->end - q->stor_begin);
}

/*
 * queue_T_capacity - 获取队列内存空间大小
 */
int TEMPLATE_FUNCTION(queue, capacity)(const TEMPLATE_TYPE(queue) *q) {
    return q->stor_end - q->stor_begin;
}

/*
 * queue_T_head - 获取队首元素
 */
T TEMPLATE_FUNCTION(queue,head)(const TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->stor_begin);
    return *(q->begin);
}

/*
 * queue_T_tail - 获取队尾元素
 */
T TEMPLATE_FUNCTION(queue,tail)(const TEMPLATE_TYPE(queue) *q)
{
    assert(q != 0);
    assert(q->stor_begin != 0);
    if (q->end == q->stor_begin)
        return *(q->stor_end-1);
    return *(q->end-1);
}

/*
 * queue_T_clear - 清空队列
 */
void TEMPLATE_FUNCTION(queue, clear)(TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->stor_begin);
    q->begin=q->stor_begin;
    q->end=NULL;
}

/*
 * queue_T_mm - 分配更多的内存
 *
 * @v: 目标队列
 * @size: 至少拥有内存数量
 */
int TEMPLATE_FUNCTION(queue, mm)(TEMPLATE_TYPE(queue) *q, int size) {
    assert(q != NULL);
    assert(q->stor_begin != NULL);

    int actual_size = TEMPLATE_FUNCTION(queue, size)(q);
    if (size <= actual_size) return 0;

    T *newmm = Calloc(size, T);
    assert(0 != newmm);
    if (NULL == q->end) {
        // 没有内容需要拷贝
    } else if (q->begin < q->end) {
        memcpy(newmm, q->begin, (size_t)(q->end - q->begin) * sizeof(T));
    } else {
        int head = q->stor_end - q->begin;
        int tail = q->end - q->stor_begin;
        if (0 != head)
            memcpy(newmm, q->begin, (size_t)head * sizeof(T));
        if (0 != tail)
            memcpy(newmm+head, q->stor_begin, (size_t)tail * sizeof(T));
    }
    Free(q->stor_begin);

    q->stor_end  = newmm + size;
    q->stor_begin = newmm;
    q->begin = newmm;
    q->end = (0 == actual_size) ? NULL : (newmm + actual_size);

    return 0;
}

/*
 * queue_T_enq - 进队操作,在队尾添加一个新元素
 */
int TEMPLATE_FUNCTION(queue,enq)(TEMPLATE_TYPE(queue) *q, T elem)
{
    assert(q != 0);
    assert(q->stor_begin != 0);

    if (q->begin != q->end) {
        // 队列不满
        if (q->end == NULL)
            q->end = q->begin;
        *(q->end) = elem;
        (q->end)++;
    } else {
        // 队列已满,则申请两倍内存
        int oldsize = q->stor_end - q->stor_begin;
        TEMPLATE_FUNCTION(queue, mm)(q, 2 * oldsize);
        *(q->end) = elem;
        (q->end)++;
    }

    // 调整队列边界
    if (q->end == q->stor_end)
        q->end = q->stor_begin;

    return 0;
}

/*
 * queue_T_deq - 出队操作,从队首取出一个元素
 */
T TEMPLATE_FUNCTION(queue,deq)(TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->begin);

    T re = *(q->begin);
    q->begin++;

    if (q->begin == q->stor_end)
        q->begin = q->stor_begin;
    if (q->begin == q->end)
        q->end = NULL;

    return re;
}

/*
 * queue_T_pop - 出栈操作,从队尾取出一个元素
 */
T TEMPLATE_FUNCTION(queue,pop)(TEMPLATE_TYPE(queue) *q)
{
    assert(0 != q);
    assert(0 != q->begin);

    T re;
    if (q->end == q->stor_begin) {
        re = *((q->stor_end) - 1);
        q->end = (q->stor_end) - 1;
    } else {
        re = *((q->end) - 1);
        q->end = (q->end) - 1;
    }

    if (q->begin == q->end)
        q->end=NULL;
    return re;
}
/*
 * queue_T_peek - 偷看队首开始的第i个元素
 */
T TEMPLATE_FUNCTION(queue,peek)(TEMPLATE_TYPE(queue) *q, int i) {
    assert(i < TEMPLATE_FUNCTION(queue, size)(q));

    T *offset = q->begin + i;
    if (offset < q->stor_end)
        return *offset;

    offset = q->stor_begin + (offset - q->stor_end);
    return *offset;
}

/*
 * queue_T_append - 在队尾添加一段元素
 *
 * @q: 目标队列
 * @buf: 元素缓存列表
 * @len: 添加元素数量
 */
int TEMPLATE_FUNCTION(queue, append)(TEMPLATE_TYPE(queue) *q, const T *buf, int len) {
    assert(q != 0);
    assert(q->stor_begin != 0);

    int csize = TEMPLATE_FUNCTION(queue, size)(q);
    int nsize = csize + len;

    TEMPLATE_FUNCTION(queue, mm)(q, 2 * nsize);
    if (NULL == q->end)
        q->end = q->begin;
    memcpy(q->end, buf, (size_t)len * sizeof(T));
    q->end += len;

    return 0;
}



