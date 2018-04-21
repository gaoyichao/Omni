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
        printf("Keyword:%d, Comment:%d, tk:%d", TK_Keyword, TK_Comment, tk->token);
        assert(TK_Keyword == tk->token || TK_Comment == tk->token);
        if (TK_Keyword == tk->token) {
            printf("KEYWORD:%s,line:%d,col:%d\n", tk->str, tk->line, tk->col);
        } else {
            printf("注释:%s,line:%d,col:%d\n", tk->str, tk->line, tk->col);
        }
        DestroyToken(tk);
        tk = GetNextToken(lexer);
    }
    DestroyToken(tk);
    DestroyInputs(inputs);

    // Symbol的测试
    Symbol *sym1 = SearchSymbol(symTable, "douniwan");
    assert(0 != sym1);
    RemoveSymbol(symTable, "douniwan");
    assert(0 == SearchSymbol(symTable, "douniwan"));
    Symbol *sym2 = InsertSymbol(symTable, "douniwan", TK_Id, 0);
    Symbol *sym3 = SearchSymbol(symTable, "douniwan");
    assert(sym1 == sym3 && sym1 == sym2);
    assert(TK_Id == sym2->type);
    sym3 = ModifySymbol(symTable, "douniwan", TK_Keyword, 0);
    assert(TK_Keyword == sym2->type);



    DestroySymbolTable(symTable);
    DestroyLexer(lexer);
    printf("\n  T_Lexer 测试结束!\n");
    printf("******************************\n");
}
