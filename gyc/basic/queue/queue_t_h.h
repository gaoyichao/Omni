/*
 * queue_t_h.h - 线性队列模板,在头文件中include
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

typedef struct TEMPLATE_TYPE(queue)
{
    T* begin;
    T* end;
    T* stor_begin;
    T* stor_end;
} TEMPLATE_TYPE(queue);

/*
 * 创建和销毁队列
 */
int TEMPLATE_FUNCTION(queue,init)(TEMPLATE_TYPE(queue) *q, int size);
void TEMPLATE_FUNCTION(queue,destroy)(TEMPLATE_TYPE(queue) *q);

/*
 * 队列的各种状态
 */
int TEMPLATE_FUNCTION(queue,is_empty)(const TEMPLATE_TYPE(queue) *q);
int TEMPLATE_FUNCTION(queue,is_full)(const TEMPLATE_TYPE(queue) *q);
int TEMPLATE_FUNCTION(queue,size)(const TEMPLATE_TYPE(queue) *q);
int TEMPLATE_FUNCTION(queue, capacity)(const TEMPLATE_TYPE(queue) *q);

T TEMPLATE_FUNCTION(queue,head)(const TEMPLATE_TYPE(queue) *q);
T TEMPLATE_FUNCTION(queue,tail)(const TEMPLATE_TYPE(queue) *q);

/*
 * 各种元素操作
 */
void TEMPLATE_FUNCTION(queue,clear)(TEMPLATE_TYPE(queue) *q);
int TEMPLATE_FUNCTION(queue, mm)(TEMPLATE_TYPE(queue) *q, int size);
int TEMPLATE_FUNCTION(queue,enq)(TEMPLATE_TYPE(queue) *q, T elem);
T TEMPLATE_FUNCTION(queue,deq)(TEMPLATE_TYPE(queue) *q);
T TEMPLATE_FUNCTION(queue,pop)(TEMPLATE_TYPE(queue) *q);
T TEMPLATE_FUNCTION(queue,peek)(TEMPLATE_TYPE(queue) *q, int i);
int TEMPLATE_FUNCTION(queue, append)(TEMPLATE_TYPE(queue) *q, const T *buf, int len);

