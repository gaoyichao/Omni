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
    struct Lexer *lexer = CreateLexer();
    struct Parser *parser = CreateParser();

    lexer->inputs = inputs;
    parser->lexer = lexer;


    struct Token *tk = GetNextToken(lexer);
    while (TK_End != tk->token) {
        if (TK_FloatingConstant == tk->token) {
            struct Expression *exp = CreatePrimaryExp_Constant(tk);
            printf("%f\n", CalcExpression(exp));
        }

        printf("KEYWORD:%s,line:%d,col:%d\n", tk->str, tk->line, tk->col);
        DestroyToken(tk);
        tk = GetNextToken(lexer);
    }
    DestroyToken(tk);





    DestroyInputs(inputs);
    DestroyLexer(lexer);
    DestroyParser(parser);

    printf("\n  T_Parser 测试结束!\n");
    printf("******************************\n");
}
