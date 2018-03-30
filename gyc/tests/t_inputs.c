#include <t_inputs.h>
#include <stdio.h>
#include <assert.h>


void T_Inputs() {
    printf("\n******************************\n");
    printf("    T_Inputs 测试开始\n");
    printf("******************************\n");


    struct Inputs *inputs = CreateInputs("hehe.c");

    struct Lexer *lexer = CreateLexer(inputs);

    struct Token *re = GetNextToken(lexer);
    assert(TK_ID == re->token);
    printf("%s", re->str);

    printf("\n  T_graph 测试结束!\n");
    printf("******************************\n");
}


