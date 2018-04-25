#include <parser.h>
#include <assert.h>
#include <stdlib.h>
#include <mm.h>

struct Parser *CreateParser() {
    struct Parser *re = (struct Parser*)malloc(sizeof(struct Parser));
    re->lexer = 0;
    re->symTable = 0;
    re->ctkIndex = 0;
    re->ctk = TK_Begin;

    return re;
}

void DestroyParser(struct Parser *parser) {
    free(parser);
}

struct Token *_CheckToken(struct Parser *parser, eToken token) {
    if (token == parser->ctk) {
        struct Token *tk = Parser_NextToken(parser->lexer);
        parser->ctk = tk->token;
        parser->ctkIndex++;

        return TokenList_GetItem(parser->lexer->tkList, parser->ctkIndex - 2);
    } else {
        assert(0);
        return 0;
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
    eToken token = parser->ctk;
    Token *tk;

    if (TK_FloatingConstant == token) {
        tk = _CheckToken(parser, token);
        re = CreatePrimaryExp_Constant((const char*)tk->str);
    } else if (TK_Id == token) {
        // TODO: TK_Id更换为TK_Variable
        tk = _CheckToken(parser, token);
        Symbol *sym = InsertSymbol(parser->symTable, tk->str, TK_Variable, 0);
        re = CreatePrimaryExp_Variable(sym);
    } else if (TK_Variable == token) {
        tk = _CheckToken(parser, token);
        Symbol *sym = SearchSymbol(parser->symTable, tk->str);
        re = CreatePrimaryExp_Variable(sym);
    } else if (TK_LParenthesis == token) {
        _CheckToken(parser, token);
        struct Expression *tmp = ParseExpression(parser);
        _CheckToken(parser, TK_RParenthesis);
        re = CreatePrimaryExp_Exp(tmp);
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
    eToken token = parser->ctk;

    if (IsUnaryOperator(token)) {
        _CheckToken(parser, token);
        struct Expression *tmp = ParseUnaryExpression(parser);
        re = CreateUnaryExp(token, tmp);
    } else if (IsPrimaryOperator(token)) {
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

    eToken token = parser->ctk;

    while (TK_Mul == token || TK_Div == token) {
        _CheckToken(parser, token);
        struct Expression *right = ParseUnaryExpression(parser);
        assert(0 != right);
        re = CreateBiOperandExp(re, token, right, ET_MultiplicativeExp);
        token = parser->ctk;
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

    eToken token = parser->ctk;
    while (TK_Add == token || TK_Sub == token) {
        _CheckToken(parser, token);
        struct Expression *right = ParseMultiplicativeExpression(parser, 0);
        assert(0 != right);
        re = CreateBiOperandExp(re, token, right, ET_AdditiveExp);
        token = parser->ctk;
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

    eToken token = parser->ctk;
    if (!IsAssignmentOperator(token))
        return ParseAdditiveExpression(parser, re);

    assert(ET_PrimaryExp_Variable == re->type);
    while (IsAssignmentOperator(token)) {
        _CheckToken(parser, token);
        struct Expression *right = ParseAssignmentExpression(parser);
        assert(0 != right);
        re = CreateBiOperandExp(re, token, right, ET_AssignmentExp);
        token = parser->ctk;
    }
    return re;
}


struct Expression *ParseExpression(struct Parser *parser) {
    if (TK_Begin == parser->ctk)
        _CheckToken(parser, TK_Begin);
    return ParseAssignmentExpression(parser);
}

