#include <stdio.h>
#include <binary_tree.h>
#include <dictionary.h>
#include <assert.h>

void T_CreateBinaryNode(void) {
    int a = 349;
    struct BinaryNode *node = CreateBinaryNode(&a, sizeof(int));
    assert(0 != node);
    assert(0 != node->pValue);
    assert(a == BinaryNodeValue(int, node));
    assert(0 == node->pLeft);
    assert(0 == node->pRight);
    assert(0 == node->pParent);
    DestroyBinaryNode(node);
}

/*****************************************/

int int_compare(const void *a, const void *b) {
    int _a = *((int*)a);
    int _b = *((int*)b);
    return _a - _b;
}

void int_print(const void *a) {
    printf("%d", *((int*)a));
}

void T_CreateBinaryTree(void) {
    struct BinaryTree *tree = CreateBinaryTree(int_compare);
    assert(0 != tree);
    assert(0 == tree->root);
    assert(int_compare == tree->compare);
}

void T_BinaryTreeInsert(void) {
    struct BinaryTree *tree = CreateBinaryTree(int_compare);
    assert(0 != tree);
    assert(0 == tree->root);
    assert(int_compare == tree->compare);
    tree->print = int_print;

    int data = 255;
    struct BinaryNode *node = BT_Insert(tree, &data, sizeof(data));
    assert(255 == BinaryNodeValue(int, node));

    struct BinaryNode *node1 = BT_Insert(tree, &data, sizeof(data));
    assert(node == node1);

    data = 121;
    node1 = BT_Insert(tree, &data, sizeof(data));
    assert(121 == BinaryNodeValue(int, node1));
    data = 255;
    node1 = BT_FindValue(tree, &data);
    assert(node == node1);

    node = BT_GetMaximum(tree->root);
    assert(255 == BinaryNodeValue(int, node));
    node = BT_GetMinimum(tree->root);
    assert(121 == BinaryNodeValue(int, node));

    node1 = BT_GetSuccessor(tree, node);
    assert(255 == BinaryNodeValue(int, node1));
    node = BT_GetPredecessor(tree, node1);
    assert(121 == BinaryNodeValue(int, node));

    InOrderPrintBinaryTree(tree, tree->root);

    BT_RemoveAndDestroy(tree, node);
    InOrderPrintBinaryTree(tree, tree->root);

    data = 213;
    node = BT_Insert(tree, &data, sizeof(data));
    assert(213 == BinaryNodeValue(int, node));

    DestroyBinaryTree(tree);
}

void T_Dictionary(void) {
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
    assert(value1 == *((int*)(pair1->vptr)));
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
    assert(value1 == *((int*)DicGetVptr(dic, (uint8*)origin1)));
    DicPairPtr pair5 = DicGetPair(dic, (uint8*)origin2);
    assert(pair5 == pair2);
    assert(value2 == *((int*)DicGetVptr(dic, (uint8*)origin2)));
    DicPairPtr pair6 = DicGetPair(dic, (uint8*)("abc"));
    assert(0 == pair6);

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
}

#include <t_queue_uint8.h>
#include <t_vector_int.h>
#include <t_graph.h>

#include <t_inputs.h>
#include <stdio.h>

int main(void)
{
    T_CreateBinaryNode();
    T_BinaryTreeInsert();
    T_Dictionary();

    T_queue_uint8();
    T_vector_int();

    T_graph();
    T_graph_weight();

    T_Inputs();

    printf("通过了!");

    return 0;
}
