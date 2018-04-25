#include <t_dictionary.h>
#include <dictionary.h>
#include <assert.h>
#include <stdio.h>

void T_Dictionary(void) {
    printf("\n******************************\n");
    printf("    T_Dictionary 测试开始\n");
    printf("******************************\n");

    struct Dictionary *dic = CreateDictionary();
    assert(0 != dic);
    assert(0 != dic->tree);

    const char *origin1 = "abcdefghijklmnopqrstuvwxyz";
    const char *origin2 = "abcdefghijkl";
    const char *origin3 = "呜哈哈哈";
    int value1 = 12, value2 = 33, value3 = 44;

    DicPairPtr pair1 = DicInsertPair(dic, (uint8*)origin1, &value1);
    assert(0 != pair1);
    assert(0 != pair1->key);
    assert(0 == pair1->key->tree);
    assert('\0' == pair1->key->byte);
    assert(value1 == *((int*)(pair1->vptr)));
    assert(&value1 == pair1->vptr);
    assert(0 == pair1->key->index);
    assert(1 == vector_DicPairPtr_size(&(dic->pairs)));
    printf("pair1:id = %d, depth = %d\n", pair1->key->byte, pair1->key->depth);
    assert(0 == DicInsertPair(dic, (uint8*)origin1, &value1));

    DicPairPtr pair2 = DicInsertPair(dic, (uint8*)origin2, &value2);
    assert(0 != pair2);
    assert(0 != pair2->key);
    assert(0 == pair2->key->tree);
    assert(value2 == *((int*)(pair2->vptr)));
    assert(1 == pair2->key->index);
    assert(2 == vector_DicPairPtr_size(&(dic->pairs)));
    printf("pair2:id = %d, depth = %d\n", pair2->key->byte, pair2->key->depth);

    DicPairPtr pair3 = DicInsertPair(dic, (uint8*)origin3, &value3);
    assert(0 != pair3);
    assert(0 != pair3->key);
    assert(0 == pair3->key->tree);
    assert(value3 == *((int*)(pair3->vptr)));
    assert(2 == pair3->key->index);
    assert(3 == vector_DicPairPtr_size(&(dic->pairs)));
    printf("pair3:id = %d, depth = %d\n", pair3->key->byte, pair3->key->depth);

    DicPairPtr pair4 = DicGetPair(dic, (uint8*)origin1);
    assert(pair4 == pair1);
    assert('\0' == pair4->key->byte);
    assert(value1 == *((int*)DicGetVptr(dic, (uint8*)origin1)));
    DicPairPtr pair5 = DicGetPair(dic, (uint8*)origin2);
    assert(pair5 == pair2);
    assert(value2 == *((int*)DicGetVptr(dic, (uint8*)origin2)));
    DicPairPtr pair6 = DicGetPair(dic, (uint8*)("abc"));
    assert(0 == pair6);
    pair6 = DicGetPair2(dic, (uint8*)origin1, strlen(origin1));
    assert(pair6 == pair1);
    assert('\0' == pair6->key->byte);
    assert(value1 == *((int*)DicGetVptr2(dic, (uint8*)origin1, strlen(origin1))));

    uint8 *str1 = DicGetKey(pair3);
    printf("str1 = %s\n", str1);

    struct BinaryNode *node1 = _DicSearch(dic, (uint8*)origin1, strlen(origin1)+1);
    assert(node1->pValue == pair1->key);
    assert(node1 == pair1->key->current);
    uint8 *str2 = _DicTraceBackStr(node1);
    printf("str2 = %s\n", str2);

    DicDeletePair(dic, pair1);
    node1 = _DicSearch(dic, (uint8*)origin1, strlen(origin1)+1);
    assert(0 == node1);
    int size = vector_DicPairPtr_size(&(dic->pairs));
    for (int i = 0; i < size; i++)
        assert(i == VECTOR(dic->pairs)[i]->key->index);
    assert(2 == size);

    pair1 = DicInsertPair(dic, (uint8*)origin1, &value1);
    assert(0 != pair1);
    assert(0 != pair1->key);
    assert(0 == pair1->key->tree);
    assert(value1 == *((int*)(pair1->vptr)));
    assert(2 == pair1->key->index);
    assert(3 == vector_DicPairPtr_size(&(dic->pairs)));
    printf("pair1:id = %d, depth = %d\n", pair1->key->byte, pair1->key->depth);
    assert(0 == DicInsertPair(dic, (uint8*)origin1, &value1));

    DicDelete(dic, (uint8*)origin1);
    node1 = _DicSearch(dic, (uint8*)origin1, strlen(origin1)+1);
    assert(0 == node1);
    size = vector_DicPairPtr_size(&(dic->pairs));
    for (int i = 0; i < size; i++)
        assert(i == VECTOR(dic->pairs)[i]->key->index);
    assert(2 == size);

    DestroyDictionary(dic);

    printf("  T_Dictionary 测试结束!\n");
    printf("******************************\n");
}

