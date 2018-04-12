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


#include <t_queue_uint8.h>
#include <t_vector_int.h>
#include <t_graph.h>
#include <t_dictionary.h>

#include <t_inputs.h>
#include <t_lexer.h>
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
    T_Lexer();

    printf("通过了!");

    return 0;
}
