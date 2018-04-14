#include <parser.h>
#include <assert.h>
#include <stdlib.h>
#include <mm.h>


struct Parser *CreateParser() {
    struct Parser *re = (struct Parser*)malloc(sizeof(struct Parser));
    re->lexer = 0;
    re->symTable = 0;
    re->ctoken = 0;

    return re;
}

void DestroyParser(struct Parser *parser) {
    free(parser);
}

struct Expression *ParsePrimaryExpression(struct Parser *parser) {
    struct Expression *re = 0;
    struct Token *tk = (0 == parser->ctoken) ? GetNextToken(parser->lexer) : parser->ctoken;

    if (TK_FloatingConstant == tk->token) {
        re = CreatePrimaryExp_Constant((const char*)tk->str);
    } else if (TK_Id == tk->token) {
        // TODO: TK_Id更换为TK_Variable
        Symbol *sym = InsertSymbol(parser->symTable, tk->str, TK_Variable, 0);
        re = CreatePrimaryExp_Variable(sym);
    }

    parser->ctoken = 0;
    DestroyToken(tk);
    return re;
}

struct Expression *ParseUnaryExpression(struct Parser *parser) {
    struct Expression *re = 0;
    struct Token *tk = (0 == parser->ctoken) ? GetNextToken(parser->lexer) : parser->ctoken;

    if (IsUnaryOperator(tk->token)) {
        parser->ctoken = 0;
        struct Expression *tmp = ParseUnaryExpression(parser);
        re = CreateUnaryExp(tk->token, tmp);
    } else if (IsPrimaryOperator(tk->token)) {
        parser->ctoken = tk;
        re = ParsePrimaryExpression(parser);
    }
    DestroyToken(tk);
    return re;
}

struct Expression *ParseMultiplicativeExpression(struct Parser *parser) {
    struct Expression *re = ParseUnaryExpression(parser);
    if (0 == re)
        return 0;

    struct Token *tk = (0 == parser->ctoken) ? GetNextToken(parser->lexer) : parser->ctoken;
    while (TK_Mul == tk->token || TK_Div == tk->token) {
        parser->ctoken = 0;
        struct Expression *right = ParseUnaryExpression(parser);
        assert(0 != right);
        re = CreateMultiplicativeExp(re, tk->token, right);
        DestroyToken(tk);
        tk = GetNextToken(parser->lexer);
    }
    parser->ctoken = tk;
    return re;
}

struct Expression *ParseExpression(struct Parser *parser) {
    struct Expression *re = ParseMultiplicativeExpression(parser);
    if (0 == re)
        return 0;

    struct Token *tk = (0 == parser->ctoken) ? GetNextToken(parser->lexer) : parser->ctoken;
    while (TK_Add == tk->token || TK_Sub == tk->token) {
        parser->ctoken = 0;
        struct Expression *right = ParseMultiplicativeExpression(parser);
        assert(0 != right);
        re = CreateAdditiveExp(re, tk->token, right);
        DestroyToken(tk);
        tk = GetNextToken(parser->lexer);
    }
    parser->ctoken = tk;

    return re;
}



