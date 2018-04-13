#include <t_lexer.h>
#include <lexer.h>
#include <stdio.h>
#include <string.h>
#include <filedirectory.h>
#include <assert.h>

void T_Lexer() {
    printf("\n******************************\n");
    printf("    T_Lexer 测试开始\n");
    printf("******************************\n");

    char *cwd = CurrentWorkDir();
    assert(0 != cwd);
    printf("cwd = %s\n", cwd);


    struct Lexer *lexer = CreateLexer();
    struct SymbolTable *symTable = CreateSymbolTable();
    lexer->symTable = symTable;
    LexerLoadKeywords(lexer, "KeyWords");


    struct Inputs *inputs = CreateInputs("KeyWords");
    LexerSetInputs(lexer, inputs);

    struct Token *tk = GetNextToken(lexer);
    while (TK_End != tk->token) {
        assert(TK_Keyword == tk->token);
        printf("KEYWORD:%s,line:%d,col:%d\n", tk->str, tk->line, tk->col);
        DestroyToken(tk);
        tk = GetNextToken(lexer);
    }
    DestroyToken(tk);

    DestroyInputs(inputs);


    DestroySymbolTable(symTable);
    DestroyLexer(lexer);
    printf("\n  T_Lexer 测试结束!\n");
    printf("******************************\n");
}
