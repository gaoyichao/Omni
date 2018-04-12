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
    assert(TK_Id == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
    DestroyToken(re);

    re = GetNextToken(lexer);
    assert(TK_Id == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
    DestroyToken(re);

    re = GetNextToken(lexer);
    assert(TK_StringLiteral == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
    DestroyToken(re);
    // 3
    for (int i = 0; i < 3; i++) {
        re = GetNextToken(lexer);
        assert(TK_IntegerConstant == re->token);
        printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
        DestroyToken(re);
    }
    // 9
    for (int i = 0; i < 9; i++) {
        re = GetNextToken(lexer);
        assert(TK_FloatingConstant == re->token);
        printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
        DestroyToken(re);
    }
    // 4
    for (int i = 0; i < 4; i++) {
        re = GetNextToken(lexer);
        assert(TK_CharConstant == re->token);
        printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
        DestroyToken(re);
    }


    DestroyInputs(inputs);
    printf("\n  T_graph 测试结束!\n");
    printf("******************************\n");
}

