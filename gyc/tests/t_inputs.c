#include <t_inputs.h>
#include <stdio.h>
#include <string.h>
#include <filedirectory.h>
#include <assert.h>

void T_Inputs() {
    printf("\n******************************\n");
    printf("    T_Inputs 测试开始\n");
    printf("******************************\n");

    char *cwd = CurrentWorkDir();
    assert(0 != cwd);
    printf("cwd = %s\n", cwd);

    struct Inputs *inputs = CreateInputs("hehe.c");

    struct Lexer *lexer = CreateLexer();
    LexerSetInputs(lexer, inputs);

    struct Token *re = Parser_NextToken(lexer);
    assert(TK_Id == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);

    re = Parser_NextToken(lexer);
    assert(TK_Id == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);

    re = Parser_NextToken(lexer);
    assert(TK_StringLiteral == re->token);
    printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
    // 3
    for (int i = 0; i < 3; i++) {
        re = Parser_NextToken(lexer);
        assert(TK_IntegerConstant == re->token);
        printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
    }
    // 9
    for (int i = 0; i < 9; i++) {
        re = Parser_NextToken(lexer);
        assert(TK_FloatingConstant == re->token);
        printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
    }
    // 4
    for (int i = 0; i < 4; i++) {
        re = Parser_NextToken(lexer);
        assert(TK_CharConstant == re->token);
        printf("%s,line:%d,col:%d\n", re->str, re->line, re->col);
    }

    re = Parser_NextToken(lexer);
    assert(TK_End == re->token);

    DestroyInputs(inputs);
    DestroyLexer(lexer);
    printf("\n  T_graph 测试结束!\n");
    printf("******************************\n");
}

