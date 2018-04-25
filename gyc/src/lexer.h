#ifndef LEXER_H
#define LEXER_H

#include <token.h>
#include <symboltable.h>
#include <inputs.h>
#include <vector_uint8.h>

struct Lexer;
typedef eToken (*ScanFunc)(struct Lexer *lexer);

struct Lexer {
    struct Inputs *inputs;          // 输入流,由用户提供
    struct SymbolTable *symTable;   // 符号表,由用户提供
    vector_Token tokens;            // Token列表,在词法分析过程中不断扩充
    vector_uint8 strings;           // Token对应的字符串,在词法分析过程中不断扩充
    ScanFunc scanners[256];
};

struct Lexer *CreateLexer();
void LexerLoadKeywords(struct Lexer *lexer, char *filename);
void LexerSetInputs(struct Lexer *lexer, struct Inputs *inputs);
void DestroyLexer(struct Lexer *lexer);

struct Token *GetNextToken(struct Lexer *lexer);

#endif // LEXER_H



