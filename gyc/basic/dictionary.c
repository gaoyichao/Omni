#include <dictionary.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define VECTOR_NUMBER
#define T DicPairPtr
#define TEMPLATE_TYPE(a) a ##_DicPairPtr
#define TEMPLATE_FUNCTION(a,b) a ##_DicPairPtr_## b
#include <vector/vector_t_c.h>
#include <vector/vector_t_num_c.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER

/*
 * _DicCompare - 对比字节
 */
static int _DicCompare(const void *a, const void *b) {
    if (0 == a) {
        if (0 == b)
            return 0;
        else
            return -1;
    } else if (0 == b) {
        return 1;
    }
    const struct ByteInfo *infoa = (struct ByteInfo*)a;
    const struct ByteInfo *infob = (struct ByteInfo*)b;

    if((infoa->byte) > (infob->byte))
        return 1;
    else if((infoa->byte) < (infob->byte))
        return -1;
    else return 0;
}
/*
 * _Dic_Insert - 插入一个字符串到字典中
 *
 * @dic: 目标字典
 * @buf: 给定字符串
 * @len: 插入长度
 */
static struct BinaryNode *_Dic_Insert(struct Dictionary *dic, const uint8 *buf, int len) {
    struct BinaryTree *tree = dic->tree;
    struct BinaryNode *tnd = 0, *p;
    struct ByteInfo info, *pinfo;
    int conflict = 1;

    for (int i = 0; i < len; i++) {
        p = tnd;
        info.byte = buf[i];
        tnd = BT_FindValue(tree, (void *)&info);
        if (0 == tnd) {
            tnd = BT_Insert(tree, &info, sizeof(struct ByteInfo));
            pinfo = (struct ByteInfo*)tnd->pValue;
            pinfo->byte = buf[i];
            pinfo->depth = i+1;
            pinfo->parent = p;
            pinfo->current = tnd;
            pinfo->index = 0;
            pinfo->tree = (pinfo->depth == len) ? 0 : CreateBinaryTree(_DicCompare);
            conflict = 0;
        }
        tree = ((struct ByteInfo*)(tnd->pValue))->tree;
    }

    return conflict ? 0 : tnd;
}
/*
 * _DicSearch - 从字典中查找字符串
 *
 * @dic: 目标字典
 * @buf: 字符串序列
 * @len: 查找长度
 */
struct BinaryNode *_DicSearch(struct Dictionary *dic, const uint8 *buf, int len) {
    struct BinaryTree *tree = dic->tree;
    struct BinaryNode *tnd = 0;
    struct ByteInfo info;

    for (int i = 0; i < len; i++) {
        info.byte = buf[i];
        tnd = BT_FindValue(tree, (void*)&info);
        if (0 == tnd)
            return 0;
        tree = ((struct ByteInfo*)(tnd->pValue))->tree;
    }

    return tnd;
}
/*
 * _DicTraceBackStr - 从给定节点中回溯字符串
 *
 * @node: 给定节点
 */
uint8* _DicTraceBackStr(const struct BinaryNode *node) {
    struct ByteInfo *tmp = (struct ByteInfo*)(node->pValue);
    int len = tmp->depth;
    uint8 *buf = malloc(len + 1);
    buf[len] = '\0';
    len--;

    while (node != 0) {
        tmp = (struct ByteInfo*)(node->pValue);
        buf[len] = (uint8)(tmp->byte);
        node = tmp->parent;
        len--;
    }

    return buf;
}


/*
 * CreateDictionary - 创建一个字典
 */
struct Dictionary *CreateDictionary(void) {
    struct Dictionary *result = malloc(sizeof(struct Dictionary));
    result->tree = CreateBinaryTree(_DicCompare);
    //
    vector_DicPairPtr_init(&(result->pairs), 0);
    //
    return result;
}

/*
 * DicInsertPair - 插入一个字典对,若字典中已经存在一个key,将返回0
 *
 * @dic: 目标字典
 * @key: 索引字符串
 * @vptr: 数值指针
 */
DicPairPtr DicInsertPair(struct Dictionary *dic, const uint8 *key, void *vptr) {
    int len = strlen((char*)key) + 1;

    struct BinaryNode *node = _Dic_Insert(dic, key, len);
    if (0 == node)
        return 0;

    DicPairPtr pairptr = (DicPairPtr)malloc(sizeof(struct DicPair));
    pairptr->key = node->pValue;
    pairptr->vptr = vptr;
    pairptr->key->index = vector_DicPairPtr_size(&(dic->pairs));
    vector_DicPairPtr_push_back(&(dic->pairs), pairptr);

    return pairptr;
}

/*
 * DicGetPair - 从字典中查询字典对,若没有查到,将返回0
 *
 * @dic: 目标字典
 * @key: 索引字符串
 */
