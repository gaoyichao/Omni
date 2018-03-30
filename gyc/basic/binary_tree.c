#include <binary_tree.h>
#include <assert.h>

/*
 * CreateBinaryNode - 创建二叉树结点
 *
 * @value: 指向一个有数值的对象
 * @size: value大小
 */
struct BinaryNode *CreateBinaryNode(const void *value, size_t size) {
    struct BinaryNode *node;
    node = (struct BinaryNode*)malloc(sizeof(struct BinaryNode));

    memset(node, 0, sizeof(struct BinaryNode));
    node->pValue = malloc(size);
    memcpy(node->pValue, value, size);
    return node;
}

/*
 * DestroyBinaryNode - 回收节点内存
 * 只有当没有子节点时才可以调用该函数
 */
void DestroyBinaryNode(struct BinaryNode *node) {
    assert(0 == node->pLeft);
    assert(0 == node->pRight);
    free(node->pValue);
    free(node);
}

/*************************************************/

/*
 * CreateBinaryTree - 创建二叉树
 *
 * @compFunc: 比较函数指针
 */
struct BinaryTree *CreateBinaryTree(CompareFunc compFunc) {
    struct BinaryTree *tree;
    tree = (struct BinaryTree*)malloc(sizeof(struct BinaryTree));

    if (0 == tree)
        return 0;

    memset(tree, 0, sizeof(struct BinaryTree));
    tree->compare = compFunc;
    return tree;
}
/*
 * BinaryTreeSubTreeCount - 计数子树节点数量
 *
 * @root: 子树根节点
 */
int BinaryTreeSubTreeCount(const struct BinaryNode *root) {
    if (0 == root)
        return 0;
    int count = 1;
    if (0 != root->pLeft)
        count += BinaryTreeSubTreeCount(root->pLeft);
    if (0 != root->pRight)
        count += BinaryTreeSubTreeCount(root->pRight);
    return count;
}

int BinaryTreeCount(const struct BinaryTree *tree) {
    if (0 == tree)
        return 0;
    return BinaryTreeSubTreeCount(tree->root);
}


/*
 * BinaryTreeFindValue - 查找数值
 */
struct BinaryNode *BT_FindValue(struct BinaryTree *ptree, const void *value) {
    struct BinaryNode *root = ptree->root;
    if (0 == root)
        return 0;

    int tmp = ptree->compare(value, root->pValue);
    while (0 != tmp) {
        if (tmp > 0)
            root = root->pRight;
        else if (tmp < 0)
            root = root->pLeft;

        if (0 == root)
            return 0;
        tmp = ptree->compare(value, root->pValue);
    }
    return root;
}
/*
 * BinaryTreeGetMaximum - 获取子树中最大值的节点
 */
struct BinaryNode *BT_GetMaximum(struct BinaryNode *x) {
    while (0 != x->pRight)
        x = x->pRight;
    return x;
}
/*
 * BinaryTreeGetMaximum - 获取子树中最小值的节点
 */
struct BinaryNode *BT_GetMinimum(struct BinaryNode *x) {
    while (0 != x->pLeft)
        x = x->pLeft;
    return x;
}
/*
 * BinaryTreeGetSuccessor - 获取给定节点的后继
 */
struct BinaryNode *BT_GetSuccessor(const struct BinaryTree *T, struct BinaryNode *x) {
    struct BinaryNode *y;
    if (0 != x->pRight)
        return BT_GetMinimum(x->pRight);

    y = x->pParent;
    while (x == y->pRight) {
        if (T->root == y)
            return 0;
        x = y;
        y = y->pParent;
    }
    return y;
}
/*
 * BinaryTreeGetSuccessor - 获取给定节点的前驱
 */
struct BinaryNode *BT_GetPredecessor(const struct BinaryTree *T, struct BinaryNode *x) {
    struct BinaryNode *y;
    if (0 != x->pLeft)
        return BT_GetMaximum(x->pLeft);

