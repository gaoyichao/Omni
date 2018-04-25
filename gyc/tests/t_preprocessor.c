#include <t_preprocessor.h>
#include <preprocessor.h>
#include <stdio.h>
#include <string.h>
#include <filedirectory.h>
#include <assert.h>

void T_PreProcessor() {
    printf("\n******************************\n");
    printf("    T_PreProcessor 测试开始\n");
    printf("******************************\n");

    struct Inputs *inputs = CreateInputs("PreProcess.gc");
    struct SymbolTable *symTable = CreateSymbolTable();
    struct Lexer *lexer = CreateLexer();
    lexer->inputs = inputs;
    lexer->symTable = symTable;
    struct Parser *parser = CreateParser();
    parser->symTable = symTable;
    parser->lexer = lexer;

    struct PreProcessor *pp = CreatePreProcessor();
    assert(0 != pp);
    pp->lexer = lexer;
    pp->parser = parser;
    pp->symTable = symTable;




    printf("\n  T_PreProcessor 测试结束!\n");
    printf("******************************\n");
}