DicPairPtr DicGetPair(struct Dictionary *dic, const uint8 *key) {
    int len = strlen((char*)key) + 1;
    struct BinaryNode *node = _DicSearch(dic, key, len);
    if (0 == node)
        return 0;

    int index = ((struct ByteInfo*)node->pValue)->index;
    return VECTOR(dic->pairs)[index];
}

DicPairPtr DicGetPair2(struct Dictionary *dic, const uint8 *key, int len) {
    struct BinaryNode *node = _DicSearch(dic, key, len);
    if (0 == node)
        return 0;

    struct ByteInfo info;
    info.byte = '\0';
    node = BT_FindValue(((struct ByteInfo*)(node->pValue))->tree, (void*)&info);
    if (0 == node)
        return 0;

    int index = ((struct ByteInfo*)node->pValue)->index;
    return VECTOR(dic->pairs)[index];
}

/*
 * DicGetVptr - 从字典中查询数值指针,若没有查到,将返回0
 *
 * @dic: 目标字典
 * @key: 索引字符串
 */
void* DicGetVptr(struct Dictionary *dic, const uint8 *key) {
    DicPairPtr pair = DicGetPair(dic, key);
    return (0 == pair) ? 0 : VECTOR(dic->pairs)[pair->key->index]->vptr;
}

void* DicGetVptr2(struct Dictionary *dic, const uint8 *key, int len) {
    DicPairPtr pair = DicGetPair2(dic, key, len);
    return (0 == pair) ? 0 : VECTOR(dic->pairs)[pair->key->index]->vptr;
}

/*
 * DicGetKey - 回溯Key字符串,申请了内存需要释放
 *
 * @pair: 字典对指针
 */
uint8* DicGetKey(DicPairPtr pair) {
    struct BinaryNode *node = 0;
    struct ByteInfo *binfo = pair->key;
    int len = binfo->depth;
    uint8 *buf = malloc(len + 1);
    buf[len--] = '\0';

    do {
        buf[len--] = (uint8)(binfo->byte);
        node = binfo->parent;
        binfo = (0 == node) ? 0 : (struct ByteInfo*)(node->pValue);
    } while (node != 0);

    return buf;
}
/*
 * _Dic_Remove - 从字典中删除对
 */
static void _Dic_Remove(struct BinaryNode *node) {
    struct ByteInfo *tmp = (struct ByteInfo*)(node->pValue);
    struct ByteInfo *tmp2;
    struct BinaryNode *node2;

    while (node != 0) {
        tmp = (struct ByteInfo*)(node->pValue);
        int count = BinaryTreeCount(tmp->tree);

        if (0 != count)
            break;

        node2 = tmp->parent;
        tmp2 = (struct ByteInfo*)(node2->pValue);

        DestroyBinaryTree(tmp->tree);
        BT_RemoveAndDestroy(tmp2->tree, node);

        node = node2;
    }
}
/*
 * DicDeletePair - 从字典中删除对
 */
void DicDeletePair(struct Dictionary *dic, DicPairPtr pair) {
    struct BinaryNode *node = pair->key->current;
    int index = pair->key->index;

    _Dic_Remove(node);
    vector_DicPairPtr_remove(&(dic->pairs), index);
    int size = vector_DicPairPtr_size(&(dic->pairs));
    for (int i = index; i < size; i++)
        (VECTOR(dic->pairs)[i]->key->index)--;

    free(pair);
}
/*
 * DicDelete - 从字典中删除对象
 *
 * @key: 索引字符串
 */
void DicDelete(struct Dictionary *dic, const uint8 *key) {
    DicPairPtr pair = DicGetPair(dic, key);
    DicDeletePair(dic, pair);
}

/*
 * _DestroyDictionary - 回收字典内存
 */
static void _DestroyDictionary(struct BinaryTree *tree) {
    int count = BinaryTreeCount(tree);
    struct BinaryNode *node = 0;
    struct BinaryNode *max = BT_GetMaximum(tree->root);


    if (0 != count) {
        for (node = BT_GetMinimum(tree->root); node != max; node = BT_GetSuccessor(tree, node)) {
            struct ByteInfo *info = (struct ByteInfo*)(node->pValue);
            _DestroyDictionary(info->tree);
        }
    }

    DestroyBinaryTree(tree);
    return;
}
/*
 * DestroyDictionary - 回收字典内存
 */
void DestroyDictionary(struct Dictionary *dic) {
    _DestroyDictionary(dic->tree);
    int vsize = vector_DicPairPtr_size(&(dic->pairs));
    for (int i = 0; i < vsize; i++)
        free(VECTOR(dic->pairs)[i]);
    vector_DicPairPtr_destroy(&(dic->pairs));
}
