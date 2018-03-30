#include <assert.h>
#include <stdio.h>
#include <t_queue_uint8.h>

void T_queue_uint8(void) {
    queue_uint8 queue;

    printf("\n******************************\n");
    printf("    T_queue_uint8 测试开始\n");
    printf("******************************\n");

    printf("测试构造\n");
    queue_uint8_init(&queue, 1);
    assert(NULL != queue.begin);
    assert(queue.begin == queue.stor_begin);
    assert(0 == queue_uint8_size(&queue));
    assert(1 == queue_uint8_capacity(&queue));
    queue_uint8_destroy(&queue);

    printf("测试构造2\n");
    queue_uint8_init(&queue, 2);
    assert(NULL != queue.begin);
    assert(queue.begin == queue.stor_begin);
    assert(0 == queue_uint8_size(&queue));
    assert(2 == queue_uint8_capacity(&queue));
    queue_uint8_destroy(&queue);

    printf("测试enq\n");
    uint8 n = 128;
    queue_uint8_init(&queue, 1);
    for (uint8 i = 0; i < n; i++) {
        queue_uint8_enq(&queue, i);
    }
    assert(n == queue_uint8_size(&queue));
    for (uint8 i = 0; i < n; i++) {
        assert(i == queue_uint8_deq(&queue));
    }
    assert(0 == queue_uint8_size(&queue));

    for (uint8 i = 0; i < n; i++) {
        queue_uint8_enq(&queue, i);
    }
    assert(n == queue_uint8_size(&queue));
    for (uint8 i = n; i > 0; i--) {
        assert((i-1) == queue_uint8_pop(&queue));
    }
    assert(0 == queue_uint8_size(&queue));

    n = 10;
    uint8 *strs = "abcdefghijklmnopqrstuvwxyz";
    queue_uint8_append(&queue, strs, n);
    assert(n == queue_uint8_size(&queue));
    for (uint8 i = 0; i < n; i++) {
        assert(strs[i] == queue_uint8_deq(&queue));
    }
    assert(0 == queue_uint8_size(&queue));
    queue_uint8_destroy(&queue);

    printf("测试peek\n");
    queue_uint8_init(&queue, 12);
    n = 10;
    for (uint8 i = 0; i < n; i++) {
        queue_uint8_enq(&queue, i);
    }
    n = 5;
    for (uint8 i = 0; i < n; i++) {
        assert(i == queue_uint8_deq(&queue));
    }
    for (uint8 i = 0; i < n; i++) {
        queue_uint8_enq(&queue, i);
    }
    assert(12 == queue_uint8_capacity(&queue));
    for (uint8 i = 0; i < n; i++) {
        assert(i == queue_uint8_peek(&queue, i + n));
    }

    queue_uint8_destroy(&queue);

    printf("  T_queue_uint8 测试结束!\n");
    printf("******************************\n");
}
