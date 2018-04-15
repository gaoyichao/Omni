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


static struct Token *_PeekToken(struct Parser *parser) {
    if (0 == parser->ctoken)
        parser->ctoken = GetNextToken(parser->lexer);
    return parser->ctoken;
}

static struct Token *_GetToken(struct Parser *parser) {
    if (0 != parser->ctoken) {
        struct Token *re = parser->ctoken;
        parser->ctoken = 0;
        return re;
    } else {
        return GetNextToken(parser->lexer);
    }
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
    struct Token *tk = _PeekToken(parser);

    if (TK_FloatingConstant == tk->token) {
        tk = _GetToken(parser);
        re = CreatePrimaryExp_Constant((const char*)tk->str);
        DestroyToken(tk);
    } else if (TK_Id == tk->token) {
        // TODO: TK_Id更换为TK_Variable
        tk = _GetToken(parser);
        Symbol *sym = InsertSymbol(parser->symTable, tk->str, TK_Variable, 0);
        re = CreatePrimaryExp_Variable(sym);
        DestroyToken(tk);
    } else if (TK_Variable == tk->token) {
        tk = _GetToken(parser);
        Symbol *sym = SearchSymbol(parser->symTable, tk->str);
        re = CreatePrimaryExp_Variable(sym);
        DestroyToken(tk);
    } else if (TK_LParenthesis == tk->token) {
        tk = _GetToken(parser);
        DestroyToken(tk);
        struct Expression *tmp = ParseExpression(parser);
        tk = _GetToken(parser);
        assert(TK_RParenthesis == tk->token);
        re = CreatePrimaryExp_Exp(tmp);
        DestroyToken(tk);
    }
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
    struct Token *tk = _PeekToken(parser);

    if (IsUnaryOperator(tk->token)) {
        tk = _GetToken(parser);
        struct Expression *tmp = ParseUnaryExpression(parser);
        re = CreateUnaryExp(tk->token, tmp);
        DestroyToken(tk);
    } else if (IsPrimaryOperator(tk->token)) {
        re = ParsePrimaryExpression(parser);
    }
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
struct Expression *ParseMultiplicativeExpression(struct Parser *parser, struct Expression *left) {
    struct Expression *re = (0 == left) ?  ParseUnaryExpression(parser) : left;
    if (0 == re)
        return 0;

    struct Token *tk = _PeekToken(parser);
    while (TK_Mul == tk->token || TK_Div == tk->token) {
        tk = _GetToken(parser);
        struct Expression *right = ParseUnaryExpression(parser);
        assert(0 != right);
        re = CreateBiOperandExp(re, tk->token, right, ET_MultiplicativeExp);
        DestroyToken(tk);
        tk = _PeekToken(parser);
    }
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
struct Expression *ParseAdditiveExpression(struct Parser *parser, struct Expression *left) {
    struct Expression *re = ParseMultiplicativeExpression(parser, left);
    if (0 == re)
        return 0;

    struct Token *tk = _PeekToken(parser);
    while (TK_Add == tk->token || TK_Sub == tk->token) {
        tk = _GetToken(parser);
        struct Expression *right = ParseMultiplicativeExpression(parser, 0);
        assert(0 != right);
        re = CreateBiOperandExp(re, tk->token, right, ET_AdditiveExp);
        DestroyToken(tk);
        tk = _PeekToken(parser);
    }

    return re;
}

/*
 * ParseAssignmentExpression - 构建一个赋值运算的表达式
 *
 * assignment-expression:
 *      additive-expression
 *      unary-expression assignment-operator assignment-expression
 *
 * @parser: 解析器
 */
struct Expression *ParseAssignmentExpression(struct Parser *parser) {
    struct Expression *re = ParseUnaryExpression(parser);
    if (0 == re)
        return 0;

    struct Token *tk = _PeekToken(parser);
    if (!IsAssignmentOperator(tk->token))
        return ParseAdditiveExpression(parser, re);

    assert(ET_PrimaryExp_Variable == re->type);
    while (IsAssignmentOperator(tk->token)) {
        tk = _GetToken(parser);
        struct Expression *right = ParseAssignmentExpression(parser);
        assert(0 != right);
        re = CreateBiOperandExp(re, tk->token, right, ET_AssignmentExp);
        DestroyToken(tk);
        tk = _PeekToken(parser);
    }
    return re;
}


struct Expression *ParseExpression(struct Parser *parser) {
    return ParseAssignmentExpression(parser);
}

