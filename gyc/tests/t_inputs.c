#include <t_inputs.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void T_Inputs() {
    printf("\n******************************\n");
    printf("    T_Inputs 测试开始\n");
    printf("******************************\n");

    struct Inputs *inputs = CreateInputs("hehe.c");

    struct Lexer *lexer = CreateLexer(inputs);

    struct Token *re = GetNextToken(lexer);
    assert(TK_ID == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);

    re = GetNextToken(lexer);
    assert(TK_ID == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);

    DestroyInputs(inputs);
    printf("\n  T_graph 测试结束!\n");
    printf("******************************\n");
}

