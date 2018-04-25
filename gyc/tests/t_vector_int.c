#include <t_vector_int.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void T_vector_uint8(void) {
    printf("\n******************************\n");
    printf("    T_vector_uint8 测试开始\n");
    printf("******************************\n");

    vector_uint8 vec1;
    vector_uint8_init(&vec1, 3);
    assert(3 == vector_uint8_size(&vec1));

    char *str = "jiushizaidouniwannibuxinma";

    uint8 *str1 = vector_uint8_new_items(&vec1, 6);
    memcpy(str1, str, 5);
    str1[5] = '\0';
    int n = vector_uint8_size(&vec1);
    for (int i = 0; i < n; i++)
        printf("%c",VECTOR(vec1)[i]);
    printf("n = %d", n);


    printf("  T_vector_uint8 测试结束!\n");
    printf("******************************\n");

}



