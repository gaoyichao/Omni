#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <dictionary.h>
#include <token.h>

typedef struct Symbol {
    eToken type;
    DicPairPtr entry;
    void *vptr;     // 符号内容,内存由用户提供
} Symbol;

#define T Symbol
#define TEMPLATE_TYPE(a) a ##_Symbol
#define TEMPLATE_FUNCTION(a,b) a ##_Symbol_## b
#include <vector/vector_t_h.h>
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T


struct SymbolTable {
    vector_Symbol table;
    struct Dictionary *dic;
};

struct SymbolTable *CreateSymbolTable(void);
void DestroySymbolTable(struct SymbolTable *st);

Symbol *SearchSymbol(struct SymbolTable *st, const uint8 *key);
Symbol *InsertSymbol(struct SymbolTable *st, const uint8 *key, eToken type, void *vptr);
void DeleteSymbol(struct SymbolTable *st, const uint8 *key);
Symbol *ModifySymbol(struct SymbolTable *st, const uint8 *key, eToken type, void *vptr);


#endif // SYMBOLTABLE_H
