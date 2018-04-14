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

static struct Token *_GetToken(struct Parser *parser) {
    return (0 == parser->ctoken) ? GetNextToken(parser->lexer) : parser->ctoken;
}


/*
 * ParsePrimaryExpression - 构建一个基本表达式
 *
 * primary-expression:
 *      float-constant
 *      variable-identifier
 *      ( expression )
 *
 * @parser: 解析器
 */
struct Expression *ParsePrimaryExpression(struct Parser *parser) {
    struct Expression *re = 0;
    struct Token *tk = _GetToken(parser);

    if (TK_FloatingConstant == tk->token) {
        re = CreatePrimaryExp_Constant((const char*)tk->str);
    } else if (TK_Id == tk->token) {
        // TODO: TK_Id更换为TK_Variable
        Symbol *sym = InsertSymbol(parser->symTable, tk->str, TK_Variable, 0);
        re = CreatePrimaryExp_Variable(sym);
    } else if (TK_LParenthesis == tk->token) {
        parser->ctoken = 0;
        DestroyToken(tk);
        struct Expression *tmp = ParseExpression(parser);
        tk = _GetToken(parser);
        assert(TK_RParenthesis == tk->token);
        re = CreatePrimaryExp_Exp(tmp);
    }

    parser->ctoken = 0;
    DestroyToken(tk);
    return re;
}
/*
 * ParseUnaryExpression - 构建一个Unary表达式
 *
 * unary-expression:
 *      primary-expression
 *      unary-operator unary-expression
 *
 * @parser: 解析器
 */
struct Expression *ParseUnaryExpression(struct Parser *parser) {
    struct Expression *re = 0;
    struct Token *tk = _GetToken(parser);

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
/*
 * ParseMultiplicativeExpression - 构建一个乘除运算的表达式
 *
 * multiplicative-expression:
 *      unary-expression
 *      multiplicative-expression * unary-expression
 *      multiplicative-expression / unary-expression
 *
 * @parser: 解析器
 */
struct Expression *ParseMultiplicativeExpression(struct Parser *parser) {
    struct Expression *re = ParseUnaryExpression(parser);
    if (0 == re)
        return 0;

    struct Token *tk = _GetToken(parser);
    while (TK_Mul == tk->token || TK_Div == tk->token) {
        parser->ctoken = 0;
        struct Expression *right = ParseUnaryExpression(parser);
        assert(0 != right);
        re = CreateMultiplicativeExp(re, tk->token, right);
        DestroyToken(tk);
        tk = _GetToken(parser);
    }
    parser->ctoken = tk;
    return re;
}
/*
 * ParseAdditiveExpression - 构建一个加减运算的表达式
 *
 * additive-expression:
 *      multiplicative-expression
 *      additive-expression + multiplicative-expression
 *      additive-expression - mulitplicative-expression
 *
 * @parser: 解析器
 */
struct Expression *ParseAdditiveExpression(struct Parser *parser) {
    struct Expression *re = ParseMultiplicativeExpression(parser);
    if (0 == re)
        return 0;

    struct Token *tk = _GetToken(parser);
    while (TK_Add == tk->token || TK_Sub == tk->token) {
        parser->ctoken = 0;
        struct Expression *right = ParseMultiplicativeExpression(parser);
        assert(0 != right);
        re = CreateAdditiveExp(re, tk->token, right);
        DestroyToken(tk);
        tk = _GetToken(parser);
    }
    parser->ctoken = tk;

    return re;
}

struct Expression *ParseExpression(struct Parser *parser) {
    return ParseAdditiveExpression(parser);
}

