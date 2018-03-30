#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdlib.h>
#include <string.h>

struct BinaryNode {
    void *pValue;
    struct BinaryNode *pParent;
    struct BinaryNode *pLeft;
    struct BinaryNode *pRight;
};
typedef struct BinaryNode* BinaryNodePtr;

/*
 * BinaryNodeValue - 获取二叉树结点的数值
 *
 * @type: 数据类型
 * @pnode: 节点指针
 */
#define BinaryNodeValue(type, pnode) (*((type*)((pnode)->pValue)))
/*
 * CreateBinaryNode - 创建二叉树结点
 *
 * @value: 指向一个有数值的对象
 * @size: value大小
 */
struct BinaryNode *CreateBinaryNode(const void *value, size_t size);
/*
 * DestroyBinaryNode - 回收节点内存
 * 只有当没有子节点时才可以调用该函数
 */
void DestroyBinaryNode(struct BinaryNode *node);
/*************************************************/

/*
 * 若a>b返回正数,若a<b返回负数,若a==b返回0
 */
typedef int (*CompareFunc)(const void *a, const void *b);
typedef void (*PrintFunc)(const void *a);

struct BinaryTree {
    struct BinaryNode *root;

    CompareFunc compare;
    PrintFunc print;
};
/*
 * CreateBinaryTree - 创建二叉树
 *
 * @compFunc: 比较函数指针
 */
struct BinaryTree *CreateBinaryTree(CompareFunc compFunc);

/*
 * BinaryTreeSubTreeCount - 计数子树节点数量
 *
 * @root: 子树根节点
 */
int BinaryTreeSubTreeCount(const struct BinaryNode *root);
int BinaryTreeCount(const struct BinaryTree *tree);
/*
 * BinaryTreeFindValue - 查找数值
 */
struct BinaryNode *BT_FindValue(struct BinaryTree *ptree, const void *value);
/*
 * BinaryTreeGetMaximum - 获取子树中最大值的节点
 */
struct BinaryNode *BT_GetMaximum(struct BinaryNode *x);
/*
 * BinaryTreeGetMaximum - 获取子树中最小值的节点
 */
struct BinaryNode *BT_GetMinimum(struct BinaryNode *x);
/*
 * BinaryTreeGetSuccessor - 获取给定节点的后继
 */
struct BinaryNode *BT_GetSuccessor(const struct BinaryTree *T, struct BinaryNode *x);
/*
 * BinaryTreeGetSuccessor - 获取给定节点的前驱
 */
struct BinaryNode *BT_GetPredecessor(const struct BinaryTree *T, struct BinaryNode *x);

/*
 * BinaryTreeInsert - 插入数值
 */
struct BinaryNode *BT_Insert(struct BinaryTree *ptree, const void *value, int size);
/*
 * BT_Remove - 从二叉树中删除节点@z
 */
void BT_Remove(struct BinaryTree *T, struct BinaryNode *z);
/*
 * BT_RemoveAndDestroy - 从二叉树中移除节点@z并释放内存
 */
void BT_RemoveAndDestroy(struct BinaryTree *T, struct BinaryNode *z);
/*
 * DestroySubBinaryTree - 回收子树内存
 */
void DestroySubBinaryTree(struct BinaryNode *root);
/*
 * DestroyBinaryTree - 回收二叉树内存
 */
void DestroyBinaryTree(struct BinaryTree *T);


/*
 * InOrderPrintBinaryTree - 按顺序打印
 */
void InOrderPrintBinaryTree(const struct BinaryTree *T, const struct BinaryNode *x);

#endif // BINARY_TREE_H
