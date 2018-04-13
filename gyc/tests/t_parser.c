#include <t_parser.h>
#include <parser.h>
#include <stdio.h>
#include <string.h>
#include <filedirectory.h>
#include <assert.h>


void T_Parser() {
    printf("\n******************************\n");
    printf("    T_Parser 测试开始\n");
    printf("******************************\n");

    struct Inputs *inputs = CreateInputs("Calculator.gyc");
    struct SymbolTable *symTable = CreateSymbolTable();

    struct Lexer *lexer = CreateLexer();
    lexer->inputs = inputs;
    lexer->symTable = symTable;

    struct Parser *parser = CreateParser();
    parser->symTable = symTable;
    parser->lexer = lexer;

    struct Expression *exp = ParseExpression(parser);
    while (0 != exp) {
        printf("%f\n", CalcExpression(exp));
        DestroyExpression(exp);
        exp = ParseExpression(parser);
    }

    DestroyInputs(inputs);
    DestroyLexer(lexer);
    DestroyParser(parser);

    printf("\n  T_Parser 测试结束!\n");
    printf("******************************\n");
}
