#include <symboltable.h>
#include <error.h>
#include <assert.h>
#include <mm.h>

#define T Symbol
#define TEMPLATE_TYPE(a) a ##_Symbol
#define TEMPLATE_FUNCTION(a,b) a ##_Symbol_## b
#include <vector/vector_t_c.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

/*
 * CreateSymbolTable - 创建一个符号表
 */
struct SymbolTable *CreateSymbolTable(void) {
    struct SymbolTable *re = Calloc(1, struct SymbolTable);

    re->dic = CreateDictionary();
    vector_int_init(&(re->hole), 0);
    vector_Symbol_init(&(re->table), 0);

    return re;
}
/*
 * DestroySymbolTable - 销毁符号表,并回收内存
 *
 * @st: 目标符号表
 */
void DestroySymbolTable(struct SymbolTable *st) {
    DestroyDictionary(st->dic);
    vector_int_destroy(&(st->hole));
    vector_Symbol_destroy(&(st->table));
    Free(st);
}
/*
 * SearchSymbol - 查找符号
 *
 * @st: 目标符号表
 * @key: 符号关键字
 * return: 若没有找到符号则返回0
 */
Symbol *SearchSymbol(struct SymbolTable *st, const uint8 *key) {
    DicPairPtr pair = DicGetPair(st->dic, key);
    if (0 == pair)
        return 0;

    Symbol *re = (Symbol*)(pair->vptr);
    return re;
}
/*
 * InsertSymbol - 插入新符号,需要确保符号表中没有新符号
 *
 * @st: 目标符号表
 * @key: 符号关键字
 * @type: 符号类型
 * @vptr: 符号内容,内存由用户提供
 * return: 若已经存在则返回0,否则返回新建的符号
 */
Symbol *InsertSymbol(struct SymbolTable *st, const uint8 *key, eToken type, void *vptr) {
    DicPairPtr pair = DicGetPair(st->dic, key);
    if (0 != pair && 0 != pair->vptr)
        return 0;

    Symbol *re = 0;
    if (!vector_int_empty(&(st->hole))) {
        int offset = vector_int_pop_back(&(st->hole));
        re = &VECTOR(st->table)[offset];
    } else {
        int isFull = vector_Symbol_full(&(st->table));
        int n = vector_Symbol_size(&(st->table));
        re = vector_Symbol_new_item(&(st->table));
        if (isFull) {
            for (int i = 0; i < n; i++) {
                VECTOR(st->table)[i].entry->vptr = &VECTOR(st->table)[i];
            }
        }
    }

    re->type = type;
    re->vptr = vptr;
    DicPairPtr entry = (0 == pair) ? DicInsertPair(st->dic, key, re) : pair;
    entry->vptr = re;
    re->entry = entry;

    return re;
}

/*
 * RemoveSymbol - 从符号表中暂时移除一个符号, 但保持存储
 *
 * @st: 目标符号表
 * @key: 符号关键字
 */
void RemoveSymbol(struct SymbolTable *st, const uint8 *key) {
    DicPairPtr pair = DicGetPair(st->dic, key);
    if (0 == pair || 0 == pair->vptr)
        return;

    Symbol *sym = (Symbol*)(pair->vptr);
    pair->vptr = 0;
    sym->entry = 0;

    int offset = vector_Symbol_offset(&(st->table), sym);
    vector_int_push_back(&(st->hole), offset);
}
/*
 * ModifySymbol - 修改符号表
 *
 * @st: 目标符号表
 * @key: 符号关键字
 * @type: 符号类型
 * @vptr: 符号内容,内存由用户提供
 */
Symbol *ModifySymbol(struct SymbolTable *st, const uint8 *key, eToken type, void *vptr) {
    DicPairPtr pair = DicGetPair(st->dic, key);
    if (0 == pair || 0 == pair->vptr)
        return InsertSymbol(st, key, type, vptr);

    Symbol *sym = (Symbol*)(pair->vptr);
    sym->type = type;
    sym->vptr= vptr;
    return sym;
}
