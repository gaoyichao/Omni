#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <binary_tree.h>

struct ByteInfo {
    uint8 byte;
    int depth;
    int index;
    struct BinaryTree *tree;    // 字典的后继
    struct BinaryNode *parent;  // 指向自己的父节点
    struct BinaryNode *current; // 指向包含自己的节点
};

struct DicPair {
    struct ByteInfo *key;
    void *vptr;
};

typedef struct DicPair* DicPairPtr;

#define VECTOR_NUMBER
#define T DicPairPtr
#define TEMPLATE_TYPE(a) a ##_DicPairPtr
#define TEMPLATE_FUNCTION(a,b) a ##_DicPairPtr_## b
#include <vector/vector_t_h.h>
#include <vector/vector_t_num_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
#undef VECTOR_NUMBER

struct Dictionary {
    struct BinaryTree *tree;
    vector_DicPairPtr pairs;
};

struct Dictionary *CreateDictionary(void);
void DestroyDictionary(struct Dictionary *dic);

struct BinaryNode *_DicSearch(struct Dictionary *dic, const uint8 *buf, int len);
uint8* _DicTraceBackStr(const struct BinaryNode *node);

DicPairPtr DicInsertPair(struct Dictionary *dic, const uint8 *key, void *vptr);
DicPairPtr DicGetPair(struct Dictionary *dic, const uint8 *key);
void* DicGetVptr(struct Dictionary *dic, const uint8 *key);
uint8* DicGetKey(DicPairPtr pair);
void DicDeletePair(struct Dictionary *dic, DicPairPtr pair);
void DicDelete(struct Dictionary *dic, const uint8 *key);

#endif // DICTIONARY_H
