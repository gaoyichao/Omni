#include <t_vector_int.h>
#include <stdio.h>
#include <assert.h>

void T_vector_int(void) {
    printf("\n******************************\n");
    printf("    T_vector_int 测试开始\n");
    printf("******************************\n");

    vector_int vec1;
    vector_int_init(&vec1, 3);
    assert(3 == vector_int_size(&vec1));

    vector_int_destroy(&vec1);
    vector_int_init_value(&vec1, 3, 19, 18, 17, 16);
    assert(3 == vector_int_size(&vec1));
    assert(19 == VECTOR(vec1)[0]);
    assert(18 == VECTOR(vec1)[1]);
    assert(17 == VECTOR(vec1)[2]);

    printf("  T_vector_int 测试结束!\n");
    printf("******************************\n");
}
