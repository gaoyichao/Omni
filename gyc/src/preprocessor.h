#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <types.h>
#include <inputs.h>
#include <lexer.h>
#include <parser.h>
#include <expression.h>

/*
 * PreProcessor - 类似C语言的预处理器
 */
struct PreProcessor {
    struct SymbolTable *symTable;
    struct Lexer *lexer;
    struct Parser *parser;
};

struct PreProcessor *CreatePreProcessor();

#endif // PREPROCESSOR_H