    y = x->pParent;
    while (x == y->pLeft) {
        if (T->root == y)
            return 0;
        x = y;
        y = y->pParent;
    }
    return y;
}



/*
 * BinaryTreeInsert - 插入数值
 */
struct BinaryNode *BT_Insert(struct BinaryTree *ptree, const void *value, int size) {
    struct BinaryNode *x = ptree->root;
    struct BinaryNode *y = 0;

    while (0 != x) {
        y = x;
        int tmp = ptree->compare(value, x->pValue);
        if (tmp > 0)
            x = x->pRight;
        else if (tmp < 0)
            x = x->pLeft;
        else
            return x;
    }

    struct BinaryNode *z = CreateBinaryNode(value, size);
    z->pParent = y;
    if (0 == y)
        ptree->root = z;
    else if (ptree->compare(value, y->pValue) > 0)
        y->pRight = z;
    else
        y->pLeft = z;

    return z;
}
/*
 * BT_Transplant - 将二叉树@T中的节点@u用@v替换
 */
static void BT_Transplant(struct BinaryTree *T, struct BinaryNode *u, struct BinaryNode *v) {
    if (0 == u->pParent)
        T->root = v;
    else if (u->pParent->pLeft == u)
        u->pParent->pLeft = v;
    else
        u->pParent->pRight = v;

    if (0 != v)
        v->pParent = u->pParent;
}
/*
 * BT_Remove - 从二叉树中删除节点@z
 */
void BT_Remove(struct BinaryTree *T, struct BinaryNode *z) {
    struct BinaryNode *y;

    if (0 == z->pLeft)
        BT_Transplant(T, z, z->pRight);
    else if (0 == z->pRight)
        BT_Transplant(T, z, z->pLeft);
    else {
        y = BT_GetMinimum(z->pRight);
        if (z != y->pParent) {
            BT_Transplant(T, y, y->pRight);
            y->pRight = z->pRight;
            y->pRight->pParent = y;
        }
        BT_Transplant(T, z, y);
        y->pLeft = z->pLeft;
        y->pLeft->pParent = y;
    }
}
/*
 * BT_RemoveAndDestroy - 从二叉树中移除节点@z并释放内存
 */
void BT_RemoveAndDestroy(struct BinaryTree *T, struct BinaryNode *z) {
    BT_Remove(T, z);
    z->pLeft = 0;
    z->pRight = 0;
    DestroyBinaryNode(z);
}
/*
 * DestroySubBinaryTree - 回收子树内存
 */
void DestroySubBinaryTree(struct BinaryNode *root) {
    if (0 == root)
        return;

    DestroySubBinaryTree(root->pLeft);
    DestroySubBinaryTree(root->pRight);

    root->pLeft = 0;
    root->pRight = 0;
    DestroyBinaryNode(root);
}
/*
 * DestroyBinaryTree - 回收二叉树内存
 */
void DestroyBinaryTree(struct BinaryTree *T) {
    if (0 == T)
        return;
    DestroySubBinaryTree(T->root);
    free(T);
}






#include <stdio.h>
/*
 * InOrderPrintBinaryTree - 按顺序打印
 */
void InOrderPrintBinaryTree(const struct BinaryTree *T, const struct BinaryNode *x) {
    if (0 == x)
        return;

    InOrderPrintBinaryTree(T, x->pLeft);
    printf("key=");T->print(x->pValue);printf(",");
    printf("||");

    printf("l->key=");
    (0 == x->pLeft) ? printf("NIL") : T->print(x->pLeft->pValue);
    printf(";");

    printf("r->key=");
    (0 == x->pRight) ? printf("NIL") : T->print(x->pRight->pValue);
    printf(";");

    printf("p->key=");
    (0 == x->pParent) ? printf("NIL") : T->print(x->pParent->pValue);
    printf("\n");

    InOrderPrintBinaryTree(T, x->pRight);
}







